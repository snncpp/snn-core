// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Allocator without state

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/destruct_n.hh"
#include "snn-core/mem/move_construct.hh"
#include "snn-core/mem/relocate.hh"
#include <cstdlib> // free, malloc, realloc
#include <memory>  // allocator

namespace snn::mem
{
    // ## Classes

    // ### optional_allocation

    template <pointer Ptr>
    class optional_allocation final
    {
      public:
        constexpr explicit optional_allocation(Ptr ptr) noexcept
            : ptr_{ptr}
        {
        }

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return ptr_ != nullptr;
        }

        [[nodiscard]] constexpr Ptr value() const
        {
            if (has_value())
            {
                return ptr_;
            }
            throw_or_abort(generic::error::memory_allocation_failure);
        }

        [[nodiscard]] constexpr Ptr value(promise::has_value_t) const noexcept
        {
            snn_assert(ptr_ != nullptr);
            return ptr_;
        }

        [[nodiscard]] constexpr Ptr value_or(Ptr alt) const noexcept
        {
            if (has_value())
            {
                return ptr_;
            }
            return alt;
        }

        [[nodiscard]] constexpr Ptr value_or_nullptr() const noexcept
        {
            return ptr_;
        }

      private:
        Ptr ptr_;
    };

    // ### allocator

    template <typename T>
    class allocator final
    {
      public:
        static_assert(!std::is_array_v<T>, "Array type is currently not supported.");

        // The pointer returned by `malloc` is suitably aligned for any scalar type.
#if SNN_INT128_BOOL
        static_assert(alignof(T) <= alignof(u128), "Alignment must be malloc() compatible.");
#else
        static_assert(alignof(T) <= alignof(u64), "Alignment must be malloc() compatible.");
#endif

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

            if (!std::is_constant_evaluated() && is_trivially_relocatable_v<T>)
            {
                void* const ptr = std::malloc(count.get() * sizeof(T));
                return optional_allocation{static_cast<T*>(ptr)};
            }
            else
            {
                try
                {
                    std::allocator<T> stdalloc;
                    T* const ptr = stdalloc.allocate(count.get());
                    return optional_allocation{ptr};
                }
                catch (...)
                {
                    // Ignore
                }

                return optional_allocation<T*>{nullptr};
            }
        }

        // Does nothing if `ptr` is null.
        constexpr void deallocate(T* const ptr, const usize initial_count) noexcept
        {
            snn_should(ptr != nullptr || initial_count == 0);

            if (!std::is_constant_evaluated() && is_trivially_relocatable_v<T>)
            {
                std::free(ptr);
            }
            else
            {
                if (ptr != nullptr)
                {
                    std::allocator<T> stdalloc;
                    stdalloc.deallocate(ptr, initial_count);
                }
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

            if (new_count.get() > max_count) [[unlikely]]
            {
                return optional_allocation<T*>{nullptr};
            }

            if (!std::is_constant_evaluated() && is_trivially_relocatable_v<T>)
            {
                void* new_ptr = nullptr;

                // Trivially copyable (no destructor), new allocation or no destruction needed?
                if (std::is_trivially_copyable_v<T> || old_ptr == nullptr ||
                    new_count.get() >= use_count) [[likely]]
                {
                    new_ptr = std::realloc(old_ptr, new_count.get() * sizeof(T));
                }
                else
                {
                    new_ptr = std::malloc(new_count.get() * sizeof(T));
                    if (new_ptr != nullptr)
                    {
                        snn_should(new_count.get() < use_count);

                        const usize relocate_count = new_count.get();
                        const usize destruct_count = use_count - relocate_count;

                        mem::relocate(not_null{old_ptr}, not_null{old_ptr + relocate_count},
                                      not_null{static_cast<T*>(new_ptr)});

                        mem::destruct_n(old_ptr + relocate_count, destruct_count);

                        std::free(old_ptr);
                    }
                }

                return optional_allocation{static_cast<T*>(new_ptr)};
            }
            else
            {
                std::allocator<T> stdalloc;

                T* new_ptr = nullptr;

                try
                {
                    new_ptr = stdalloc.allocate(new_count.get());
                }
                catch (...)
                {
                    snn_should(new_ptr == nullptr);
                }

                if (new_ptr != nullptr && old_ptr != nullptr)
                {
                    const usize move_count = math::min(new_count.get(), use_count);
                    mem::move_construct(not_null{old_ptr}, not_null{old_ptr + move_count},
                                        not_null{new_ptr}, promise::no_overlap);

                    mem::destruct_n(old_ptr, use_count);

                    stdalloc.deallocate(old_ptr, initial_count);
                }

                return optional_allocation{new_ptr};
            }
        }
    };
}
