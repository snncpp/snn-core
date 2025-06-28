// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Array view with `[c]strview` specializations

// A non-owning view of a contiguous sequence of objects. With a static or a dynamic count.

// * Can be compared (deep comparison).
// * `array_view<T, ...>` is mutable and `array_view<const T, ...>` is immutable.
// * `array_view<[const] char, ...>` have string like functionality.
// * `array_view<[const] char, dynamic_count>` specializations never hold null pointers.
// * `strview` is an alias for `array_view<char, dynamic_count>`.
// * `cstrview` is an alias for `array_view<const char, dynamic_count>`.

#pragma once

#include "snn-core/array.hh"
#include "snn-core/array_view.fwd.hh"
#include "snn-core/contiguous_interface.hh"
#include "snn-core/optional.hh"
#include "snn-core/optional_index.hh"
#include "snn-core/strcore.fwd.hh"
#include "snn-core/val_or_ref.hh"
#include "snn-core/algo/is_sorted.hh"
#include "snn-core/algo/reverse.hh"
#include "snn-core/algo/sort.fwd.hh"
#include "snn-core/ascii/to_integral.hh"
#include "snn-core/ascii/to_integral_prefix.hh"
#include "snn-core/detail/array_view/common.hh"
#include "snn-core/detail/cityhash64/hash.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/raw/compare.hh"
#include "snn-core/mem/raw/copy.hh"
#include "snn-core/mem/raw/fill.hh"
#include "snn-core/mem/raw/is_equal.hh"
#include "snn-core/mem/raw/is_overlapping.hh"
#include "snn-core/mem/raw/load.hh"
#include "snn-core/mem/raw/load_swap.hh"
#include "snn-core/mem/raw/move.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/forward.hh"
#include "snn-core/range/contiguous.fwd.hh"
#include "snn-core/string/size.hh"

namespace snn
{
    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    // ## Classes

    // ### array_view

    // Static count that is known at compile time.

    template <typename T, usize Count>
    class array_view final
    {
      public:
        // #### Types

        using element_type = T;
        using value_type   = std::remove_cv_t<T>;

        using iterator       = T*;
        using const_iterator = const T*;

        using pointer       = T*;
        using const_pointer = const T*;

        using reference       = T&;
        using const_reference = const T&;

        // #### Default constructor

        constexpr array_view() noexcept
            requires(Count == 0)
        = default;

        // #### Explicit constructors

        constexpr explicit array_view(val_or_ref<T&> ref) noexcept
            requires(Count == 1)
            : data_{&ref.get()}
        {
        }

        constexpr explicit array_view(const pointer data, assume::has_capacity_t) noexcept
            : data_{data}
        {
        }

        constexpr explicit array_view(const not_null<pointer> data, assume::has_capacity_t) noexcept
            : data_{data.get()}
        {
        }

        // #### Converting constructors

        template <typename U, usize C>
            requires(C != constant::dynamic_count && C >= Count &&
                     std::is_convertible_v<U*, iterator>)
        constexpr array_view(array_view<U, C> v) noexcept
            : data_{v.begin()}
        {
        }

        template <typename U, usize C>
            requires(C >= Count && std::is_convertible_v<U*, iterator>)
        constexpr array_view(array<U, C>& a) noexcept
            : data_{a.begin()}
        {
        }

        template <typename U, usize C>
            requires(C >= Count && std::is_convertible_v<const U*, iterator>)
        constexpr array_view(const array<U, C>& a) noexcept
            : data_{a.begin()}
        {
        }

        template <typename U, usize C>
            requires(C >= Count && std::is_convertible_v<U*, iterator>)
        array_view(const array<U, C>&&) = delete; // Temporary, use view<>() if safe.

        template <same_as<const char> ConstChar, usize N>
            requires(Count == N - 1 && std::is_same_v<T, const char>)
        constexpr array_view(ConstChar (&s)[N]) noexcept
            : data_{s}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return data_ + Count;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return data_ + Count;
        }

        // #### Count/Size

        [[nodiscard]] static constexpr snn::byte_size<usize> byte_size() noexcept
        {
            return snn::byte_size<usize>{sizeof(value_type) * Count};
        }

        [[nodiscard]] static constexpr usize count() noexcept
        {
            return Count;
        }

        [[nodiscard]] static constexpr bool is_dynamic() noexcept
        {
            return false;
        }

        [[nodiscard]] static constexpr bool is_empty() noexcept
        {
            return Count == 0;
        }

        [[nodiscard]] static constexpr usize size() noexcept
            requires(sizeof(value_type) == 1) // Use `count()` or `byte_size()` instead.
        {
            return Count;
        }

        // #### Data

        [[nodiscard]] constexpr const_pointer data() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr pointer writable() noexcept
            requires(!std::is_const_v<T>)
        {
            return data_;
        }

