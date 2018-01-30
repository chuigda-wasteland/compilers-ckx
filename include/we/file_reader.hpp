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

#include "saber/string.hpp"
#include "we/defs.hpp"

namespace we
{
namespace detail
{
    class we_fp_reader_impl;
    class we_istream_reader_impl;
} // namespace detail


interface we_file_reader
{
public:
    we_file_reader() = default;
    virtual ~we_file_reader() = 0;
    virtual qchar get_next_char(void) = 0;

    we_file_reader(const we_file_reader& _another) = delete;
    we_file_reader* operator= (const we_file_reader& _another) = delete;
};


class we_fp_reader final implements we_file_reader
{
public:
    explicit we_fp_reader(std::FILE* _fp);
    ~we_fp_reader() override final;
    qchar get_next_char() override final;

    we_fp_reader(const we_fp_reader& _another) = delete;
    we_fp_reader& operator= (const we_fp_reader& _another) = delete;

private:
    detail::we_fp_reader_impl *impl;
};


class we_istream_reader final implements we_file_reader
{
public:
    explicit we_istream_reader(std::istream& _stream);
    ~we_istream_reader() override final;
    qchar get_next_char() override final;

    we_istream_reader(const we_istream_reader& _another) = delete;
    we_istream_reader& operator= (const we_istream_reader& _another) = delete;

private:
    detail::we_istream_reader_impl *impl;
};

} // namespace ckx

#endif // CKX_FILE_READER_HPP
