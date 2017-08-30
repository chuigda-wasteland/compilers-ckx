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

#include "ckx_type.hpp"

#include "string.hpp"
#include "vector.hpp"
#include "unordered_map.hpp"

#include "defs.hpp"

#include "ckx_ast_node_fwd.hpp"

namespace ckx
{

using saber::saber_ptr;

open_class ckx_var_entry
{
    ckx_var_entry(saber_ptr<ckx_type> _var_type);
    ~ckx_var_entry() = default;

    saber_ptr<ckx_type> var_type;
};

open_class ckx_func_entry
{
    ckx_func_entry(saber_ptr<ckx_function_type> _func_type);
    ~ckx_func_entry() = default;

    saber_ptr<ckx_function_type> func_type;
    ckx_ast_func_stmt *the_function;

};

open_class ckx_type_entry
{
    ckx_type_entry(saber_ptr<ckx_type> type);
    ~ckx_type_entry() = default;

    saber_ptr<ckx_type> type;
};

class ckx_env_table
{
public:
    enum add_status : qchar
    {
        success,
        duplicate
    };

    ckx_env_table(ckx_env_table *_parent);
    ~ckx_env_table();

    qpair<add_status, ckx_type_entry*> add_new_type(
            saber::string&& _name, saber_ptr<ckx_type> _type);
    qpair<add_status, ckx_func_entry*> add_new_func(
            saber::string&& _name, saber_ptr<ckx_function_type> _type);
    qpair<add_status, ckx_var_entry*> add_new_var(
            saber::string&& _name, saber_ptr<ckx_type> _type);

    bool lookup_name(const saber::string& _name);

    ckx_var_entry* lookup_var(const saber::string& _name);
    ckx_type_entry* lookup_type(const saber::string& _name);

    /// @note We have independent representation for function table
    /// since we need to solve function overloading in the future.
    saber::vector<ckx_func_entry*> lookup_func(const saber::string& _name);

    ckx_var_entry* lookup_var_local(const saber::string& _name);

    inline ckx_env_table* get_parent() { return parent; }

private:
    saber::unordered_map<saber::string, ckx_var_entry*> var_entry_table;
    saber::unordered_map<saber::string, ckx_type_entry*> type_entry_table;
    saber::unordered_multimap<saber::string, ckx_func_entry*> func_entry_table;

    ckx_env_table *parent;
};

} // namespace ckx

#endif // CKX_ENV_TABLE_HPP
