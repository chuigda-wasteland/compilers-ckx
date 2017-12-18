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
#include "list.hpp"

namespace ckx
{

namespace detail
{
class ckx_parser_impl;
} // namespace detail

class ckx_parser
{
public:
    struct parse_result
    {
        parse_result(ckx_ast_translation_unit *_trans_unit,
                     saber::list<ckx_error> &&_error_list,
                     saber::list<ckx_error> &&_warn_list);
        ~parse_result();

        parse_result(const parse_result& _another) = delete;
        parse_result(parse_result&& _another);

        ckx_ast_translation_unit *trans_unit;
        saber::list<ckx_error> error_list;
        saber::list<ckx_error> warn_list;
    };

    ckx_parser();
    ~ckx_parser();

    parse_result parse(ckx_token_stream *_token_stream);

private:
    detail::ckx_parser_impl* p_impl;
};

} // namespace ckx

#endif // CKX_PARSER_HPP
