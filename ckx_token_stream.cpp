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
    int current_pos;
    saber::vector<saber_ptr<ckx_token>> token_buffer;
};

}

ckx_default_token_stream::ckx_default_token_stream(ckx_file_reader &_file_reader)
{
    impl = new detail::ckx_default_token_stream_impl(_file_reader);
}

ckx_default_token_stream::~ckx_default_token_stream()
{
    delete impl;
}

saber_ptr<ckx_token> ckx_default_token_stream::operator [](int _offset)
{
    return impl->operator_index_impl(_offset);
}

void ckx_default_token_stream::operator ++ ()
{
    impl->operator_incr_impl();
}

namespace detail
{
// On hold
constexpr size_t default_reserved_size = 8192;

ckx_default_token_stream_impl::ckx_default_token_stream_impl(
        ckx_file_reader &_file_reader)
{
    saber::string str;
    str.reserve(default_reserved_size);

    qchar ch;
    while ((ch = _file_reader.get_next_char())
           != saber::char_traits<qchar>::eof())
    {
        str.push_back(ch);
    }
}

ckx_default_token_stream_impl::~ckx_default_token_stream_impl()
{}

saber_ptr<ckx_token> ckx_default_token_stream_impl::operator_index_impl(
        int _offset)
{ (void)_offset; return saber_ptr<ckx_token>(nullptr); }

void ckx_default_token_stream_impl::operator_incr_impl()
{  }

} // namespace detail

} // namespace ckx
