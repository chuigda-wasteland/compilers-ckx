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

#include "string_pool.hpp"
#include "vector.hpp"
#include "memory.hpp"
#include "list.hpp"

#include "ckx_type.hpp"
#include "ckx_token.hpp"
#include "ckx_operator.hpp"
#include "ckx_env_table.hpp"
#include "file_writer.hpp"
#include "ckx_ast_node_fwd.hpp"

namespace ckx
{

using saber::saber_ptr;

interface ckx_ast_node
{
public:
    explicit ckx_ast_node(ckx_token _at_token);
    ~ckx_ast_node() = default;

    ckx_token get_at_token();
    virtual void ast_dump(we::we_file_writer& _writer, quint16 _level) = 0;

private:
    const ckx_token at_token;
};

class ckx_ast_translation_unit final implements ckx_ast_node
{
public:
    explicit ckx_ast_translation_unit(ckx_token _at_token);
    ~ckx_ast_translation_unit();

    void add_new_stmt(ckx_ast_stmt *_stmt);
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber::vector<ckx_ast_stmt*> stmts;
};

interface ckx_ast_stmt implements ckx_ast_node
{
public:
    explicit ckx_ast_stmt(ckx_token _at_token);
    virtual ~ckx_ast_stmt() = 0;

    virtual void ast_dump(we::we_file_writer& _writer, quint16 _level) = 0;

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
    ckx_ast_compound_stmt(ckx_token _at_token);
    ~ckx_ast_compound_stmt() override final;

    void add_new_stmt(ckx_ast_stmt *_stmt);
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber::vector<ckx_ast_stmt*> stmts;
};

class ckx_ast_if_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_if_stmt(ckx_token _at_token,
                    ckx_ast_expr* _condition,
                    ckx_ast_stmt* _then_clause,
                    ckx_ast_stmt* _else_clause);
    ~ckx_ast_if_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *then_clause;
    ckx_ast_stmt *else_clause;
};

class ckx_ast_while_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_while_stmt(ckx_token _at_token,
                       ckx_ast_expr *_condition,
                       ckx_ast_stmt *_clause);
    ~ckx_ast_while_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_do_while_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_do_while_stmt(ckx_token _at_token,
                          ckx_ast_expr *_condition,
                          ckx_ast_stmt *_clause);
    ~ckx_ast_do_while_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_for_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_for_stmt(ckx_token _at_token,
                     ckx_ast_expr *_init,
                     ckx_ast_expr *_condition,
                     ckx_ast_expr *_incr,
                     ckx_ast_stmt *_clause);
    ~ckx_ast_for_stmt();

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *init;
    ckx_ast_expr *condition;
    ckx_ast_expr *incr;
    ckx_ast_stmt *clause;
};

class ckx_ast_break_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_break_stmt(ckx_token _at_token);
    ~ckx_ast_break_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
};

class ckx_ast_continue_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_continue_stmt(ckx_token _at_token);
    ~ckx_ast_continue_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
};

class ckx_ast_return_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_return_stmt(ckx_token _at_token,
                        ckx_ast_expr* _return_expr);
    ~ckx_ast_return_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *return_expr;
};

class ckx_ast_decl_stmt final implements ckx_ast_stmt
{
public:
    open_class init_decl
    {
        init_decl(saber_string_view _name, ckx_ast_expr* _init) :
            name(_name), init(_init) {}
        ~init_decl();

        init_decl(const init_decl& _another) = delete;
        init_decl(init_decl&& _another) :
            name(_another.name), init(_another.init)
        { _another.init = nullptr; }

        saber_string_view name;
        ckx_ast_expr* init;
    };

    explicit ckx_ast_decl_stmt(ckx_token _at_token,
                               saber_ptr<ckx_type> _type,
                               saber::vector<init_decl>&& _decls);
    ~ckx_ast_decl_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_ptr<ckx_type> type;
    saber::vector<init_decl> decls;
};

class ckx_ast_expr_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_expr_stmt(ckx_token _at_token, ckx_ast_expr* _expr);
    ~ckx_ast_expr_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *expr;
};

