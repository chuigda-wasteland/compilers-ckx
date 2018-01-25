#include "ckx_func_name_mangler.hpp"
#include <atomic>

#include "defs.hpp"

namespace ckx
{

static saber_string qual2str(quint8 qual_bits)
{
    saber_string ret;
    if (qual_bits & ckx_type::qual_const) ret += "c";
    if (qual_bits & ckx_type::qual_volatile) ret += "v";
    if (qual_bits & ckx_type::qual_restrict) ret += "r";
    return ret;
}

static saber_string type2str(ckx_type *_type)
{
    saber_string qual_str = qual2str(_type->get_qual_bits());
    if (_type->is_basic())
    {
        saber_string str;
        switch (_type->get_category())
        {
        case ckx_type::category::type_vi8:   str = "b"; break;
        case ckx_type::category::type_vi16:  str = "s"; break;
        case ckx_type::category::type_vi32:  str = "i"; break;
        case ckx_type::category::type_vi64:  str = "l"; break;
        case ckx_type::category::type_vu8:   str = "B"; break;
        case ckx_type::category::type_vu16:  str = "S"; break;
        case ckx_type::category::type_vu32:  str = "U"; break;
        case ckx_type::category::type_vu64:  str = "L"; break;
        case ckx_type::category::type_vch:   str = "K"; break;
        case ckx_type::category::type_vr32:  str = "F"; break;
        case ckx_type::category::type_vr64:  str = "D"; break;
        case ckx_type::category::type_void:  str = "N"; break;
        default:
            C8ASSERT(false); // What the fuck!
        }
        return qual_str + str;
    }
    else if (_type->get_category() == ckx_type::category::type_struct)
    {
        ckx_struct_type *type = static_cast<ckx_struct_type*>(_type);
        return qual_str + "Q" + type->get_name().get();
    }
    else if (_type->get_category() == ckx_type::category::type_variant)
    {
        ckx_variant_type *type = static_cast<ckx_variant_type*>(_type);
        return qual_str + "q" + type->get_name().get();
    }
    else if (_type->get_category() == ckx_type::category::type_enum)
    {
        ckx_enum_type *type = static_cast<ckx_enum_type*>(_type);
        return qual_str + "E" + type->get_name().get();
    }
    else if (_type->get_category() == ckx_type::category::type_pointer)
    {
        ckx_pointer_type *type = static_cast<ckx_pointer_type*>(_type);
        return qual_str + "P" + type2str(type->get_pointee());
    }
    else if (_type->get_category() == ckx_type::category::type_alias)
    {
        ckx_type_alias *alias = static_cast<ckx_type_alias*>(_type);
        return qual_str + type2str(alias->get_aliasee());
    }
    else
    {
        C8ASSERT(false); // What the fuck!
    }
}

saber_string_view
ckx_func_name_mangler::std_mangle(saber_string_view _func_name,
                                  ckx_type *_func_type)
{
    C8ASSERT(_func_type->get_category() == ckx_type::category::type_function);
    ckx_func_type *func_type = reinterpret_cast<ckx_func_type*>(_func_type);
    saber_string func_appendix = "_";
    for (ckx_type* type : func_type->get_param_type_list())
        func_appendix += type2str(type);
    func_appendix += "_" + type2str(func_type->get_return_type());
    return saber_string_pool::create_view(
        saber::string_paste(_func_name, func_appendix));
}

saber_string_view
ckx_func_name_mangler::simple_mangle(saber_string_view _func_name)
{
    static std::atomic<quint64> mangle_count {0};
    return saber_string_pool::create_view(
        saber::string_paste(_func_name, mangle_count++));
}

saber_string_view
ckx_func_name_mangler::prime(saber_string_view _func_name)
{
    return _func_name;
}

};

