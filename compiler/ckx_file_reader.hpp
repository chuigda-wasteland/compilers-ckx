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


#ifndef CKX_FILE_READER_HPP
#define CKX_FILE_READER_HPP

#include <iosfwd>
#include <cstdio>

#include "string.hpp"
#include "defs.hpp"

namespace ckx
{
namespace detail
{
    class ckx_fp_reader_impl;
    class ckx_istream_reader_impl;
} // namespace detail


interface ckx_file_reader
{
public:
    ckx_file_reader() = default;
    virtual ~ckx_file_reader() = 0;
    virtual qchar get_next_char(void) = 0;
};


class ckx_fp_reader final implements ckx_file_reader
{
public:
    explicit ckx_fp_reader(std::FILE* _fp);
    ~ckx_fp_reader() override final;
    qchar get_next_char() override final;

private:
    detail::ckx_fp_reader_impl *impl;
};


class ckx_istream_reader final implements ckx_file_reader
{
public:
    explicit ckx_istream_reader(std::istream& _stream);
    ~ckx_istream_reader() override final;
    qchar get_next_char() override final;

private:
    detail::ckx_istream_reader_impl *impl;
};

} // namespace ckx

#endif // CKX_FILE_READER_HPP
