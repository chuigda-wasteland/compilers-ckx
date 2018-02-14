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

#include "we/defs.hpp"
#include "we/file_writer.hpp"
#include "saber/string_pool.hpp"
#include "saber/vector.hpp"
#include "saber/memory.hpp"
#include "saber/optional.hpp"
#include "frontend/lex/ckx_token.hpp"
#include "frontend/AST/ckx_operator.hpp"
#include "frontend/AST/ckx_ast_node_fwd.hpp"
#include "frontend/parse/ckx_prelexed_type.hpp"
#include "frontend/sema/ckx_sema_result_fwd.hpp"
#include "frontend/sema/ckx_sema_fwd.hpp"

namespace ckx
{

using saber::saber_ptr;

interface ckx_ast_node
{
public:
    explicit ckx_ast_node() = default;
    virtual ~ckx_ast_node() = default;

    virtual void ast_dump(we::we_file_writer& _writer, quint16 _level) = 0;
};

class ckx_ast_translation_unit final implements ckx_ast_node
{
    friend class ckx_sema_engine;
public:
    explicit ckx_ast_translation_unit();
    ~ckx_ast_translation_unit();

    void add_new_stmt(ckx_ast_stmt *_stmt);
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;

    void accept(ckx_sema_engine& _sema);

private:
    saber::vector<ckx_ast_stmt*> stmts;
};

interface ckx_ast_stmt implements ckx_ast_node
{
public:
    explicit ckx_ast_stmt() = default;
    virtual ~ckx_ast_stmt() override = default;

    virtual void
    ast_dump(we::we_file_writer& _writer, quint16 _level) override = 0;
    virtual void accept(ckx_sema_engine& sema) = 0;
};

class ckx_ast_compound_stmt final implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    ckx_ast_compound_stmt(ckx_src_rng _lbrace_rng, ckx_src_rng _rbrace_rng,
                          saber::vector<ckx_ast_stmt*>&& _stmts);
    ~ckx_ast_compound_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng lbrace_rng, rbrace_rng;
    saber::vector<ckx_ast_stmt*> stmts;
};

class ckx_ast_if_stmt final implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    ckx_ast_if_stmt(ckx_src_rng _if_rng, ckx_src_rng _else_rng,
                    ckx_ast_expr* _condition,
                    ckx_ast_stmt* _then_clause,
                    ckx_ast_stmt* _else_clause);

    ckx_ast_if_stmt(ckx_src_rng _if_rng,
                    ckx_ast_expr* _condition,
                    ckx_ast_stmt* _then_clause);

    ~ckx_ast_if_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &_sema) override final;

private:
    ckx_src_rng if_rng;
    ckx_src_rng else_rng;
    ckx_ast_expr *condition;
    ckx_ast_stmt *then_clause;
    ckx_ast_stmt *else_clause;
};

class ckx_ast_while_stmt final implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    ckx_ast_while_stmt(ckx_src_rng _while_rng,
                       ckx_ast_expr *_condition,
                       ckx_ast_stmt *_clause);
    ~ckx_ast_while_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng while_rng;
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_do_while_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_do_while_stmt(ckx_src_rng _do_rng, ckx_src_rng _while_rng,
                          ckx_ast_expr *_condition,
                          ckx_ast_stmt *_clause);
    ~ckx_ast_do_while_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng do_rng;
    ckx_src_rng while_rng;
    ckx_ast_expr *condition;
    ckx_ast_stmt *clause;
};

class ckx_ast_for_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_for_stmt(ckx_src_rng _for_rng,
                     ckx_ast_expr *_init,
                     ckx_ast_expr *_condition,
                     ckx_ast_expr *_incr,
                     ckx_ast_stmt *_clause);
    ~ckx_ast_for_stmt();

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng for_rng;
    ckx_ast_expr *init;
    ckx_ast_expr *condition;
    ckx_ast_expr *incr;
    ckx_ast_stmt *clause;
};

class ckx_ast_break_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_break_stmt(ckx_src_rng _rng);
    ~ckx_ast_break_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng rng;
};

class ckx_ast_continue_stmt final implements ckx_ast_stmt
{
public:
    explicit ckx_ast_continue_stmt(ckx_src_rng _rng);
    ~ckx_ast_continue_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng rng;
};

class ckx_ast_return_stmt final implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    ckx_ast_return_stmt(ckx_src_rng _return_rng, ckx_ast_expr* _return_expr);
    ~ckx_ast_return_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng return_rng;
    ckx_ast_expr *return_expr;
};

class ckx_ast_decl_stmt final implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    open_class init_decl
    {
        init_decl(ckx_src_rng _rng,saber_string_view _name,ckx_ast_expr* _init):
            rng(_rng), name(_name), init(_init) {}
        ~init_decl();

        init_decl(const init_decl& _another) = delete;
        init_decl(init_decl&& _another) :
            rng(_another.rng), name(_another.name), init(_another.init)
        { _another.init = nullptr; }

        ckx_src_rng rng;
        saber_string_view name;
        ckx_ast_expr* init;
    };

    explicit ckx_ast_decl_stmt(ckx_prelexed_type&& _type,
                               saber::vector<init_decl>&& _decls);
    ~ckx_ast_decl_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_prelexed_type type;
    saber::vector<init_decl> decls;
};

