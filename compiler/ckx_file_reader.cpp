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


#include "ckx_file_reader.hpp"
#include <iostream>

using namespace ckx;

ckx_file_reader::~ckx_file_reader() {}

namespace ckx
{

namespace detail
{

class ckx_fp_reader_impl
{
public:
    ckx_fp_reader_impl(std::FILE* _fp);
    ~ckx_fp_reader_impl() = default;

    qchar get_next_char_impl();

private:
    std::FILE *fp;
};


class ckx_istream_reader_impl
{
public:
    ckx_istream_reader_impl(std::istream& _stream);
    ~ckx_istream_reader_impl() = default;

    qchar get_next_char_impl();

private:
    std::istream& stream;
};

} // namespace detail

ckx_fp_reader::ckx_fp_reader(FILE *_fp)
{
    this->impl = new detail::ckx_fp_reader_impl(_fp);
}

ckx_fp_reader::~ckx_fp_reader()
{
    delete this->impl;
}

qchar ckx_fp_reader::get_next_char()
{
    return this->impl->get_next_char_impl();
}


ckx_istream_reader::ckx_istream_reader(std::istream &_stream)
{
    this->impl = new detail::ckx_istream_reader_impl(_stream);
}

ckx_istream_reader::~ckx_istream_reader()
{
    delete this->impl;
}

qchar ckx_istream_reader::get_next_char()
{
    return this->impl->get_next_char_impl();
}

namespace detail
{

ckx_fp_reader_impl::ckx_fp_reader_impl(FILE *_fp) : fp(_fp) {}
qchar ckx_fp_reader_impl::get_next_char_impl()
{
    qchar ret;
    std::fscanf(this->fp, "%c", &ret);
    return ret;
}


ckx_istream_reader_impl::ckx_istream_reader_impl(std::istream &_stream) :
    stream(_stream)
{}

qchar ckx_istream_reader_impl::get_next_char_impl()
{
    qchar ch;
    return stream >> ch ? ch :
                          static_cast<qchar>(std::char_traits<qchar>::eof());
}


} // namespace detail

} // namespace ckx