        // #### Single element access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos)
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < Count)
            {
                return data_[pos];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < Count)
            {
                return data_[pos];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference at(const strict_integral auto pos,
                                             assume::within_bounds_t) noexcept
        {
            if constexpr (detail::can_overflow_count<decltype(pos), Count>())
            {
                snn_assert(pos >= 0 && force_unsigned(pos) < Count);
            }
            return data_[pos];
        }

        [[nodiscard]] constexpr const_reference at(const strict_integral auto pos,
                                                   assume::within_bounds_t) const noexcept
        {
            if constexpr (detail::can_overflow_count<decltype(pos), Count>())
            {
                snn_assert(pos >= 0 && force_unsigned(pos) < Count);
            }
            return data_[pos];
        }

        [[nodiscard]] constexpr reference at(const strict_integral auto pos,
                                             bounds::mask_t) noexcept
            requires(power_of_two<Count>)
        {
            return data_[force_unsigned(pos) & (Count - 1u)];
        }

        [[nodiscard]] constexpr const_reference at(const strict_integral auto pos,
                                                   bounds::mask_t) const noexcept
            requires(power_of_two<Count>)
        {
            return data_[force_unsigned(pos) & (Count - 1u)];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return data_[Count - 1];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return data_[Count - 1];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference back(assume::not_empty_t) noexcept
            requires(Count > 0)
        {
            return data_[Count - 1];
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
            requires(Count > 0)
        {
            return data_[Count - 1];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front()
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return data_[0];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return data_[0];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference front(assume::not_empty_t) noexcept
            requires(Count > 0)
        {
            return data_[0];
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
            requires(Count > 0)
        {
            return data_[0];
        }

        template <usize Index>
            requires(Index < Count)
        [[nodiscard]] constexpr reference get() noexcept
        {
            return data_[Index];
        }

        template <usize Index>
            requires(Index < Count)
        [[nodiscard]] constexpr const_reference get() const noexcept
        {
            return data_[Index];
        }

        // #### Load (word)

        template <strict_unsigned_integral UInt>
            requires(octet<T> && sizeof(UInt) <= Count)
        [[nodiscard]] constexpr UInt load() const noexcept
        {
            return mem::raw::load<UInt>(data_);
        }

        // #### Load-swap (word)

        template <strict_unsigned_integral UInt>
            requires(octet<T> && sizeof(UInt) <= Count)
        [[nodiscard]] constexpr UInt load_swap() const noexcept
        {
            return mem::raw::load_swap<UInt>(data_);
        }

        // #### View - Dynamic count

        [[nodiscard]] constexpr auto view() noexcept
        {
            if constexpr (Count > 0)
            {
                return array_view<T>{not_null{data_}, Count};
            }
            return array_view<T>{data_, Count};
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            if constexpr (Count > 0)
            {
                return array_view<const T>{not_null{data_}, Count};
            }
            return array_view<const T>{data_, Count};
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

        // #### View - Static count

        template <usize Pos = 0, usize C = constant::npos>
        [[nodiscard]] constexpr auto view() noexcept
        {
            constexpr usize FinalPos   = math::min(Pos, Count);
            constexpr usize FinalCount = math::min(C, Count - FinalPos);
            return array_view<T, FinalCount>{data_ + FinalPos, assume::has_capacity};
        }

        template <usize Pos = 0, usize C = constant::npos>
        [[nodiscard]] constexpr auto view() const noexcept
        {
            constexpr usize FinalPos   = math::min(Pos, Count);
            constexpr usize FinalCount = math::min(C, Count - FinalPos);
            return array_view<const T, FinalCount>{data_ + FinalPos, assume::has_capacity};
        }

        // #### View exactly

        template <usize Pos, usize C>
            requires(Pos <= Count && (Count - Pos) >= C)
        [[nodiscard]] constexpr auto view_exactly() noexcept
        {
            return array_view<T, C>{data_ + Pos, assume::has_capacity};
        }

        template <usize Pos, usize C>
            requires(Pos <= Count && (Count - Pos) >= C)
        [[nodiscard]] constexpr auto view_exactly() const noexcept
        {
            return array_view<const T, C>{data_ + Pos, assume::has_capacity};
        }

        template <usize Pos, usize C>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) noexcept
        {
            static_assert(Pos <= Count && (Count - Pos) >= C);
            return array_view<T, C>{data_ + Pos, assume::has_capacity};
        }

        template <usize Pos, usize C>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) const noexcept
        {
            static_assert(Pos <= Count && (Count - Pos) >= C);
            return array_view<const T, C>{data_ + Pos, assume::has_capacity};
        }

        // #### View offset - Dynamic count

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) noexcept
        {
            return view().view_offset(pos, count);
        }

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) const noexcept
        {
            return view().view_offset(pos, count);
        }

        // #### View offset - Static count

        template <isize Pos, isize C = constant::limit<isize>::max>
        [[nodiscard]] constexpr auto view_offset() noexcept
        {
            constexpr auto p = detail::array_view::from_offset(Count, Pos, C);
            return view<p.position, p.count>();
        }

        template <isize Pos, isize C = constant::limit<isize>::max>
        [[nodiscard]] constexpr auto view_offset() const noexcept
        {
            constexpr auto p = detail::array_view::from_offset(Count, Pos, C);
            return view<p.position, p.count>();
        }

        // #### As bytes - Dynamic count

        [[nodiscard]] auto as_bytes() noexcept
        {
            return view().as_bytes();
        }

        [[nodiscard]] auto as_bytes() const noexcept
        {
            return view().as_bytes();
        }

        // #### As bytes - Static count

        template <typename = void>
        [[nodiscard]] auto as_bytes() noexcept
        {
            using byte_type = std::conditional_t<std::is_const_v<T>, const byte, byte>;
            return array_view<byte_type, byte_size().get()>{reinterpret_cast<byte_type*>(data_),
                                                            assume::has_capacity};
        }

        template <typename = void>
        [[nodiscard]] auto as_bytes() const noexcept
        {
            return array_view<const byte, byte_size().get()>{reinterpret_cast<const byte*>(data_),
                                                             assume::has_capacity};
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

        // #### Operations

        template <typename V>
        constexpr void fill(const V& value)
        {
            for (usize i = 0; i < Count; ++i)
            {
                data_[i] = value;
            }
        }

        template <typename V, usize C>
        constexpr usize fill_front(const array_view<V, C> values)
        {
            const usize fill_count = Count < values.count() ? Count : values.count();
            for (usize i = 0; i < fill_count; ++i)
            {
                data_[i] = values.begin()[i];
            }
            return fill_count;
        }

        template <typename U>
        constexpr void overwrite(const array_view<U, Count> other)
        {
            for (usize i = 0; i < Count; ++i)
            {
                data_[i] = other.begin()[i];
            }
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr bool contains(const V& needle) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (data_[i] == needle)
                {
                    return true;
                }
            }
            return false;
        }

        template <typename V>
        [[nodiscard]] constexpr usize count(const V& needle) const
        {
            usize needle_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (data_[i] == needle)
                {
                    ++needle_count;
                }
            }
            return needle_count;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr usize count_if(OneArgPred p) const
        {
            usize pred_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(data_[i])))
                {
                    ++pred_count;
                }
            }
            return pred_count;
        }

        template <typename V>
        [[nodiscard]] constexpr optional_index find(const V& value, const usize start_pos = 0) const
        {
            for (usize i = start_pos; i < Count; ++i)
            {
                if (data_[i] == value)
                {
                    return i;
                }
            }
            return constant::npos;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr optional_index find_if(OneArgPred p,
                                                       const usize start_pos = 0) const
        {
            for (usize i = start_pos; i < Count; ++i)
            {
                if (p(std::as_const(data_[i])))
                {
                    return i;
                }
            }
            return constant::npos;
        }

        template <typename V>
        [[nodiscard]] constexpr optional_index find_in_reverse(
            const V& value, const usize start_pos = constant::npos) const
        {
            usize i = Count;
            if (start_pos < i)
            {
                i = start_pos + 1;
            }
            while (i > 0)
            {
                --i;
                if (data_[i] == value)
                {
                    return i;
                }
            }
            return constant::npos;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr optional_index find_in_reverse_if(
            OneArgPred p, const usize start_pos = constant::npos) const
        {
            usize i = Count;
            if (start_pos < i)
            {
                i = start_pos + 1;
            }
            while (i > 0)
            {
                --i;
                if (p(std::as_const(data_[i])))
                {
                    return i;
                }
            }
            return constant::npos;
        }

        // #### Comparison

        template <typename V>
        [[nodiscard]] constexpr bool has_front(const V& value) const
        {
            return Count > 0 && data_[0] == value;
        }

        template <typename V>
        [[nodiscard]] constexpr bool has_back(const V& value) const
        {
            return Count > 0 && data_[Count - 1] == value;
        }

        // #### Overlaps

        template <usize C>
        [[nodiscard]] constexpr bool overlaps(const array_view<const T, C> v) const noexcept
        {
            return mem::raw::is_overlapping(v.begin(), v.end(), begin(), end());
        }

        // #### Algorithm helpers

        template <typename OneArgFn>
        constexpr void each(OneArgFn f)
        {
            for (usize i = 0; i < Count; ++i)
            {
                f(data_[i]);
            }
        }

        template <typename OneArgFn>
        constexpr void each(OneArgFn f) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                f(std::as_const(data_[i]));
            }
        }

        template <typename OneArgFn>
        constexpr void each_in_reverse(OneArgFn f)
        {
            usize i = Count;
            while (i > 0)
            {
                --i;
                f(data_[i]);
            }
        }

        template <typename OneArgFn>
        constexpr void each_in_reverse(OneArgFn f) const
        {
            usize i = Count;
            while (i > 0)
            {
                --i;
                f(std::as_const(data_[i]));
            }
        }

        template <typename TwoArgPred = fn::less_than>
        [[nodiscard]] constexpr bool is_sorted(TwoArgPred is_less = TwoArgPred{}) const
        {
            return algo::is_sorted(range(), std::move(is_less));
        }

        template <typename U, typename V>
        constexpr usize replace(const U& needle, const V& replacement)
        {
            usize replace_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (data_[i] == needle)
                {
                    data_[i] = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        template <typename OneArgPred, typename V>
        constexpr usize replace_if(OneArgPred p, const V& replacement)
        {
            usize replace_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(data_[i])))
                {
                    data_[i] = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        constexpr void reverse()
        {
            algo::reverse(range());
        }

        template <typename TwoArgPred = fn::less_than>
        constexpr void sort(TwoArgPred is_less = TwoArgPred{})
        {
            algo::sort(range(), std::move(is_less));
        }

        template <typename OneArgOp>
        constexpr void transform(OneArgOp op)
        {
            for (usize i = 0; i < Count; ++i)
            {
                data_[i] = op(std::as_const(data_[i]));
            }
        }

        template <typename U, typename TwoArgOp>
        constexpr void transform_with(const array_view<U, Count> other, TwoArgOp op)
        {
            for (usize i = 0; i < Count; ++i)
            {
                data_[i] = op(std::as_const(data_[i]), other.begin()[i]);
            }
        }

        // #### Validation helpers

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool all(OneArgPred p) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (!p(std::as_const(data_[i])))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool any(OneArgPred p) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(data_[i])))
                {
                    return true;
                }
            }
            return false;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool none(OneArgPred p) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(data_[i])))
                {
                    return false;
                }
            }
            return true;
        }

      private:
        pointer data_{nullptr};
    };

    // ### array_view<T>

    // Dynamic count.

    template <typename T>
    class array_view<T> final : public contiguous_interface<array_view<T>>
    {
      public:
        // #### Types

        using element_type = T;
        using value_type   = std::remove_cv_t<T>;

        using iterator       = T*;
        using const_iterator = const T*;

        using pointer       = T*;
        using const_pointer = const T*;

        using reference       = T&;
        using const_reference = const T&;

        // #### Default constructor

        constexpr array_view() noexcept = default;

        // #### Explicit constructors

        constexpr explicit array_view(val_or_ref<T&> ref) noexcept
            : data_{&ref.get()},
              count_{1}
        {
        }

        template <convertible_to<pointer> Ptr>
        constexpr explicit array_view(range::contiguous<Ptr> rng) noexcept
            : data_{rng.begin()},
              count_{rng.count()}
        {
        }

        constexpr explicit array_view(const pointer data, const usize count) noexcept
            : data_{data},
              count_{count}
        {
        }

        constexpr explicit array_view(const not_null<pointer> data, const usize count) noexcept
            : data_{data.get()},
              count_{count}
        {
        }

        constexpr explicit array_view(const pointer first, assume::null_terminated_t) noexcept
            : data_{first},
              count_{count_null_terminated_(first)}
        {
        }

        constexpr explicit array_view(const not_null<pointer> first,
                                      assume::null_terminated_t) noexcept
            : data_{first.get()},
              count_{count_null_terminated_(first.get())}
        {
        }

        constexpr explicit array_view(init::from_t, const iterator first,
                                      const iterator last) noexcept
            : data_{first},
              count_{to_usize(last - first)}
        {
            snn_should(first == last || (first != nullptr && last != nullptr && first < last));
        }

        // #### Converting constructors

        template <typename U, usize C>
            requires(std::is_convertible_v<U*, iterator>)
        constexpr array_view(array_view<U, C> v) noexcept
            : data_{v.begin()},
              count_{v.count()}
        {
        }

        template <typename U, usize C>
            requires(std::is_convertible_v<U*, iterator>)
        constexpr array_view(array<U, C>& a) noexcept
            : data_{a.begin()},
              count_{a.count()}
        {
        }

        template <typename U, usize C>
            requires(std::is_convertible_v<const U*, iterator>)
        constexpr array_view(const array<U, C>& a) noexcept
            : data_{a.begin()},
              count_{a.count()}
        {
        }

        template <typename U, usize C>
            requires(std::is_convertible_v<U*, iterator>)
        array_view(const array<U, C>&&) = delete; // Temporary, use view() if safe.

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return count_ != 0;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return data_ + count_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return data_ + count_;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return data_ + count_;
        }

        // #### Count/Size

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{sizeof(value_type) * count_};
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] static constexpr bool is_dynamic() noexcept
        {
            return true;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return count_ == 0;
        }

        [[nodiscard]] constexpr usize size() const noexcept
            requires(sizeof(value_type) == 1) // Use `count()` or `byte_size()` instead.
        {
            return count_;
        }

        // #### Data

        [[nodiscard]] constexpr const_pointer data() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr pointer writable() noexcept
            requires(!std::is_const_v<T>)
        {
            return data_;
        }

        // #### Single element access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos)
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < count_)
            {
                return data_[pos];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < count_)
            {
                return data_[pos];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference at(const usize pos, assume::within_bounds_t) noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   assume::within_bounds_t) const noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (count_)
            {
                return data_[count_ - 1];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (count_)
            {
                return data_[count_ - 1];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference back(assume::not_empty_t) noexcept
        {
            snn_assert(count_ != 0);
            return data_[count_ - 1];
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
        {
            snn_assert(count_ != 0);
            return data_[count_ - 1];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front()
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (count_)
            {
                return data_[0];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (count_)
            {
                return data_[0];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference front(assume::not_empty_t) noexcept
        {
            snn_assert(count_ != 0);
            return data_[0];
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
        {
            snn_assert(count_ != 0);
            return data_[0];
        }

        // #### View

        [[nodiscard]] constexpr auto view() noexcept
        {
            return array_view<T>{data_, count_};
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<const T>{data_, count_};
        }

        [[nodiscard]] constexpr auto view(const usize pos,
                                          const usize count = constant::npos) noexcept
        {
            const usize i = math::min(pos, count_);
            return array_view<T>{data_ + i, math::min(count_ - i, count)};
        }

        [[nodiscard]] constexpr auto view(const usize pos,
                                          const usize count = constant::npos) const noexcept
        {
            const usize i = math::min(pos, count_);
            return array_view<const T>{data_ + i, math::min(count_ - i, count)};
        }

        // #### View exactly

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) noexcept
        {
            static_assert(Count != constant::dynamic_count);
            snn_assert(Pos <= count_ && (count_ - Pos) >= Count);
            return array_view<T, Count>{data_ + Pos, assume::has_capacity};
        }

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) const noexcept
        {
            static_assert(Count != constant::dynamic_count);
            snn_assert(Pos <= count_ && (count_ - Pos) >= Count);
            return array_view<const T, Count>{data_ + Pos, assume::has_capacity};
        }

        // #### View offset

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) noexcept
        {
            const auto p = detail::array_view::from_offset(count_, pos, count);
            return view(p.position, p.count);
        }

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) const noexcept
        {
            const auto p = detail::array_view::from_offset(count_, pos, count);
            return view(p.position, p.count);
        }

        // #### As bytes

        [[nodiscard]] auto as_bytes() noexcept
        {
            using byte_type = std::conditional_t<std::is_const_v<T>, const byte, byte>;
            return array_view<byte_type>{reinterpret_cast<byte_type*>(data_), byte_size().get()};
        }

        [[nodiscard]] auto as_bytes() const noexcept
        {
            return array_view<const byte>{reinterpret_cast<const byte*>(data_), byte_size().get()};
        }

        // #### Range

        [[nodiscard]] constexpr auto range() noexcept
        {
            return range::contiguous<pointer>{init::from, data_, data_ + count_};
        }

        [[nodiscard]] constexpr auto range() const noexcept
        {
            return range::contiguous<const_pointer>{init::from, data_, data_ + count_};
        }

        // #### Modifiers

        constexpr void clear() noexcept
        {
            count_ = 0;
        }

        // Drop up to N elements from the front.

        constexpr void drop_front_n(const usize count) noexcept
        {
            const usize c = math::min(count, count_);
            data_ += c;
            count_ -= c;
        }

        // Drop up to N elements from the back.

        constexpr void drop_back_n(const usize count) noexcept
        {
            count_ -= math::min(count, count_);
        }

        constexpr void truncate(const usize count) noexcept
        {
            count_ = math::min(count, count_);
        }

        // #### Operations

        template <typename V>
        constexpr void fill(const V& value)
        {
            for (usize i = 0; i < count_; ++i)
            {
                data_[i] = value;
            }
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr usize count(const V& needle) const
        {
            usize needle_count = 0;
            for (usize i = 0; i < count_; ++i)
            {
                if (data_[i] == needle)
                {
                    ++needle_count;
                }
            }
            return needle_count;
        }

        template <typename V>
        [[nodiscard]] constexpr optional_index find(const V& value, const usize start_pos = 0) const
        {
            for (usize i = start_pos; i < count_; ++i)
            {
                if (data_[i] == value)
                {
                    return i;
                }
            }
            return constant::npos;
        }

        template <typename V>
        [[nodiscard]] constexpr optional_index find_in_reverse(
            const V& value, const usize start_pos = constant::npos) const
        {
            usize i = count_;
            if (start_pos < i)
            {
                i = start_pos + 1;
            }
            while (i > 0)
            {
                --i;
                if (data_[i] == value)
                {
                    return i;
                }
            }
            return constant::npos;
        }

        // #### Comparison

        template <typename V>
        [[nodiscard]] constexpr bool has_front(const V& value) const
        {
            return count_ != 0 && data_[0] == value;
        }

        template <typename V>
        [[nodiscard]] constexpr bool has_back(const V& value) const
        {
            return count_ != 0 && data_[count_ - 1] == value;
        }

        // #### Overlaps

        [[nodiscard]] constexpr bool overlaps(const array_view<const T> v) const noexcept
        {
            return mem::raw::is_overlapping(v.cbegin(), v.cend(), cbegin(), cend());
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.

      private:
        pointer data_{nullptr};
        usize count_{};

        static constexpr usize count_null_terminated_(const pointer ptr) noexcept
        {
            if (ptr != nullptr)
            {
                usize i = 0;
                while (ptr[i] != 0)
                {
                    ++i;
                }
                return i;
            }
            return 0;
        }
    };

    namespace detail
    {
        template <typename T, typename Ptr>
        concept std_contiguous_container_convertible_to = requires(T& v) {
            requires unsigned_integral<typename T::size_type>;
            { v.data() } -> convertible_to<Ptr>;
            { v.size() } -> same_as<typename T::size_type>;
        };
    }

    // ### array_view<const char>

    // Dynamic count.
    // Alias: `cstrview`

    template <>
    class array_view<const char> final : public contiguous_interface<cstrview>
    {
      public:
        // #### Types

        using element_type = const char;
        using value_type   = char;

        using iterator       = const char*;
        using const_iterator = const char*;

        using pointer       = const char*;
        using const_pointer = const char*;

        using reference       = const char&;
        using const_reference = const char&;

        // #### Default constructor

        constexpr array_view() noexcept = default;

        // #### Explicit constructors

        constexpr explicit array_view(val_or_ref<const char&> ref) noexcept
            : data_{&ref.get()},
              count_{1}
        {
        }

        template <character Char>
        constexpr explicit array_view(range::contiguous<Char*> rng) noexcept
            : data_{rng.begin()},
              count_{rng.count()}
        {
        }

        constexpr explicit array_view(const const_pointer data, const usize count) noexcept
            : data_{replace_if_nullptr_(data)},
              count_{count}
        {
            snn_should(data != nullptr || count == 0);
        }

        constexpr explicit array_view(const not_null<const_pointer> data,
                                      const usize count) noexcept
            : data_{data.get()},
              count_{count}
        {
        }

        constexpr explicit array_view(const const_pointer first, assume::null_terminated_t) noexcept
            : data_{replace_if_nullptr_(first)},
              count_{string::size(first, assume::null_terminated)}
        {
        }

        constexpr explicit array_view(const not_null<const_pointer> first,
                                      assume::null_terminated_t) noexcept
            : data_{first.get()},
              count_{string::size(first, assume::null_terminated)}
        {
        }

        template <detail::std_contiguous_container_convertible_to<const char*> StdContainer>
        constexpr explicit array_view(init::from_t, const StdContainer& c) noexcept
            : array_view{c.data(), c.size()}
        {
        }

        template <typename StdContainer>
        explicit array_view(init::from_t, const StdContainer&&) = delete;

        constexpr explicit array_view(init::from_t, const const_iterator first,
                                      const const_iterator last) noexcept
            : data_{replace_if_nullptr_(first)},
              count_{to_usize(last - first)}
        {
            snn_should(first == last || (first != nullptr && last != nullptr && first < last));
        }

        // #### Converting constructors

        template <character Char, usize Count>
        constexpr array_view(array_view<Char, Count> v) noexcept
            : data_{v.begin()},
              count_{v.count()}
        {
            if constexpr (Count == 0)
            {
                data_ = replace_if_nullptr_(data_);
            }
        }

        template <usize Count>
        constexpr array_view(const array<char, Count>& a) noexcept
            : data_{a.data().get()},
              count_{a.count()}
        {
        }

        template <usize Count>
        array_view(const array<char, Count>&&) = delete; // Temporary, use view() if safe.

        template <same_as<const char> ConstChar, usize N>
        constexpr array_view(ConstChar (&s)[N]) noexcept
            : data_{s},
              count_{N - 1}
        {
        }

        template <typename Buf>
        constexpr array_view(const strcore<Buf>& s) noexcept
            : array_view{s.view()}
        {
        }

        template <typename Buf>
        array_view(const strcore<Buf>&&) = delete; // Temporary, use view() if safe.

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return count_ != 0;
        }

        // #### Iterators

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return data_ + count_;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return data_ + count_;
        }

        // #### Count/Size

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{count_};
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] static constexpr bool is_dynamic() noexcept
        {
            return true;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return count_ == 0;
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return count_;
        }

        // #### Data

        [[nodiscard]] constexpr auto data() const noexcept
        {
            return not_null<const_pointer>{data_};
        }

        // #### Single element access

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const noexcept
        {
            if (pos < count_)
            {
                return data_[pos];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   assume::within_bounds_t) const noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const noexcept
        {
            if (count_)
            {
                return data_[count_ - 1];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
        {
            snn_assert(count_ != 0);
            return data_[count_ - 1];
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const noexcept
        {
            if (count_)
            {
                return data_[0];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
        {
            snn_assert(count_ != 0);
            return data_[0];
        }

        // #### View

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return *this;
        }

        [[nodiscard]] constexpr cstrview view(const usize pos,
                                              const usize count = constant::npos) const noexcept
        {
            const usize i = math::min(pos, count_);
            return cstrview{init::internal, data_ + i, math::min(count_ - i, count)};
        }

        // #### View exactly

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) const noexcept
        {
            static_assert(Count != constant::dynamic_count);
            snn_assert(Pos <= count_ && (count_ - Pos) >= Count);
            return array_view<const char, Count>{data_ + Pos, assume::has_capacity};
        }

        // #### View offset

        [[nodiscard]] constexpr cstrview view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) const noexcept
        {
            const auto p = detail::array_view::from_offset(count_, pos, count);
            return view(p.position, p.count);
        }

        // #### As bytes

        [[nodiscard]] auto as_bytes() const noexcept
        {
            return array_view<const byte>{not_null{reinterpret_cast<const byte*>(data_)}, count_};
        }

        // #### Range

        template <same_as<const_pointer> Ptr = const_pointer>
        [[nodiscard]] constexpr auto range() const noexcept
        {
            // `range::contiguous` has only been declared, not defined.
            return range::contiguous<Ptr>{*this};
        }

        // #### To

        template <brace_constructible_from<const_pointer, usize> T>
        [[nodiscard]] constexpr T to() const noexcept(noexcept(T{const_pointer{}, usize{}}))
        {
            // Note: `data_` is never `nullptr` here.
            return T{data_, count_};
        }

        template <strict_integral Int, math::base Base = math::base::decimal>
        [[nodiscard]] constexpr optional<Int> to() const noexcept
        {
            return ascii::to_integral<Int, Base>(range::forward{init::from, begin(), end()});
        }

        // #### To prefix

        template <strict_integral Int, math::base Base = math::base::decimal, usize MaxDigits = 0>
        [[nodiscard]] constexpr pair::value_count<Int, usize> to_prefix(
            ascii::leading_zeros policy = ascii::leading_zeros::disallow) const noexcept
        {
            return ascii::to_integral_prefix<Int, Base, MaxDigits>(
                range::forward{init::from, begin(), end()}, policy);
        }

        // #### Modifiers

        constexpr void clear() noexcept
        {
            count_ = 0;
        }

        constexpr void drop_front_n(const usize count) noexcept
        {
            const usize c = math::min(count, count_);
            data_ += c;
            count_ -= c;
        }

        constexpr void drop_back_n(const usize count) noexcept
        {
            count_ -= math::min(count, count_);
        }

        constexpr void truncate(const usize count) noexcept
        {
            count_ = math::min(count, count_);
        }

        // #### Search

        [[nodiscard]] constexpr usize count(const same_as<char> auto needle) const noexcept
        {
            usize needle_count = 0;
            for (usize i = 0; i < count_; ++i)
            {
                if (data_[i] == needle)
                {
                    ++needle_count;
                }
            }
            return needle_count;
        }

        [[nodiscard]] constexpr usize count(const transient<cstrview> needle) const noexcept
        {
            const cstrview ndl = needle.get();

            if (ndl.is_empty())
            {
                return count_ + 1;
            }

            usize needle_count = 0;
            usize start_pos    = 0;
            while (true)
            {
                start_pos =
                    detail::array_view::find(data_, count_, ndl.data_, ndl.count_, start_pos)
                        .value_or_npos();
                if (start_pos == constant::npos)
                {
                    break;
                }
                start_pos += ndl.count();
                ++needle_count;
            }
            return needle_count;
        }

        [[nodiscard]] constexpr optional_index find(const same_as<char> auto c,
                                                    const usize start_pos = 0) const noexcept
        {
            return detail::array_view::find(data_, count_, c, start_pos);
        }

        [[nodiscard]] constexpr optional_index find(const transient<cstrview> needle,
                                                    const usize start_pos = 0) const noexcept
        {
            const cstrview ndl = needle.get();
            return detail::array_view::find(data_, count_, ndl.data_, ndl.count_, start_pos);
        }

        [[nodiscard]] constexpr optional_index find_in_reverse(
            const same_as<char> auto c, const usize start_pos = constant::npos) const noexcept
        {
            return detail::array_view::find_in_reverse(data_, count_, c, start_pos);
        }

        [[nodiscard]] constexpr optional_index find_in_reverse(
            const transient<cstrview> needle, const usize start_pos = constant::npos) const noexcept
        {
            const cstrview ndl = needle.get();
            return detail::array_view::find_in_reverse(data_, count_, ndl.data_, ndl.count_,
                                                       start_pos);
        }

        // #### Hash

        [[nodiscard]] constexpr usize hash() const noexcept
        {
            return detail::cityhash64::hash(data_, count_);
        }

        // #### Comparison

        [[nodiscard]] constexpr bool has_front(const same_as<char> auto c) const noexcept
        {
            return count_ != 0 && data_[0] == c;
        }

        [[nodiscard]] constexpr bool has_front(const transient<cstrview> s) const noexcept
        {
            const cstrview sv = s.get();
            if (sv.count() <= count_)
            {
                return mem::raw::is_equal(data(), sv.data(), sv.byte_size());
            }
            return false;
        }

        [[nodiscard]] constexpr bool has_back(const same_as<char> auto c) const noexcept
        {
            return count_ != 0 && data_[count_ - 1] == c;
        }

        [[nodiscard]] constexpr bool has_back(const transient<cstrview> s) const noexcept
        {
            const cstrview sv = s.get();
            if (sv.count() <= count_)
            {
                const usize pos = count_ - sv.count();
                return mem::raw::is_equal(not_null{data_ + pos}, sv.data(), sv.byte_size());
            }
            return false;
        }

        // #### Overlaps

        [[nodiscard]] constexpr bool overlaps(const transient<cstrview> s) const noexcept
        {
            const cstrview sv = s.get();
            return mem::raw::is_overlapping(sv.cbegin(), sv.cend(), cbegin(), cend());
        }

        template <typename Buf>
        [[nodiscard]] constexpr bool overlaps(const strcore<Buf>& s) const noexcept
        {
            // This checks if any part of the strcore overlaps, even unused capacity.
            return s.overlaps(*this);
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.

      private:
        const_pointer data_{&detail::array_view::single_char};
        usize count_{};

        friend class array_view<char>;

        constexpr explicit array_view(init::internal_t, const const_pointer data,
                                      const usize count) noexcept
            : data_{data},
              count_{count}
        {
        }

        static constexpr const_pointer replace_if_nullptr_(const const_pointer ptr) noexcept
        {
            return ptr != nullptr ? ptr : &detail::array_view::single_char;
        }
    };

    // ### array_view<char>

    // Dynamic count.
    // Alias: `strview`

    template <>
    class array_view<char> final : public contiguous_interface<strview>
    {
      public:
        // #### Types

        using element_type = char;
        using value_type   = char;

        using iterator       = char*;
        using const_iterator = const char*;

        using pointer       = char*;
        using const_pointer = const char*;

        using reference       = char&;
        using const_reference = const char&;

        // #### Default constructor

        constexpr array_view() noexcept = default;

        // #### Explicit constructors

        constexpr explicit array_view(val_or_ref<char&> ref) noexcept
            : data_{&ref.get()},
              count_{1}
        {
        }

        template <convertible_to<pointer> Ptr>
        constexpr explicit array_view(range::contiguous<Ptr> rng) noexcept
            : data_{rng.begin()},
              count_{rng.count()}
        {
        }

        constexpr explicit array_view(const pointer data, const usize count) noexcept
            : data_{replace_if_nullptr_(data)},
              count_{count}
        {
            snn_should(data != nullptr || count == 0);
        }

        constexpr explicit array_view(const not_null<pointer> data, const usize count) noexcept
            : data_{data.get()},
              count_{count}
        {
        }

        constexpr explicit array_view(const pointer first, assume::null_terminated_t) noexcept
            : data_{replace_if_nullptr_(first)},
              count_{string::size(first, assume::null_terminated)}
        {
        }

        constexpr explicit array_view(const not_null<pointer> first,
                                      assume::null_terminated_t) noexcept
            : data_{first.get()},
              count_{string::size(first, assume::null_terminated)}
        {
        }

        template <detail::std_contiguous_container_convertible_to<char*> StdContainer>
        constexpr explicit array_view(init::from_t, StdContainer& c) noexcept
            : array_view{c.data(), c.size()}
        {
        }

        template <typename StdContainer>
        explicit array_view(init::from_t, const StdContainer&&) = delete;

        constexpr explicit array_view(init::from_t, const iterator first,
                                      const iterator last) noexcept
            : data_{replace_if_nullptr_(first)},
              count_{to_usize(last - first)}
        {
            snn_should(first == last || (first != nullptr && last != nullptr && first < last));
        }

        // #### Converting constructors

        template <usize Count>
        constexpr array_view(array_view<char, Count> v) noexcept
            : data_{v.begin()},
              count_{v.count()}
        {
            if constexpr (Count == 0)
            {
                data_ = replace_if_nullptr_(data_);
            }
        }

        template <usize Count>
        constexpr array_view(array<char, Count>& a) noexcept
            : data_{a.writable().get()},
              count_{a.count()}
        {
        }

        template <typename Buf>
        array_view(const strcore<Buf>&) = delete; // Error prone, use view() for a writable view.

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return count_ != 0;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return data_ + count_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return data_ + count_;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return data_;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return data_ + count_;
        }

        // #### Count/Size

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{count_};
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] static constexpr bool is_dynamic() noexcept
        {
            return true;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return count_ == 0;
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return count_;
        }

        // #### Data

        [[nodiscard]] constexpr auto data() const noexcept
        {
            return not_null<const_pointer>{data_};
        }

        [[nodiscard]] constexpr auto writable() noexcept
        {
            return not_null<pointer>{data_};
        }

        // #### Single element access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) noexcept
        {
            if (pos < count_)
            {
                return data_[pos];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const noexcept
        {
            if (pos < count_)
            {
                return data_[pos];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference at(const usize pos, assume::within_bounds_t) noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   assume::within_bounds_t) const noexcept
        {
            snn_assert(pos < count_);
            return data_[pos];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept
        {
            if (count_)
            {
                return data_[count_ - 1];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const noexcept
        {
            if (count_)
            {
                return data_[count_ - 1];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference back(assume::not_empty_t) noexcept
        {
            snn_assert(count_ != 0);
            return data_[count_ - 1];
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
        {
            snn_assert(count_ != 0);
            return data_[count_ - 1];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front() noexcept
        {
            if (count_)
            {
                return data_[0];
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const noexcept
        {
            if (count_)
            {
                return data_[0];
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference front(assume::not_empty_t) noexcept
        {
            snn_assert(count_ != 0);
            return data_[0];
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
        {
            snn_assert(count_ != 0);
            return data_[0];
        }

        // #### View

        [[nodiscard]] constexpr strview view() noexcept
        {
            return strview{init::internal, data_, count_};
        }

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return cstrview{init::internal, data_, count_};
        }

        [[nodiscard]] constexpr strview view(const usize pos,
                                             const usize count = constant::npos) noexcept
        {
            const usize i = math::min(pos, count_);
            return strview{init::internal, data_ + i, math::min(count_ - i, count)};
        }

        [[nodiscard]] constexpr cstrview view(const usize pos,
                                              const usize count = constant::npos) const noexcept
        {
            const usize i = math::min(pos, count_);
            return cstrview{init::internal, data_ + i, math::min(count_ - i, count)};
        }

        // #### View exactly

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) noexcept
        {
            static_assert(Count != constant::dynamic_count);
            snn_assert(Pos <= count_ && (count_ - Pos) >= Count);
            return array_view<char, Count>{data_ + Pos, assume::has_capacity};
        }

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(assume::within_bounds_t) const noexcept
        {
            static_assert(Count != constant::dynamic_count);
            snn_assert(Pos <= count_ && (count_ - Pos) >= Count);
            return array_view<const char, Count>{data_ + Pos, assume::has_capacity};
        }

        // #### View offset

        [[nodiscard]] constexpr strview view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) noexcept
        {
            const auto p = detail::array_view::from_offset(count_, pos, count);
            return view(p.position, p.count);
        }

        [[nodiscard]] constexpr cstrview view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) const noexcept
        {
            const auto p = detail::array_view::from_offset(count_, pos, count);
            return view(p.position, p.count);
        }

        // #### As bytes

        [[nodiscard]] auto as_bytes() noexcept
        {
            return array_view<byte>{not_null{reinterpret_cast<byte*>(data_)}, count_};
        }

        [[nodiscard]] auto as_bytes() const noexcept
        {
            return array_view<const byte>{not_null{reinterpret_cast<const byte*>(data_)}, count_};
        }

        // #### Range

        template <same_as<pointer> Ptr = pointer>
        [[nodiscard]] constexpr auto range() noexcept
        {
            // `range::contiguous` has only been declared, not defined.
            return range::contiguous<Ptr>{view()};
        }

        template <same_as<const_pointer> Ptr = const_pointer>
        [[nodiscard]] constexpr auto range() const noexcept
        {
            // `range::contiguous` has only been declared, not defined.
            return range::contiguous<Ptr>{view()};
        }

        // #### To

        template <brace_constructible_from<pointer, usize> T>
        [[nodiscard]] constexpr T to() noexcept(noexcept(T{pointer{}, usize{}}))
        {
            // Note: `data_` is never `nullptr` here.
            return T{data_, count_};
        }

        template <brace_constructible_from<const_pointer, usize> T>
        [[nodiscard]] constexpr T to() const noexcept(noexcept(T{const_pointer{}, usize{}}))
        {
            // Note: `data_` is never `nullptr` here.
            return T{const_pointer{data_}, count_};
        }

        template <strict_integral Int, math::base Base = math::base::decimal>
        [[nodiscard]] constexpr optional<Int> to() const noexcept
        {
            return ascii::to_integral<Int, Base>(range::forward{init::from, begin(), end()});
        }

        // #### To prefix

        template <strict_integral Int, math::base Base = math::base::decimal, usize MaxDigits = 0>
        [[nodiscard]] constexpr pair::value_count<Int, usize> to_prefix(
            ascii::leading_zeros policy = ascii::leading_zeros::disallow) const noexcept
        {
            return ascii::to_integral_prefix<Int, Base, MaxDigits>(
                range::forward{init::from, begin(), end()}, policy);
        }

        // #### Modifiers

        constexpr void clear() noexcept
        {
            count_ = 0;
        }

        constexpr void drop_front_n(const usize count) noexcept
        {
            const usize c = math::min(count, count_);
            data_ += c;
            count_ -= c;
        }

        constexpr void drop_back_n(const usize count) noexcept
        {
            count_ -= math::min(count, count_);
        }

        constexpr void truncate(const usize count) noexcept
        {
            count_ = math::min(count, count_);
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr usize count(V&& value) const noexcept
        {
            return view().count(std::forward<V>(value));
        }

        // #### Operations

        constexpr void fill(const same_as<char> auto c) noexcept
        {
            mem::raw::fill(writable(), byte_size(), to_byte(c));
        }

        constexpr void fill(const transient<cstrview> s) noexcept
        {
            const cstrview sv = s.get();
            if (sv.count() >= 2)
            {
                char* dst      = data_;
                usize dst_size = count_;

                // First copy, might be 0 bytes and can overlap.
                snn::byte_size<usize> bsize{math::min(dst_size, sv.size())};
                mem::raw::move(sv.data(), not_null{dst}, bsize);

                const char* src = dst;
                usize src_size  = bsize.get();

                dst += bsize.get();
                dst_size -= bsize.get();

                while (dst_size > 0)
                {
                    // Nth copy, is never 0 bytes and never overlaps.
                    bsize = snn::byte_size<usize>{math::min(dst_size, src_size)};
                    mem::raw::copy(not_null{src}, not_null{dst}, bsize, assume::no_overlap);
                    dst += bsize.get();
                    dst_size -= bsize.get();
                    src_size += bsize.get();
                }
            }
            else if (sv)
            {
                fill(sv.front(assume::not_empty));
            }
        }

        // #### Hash

        [[nodiscard]] constexpr usize hash() const noexcept
        {
            return detail::cityhash64::hash(data_, count_);
        }

        // #### Overlaps

        [[nodiscard]] constexpr bool overlaps(const transient<cstrview> s) const noexcept
        {
            const cstrview sv = s.get();
            return mem::raw::is_overlapping(sv.cbegin(), sv.cend(), cbegin(), cend());
        }

        template <typename Buf>
        [[nodiscard]] constexpr bool overlaps(const strcore<Buf>& s) const noexcept
        {
            // This checks if any part of the strcore overlaps, even unused capacity.
            return s.overlaps(view());
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.

      private:
        pointer data_{&detail::array_view::single_char};
        usize count_{};

        constexpr explicit array_view(init::internal_t, const pointer data,
                                      const usize count) noexcept
            : data_{data},
              count_{count}
        {
        }

        static constexpr pointer replace_if_nullptr_(const pointer ptr) noexcept
        {
            return ptr != nullptr ? ptr : &detail::array_view::single_char;
        }
    };

    // ## Deduction guides

    // ### array_view

    template <typename T, usize Count>
    array_view(array<T, Count>&) -> array_view<T, Count>;

    template <typename T, usize Count>
    array_view(const array<T, Count>&) -> array_view<const T, Count>;

    template <typename T>
    array_view(val_or_ref<T&>) -> array_view<T, 1>;

    template <usize N>
    array_view(const char (&)[N]) -> array_view<const char, N - 1>;

    template <typename T>
    array_view(range::contiguous<T*>) -> array_view<T, constant::dynamic_count>;

    template <typename T>
    array_view(T*, usize) -> array_view<T, constant::dynamic_count>;

    template <typename T>
    array_view(not_null<T*>, usize) -> array_view<T, constant::dynamic_count>;

    template <typename T>
    array_view(T*, assume::null_terminated_t) -> array_view<T, constant::dynamic_count>;

    template <typename T>
    array_view(not_null<T*>, assume::null_terminated_t) -> array_view<T, constant::dynamic_count>;

    template <typename T>
    array_view(init::from_t, T*, T*) -> array_view<T, constant::dynamic_count>;

    // ## Functions

    // ### Comparison

    template <typename A, usize Ca, typename B, usize Cb>
        requires(std::is_same_v<const A, const B>)
    constexpr bool operator==(const array_view<A, Ca> left, const array_view<B, Cb> right)
    {
        if (left.count() == right.count())
        {
            if constexpr (left.is_dynamic() && right.is_dynamic() &&
                          std::is_same_v<const A, const char>)
            {
                // Dynamic char, can't be nullptr.
                return mem::raw::is_equal(left.data(), right.data(), left.byte_size());
            }
            else
            {
                if (left.count() == 0)
                {
                    return true;
                }

                if (!std::is_constant_evaluated() && is_strict_integral_v<A>)
                {
                    return mem::raw::is_equal(not_null{left.data()}, not_null{right.data()},
                                              left.byte_size());
                }
                else
                {
                    usize i = 0;
                    do
                    {
                        if (left.begin()[i] != right.begin()[i])
                        {
                            return false;
                        }
                        ++i;
                    } while (i < left.count());
                    return true;
                }
            }
        }
        return false;
    }

    template <character Char, usize C, usize N>
    constexpr bool operator==(const array_view<Char, C> left, const char (&right)[N]) noexcept
    {
        constexpr usize Size = N - 1;
        // Data can be nullptr.
        return left.size() == Size &&
               (Size == 0 || mem::raw::is_equal<Size>(not_null{left.data()}, right));
    }

    template <typename A, usize Ca, typename B, usize Cb>
        requires(std::is_same_v<const A, const B>)
    constexpr auto operator<=>(const array_view<A, Ca> left, const array_view<B, Cb> right)
        -> decltype(left.front(assume::not_empty) <=> right.front(assume::not_empty))
    {
        const usize min_count = math::min(left.count(), right.count());
        if constexpr (left.is_dynamic() && right.is_dynamic() &&
                      std::is_same_v<const A, const char>)
        {
            // Dynamic char, can't be nullptr.
            const auto cmp =
                mem::raw::compare(left.data(), right.data(), byte_size{min_count}) <=> 0;
            if (cmp != 0)
            {
                return cmp;
            }
        }
        else
        {
            for (usize i = 0; i < min_count; ++i)
            {
                const auto cmp = left.begin()[i] <=> right.begin()[i];
                if (cmp != 0)
                {
                    return cmp;
                }
            }
        }
        return left.count() <=> right.count();
    }

    template <character Char, usize C, usize N>
    constexpr std::strong_ordering operator<=>(const array_view<Char, C> left,
                                               const char (&right)[N]) noexcept
    {
        return left <=> cstrview{right};
    }

    // ### as_bytes

    // Unlike the member function `.as_bytes()`, the free-standing `as_bytes(...)` will always
    // return an `array_view` with a static count. Use `as_bytes(...).as_bytes()` to get an
    // `array_view` with a dynamic count.

    template <typename T>
    [[nodiscard]] auto as_bytes(val_or_ref<T&> ref) noexcept
    {
        using byte_type = std::conditional_t<std::is_const_v<T>, const byte, byte>;
        return array_view<byte_type, sizeof(T)>{reinterpret_cast<byte_type*>(&ref.get()),
                                                assume::has_capacity};
    }

    // ### get

    template <usize Index, typename T, usize Count>
        requires(Count != constant::dynamic_count && Index < Count)
    [[nodiscard]] constexpr T& get(array_view<T, Count>& v) noexcept
    {
        return v.begin()[Index];
    }

    template <usize Index, typename T, usize Count>
        requires(Count != constant::dynamic_count && Index < Count)
    [[nodiscard]] constexpr const T& get(const array_view<T, Count>& v) noexcept
    {
        return v.begin()[Index];
    }

    SNN_DIAGNOSTIC_POP
}

// ## Specializations

// ### std::hash

template <snn::character Char>
struct std::hash<snn::array_view<Char>>
{
    [[nodiscard]] constexpr std::size_t operator()(const snn::array_view<Char> s) const noexcept
    {
        return s.hash();
    }
};

// ### std::tuple_size

template <typename T, std::size_t Count>
    requires(Count != snn::constant::dynamic_count)
struct std::tuple_size<snn::array_view<T, Count>>
    : public std::integral_constant<std::size_t, Count>
{
};

// ### std::tuple_element

template <std::size_t Index, typename T, std::size_t Count>
    requires(Count != snn::constant::dynamic_count && Index < Count)
struct std::tuple_element<Index, snn::array_view<T, Count>>
{
    using type = T;
};
