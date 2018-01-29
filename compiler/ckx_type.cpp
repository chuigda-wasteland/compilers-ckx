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
#include "c8assert.hpp"

namespace ckx
{

ckx_type::ckx_type(category _category) :
    type_category(_category),
    qual(0)
{}

ckx_type::~ckx_type() {}

ckx_type::category
ckx_type::get_category() const
{
    return type_category;
}

bool ckx_type::is_basic() const
{
    return get_category() >= category::type_vi8
            && get_category() <= category::type_void;
}

bool ckx_type::is_scalar() const
{
    return is_basic() || is_pointer();
}

bool ckx_type::is_signed() const
{
    return get_category() >= category::type_vi8
           && get_category() <= category::type_vi64;
}

bool ckx_type::is_unsigned() const
{
    return get_category() >= category::type_vu8
            && get_category() <= category::type_vu64;
}

bool ckx_type::is_integral() const
{
    return is_signed() || is_unsigned();
}

bool ckx_type::is_floating() const
{
    return get_category() == category::type_vr32
            || get_category() == category::type_vr64;
}

bool ckx_type::is_pointer() const
{
    return get_category() == ckx_type::category::type_pointer;
}

bool ckx_type::is_function() const
{
    return get_category() == ckx_type::category::type_function;
}

bool ckx_type::is_record() const
{
    return get_category() == ckx_type::category::type_struct
            || get_category() == ckx_type::category::type_variant;
}

bool ckx_type::is_enum() const
{
    return get_category() == ckx_type::category::type_enum;
}

bool ckx_type::is_alias() const
{
    return get_category() == ckx_type::category::type_alias;
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


ckx_basic_type::ckx_basic_type(category _basic_category) :
    ckx_type(_basic_category)
{}


ckx_struct_type::ckx_struct_type(saber_string_view _struct_name) :
    ckx_type(ckx_type::category::type_struct),
    struct_name(_struct_name)
{}

saber_string_view ckx_struct_type::get_name() const
{
    return struct_name;
}

ckx_struct_type::add_status
ckx_struct_type::add_field(saber_string_view _name, ckx_type* _type)
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

saber_string_view
ckx_variant_type::get_name() const
{
    return variant_name;
}

ckx_variant_type::add_status
ckx_variant_type::add_field(saber_string_view _name, ckx_type* _type)
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

saber_string_view
ckx_enum_type::get_name() const
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
        ckx_type* _return_type,
        saber::vector<ckx_type*> &&_param_type_list) :
    ckx_type(ckx_type::category::type_function),
    return_type(saber::move(_return_type)),
    param_type_list(saber::move(_param_type_list))
{}

ckx_type*
ckx_func_type::get_return_type()
{
    return return_type;
}

const saber::vector<ckx_type*>&
ckx_func_type::get_param_type_list()
{
    return param_type_list;
}


ckx_pointer_type::ckx_pointer_type(ckx_type* _target) :
    ckx_type(ckx_type::category::type_pointer),
    target(_target)
{}

ckx_type*
ckx_pointer_type::get_pointee()
{
    return target;
}

ckx_array_type::ckx_array_type(ckx_type* _element) :
    ckx_type(ckx_type::category::type_array),
    element_type(_element)
{}

ckx_type* ckx_array_type::get_element_type()
{
    return element_type;
}


ckx_type_alias::ckx_type_alias(ckx_type *_origin) :
    ckx_type(ckx_type::category::type_alias),
    origin(_origin)
{}

ckx_type* ckx_type_alias::get_aliasee()
{
    return origin;
}


ckx_type*
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
    case ckx_token::type::tk_void:  return get_void_type();
    default: C8ASSERT(0);
    }
}



thread_local saber::object_pool<ckx_struct_type>
ckx_type_helper::struct_type_pool;
thread_local saber::object_pool<ckx_variant_type>
ckx_type_helper::variant_type_pool;
thread_local saber::object_pool<ckx_enum_type>
ckx_type_helper::enum_type_pool;
thread_local saber::object_pool<ckx_type_alias>
ckx_type_helper::type_alias_pool;
thread_local saber::object_pool<ckx_func_type>
ckx_type_helper::func_type_pool;
thread_local saber::object_pool<ckx_array_type>
ckx_type_helper::array_type_pool;
thread_local saber::object_pool<ckx_pointer_type>
ckx_type_helper::pointer_type_pool;

ckx_struct_type*
ckx_type_helper::create_struct_type(saber_string_view _name)
{
    return struct_type_pool.store(ckx_struct_type(_name));
}

ckx_variant_type*
ckx_type_helper::create_variant_type(saber_string_view _name)
{
    return variant_type_pool.store(ckx_variant_type(_name));
}

ckx_enum_type*
ckx_type_helper::create_enum_type(saber_string_view _name)
{
    return enum_type_pool.store(ckx_enum_type(_name));
}

ckx_type_alias*
ckx_type_helper::create_alias(ckx_type *_type)
{
    return type_alias_pool.store(ckx_type_alias(_type));
}

ckx_func_type*
ckx_type_helper::create_func_type(ckx_type *_ret_type,
                                  saber::vector<ckx_type*> &&_params)
{
    return func_type_pool.store(ckx_func_type(_ret_type, saber::move(_params)));
}

ckx_array_type*
ckx_type_helper::create_array_type(ckx_type *_elem_type)
{
    return array_type_pool.store(ckx_array_type(_elem_type));
}

ckx_pointer_type *ckx_type_helper::pointer_to(ckx_type* _base)
{
    return pointer_type_pool.store(ckx_pointer_type(_base));
}

ckx_type*
ckx_type_helper::qual_const(ckx_type* _base)
{
    _base->add_const(); return _base;
}

ckx_type*
ckx_type_helper::get_vi8_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi8);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vi16_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi16);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vi32_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi32);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vi64_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi64);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vu8_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu8);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vu16_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu16);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vu32_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu32);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vu64_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu64);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vch_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vch);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vr32_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vr32);
    return &ret;
}

ckx_type*
ckx_type_helper::get_vr64_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vr64);
    return &ret;
}

ckx_type*
ckx_type_helper::get_void_type()
{
    static ckx_basic_type ret(ckx_type::category::type_void);
    return &ret;
}

} // namespace ckx