class ckx_ast_expr_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_expr_stmt(ckx_ast_expr* _expr);
    ~ckx_ast_expr_stmt() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_ast_expr *expr;
};

class ckx_ast_func_stmt implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    open_class param_decl
    {
        param_decl(ckx_src_rng _rng,
                   ckx_prelexed_type&& _type,
                   saber_string_view _name) :
            rng(_rng), type(saber::move(_type)), name(_name) {}
        ckx_src_rng rng;
        ckx_prelexed_type type;
        saber_string_view name;
    };

    ckx_ast_func_stmt(ckx_src_rng _kwd_rng,
                      saber_string_view _name,
                      saber::vector<param_decl>&& _param_decls,
                      ckx_prelexed_type _ret_type,
                      ckx_ast_compound_stmt *_fnbody = nullptr);
    ~ckx_ast_func_stmt();

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &_sema) override final;

private:
    ckx_src_rng kwd_rng;
    saber_string_view name;
    saber::vector<param_decl> param_decls;
    ckx_prelexed_type ret_type;
    ckx_ast_compound_stmt *fnbody;
};

class ckx_ast_record_stmt final implements ckx_ast_stmt
{
    friend class ckx_sema_engine;
public:
    open_class field
    {
        field(ckx_src_rng _rng, saber_string_view _name)
            : rng(_rng), name(_name) {}
        ckx_src_rng rng;
        saber_string_view name;
    };

    open_class field_row
    {
        field_row(ckx_prelexed_type&& _type, saber::vector<field>&& _fields) :
            type(saber::move(_type)),
            fields(saber::move(_fields)) {}
        ckx_prelexed_type type;
        saber::vector<field> fields;
    };

    enum class record_tag : qchar
    {
        rt_struct,
        rt_variant
    };

    ckx_ast_record_stmt(ckx_src_rng _kwd_rng, ckx_src_rng _id_rng,
                        ckx_src_rng _lbrace_rng, ckx_src_rng _rbrace_rng,
                        record_tag _tag, saber_string_view _name,
                        saber::vector<field_row>&& _fields);
    ~ckx_ast_record_stmt() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng kwd_rng;
    ckx_src_rng id_rng;
    ckx_src_rng lbrace_rng;
    ckx_src_rng rbrace_rng;

    record_tag tag;
    saber_string_view name;
    saber::vector<field_row> fields;
};

class ckx_ast_enum_stmt final implements ckx_ast_stmt
{
public:
    open_class enumerator
    {
        enumerator(ckx_src_rng _id_rng, ckx_src_rng _value_rng,
                   saber_string_view _name, qint64 _value)
            : id_rng(_id_rng), value_rng(_value_rng),
              name(_name), value(_value) {}
        ckx_src_rng id_rng, value_rng;
        saber_string_view name;
        qint64 value;
    };

    ckx_ast_enum_stmt(ckx_src_rng _kwd_rng, ckx_src_rng _id_rng,
                      ckx_src_rng _lbrace_rng, ckx_src_rng _rbrace_rng,
                      saber_string_view _name,
                      saber::vector<enumerator>&& _enumerators);
    ~ckx_ast_enum_stmt() override final;

    const saber::vector<enumerator>& get_enumerators() const;
    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng kwd_rng;
    ckx_src_rng id_rng;
    ckx_src_rng lbrace_rng, rbrace_rng;
    saber_string_view name;
    saber::vector<enumerator> enumerators;
};

class ckx_ast_alias_stmt final implements ckx_ast_stmt
{
public:
    ckx_ast_alias_stmt(ckx_src_rng _kwd_rng, ckx_src_rng _id_rng,
                       saber_string_view _name, ckx_prelexed_type _type);
    ~ckx_ast_alias_stmt() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    void accept(ckx_sema_engine &sema) override final;

private:
    ckx_src_rng kwd_rng;
    ckx_src_rng id_rng;
    saber_string_view name;
    ckx_prelexed_type type;
};

interface ckx_ast_expr implements ckx_ast_node
{
public:
    ckx_ast_expr() = default;
    virtual ~ckx_ast_expr() override = default;

    virtual void
    ast_dump(we::we_file_writer& _writer, quint16 _level) override = 0;
    virtual saber::optional<ckx_expr_result> accept(ckx_sema_engine& _sema) = 0;
};

class ckx_ast_binary_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_binary_expr(ckx_src_rng _operator_rng,
                        ckx_op _opercode,
                        ckx_ast_expr *_loperand,
                        ckx_ast_expr *_roperand);
    ~ckx_ast_binary_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng operator_rng;
    ckx_op opercode;
    ckx_ast_expr *loperand;
    ckx_ast_expr *roperand;
};

class ckx_ast_unary_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_unary_expr(ckx_src_rng _operator_rng,
                       ckx_op _opercode,
                       ckx_ast_expr *_operand);
    ~ckx_ast_unary_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng operator_rng;
    ckx_op opercode;
    ckx_ast_expr *operand;
};

