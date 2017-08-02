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

class ckx_identifier_table
{
public:
    static qpair<ckx_token::type, bool> lookup(const saber::string& _name);

private:
    static void initialize();

    static saber::unordered_map<saber::string, ckx_token::type> map;
    static bool initialized;
};

bool ckx_identifier_table::initialized = false;
saber::unordered_map<saber::string, ckx_token::type> ckx_identifier_table::map;

class ckx_default_token_stream_impl
{
public:
    ckx_default_token_stream_impl(ckx_file_reader &_file_reader);
    ~ckx_default_token_stream_impl();

    saber_ptr<ckx_token> operator_index_impl(int _offset);
    saber::vector<ckx_error>& get_error_impl(void);
    void operator_incr_impl(void);

private:
    qsizet current_pos;
    saber::vector<saber_ptr<ckx_token>> token_buffer;
    saber::vector<ckx_error> errors;
    saber::string src;

    void do_split_tokens();

    void solve_numbers();
    void solve_char_literal();
    void solve_bitwise_or_logic_op();
    void solve_op_or_opassign();
    void solve_colon_or_scope();
    void solve_ordinary_op();
    void scan_full_id_string();
    bool solve_keyword();
    void solve_identifier();

    qint64 scan_integer();
    qreal scan_floating_part();
    qchar make_conversion();

    inline bool is_part_of_id(qchar _ch);
    inline qchar ch();
    inline saber::string& str();
    inline qcoord& char_coord();
    inline void next_char();
    inline void next_line();

    inline void issue_error(const char* _desc);

    qcoord char_coord_temp;
    qsizet char_index_temp;

    saber::string string_temp;
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

saber::vector<ckx_error>& ckx_default_token_stream::get_error()
{
    return impl->get_error_impl();
}

void ckx_default_token_stream::operator ++ ()
{
    impl->operator_incr_impl();
}

namespace detail
{

static const saber::string __id_string_data[] =
{
#define GGLEX(X, Y) X,
#include "gg.h"
#undef GGLEX
""
};

static const ckx_token::type __id_tokentype_data[] =
{
#define GGLEX(X, Y) ckx_token::type::token_##Y,
#include "gg.h"
#undef GGLEX
ckx_token::type::token_eoi
};

qpair<ckx_token::type, bool>
ckx_identifier_table::lookup(const std::string &_name)
{
    if (!initialized) initialize();
    if (map.find(_name) != map.end())
    {
        return qpair<ckx_token::type, bool>(map[_name], true);
    }
    else
    {
        return qpair<ckx_token::type, bool>(
                    ckx_token::type::token_eoi, false);
    }
}

void ckx_identifier_table::initialize()
{
    for (qsizet i = 0; i < countof(__id_string_data); i++)
    {
        map.insert(
            qpair<saber::string, ckx_token::type>(
                        __id_string_data[i], __id_tokentype_data[i]));
    }
}



static constexpr size_t default_reserved_size = 8192;

ckx_default_token_stream_impl::ckx_default_token_stream_impl(
        ckx_file_reader &_file_reader) :
    current_pos(0),
    char_coord_temp(1, 1),
    char_index_temp(0)
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

saber::vector<ckx_error> &ckx_default_token_stream_impl::get_error_impl()
{
    return errors;
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
            next_line(); break;

        case '\f': case '\v': case '\t': case '\r': case ' ':
            next_char(); break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            solve_numbers(); break;

        case '\'':
            solve_char_literal(); break;

        case '&': case '|': case '^':
            solve_bitwise_or_logic_op(); break;

        case '+': case '-': case '*': case '/': case '%':
        case '!': case '<': case '>': case '=':
            // the processing of >=, <=, != and == is similar to op=,
            // so we combine them in one function solve_op_or_opassign().
            solve_op_or_opassign(); break;

        case ':':
            solve_colon_or_scope(); break;

        case '~': case ';': case ',': case '.': case '{':
        case '}': case '[': case ']': case '(': case ')':
            solve_ordinary_op();

        case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'i': case 'o': case 'r': case 's': case 'v':
        case 'w':
            scan_full_id_string();
            if (!solve_keyword()) goto ordinary_identifier_tag;
            break;

        case 'a': case 'g': case 'h': case 'j': case 'k':
        case 'l': case 'm': case 'n': case 'p': case 'q':
        case 't': case 'u': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z': case '_':
            scan_full_id_string();
        ordinary_identifier_tag:
            solve_identifier(); break;

        default:
            // What the fuck!
            issue_error("Unrecognized character");
        }
    }

    token_buffer.emplace_back(
                new ckx_token(char_coord(), ckx_token::type::token_eoi));
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
        token_buffer.emplace_back(new ckx_token(char_coord(), r));
    }
    else
    {
        token_buffer.emplace_back(new ckx_token(char_coord(), i));
    }
}

void ckx_default_token_stream_impl::solve_char_literal()
{
    next_char();
    qchar result = ch()!='\\' ? ch() : make_conversion();

    if (ch() != '\'')
    {
        issue_error("Missing \' character after char literal.");
        return;
    }

    token_buffer.emplace_back(new ckx_token(char_coord(), result));
    next_char();
}

void ckx_default_token_stream_impl::solve_bitwise_or_logic_op()
{
    ckx_token::type new_token_type;
    qchar pre = ch();
    next_char();

    bool is_logic = (pre == ch());

    switch (pre)
    {
    case '&':
        new_token_type = is_logic ? ckx_token::type::token_logic_and :
                                    ckx_token::type::token_bit_and;
        break;

    case '|':
        new_token_type = is_logic ? ckx_token::type::token_logic_or :
                                    ckx_token::type::token_bit_or;
        break;

    case '^':
        new_token_type = is_logic ? ckx_token::type::token_logic_xor :
                                    ckx_token::type::token_bit_xor;
        break;

    default:
        assert(false); // what the fuck!
    }

    token_buffer.emplace_back(new ckx_token(char_coord(), new_token_type));
    if (is_logic) next_char();
}

