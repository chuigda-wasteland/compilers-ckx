#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include "memory.hpp"
#include "c8assert.hpp"

namespace saber
{

template <typename T>
class optional
{
public:
    optional() : contains_value(false) {}
    optional(optional&& _another)
    {
        if (_another.is_type())
        {
            contains_value = true;
            construct(reinterpret_cast<T*>(&storage), move(_another.get()));
        }
        else
        {
            contains_value = false;
        }
    }

    template <typename ...Args>
    optional(Args&& ..._args)
    {
        contains_value = true;
        construct(reinterpret_cast<T*>(&storage), forward<Args>(_args)...);
    }

    ~optional()
    {
        if (is_type()) destroy_at(reinterpret_cast<T*>(&storage));
    }

    T& get()
    {
        C8ASSERT(is_type());
        return *reinterpret_cast<T*>(&storage);
    }

    bool is_type() const noexcept { return contains_value; }

private:
    bool contains_value;
    aligned_storage_t<sizeof(T), alignof(T)> storage;
};

} // namespace saber

#endif // OPTIONAL_HPP
