#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include "saber/memory.hpp"
#include "saber/utility.hpp"
#include "saber/algorithm.hpp"
#include "saber/vector.hpp"

namespace saber
{

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
        mem_block *init_block =
            allocator_traits<block_alloc_type>::allocate(alloc, 1);
        blocks.push_back(init_block);
        last_object = reinterpret_cast<T*>(init_block);
    }

    ~object_pool()
    {
        for (mem_block* block : blocks)
        {
            T* storage_begin = reinterpret_cast<T*>(block);
            T* storage_end = block == blocks.back() ?
                                 last_object : storage_begin + block_size;
            destroy(storage_begin, storage_end);
            allocator_traits<block_alloc_type>::deallocate(alloc, block, 1);
        }
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
    void add_block_by_need()
    {
        if (last_object == (reinterpret_cast<T*>(blocks.back())+block_size-1))
        {
            mem_block *new_block =
                allocator_traits<block_alloc_type>::allocate(alloc, 1);
            blocks.push_back(new_block);
            last_object = reinterpret_cast<T*>(new_block);
        }
    }

    static constexpr size_t block_size = 4096;
    using mem_block = aligned_storage_t<sizeof(T) * block_size, alignof(T)>;
    using block_alloc_type =
        typename  Allocator:: template rebind<mem_block>::other;

    T* last_object;

    vector<mem_block*> blocks;
    block_alloc_type alloc;
};

} // namespace saber

#endif // OBJECT_POOL_HPP
