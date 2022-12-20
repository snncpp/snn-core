// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Vector with optional small-capacity

// * Optional small-capacity (on stack).
// * Trivially-relocatable optimization.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/exception.hh"
#include "snn-core/contiguous_interface.hh"
#include "snn-core/detail/vec/common.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/copy_construct.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct.hh"
#include "snn-core/mem/destruct_n.hh"
#include "snn-core/mem/relocate_left.hh"
#include "snn-core/mem/relocate_right.hh"
#include "snn-core/range/contiguous.hh"

namespace snn
{
    // ## Classes

    // ### vec

    template <typename T, usize SmallCapacity = 0>
        requires(std::is_move_constructible_v<T> && std::is_same_v<std::remove_cvref_t<T>, T>)
    class vec final : public contiguous_interface<vec<T, SmallCapacity>>
    {
      private:
        using buffer_type = detail::vec::buffer<T, SmallCapacity>;

      public:
        // #### Types

        using value_type = T;

        using iterator       = T*;
        using const_iterator = const T*;

        using pointer       = T*;
        using const_pointer = const T*;

        using reference       = T&;
        using const_reference = const T&;

        using trivially_relocatable_type = trivially_relocatable_if_t<vec, buffer_type>;

        // #### Default constructor

        constexpr vec() noexcept = default;

        // #### Explicit constructors

        constexpr explicit vec(container::reserve_t, const usize capacity)
            : vec{}
        {
            reserve(capacity);
        }

        constexpr explicit vec(meta::iterators_t, const const_iterator first,
                               const const_iterator last)
            : vec{}
        {
            snn_should(first == last || (first != nullptr && last != nullptr && last > first));

            const auto count = static_cast<usize>(last - first);
            if (count > 0)
            {
                reserve(count);
                mem::copy_construct(not_null{first}, not_null{last}, not_null{buf_.begin()},
                                    promise::no_overlap);
                buf_.set_count(count, promise::has_capacity);
            }
        }

        // #### Converting constructors

        constexpr vec(initializer_list<T> init)
            : vec{}
        {
            if (init.size() > 0)
            {
                reserve(init.size());
                mem::copy_construct(not_null{init.begin()}, not_null{init.end()},
                                    not_null{buf_.begin()}, promise::no_overlap);
                buf_.set_count(init.size(), promise::has_capacity);
            }
        }

        // #### Copy/move-assignment/constructor

        constexpr vec(const vec& other)
            : vec{}
        {
            append(other);
        }

        constexpr vec& operator=(const vec& other)
        {
            if (this != &other)
            {
                clear();
                append(other);
            }
            return *this;
        }

        constexpr vec(vec&& other) noexcept
            : buf_{std::move(other.buf_)}
        {
        }

        constexpr vec& operator=(vec&& other) noexcept
        {
            swap(other);
            return *this;
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return buf_.begin();
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return buf_.end();
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return buf_.cbegin();
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return buf_.cend();
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return buf_.cbegin();
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return buf_.cend();
        }

