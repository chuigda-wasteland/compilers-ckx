#include "frontend/sema/ckx_sema.hpp"

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

void
ckx_sema_engine::visit_decl_node(ckx_ast_decl_stmt* _decl_stmt)
{
    if (is_in_func())
        visit_local_decl(_decl_stmt);
    else
        visit_global_decl(_decl_stmt);
}

void ckx_sema_engine::visit_record_node(ckx_ast_record_stmt *_record_stmt)
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

void ckx_sema_engine::visit_func_node(ckx_ast_func_stmt *_func_stmt)
{
    if (_func_stmt->fnbody == nullptr)
        visit_func_decl(_func_stmt);
    else
        visit_func_def(_func_stmt);
}

void ckx_sema_engine::test_print(we::we_file_writer &writer)
{
    builder.pretty_print(writer);
}

void ckx_sema_engine::test_enter_func()
{
    enter_func();
}

void ckx_sema_engine::test_leave_func()
{
    leave_func();
}

void ckx_sema_engine::test_enter_scope()
{
    enter_scope();
}

void ckx_sema_engine::test_leave_scope()
{
    leave_scope();
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
    saber::optional<ckx_type_result> result = re_lex_type(_decl_stmt->type);
    if (!result.is_type())
    {
        error();
        return;
    }

    for (const ckx_ast_decl_stmt::init_decl &decl : _decl_stmt->decls)
    {
        saber::result<ckx_env_var_entry*, ckx_env::err_add_var> add_result =
            current_env->add_var(decl.rng, decl.name, result.get().type);
        if (add_result.is_err())
        {
            error();
        }
        else
        {
            saber_string_view mangled_name =
                saber_string_pool::create_view(
                    saber::string_paste(decl.name, var_name_mangle_count()));
            faker::llvm_value* llvm_value_bind =
                builder.create_named_var(mangled_name);
            builder.create_alloca(llvm_value_bind,
                                   result.get().llvm_type_bind, 1);
            add_result.value()->llvm_value_bind = llvm_value_bind;
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
        saber_string_pool::create_view(
            saber::string_paste("struct.", _struct_stmt->name));
    entry->llvm_type_bind =
        saber_string_pool::create_view(
            saber::string_paste("%", llvm_type_name));

    saber::vector<faker::llvm_type> llvm_type_fields;

    for (ckx_ast_record_stmt::field_row& row : _struct_stmt->fields)
    {
        saber::optional<ckx_type_result> result = re_lex_type(row.type);
        if (!result.is_type())
        {
            error();
            break;
        }

        for (ckx_ast_record_stmt::field& field : row.fields)
        {
            struct_type->add_field(field.name, result.get().type);
            llvm_type_fields.push_back(result.get().llvm_type_bind);
        }
    }

    builder.create_udt(llvm_type_name, saber::move(llvm_type_fields));
}

void ckx_sema_engine::visit_variant_decl(ckx_ast_record_stmt *_variant_stmt)
{
    C8ASSERT(_variant_stmt->tag == ckx_ast_record_stmt::record_tag::rt_variant);
    /// @todo this feature will be discarded in the future.
    /// And now I don't know how to implement
    /// @todo How can we calculate size of types correctly?
    error();
    /*
    ckx_variant_type *variant_type =
        ckx_type_helper::create_variant_type(_variant_stmt->name);
    ckx_env_type_entry *entry =
        root_env.add_type(
            _variant_stmt->kwd_rng, _variant_stmt->name, variant_type).value();

    for (ckx_ast_record_stmt::field_row& row : _variant_stmt->fields)
    {
        saber::optional<ckx_type_result> result = re_lex_type(row.type);
        if (!result.is_type())
        {
            error();
            break;
        }

        for (ckx_ast_record_stmt::field& field : row.fields)
            variant_type->add_field(field.name, result.get().type);
    }
    */
}

void ckx_sema_engine::visit_func_decl(ckx_ast_func_stmt *_func_stmt)
{
    saber::optional<function_header_info> func_header_info =
        visit_function_header(_func_stmt);

    if (!func_header_info.is_type())
        return;

    saber::vector<ckx_type*> param_types;
    saber::vector<faker::llvm_type> param_llvm_types;

    for (ckx_type_result& result : func_header_info.get().param_types_results)
    {
        param_types.push_back(result.type);
        param_llvm_types.push_back(result.llvm_type_bind);
    }

    ckx_func_type *func_type =
        ckx_type_helper::create_func_type(
            func_header_info.get().ret_type_result.type,
            saber::move(param_types));

    ckx_env::result_add_func entry =
        current_env->add_func(_func_stmt->kwd_rng, _func_stmt->name, func_type);

    if (entry.status == ckx_env::result_add_func::add_status::conflict
        || entry.status == ckx_env::result_add_func::add_status::conflict)
    {
        error();
        return;
    }

    builder.create_func_decl(
        func_header_info.get().ret_type_result.llvm_type_bind,
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

    saber::vector<ckx_type*> param_types;
    saber::vector<faker::llvm_type> param_llvm_types;

    for (ckx_type_result& result : func_header_info.get().param_types_results)
    {
        param_types.push_back(result.type);
        param_llvm_types.push_back(result.llvm_type_bind);
    }

    saber::vector<ckx_type*> param_types_1 = param_types;
    saber::vector<saber_string_view> param_names_1 =
        func_header_info.get().param_names;

    ckx_func_type *func_type =
        ckx_type_helper::create_func_type(
            func_header_info.get().ret_type_result.type,
            saber::move(param_types_1));

    ckx_env::result_add_func entry =
        current_env->add_func(_func_stmt->kwd_rng, _func_stmt->name, func_type);

    if (entry.status == ckx_env::result_add_func::add_status::conflict
        || entry.status == ckx_env::result_add_func::add_status::conflict)
    {
        error();
        return;
    }

    builder.create_n_enter_func(
        func_header_info.get().ret_type_result.llvm_type_bind,
        entry.added_or_conflict_func->llvm_name,
        saber::move(param_llvm_types), saber::move(param_names_1),
        faker::llvm_func_attrs(true, faker::llvm_func_attrs::it_default));

    enter_func();
    for (size_t i = 0; i < _func_stmt->param_decls.size(); ++i)
    {
        saber::result<ckx_env_var_entry*, ckx_env::err_add_var> result =
        current_env->add_var(_func_stmt->param_decls[i].rng,
                             _func_stmt->param_decls[i].name,
                             param_types[i]);
        result.value()->llvm_value_bind =
            builder.create_named_var(func_header_info.get().param_names[i]);
    }
    _func_stmt->fnbody->accept(*this);
    leave_func();
}

saber::optional<ckx_sema_engine::function_header_info>
ckx_sema_engine::visit_function_header(ckx_ast_func_stmt *_func_stmt)
{
    saber::optional<ckx_type_result> ret_type_result =
        re_lex_type(_func_stmt->ret_type);
    if (!ret_type_result.is_type())
    {
        error();
        return saber::optional<function_header_info>();
    }

    saber::vector<ckx_type_result> param_type_results;
    saber::vector<saber_string_view> param_names;

    for (ckx_ast_func_stmt::param_decl &param_decl : _func_stmt->param_decls)
    {
        saber::optional<ckx_type_result> param_type_result =
            re_lex_type(param_decl.type);
        if (!param_type_result.is_type())
        {
            error();
            break;
        }

        param_type_results.push_back(param_type_result.get());
        param_names.push_back(param_decl.name);
    }

    return saber::optional<function_header_info>(
        ret_type_result.get(),
        saber::move(param_type_results),
        saber::move(param_names));
}

saber::optional<ckx_type_result>
ckx_sema_engine::re_lex_type(const ckx_prelexed_type& _prelexed_type)
{
    const saber::vector<ckx_token> &prelexed_tokens =
        _prelexed_type.get_prelexed_tokens();

    C8ASSERT(!prelexed_tokens.empty()); // What the fuck!

    ckx_type *type = nullptr;
    saber_string llvm_type_string = "";

    if (prelexed_tokens.front().token_type >= ckx_token::type::tk_vi8
        && prelexed_tokens.front().token_type <= ckx_token::type::tk_void)
    {
        type = ckx_type_helper::get_type(prelexed_tokens.front().token_type);
        llvm_type_string = map_basic_type(type->get_category());
    }
    else if (prelexed_tokens.front().token_type == ckx_token::type::tk_id)
    {
        ckx_env_type_entry *entry =
            current_env->lookup_type(prelexed_tokens.front().str);
        if (entry)
        {
            type = entry->type;
            llvm_type_string = entry->llvm_type_bind.get();
        }
        else return saber::optional<ckx_type_result>();
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
            llvm_type_string += '*';
            break;
        default:
            C8ASSERT(false);
        }
    }

    return saber::optional<ckx_type_result>(
        type, saber_string_pool::create_view(llvm_type_string));
}

saber_string
ckx_sema_engine::map_basic_type(ckx_type::category _basic_type)
{
    switch (_basic_type)
    {
    case ckx_type::category::type_vi8:  return "i8";
    case ckx_type::category::type_vi16: return "i16";
    case ckx_type::category::type_vi32: return "i32";
    case ckx_type::category::type_vi64: return "i64";
    case ckx_type::category::type_vu8:  return "i8";
    case ckx_type::category::type_vu16: return "i16";
    case ckx_type::category::type_vu32: return "i32";
    case ckx_type::category::type_vu64: return "i64";
    case ckx_type::category::type_vr32: return "float";
    case ckx_type::category::type_vr64: return "double";
    case ckx_type::category::type_void: return "void";

    case ckx_type::category::type_vch:
    /// @todo I don't know how to implement this char type
    default:
        C8ASSERT(false);
    }
}

void ckx_sema_engine::enter_func()
{
    in_func = true;
    enter_scope();
}

void ckx_sema_engine::leave_func()
{
    C8ASSERT(in_func);
    in_func = false;
    leave_scope();
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

} // namespace ckx
