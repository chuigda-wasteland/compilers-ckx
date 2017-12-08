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

#include "ckx_type.hpp"
#include "unordered_map.hpp"

namespace ckx
{

ckx_type::ckx_type(category _category) :
    type_category(_category),
    qual(0)
{}

bool ckx_type::exact_equal_to(saber_ptr<ckx_type> _another) const
{
    return type_equal_to(_another)
           && get_qual_bits() == _another->get_qual_bits();
}

ckx_type::~ckx_type() {}

const ckx_type::category &ckx_type::get_category() const
{
    return type_category;
}

bool ckx_type::is_const() const
{
    return qual & qual_const;
}

bool ckx_type::is_volatile() const
{
    return qual & qual_volatile;
}

bool ckx_type::is_restrict() const
{
    return qual & qual_volatile;
}

void ckx_type::add_const()
{
    qual |= qual_const;
}

void ckx_type::add_volatile()
{
    qual |= qual_volatile;
}

void ckx_type::add_restrict()
{
    qual |= qual_restrict;
}

void ckx_type::remove_const()
{
    qual &=  ~qual_const;
}

void ckx_type::remove_volatile()
{
    qual &=  ~qual_volatile;
}

void ckx_type::remove_restrict()
{
    qual &=  ~qual_restrict;
}

unsigned char ckx_type::get_qual_bits() const
{
    return qual;
}

void ckx_type::from_qual_bits(unsigned char _qual_bits)
{
    qual = _qual_bits;
}

saber_string ckx_type::qual_to_string() const
{
    saber_string ret;
    if (qual & qual_const) ret += " const";
    if (qual & qual_volatile) ret += " volatile";
    if (qual & qual_restrict) ret += " restrict";
    return ret;
}


ckx_basic_type::ckx_basic_type(category _basic_category) :
    ckx_type(_basic_category)
{}


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
        {category::type_void, "void"}
    };

    return typename_string_list.find(type_category)->second + qual_to_string();
}

bool
ckx_basic_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    return (type_category == _another->get_category());
}



ckx_id_type::ckx_id_type(saber_string_view _name) :
    ckx_type(category::type_id),
    name(_name)
{}

saber_string
ckx_id_type::to_string() const
{
    return name.get();
}

bool
ckx_id_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() == ckx_type::category::type_id)
    {
        ckx_id_type *id_type = static_cast<ckx_id_type*>(_another.get());
        return (get_name() == id_type->get_name());
    }
    return false;
}

saber_string_view
ckx_id_type::get_name() const
{
    return name;
}



ckx_struct_type::ckx_struct_type(saber_string_view _struct_name) :
    ckx_type(ckx_type::category::type_struct),
    struct_name(_struct_name)
{}

bool
ckx_struct_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() == ckx_type::category::type_struct)
    {
        ckx_struct_type *struct_type =
                static_cast<ckx_struct_type*>(_another.get());
        return this->get_name() == struct_type->get_name();
    }
    return false;
}

saber_string
ckx_struct_type::to_string() const
{
    return "StructType[[" + struct_name.get() + "]]" + qual_to_string();
}

saber_string_view ckx_struct_type::get_name() const
{
    return struct_name;
}

ckx_struct_type::add_status
ckx_struct_type::add_field(saber_string_view _name, saber_ptr<ckx_type> _type)
{
    for (field& x : fields)
        if (x.name == _name)
            return add_status::add_duplicate;

    fields.emplace_back(saber::move(_name), _type);
    return add_status::add_success;
}



ckx_variant_type::ckx_variant_type(saber_string_view _variant_name) :
    ckx_type(ckx_type::category::type_variant),
    variant_name(_variant_name)
{}

bool
ckx_variant_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() == ckx_type::category::type_variant)
    {
        ckx_variant_type *variant_type =
                static_cast<ckx_variant_type*>(_another.get());
        return this->get_name() == variant_type->get_name();
    }
    return false;
}

saber_string
ckx_variant_type::to_string() const
{
    return "VariantType[[" + variant_name.get() + "]]" + qual_to_string();
}

saber_string_view ckx_variant_type::get_name() const
{
    return variant_name;
}

ckx_variant_type::add_status
ckx_variant_type::add_field(saber_string_view _name, saber_ptr<ckx_type> _type)
{
    for (field& x : fields)
        if (x.name == _name)
            return add_status::add_duplicate;

    fields.emplace_back(saber::move(_name), _type);
    return add_status::add_success;
}



ckx_enum_type::ckx_enum_type(saber_string_view _enum_name) :
    ckx_type(ckx_type::category::type_enum),
    enum_name(_enum_name)
{}

bool
ckx_enum_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() == ckx_type::category::type_enum)
    {
        ckx_enum_type *enum_type =
                static_cast<ckx_enum_type*>(_another.get());
        return (get_name() == enum_type->get_name());
    }
    return false;
}

saber_string
ckx_enum_type::to_string() const
{
    return "EnumType[[" + enum_name.get() + "]]" + qual_to_string();
}