class ckx_ast_subscript_expr final implements ckx_ast_expr
{
public:
    ckx_ast_subscript_expr(ckx_src_rng _lbracket_rng, ckx_src_rng _rbracket_rng,
                           ckx_ast_expr *_base,
                           ckx_ast_expr *_subscript);
    ~ckx_ast_subscript_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng lbracket_rng;
    ckx_src_rng rbracket_rng;
    ckx_ast_expr *base;
    ckx_ast_expr *subscript;
};

class ckx_ast_invoke_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_invoke_expr(ckx_src_rng _lparen_rng, ckx_src_rng _rparen_rng,
                        ckx_ast_expr *_invokable,
                        saber::vector<ckx_ast_expr*> &&_args);
    ~ckx_ast_invoke_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng lparen_rng;
    ckx_src_rng rparen_rng;
    ckx_ast_expr *invokable;
    saber::vector<ckx_ast_expr*> args;
};

class ckx_ast_extract_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_extract_expr(ckx_ast_expr *_extracted,
                         saber_string_view _field_name);
    ~ckx_ast_extract_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_ast_expr *extracted;
    saber_string_view field_name;
};

class ckx_ast_enumerator_expr final implements ckx_ast_expr
{
public:
    ckx_ast_enumerator_expr(ckx_src_rng _enum_rng, ckx_src_rng _enumerator_rng,
                            saber_string_view _enum_name,
                            saber_string_view _enumerator_name);
    ~ckx_ast_enumerator_expr() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng enum_rng;
    ckx_src_rng enumerator_rng;
    saber_string_view enum_name;
    saber_string_view enumerator_name;
};

class ckx_ast_cond_expr final implements ckx_ast_expr
{
public:
    ckx_ast_cond_expr(ckx_src_rng _ques_rng, ckx_src_rng _colon_rng,
                      ckx_ast_expr* _cond_expr,
                      ckx_ast_expr* _then_expr,
                      ckx_ast_expr* _else_expr);
    ~ckx_ast_cond_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng ques_rng;
    ckx_src_rng colon_rng;
    ckx_ast_expr* cond_expr;
    ckx_ast_expr* then_expr;
    ckx_ast_expr* else_expr;
};

class ckx_ast_id_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_id_expr(ckx_src_rng _rng, saber_string_view _name);
    ~ckx_ast_id_expr() override final = default;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng rng;
    saber_string_view name;
};

class ckx_ast_cast_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    enum class castop : qchar
    { cst_static, cst_const, cst_reinterpret, cst_ckx };

    ckx_ast_cast_expr(ckx_src_rng _kwd_rng,
                      castop _op,
                      ckx_prelexed_type _desired_type,
                      ckx_ast_expr* _expr);
    ~ckx_ast_cast_expr() override final;

    void ast_dump(we::we_file_writer& _writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng kwd_rng;
    castop op;
    ckx_prelexed_type desired_type;
    ckx_ast_expr *expr;
};

class ckx_ast_sizeof_expr final implements ckx_ast_expr
{
public:
    ckx_ast_sizeof_expr(ckx_src_rng _kwd_rng, ckx_prelexed_type _type);
    ~ckx_ast_sizeof_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng kwd_rng;
    ckx_prelexed_type type;
};

class ckx_ast_vi_literal_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_vi_literal_expr(ckx_src_rng _rng, qint64 _val);
    ~ckx_ast_vi_literal_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng rng;
    qint64 val;
};

class ckx_ast_vr_literal_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_vr_literal_expr(ckx_src_rng _rng, qreal _val);
    ~ckx_ast_vr_literal_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng rng;
    qreal val;
};

class ckx_ast_bool_literal_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_bool_literal_expr(ckx_src_rng _rng, bool _val);
    ~ckx_ast_bool_literal_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng rng;
    bool val;
};

class ckx_ast_nullptr_expr final implements ckx_ast_expr
{
    friend class ckx_sema_engine;
public:
    ckx_ast_nullptr_expr(ckx_src_rng _rng);
    ~ckx_ast_nullptr_expr() override final = default;

    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_src_rng rng;
};

class ckx_ast_array_expr final implements ckx_ast_expr
{
public:
    ckx_ast_array_expr(ckx_prelexed_type _array_of_type);
    ~ckx_ast_array_expr() override final;

    void set_size(qint32 _size);
    void set_range(ckx_ast_expr* _start, ckx_ast_expr* _finish);
    void set_init_list(saber::vector<ckx_ast_expr*> &&_init_list);
    void ast_dump(we::we_file_writer &_writer, quint16 _level) override final;
    saber::optional<ckx_expr_result>
    accept(ckx_sema_engine& _sema) override final;

private:
    ckx_prelexed_type array_of_type;
    qint32 size = -1;
    ckx_ast_expr *start = nullptr;
    ckx_ast_expr *finish = nullptr;
    saber::vector<ckx_ast_expr*> init_list;
};

} // namespace ckx

#endif // CKX_AST_NODE_HPP