class ckx_ast_func_stmt implements ckx_ast_stmt
{
public:
    open_class param_decl
    {
        param_decl(saber_ptr<ckx_type> _type, saber_string_view _name) :
            type(_type), name(_name) {}
        saber_ptr<ckx_type> type;
        saber_string_view name;
    };

    ckx_ast_func_stmt(ckx_token _at_token,
                      saber_string_view _name,
                      saber::vector<param_decl>&& _param_decls,
                      saber_ptr<ckx_type> _ret_type,
                      ckx_ast_compound_stmt *_fnbody = nullptr);
    ~ckx_ast_func_stmt();

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_string_view name;
    saber::vector<param_decl> param_decls;
    saber_ptr<ckx_type> ret_type;
    ckx_ast_compound_stmt *fnbody;
};

class ckx_ast_struct_stmt final implements ckx_ast_stmt
{
public:
    open_class field
    {
        field(saber_ptr<ckx_type> _type, saber_string_view _name) :
            type(_type), name(_name) {}
        saber_ptr<ckx_type> type;
        saber_string_view name;
    };

    ckx_ast_struct_stmt(ckx_token _at_token,
                        saber_string_view _name,
                        saber::vector<field>&& _fields);
    ~ckx_ast_struct_stmt() override final;

    const saber::vector<field>& get_fields() const;
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_string_view name;
    saber::vector<field> fields;
};

class ckx_ast_variant_stmt final implements ckx_ast_stmt
{
public:
    open_class field
    {
        field(saber_ptr<ckx_type> _type, saber_string_view _name) :
            type(_type), name(_name) {}
        saber_ptr<ckx_type> type;
        saber_string_view name;
    };

    ckx_ast_variant_stmt(ckx_token _at_token,
                         saber_string_view _name,
                         saber::vector<field>&& _fields);
    ~ckx_ast_variant_stmt() override final;

    const saber::vector<field>& get_fields() const;
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_string_view name;
    saber::vector<field> fields;
};

class ckx_ast_enum_stmt final implements ckx_ast_stmt
{
public:
    open_class enumerator
    {
        enumerator(saber_string_view _name, qint64 _value)
            : name(_name), value(_value) {}
        saber_string_view name;
        qint64 value;
    };

    ckx_ast_enum_stmt(ckx_token _at_token,
                      saber_string_view _name,
                      saber::vector<enumerator>&& _enumerators);
    ~ckx_ast_enum_stmt() override final;

    const saber::vector<enumerator>& get_enumerators() const;
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_string_view name;
    saber::vector<enumerator> enumerators;
};

class ckx_ast_alias_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_alias_stmt(ckx_token _at_token,
                       saber_string_view _name,
                       saber_ptr<ckx_type> _type);
    ~ckx_ast_alias_stmt() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;

private:
    saber_string_view name;
    saber_ptr<ckx_type> type;
};

interface ckx_ast_expr implements ckx_ast_node
{
public:
    ckx_ast_expr(ckx_token _at_token);
    virtual ~ckx_ast_expr() = 0;

    virtual void ast_dump(we::we_file_writer& _writer, quint16 _level) = 0;
};

class ckx_ast_binary_expr final implements ckx_ast_expr
{
public:
    ckx_ast_binary_expr(ckx_token _at_token,
                        ckx_op _opercode,
                        ckx_ast_expr *_loperand,
                        ckx_ast_expr *_roperand);
    ~ckx_ast_binary_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_op opercode;
    ckx_ast_expr *loperand;
    ckx_ast_expr *roperand;
};

class ckx_ast_unary_expr final implements ckx_ast_expr
{
public:
    ckx_ast_unary_expr(ckx_token _at_token,
                       ckx_op _opercode,
                       ckx_ast_expr *_operand);
    ~ckx_ast_unary_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_op opercode;
    ckx_ast_expr *operand;
};

