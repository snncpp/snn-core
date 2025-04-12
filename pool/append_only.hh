// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Append-only object pool

// Can hold an unlimited number of elements (only limited by available memory).
// Elements are stored in blocks, when a block is full a new one is allocated.
// Elements can only be added, not removed.
// Can hold non-movable types.
// Sacrifices size (of the pool object itself) for simplicity and performance.

#pragma once

#include "snn-core/vec.hh"
#include "snn-core/mem/allocator.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/raw/optimal_size.hh"
#include "snn-core/num/bounded.hh"
#include "snn-core/num/safe.hh"

namespace snn::pool
{
    // ## Classes

    // ### append_only

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T>)
    class append_only final
    {
      public:
        // #### Constants

        static constexpr usize max_elements_per_block = constant::limit<iptrdiff>::max / sizeof(T);

        // #### Explicit constructors

        constexpr explicit append_only(const num::bounded<usize, 1, max_elements_per_block>
                                           min_elements_per_block = 100) noexcept
            : elements_per_block_{optimal_elements_per_block_(min_elements_per_block.not_zero())}
        {
            snn_should(elements_per_block_.get() >= min_elements_per_block.get());
        }

        // #### Copy-constructor/assignment operator

        append_only(const append_only&)            = delete;
        append_only& operator=(const append_only&) = delete;

        // #### Move-constructor/assignment operator

        constexpr append_only(append_only&& other) noexcept
            : blocks_{std::move(other.blocks_)},
              next_{std::exchange(other.next_, nullptr)},
              end_{std::exchange(other.end_, nullptr)},
              back_{std::exchange(other.back_, nullptr)},
              count_{std::exchange(other.count_, 0)},
              elements_per_block_{other.elements_per_block_}
        {
        }

        constexpr append_only& operator=(append_only&& other) noexcept
        {
            swap(other);
            return *this;
        }

        // #### Destructor

        constexpr ~append_only()
        {
            destruct_deallocate_();
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Append

        template <typename... Args>
        constexpr T& append_inplace(Args&&... args)
        {
            if (next_ == end_) [[unlikely]]
            {
                grow_();
            }

            T* p = mem::construct(not_null{next_}, std::forward<Args>(args)...).get();

            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            ++next_;
            snn_should(next_ <= end_);

            SNN_DIAGNOSTIC_POP

            back_ = p;
            ++count_;
            return *p;
        }

        // #### Single element access

        [[nodiscard]] constexpr T& back(promise::not_empty_t) noexcept
        {
            snn_assert(!is_empty());
            snn_should(back_ != nullptr);
            return *back_;
        }

        // #### Count

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return count_ == 0;
        }

        // #### Status

        [[nodiscard]] constexpr usize elements_per_block() const noexcept
        {
            return elements_per_block_.get();
        }

        // #### Swap

        constexpr void swap(append_only& other) noexcept
        {
            // This works even if this == &other.
            blocks_.swap(other.blocks_);
            std::swap(next_, other.next_);
            std::swap(end_, other.end_);
            std::swap(back_, other.back_);
            std::swap(count_, other.count_);
            std::swap(elements_per_block_, other.elements_per_block_);
        }

#if SNN_SHOULD_ENABLED
        [[nodiscard]] constexpr const vec<T*>& blocks() const noexcept // For unit test.
        {
            return blocks_;
        }
#endif

      private:
        vec<T*> blocks_;
        T* next_{nullptr};
        T* end_{nullptr};
        T* back_{nullptr};
        usize count_{0};
        not_zero<usize> elements_per_block_;

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        constexpr void destruct_deallocate_() noexcept
        {
            mem::allocator<T> alloc;

            if (!std::is_constant_evaluated() && std::is_trivially_destructible_v<T>)
            {
                for (T* block : blocks_)
                {
                    alloc.deallocate(block, elements_per_block_.get());
                }
            }
            else if (next_ != nullptr)
            {
                auto rng = blocks_.range();
                snn_should(rng);

                // Current block (last block).
                if (rng)
                {
                    T* const block = rng.pop_back(promise::not_empty);
                    while (next_ > block)
                    {
                        --next_;                        // Step back to the last element.
                        mem::destruct(not_null{next_}); // Call element destructor.
                    }
                    snn_should(next_ == block);
                    alloc.deallocate(block, elements_per_block_.get());
                }

                // Remaining blocks.
                while (rng)
                {
                    T* const block = rng.pop_back(promise::not_empty);
                    next_          = block + elements_per_block_.get();
                    do
                    {
                        --next_;                        // Step back to the last element.
                        mem::destruct(not_null{next_}); // Call element destructor.
                    } while (next_ > block);
                    snn_should(next_ == block);
                    alloc.deallocate(block, elements_per_block_.get());
                }
            }
        }

        constexpr void grow_()
        {
            blocks_.reserve_append(1);

            mem::allocator<T> alloc;
            T* block = alloc.allocate(elements_per_block_).value();

            blocks_.append(block); // Will never throw since we reserved beforehand.
            next_ = block;
            end_  = next_ + elements_per_block_.get();
        }

        SNN_DIAGNOSTIC_POP

        static constexpr auto optimal_elements_per_block_(const not_zero<usize> min) noexcept
        {
            const auto optimal = mem::raw::optimal_size(not_zero{min.get() * sizeof(T)});
            return not_zero{optimal.get() / sizeof(T)};
        }
    };
}
