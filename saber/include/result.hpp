#ifndef RESULT_HPP
#define RESULT_HPP

#include "memory.hpp"
#include <cstdlib>
#include "c8assert.hpp"

namespace saber
{

struct type_t {};
struct err_t {};

template <typename T, typename E>
class result
{
public:
    result(result&& _another)
    {
        if (_another.is_type())
        {
            contains_value = true;
            construct(reinterpret_cast<T*>(&storage), move(_another.value()));
        }
        else
        {
            contains_value = false;
            construct(reinterpret_cast<E*>(&storage), move(_another.err()));
        }
    }

    result(const T& _t)
    {
        contains_value = true;
        construct(reinterpret_cast<T*>(&storage), _t);
    }

    result(const E& _e)
    {
        contains_value = false;
        construct(reinterpret_cast<E*>(&storage), _e);
    }

    result(T&& _t)
    {
        contains_value = true;
        construct(reinterpret_cast<T*>(&storage), move(_t));
    }

    result(E&& _e)
    {
        contains_value = false;
        construct(reinterpret_cast<E*>(&storage), move(_e));
    }

    template <typename ...Args>
    result(type_t, Args&& ..._args)
    {
        contains_value = true;
        construct(reinterpret_cast<T*>(&storage), forward<Args>(_args)...);
    }

    template <typename ...Args>
    result(err_t, Args&& ..._args)
    {
        contains_value = false;
        construct(reinterpret_cast<E*>(&storage), forward<Args>(_args)...);
    }

    ~result()
    {
        if (is_type()) destroy_at(reinterpret_cast<T*>(&storage));
        else destroy_at(reinterpret_cast<E*>(&storage));
    }

    bool is_type() const noexcept { return contains_value; }
    bool is_err() const noexcept { return !is_type(); }

    T& expect(const char* _desc)
    {
        if (!is_type())
        {
            std::printf("%s", _desc);
            std::abort();
        }
        return *reinterpret_cast<T*>(&storage);
    }

    T& unwarp()
    {
        if (!is_type())
            std::abort();
        return *reinterpret_cast<T*>(&storage);
    }

    T& value()
    {
        C8ASSERT(is_type());
        return *reinterpret_cast<T*>(&storage);
    }

    E& err()
    {
        C8ASSERT(is_err());
        return *reinterpret_cast<E*>(&storage);
    }

private:
    static constexpr std::size_t storage_size =
        sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E);
    static constexpr std::size_t storage_align =
        alignof(T) > alignof(E) ? alignof(T) : alignof(E);

    aligned_storage_t<storage_size, storage_align> storage;
    bool contains_value;
};

} // namespace saber

#endif // RESULT_HPP
