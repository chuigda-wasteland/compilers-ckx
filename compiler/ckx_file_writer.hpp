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


#ifndef CKX_FILE_WRITER_HPP
#define CKX_FILE_WRITER_HPP

#include <iosfwd>
#include <cstdio>

#include "string.hpp"
#include "defs.hpp"

namespace ckx
{
namespace detail
{
    class ckx_fp_writer_impl;
    class ckx_ostream_writer_impl;
} // namespace detail


interface ckx_file_writer
{
public:
    ckx_file_writer() = default;
    virtual ~ckx_file_writer() = 0;

    virtual void write(qint64 _value) = 0;
    virtual void write(quint64 _value) = 0;
    virtual void write(qreal _value) = 0;
    virtual void write(const qchar* _str) = 0;
    virtual void write_whitespace(qsizet _count) = 0;

    ckx_file_writer(const ckx_file_writer& _another) = delete;
    ckx_file_writer& operator= (const ckx_file_writer& _another) = delete;
};

class ckx_fp_writer final implements ckx_file_writer
{
public:
    explicit ckx_fp_writer(std::FILE* _fp);
    ~ckx_fp_writer() override final;

    void write(qint64 _value) override final;
    void write(quint64 _value) override final;
    void write(qreal _value) override final;
    void write(const qchar* _str) override final;
    void write_whitespace(qsizet _count) override final;

    ckx_fp_writer(const ckx_fp_writer& _another) = delete;
    ckx_fp_writer& operator= (const ckx_fp_writer& _another) = delete;

private:
    detail::ckx_fp_writer_impl *impl;
};

class ckx_ostream_writer final implements ckx_file_writer
{
public:
    explicit ckx_ostream_writer(std::ostream& _stream);
    ~ckx_ostream_writer();

    void write(qint64 _value) override final;
    void write(quint64 _value) override final;
    void write(qreal _value) override final;
    void write(const qchar* _str) override final;
    void write_whitespace(qsizet _count) override final;

    ckx_ostream_writer(const ckx_ostream_writer& _another) = delete;
    ckx_ostream_writer& operator= (const ckx_ostream_writer& _another) = delete;

private:
    detail::ckx_ostream_writer_impl *impl;
};

} // namespace ckx

#endif // CKX_FILE_WRITER_HPP
