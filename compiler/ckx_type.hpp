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

#include "ckx_token.hpp"
#include "defs.hpp"


namespace ckx
{

using saber::saber_ptr;

interface ckx_type
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

        type_id,
        type_struct,
        type_variant,
        type_enum,

        type_pointer,
        type_array,
        type_alias
    };

    enum qualifiers
    {
        qual_const    = 0x01,
        qual_volatile = 0x02,
        qual_restrict = 0x04,

        qual_mask = qual_const | qual_volatile | qual_restrict
    };

    ckx_type(category _category);
    virtual ~ckx_type() = 0;

    virtual bool type_equal_to(saber_ptr<ckx_type> _another) const = 0;

    /// @todo finish the following two functions.
    // bool exact_equal_to(saber_ptr<ckx_type> _another) const;
    // bool comptiable_with(saber_ptr<ckx_type> _another) const;
    // bool can_cast_to(saber_ptr<ckx_type> _another) const;

    virtual saber_string to_string() const = 0;

    const category& get_category() const;

    bool is_const() const;
    bool is_volatile() const;
    bool is_restrict() const;

    void add_const();
    void add_volatile();
    void add_restrict();

    void remove_const();
    void remove_volatile();
    void remove_restrict();

    unsigned char get_qual_bits() const;
    void from_qual_bits(unsigned char _qual_bits);

    bool is_more_qual_than(saber_ptr<ckx_type> _another) const;
    bool is_less_qual_than(saber_ptr<ckx_type> _another) const;

protected:
    category type_category;
    unsigned char qual;

    saber_string qual_to_string() const;
};


class ckx_basic_type final implements ckx_type
{
public:
    ckx_basic_type(category _basic_category);
    ~ckx_basic_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;
};

class ckx_id_type final implements ckx_type
{
public:
    ckx_id_type(saber_string_view _name);
    ~ckx_id_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

    saber_string_view get_name() const;

private:
    saber_string_view name;
};

class ckx_struct_type final implements ckx_type
{
public:
    enum class add_status : qchar
    {
        add_success,
        add_duplicate
    };

    open_class field
    {
        saber_string_view name;
        saber_ptr<ckx_type> type;
        qsizet offset;

        field(saber_string_view _name,
              saber_ptr<ckx_type> _type)
            : name(_name), type(_type) {}
    };

    ckx_struct_type(saber_string_view _struct_name);
    ~ckx_struct_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

    saber_string_view get_name() const;
    add_status add_field(saber_string_view _name, saber_ptr<ckx_type> _type);

private:
    saber::vector<field> fields;
    saber_string_view struct_name;
};

class ckx_variant_type final implements ckx_type
{
public:
    enum class add_status : qchar
    {
        add_success,
        add_duplicate
    };

    open_class field
    {
        saber_string_view name;
        saber_ptr<ckx_type> type;
        qsizet offset;

        field(saber_string_view _name,
              saber_ptr<ckx_type> _type)
            : name(saber::move(_name)), type(_type) {}
    };

    ckx_variant_type(saber_string_view _variant_name);
    ~ckx_variant_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

    saber_string_view get_name() const;
    add_status add_field(saber_string_view _name, saber_ptr<ckx_type> _type);

private:
    saber::vector<field> fields;
    saber_string_view variant_name;
};

class ckx_enum_type final implements ckx_type
{
public:
    enum class add_status : qchar
    {
        add_success,
        add_duplicate
    };

    open_class enumerator
    {
        saber_string_view name;
        qint64 value;

        enumerator(saber_string_view _name, qint64 _value)
            : name(_name), value(_value) {}
    };

    ckx_enum_type(saber_string_view _enum_name);
    ~ckx_enum_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

    saber_string_view get_name() const;
    add_status add_enumerator(saber_string_view _name, qint64 _value);

private:
    saber::vector<enumerator> enumerators;
    saber_string_view enum_name;
};

