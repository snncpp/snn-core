// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pair element with its index

#pragma once

#include "snn-core/pair/common.hh"
#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### enumerate

    template <input_range Rng>
    class enumerate final
    {
      public:
        constexpr explicit enumerate(Rng rng, const usize start_index = 0) noexcept
            : rng_{std::move(rng)},
              index_{start_index}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<enumerate>)
            {
                return iter::forward<enumerate>{*this};
            }
            else
            {
                return iter::forward_reference<enumerate>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_front(assume::not_empty);
            ++index_;
        }

        [[nodiscard]] constexpr auto front(assume::not_empty_t)
        {
            using iv = snn::pair::index_value<usize, decltype(rng_.front(assume::not_empty))>;
            return iv{index_, rng_.front(assume::not_empty)};
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty();
        }

      private:
        Rng rng_;
        usize index_;
    };

    template <random_access_range Rng>
    class enumerate<Rng> final
    {
      public:
        constexpr explicit enumerate(Rng rng, const usize start_index = 0) noexcept
            : rng_{std::move(rng)},
              front_index_{start_index},
              end_index_{start_index + rng_.count()}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin() const
        {
            return iter::forward<enumerate>{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_front(assume::not_empty);
            ++front_index_;
        }

        constexpr void drop_back(assume::not_empty_t)
        {
            rng_.drop_back(assume::not_empty);
            --end_index_;
        }

        [[nodiscard]] constexpr auto front(assume::not_empty_t)
        {
            using iv = snn::pair::index_value<usize, decltype(rng_.front(assume::not_empty))>;
            return iv{front_index_, rng_.front(assume::not_empty)};
        }

        [[nodiscard]] constexpr auto back(assume::not_empty_t)
        {
            using iv = snn::pair::index_value<usize, decltype(rng_.back(assume::not_empty))>;
            return iv{end_index_ - 1, rng_.back(assume::not_empty)};
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty();
        }

      private:
        Rng rng_;
        usize front_index_;
        usize end_index_;
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### enumerate

    class enumerate final
    {
      public:
        constexpr explicit enumerate(const usize start_index = 0) noexcept
            : start_index_{start_index}
        {
        }

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::enumerate{std::move(rng), start_index_};
        }

      private:
        usize start_index_;
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, enumerate f) noexcept
    {
        return f(std::move(rng));
    }
}
