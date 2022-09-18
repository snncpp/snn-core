// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Limit

#pragma once

#include "snn-core/time/unit.hh"

namespace snn::time
{
    // ## Classes

    // ### limit

    template <any_unit Unit, i64 Min, i64 Max>
        requires(Min >= 0 && Min <= Max)
    class limit final
    {
      public:
        // #### Converting constructors

        constexpr limit(const Unit u) noexcept
            : count_{u.value_or(0)}
        {
            if (count_ < Min)
            {
                count_ = Min;
            }
            else if (count_ > Max)
            {
                count_ = Max;
            }
        }

        template <i64 N, i64 D>
        constexpr limit(const unit<N, D> u) noexcept
            : limit{u.template to<Unit>()}
        {
        }

        template <typename U, i64 Mn, i64 Mx>
        constexpr limit(const limit<U, Mn, Mx> other) noexcept
            : limit{other.unit()}
        {
        }

        // #### Value

        [[nodiscard]] constexpr i64 count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] static constexpr i64 max() noexcept
        {
            return Max;
        }

        [[nodiscard]] static constexpr i64 min() noexcept
        {
            return Min;
        }

        // #### Conversion

        [[nodiscard]] constexpr Unit unit() const noexcept
        {
            return Unit{count_};
        }

      private:
        i64 count_;
    };
}
