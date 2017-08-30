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

namespace ckx
{

ckx_type::ckx_type(category _category) :
    this_category(_category)
{}

ckx_type::~ckx_type() {}

const ckx_type::category &ckx_type::get_category() const
{
    return this_category;
}



ckx_basic_type::ckx_basic_type(category _basic_category) :
    ckx_type(_basic_category)
{}

qsizet ckx_basic_type::size() const
{
    switch (this_category)
    {
    case category::type_vi8:
    case category::type_vu8:
        return 1;

    case category::type_vi16:
    case category::type_vu16:
        return 2;

    case category::type_vi32:
    case category::type_vu32:
    case category::type_vr32:
        return 3;

    case category::type_vi64:
    case category::type_vu64:
    case category::type_vr64:
        return 4;

    case category::type_vch:
        return 2;

    case category::type_void:
    default:
        // what the fuck!
        return 0;
    }
}



ckx_struct_type::ckx_struct_type() :
    ckx_type(ckx_type::category::type_struct)
{}

qsizet ckx_struct_type::size() const
{
    if ( fields.empty() ) return 0;

    return (*fields.rbegin()).offset
           + (*fields.rbegin()).type->size();
}

ckx_struct_type::add_status
ckx_struct_type::add_field(saber::string &&_name, saber_ptr<ckx_type> _type)
{
    for (field& x : fields)
        if (x.name == _name)
            return add_status::add_duplicate;

    fields.emplace_back(saber::move(_name), _type, size());
    return add_status::add_success;
}



ckx_variant_type::ckx_variant_type() :
    ckx_type(ckx_type::category::type_variant)
{}

qsizet ckx_variant_type::size() const
{
    return field_size_max;
}

ckx_variant_type::add_status
ckx_variant_type::add_field(saber::string &&_name, saber_ptr<ckx_type> _type)
{
    for (field& x : fields)
        if (x.name == _name)
            return add_status::add_duplicate;

    fields.emplace_back(saber::move(_name), _type, size());
    field_size_max = (field_size_max > _type->size()) ?
                         field_size_max : _type->size();
    return add_status::add_success;
}



ckx_enum_type::ckx_enum_type() :
    ckx_type(ckx_type::category::type_enum)
{}

qsizet ckx_enum_type::size() const
{
    return 8;
}

ckx_enum_type::add_status
ckx_enum_type::add_enumerator(std::string &&_name, qint64 _value)
{
    for (enumerator& x : enumerators)
        if (x.name == _name)
            return add_status::add_duplicate;

    enumerators.emplace_back(saber::move(_name), _value);
    return add_status::add_success;
}



ckx_function_type::ckx_function_type(
        saber_ptr<ckx_type> _return_type,
        saber::vector<saber_ptr<ckx_type>> &&_param_type_list) :
    ckx_type(ckx_type::category::type_function),
    return_type(saber::move(_return_type)),
    param_type_list(saber::move(_param_type_list))
{}

qsizet ckx_function_type::size() const
{
    return 8;
}

ckx_qualification::ckx_qualification(saber_ptr<ckx_type> _qualified) :
    ckx_type(ckx_type::category::type_qualifier),
    qualified(_qualified)
{}

qsizet ckx_qualification::size() const
{
    return qualified->size();
}

ckx_pointer_type::ckx_pointer_type(saber_ptr<ckx_type> _target) :
    ckx_type(ckx_type::category::type_pointer),
    target(_target)
{}

qsizet ckx_pointer_type::size() const
{
    return target->size();
}

saber_ptr<ckx_type>
ckx_type_helper::get_type(ckx_token::type _basic_type_token)
{
    switch ( _basic_type_token )
    {
    case ckx_token::type::token_int8:   return get_vi8_type();
    case ckx_token::type::token_int16:  return get_vi16_type();
    case ckx_token::type::token_int32:  return get_vi32_type();
    case ckx_token::type::token_int64:  return get_vi64_type();
    case ckx_token::type::token_uint8:  return get_vu8_type();
    case ckx_token::type::token_uint16: return get_vu16_type();
    case ckx_token::type::token_uint32: return get_vu32_type();
    case ckx_token::type::token_uint64: return get_vu64_type();
    case ckx_token::type::token_char:   return get_vch_type();
    case ckx_token::type::token_real32: return get_vr32_type();
    case ckx_token::type::token_real64: return get_vr64_type();
    case ckx_token::type::token_void:   return get_void_type();

    default: assert(0);
    }
}

saber_ptr<ckx_type>
ckx_type_helper::qual_const(saber_ptr<ckx_type> _base)
{
    return saber_ptr<ckx_type>(new ckx_qualification(_base));
}

saber_ptr<ckx_type>
ckx_type_helper::pointer_to(saber_ptr<ckx_type> _base)
{
    return saber_ptr<ckx_type>(new ckx_pointer_type(_base));
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
