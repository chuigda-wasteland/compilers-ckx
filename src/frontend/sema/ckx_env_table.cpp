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

#include "frontend/sema/ckx_env_table.hpp"
#include "frontend/sema/ckx_func_name_mangler.hpp"

namespace ckx
{

bool
ckx_env::lookup(saber_string_view _name)
{
    return (lookup_var(_name) != nullptr)
           || (lookup_type(_name) != nullptr)
           || (lookup_var(_name) != nullptr);
}

bool
ckx_env::lookup_local(saber_string_view _name)
{
    return (lookup_var_local(_name) != nullptr)
           || (lookup_type_local(_name) != nullptr)
           || (lookup_func_local(_name) != nullptr);
}

ckx_env_var_entry*
ckx_env::lookup_var(saber_string_view _name)
{
    ckx_env *env_iter = this;
    while (env_iter != nullptr)
    {
        ckx_env_var_entry *entry = env_iter->lookup_var_local(_name);
        if (entry != nullptr) return entry;
        env_iter = env_iter->parent;
    }
    return nullptr;
}

ckx_env_type_entry*
ckx_env::lookup_type(saber_string_view _name)
{
    ckx_env *env_iter = this;
    while (env_iter != nullptr)
    {
        ckx_env_type_entry *entry = env_iter->lookup_type_local(_name);
        if (entry != nullptr) return entry;
        env_iter = env_iter->parent;
    }
    return nullptr;
}

saber::vector<ckx_env_func_entry>*
ckx_env::lookup_func(saber_string_view _name)
{
    ckx_env *env_iter = this;
    while (env_iter != nullptr)
    {
        saber::vector<ckx_env_func_entry> *entry =
            env_iter->lookup_func_local(_name);
        if (entry != nullptr) return entry;
        env_iter = env_iter->parent;
    }
    return nullptr;
}

ckx_env_var_entry*
ckx_env::lookup_var_local(saber_string_view _name)
{
    auto iter = vars.find(_name);
    if (iter != vars.end())
        return &(iter->second);
    else
        return nullptr;
}

ckx_env_type_entry*
ckx_env::lookup_type_local(saber_string_view _name)
{
    auto iter = types.find(_name);
    if (iter != types.end())
        return &(iter->second);
    else
        return nullptr;
}

saber::vector<ckx_env_func_entry>*
ckx_env::lookup_func_local(saber_string_view _name)
{
    auto iter = funcs.find(_name);
    if (iter != funcs.end())
        return &(iter->second);
    else
        return nullptr;
}

saber::result<ckx_env_var_entry *, ckx_env::err_add_var>
ckx_env::add_var(ckx_src_rng _decl_at, saber_string_view _name, ckx_type* _type)
{
    ckx_env_var_entry *entry = lookup_var_local(_name);
    if (entry != nullptr)
        return saber::result<ckx_env_var_entry*, err_add_var>(
            saber::err_t(), err_add_var::err_status::conflict, entry);

    auto it = vars.emplace(_name, ckx_env_var_entry(_decl_at, _name, _type))
                  .first;
    return saber::result<ckx_env_var_entry*, err_add_var>(
        saber::type_t(), &(it->second));
}

saber::result<ckx_env_type_entry *, ckx_env::err_add_type>
ckx_env::add_type(ckx_src_rng _decl_at, saber_string_view _name, ckx_type *_type)
{
    ckx_env_type_entry *entry = lookup_type(_name);
    if (entry != nullptr)
        return saber::result<ckx_env_type_entry*, err_add_type>(
             saber::err_t(), err_add_type::err_status::conflict,  entry);

    auto it = types.emplace(_name, ckx_env_type_entry(_decl_at, _name, _type))
                   .first;
    return saber::result<ckx_env_type_entry*, err_add_type>(
        saber::type_t(), &(it->second));
}

ckx_env::result_add_func
ckx_env::add_func(ckx_src_rng _decl_at, saber_string_view _name, ckx_type* _type)
{
    saber::vector<ckx_env_func_entry>* seen_funcs = lookup_func_local(_name);
    if (seen_funcs != nullptr)
    {
        for (ckx_env_func_entry &func : *seen_funcs)
        {
            /// @todo overload resolver
        }
    }

    auto it = funcs.emplace(
        _name, saber::vector<ckx_env_func_entry>{
                   ckx_env_func_entry(_decl_at, _name, _type,
                                      ckx_func_name_mangler::std_mangle(
                                          _name, _type))
                           }).first;
    return result_add_func{.status=result_add_func::declare,
                           .v.added_func=&(it->second[0])};
}

} // namespace ckx
