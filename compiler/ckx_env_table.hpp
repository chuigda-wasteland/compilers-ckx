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

#ifndef CKX_ENV_TABLE_HPP
#define CKX_ENV_TABLE_HPP

#include "string_pool.hpp"
#include "vector.hpp"
#include "unordered_map.hpp"
#include "result.hpp"
#include "defs.hpp"

#include "ckx_type.hpp"
#include "ckx_token.hpp"
#include "../llvm_ir_builder/llvm_value.hpp"
#include "../llvm_ir_builder/llvm_type.hpp"

namespace ckx
{

using saber::saber_ptr;

open_class ckx_env_var_entry
{
    ckx_token decl_at;
    saber_string_view name;
    ckx_type* type;

    /// @note we are not sure that this is correct.
    faker::llvm_value *llvm_value_bind;

    ckx_env_var_entry(ckx_token _decl_at,
                      saber_string_view _name,
                      ckx_type* _type) :
        decl_at(_decl_at),
        name(_name),
        type(_type)
    {}
};

open_class ckx_env_type_entry
{
    ckx_token decl_at;
    saber_string_view name;
    ckx_type* type;

    /// @note we are not sure that is this correct.
    faker::llvm_type *llvm_type_bind;

    ckx_env_type_entry(ckx_token _decl_at,
                       saber_string_view _name,
                       ckx_type* _type) :
        decl_at(_decl_at),
        name(_name),
        type(_type)
    {}
};

open_class ckx_env_func_entry
{
    ckx_token decl_at;
    saber_string_view name;
    ckx_type* type;
    saber_string llvm_name;

    ckx_env_func_entry(ckx_token _decl_at,
                       saber_string_view _name,
                       ckx_type* _type,
                       saber_string&& _llvm_name) :
        decl_at(_decl_at),
        name(_name),
        type(_type),
        llvm_name(saber::move(_llvm_name))
    {}
};


class ckx_env
{
public:
    struct result_add_var
    {
        enum { success, conflict, fail } status;
        variant value
        {
            ckx_env_var_entry* added_decl;
            ckx_env_var_entry* conflict_decl;
        } v;
    };

    struct result_add_type
    {
        enum { success, conflict, fail } status;
        variant value
        {
            ckx_env_type_entry* added_type;
            ckx_env_type_entry* conflict_type;
        } v;
    };

    struct result_add_func
    {
        enum { declare, redeclare, define, overload, conflict, fail } status;
        variant value
        {
            ckx_env_func_entry* added_func;
            ckx_env_func_entry* conflict_func;
        } v;
    };

    ckx_env(ckx_env* _parent = nullptr) : parent(_parent) {}

    bool lookup(saber_string_view _name);
    bool lookup_local(saber_string_view _name);

    ckx_env_var_entry* lookup_var(saber_string_view _name);
    ckx_env_type_entry* lookup_type(saber_string_view _name);
    saber::vector<ckx_env_func_entry>* lookup_func(saber_string_view _name);

    ckx_env_var_entry* lookup_var_local(saber_string_view _name);
    ckx_env_type_entry* lookup_type_local(saber_string_view _name);
    saber::vector<ckx_env_func_entry>*
    lookup_func_local(saber_string_view _name);

    result_add_var add_var(ckx_token _decl_at,
                           saber_string_view _name,
                           ckx_type* _type);

    result_add_type add_type(ckx_token _decl_at,
                             saber_string_view _name,
                             ckx_type* _type);

    result_add_func add_func(ckx_token _decl_at,
                             saber_string_view _name,
                             ckx_type* _type);

private:
    /// just for shortening identifiers
    using sv = saber_string_view;
    using svhash = string_view_hash;

    saber::unordered_map<sv, ckx_env_var_entry, svhash> vars;
    saber::unordered_map<sv, ckx_env_type_entry, svhash> types;
    saber::unordered_map<sv, saber::vector<ckx_env_func_entry>, svhash> funcs;

    ckx_env *parent;
};


} // namespace ckx

#endif // CKX_ENV_TABLE_HPP
