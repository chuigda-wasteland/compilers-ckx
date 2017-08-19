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
{
}

qsizet ckx_struct_type::size() const
{
    if ( fields.empty() ) return 0;

    return field_offset(*fields.rbegin())
           + field_type(*fields.rbegin()).size();
}

saber::string& ckx_struct_type::field_name(field &_field)
{
    return std::get<0>(_field);
}

ckx_type& ckx_struct_type::field_type(field &_field)
{
    return *(std::get<1>(_field).get());
}

qsizet &ckx_struct_type::field_offset(field &_field)
{
    return std::get<2>(_field);
}

const saber::string& ckx_struct_type::field_name(const field &_field)
{
    return std::get<0>(_field);
}

const ckx_type& ckx_struct_type::field_type(const field &_field)
{
    return *(std::get<1>(_field).get());
}

const qsizet &ckx_struct_type::field_offset(const field &_field)
{
    return std::get<2>(_field);
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



} // namespace ckx
