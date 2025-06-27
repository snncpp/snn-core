// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Wrap long lines

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/iter/forward.hh"
#include "snn-core/range/view/enumerate.hh"
#include "snn-core/utf8/range/codepoint.hh"

namespace snn::string::range
{
    // ## Classes

    // ### wrap

    class wrap final
    {
      public:
        constexpr explicit wrap(const cstrview subject, const usize line_length,
                                const cstrview delimiter) noexcept
            : subject_{subject},
              delimiter_{delimiter},
              line_length_{line_length}
        {
            next_();
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
            snn_should(!is_empty());
            subject_.drop_front_n(current_size_ + 1);
            next_();
        }

        [[nodiscard]] constexpr pair::first_second<cstrview> front(
            assume::not_empty_t) const noexcept
        {
            snn_should(!is_empty());
            return {subject_.view(0, current_size_), current_delim_};
        }

        [[nodiscard]] constexpr optional<pair::first_second<cstrview>> front() const noexcept
        {
            if (!is_empty())
            {
                return front(assume::not_empty);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return subject_.is_empty();
        }

      private:
        cstrview subject_;
        cstrview delimiter_;
        cstrview current_delim_{};
        usize current_size_{};
        usize line_length_;

        constexpr void next_() noexcept
        {
            usize byte_offset            = 0;
            usize last_index             = 0;
            usize last_space_byte_offset = 0;

            for (const auto [index, cp] :
                 utf8::range::codepoint{subject_} | snn::range::v::enumerate{})
            {
                if (cp.value == '\n')
                {
                    if (index > line_length_ && last_space_byte_offset > 0)
                    {
                        current_delim_ = delimiter_;
                        current_size_  = last_space_byte_offset;
                    }
                    else
                    {
                        current_delim_ = cstrview{"\n"};
                        current_size_  = byte_offset;
                    }

                    return;
                }

                if (cp.value == ' ')
                {
                    if (index >= line_length_)
                    {
                        usize space_byte_offset = byte_offset;
                        if (index > line_length_ && last_space_byte_offset > 0)
                        {
                            space_byte_offset = last_space_byte_offset;
                        }

                        current_delim_ = delimiter_;
                        current_size_  = space_byte_offset;

                        return;
                    }

                    last_space_byte_offset = byte_offset;
                }

                byte_offset += cp.count;
                last_index = index;
            }

            if (last_index >= line_length_ && last_space_byte_offset > 0)
            {
                current_delim_ = delimiter_;
                current_size_  = last_space_byte_offset;
            }
            else
            {
                current_delim_ = cstrview{};
                current_size_  = subject_.size();
            }
        }
    };
}
