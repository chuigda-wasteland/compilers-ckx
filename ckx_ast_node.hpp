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

#ifndef CKX_AST_NODE_HPP
#define CKX_AST_NODE_HPP

#include "defs.hpp"

#include "memory.hpp"
#include "string.hpp"
#include "vector.hpp"

#include "ckx_type.hpp"

namespace ckx
{

using saber::saber_ptr;

interface ckx_ast_node
{
public:
    ckx_ast_node() = default;
    virtual ~ckx_ast_node() = 0;

    // virtual void translate(saber::vector<int>& _ret);
};

class ckx_ast_translation_unit implements ckx_ast_node
{
public:
    ckx_ast_translation_unit() = default;
    ~ckx_ast_translation_unit() override final;
};

class ckx_ast_decl implements ckx_ast_node
{
private:
    saber_ptr<ckx_type> type;
    saber::vector<saber_ptr<saber::string>> names;
};

class ckx_ast_record_decl implements ckx_ast_node
{
private:
    saber_ptr<ckx_record_type> type;
};

class ckx_ast_expr implements ckx_ast_node
{
    // On hold
};

class ckx_ast_if_stmt implements ckx_ast_node
{
private:
    ckx_ast_expr *condition;
    ckx_ast_node *then_execution;
    ckx_ast_node *else_execution;
};

class ckx_ast_switch_stmt implements ckx_ast_node
{
    // On hold
};

open_class ckx_function_param
{
    saber_ptr<ckx_type> type;
    saber_ptr<saber::string> name;
};

class ckx_ast_function implements ckx_ast_node
{
private:
    saber_ptr<ckx_function_type> type;
    ckx_ast_node *function_body;
};

} // namespace ckx

#endif // CKX_AST_NODE_HPP
