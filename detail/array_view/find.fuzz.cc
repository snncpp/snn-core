// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/array_view.hh"

#include "snn-core/fuzz.hh"
#include <string_view>

namespace snn::app
{
    namespace
    {
        [[nodiscard]] usize std_count(const std::string_view subject, const char needle)
        {
            usize count = 0;
            usize pos   = 0;
            while ((pos = subject.find(needle, pos)) != std::string_view::npos)
            {
                ++count;
                ++pos;
            }
            return count;
        }

        [[nodiscard]] usize std_count(const std::string_view subject, const std::string_view needle)
        {
            if (needle.empty())
            {
                return subject.size() + 1;
            }

            usize count = 0;
            usize pos   = 0;
            while ((pos = subject.find(needle, pos)) != std::string_view::npos)
            {
                ++count;
                pos += needle.size();
            }
            return count;
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        // `[needle_size][start_pos][needle][subject]`
        // Where `needle_size` is 2 bytes and `start_pos` is 2 bytes.

        if (input.size() >= 4)
        {
            const auto prefix = input.view_exactly<0, 4>(assume::within_bounds);
            input.drop_front_n(4);

            // Limit needle size to max 511 bytes.
            constexpr usize mask = 0b0001'1111'1111;
            static_assert(mask == 511);

            const usize needle_size = prefix.view_exactly<0, 2>().load<u16>() & mask;
            usize start_pos         = prefix.view_exactly<2, 2>().load<u16>();

            if (start_pos == constant::limit<u16>::max)
            {
                start_pos = constant::npos;
            }

            if (input.size() >= needle_size)
            {
                const cstrview needle  = input.view(0, needle_size);
                const cstrview subject = input.view(needle_size);

                const auto std_needle  = needle.to<std::string_view>();
                const auto std_subject = subject.to<std::string_view>();

                // String needle (0-511 bytes).
                {
                    // Find
                    const usize pos_first = subject.find(needle, start_pos).value_or_npos();
                    snn_assert(pos_first == constant::npos ||
                               (pos_first >= start_pos && pos_first <= subject.size()));
                    snn_assert(pos_first == std_subject.find(std_needle, start_pos));

                    // Find in reverse.
                    const usize pos_last =
                        subject.find_in_reverse(needle, start_pos).value_or_npos();
                    snn_assert(pos_last == constant::npos ||
                               (pos_last <= start_pos && pos_last <= subject.size()));
                    snn_assert(pos_last == std_subject.rfind(std_needle, start_pos));

                    // Count
                    snn_assert(subject.count(needle) == app::std_count(std_subject, std_needle));
                }

                // Byte needle.
                if (needle)
                {
                    const char needle_prefix = needle.front(assume::not_empty);

                    // Find
                    const usize pos_first = subject.find(needle_prefix, start_pos).value_or_npos();
                    snn_assert(pos_first == constant::npos ||
                               (pos_first >= start_pos && pos_first <= subject.size()));
                    snn_assert(pos_first == std_subject.find(needle_prefix, start_pos));

                    // Find in reverse.
                    const usize pos_last =
                        subject.find_in_reverse(needle_prefix, start_pos).value_or_npos();
                    snn_assert(pos_last == constant::npos ||
                               (pos_last <= start_pos && pos_last <= subject.size()));
                    snn_assert(pos_last == std_subject.rfind(needle_prefix, start_pos));

                    // Count
                    snn_assert(subject.count(needle_prefix) ==
                               app::std_count(std_subject, needle_prefix));
                }
            }
        }

        return constant::exit::success;
    }
}
