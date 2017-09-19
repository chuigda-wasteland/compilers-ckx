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
#include "string_pool.hpp"
#include "vector.hpp"
#include "list.hpp"

#include "ckx_type.hpp"
#include "ckx_env_table.hpp"

#include "ckx_token.hpp"

#include "ckx_ast_node_fwd.hpp"

namespace ckx
{

using saber::saber_ptr;

interface ckx_ast_node
{
public:
    explicit ckx_ast_node(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_node() = default;

    saber_ptr<ckx_token> get_at_token();
    virtual void ast_dump(FILE *_fp, qint8 _level) = 0;

private:
    saber_ptr<ckx_token> at_token;
};



class ckx_ast_translation_unit implements ckx_ast_node
{
public:
    explicit ckx_ast_translation_unit(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_translation_unit();

    void add_new_stmt(ckx_ast_stmt *_stmt);
    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    saber::vector<ckx_ast_stmt*> stmts;
    ckx_env *global_table;
};



interface ckx_ast_stmt implements ckx_ast_node
{
public:
    explicit ckx_ast_stmt(saber_ptr<ckx_token> _at_token);
    virtual ~ckx_ast_stmt() = 0;

    virtual void ast_dump(FILE *_fp, qint8 _level) = 0;

    /// @attention all AST-Nodes can be dumped
    /// But only Statements can be translated into intermediate codes.
    /// Expression system will have another translating system.
    /// This function will be finished after we get a proper intermediate
    /// representation and start to write syntax-directed translation
    Q_ON_HOLD(virtual void translate(saber::list<ckx_ir_instance>& ret) = 0;)
};

class ckx_ast_compound_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_compound_stmt(saber_ptr<ckx_token> _at_token,
                          ckx_env *_table);
    ~ckx_ast_compound_stmt() override final;

    void add_new_stmt(ckx_ast_stmt *_stmt);

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    saber::vector<ckx_ast_stmt*> stmts;
    ckx_env *local_table;
};

class ckx_ast_if_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_if_stmt(saber_ptr<ckx_token> _at_token,
                    ckx_ast_expr* _condition,
                    ckx_ast_stmt* _then_clause,
                    ckx_ast_stmt* _else_clause);
    ~ckx_ast_if_stmt() override final;

    void ast_dump(FILE *_fp, qint8 _level) override final;

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

    void ast_dump(FILE *_fp, qint8 _level) override final;

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

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_for_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_for_stmt(saber_ptr<ckx_token> _at_token,
                     ckx_ast_expr *_init,
                     ckx_ast_expr *_condition,
                     ckx_ast_expr *_incr,
                     ckx_ast_stmt *_clause);
    ~ckx_ast_for_stmt();

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_ast_expr *init;
    ckx_ast_expr *condition;
    ckx_ast_expr *incr;
    ckx_ast_stmt *clause;
};

class ckx_ast_break_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_break_stmt(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_break_stmt() override final = default;

    void ast_dump(FILE *_fp, qint8 _level) override final;
};

class ckx_ast_continue_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_continue_stmt(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_continue_stmt() override final = default;

    void ast_dump(FILE *_fp, qint8 _level) override final;
};

class ckx_ast_return_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_return_stmt(saber_ptr<ckx_token> _at_token,
                        ckx_ast_expr* _return_expr);
    ~ckx_ast_return_stmt() override final = default;

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_ast_expr *return_expr;
};

class ckx_ast_decl_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_decl_stmt(saber_ptr<ckx_token> _at_token);
    ~ckx_ast_decl_stmt() override final;

    void add_decl(ckx_ast_init_decl* _decl);

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    saber::vector<ckx_ast_init_decl*> decls;
};

class ckx_ast_expr_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_expr_stmt(saber_ptr<ckx_token> _at_token, ckx_ast_expr* _expr);
    ~ckx_ast_expr_stmt() override final;

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_ast_expr *expr;
};

class ckx_ast_func_stmt implements ckx_ast_stmt
{
public:
    ckx_ast_func_stmt(saber_ptr<ckx_token> _at_token,
                      ckx_func_entry *_entry,
                      ckx_env *_param_env_table);
    ~ckx_ast_func_stmt();

    bool is_defined() const;

    ckx_env* get_param_env_table();
    void define(ckx_ast_compound_stmt* _fnbody);

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_func_entry *entry;
    ckx_env *param_env_table;
    ckx_ast_compound_stmt *fnbody = nullptr;
};