void ckx_default_token_stream_impl::solve_op_or_opassign()
{
    ckx_token::type new_token_type;

    qchar op = ch();
    next_char();
    bool is_opassign = ch() == '=';

    switch (op)
    {
    case '+':
        new_token_type = is_opassign ? ckx_token::type::token_add_assign :
                                       ckx_token::type::token_add;
        break;

    case '-':
        new_token_type = is_opassign ? ckx_token::type::token_sub_assign :
                                       ckx_token::type::token_sub;
        break;

    case '*':
        new_token_type = is_opassign ? ckx_token::type::token_mul_assign :
                                       ckx_token::type::token_mul;
        break;

    case '/':
        new_token_type = is_opassign ? ckx_token::type::token_div_assign :
                                       ckx_token::type::token_div;
        break;

    case '%':
        new_token_type = is_opassign ? ckx_token::type::token_mod_assign :
                                       ckx_token::type::token_mod;
        break;

    case '!':
        new_token_type = is_opassign ? ckx_token::type::token_neq :
                                       ckx_token::type::token_logic_not;
        break;

    case '<':
        new_token_type = is_opassign ? ckx_token::type::token_leq :
                                       ckx_token::type::token_lt;
        break;

    case '>':
        new_token_type = is_opassign ? ckx_token::type::token_geq :
                                       ckx_token::type::token_gt;
        break;

    case '=':
        new_token_type = is_opassign ? ckx_token::type::token_eq :
                                       ckx_token::type::token_assign;
        break;
    }

    token_buffer.emplace_back(new ckx_token(char_coord(), new_token_type));
    if (is_opassign) next_char();
}

void ckx_default_token_stream_impl::solve_colon_or_scope()
{
    next_char();
    if (ch() == ':')
    {
        next_char();
        token_buffer.emplace_back(
                    new ckx_token(char_coord(), ckx_token::type::token_scope));
    }
    else
    {
        token_buffer.emplace_back(
                    new ckx_token(char_coord(), ckx_token::type::token_colon));
    }
}

void ckx_default_token_stream_impl::solve_ordinary_op()
{
    ckx_token::type new_token_type;

    switch (ch())
    {
    case '~': new_token_type = ckx_token::type::token_bit_not;   break;
    case ';': new_token_type = ckx_token::type::token_semicolon; break;
    case ',': new_token_type = ckx_token::type::token_comma;     break;
    case '.': new_token_type = ckx_token::type::token_dot;       break;
    case '{': new_token_type = ckx_token::type::token_lbrace;    break;
    case '}': new_token_type = ckx_token::type::token_rbrace;    break;
    case '[': new_token_type = ckx_token::type::token_lbracket;  break;
    case ']': new_token_type = ckx_token::type::token_rbracket;  break;
    case '(': new_token_type = ckx_token::type::token_lparth;    break;
    case ')': new_token_type = ckx_token::type::token_rparth;    break;
        break;
    default:
        // What the fuck!
        assert(false);
    }

    token_buffer.emplace_back(new ckx_token(char_coord(), new_token_type));
}

void ckx_default_token_stream_impl::scan_full_id_string()
{
    str().clear();
    str().push_back(ch());
    next_char();

    while (is_part_of_id(ch()))
    {
        str().push_back(ch());
        next_char();
    }
}

bool ckx_default_token_stream_impl::solve_keyword()
{
    ckx_token::type type;
    bool is_identifier;
    std::tie(type, is_identifier) = ckx_identifier_table::lookup(str());

    if (is_identifier)
    {
        token_buffer.emplace_back(new ckx_token(char_coord(), type));
        return true;
    }
    else
    {
        return false;
    }
}

void ckx_default_token_stream_impl::solve_identifier()
{
    token_buffer.emplace_back(
        new ckx_token(char_coord(), saber::move(str())) );
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

qchar ckx_default_token_stream_impl::make_conversion()
{
    next_char();

    switch (ch())
    {
    case '0': return '\0';
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 'v': return '\v';

    default:
        // what the fuck!
        issue_error("Illegal conversion sequence");
        return static_cast<qchar>(saber::char_traits<qchar>::eof());
    }
}



inline bool ckx_default_token_stream_impl::is_part_of_id(qchar _ch)
{
    if (_ch == '_') return true;
    else if (_ch >= 'A' && _ch <= 'Z') return true;
    else if (_ch >= 'a' && _ch <= 'z') return true;
    else if (_ch >= '0' && _ch <= '9') return true;
    else return false;
}

inline qchar ckx_default_token_stream_impl::ch()
{
    return src[char_index_temp];
}

inline saber::string& ckx_default_token_stream_impl::str()
{
    return string_temp;
}

inline qcoord& ckx_default_token_stream_impl::char_coord()
{
    return char_coord_temp;
}

inline void ckx_default_token_stream_impl::next_char()
{
    char_coord_temp.second++;
    char_index_temp++;
}

inline void ckx_default_token_stream_impl::next_line()
{
    char_coord_temp.first++;
    char_coord_temp.second = 1;
    char_index_temp++;
}

inline void ckx_default_token_stream_impl::issue_error(const char *_desc)
{
    errors.emplace_back(char_coord(), saber::string(_desc));
}

} // namespace detail

} // namespace ckx
