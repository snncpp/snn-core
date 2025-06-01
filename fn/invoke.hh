// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Invoke with projections

#pragma once

#include "snn-core/fn/common.hh"
#include <functional> // invoke

namespace snn::fn
{
    // ## Classes

    // ### invoke

    template <typename F, typename Projection = fn::transparent, typename ProjectionN = Projection>
    class invoke final
    {
      public:
        constexpr explicit invoke(F f) noexcept
            : f_{std::move(f)}
        {
        }

        constexpr explicit invoke(F f, Projection p)
            : f_{std::move(f)},
              p_{p},
              pn_{p}
        {
        }

        constexpr explicit invoke(F f, Projection p, ProjectionN pn) noexcept
            : f_{std::move(f)},
              p_{std::move(p)},
              pn_{std::move(pn)}
        {
        }

        template <typename Arg, typename... ArgN>
        constexpr decltype(auto) operator()(Arg&& arg, ArgN&&... argn) const
        {
            return std::invoke(f_, std::invoke(p_, std::forward<Arg>(arg)),
                               std::invoke(pn_, std::forward<ArgN>(argn))...);
        }

      private:
        F f_;
        Projection p_{};
        ProjectionN pn_{};
    };
}
