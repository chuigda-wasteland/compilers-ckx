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

#include "ckx_env_table.hpp"

namespace ckx
{

ckx_var_entry::ckx_var_entry(saber_ptr<ckx_type> _var_type) :
    var_type(_var_type)
{}

ckx_func_entry::ckx_func_entry(saber_ptr<ckx_function_type> _func_type) :
    func_type(_func_type)
{}

ckx_type_entry::ckx_type_entry(saber_ptr<ckx_type> _type) :
    type(_type)
{}

ckx_env_table::ckx_env_table(ckx_env_table *_parent) :
    parent(_parent)
{}

ckx_env_table::add_status
ckx_env_table::add_new_var(saber::string &&_name, saber_ptr<ckx_type> _type)
{
    if ( var_entry_table.find(_name) != var_entry_table.end() )
    {
        return add_status::duplicate;
    }

    var_entry_table.insert(
                qpair<saber::string, ckx_var_entry>(
                    saber::move(_name), ckx_var_entry(_type) ) );

    return add_status::success;
}

ckx_env_table::add_status
ckx_env_table::add_new_type(saber::string &&_name, saber_ptr<ckx_type> _type)
{
    if ( query_type(_name) != nullptr )
    {
         return add_status::duplicate;
    }

    type_entry_table.insert(
                qpair<const saber::string, ckx_type_entry>(
                    saber::move(_name), ckx_type_entry(_type) ) );

    return add_status::success;
}

ckx_env_table::add_status
ckx_env_table::add_new_func(saber::string &&_name,
                            saber_ptr<ckx_function_type> _type)
{
    saber::vector<saber_ptr<ckx_function_type>>
            query_result = query_func(_name);

    if (query_result.size() != 0)
    {
        Q_ON_HOLD(...)
        return add_status::duplicate;
    }

    func_entry_table.insert(
                qpair<const saber::string, saber_ptr<ckx_func_entry>>(
                    saber::move(_name),
                    saber_ptr<ckx_func_entry>(new ckx_func_entry(_type)) ) );

    return add_status::success;
}

saber_ptr<ckx_type>
ckx_env_table::query_var(const saber::string& _name)
{
    ckx_env_table *this_iter = this;

    while (this_iter != nullptr)
    {
        auto it = var_entry_table.find(_name);

        if (it != var_entry_table.end())
        {
            qpair<const saber::string, ckx_var_entry> &tref = *it;
            return tref.second.var_type;
        }

        this_iter = this_iter->parent;
    }

    return nullptr;
}

saber_ptr<ckx_type>
ckx_env_table::query_type(const saber::string& _name)
{
    ckx_env_table *this_iter = this;

    while (this_iter != nullptr)
    {
        auto it = type_entry_table.find(_name);

        if (it != type_entry_table.end())
        {
            qpair<const saber::string, ckx_type_entry> &tref = *it;
            return tref.second.type;
        }

        this_iter = this_iter->parent;
    }

    return nullptr;
}

saber::vector<saber_ptr<ckx_function_type>>
ckx_env_table::query_func(const std::string &_name)
{
    // ckx_env_table *this_iter = this;
    Q_ON_HOLD(...)
    Q_UNUSED(_name)

    return saber::vector<saber_ptr<ckx_function_type>>();
}

} // namespace ckx
