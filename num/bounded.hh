// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Integral within min-max (inclusive) or throw

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/num/error.hh"

namespace snn::num
{
    // ## Classes

    // ### bounded

    template <integral Int, Int Min, Int Max>
        requires(Min <= Max)
    class bounded final
    {
      public:
        // #### Converting constructors

        constexpr bounded(const Int value)
            : value_{value}
        {
            if (value_ < Min || value_ > Max)
            {
                throw_or_abort(error::out_of_range);
            }
        }

        template <typename I, I MinOther, I MaxOther>
            requires same_signedness_as<I, Int>
        constexpr bounded(const bounded<I, MinOther, MaxOther> other)
            : value_{static_cast<Int>(other.get())}
        {
            if constexpr (MinOther < Min)
            {
                if (other.get() < Min)
                {
                    throw_or_abort(error::out_of_range);
                }
            }

            if constexpr (MaxOther > Max)
            {
                if (other.get() > Max)
                {
                    throw_or_abort(error::out_of_range);
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
