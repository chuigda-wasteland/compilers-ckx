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

#ifndef CKX_TYPE_HPP
#define CKX_TYPE_HPP

#include "memory.hpp"
#include "utility.hpp"
#include "string.hpp"
#include "vector.hpp"

#include "defs.hpp"


namespace ckx
{

class ckx_type
{
public:
    enum class category : unsigned char
    {
        type_vi8,
        type_vi16,
        type_vi32,
        type_vi64,

        type_vu8,
        type_vu16,
        type_vu32,
        type_vu64,

        type_vch,
        type_vr32,
        type_vr64,

        type_void,

        type_function,
        type_enum,

        type_struct,
        type_variant,

        type_pointer,
        type_typedef
    };

    ckx_type(category _category);
    virtual ~ckx_type() = 0;

    category get_category() const;
    virtual bool is_const() const = 0;
    virtual qsizet size() const = 0;

protected:
    category this_category;
};


class ckx_basic_type : public ckx_type
{
public:
    ckx_basic_type(category _basic_category, bool _is_const);
    ~ckx_basic_type() = default;

    bool is_const() const override final;
    qsizet size() const override final;

private:
    bool is_const_value;
};

class ckx_record_type : public ckx_type
{
};

class ckx_function_type : public ckx_type
{
};

class ckx_pointer_type : public ckx_type
{
};

} // namespace ckx

#endif // CKX_TYPE_HPP