class ckx_func_type final implements ckx_type
{
public:
    ckx_func_type(saber_ptr<ckx_type> _return_type,
                  saber::vector<saber_ptr<ckx_type>>&& _param_type_list);
    ~ckx_func_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

private:
    saber_ptr<ckx_type> return_type;
    saber::vector<saber_ptr<ckx_type>> param_type_list;
};

class ckx_pointer_type final implements ckx_type
{
public:
    ckx_pointer_type(saber_ptr<ckx_type> _target);
    ~ckx_pointer_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

private:
    saber_ptr<ckx_type> target;
};

class ckx_array_type final implements ckx_type
{
public:
    ckx_array_type(saber_ptr<ckx_type> _element);
    ~ckx_array_type() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

private:
    saber_ptr<ckx_type> element;
};

class ckx_type_alias final implements ckx_type
{
public:
    ckx_type_alias(saber_ptr<ckx_type> _origin);
    ~ckx_type_alias() override final = default;

    bool type_equal_to(saber_ptr<ckx_type> _another) const override final;
    saber_string to_string() const override final;

private:
    saber_ptr<ckx_type> origin;
};



open_class ckx_type_cast_step
{
    enum class castop : qchar
    { cst_static, cst_const, cst_reinterpret, cst_ckx };

    castop op;
    saber_ptr<ckx_type> from;
    saber_ptr<ckx_type> to;

    ckx_type_cast_step(castop _op,
                       saber_ptr<ckx_type> from,
                       saber_ptr<ckx_type> to);
    ~ckx_type_cast_step() = default;
};

using ckx_type_cast_path = saber::pair<ckx_type_cast_step, ckx_type_cast_step>;

class ckx_type_helper
{
public:
    static saber_ptr<ckx_type> get_type(ckx_token::type _basic_type_token);
    static saber_ptr<ckx_type> qual_const(saber_ptr<ckx_type> _base);
    static saber_ptr<ckx_type> pointer_to(saber_ptr<ckx_type> _base);
    static saber_ptr<ckx_type> array_of(saber_ptr<ckx_type> _base);

    /// @note made public may benefit testing a lot.

    static saber_ptr<ckx_type> get_vi8_type();
    static saber_ptr<ckx_type> get_vi16_type();
    static saber_ptr<ckx_type> get_vi32_type();
    static saber_ptr<ckx_type> get_vi64_type();

    static saber_ptr<ckx_type> get_vu8_type();
    static saber_ptr<ckx_type> get_vu16_type();
    static saber_ptr<ckx_type> get_vu32_type();
    static saber_ptr<ckx_type> get_vu64_type();

    static saber_ptr<ckx_type> get_vch_type();
    static saber_ptr<ckx_type> get_vr32_type();
    static saber_ptr<ckx_type> get_vr64_type();

    static saber_ptr<ckx_type> get_void_type();

    static bool
    type_equal(saber_ptr<ckx_type> _ty1, saber_ptr<ckx_type> _ty2);

    static bool
    can_implicit_cast(saber_ptr<ckx_type> _from, saber_ptr<ckx_type> _to);

    static bool
    can_static_cast(saber_ptr<ckx_type> _from, saber_ptr<ckx_type> _to);

    static bool
    can_reinterpret_cast(saber_ptr<ckx_type> _from, saber_ptr<ckx_type> _to);

    static bool
    can_const_cast(saber_ptr<ckx_type> _from, saber_ptr<ckx_type> _to);

    static ckx_type_cast_path
    try_ckx_cast(saber_ptr<ckx_type> _from, saber_ptr<ckx_type> _to);

    enum class type_relation
    {
        tr_equal,
        tr_comptiable,
        tr_cancast,
        tr_incomtiable
    };

    static type_relation
    resolve_type_relation(saber_ptr<ckx_type> _ty1, saber_ptr<ckx_type> _ty2);
};

} // namespace ckx

#endif // CKX_TYPE_HPP