        // #### Single element access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) noexcept(
            std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < buf_.count())
            {
                return buf_.begin()[pos];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < buf_.count())
            {
                return buf_.cbegin()[pos];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference at(const usize pos, promise::within_bounds_t) noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(pos < buf_.count());
            return buf_.begin()[pos];
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   promise::within_bounds_t) const noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(pos < buf_.count());
            return buf_.cbegin()[pos];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            const usize c = buf_.count();
            if (c > 0)
            {
                return buf_.begin()[c - 1];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            const usize c = buf_.count();
            if (c > 0)
            {
                return buf_.cbegin()[c - 1];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference back(promise::not_empty_t) noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(buf_.count() > 0);
            return buf_.begin()[buf_.count() - 1];
        }

        [[nodiscard]] constexpr const_reference back(promise::not_empty_t) const noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(buf_.count() > 0);
            return buf_.cbegin()[buf_.count() - 1];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front() noexcept(
            std::is_nothrow_copy_constructible_v<R>)
        {
            if (buf_.count() > 0)
            {
                return buf_.begin()[0];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (buf_.count() > 0)
            {
                return buf_.cbegin()[0];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference front(promise::not_empty_t) noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(buf_.count() > 0);
            return buf_.begin()[0];
        }

        [[nodiscard]] constexpr const_reference front(promise::not_empty_t) const noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(buf_.count() > 0);
            return buf_.cbegin()[0];
        }

        // #### Append value

        constexpr void append(T value)
        {
            if (buf_.count() == buf_.capacity()) [[unlikely]]
            {
                buf_.grow(not_zero{check_capacity_(recommend_capacity_())}, promise::is_valid);
            }
            mem::construct(not_null{buf_.end()}, std::move(value));
            buf_.increment_count(promise::has_capacity);
        }

        template <typename... Args>
        constexpr void append_inplace(Args&&... args)
        {
            if (buf_.count() < buf_.capacity()) [[likely]]
            {
                mem::construct(not_null{buf_.end()}, std::forward<Args>(args)...);
                buf_.increment_count(promise::has_capacity);
            }
            else
            {
                // Arguments could come from within self, use slow grow path.
                buf_.grow_append_inplace(not_zero{check_capacity_(recommend_capacity_())},
                                         promise::is_valid, std::forward<Args>(args)...);
            }
        }

        // #### Append another vec

        constexpr void append(const same_as<vec> auto& other)
        {
            // Note: Other could be *this.
            if (other.count() > 0)
            {
                reserve_append(other.count());
                mem::copy_construct(not_null{other.cbegin()}, not_null{other.cend()},
                                    not_null{buf_.end()}, promise::no_overlap);
                buf_.set_count(buf_.count() + other.count(), promise::has_capacity);
            }
        }

        // #### Count/Size

        [[nodiscard]] constexpr snn::byte_size byte_size() const noexcept
        {
            return snn::byte_size{sizeof(T) * buf_.count()};
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return buf_.count();
        }

        template <strict_integral_min<64> Int>
        [[nodiscard]] constexpr Int count() const noexcept
        {
            // This can't overflow `i64` (57-bit-virtual-address-space).
            return static_cast<Int>(buf_.count());
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return buf_.count() == 0;
        }

        [[nodiscard]] constexpr usize size() const noexcept
            requires(sizeof(T) == 1) // Use `count()` or `byte_size()` instead.
        {
            return buf_.count();
        }

        // #### Capacity

        [[nodiscard]] constexpr usize capacity() const noexcept
        {
            return buf_.capacity();
        }

        [[nodiscard]] static constexpr usize default_capacity() noexcept
        {
            return SmallCapacity;
        }

        constexpr void reserve(const usize capacity)
        {
            if (capacity > buf_.capacity())
            {
                buf_.grow(not_zero{check_capacity_(math::max(capacity, recommend_capacity_()))},
                          promise::is_valid);
            }
        }

        constexpr void reserve_append(const usize append_count)
        {
            reserve(math::add_with_saturation(buf_.count(), append_count));
        }

        // #### Raw data access

        [[nodiscard]] constexpr const_pointer data() const noexcept
        {
            return buf_.cbegin();
        }

        // pointer data() noexcept
        // {
        //     // Not implemented.
        //     // Use writable(), which is more readable and easier to audit.
        // }

        [[nodiscard]] constexpr pointer writable() noexcept
        {
            return buf_.begin();
        }

        // #### Range

        [[nodiscard]] constexpr auto range() noexcept
        {
            return view().range();
        }

        [[nodiscard]] constexpr auto range() const noexcept
        {
            return view().range();
        }

        // #### View

        [[nodiscard]] constexpr auto view() noexcept
        {
            return array_view<T>{buf_.begin(), buf_.count()};
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<const T>{buf_.cbegin(), buf_.count()};
        }

        [[nodiscard]] constexpr auto view(const usize pos,
                                          const usize count = constant::npos) noexcept
        {
            return view().view(pos, count);
        }

        [[nodiscard]] constexpr auto view(const usize pos,
                                          const usize count = constant::npos) const noexcept
        {
            return view().view(pos, count);
        }

        // #### Modifiers

        constexpr void clear() noexcept
        {
            buf_.clear();
        }

        constexpr void insert_at(const usize pos, T value)
        {
            if (pos < buf_.count())
            {
                if (buf_.count() == buf_.capacity()) [[unlikely]]
                {
                    buf_.grow(not_zero{check_capacity_(recommend_capacity_())}, promise::is_valid);
                }

                mem::relocate_right(not_null{buf_.begin() + pos}, not_null{buf_.end()},
                                    not_zero<usize>{1});

                mem::construct(not_null{buf_.begin() + pos}, std::move(value)); // Must not throw.
                buf_.increment_count(promise::has_capacity);
            }
            else if (pos == buf_.count())
            {
                append(std::move(value));
            }
            else
            {
                throw_or_abort(generic::error::invalid_position_for_replace_or_insert);
            }
        }

        constexpr void drop_at(const usize pos, const usize count) noexcept
        {
            if (pos < buf_.count() && count > 0)
            {
                drop_at_(pos, count);
            }
        }

        constexpr void drop_back(promise::not_empty_t) noexcept
        {
            snn_assert(buf_.count() > 0);
            buf_.decrement_count(promise::not_empty);
            mem::destruct(not_null{buf_.end()});
        }

        constexpr void drop_back_n(const usize count) noexcept
        {
            drop_back_n_(math::min(count, buf_.count()));
        }

        constexpr void truncate(const usize count) noexcept
        {
            if (count < buf_.count())
            {
                drop_back_n_(buf_.count() - count);
            }
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr usize count(V&& value) const
        {
            return view().count(std::forward<V>(value));
        }

        // #### Swap

        constexpr void swap(vec& other) noexcept
        {
            // This works even if this == &other.
            buf_.swap(other.buf_);
        }

        // #### Stream append (insertion operator)

        constexpr vec& operator<<(T value)
        {
            append(std::move(value));
            return *this;
        }

        constexpr vec& operator<<(const same_as<vec> auto& other)
        {
            append(other);
            return *this;
        }

        // #### Comparison

        constexpr bool operator==(const vec& other) const noexcept
        {
            return view() == other.view();
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.

      private:
        buffer_type buf_;

        static constexpr usize check_capacity_(const usize capacity)
        {
            if (capacity <= detail::vec::max_capacity<T>)
            {
                return capacity;
            }
            throw_or_abort(generic::error::capacity_would_exceed_max_capacity);
        }

        constexpr usize recommend_capacity_() const noexcept
        {
            // Always return a capacity greater than the current count.
            // This can't overflow `usize`, count is always `<= constant::limit<iptrdiff>::max`.
            return buf_.count() + ((buf_.count() + 2) / 2); // ~1.5x
        }

        constexpr void drop_at_(const usize pos, const usize count) noexcept
        {
            snn_should(pos < buf_.count() && count > 0);

            const usize remaining_count = buf_.count() - pos;
            if (count >= remaining_count)
            {
                drop_back_n_(remaining_count);
            }
            else
            {
                mem::destruct_n(buf_.begin() + pos, count);
                mem::relocate_left(not_null{buf_.begin() + pos + count}, not_null{buf_.end()},
                                   not_zero{count});
                buf_.set_count(buf_.count() - count, promise::has_capacity);
            }
        }

        constexpr void drop_back_n_(usize count) noexcept
        {
            snn_should(count <= buf_.count());
            buf_.set_count(buf_.count() - count, promise::has_capacity);
            mem::destruct_n(buf_.end(), count);
        }
    };

    // ## Functions

    // ### swap

    template <typename T, usize SmallCapacity>
    constexpr void swap(vec<T, SmallCapacity>& a, vec<T, SmallCapacity>& b) noexcept
    {
        a.swap(b);
    }
}
