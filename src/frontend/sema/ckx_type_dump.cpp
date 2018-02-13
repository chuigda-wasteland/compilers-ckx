#include "frontend/sema/ckx_type.hpp"

namespace ckx
{

saber_string ckx_type::qual_to_string() const
{
    saber_string ret;
    if (qual & qual_const) ret += " const";
    if (qual & qual_volatile) ret += " volatile";
    if (qual & qual_restrict) ret += " restrict";
    return ret;
}

saber_string
ckx_basic_type::to_string() const
{
    struct category_hash
    {
        bool operator() (category _ctg) const
        {
            return std::hash<qchar>()(static_cast<qchar>(_ctg));
        }
    };

    static saber::unordered_map<category, saber_string, category_hash>
            typename_string_list
    {
        {category::type_vi8,  "vi8"},
        {category::type_vi16, "vi16"},
        {category::type_vi32, "vi32"},
        {category::type_vi64, "vi64"},
        {category::type_vu8,  "vu8"},
        {category::type_vu16, "vu16"},
        {category::type_vu32, "vu32"},
        {category::type_vu64, "vu64"},
        {category::type_vr32, "vr32"},
        {category::type_vr64, "vr64"},
        {category::type_vch,  "vch"},
        {category::type_vnullptr_t, "vnptr_t"},
        {category::type_vbool, "vb1"},
        {category::type_void, "void"}
    };

    return typename_string_list.find(type_category)->second + qual_to_string();
}

saber_string
ckx_struct_type::to_string() const
{
    return "StructType[[" + struct_name.get() + "]]" + qual_to_string();
}

saber_string
ckx_variant_type::to_string() const
{
    return "VariantType[[" + variant_name.get() + "]]" + qual_to_string();
}

saber_string
ckx_enum_type::to_string() const
{
    return "EnumType[[" + enum_name.get() + "]]" + qual_to_string();
}

saber_string
ckx_func_type::to_string() const
{
    saber_string ret = "fn (";
    for (const ckx_type* type : param_type_list)
        ret += type->to_string() + saber_string(",");
    ret += saber_string(") -> ");
    ret += return_type->to_string();
    return ret  + qual_to_string();
}

saber_string
ckx_pointer_type::to_string() const
{
    return target->to_string() + saber_string(" *")  + qual_to_string();
}

saber_string
ckx_array_type::to_string() const
{
    return element_type->to_string() + "[]"  + qual_to_string();
}

saber_string ckx_type_alias::to_string() const
{
    return "AliasTo[[" + origin->to_string() + "]]" + qual_to_string();
}

} // namespace ckx
