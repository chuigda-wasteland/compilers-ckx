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

#include "saber/memory.hpp"
#include "saber/utility.hpp"
#include "saber/string.hpp"
#include "saber/string_pool.hpp"
#include "saber/object_pool.hpp"
#include "we/defs.hpp"
#include "frontend/lex/ckx_token.hpp"

namespace ckx
{

using saber::saber_ptr;

class ckx_type_helper;

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

    explicit ckx_type(category _category);
    virtual ~ckx_type() = 0;
    virtual saber_string to_string() const = 0;

    bool equal_to(ckx_type *_another) const;
    virtual bool equal_to_no_cvr(ckx_type *_another) const = 0;

    category get_category() const;

    bool is_basic() const;
    bool is_scalar() const;
    bool is_signed() const;
    bool is_unsigned() const;
    bool is_integral() const;
    bool is_floating() const;
    bool is_numeric() const;
    bool is_pointer() const;
    bool is_function() const;
    bool is_record() const;
    bool is_enum() const;
    bool is_alias() const;

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
    bool more_qual_than(unsigned char _another_qual_bits) const;

protected:
    category type_category;
    unsigned char qual;

    saber_string qual_to_string() const;
};


class ckx_basic_type final implements ckx_type
{
public:
    ~ckx_basic_type() override final = default;
    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;

private:
    friend class ckx_type_helper;
    explicit ckx_basic_type(category _basic_category);
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
        ckx_type* type;
        qsizet offset;

        field(saber_string_view _name,
              ckx_type* _type)
            : name(_name), type(_type) {}
    };

    ~ckx_struct_type() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;

    saber_string_view get_name() const;
    add_status add_field(saber_string_view _name, ckx_type* _type);

    saber::vector<field>& get_fields();

private:
    friend class ckx_type_helper;
    explicit ckx_struct_type(saber_string_view _struct_name);

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
        ckx_type* type;
        qsizet offset;

        field(saber_string_view _name,
              ckx_type* _type)
            : name(saber::move(_name)), type(_type) {}
    };

    ~ckx_variant_type() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;

    saber_string_view get_name() const;
    add_status add_field(saber_string_view _name, ckx_type* _type);

private:
    friend class ckx_type_helper;
    explicit ckx_variant_type(saber_string_view _variant_name);

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

    ~ckx_enum_type() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;

    saber_string_view get_name() const;
    add_status add_enumerator(saber_string_view _name, qint64 _value);

private:
    friend class ckx_type_helper;
    explicit ckx_enum_type(saber_string_view _enum_name);

    saber::vector<enumerator> enumerators;
    saber_string_view enum_name;
};

class ckx_func_type final implements ckx_type
{
public:
    ~ckx_func_type() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;

    ckx_type* get_return_type();
    const saber::vector<ckx_type*>& get_param_type_list();

private:
    friend class ckx_type_helper;
    ckx_func_type(ckx_type* _return_type,
                  saber::vector<ckx_type*>&& _param_type_list);

    ckx_type* return_type;
    saber::vector<ckx_type*> param_type_list;
};

class ckx_pointer_type final implements ckx_type
{
public:
    ~ckx_pointer_type() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;
    ckx_type* get_pointee();

private:
    friend class ckx_type_helper;
    explicit ckx_pointer_type(ckx_type* _target);

    ckx_type* target;
};

class ckx_array_type final implements ckx_type
{
public:
    ~ckx_array_type() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;
    ckx_type* get_element_type();

private:
    friend class ckx_type_helper;
    explicit ckx_array_type(ckx_type* _element);

    ckx_type* element_type;
};

class ckx_type_alias final implements ckx_type
{
public:
    ~ckx_type_alias() override final = default;

    saber_string to_string() const override final;
    bool equal_to_no_cvr(ckx_type *_another) const override final;
    ckx_type* get_aliasee();

private:
    friend class ckx_type_helper;
    explicit ckx_type_alias(ckx_type* _origin);

    ckx_type* origin;
};



class ckx_type_helper
{
public:
    static ckx_struct_type*  create_struct_type(saber_string_view _name);
    static ckx_variant_type* create_variant_type(saber_string_view _name);
    static ckx_enum_type*    create_enum_type(saber_string_view _name);
    static ckx_type_alias*   create_alias(ckx_type *_type);
    static ckx_func_type*    create_func_type(ckx_type *_ret_type,
                                            saber::vector<ckx_type*> &&_params);
    static ckx_array_type*   create_array_type(ckx_type *_elem_type);
    static ckx_pointer_type* pointer_to(ckx_type* _base);
    static ckx_type*         qual_const(ckx_type* _base);

    static ckx_type* get_type_by_token(ckx_token::type _basic_type_token);
    static ckx_type* get_basic_type(ckx_type::category _basic_type_categ);

    static ckx_type* get_vi8_type();
    static ckx_type* get_vi16_type();
    static ckx_type* get_vi32_type();
    static ckx_type* get_vi64_type();

    static ckx_type* get_vu8_type();
    static ckx_type* get_vu16_type();
    static ckx_type* get_vu32_type();
    static ckx_type* get_vu64_type();

    static ckx_type* get_vch_type();
    static ckx_type* get_vr32_type();
    static ckx_type* get_vr64_type();

    static ckx_type* get_void_type();

    static qint8 rank_of(ckx_type::category _type_category);

    static bool can_implicit_cast(ckx_type* _from, ckx_type* _dest);
    static bool can_static_cast(ckx_type* _from, ckx_type* _dest);
    static bool can_reinterpret_cast(ckx_type* _from, ckx_type* _dest);
    static bool can_const_cast(ckx_type* _from, ckx_type* _dest);
    static bool can_ckx_cast(ckx_type* _from, ckx_type* _dest);

    static ckx_type* common_numeric_type(ckx_type* _ty1, ckx_type *_ty2);

    enum class function_relation { fr_duplicate, fr_overload };
    function_relation predicate_function_relation(ckx_func_type *_f1,
                                                  ckx_func_type *_f2);

private:
    thread_local static saber::object_pool<ckx_struct_type>  struct_type_pool;
    thread_local static saber::object_pool<ckx_variant_type> variant_type_pool;
    thread_local static saber::object_pool<ckx_enum_type>    enum_type_pool;
    thread_local static saber::object_pool<ckx_type_alias>   type_alias_pool;
    thread_local static saber::object_pool<ckx_func_type>    func_type_pool;
    thread_local static saber::object_pool<ckx_array_type>   array_type_pool;
    thread_local static saber::object_pool<ckx_pointer_type> pointer_type_pool;
};

} // namespace ckx

#endif // CKX_TYPE_HPP
