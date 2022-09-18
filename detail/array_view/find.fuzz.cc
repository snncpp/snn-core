// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/array_view.hh"

#include "snn-core/fuzz.hh"
#include <string_view>

namespace snn::app
{
    namespace
    {
        [[nodiscard]] std::string_view std_view(const cstrview s)
        {
            return std::string_view{s.cbegin(), s.size()};
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        // [needle_size][start_pos][needle][subject]
        // Where needle_size is 2 bytes and start_pos is 2 bytes.

        if (input.size() >= 4)
        {
            const auto prefix = input.view_exactly<0, 4>(promise::within_bounds);
            input.drop_front_n(4);

            constexpr usize mask = 0b0001'1111'1111;
            static_assert(mask == 511); // 0-511

            const usize needle_size = prefix.view_exactly<0, 2>().load<u16>() & mask;
            const usize start_pos   = prefix.view_exactly<2, 2>().load<u16>() & mask;

            if (input.size() >= needle_size)
            {
                const cstrview needle  = input.view(0, needle_size);
                const cstrview subject = input.view(needle_size);

                const usize pos_first = subject.find(needle, start_pos).value_or_npos();
                snn_assert(pos_first == constant::npos ||
                           (pos_first >= start_pos && pos_first <= subject.size()));

                const usize pos_last = subject.find_in_reverse(needle, start_pos).value_or_npos();
                snn_assert(pos_last == constant::npos ||
                           (pos_last <= start_pos && pos_last <= subject.size()));

                // Validate values with std::string_view.

                const std::string_view needle_sv  = app::std_view(needle);
                const std::string_view subject_sv = app::std_view(subject);

                const usize pos_first_sv = subject_sv.find(needle_sv, start_pos);
                snn_assert(pos_first == pos_first_sv);

                const usize pos_last_sv = subject_sv.rfind(needle_sv, start_pos);
                snn_assert(pos_last == pos_last_sv);
            }
        }

        return constant::exit::success;
    }
}
