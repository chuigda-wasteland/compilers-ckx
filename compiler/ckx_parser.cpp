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


#include "ckx_parser.hpp"

namespace ckx
{

namespace detail
{

template <typename TokenStream>
class ckx_parser_impl
{
public:
    ckx_parser_impl();
    ~ckx_parser_impl();

    ckx_ast_translation_unit* parse_impl(TokenStream& _token_stream);

private:

};

} // namespace detail

} // namespace ckx
