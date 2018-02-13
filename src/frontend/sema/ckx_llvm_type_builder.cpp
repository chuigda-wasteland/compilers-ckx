#include "frontend/sema/ckx_llvm_type_builder.hpp"

namespace ckx
{

faker::llvm_type
ckx_llvm_type_builder::build(ckx_type *_type)
{
    static saber_string_view i8str = saber_string_pool::create_view("i8");
    static saber_string_view i16str = saber_string_pool::create_view("i16");
    static saber_string_view i32str = saber_string_pool::create_view("i32");
    static saber_string_view i64str = saber_string_pool::create_view("i64");
    static saber_string_view fstr = saber_string_pool::create_view("float");
    static saber_string_view dstr = saber_string_pool::create_view("double");
    static saber_string_view nullstr = saber_string_pool::create_view("i8*");
    static saber_string_view i1str = saber_string_pool::create_view("i1");
    static saber_string_view voidstr = saber_string_pool::create_view("void");

    switch (_type->get_category())
    {
    case ckx_type::category::type_vi8:
    case ckx_type::category::type_vu8:
        return i8str;

    case ckx_type::category::type_vi16:
    case ckx_type::category::type_vu16:
        return i16str;

    case ckx_type::category::type_vi32:
    case ckx_type::category::type_vu32:
        return i32str;

    case ckx_type::category::type_vi64:
    case ckx_type::category::type_vu64:
        return i64str;

    /// case ckx_type::category::type_vch: @note not implemented

    case ckx_type::category::type_vr32:
        return fstr;

    case ckx_type::category::type_vr64:
        return dstr;

    case ckx_type::category::type_vnullptr_t:
        return nullstr;

    case ckx_type::category::type_vbool:
        return i1str;

    case ckx_type::category::type_void:
        return voidstr;

    case ckx_type::category::type_function:
        {
            ckx_func_type *func_type = static_cast<ckx_func_type*>(_type);
            saber_string ret = build(func_type->get_return_type()).get() + "(";
            for (int i = 0; i < func_type->get_param_type_list().size(); ++i)
            {
                ret += build(func_type->get_param_type_list()[i]).get();
                if (i != func_type->get_param_type_list().size()-1)
                    ret += ',';
            }
            ret += ")";
            return saber_string_pool::create_view(saber::move(ret));
        }

    case ckx_type::category::type_struct:
        {
            ckx_struct_type *struct_type = static_cast<ckx_struct_type*>(_type);
            return saber_string_pool::create_view(
                saber::string_paste(
                    "%", full_name_for_struct(struct_type->get_name())));
        }

    case ckx_type::category::type_variant:
        {
            ckx_variant_type *variant_type =
                static_cast<ckx_variant_type*>(_type);
            return saber_string_pool::create_view(
                saber::string_paste(
                    "%", full_name_for_variant(variant_type->get_name())));
        }

    case ckx_type::category::type_enum:
            return i64str;

    case ckx_type::category::type_pointer:
        {
            ckx_pointer_type *ptr_type = static_cast<ckx_pointer_type*>(_type);
            return saber_string_pool::create_view(
                saber::string_paste(build(ptr_type->get_pointee()), "*"));
        }

    /// case ckx_type::category::type_array: @note deprecated and to be removed

    case ckx_type::category::type_alias:
        {
            ckx_type_alias *alias = static_cast<ckx_type_alias*>(_type);
            return build(alias->get_aliasee());
        }

    default:
        C8ASSERT(false);
    }
}

saber_string_view
ckx_llvm_type_builder::full_name_for_struct(saber_string_view _origin)
{
    return saber_string_pool::create_view(
        saber::string_paste("struct.", _origin));
}

saber_string_view
ckx_llvm_type_builder::full_name_for_variant(saber_string_view _origin)
{
    return saber_string_pool::create_view(
        saber::string_paste("union.", _origin));
}

} // namespace ckx
