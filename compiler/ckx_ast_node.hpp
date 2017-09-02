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
#include "list.hpp"

#include "ckx_type.hpp"
#include "ckx_env_table.hpp"

#include "ckx_token.hpp"

#include "ckx_ast_node_fwd.hpp"

namespace ckx
{

using saber::saber_ptr;

class ckx_ast_node
{
public:
    explicit ckx_ast_node(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_node() = default;

    saber_ptr<ckx_token> get_at_token();

private:
    saber_ptr<ckx_token> at_token;
};



class ckx_ast_translation_unit make_use_of ckx_ast_node
{
public:
    explicit ckx_ast_translation_unit(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_translation_unit();

    void add_new_stmt(ckx_ast_stmt *_stmt);

private:
    saber::vector<ckx_ast_stmt*> stmts;
    ckx_env_table *global_table;
};



interface ckx_ast_stmt make_use_of ckx_ast_node
{
public:
    explicit ckx_ast_stmt(saber_ptr<ckx_token> _at_token);
    virtual ~ckx_ast_stmt() = 0;

    // SKTT1Faker
    // This function will be finished after we get a proper intermediate
    // representation and start to write syntax-directed translation
    Q_ON_HOLD(virtual void translate(saber::list<ckx_ir_instance>& ret) = 0;)
};

class ckx_ast_compound_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_compound_stmt(saber_ptr<ckx_token> _at_token,
                          ckx_env_table *_parent_scope_table);
    ~ckx_ast_compound_stmt() override final;

    void add_new_stmt(ckx_ast_stmt *_stmt);

private:
    saber::vector<ckx_ast_stmt*> stmts;
    ckx_env_table *local_table;
};

class ckx_ast_if_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_if_stmt(saber_ptr<ckx_token> _at_token,
                    ckx_ast_expr* _condition,
                    ckx_ast_stmt* _then_clause,
                    ckx_ast_stmt* _else_clause);
    ~ckx_ast_if_stmt() override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *then_clause;
    ckx_ast_stmt *else_clause;
};

class ckx_ast_while_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_while_stmt(saber_ptr<ckx_token> _at_token,
                       ckx_ast_expr *_condition,
                       ckx_ast_stmt *_clause);
    ~ckx_ast_while_stmt() override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_do_while_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_do_while_stmt(saber_ptr<ckx_token> _at_token,
                          ckx_ast_expr *_condition,
                          ckx_ast_stmt *_clause);
    ~ckx_ast_do_while_stmt() override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_break_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_break_stmt(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_break_stmt() override final = default;
};

class ckx_ast_continue_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_continue_stmt(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_continue_stmt() override final = default;
};

class ckx_ast_return_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_return_stmt(saber_ptr<ckx_token> _at_token,
                        ckx_ast_expr* _return_expr);
    ~ckx_ast_return_stmt() override final = default;

private:
    ckx_ast_expr *return_expr;
};

class ckx_ast_decl_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_decl_stmt(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_decl_stmt() override final;

    void add_decl(ckx_ast_init_decl* _decl);

private:
    saber::vector<ckx_ast_init_decl*> decls;
};

class ckx_ast_expr_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_expr_stmt(saber_ptr<ckx_token> _at_token, ckx_ast_expr* _expr);
    ~ckx_ast_expr_stmt() override final;

private:
    ckx_ast_expr *expr;
};


class ckx_ast_expr make_use_of ckx_ast_node
{
    Q_ON_HOLD(...)
};

class ckx_ast_func_stmt make_use_of ckx_ast_node
{
public:
    ckx_ast_func_stmt(saber_ptr<ckx_token> _at_token,
                      ckx_func_entry *_entry,
                      ckx_env_table *_param_env_table);
    ~ckx_ast_func_stmt();

    bool is_defined() const;

    ckx_env_table* get_param_env_table();
    void define(ckx_ast_compound_stmt* _fnbody) const;

private:
    ckx_func_entry *entry;
    ckx_env_table *param_env_table;
};

class ckx_ast_init_decl make_use_of ckx_ast_node
{
public:
    ckx_ast_init_decl(saber_ptr<ckx_token> _at_token,
                      ckx_var_entry* _entry,
                      ckx_ast_expr* _init);
    ~ckx_ast_init_decl();

private:
    ckx_var_entry *entry;
    ckx_ast_expr *init;
};

class ckx_ast_param_decl make_use_of ckx_ast_node
{
public:
    ckx_ast_param_decl(saber_ptr<ckx_token> _at_token,
                       ckx_var_entry* _entry);
    ~ckx_ast_param_decl() = default;

private:
    ckx_var_entry *entry;
};

class ckx_ast_struct_stmt make_use_of ckx_ast_node
{
public:
    ckx_ast_struct_stmt(saber_ptr<ckx_token> _at_token,
                   ckx_type_entry* _entry);
    ~ckx_ast_struct_stmt();

private:
    ckx_type_entry *entry;
};

class ckx_ast_variant_stmt make_use_of ckx_ast_node
{
public:
    ckx_ast_variant_stmt(saber_ptr<ckx_token> _at_token,
                    ckx_type_entry* _entry);
    ~ckx_ast_variant_stmt();

private:
    ckx_type_entry *entry;
};

class ckx_ast_enum_stmt make_use_of ckx_ast_node
{
public:
    ckx_ast_enum_stmt(saber_ptr<ckx_token> _at_token,
                      ckx_type_entry* _entry);
    ~ckx_ast_enum_stmt();

private:
    ckx_type_entry *entry;
};

} // namespace ckx

#endif // CKX_AST_NODE_HPP
