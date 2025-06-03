// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Contiguous interface

// Shared interface class for contiguous containers using [Curiously recurring template
// pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern).

// Notable types that inherit from this class:
// * `[c]strrng` (`range::contiguous<...>`)
// * `[c]strview` (`array_view<..., dynamic_count>`)
// * `str`/`strbuf` (`strcore<...>`)
// * `vec`

// The `Derived` class must have the following member functions:
// * `auto range() noexcept`
// * `auto range() const noexcept`
// * `void truncate(const usize count) noexcept`
// * `auto view() noexcept`
// * `auto view() const noexcept`

// The following classes should have the same interface when possible but doesn't inherit from this
// class:
// `array` is an aggregate so it can't inherit, even if `truncate` is made optional.
// `array_view` with a static count could inherit if `truncate` is made optional, but some
// optimization opportunities might be lost.

#pragma once

#include "snn-core/optional_index.hh"
#include "snn-core/algo/is_sorted.hh"
#include "snn-core/algo/remove_consecutive_duplicates.fwd.hh"
#include "snn-core/algo/remove_if.fwd.hh"
#include "snn-core/algo/reverse.hh"
#include "snn-core/algo/sort.fwd.hh"
#include "snn-core/fn/common.hh"

namespace snn
{
    // ## Classes

    // ### contiguous_interface

    template <typename Derived>
    class contiguous_interface
    {
      public:
        // #### Comparison

        template <typename V>
        [[nodiscard]] constexpr bool has_front(V&& value) const noexcept
        {
            return derived_().view().has_front(std::forward<V>(value));
        }

        template <typename V>
        [[nodiscard]] constexpr bool has_back(V&& value) const noexcept
        {
            return derived_().view().has_back(std::forward<V>(value));
        }

        // #### View exactly

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(promise::within_bounds_t) noexcept
        {
            return derived_().view().template view_exactly<Pos, Count>(promise::within_bounds);
        }

        template <usize Pos, usize Count>
        [[nodiscard]] constexpr auto view_exactly(promise::within_bounds_t) const noexcept
        {
            return derived_().view().template view_exactly<Pos, Count>(promise::within_bounds);
        }

        // #### View offset

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) noexcept
        {
            return derived_().view().view_offset(pos, count);
        }

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) const noexcept
        {
            return derived_().view().view_offset(pos, count);
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr bool contains(V&& value) const noexcept
        {
            return derived_().view().find(std::forward<V>(value)).has_value();
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr usize count_if(OneArgPred p) const
        {
            usize pred_count = 0;
            for (const auto& e : derived_().range())
            {
                if (p(e))
                {
                    ++pred_count;
                }
            }
            return pred_count;
        }

        template <typename V>
        [[nodiscard]] constexpr optional_index find(V&& value,
                                                    const usize start_pos = 0) const noexcept
        {
            return derived_().view().find(std::forward<V>(value), start_pos);
        }

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        template <typename OneArgPred>
        [[nodiscard]] constexpr optional_index find_if(OneArgPred p,
                                                       const usize start_pos = 0) const
        {
            const auto view        = derived_().view();
            const auto* const data = view.begin();
            const usize count      = view.count();
            for (usize i = start_pos; i < count; ++i)
            {
                if (p(std::as_const(data[i])))
                {
                    return i;
                }
            }
            return constant::npos;
        }

        template <typename V>
        [[nodiscard]] constexpr optional_index find_in_reverse(
            V&& value, const usize start_pos = constant::npos) const noexcept
        {
            return derived_().view().find_in_reverse(std::forward<V>(value), start_pos);
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr optional_index find_in_reverse_if(
            OneArgPred p, const usize start_pos = constant::npos) const
        {
            const auto view        = derived_().view();
            const auto* const data = view.begin();
            usize i                = view.count();
            if (start_pos < i)
            {
                i = start_pos + 1;
            }
            while (i > 0)
            {
                --i;
                if (p(std::as_const(data[i])))
                {
                    return i;
                }
            }
            return constant::npos;
        }

        SNN_DIAGNOSTIC_POP

        // #### Algorithm helpers

        template <typename OneArgFn>
        constexpr void each(OneArgFn f)
        {
            for (auto& e : derived_().range())
            {
                f(e);
            }
        }

        template <typename OneArgFn>
        constexpr void each(OneArgFn f) const
        {
            for (const auto& e : derived_().range())
            {
                f(e);
            }
        }

        template <typename TwoArgPred = fn::less_than>
        [[nodiscard]] constexpr bool is_sorted(TwoArgPred is_less = TwoArgPred{}) const
        {
            return algo::is_sorted(derived_().range(), std::move(is_less));
        }

        template <typename V>
        constexpr usize remove(const V& value)
        {
            return remove_if(fn::is{fn::equal_to{}, value});
        }

        template <typename TwoArgPred = fn::equal_to>
        constexpr usize remove_consecutive_duplicates(TwoArgPred is_equal = TwoArgPred{})
        {
            auto rng                = derived_().range();
            const auto count_before = rng.count();
            const auto count_after =
                algo::remove_consecutive_duplicates(rng, std::move(is_equal)).count();
            derived_().truncate(count_after);
            const auto remove_count = count_before - count_after;
            return remove_count;
        }

        template <typename OneArgPred>
        constexpr usize remove_if(OneArgPred p)
        {
            auto rng                = derived_().range();
            const auto count_before = rng.count();
            const auto count_after  = algo::remove_if(rng, std::move(p)).count();
            derived_().truncate(count_after);
            const auto remove_count = count_before - count_after;
            return remove_count;
        }

        template <typename T, typename U>
        constexpr usize replace(const T& needle, const U& replacement)
        {
            usize replace_count = 0;
            for (auto& e : derived_().range())
            {
                if (e == needle)
                {
                    e = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        template <typename OneArgPred, typename V>
        constexpr usize replace_if(OneArgPred p, const V& replacement)
        {
            usize replace_count = 0;
            for (auto& e : derived_().range())
            {
                if (p(std::as_const(e)))
                {
                    e = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        constexpr void reverse()
        {
            algo::reverse(derived_().range());
        }

        template <typename TwoArgPred = fn::less_than>
        constexpr void sort(TwoArgPred is_less = TwoArgPred{})
        {
            algo::sort(derived_().range(), std::move(is_less));
        }

        template <typename OneArgOp>
        constexpr void transform(OneArgOp op)
        {
            for (auto& e : derived_().range())
            {
                e = op(std::as_const(e));
            }
        }

        // #### Validation helpers

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool all(OneArgPred p) const
        {
            for (const auto& e : derived_().range())
            {
                if (!p(e))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool any(OneArgPred p) const
        {
            for (const auto& e : derived_().range())
            {
                if (p(e))
                {
                    return true;
                }
            }
            return false;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool none(OneArgPred p) const
        {
            for (const auto& e : derived_().range())
            {
                if (p(e))
                {
                    return false;
                }
            }
            return true;
        }

      private:
        constexpr Derived& derived_() noexcept
        {
            return *static_cast<Derived*>(this);
        }

        constexpr const Derived& derived_() const noexcept
        {
            return *static_cast<const Derived*>(this);
        }
    };
}
