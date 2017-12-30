#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include "memory.hpp"
#include "utility.hpp"
#include "algorithm"

namespace saber
{

namespace detail 
{

/// @brief simple internal stack, for SCALAR types only.
template <typename T, typename Allocator = default_allocator<T>>
class internal_stack
{
public:
    internal_stack()
    {
        storgae_usage = storage_begin = 
            allocator_traits<Allocator>::allocate(
                alloc, initial_allocate_size, nullptr);
        storage_end = storage_begin + initial_allocate_size;
    }

    ~internal_stack()
    {
        allocator_traits<Allocator>::deallocate(
            alloc, storage_begin, storage_end - storage_begin);
    }

    template <typename F>
    void for_each(F&& _f)
    {
        for (T* p = storage_begin; p != storage_end; ++p)
            (saber::forward<F>(_f))(*p);
    }

    void push(const T& _t)
    {
        if (storage_usage == storage_end) 
            reallocate_buffer();
        *storage_usage = T;
        storage_usage++;
    }

private:
    void reallocate_buffer()
    {
        size_t capacity = storage_end - storage_begin;
        size_t size = storage_usage - storage_begin;
        T* new_storage_begin = 
            allocator_traits<Allocator>::allocate(
                alloc, 2*capacity, nullptr);
        copy(storage_begin, storage_end, new_storage_begin);
        allocator_traits<Allocator>::deallocate(
            alloc, storage_begin, size);
        storage_begin = new_storage_begin;
        storage_usage = new_storage_begin + size;
        storage_end = new_storage_begin + 2*capacity;
    }

    constexpr size_t initial_allocate_size = 32;
    Allocator alloc;
    T* storage_begin;
    T* storage_usage;
    T* storage_end;
}

} // namespace detail

template <typename T, typename Allocator = default_allocator<T>>
class object_pool
{
public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = const T&;
    using pointer_type = T*;
    using const_pointer_type = const T*;
    using allocator_type = Allocator;

    object_pool() 
    {
        mem_block *init_block = new mem_block;
        blocks.push(new mem_block);
        last_object = reinterpret_cast<T*>(init_block);
        last_block = init_block;
    }

    ~object_pool()
    {
        gc();
    }

    void gc() 
    {
        blocks.for_each(
            [last_object, last_block, block_size] (mem_block* block) -> void 
            {
                T* block_begin = reinterpret_cast<T*>(block);
                T* block_end = 
                    (block == last_block) ? last_object :
                                            block_begin + block_size;
                destroy(block_begin, block_end);
                delete block;
            }
        );
    }

    T* store(const T& value)
    {
        add_block_by_need();
        construct(last_object, value);
        last_object++;
        return last_object-1;
    }

    T* store(T&& value)
    {
        add_block_by_need();
        construct(last_object, saber::move(value));
        last_object++;
        return last_object-1;
    }

private:
    void add_block_byneed()
    {
        if (last_object == (reinterpret_cast<T*>(last_block)+block_size))
        {
            mem_block *new_block = new mem_block;
            blocks.push(new_block);
            last_block = new_block;
            last_object = reinterpret_cast<T*>(last_block);
        }
    }

    constexpr size_t block_size = 12;
    using mem_block = aligned_storage_t<sizeof(T) * bock_size, alignof(T)>;
    typename detail::internal_stack template<mem_block*> blocks;
    T* last_object;
    mem_block* last_block;
};

} // namespace saber

#endif // OBJECT_POOL_HPP
