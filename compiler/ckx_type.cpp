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

ckx_type::category ckx_type::get_category() const
{
    return this_category;
}

ckx_basic_type::ckx_basic_type(category _basic_category, bool _is_const) :
    ckx_type(_basic_category),
    is_const_value(_is_const)
{}

bool ckx_basic_type::is_const() const
{
    return is_const_value;
}

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

} // namespace ckx
