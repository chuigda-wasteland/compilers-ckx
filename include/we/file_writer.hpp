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

#include "saber/string_pool.hpp"
#include "we/defs.hpp"

namespace we
{
namespace detail
{
    class we_fp_writer_impl;
    class we_ostream_writer_impl;
} // namespace detail


interface we_file_writer
{
public:
    we_file_writer() = default;
    virtual ~we_file_writer() = 0;

    virtual void write(qint64 _value) = 0;
    virtual void write(quint64 _value) = 0;
    virtual void write(qreal _value) = 0;
    virtual void write(const qchar* _str) = 0;
    virtual void write(const saber_string& _str) = 0;
    virtual void write(saber_string_view _str_view) = 0;
    virtual void write_whitespace(qsizet _count) = 0;

    we_file_writer(const we::we_file_writer& _another) = delete;
    we_file_writer& operator= (const we::we_file_writer& _another) = delete;
};

class we_fp_writer final implements we::we_file_writer
{
public:
    explicit we_fp_writer(std::FILE* _fp);
    ~we_fp_writer() override final;

    void write(qint64 _value) override final;
    void write(quint64 _value) override final;
    void write(qreal _value) override final;
    void write(const qchar* _str) override final;
    void write(const saber_string& _str) override final;
    void write(saber_string_view _str_view) override final;
    void write_whitespace(qsizet _count) override final;

    we_fp_writer(const we::we_fp_writer& _another) = delete;
    we_fp_writer& operator= (const we::we_fp_writer& _another) = delete;

private:
    detail::we_fp_writer_impl *impl;
};

class we_ostream_writer final implements we::we_file_writer
{
public:
    explicit we_ostream_writer(std::ostream& _stream);
    ~we_ostream_writer();

    void write(qint64 _value) override final;
    void write(quint64 _value) override final;
    void write(qreal _value) override final;
    void write(const qchar* _str) override final;
    void write(const saber_string& _str) override final;
    void write(saber_string_view _str_view) override final;
    void write_whitespace(qsizet _count) override final;

    we_ostream_writer(const we_ostream_writer& _another) = delete;
    we_ostream_writer& operator= (const we_ostream_writer& _another) = delete;

private:
    detail::we_ostream_writer_impl *impl;
};

} // namespace ckx

#endif // CKX_FILE_WRITER_HPP
