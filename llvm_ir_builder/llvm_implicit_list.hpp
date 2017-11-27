/**
    The Opensource Compiler CKX -- for my honey ChenKX
    [[ The self-written "Faker" LLVM Builder part ]]
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

#ifndef LLVM_IMPLICIT_LIST_HPP
#define LLVM_IMPLICIT_LIST_HPP

namespace faker
{

class llvm_implicit_list_node
{
public:
    llvm_implicit_list_node() {}
    ~llvm_implicit_list_node() {}

    llvm_implicit_list_node* get_prev() noexcept { return prev; }
    llvm_implicit_list_node* get_next() noexcept { return next; }

    void insert_self_before(llvm_implicit_list_node *_another) noexcept
    {
        this->prev = _another->prev;
        _another->next = this->next;
        this->next = _another;
        _another->prev = this;
    }

    void insert_self_after(llvm_implicit_list_node* _another) noexcept
    {
        this->next = _another->next;
        _another->prev = this->prev;
        this->prev = _another;
        _another->next = this;
    }

private:
    llvm_implicit_list_node *prev = nullptr, *next = nullptr;
};

} // namespace faker

#endif // LLVM_IMPLICIT_LIST_HPP