class ckx_ast_init_decl implements ckx_ast_node
{
public:
    ckx_ast_init_decl(saber_ptr<ckx_token> _at_token,
                      ckx_var_entry* _entry,
                      ckx_ast_expr* _init);
    virtual ~ckx_ast_init_decl() final;

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_var_entry *entry;
    ckx_ast_expr *init;
};

class ckx_ast_struct_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_struct_stmt(saber_ptr<ckx_token> _at_token,
                   ckx_type_entry* _entry);
    ~ckx_ast_struct_stmt();

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_type_entry *entry;
};

class ckx_ast_variant_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_variant_stmt(saber_ptr<ckx_token> _at_token,
                    ckx_type_entry* _entry);
    ~ckx_ast_variant_stmt();

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_type_entry *entry;
};

class ckx_ast_enum_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_enum_stmt(saber_ptr<ckx_token> _at_token,
                      ckx_type_entry* _entry);
    ~ckx_ast_enum_stmt();

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_type_entry *entry;
};



interface ckx_ast_expr implements ckx_ast_node
{
public:
    ckx_ast_expr(saber_ptr<ckx_token> _at_token);
    virtual ~ckx_ast_expr() = 0;

    virtual void ast_dump(FILE *_fp, qint8 _level) = 0;
};

class ckx_ast_binary_expr final implements ckx_ast_expr
{
public:
    ckx_ast_binary_expr(saber_ptr<ckx_token> _at_token,
                        ckx_token::type _opercode,
                        ckx_ast_expr *_loperand,
                        ckx_ast_expr *_roperand);
    ~ckx_ast_binary_expr() override final;

    void ast_dump(FILE* _fp, qint8 _level) override final;

private:
    ckx_token::type opercode;
    ckx_ast_expr *loperand;
    ckx_ast_expr *roperand;
};

class ckx_ast_unary_expr final implements ckx_ast_expr
{
public:
    ckx_ast_unary_expr(saber_ptr<ckx_token> _at_token,
                       ckx_token::type _opercode,
                       ckx_ast_expr *_operand);
    ~ckx_ast_unary_expr() override final;

    void ast_dump(FILE* _fp, qint8 _level) override final;

private:
    ckx_token::type opercode;
    ckx_ast_expr *operand;
};

class ckx_ast_subscript_expr final implements ckx_ast_expr
{
public:
    ckx_ast_subscript_expr(saber_ptr<ckx_token> _at_token,
                           ckx_ast_expr *_base,
                           ckx_ast_expr *_subscript);
    ~ckx_ast_subscript_expr() override final;

    void ast_dump(FILE *_fp, qint8 _level) override final;

private:
    ckx_ast_expr *base;
    ckx_ast_expr *subscript;
};

class ckx_ast_invoke_expr final implements ckx_ast_expr
{
public:
    ckx_ast_invoke_expr(saber_ptr<ckx_token> _at_token,
                        ckx_ast_expr *_invokable,
                        saber::vector<ckx_ast_expr*> &&_args);
    ~ckx_ast_invoke_expr() override final;

    void ast_dump(FILE* _fp, qint8 _level) override final;

private:
    ckx_ast_expr *invokable;
    saber::vector<ckx_ast_expr*> args;
};

class ckx_ast_cond_expr final implements ckx_ast_expr
{
public:
    ckx_ast_cond_expr(saber_ptr<ckx_token> _at_token,
                      ckx_ast_expr* _cond_expr,
                      ckx_ast_expr* _then_expr,
                      ckx_ast_expr* _else_expr);
    ~ckx_ast_cond_expr() override final;

    void ast_dump(FILE* _fp, qint8 _level) override final;

private:
    ckx_ast_expr* cond_expr;
    ckx_ast_expr* then_expr;
    ckx_ast_expr* else_expr;
};

class ckx_ast_id_expr final implements ckx_ast_expr
{
public:
    ckx_ast_id_expr(saber_ptr<ckx_token> _at_token, ckx_var_entry* _entry);
    ~ckx_ast_id_expr() override final;

    void ast_dump(FILE* _fp, qint8 _level) override final;

private:
    ckx_var_entry *entry;
};

/**
    @note maybe we can use constant-expression in place of sizeof-expression
class ckx_ast_sizeof_expr final implements ckx_ast_expr
{
public:
    ckx_ast_sizeof_expr(saber_ptr<ckx_token> _at_token,
                        saber_ptr<ckx_type> _type);
    ~ckx_ast_sizeof_expr() override final;

private:
    saber_ptr<ckx_type> type;
};
*/

} // namespace ckx

#endif // CKX_AST_NODE_HPP
