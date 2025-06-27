// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Split a string with a delimiter

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/math/common.hh"
#include "snn-core/range/iter/forward.hh"
#include "snn-core/string/size.hh"

namespace snn::string::range
{
    // ## Classes

    // ### split

    template <typename Delimiter>
    class split final
    {
      public:
        constexpr explicit split(const cstrview subject, const cstrview delimiter) noexcept
            : subject_{subject},
              delimiter_{delimiter}
        {
            find_first_();
        }

        constexpr explicit split(const cstrview subject, const char delimiter) noexcept
            : subject_{subject},
              delimiter_{delimiter}
        {
            find_first_();
        }

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return snn::range::iter::forward{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return snn::range::iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t) noexcept
        {
            find_next_();
        }

        [[nodiscard]] constexpr optional<cstrview> front() const noexcept
        {
            if (!is_empty())
            {
                return front(assume::not_empty);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr cstrview front(assume::not_empty_t) const noexcept
        {
            snn_should(!is_empty());
            snn_should(front_pos_ <= delimiter_pos_);
            return subject_.view(front_pos_, delimiter_pos_ - front_pos_);
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return front_pos_ > subject_.size();
        }

        [[nodiscard]] constexpr optional<cstrview> pop_front() noexcept
        {
            if (!is_empty())
            {
                const cstrview s = front(assume::not_empty);
                find_next_();
                return s;
            }
            return nullopt;
        }

        [[nodiscard]] constexpr cstrview pop_front(assume::not_empty_t) noexcept
        {
            const cstrview s = front(assume::not_empty);
            find_next_();
            return s;
        }

      private:
        usize front_pos_     = 0;
        usize delimiter_pos_ = 0;
        cstrview subject_;
        Delimiter delimiter_;

        constexpr void find_first_() noexcept
        {
            if (subject_ && string::size(delimiter_) > 0)
            {
                delimiter_pos_ = subject_.find(delimiter_).value_or_npos();
            }
            else
            {
                front_pos_     = constant::npos;
                delimiter_pos_ = constant::npos;
            }
        }

        constexpr void find_next_() noexcept
        {
            front_pos_ =
                math::max(delimiter_pos_,
                          math::add_with_overflow(delimiter_pos_, string::size(delimiter_)));
            delimiter_pos_ = subject_.find(delimiter_, front_pos_).value_or_npos();
        }
    };

    // ## Deduction guides

    // ### split

    split(cstrview, cstrview) -> split<cstrview>;
    split(cstrview, char) -> split<char>;
}