saber_string_view ckx_enum_type::get_name() const
{
    return enum_name;
}

ckx_enum_type::add_status
ckx_enum_type::add_enumerator(saber_string_view _name, qint64 _value)
{
    for (enumerator& x : enumerators)
        if (x.name == _name)
            return add_status::add_duplicate;

    enumerators.emplace_back(_name, _value);
    return add_status::add_success;
}



ckx_func_type::ckx_func_type(
        saber_ptr<ckx_type> _return_type,
        saber::vector<saber_ptr<ckx_type>> &&_param_type_list) :
    ckx_type(ckx_type::category::type_function),
    return_type(saber::move(_return_type)),
    param_type_list(saber::move(_param_type_list))
{}

bool
ckx_func_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() != ckx_type::category::type_function)
        return false;

    ckx_func_type *func_type =
            static_cast<ckx_func_type*>(_another.get());
    if (!return_type->type_equal_to(func_type->return_type))
        return false;

    if (param_type_list.size() == func_type->param_type_list.size())
        return false;

    for (size_t i = 0; i < param_type_list.size(); i++)
    {
        if (!param_type_list[i]->type_equal_to(func_type->param_type_list[i]))
            return false;
    }

    return true;
}

saber_string
ckx_func_type::to_string() const
{
    saber_string ret = "fn (";
    for (const saber_ptr<ckx_type>& type : param_type_list)
        ret += type->to_string() + saber_string(",");
    ret += saber_string(") -> ");
    ret += return_type->to_string();
    return ret  + qual_to_string();
}


ckx_pointer_type::ckx_pointer_type(saber_ptr<ckx_type> _target) :
    ckx_type(ckx_type::category::type_pointer),
    target(_target)
{}

bool
ckx_pointer_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() == ckx_type::category::type_pointer)
    {
        ckx_pointer_type *ptr_type =
                static_cast<ckx_pointer_type*>(_another.get());
        return target->type_equal_to(ptr_type->target);
    }
    return false;
}

saber_string
ckx_pointer_type::to_string() const
{
    return target->to_string() + saber_string(" *")  + qual_to_string();
}



ckx_array_type::ckx_array_type(saber_ptr<ckx_type> _element) :
    ckx_type(ckx_type::category::type_array),
    element(_element)
{}

bool
ckx_array_type::type_equal_to(saber_ptr<ckx_type> _another) const
{
    if (_another->get_category() == ckx_type::category::type_array)
    {
        ckx_array_type *arr_type =
                static_cast<ckx_array_type*>(_another.get());
        return element->type_equal_to(arr_type->element);
    }
    return false;
}

saber_string
ckx_array_type::to_string() const
{
    return element->to_string() + "[]"  + qual_to_string();
}



saber_ptr<ckx_type>
ckx_type_helper::get_type(ckx_token::type _basic_type_token)
{
    switch ( _basic_type_token )
    {
    case ckx_token::type::tk_vi8:   return get_vi8_type();
    case ckx_token::type::tk_vi16:  return get_vi16_type();
    case ckx_token::type::tk_vi32:  return get_vi32_type();
    case ckx_token::type::tk_vi64:  return get_vi64_type();
    case ckx_token::type::tk_vu8:   return get_vu8_type();
    case ckx_token::type::tk_vu16:  return get_vu16_type();
    case ckx_token::type::tk_vu32:  return get_vu32_type();
    case ckx_token::type::tk_vu64:  return get_vu64_type();
    case ckx_token::type::tk_vch:   return get_vch_type();
    case ckx_token::type::tk_vr32:  return get_vr32_type();
    case ckx_token::type::tk_vr64:  return get_vr64_type();
    case ckx_token::type::tk_void:   return get_void_type();

    default: assert(0);
    }
}

saber_ptr<ckx_type>
ckx_type_helper::qual_const(saber_ptr<ckx_type> _base)
{
    _base->add_const(); return _base;
}

saber_ptr<ckx_type>
ckx_type_helper::pointer_to(saber_ptr<ckx_type> _base)
{
    return saber_ptr<ckx_type>(new ckx_pointer_type(_base));
}

saber_ptr<ckx_type>
ckx_type_helper::array_of(saber_ptr<ckx_type> _base)
{
    return saber_ptr<ckx_type>(new ckx_array_type(_base));
}

saber_ptr<ckx_type>
ckx_type_helper::get_vi8_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vi8) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vi16_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vi16) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vi32_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vi32) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vi64_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vi64) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vu8_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vu8) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vu16_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vu16) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vu32_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vu32) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vu64_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vu64) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vch_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vch) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vr32_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vr32) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_vr64_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_vr64) );
    return ret;
}

saber_ptr<ckx_type>
ckx_type_helper::get_void_type()
{
    static saber_ptr<ckx_type> ret(
        new ckx_basic_type(ckx_type::category::type_void));
    return ret;
}

} // namespace ckx
