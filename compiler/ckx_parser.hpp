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


#ifndef CKX_PARSER_HPP
#define CKX_PARSER_HPP

#include "ckx_token_stream.hpp"
#include "ckx_ast_node_fwd.hpp"

namespace ckx
{

namespace detail
{

template <typename CkxTokenStream>
class ckx_parser_impl;

} // namespace detail

template <typename CkxTokenStream>
class ckx_parser
{
public:
    ckx_parser();
    ~ckx_parser();

    ckx_ast_translation_unit* parse(saber_ptr<CkxTokenStream> _token_stream);

private:
    detail::ckx_parser_impl<CkxTokenStream>* p_impl;
};

extern template class ckx_parser<ckx_default_token_stream>;

} // namespace ckx

#endif // CKX_PARSER_HPP
