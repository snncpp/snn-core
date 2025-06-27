// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Cache front element

// Caches the `front()` return value of the underlying range until `drop_front()` is called, then
// the next `front()` value is cached and so on.

// This range is single-pass/non-copyable. Why? Copying the cached value could be very inefficient.
// (If a copyable cache view is needed in the future this class can be specialized for small
// trivially-copyable values.)

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### cache

    template <input_range Rng>
    class cache final
    {
      public:
        constexpr explicit cache(Rng rng)
            : rng_{std::move(rng)},
              cached_{front_maybe_()}
        {
        }

        // Non-copyable
        cache(const cache&)            = delete;
        cache& operator=(const cache&) = delete;

        // Movable
        cache(cache&&) noexcept            = default;
        cache& operator=(cache&&) noexcept = default;

        ~cache() = default; // "Rule of five".

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin() noexcept
        {
            return iter::forward_reference<cache>{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_front(assume::not_empty);
            cached_ = front_maybe_();
        }

        [[nodiscard]] constexpr decltype(auto) front(assume::not_empty_t) noexcept
        {
            return cached_.value(assume::has_value);
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            // cached_ has a value when the range isn't empty, but the optimizer doesn't know that.
            return rng_.is_empty() || !cached_.has_value();
        }

      private:
        Rng rng_;
        optional<result_compat_t<decltype(rng_.front(assume::not_empty))>> cached_;

        constexpr decltype(cached_) front_maybe_()
        {
            if (rng_)
            {
                return rng_.front(assume::not_empty);
            }
            return nullopt;
        }
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### cache

    class cache final
    {
      public:
        explicit cache() = default;

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng)
        {
            return range::view::cache{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, cache f)
    {
        return f(std::move(rng));
    }
}
