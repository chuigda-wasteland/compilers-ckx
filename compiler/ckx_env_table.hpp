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

#include "string_pool.hpp"
#include "vector.hpp"
#include "unordered_map.hpp"

#include "defs.hpp"

#include "ckx_ast_node_fwd.hpp"

namespace ckx
{

using saber::saber_ptr;

open_class ckx_var_entry
{
    explicit ckx_var_entry(saber_ptr<ckx_type> _var_type,
                           saber_string_view _var_name);
    ~ckx_var_entry() = default;

    saber_ptr<ckx_type> var_type;
    saber_string_view var_name;
};

open_class ckx_func_entry
{
    explicit ckx_func_entry(saber_ptr<ckx_func_type> _func_type,
                            saber_string_view _func_name);
    ~ckx_func_entry() = default;

    saber_ptr<ckx_func_type> func_type;
    saber_string_view func_name;
    ckx_ast_func_stmt *the_function;
};

open_class ckx_type_entry
{
    explicit ckx_type_entry(saber_ptr<ckx_type> type,
                            saber_string_view _type_name);
    ~ckx_type_entry() = default;

    saber_ptr<ckx_type> type;
    saber_string_view type_name;

};

class ckx_env
{
public:
    enum add_status : qchar
    {
        success,
        duplicate
    };

    explicit ckx_env(ckx_env *_parent);
    ~ckx_env();

    qpair<add_status, ckx_type_entry*> add_new_type(
            saber_string_view _name, saber_ptr<ckx_type> _type);
    qpair<add_status, ckx_func_entry*> add_new_func(
            saber_string_view, saber_ptr<ckx_func_type> _type);
    qpair<add_status, ckx_var_entry*> add_new_var(
            saber_string_view, saber_ptr<ckx_type> _type);

    bool lookup_name(saber_string_view _name);

    ckx_var_entry* lookup_var(saber_string_view _name);
    ckx_type_entry* lookup_type(saber_string_view _name);

    /// @note We have independent representation for function table
    /// since we need to solve function overloading in the future.
    saber::vector<ckx_func_entry*> lookup_func(saber_string_view _name);

    ckx_var_entry* lookup_var_local(saber_string_view _name);

    inline ckx_env* get_parent() { return parent; }

private:
    saber::unordered_map<saber_string_view, ckx_var_entry*, string_view_hash>
    var_entry_table;
    saber::unordered_map<saber_string_view, ckx_type_entry*, string_view_hash>
    type_entry_table;

    saber::unordered_multimap<saber_string_view, ckx_func_entry*, string_view_hash>
    func_entry_table;

    ckx_env *parent;
};

} // namespace ckx

#endif // CKX_ENV_TABLE_HPP
