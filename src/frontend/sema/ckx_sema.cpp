#include "frontend/sema/ckx_sema.hpp"
#include "frontend/sema/ckx_llvm_type_builder.hpp"

#include <algorithm>

namespace ckx
{

void ckx_sema_engine::visit_translation_unit(ckx_ast_translation_unit *_unit)
{
    for (ckx_ast_stmt *stmt : _unit->stmts) stmt->accept(*this);
}

void ckx_sema_engine::visit_compound_stmt(ckx_ast_compound_stmt *_stmt)
{
    enter_scope();
    for (ckx_ast_stmt *stmt : _stmt->stmts) stmt->accept(*this);
    leave_scope();
}

void ckx_sema_engine::visit_decl_stmt(ckx_ast_decl_stmt* _decl_stmt)
{
    if (is_in_func())
        visit_local_decl(_decl_stmt);
    else
        visit_global_decl(_decl_stmt);
}

void ckx_sema_engine::visit_record_stmt(ckx_ast_record_stmt *_record_stmt)
{
    C8ASSERT(!is_in_func());
    if (root_env.lookup(_record_stmt->name))
    {
        error();
        return;
    }

    if (_record_stmt->tag == ckx_ast_record_stmt::record_tag::rt_struct)
        visit_struct_decl(_record_stmt);
    else
        visit_variant_decl(_record_stmt);
}

void ckx_sema_engine::visit_func_stmt(ckx_ast_func_stmt *_func_stmt)
{
    if (_func_stmt->fnbody == nullptr)
        visit_func_decl(_func_stmt);
    else
        visit_func_def(_func_stmt);
}

void ckx_sema_engine::visit_return_stmt(ckx_ast_return_stmt *_return_stmt)
{
    if (_return_stmt->return_expr != nullptr)
    {
        saber::optional<ckx_expr_result> return_expr_result =
            _return_stmt->return_expr->accept(*this);
        if (!return_expr_result.is_type()) return;

        ckx_expr_result return_expr_result1 =
            decay_to_rvalue(return_expr_result.get());
        ckx_type *correct_ret_type =
            context_manager.lookup_func_context()->func_type->get_return_type();

        saber::optional<ckx_expr_result> casted_result =
            try_implicit_cast(return_expr_result1, correct_ret_type);

        if (!casted_result.is_type())
        {
            error();
            return;
        }

        builder.create_return(ckx_llvm_type_builder::build(correct_ret_type),
                              casted_result.get().llvm_value_bind);
    }
    else
    {
        builder.create_return_void();
    }
}

ckx_expr_result ckx_sema_engine::decay_to_rvalue(ckx_expr_result _expr)
{
    if (_expr.categ == ckx_expr_result::value_category::lvalue)
    {
        faker::llvm_value *decayed_value = builder.create_temporary_var();
        builder.create_load(decayed_value,
                            ckx_llvm_type_builder::build(_expr.type),
                            _expr.llvm_value_bind);
        return ckx_expr_result(_expr.type,
                               ckx_expr_result::value_category::prvalue,
                               decayed_value);
    }
    else
    {
        return _expr;
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::try_implicit_cast(ckx_expr_result _expr, ckx_type *_desired)
{
    if (!ckx_type_helper::can_implicit_cast(_expr.type, _desired))
        return saber::optional<ckx_expr_result>();

    if (_expr.type->equal_to_no_cvr(_desired))
    {
        /// @note Do nothing, qualifiers never matter.
        return _expr;
    }
    else if (_expr.type->is_signed())
    {
        C8ASSERT(_desired->is_signed());
        faker::llvm_value *destloc = builder.create_temporary_var();
        builder.create_sext(destloc,
                            ckx_llvm_type_builder::build(_expr.type),
                            _expr.llvm_value_bind,
                            ckx_llvm_type_builder::build(_desired));
        return saber::optional<ckx_expr_result>(
            _desired, ckx_expr_result::value_category::prvalue, destloc);
    }
    else if (_expr.type->is_unsigned())
    {
        C8ASSERT(_desired->is_unsigned());
        faker::llvm_value *destloc = builder.create_temporary_var();
        builder.create_zext(destloc,
                            ckx_llvm_type_builder::build(_expr.type),
                            _expr.llvm_value_bind,
                            ckx_llvm_type_builder::build(_desired));
        return saber::optional<ckx_expr_result>(
            _desired, ckx_expr_result::value_category::prvalue, destloc);
    }
    else if (_expr.type->is_floating())
    {
        C8ASSERT(_desired->is_floating());
        faker::llvm_value *destloc = builder.create_temporary_var();
        builder.create_fpext(destloc,
                             ckx_llvm_type_builder::build(_expr.type),
                             _expr.llvm_value_bind,
                             ckx_llvm_type_builder::build(_desired));
        return saber::optional<ckx_expr_result>(
            _desired, ckx_expr_result::value_category::prvalue, destloc);
    }
    else
    {
        C8ASSERT(false);
        return saber::optional<ckx_expr_result>();
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_binary_expr(ckx_ast_binary_expr *_binary_expr)
{
    if (!ckx_op_helper::is_logical(_binary_expr->opercode))
    {
        saber::optional<ckx_expr_result> loperand_result =
            _binary_expr->loperand->accept(*this);

        saber::optional<ckx_expr_result> roperand_result =
            _binary_expr->roperand->accept(*this);

        if (!loperand_result.is_type() || !roperand_result.is_type())
        {
            error();
            return saber::optional<ckx_expr_result>();
        }

        switch (_binary_expr->opercode)
        {
        case ckx_op::op_assign:
            return visit_assign(loperand_result.get(), roperand_result.get());
        case ckx_op::op_add:
            return visit_add(loperand_result.get(), roperand_result.get());
        case ckx_op::op_sub:
            return visit_sub(loperand_result.get(), roperand_result.get());
        default:
            C8ASSERT(false);
        }
    }
    else
    {
        /// @note logical operators involves shortcuts and must be evaluated
        /// individually.
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_unary_expr(ckx_ast_unary_expr *_unary_expr)
{
    switch (_unary_expr->opercode)
    {
    case ckx_op::op_addr:
        return visit_addressof_expr(_unary_expr);
    case ckx_op::op_deref:
        return visit_deref_expr(_unary_expr);
    default:
        C8ASSERT(false);
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_invoke_expr(ckx_ast_invoke_expr *_invoke_expr)
{
    saber::vector<ckx_expr_result> arg_results;

    /// Evaluate arguments
    for (ckx_ast_expr *expr : _invoke_expr->args)
    {
        saber::optional<ckx_expr_result> arg_result = expr->accept(*this);
        if (!arg_result.is_type())
            return saber::optional<ckx_expr_result>();
        arg_results.push_back(decay_to_rvalue(arg_result.get()));
    }

    if (ckx_ast_id_expr *id_expr =
        dynamic_cast<ckx_ast_id_expr*>(_invoke_expr->invokable))
    {
        /// Lookup function in current scope
        saber::vector<ckx_env_func_entry> *funcs =
            current_env->lookup_func(id_expr->name);

        if (!funcs)
        {
            error();
            return saber::optional<ckx_expr_result>();
        }

        /// Overloading resolution based on "disagreements"
        /// between paramaters and arguments
        saber::vector<quint64> disagreements;

        for (ckx_env_func_entry& entry : *funcs)
        {
            ckx_func_type* func_type = entry.type;
            disagreements.push_back(
                calculate_disagreements(
                    arg_results, func_type->get_param_type_list()));
        }

        auto it = std::min_element(disagreements.begin(), disagreements.end());
        if (*it == std::numeric_limits<quint64>::max())
        {
            error();
            return saber::optional<ckx_expr_result>();
        }

        ckx_env_func_entry &selected = (*funcs)[it - disagreements.begin()];

        /// Emit instructions and build result
        saber::vector<faker::llvm_type> types;
        saber::vector<faker::llvm_value*> args;
        for (qsizet i = 0; i < arg_results.size(); i++)
        {
            types.push_back(ckx_llvm_type_builder::build(
                                selected.type->get_param_type_list()[i]));

            /// Once we selected one function, it never fails
            saber::optional<ckx_expr_result> casted_result =
                try_implicit_cast(arg_results[i],
                                  selected.type->get_param_type_list()[i]);
            args.push_back(casted_result.get().llvm_value_bind);
        }

        faker::llvm_value* value =
            (selected.type->get_return_type()->get_category()
            != ckx_type::category::type_void) ?
                builder.create_temporary_var() : nullptr;
        builder.create_call(
            value,
            ckx_llvm_type_builder::build(selected.type->get_return_type()),
            selected.llvm_name, saber::move(types), saber::move(args));

        return saber::optional<ckx_expr_result>(
            selected.type->get_return_type(),
            ckx_expr_result::value_category::prvalue, value);
    }

    C8ASSERT(false);
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_extract_expr(ckx_ast_extract_expr *_extract_expr)
{
    saber::optional<ckx_expr_result> base_result =
        _extract_expr->extracted->accept(*this);

    if (!base_result.is_type()) return saber::optional<ckx_expr_result>();

    if (!base_result.get().type->is_record())
    {
        error();
        return saber::optional<ckx_expr_result>();
    }

    /// @todo remember to add solution for variant type
    /// once it gets supported.
    ckx_struct_type *struct_type =
        static_cast<ckx_struct_type*>(base_result.get().type);
    auto it = std::find_if(struct_type->get_fields().begin(),
                           struct_type->get_fields().end(),
                           [&](ckx_struct_type::field &field) {
                               return field.name == _extract_expr->field_name;
                           });
    if (it == struct_type->get_fields().end())
    {
        error();
        return saber::optional<ckx_expr_result>();
    }

    qsizet index = it - struct_type->get_fields().begin();

    faker::llvm_value *llvm_value = builder.create_temporary_var();
    if (base_result.get().categ == ckx_expr_result::value_category::lvalue)
    {
        builder.create_getelementptr2(
            llvm_value,
            ckx_llvm_type_builder::build(struct_type),
            base_result.get().llvm_value_bind,
            ckx_llvm_type_builder::build(ckx_type_helper::get_vi32_type()),
            builder.create_unsigned_constant(0),
            ckx_llvm_type_builder::build(ckx_type_helper::get_vi32_type()),
            builder.create_unsigned_constant(index));
        return saber::optional<ckx_expr_result>(
            it->type, ckx_expr_result::value_category::lvalue, llvm_value);
    }
    /// @todo add xvalue resolution if it gets introduced
    else /*
    if (base_result.get().categ == ckx_expr_result::value_category::prvalue) */
    {
        builder.create_extractvalue(llvm_value,
                                    ckx_llvm_type_builder::build(struct_type),
                                    base_result.get().llvm_value_bind,
                                    builder.create_unsigned_constant(index));
        return saber::optional<ckx_expr_result>(
            it->type, ckx_expr_result::value_category::prvalue, llvm_value);
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_id_expr(ckx_ast_id_expr *_id_expr)
{
    ckx_env_var_entry* entry = current_env->lookup_var(_id_expr->name);
    if (entry != nullptr)
        return saber::optional<ckx_expr_result>(
            entry->type, ckx_expr_result::value_category::lvalue,
            entry->llvm_value_bind);
    else
        return saber::optional<ckx_expr_result>();
}

ckx_expr_result
ckx_sema_engine::visit_vi_literal_node(ckx_ast_vi_literal_expr *_literal_expr)
{
    qint64 expr_value = _literal_expr->val;
    ckx_type *expr_type = nullptr;

    if (expr_value >= std::numeric_limits<qint8>::min()
        && expr_value <= std::numeric_limits<qint8>::max())
        expr_type = ckx_type_helper::get_vi8_type();
    else if (expr_value >= std::numeric_limits<qint16>::min()
             && expr_value <= std::numeric_limits<qint16>::max())
        expr_type = ckx_type_helper::get_vi16_type();
    else if (expr_value >= std::numeric_limits<qint32>::min()
             && expr_value <= std::numeric_limits<qint32>::max())
        expr_type = ckx_type_helper::get_vi32_type();
    else /* if (expr_value >= std::numeric_limits<qint64>::min()
             && expr_value <= std::numeric_limits<qint64>::max()) */
        expr_type = ckx_type_helper::get_vi64_type();

    return ckx_expr_result(
        expr_type, ckx_expr_result::value_category::prvalue,
        builder.create_signed_constant(expr_value));
}

ckx_expr_result
ckx_sema_engine::visit_vr_literal_node(ckx_ast_vr_literal_expr *_literal_expr)
{
    qreal expr_value = _literal_expr->val;
    /// @todo this is temporary
    ckx_type *const expr_type = ckx_type_helper::get_vr64_type();

    return ckx_expr_result(
        expr_type, ckx_expr_result::value_category::prvalue,
        builder.create_floating_constant(expr_value));
}


void
ckx_sema_engine::visit_global_decl(ckx_ast_decl_stmt *_decl_stmt)
{
    C8ASSERT(!is_in_func());
    error(); /// @todo currently I don't know how to implement it.
}

void ckx_sema_engine::visit_local_decl(ckx_ast_decl_stmt *_decl_stmt)
{
    C8ASSERT(is_in_func());
    ckx_type *decl_type = re_lex_type(_decl_stmt->type);
    if (decl_type == nullptr)
    {
        error();
        return;
    }

    for (const ckx_ast_decl_stmt::init_decl &decl : _decl_stmt->decls)
    {
        saber::result<ckx_env_var_entry*, ckx_env::err_add_var> add_result =
            current_env->add_var(decl.rng, decl.name, decl_type);
        if (add_result.is_err())
        {
            error();
            break;
        }
        else
        {
            saber_string_view mangled_name =
                saber_string_pool::create_view(
                    saber::string_paste(decl.name, var_name_mangle_count()));
            faker::llvm_value* llvm_value_bind =
                builder.create_named_var(mangled_name);
            builder.create_alloca(llvm_value_bind,
                                  ckx_llvm_type_builder::build(decl_type), 1);
            add_result.value()->llvm_value_bind = llvm_value_bind;
        }

        if (decl.init != nullptr)
        {
            saber::optional<ckx_expr_result> init_expr_result =
                decl.init->accept(*this);
            if (!init_expr_result.is_type())
            {
                error();
                continue;
            }

            saber::optional<ckx_expr_result> casted_result =
                try_implicit_cast(
                    decay_to_rvalue(init_expr_result.get()), decl_type);

            if (!casted_result.is_type())
            {
                error();
                continue;
            }

            builder.create_store(ckx_llvm_type_builder::build(decl_type),
                                 casted_result.get().llvm_value_bind,
                                 add_result.value()->llvm_value_bind);
        }
    }
}

void ckx_sema_engine::visit_struct_decl(ckx_ast_record_stmt *_struct_stmt)
{
    C8ASSERT(_struct_stmt->tag == ckx_ast_record_stmt::record_tag::rt_struct);

    ckx_struct_type *struct_type =
        ckx_type_helper::create_struct_type(_struct_stmt->name);
    ckx_env_type_entry *entry =
        root_env.add_type(
            _struct_stmt->kwd_rng, _struct_stmt->name, struct_type).value();
    saber_string_view llvm_type_name =
        ckx_llvm_type_builder::full_name_for_struct(_struct_stmt->name);
    entry->llvm_type_bind =
        ckx_llvm_type_builder::build(struct_type);

    saber::vector<faker::llvm_type> llvm_type_fields;

    for (ckx_ast_record_stmt::field_row& row : _struct_stmt->fields)
    {
        ckx_type *row_decl_type = re_lex_type(row.type);
        if (row_decl_type == nullptr)
        {
            error();
            break;
        }

        faker::llvm_type llvm_type_bind =
            ckx_llvm_type_builder::build(row_decl_type);

        for (ckx_ast_record_stmt::field& field : row.fields)
        {
            ckx_struct_type::add_status add_status =
                struct_type->add_field(field.name, row_decl_type);
            if (add_status != ckx_struct_type::add_status::add_success)
            {
                error();
                continue;
            }
            llvm_type_fields.push_back(llvm_type_bind);
        }
    }

    builder.create_udt(llvm_type_name, saber::move(llvm_type_fields));
}

void ckx_sema_engine::visit_variant_decl(ckx_ast_record_stmt *_variant_stmt)
{
    C8ASSERT(_variant_stmt->tag == ckx_ast_record_stmt::record_tag::rt_variant);
    /// @todo this feature may be discarded in the future.
    /// And now I don't know how to implement
    /// @todo How can we calculate size of types correctly?
    error();
}

void ckx_sema_engine::visit_func_decl(ckx_ast_func_stmt *_func_stmt)
{
    saber::optional<function_header_info> func_header_info =
        visit_function_header(_func_stmt);

    if (!func_header_info.is_type())
        return;

    ckx_env::result_add_func entry =
        current_env->add_func(_func_stmt->kwd_rng, _func_stmt->name,
                              func_header_info.get().func_type);

    if (entry.status == ckx_env::result_add_func::add_status::conflict
        || entry.status == ckx_env::result_add_func::add_status::fail)
    {
        error();
        return;
    }

    ckx_func_type *func_type = func_header_info.get().func_type;

    saber::vector<faker::llvm_type> param_llvm_types;
    for (ckx_type *param_type : func_type->get_param_type_list())
        param_llvm_types.push_back(ckx_llvm_type_builder::build(param_type));

    builder.create_func_decl(
        ckx_llvm_type_builder::build(func_type->get_return_type()),
        entry.added_or_conflict_func->llvm_name,
        saber::move(param_llvm_types),
        saber::move(func_header_info.get().param_names),
        faker::llvm_func_attrs(true, faker::llvm_func_attrs::it_default));
}

void ckx_sema_engine::visit_func_def(ckx_ast_func_stmt *_func_stmt)
{
    saber::optional<function_header_info> func_header_info =
        visit_function_header(_func_stmt);

    if (!func_header_info.is_type())
        return;

    ckx_env::result_add_func entry =
        current_env->add_func(_func_stmt->kwd_rng, _func_stmt->name,
                              func_header_info.get().func_type);

    if (entry.status == ckx_env::result_add_func::add_status::conflict
        || entry.status == ckx_env::result_add_func::add_status::fail)
    {
        error();
        return;
    }

    if (entry.status == ckx_env::result_add_func::add_status::redeclare
        && entry.added_or_conflict_func->defined)
    {
        error();
        return;
    }

    ckx_func_type *func_type = func_header_info.get().func_type;

    saber::vector<faker::llvm_type> param_llvm_types;
    for (ckx_type *param_type : func_type->get_param_type_list())
        param_llvm_types.push_back(ckx_llvm_type_builder::build(param_type));

    entry.added_or_conflict_func->defined = true;

    /// @todo FIXME this part cannot be managed by RAII
    builder.create_n_enter_func(
        ckx_llvm_type_builder::build(func_type->get_return_type()),
        entry.added_or_conflict_func->llvm_name,
        saber::move(param_llvm_types), func_header_info.get().param_names,
        faker::llvm_func_attrs(true, faker::llvm_func_attrs::it_default));

    enter_func_protection_raii raii(*this, func_type);
    for (size_t i = 0; i < _func_stmt->param_decls.size(); ++i)
    {
        faker::llvm_value *llvm_value =
            builder.create_named_var(
                saber_string_pool::create_view(
                    saber::string_paste(
                        func_header_info.get().param_names[i],
                        vname_mangle_count)));

        faker::llvm_value *param_llvm_value =
            builder.create_named_var(func_header_info.get().param_names[i]);

        faker::llvm_type param_llvm_type =
            ckx_llvm_type_builder::build(func_type->get_param_type_list()[i]);

        builder.create_alloca(llvm_value, param_llvm_type, 1);
        builder.create_store(param_llvm_type, param_llvm_value, llvm_value);

        saber::result<ckx_env_var_entry*, ckx_env::err_add_var> result =
        current_env->add_var(_func_stmt->param_decls[i].rng,
                             _func_stmt->param_decls[i].name,
                             func_type->get_param_type_list()[i]);
        result.value()->llvm_value_bind = llvm_value;
    }
    _func_stmt->fnbody->accept(*this);
    builder.leave_func();
}

quint64 ckx_sema_engine::calculate_disagreements(
            const saber::vector<ckx_expr_result> &_args,
            const saber::vector<ckx_type *> &_params)
{
    if (_args.size() != _params.size())
        return std::numeric_limits<quint64>::max();

    quint64 disagreements = 0;
    for (quint64 i = 0; i < _args.size(); ++i)
    {
        if (_args[i].type->equal_to(_params[i]))
            continue;
        else if (ckx_type_helper::can_implicit_cast(_args[i].type, _params[i]))
            disagreements++;
        else
            return std::numeric_limits<quint64>::max();
    }
    return disagreements;
}

saber::optional<ckx_sema_engine::function_header_info>
ckx_sema_engine::visit_function_header(ckx_ast_func_stmt *_func_stmt)
{
    ckx_type* ret_type = re_lex_type(_func_stmt->ret_type);
    if (ret_type == nullptr)
    {
        error();
        return saber::optional<function_header_info>();
    }

    saber::vector<ckx_type*> param_type_results;
    saber::vector<saber_string_view> param_names;

    for (ckx_ast_func_stmt::param_decl &param_decl : _func_stmt->param_decls)
    {
        ckx_type* param_type = re_lex_type(param_decl.type);
        if (param_type == nullptr)
        {
            error();
            break;
        }

        param_type_results.push_back(param_type);
        param_names.push_back(param_decl.name);
    }

    ckx_func_type *func_type =
        ckx_type_helper::create_func_type(
            ret_type, saber::move(param_type_results));

    return saber::optional<function_header_info>(
                func_type, saber::move(param_names));
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_assign(ckx_expr_result _dest, ckx_expr_result _from)
{
    if (_dest.categ != ckx_expr_result::value_category::lvalue)
    {
        error();
        return saber::optional<ckx_expr_result>();
    }

    saber::optional<ckx_expr_result> casted_result =
        try_implicit_cast(decay_to_rvalue(_from), _dest.type);
    if (!casted_result.is_type())
    {
        error();
        return saber::optional<ckx_expr_result>();
    }
    else
    {
        builder.create_store(
            ckx_llvm_type_builder::build(casted_result.get().type),
            casted_result.get().llvm_value_bind,
            _dest.llvm_value_bind);
        return saber::optional<ckx_expr_result>(_dest);
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_add(ckx_expr_result _expr1, ckx_expr_result _expr2)
{
    if (_expr1.type->is_numeric() && _expr2.type->is_numeric())
    {
        return visit_numeric_calc(_expr1, _expr2,
                                  &faker::llvm_ir_builder::create_fadd,
                                  &faker::llvm_ir_builder::create_add);
    }
    else if ( (_expr1.type->is_pointer() || _expr2.type->is_pointer())
              && (_expr1.type->is_integral() || _expr2.type->is_integral()) )
    {
        return visit_ptroffset(_expr1, _expr2, true);
    }
    else
    {
        error();
        return saber::optional<ckx_expr_result>();
    }
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_sub(ckx_expr_result _expr1, ckx_expr_result _expr2)
{

}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_ptroffset(ckx_expr_result _expr1, ckx_expr_result _expr2,
                                 bool _add)
{
    C8ASSERT((_expr1.type->is_pointer() || _expr2.type->is_pointer())
             && (_expr1.type->is_integral() || _expr2.type->is_integral()));
    saber::pair<ckx_expr_result, ckx_expr_result> casted_results = [&]() {
        if (_expr1.type->is_pointer())
            return make_pair(decay_to_rvalue(_expr1),
                             decay_to_rvalue(_expr2));
        else
            return make_pair(decay_to_rvalue(_expr2),
                             decay_to_rvalue(_expr1));
    }();

    ckx_pointer_type *pointer =
        static_cast<ckx_pointer_type*>(casted_results.first.type);

    faker::llvm_value *offset_value = _add ?
        casted_results.second.llvm_value_bind :
        [&] {
            faker::llvm_value *ret = builder.create_temporary_var();
            faker::llvm_value *zero = builder.create_unsigned_constant(0);
            builder.create_sub(
                ret, ckx_llvm_type_builder::build(casted_results.second.type),
                zero, casted_results.second.llvm_value_bind);
            return ret;
        }();

    faker::llvm_value *llvm_value = builder.create_temporary_var();
    builder.create_getelementptr(
        llvm_value,
        ckx_llvm_type_builder::build(pointer->get_pointee()),
        casted_results.first.llvm_value_bind,
        ckx_llvm_type_builder::build(casted_results.second.type),
        offset_value);
    return saber::optional<ckx_expr_result>(
        casted_results.first.type,
        ckx_expr_result::value_category::prvalue, llvm_value);
}

template <typename ActionOnInt, typename ActionOnFloat>
saber::optional<ckx_expr_result>
ckx_sema_engine::visit_numeric_calc(ckx_expr_result _expr1,
                                    ckx_expr_result _expr2,
                                    ActionOnInt&& _action_on_int,
                                    ActionOnFloat&& _action_on_float)
{
    C8ASSERT(_expr1.type->is_numeric() && _expr2.type->is_numeric());
    ckx_type *common_type = ckx_type_helper::common_numeric_type(_expr1.type,
                                                                 _expr2.type);

    if (common_type == nullptr)
    {
        error();
        return saber::optional<ckx_expr_result>();
    }

    ckx_expr_result casted_result1 =
        try_implicit_cast(decay_to_rvalue(_expr1), common_type).get();
    ckx_expr_result casted_result2 =
        try_implicit_cast(decay_to_rvalue(_expr2), common_type).get();

    faker::llvm_value *llvm_value = builder.create_temporary_var();
    if (common_type->is_floating())
        ((builder).*(_action_on_float))
            (llvm_value, ckx_llvm_type_builder::build(common_type),
             casted_result1.llvm_value_bind, casted_result2.llvm_value_bind);
    else
        ((builder).*(_action_on_int))
            (llvm_value, ckx_llvm_type_builder::build(common_type),
             casted_result1.llvm_value_bind, casted_result2.llvm_value_bind);

    return saber::optional<ckx_expr_result>(
       common_type, ckx_expr_result::value_category::prvalue, llvm_value);
}



saber::optional<ckx_expr_result>
ckx_sema_engine::visit_addressof_expr(ckx_ast_unary_expr *_unary_expr)
{
    saber::optional<ckx_expr_result> operand_result =
        _unary_expr->operand->accept(*this);
    if (!operand_result.is_type()) return saber::optional<ckx_expr_result>();

    if (operand_result.get().categ != ckx_expr_result::value_category::lvalue)
    {
        error();
        return saber::optional<ckx_expr_result>();
    }

    /// FIXME This is a bogus hack, ask me if you don't comprehend.
    /// And give me some advice if you have better solutions.
    return saber::optional<ckx_expr_result>(
        ckx_type_helper::pointer_to(operand_result.get().type),
        ckx_expr_result::value_category::prvalue,
        operand_result.get().llvm_value_bind);
}

saber::optional<ckx_expr_result>
ckx_sema_engine::visit_deref_expr(ckx_ast_unary_expr *_unary_expr)
{
    saber::optional<ckx_expr_result> operand_result =
        _unary_expr->operand->accept(*this);
    if (!operand_result.is_type()) return saber::optional<ckx_expr_result>();

    if (!operand_result.get().type->is_pointer())
    {
        error();
        return saber::optional<ckx_expr_result>();
    }

    ckx_pointer_type *pointer =
        static_cast<ckx_pointer_type*>(operand_result.get().type);

    ckx_expr_result decayed_result = decay_to_rvalue(operand_result.get());

    /// FIXME This is a bogus hack, ask me if you don't comprehend.
    /// And give me some advice if you have better solutions.
    return saber::optional<ckx_expr_result>(
        pointer->get_pointee(),
        ckx_expr_result::value_category::lvalue,
        decayed_result.llvm_value_bind);
}

ckx_type* ckx_sema_engine::re_lex_type(const ckx_prelexed_type& _prelexed_type)
{
    const saber::vector<ckx_token> &prelexed_tokens =
        _prelexed_type.get_prelexed_tokens();
    C8ASSERT(!prelexed_tokens.empty()); // What the fuck!

    ckx_type *type = nullptr;
    if (prelexed_tokens.front().token_type >= ckx_token::type::tk_vi8
        && prelexed_tokens.front().token_type <= ckx_token::type::tk_void)
    {
        type = ckx_type_helper::get_type_by_token(
                   prelexed_tokens.front().token_type);
    }
    else if (prelexed_tokens.front().token_type == ckx_token::type::tk_id)
    {
        ckx_env_type_entry *entry =
            current_env->lookup_type(prelexed_tokens.front().str);
        if (entry) type = entry->type;
        else return nullptr;
    }
    else
    {
        C8ASSERT(false); // What the fuck!
    }

    for (auto it = prelexed_tokens.cbegin()+1;
         it != prelexed_tokens.cend(); ++it)
    {
        switch (it->token_type)
        {
        case ckx_token::type::tk_const:
            type = ckx_type_helper::qual_const(type);
            break;
        case ckx_token::type::tk_mul:
            type = ckx_type_helper::pointer_to(type);
            break;
        default:
            C8ASSERT(false);
        }
    }

    return type;
}

bool ckx_sema_engine::is_in_func()
{
    return context_manager.lookup_func_context() != nullptr;
}


void ckx_sema_engine::enter_scope()
{
    current_env = new ckx_env(current_env);
}

void ckx_sema_engine::leave_scope()
{
    C8ASSERT(current_env != &root_env);
    ckx_env *parent_env = current_env->get_parent();
    delete current_env;
    current_env = parent_env;
}

ckx_sema_engine::
enter_func_protection_raii::enter_func_protection_raii(ckx_sema_engine &_sema,
                                                    ckx_func_type *_func_type) :
    sema(_sema)
{
    sema.context_manager.enter_func_context(_func_type);
    sema.enter_scope();
}

ckx_sema_engine::
enter_func_protection_raii::~enter_func_protection_raii()
{
    C8ASSERT(sema.is_in_func());
    sema.context_manager.exit_func_context();
    sema.leave_scope();
    sema.vname_mangle_count = 0;
}

} // namespace ckx