class ckx_ast_subscript_expr final implements ckx_ast_expr
{
public:
    ckx_ast_subscript_expr(ckx_token _at_token,
                           ckx_ast_expr *_base,
                           ckx_ast_expr *_subscript);
    ~ckx_ast_subscript_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *base;
    ckx_ast_expr *subscript;
};

class ckx_ast_invoke_expr final implements ckx_ast_expr
{
public:
    ckx_ast_invoke_expr(ckx_token _at_token,
                        ckx_ast_expr *_invokable,
                        saber::vector<ckx_ast_expr*> &&_args);
    ~ckx_ast_invoke_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *invokable;
    saber::vector<ckx_ast_expr*> args;
};

class ckx_ast_extract_expr final implements ckx_ast_expr
{
public:
    ckx_ast_extract_expr(ckx_token _at_token,
                         ckx_ast_expr *_extracted,
                         saber_string_view _field_name);
    ~ckx_ast_extract_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr *extracted;
    saber_string_view field_name;
};

class ckx_ast_enumerator_expr final implements ckx_ast_expr
{
public:
    ckx_ast_enumerator_expr(ckx_token _at_token,
                            saber_string_view _enum_name,
                            saber_string_view _enumerator_name);
    ~ckx_ast_enumerator_expr() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_string_view enum_name;
    saber_string_view enumerator_name;
};

class ckx_ast_cond_expr final implements ckx_ast_expr
{
public:
    ckx_ast_cond_expr(ckx_token _at_token,
                      ckx_ast_expr* _cond_expr,
                      ckx_ast_expr* _then_expr,
                      ckx_ast_expr* _else_expr);
    ~ckx_ast_cond_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    ckx_ast_expr* cond_expr;
    ckx_ast_expr* then_expr;
    ckx_ast_expr* else_expr;
};

class ckx_ast_id_expr final implements ckx_ast_expr
{
public:
    ckx_ast_id_expr(ckx_token _at_token, saber_string_view _name);
    ~ckx_ast_id_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    saber_string_view name;
};

class ckx_ast_cast_expr final implements ckx_ast_expr
{
public:
    enum class castop : qchar
    { cst_static, cst_const, cst_reinterpret, cst_ckx };

    ckx_ast_cast_expr(ckx_token _at_token,
                      castop _op,
                      saber_ptr<ckx_type> _desired_type,
                      ckx_ast_expr* _expr);
    ~ckx_ast_cast_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

private:
    castop op;
    saber_ptr<ckx_type> desired_type;
    ckx_ast_expr *expr;
};

class ckx_ast_sizeof_expr final implements ckx_ast_expr
{
public:
    ckx_ast_sizeof_expr(ckx_token _at_token,
                        saber_ptr<ckx_type> _type);
    ~ckx_ast_sizeof_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;

private:
    saber_ptr<ckx_type> type;
};

class ckx_ast_vi_literal_expr final implements ckx_ast_expr
{
public:
    ckx_ast_vi_literal_expr(ckx_token _at_token, qint64 _val);
    ~ckx_ast_vi_literal_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;

private:
    qint64 val;
};

class ckx_ast_vr_literal_expr final implements ckx_ast_expr
{
public:
    ckx_ast_vr_literal_expr(ckx_token _at_token, qreal _val);
    ~ckx_ast_vr_literal_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;

private:
    qreal val;
};

class ckx_ast_array_expr final implements ckx_ast_expr
{
public:
    ckx_ast_array_expr(ckx_token _at_token,
                       saber_ptr<ckx_type> _array_of_type);
    ~ckx_ast_array_expr() override final;

    void set_size(qint32 _size);
    void set_range(ckx_ast_expr* _start, ckx_ast_expr* _finish);
    void set_init_list(saber::vector<ckx_ast_expr*> &&_init_list);
    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;

private:
    saber_ptr<ckx_type> array_of_type;
    qint32 size = -1;
    ckx_ast_expr *start = nullptr;
    ckx_ast_expr *finish = nullptr;
    saber::vector<ckx_ast_expr*> init_list;
};

} // namespace ckx

#endif // CKX_AST_NODE_HPP
