// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Split with N elements per chunk

// The last chunk might have fewer than N elements.

#pragma once

#include "snn-core/math/common.hh"
#include "snn-core/range/iter/forward.hh"
#include "snn-core/range/view/take.hh"

namespace snn::range::view
{
    // ## Classes

    // ### chunk

    namespace detail
    {
        template <typename T>
        concept has_pop_front_n = requires(T& v) { v.pop_front_n(usize{}); };

        // For ranges without `pop_front_n(usize)`.
        template <typename Rng>
        class chunk
        {
          public:
            constexpr explicit chunk(Rng rng, const not_zero<usize> count) noexcept
                : rng_{std::move(rng)},
                  count_{count.get()}
            {
            }

            constexpr explicit operator bool() const
            {
                return !is_empty();
            }

            constexpr void drop_front(promise::not_empty_t)
            {
                snn_should(!is_empty());
                for (usize count = count_; count > 0 && !rng_.is_empty(); --count)
                {
                    rng_.drop_front(promise::not_empty);
                }
            }

            [[nodiscard]] constexpr auto front(promise::not_empty_t) const
            {
                snn_should(!is_empty());
                return range::view::take{rng_, count_};
            }

            [[nodiscard]] constexpr bool is_empty() const
            {
                return rng_.is_empty();
            }

          private:
            Rng rng_;
            usize count_;
        };

        // For ranges with `pop_front_n(usize)`.
        template <has_pop_front_n Rng>
        class chunk<Rng>
        {
          public:
            constexpr explicit chunk(Rng rng, const not_zero<usize> count) noexcept
                : rng_{std::move(rng)},
                  count_{count.get()}
            {
            }

            constexpr explicit operator bool() const
            {
                return !is_empty();
            }

            constexpr void drop_front(promise::not_empty_t)
            {
                snn_should(!is_empty());
                rng_.pop_front_n(count_);
            }

            [[nodiscard]] constexpr decltype(auto) front(promise::not_empty_t) const
            {
                snn_should(!is_empty());
                auto cpy = rng_;
                return cpy.pop_front_n(count_);
            }

            [[nodiscard]] constexpr bool is_empty() const
            {
                return rng_.is_empty();
            }

          private:
            Rng rng_;
            usize count_;
        };
    }

    template <forward_range Rng>
    class chunk final : public detail::chunk<Rng>
    {
      private:
        using base_type = detail::chunk<Rng>;

      public:
        constexpr explicit chunk(Rng rng, const usize count) noexcept
            : base_type{std::move(rng), not_zero{math::max(count, 1)}}
        {
        }

        [[nodiscard]] constexpr auto begin() const
        {
            return iter::forward<chunk>{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### chunk

    class chunk final
    {
      public:
        constexpr explicit chunk(const usize count) noexcept
            : count_{count}
        {
        }

        template <forward_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::chunk{std::move(rng), count_};
        }

      private:
        usize count_;
    };

    // ## Functions

    // ### operator|

    template <forward_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, chunk f) noexcept
    {
        return f(std::move(rng));
    }
}
