// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pair two ranges

#pragma once

#include "snn-core/math/common.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### pair

    namespace detail
    {
        // For ranges that are not contiguous.
        template <typename FirstRng, typename SecondRng>
        class pair
        {
          public:
            constexpr explicit pair(FirstRng first, SecondRng second)
                : first_{std::move(first)},
                  second_{std::move(second)}
            {
            }

            constexpr explicit operator bool() const
            {
                return !is_empty();
            }

            constexpr void drop_front(assume::not_empty_t)
            {
                first_.drop_front(assume::not_empty);
                second_.drop_front(assume::not_empty);
            }

            [[nodiscard]] constexpr auto front(assume::not_empty_t)
            {
                using pair_type =
                    snn::pair::first_second<decltype(first_.front(assume::not_empty)),
                                            decltype(second_.front(assume::not_empty))>;
                return pair_type{first_.front(assume::not_empty), second_.front(assume::not_empty)};
            }

            [[nodiscard]] constexpr bool is_empty() const
            {
                return first_.is_empty() || second_.is_empty();
            }

          private:
            FirstRng first_;
            SecondRng second_;
        };

        // For ranges that are contiguous.
        template <contiguous_range FirstRng, contiguous_range SecondRng>
        class pair<FirstRng, SecondRng>
        {
          public:
            constexpr explicit pair(FirstRng first, SecondRng second)
                : count_{math::min(first.count(), second.count())},
                  first_{first.begin()},
                  second_{second.begin()}
            {
            }

            constexpr explicit operator bool() const noexcept
            {
                return !is_empty();
            }

            [[nodiscard]] constexpr usize count() const noexcept
            {
                return count_;
            }

            constexpr void drop_front(assume::not_empty_t)
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                snn_assert(count_ > 0);
                --count_;
                ++first_;
                ++second_;

                SNN_DIAGNOSTIC_POP
            }

            [[nodiscard]] constexpr auto front(assume::not_empty_t) const
            {
                snn_assert(count_ > 0);
                using pair_type = snn::pair::first_second<decltype(*first_), decltype(*second_)>;
                return pair_type{*first_, *second_};
            }

            [[nodiscard]] constexpr bool is_empty() const noexcept
            {
                return count_ == 0;
            }

          private:
            using first_iterator  = decltype(std::declval<FirstRng&>().begin());
            using second_iterator = decltype(std::declval<SecondRng&>().begin());

            usize count_;
            first_iterator first_;
            second_iterator second_;
        };
    }

    template <input_range FirstRng, input_range SecondRng>
    class pair final : public detail::pair<FirstRng, SecondRng>
    {
      private:
        using base_type = detail::pair<FirstRng, SecondRng>;

      public:
        constexpr explicit pair(FirstRng first, SecondRng second)
            : base_type{std::move(first), std::move(second)}
        {
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<pair>)
            {
                return iter::forward<pair>{*this};
            }
            else
            {
                return iter::forward_reference<pair>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }
    };
}
