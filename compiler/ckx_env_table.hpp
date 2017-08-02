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

#include "defs.hpp"

namespace ckx
{

using saber::saber_ptr;

open_class ckx_table_entry
{
    enum class category: char
    {
        entry_data,
        entry_type
    } entry_category;

    variant
    {
        pod_struct
        {
            saber_ptr<ckx_type> data_type;
        } data;

        pod_struct
        {
            saber_ptr<ckx_type> ref;
        } type;
    } v;

    saber_ptr<saber::string> name;
};

open_class ckx_env_table
{
    saber::vector<ckx_table_entry*> entries;
    ckx_env_table *parent;
};

} // namespace ckx

#endif // CKX_ENV_TABLE_HPP
