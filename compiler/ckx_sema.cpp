#include "ckx_sema.hpp"

namespace ckx
{

void ckx_sema_engine::visit_decl_node(ckx_ast_decl_stmt* _decl_stmt)
{
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
                is_in_func() ?
                saber_string_pool::create_view(
                    saber::string_paste(decl.name, var_name_mangle_count())) :
                decl.name;
            faker::llvm_value* llvm_value_bind =
                builder->create_named_var(mangled_name);
            builder->create_alloca(llvm_value_bind,
                                   result.get().llvm_type_bind, 1);
            add_result.value()->llvm_value_bind = llvm_value_bind;
        }
    }
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
    case ckx_type::category::type_vr32: return "f32";
    case ckx_type::category::type_vr64: return "f64";
    case ckx_type::category::type_void: return "void";

    case ckx_type::category::type_vch: // I don't know how to implement this
    default:
        C8ASSERT(false);
    }
}

} // namespace ckx
