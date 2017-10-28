#ifndef LLVM_IMPLICIT_LIST_HPP
#define LLVM_IMPLICIT_LIST_HPP

namespace faker
{

class llvm_implicit_list_node
{
public:
    llvm_implicit_list_node() : llvm_implicit_list_node(nullptr, nullptr) {}
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
    llvm_implicit_list_node *prev, *next;
};

} // namespace faker

#endif // LLVM_IMPLICIT_LIST_HPP
