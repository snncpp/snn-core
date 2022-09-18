// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Fixed object pool

// Can hold up to a fixed number of elements.
// The capacity is set when the pool is constructed and can not be changed after that.
// Can hold non-movable types.
// Contiguous storage.

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/mem/allocator.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct.hh"
#include "snn-core/mem/destruct_n.hh"

namespace snn::pool
{
    // ## Classes

    // ### fixed

    template <typename T>
    class fixed final
    {
      public:
        // #### Types

        using iterator       = T*;
        using const_iterator = const T*;

        // #### Explicit constructors

        constexpr explicit fixed(const not_zero<usize> capacity)
            : data_{nullptr},
              count_{0},
              capacity_{capacity.get()}
        {
            mem::allocator<T> alloc;
            data_ = alloc.allocate(capacity).value();
        }

        // #### Copy constructor/copy assignment operator

        fixed(const fixed&)            = delete; // Non-copyable
        fixed& operator=(const fixed&) = delete; // Non-copyable

        // #### Move constructor/move assignment operator

        constexpr fixed(fixed&& other) noexcept
            : data_{std::exchange(other.data_, nullptr)},
              count_{std::exchange(other.count_, 0)},
              capacity_{std::exchange(other.capacity_, 0)}
        {
        }

        constexpr fixed& operator=(fixed&& other) noexcept
        {
            swap(other);
            return *this;
        }

        // #### Destructor

        constexpr ~fixed()
        {
            mem::destruct_n(data_, count_);

            mem::allocator<T> alloc;
            alloc.deallocate(data_, capacity_); // Does nothing if nullptr (moved from).
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
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

        [[nodiscard]] constexpr bool is_full() const noexcept
        {
            // Count should never be over capacity but a `>=` comparison can help the optimizer.
            return count_ >= capacity_;
        }

        // #### Capacity

        [[nodiscard]] constexpr usize capacity() const noexcept
        {
            return capacity_;
        }

        // #### Single element access

        [[nodiscard]] constexpr T& at(const usize pos, promise::within_bounds_t) noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        [[nodiscard]] constexpr const T& at(const usize pos,
                                            promise::within_bounds_t) const noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        [[nodiscard]] constexpr T& back(promise::not_empty_t) noexcept
        {
            snn_assert(count_ > 0);
            return data_[count_ - 1];
        }

        [[nodiscard]] constexpr T& front(promise::not_empty_t) noexcept
        {
            snn_assert(count_ > 0);
            return data_[0];
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return begin_();
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return end_();
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return cbegin_();
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return cend_();
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return begin();
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return end();
        }

        // #### Operations

        constexpr T& append(T value)
        {
            if (count_ < capacity_)
            {
                T* p = mem::construct(not_null{data_ + count_}, std::move(value)).get();
                ++count_;
                return *p;
            }
            throw_or_abort(generic::error::insufficient_capacity);
        }

        template <typename... Args>
        constexpr T& append_inplace(Args&&... args)
        {
            if (count_ < capacity_)
            {
                T* p = mem::construct(not_null{data_ + count_}, std::forward<Args>(args)...).get();
                ++count_;
                return *p;
            }
            throw_or_abort(generic::error::insufficient_capacity);
        }

        constexpr void drop_back(promise::not_empty_t) noexcept
        {
            snn_assert(count_ > 0);
            --count_;
            mem::destruct(not_null{data_ + count_});
        }

        // #### Swap

        constexpr void swap(fixed& other) noexcept
        {
            // This works even if this == &other.
            std::swap(data_, other.data_);
            std::swap(count_, other.count_);
            std::swap(capacity_, other.capacity_);
        }

#if SNN_SHOULD_BOOL
        [[nodiscard]] constexpr const T* block() const noexcept // For unit test.
        {
            return data_;
        }
#endif

      private:
        T* data_;
        usize count_;
        usize capacity_;

        constexpr T* begin_() noexcept
        {
            return data_;
        }

        constexpr T* end_() noexcept
        {
            // Returns nullptr if data_ is nullptr and count_ is 0 (well-defined in C++).
            return data_ + count_;
        }

        constexpr const T* cbegin_() const noexcept
        {
            return data_;
        }

        constexpr const T* cend_() const noexcept
        {
            // Returns nullptr if data_ is nullptr and count_ is 0 (well-defined in C++).
            return data_ + count_;
        }
    };
}
