// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Integral within min-max (inclusive) or clamp

#pragma once

#include "snn-core/core.hh"

namespace snn::num
{
    // ## Classes

    // ### clamped

    template <integral Int, Int Min, Int Max>
        requires(Min <= Max)
    class clamped final
    {
      public:
        // #### Converting constructors

        constexpr clamped(const Int value) noexcept
            : value_{value}
        {
            // This is typically branchless (using conditional moves).

            if (value < Min)
            {
                value_ = Min;
            }

            if (value > Max)
            {
                value_ = Max;
            }
        }

        template <typename I, I MinOther, I MaxOther>
            requires same_signedness_as<I, Int>
        constexpr clamped(const clamped<I, MinOther, MaxOther> other) noexcept
            : value_{static_cast<Int>(other.get())}
        {
            if constexpr (MinOther < Min)
            {
                if (other.get() < Min)
                {
                    value_ = Min;
                }
            }

            if constexpr (MaxOther > Max)
            {
                if (other.get() > Max)
                {
                    value_ = Max;
                }
            }
        }

        // #### Can be zero

        [[nodiscard]] static constexpr bool can_be_zero() noexcept
        {
            return Min <= 0 && Max >= 0;
        }

        // #### Min / Max

        [[nodiscard]] static constexpr Int min() noexcept
        {
            return Min;
        }

        [[nodiscard]] static constexpr Int max() noexcept
        {
            return Max;
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            if constexpr (can_be_zero())
                return static_cast<bool>(value_);
            else
                return true;
        }

        // #### Get value

        [[nodiscard]] constexpr Int get() const noexcept
        {
            return value_;
        }

        // #### Not-zero wrapped value

        [[nodiscard]] constexpr snn::not_zero<Int> not_zero() const noexcept
            requires(!can_be_zero())
        {
            return snn::not_zero{value_};
        }

      private:
        Int value_;
    };
}
