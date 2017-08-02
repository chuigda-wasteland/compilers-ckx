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

#ifndef CKX_TOKEN_STREAM_HPP
#define CKX_TOKEN_STREAM_HPP

#include "ckx_token.hpp"
#include "ckx_file_reader.hpp"

#include "memory.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "unordered_map.hpp"

#include "ckx_error.hpp"

namespace ckx
{

using saber::saber_ptr;


interface ckx_token_stream
{
public:
    ckx_token_stream() = default;
    virtual ~ckx_token_stream() = 0;

    virtual saber_ptr<ckx_token> operator[] (int _offset) = 0;
    virtual saber::vector<ckx_error>& get_error() = 0;
    virtual void operator++ () = 0;
};


namespace detail
{
class ckx_default_token_stream_impl;
} // namespace detail


class ckx_default_token_stream implements ckx_token_stream
{
public:
    ckx_default_token_stream(ckx_file_reader& _file_reader);
    ~ckx_default_token_stream();

    saber_ptr<ckx_token> operator[] (int _offset) override final;
    saber::vector<ckx_error>& get_error() override final;
    void operator++ () override final;

private:
    detail::ckx_default_token_stream_impl *impl;
};

} // namespace ckx

#endif // CKX_TOKEN_STREAM_HPP
