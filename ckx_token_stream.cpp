/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#include "ckx_token_stream.hpp"

#include "ckx_file_reader.hpp"
#include "vector.hpp"

namespace ckx
{

ckx_token_stream::~ckx_token_stream() {}

namespace detail
{

class ckx_default_token_stream_impl
{
public:
    ckx_default_token_stream_impl(ckx_file_reader &_file_reader);
    ~ckx_default_token_stream_impl();

    saber_ptr<ckx_token> operator_index_impl(int _offset);
    void operator_incr_impl(void);

private:
    qsizet current_pos;
    saber::vector<saber_ptr<ckx_token>> token_buffer;
    saber::vector<ckx_error> errors;
    saber::string src;

    void do_split_tokens();

    void solve_numbers();

    qint64 scan_integer();
    qreal scan_floating_part();

    inline qchar ch();
    inline void next_char();
    inline void next_line();

    inline void issue_error(const char* _desc);

    qcoord char_coord;
    qsizet char_index;
};

}

ckx_default_token_stream::ckx_default_token_stream(
        ckx_file_reader &_file_reader)
{
    impl = new detail::ckx_default_token_stream_impl(_file_reader);
}

ckx_default_token_stream::~ckx_default_token_stream()
{
    delete impl;
}

saber_ptr<ckx_token> ckx_default_token_stream::operator [](
        int _offset)
{
    return impl->operator_index_impl(_offset);
}

void ckx_default_token_stream::operator ++ ()
{
    impl->operator_incr_impl();
}

namespace detail
{

constexpr size_t default_reserved_size = 8192;

ckx_default_token_stream_impl::ckx_default_token_stream_impl(
        ckx_file_reader &_file_reader) :
    current_pos(0),
    char_coord(1, 1),
    char_index(0)
{
    src.reserve(default_reserved_size);

    qchar ch;
    while ((ch = _file_reader.get_next_char())
           != (qchar)saber::char_traits<qchar>::eof())
    {
        src.push_back(ch);
    }

    do_split_tokens();
}

ckx_default_token_stream_impl::~ckx_default_token_stream_impl()
{}

saber_ptr<ckx_token> ckx_default_token_stream_impl::operator_index_impl(
        int _offset)
{
    if ( _offset + current_pos < token_buffer.size() )
    {
        return token_buffer[current_pos + _offset];
    }
    else
    {
        return saber_ptr<ckx_token>(nullptr);
    }
}

void ckx_default_token_stream_impl::operator_incr_impl()
{
    current_pos++;
}


void ckx_default_token_stream_impl::do_split_tokens()
{
    while (ch() != '\0')
    {
        switch (ch())
        {
        case '\n':
            {
                next_line();
                break;
            }

        case '\f': case '\v': case '\t': case '\r':
            {
                next_char();
                break;
            }

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                solve_numbers();
                break;
            }

        case '=':
            {
                next_char();
                if (ch() == '=')
                {
                    next_char();
                    token_buffer.emplace_back(
                        new ckx_token(
                            char_coord, ckx_token::type::token_eq) );
                }
                else
                {
                    token_buffer.emplace_back(
                        new ckx_token(
                            char_coord, ckx_token::type::token_assign) );
                }
                break;
            }

        }
    }
}

void ckx_default_token_stream_impl::solve_numbers()
{
    qint64 i = scan_integer();
    qreal r = i;

    bool is_real;

    if (ch() == '.')
    {
        next_char();
        if (ch() < '0' || ch() > '9')
        {
            issue_error("expected digits after floating point.");
            return;
        }

        is_real = true;
        r += scan_floating_part();
    }

    if (ch() == 'E' || ch() == 'e')
    {
        next_char();
        if (ch() < '0' || ch() > '9')
        {
            issue_error("expected digits after exponent.");
            return;
        }

        is_real = true;
        qint64 exponent = scan_integer();
        for (qint64 i = 0; i < exponent; i++) r *= 10;
    }

    if (is_real)
    {
        token_buffer.emplace_back(new ckx_token(char_coord, r));
    }
    else
    {
        token_buffer.emplace_back(new ckx_token(char_coord, i));
    }
}

qint64 ckx_default_token_stream_impl::scan_integer()
{
    qint64 ret = 0;
    while (ch() >= '0' && ch() <= '9')
    {
       ret *= 10;
       ret += ch() - '0';
       next_char();
    }
    return ret;
}

qreal ckx_default_token_stream_impl::scan_floating_part()
{
    qreal ret = 0;
    while (ch() >= '0' && ch() <= '9')
    {
        ret /= 10;
        ret += (qreal(ch() - '0'))/10;
    }
    return ret;
}

inline qchar ckx_default_token_stream_impl::ch()
{
    return src[char_index];
}

inline void ckx_default_token_stream_impl::next_char()
{
    char_coord.second++;
    char_index++;
}

inline void ckx_default_token_stream_impl::next_line()
{
    char_coord.first++;
    char_coord.second = 1;
    char_index++;
}

inline void ckx_default_token_stream_impl::issue_error(const char *_desc)
{
    errors.emplace_back(char_coord, saber::string(_desc));
}

} // namespace detail

} // namespace ckx
