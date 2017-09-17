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

ckx_var_entry::ckx_var_entry(saber_ptr<ckx_type> _var_type,
                             saber_string_view _var_name) :
    var_type(_var_type),
    var_name(_var_name)
{}

ckx_func_entry::ckx_func_entry(saber_ptr<ckx_func_type> _func_type,
                               saber_string_view _func_name) :
    func_type(_func_type),
    func_name(_func_name)
{}

ckx_type_entry::ckx_type_entry(saber_ptr<ckx_type> _type,
                               saber_string_view _type_name) :
    type(_type),
    type_name(_type_name)
{}



ckx_env::ckx_env(ckx_env *_parent) :
    parent(_parent)
{}

ckx_env::~ckx_env()
{
    for (auto &v : var_entry_table)   delete v.second;
    for (auto &t : type_entry_table)  delete t.second;
    for (auto &f : func_entry_table)  delete f.second;
}


qpair<ckx_env::add_status, ckx_var_entry*>
ckx_env::add_new_var(saber_string_view _name, saber_ptr<ckx_type> _type)
{
    if ( var_entry_table.find(_name) != var_entry_table.end() )
    {
        return make_pair(add_status::duplicate, nullptr);
    }

    ckx_var_entry *entry = new ckx_var_entry(_type, _name);
    var_entry_table.insert(
        qpair<saber_string_view, ckx_var_entry*>(_name, entry) );

    return make_pair(add_status::success, entry);
}

qpair<ckx_env::add_status, ckx_type_entry*>
ckx_env::add_new_type(saber_string_view _name, saber_ptr<ckx_type> _type)
{
    if ( lookup_type(_name) != nullptr )
    {
         return make_pair(add_status::duplicate, nullptr);
    }

    ckx_type_entry *entry = new ckx_type_entry(_type, _name);
    type_entry_table.insert(
        qpair<saber_string_view, ckx_type_entry*>(_name, entry ) );

    return make_pair(add_status::success, entry);
}

qpair<ckx_env::add_status, ckx_func_entry*>
ckx_env::add_new_func(saber_string_view _name,
                      saber_ptr<ckx_func_type> _type)
{
    if (lookup_func(_name) == nullptr)
    {
        auto it = func_entry_table.emplace(
                      _name, new saber::vector<ckx_func_entry*>());
        ckx_func_entry *ret = new ckx_func_entry(_type, _name);
        it.first->second->emplace_back(ret);
        return qpair<add_status, ckx_func_entry*>(add_status::success, ret);
    }
    else
    {
        return qpair<add_status, ckx_func_entry*>(add_status::duplicate,
                                                  nullptr);
    }
}

bool
ckx_env::lookup_name(saber_string_view _name)
{
    return lookup_var(_name)
           || lookup_type(_name)
           || (!lookup_func(_name));
}

ckx_var_entry*
ckx_env::lookup_var(saber_string_view _name)
{
    ckx_env *this_iter = this;

    while (this_iter != nullptr)
    {
        auto it = var_entry_table.find(_name);

        if (it != var_entry_table.end())
        {
            qpair<const saber_string_view, ckx_var_entry*> &tref = *it;
            return tref.second;
        }

        this_iter = this_iter->parent;
    }

    return nullptr;
}

ckx_type_entry*
ckx_env::lookup_type(saber_string_view _name)
{
    ckx_env *this_iter = this;

    while (this_iter != nullptr)
    {
        auto it = type_entry_table.find(_name);
        if (it != type_entry_table.end())
            return it->second;
        this_iter = this_iter->parent;
    }
    return nullptr;
}

saber::vector<ckx_func_entry*>* ckx_env::lookup_func(saber_string_view _name)
{
    auto it = func_entry_table.find(_name);
    if (it != func_entry_table.end())
        return it->second;
    return nullptr;
}

ckx_var_entry*
ckx_env::lookup_var_local(saber_string_view _name)
{
    auto it = var_entry_table.find(_name);
    return (it == var_entry_table.end()) ? (*it).second : nullptr;
}

} // namespace ckx
