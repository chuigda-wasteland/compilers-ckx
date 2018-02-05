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

#include "saber/string_pool.hpp"
#include "saber/vector.hpp"
#include "saber/unordered_map.hpp"
#include "saber/result.hpp"
#include "saber/optional.hpp"
#include "we/defs.hpp"

#include "frontend/util/ckx_src_rng.hpp"
#include "frontend/sema/ckx_type.hpp"
#include "llvm/llvm_value.hpp"
#include "llvm/llvm_type.hpp"

namespace ckx
{

using saber::saber_ptr;

open_class ckx_env_var_entry
{
    ckx_src_rng decl_at;
    saber_string_view name;
    ckx_type* type;

    /// @note these variables require external components working on it.
    faker::llvm_value *llvm_value_bind = nullptr;

    ckx_env_var_entry(ckx_src_rng _decl_at, saber_string_view _name,
                      ckx_type* _type) :
        decl_at(_decl_at), name(_name), type(_type) {}
};

open_class ckx_env_type_entry
{
    ckx_src_rng decl_at;
    saber_string_view name;
    ckx_type* type;

    faker::llvm_type llvm_type_bind = saber_string_pool::create_view("");

    ckx_env_type_entry(ckx_src_rng _decl_at, saber_string_view _name,
                       ckx_type* _type) :
        decl_at(_decl_at), name(_name), type(_type) {}
};

open_class ckx_env_func_entry
{
    ckx_src_rng decl_at;
    saber_string_view name;
    ckx_func_type* type;
    saber_string_view llvm_name;

    bool defined = false;

    ckx_env_func_entry(ckx_src_rng _decl_at, saber_string_view _name,
                       ckx_func_type* _type, saber_string_view _llvm_name) :
        decl_at(_decl_at), name(_name),
        type(_type), llvm_name(_llvm_name) {}
};


class ckx_env
{
public:
    open_class err_add_var
    {
        enum class err_status : qchar { conflict, fail } status;
        saber::optional<ckx_env_var_entry*> conflict_decl;
        err_add_var(err_status _status,
                    ckx_env_var_entry* _conflict_decl = nullptr) :
            status(_status), conflict_decl(_conflict_decl) {}
    };

    open_class err_add_type
    {
        enum class err_status : qchar{ conflict, fail } status;
        saber::optional<ckx_env_type_entry*> conflict_decl;
        err_add_type(err_status _status,
                     ckx_env_type_entry* _conflict_decl = nullptr):
            status(_status), conflict_decl(_conflict_decl) {}
    };

    open_class result_add_func
    {
        enum class add_status
        {
            declare, redeclare, overload, conflict, fail
        }
        status;
        ckx_env_func_entry* added_or_conflict_func;
        result_add_func(add_status _status, ckx_env_func_entry *_entry) :
            status(_status), added_or_conflict_func(_entry) {}
    };

    ckx_env(ckx_env* _parent = nullptr) : parent(_parent) {}

    ckx_env* get_parent() { return parent; }

    bool lookup(saber_string_view _name);
    bool lookup_local(saber_string_view _name);

    ckx_env_var_entry* lookup_var(saber_string_view _name);
    ckx_env_type_entry* lookup_type(saber_string_view _name);
    saber::vector<ckx_env_func_entry>* lookup_func(saber_string_view _name);

    ckx_env_var_entry* lookup_var_local(saber_string_view _name);
    ckx_env_type_entry* lookup_type_local(saber_string_view _name);
    saber::vector<ckx_env_func_entry>*
    lookup_func_local(saber_string_view _name);

    saber::result<ckx_env_var_entry*, err_add_var>
    add_var(ckx_src_rng _decl_at, saber_string_view _name, ckx_type* _type);

    saber::result<ckx_env_type_entry*, err_add_type>
    add_type(ckx_src_rng _decl_at, saber_string_view _name, ckx_type* _type);

    result_add_func add_func(ckx_src_rng _decl_at,
                             saber_string_view _name,
                             ckx_func_type *_type);

private:
    /// just for shortening identifiers
    using sv = saber_string_view;
    using svhash = string_view_hash;

    saber::unordered_map<sv, ckx_env_var_entry, svhash> vars;
    saber::unordered_map<sv, ckx_env_type_entry, svhash> types;
    saber::unordered_map<sv, saber::vector<ckx_env_func_entry>, svhash> funcs;

    ckx_env *const parent;
};


} // namespace ckx

#endif // CKX_ENV_TABLE_HPP
