// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return `get<Index>(...)`

// Call `get<Index>(...)` on the underlying range's `front()` return value.

// Rvalue references will be dropped. E.g. if `get<Index>(...)` returns `int&&` then this range will
// return `int`.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### element

    template <input_range Rng, usize Index>
    class element final
    {
      private:
        Rng rng_;

      public:
        constexpr explicit element(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit element(Rng rng, meta::index_t<Index>) noexcept
            : element{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<element>)
            {
                return iter::forward<element>{*this};
            }
            else
            {
                return iter::forward_reference<element>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(promise::not_empty_t)
        {
            rng_.drop_front(promise::not_empty);
        }

        [[nodiscard]] constexpr auto front(promise::not_empty_t)
            -> remove_cv_rvalue_ref_t<decltype(get<Index>(rng_.front(promise::not_empty)))>
        {
            return get<Index>(rng_.front(promise::not_empty));
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty();
        }
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### element

    template <usize Index>
    class element final
    {
      public:
        constexpr explicit element() noexcept
        {
        }

        constexpr explicit element(meta::index_t<Index>) noexcept
        {
        }

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::element<Rng, Index>{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng, usize Index>
    [[nodiscard]] constexpr auto operator|(Rng rng, element<Index> f) noexcept
    {
        return f(std::move(rng));
    }
}
