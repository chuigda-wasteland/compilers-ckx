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


#include "we/file_reader.hpp"
#include <iostream>

namespace we
{

we_file_reader::~we_file_reader() {}

namespace detail
{

class we_fp_reader_impl
{
public:
    explicit we_fp_reader_impl(std::FILE* _fp);
    ~we_fp_reader_impl() = default;

    inline qchar get_next_char_impl();

private:
    std::FILE *fp;
};


class we_istream_reader_impl
{
public:
    explicit we_istream_reader_impl(std::istream& _stream);
    ~we_istream_reader_impl() = default;

    inline qchar get_next_char_impl();

private:
    std::istream& stream;
};

} // namespace detail

we_fp_reader::we_fp_reader(FILE *_fp)
{
    this->impl = new detail::we_fp_reader_impl(_fp);
}

we_fp_reader::~we_fp_reader()
{
    delete this->impl;
}

qchar we_fp_reader::get_next_char()
{
    return this->impl->get_next_char_impl();
}


we_istream_reader::we_istream_reader(std::istream &_stream)
{
    this->impl = new detail::we_istream_reader_impl(_stream);
}

we_istream_reader::~we_istream_reader()
{
    delete this->impl;
}

qchar we_istream_reader::get_next_char()
{
    return this->impl->get_next_char_impl();
}

namespace detail
{

we_fp_reader_impl::we_fp_reader_impl(FILE *_fp) : fp(_fp) {}

inline qchar we_fp_reader_impl::get_next_char_impl()
{
    qchar ret;
    if (~std::fscanf(fp, "%c", &ret)) return ret;
    else return static_cast<qchar>(std::char_traits<qchar>::eof());
}


we_istream_reader_impl::we_istream_reader_impl(std::istream &_stream) :
    stream(_stream)
{}

inline qchar we_istream_reader_impl::get_next_char_impl()
{
    qchar ch;
    return stream >> ch ? ch :
                          static_cast<qchar>(std::char_traits<qchar>::eof());
}


} // namespace detail

} // namespace ckx
