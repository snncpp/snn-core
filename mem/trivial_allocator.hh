// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Allocator without state for trivial types

#pragma once

#include "snn-core/math/common.hh"
#include "snn-core/mem/allocator.hh"
#include <cstdlib> // free, malloc, realloc
#include <new>     // nothrow

namespace snn::mem
{
    // ## Classes

    // ### trivial_allocator

    // When constant evaluated the allocated memory is value initialized and destruction is
    // automatic when released.

    // Do not directly or indirectly (e.g. `mem::relocate`) call `mem::construct` or
    // `mem::destruct*` on the allocated memory, simply use `mem::raw::copy` or `mem::raw::move`.

    // When not constant evaluated there is no difference between `allocator<T>` and
    // `trivial_allocator<T>`.

    template <typename T>
        requires(std::is_trivially_copyable_v<T> && std::is_nothrow_default_constructible_v<T>)
    class trivial_allocator final
    {
      public:
        // #### Constants

        static constexpr usize max_count = constant::limit<usize>::max / sizeof(T);

        // #### Allocation/Deallocation

        [[nodiscard]] constexpr optional_allocation<T*> allocate(
            const not_zero<usize> count) noexcept
        {
            if (count.get() > max_count) [[unlikely]]
            {
                return optional_allocation<T*>{nullptr};
            }

            if (std::is_constant_evaluated())
            {
                T* const ptr = ::new (std::nothrow) T[count.get()]{}; // Value init.
                return optional_allocation{ptr};
            }
            else
            {
                void* const ptr = std::malloc(count.get() * sizeof(T));
                return optional_allocation{static_cast<T*>(ptr)};
            }
        }

        // Does nothing if `ptr` is null.
        constexpr void deallocate(T* const ptr, const usize initial_count) noexcept
        {
            snn_should(ptr != nullptr || initial_count == 0);
            ignore_if_unused(initial_count);

            if (std::is_constant_evaluated())
            {
                ::delete[] ptr; // Destruct _and_ release memory.
            }
            else
            {
                std::free(ptr);
            }
        }

        // If this fails the old memory is not deallocated.
        [[nodiscard]] constexpr optional_allocation<T*> reallocate(T* const old_ptr,
                                                                   const usize initial_count,
                                                                   const not_zero<usize> new_count,
                                                                   const usize use_count) noexcept
        {
            snn_should(old_ptr != nullptr || (initial_count == 0 && use_count == 0));
            snn_should(use_count <= initial_count);
            ignore_if_unused(initial_count);

            if (new_count.get() > max_count) [[unlikely]]
            {
                return optional_allocation<T*>{nullptr};
            }

            if (std::is_constant_evaluated())
            {
                T* const new_ptr = ::new (std::nothrow) T[new_count.get()]{}; // Value init.

                if (new_ptr != nullptr && old_ptr != nullptr)
                {
                    const usize copy_count = math::min(new_count.get(), use_count);

                    SNN_DIAGNOSTIC_PUSH
                    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

                    __builtin_memcpy(new_ptr, old_ptr, copy_count * sizeof(T));

                    SNN_DIAGNOSTIC_POP

                    ::delete[] old_ptr; // Destruct _and_ release memory.
                }

                return optional_allocation{new_ptr};
            }
            else
            {
                void* const new_ptr = std::realloc(old_ptr, new_count.get() * sizeof(T));
                return optional_allocation{static_cast<T*>(new_ptr)};
            }
        }
    };
}
