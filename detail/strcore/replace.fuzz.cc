// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/strcore.hh"

#include "snn-core/fuzz.hh"
#include <string>
#include <string_view>

namespace snn::app
{
    namespace
    {
        usize std_replace_inplace(std::string& s, const std::string_view needle,
                                  const std::string_view replacement)
        {
            usize replace_count = 0;
            if (needle.size() > 0)
            {
                usize pos = s.find(needle);
                if (pos != s.npos)
                {
                    std::string tmp;

                    tmp.reserve(s.size());

                    usize last_pos = 0;

                    do
                    {
                        if (pos > last_pos)
                        {
                            tmp.append(s, last_pos, pos - last_pos);
                        }

                        tmp.append(replacement);

                        pos += needle.size();
                        last_pos = pos;
                        pos      = s.find(needle, pos);

                        ++replace_count;
                    } while (pos != s.npos);

                    if (s.size() > last_pos)
                    {
                        tmp.append(s, last_pos, s.size() - last_pos);
                    }

                    s.swap(tmp);
                }
            }
            return replace_count;
        }

        [[nodiscard]] std::string_view std_sub_view(const std::string_view s, const usize pos,
                                                    const usize size)
        {
            return s.substr(math::min(s.size(), pos), size);
        }

        [[nodiscard]] std::string_view std_view(const cstrview s)
        {
            return std::string_view{s.cbegin(), s.size()};
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const cstrview needle, const cstrview replacement)

        {
            Str s1{subject};
            const usize s1_count = s1.replace(needle, replacement);

            std::string s2{app::std_view(subject)};
            const usize s2_count =
                app::std_replace_inplace(s2, app::std_view(needle), app::std_view(replacement));

            snn_assert(s1_count == s2_count);
            snn_assert(app::std_view(s1) == std::string_view{s2});
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const usize needle_pos, const usize needle_size,
                          const usize replacement_pos, const usize replacement_size)
        {
            Str s1{subject};
            const cstrview s1_needle      = s1.view(needle_pos, needle_size);
            const cstrview s1_replacement = s1.view(replacement_pos, replacement_size);
            const usize s1_count          = s1.replace(s1_needle, s1_replacement);

            std::string s2{app::std_view(subject)};
            const auto s2_needle      = app::std_sub_view(s2, needle_pos, needle_size);
            const auto s2_replacement = app::std_sub_view(s2, replacement_pos, replacement_size);
            const usize s2_count      = app::std_replace_inplace(s2, s2_needle, s2_replacement);

            snn_assert(s1_count == s2_count);
            snn_assert(app::std_view(s1) == std::string_view{s2});
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input.size() >= 8)
        {
            const auto prefix = input.view_exactly<0, 8>(promise::within_bounds);
            input.drop_front_n(8);

            constexpr usize mask = 0b0001'1111'1111;
            static_assert(mask == 511); // 0-511

            const usize needle_pos       = prefix.view_exactly<0, 2>().load<u16>() & mask;
            const usize needle_size      = prefix.view_exactly<2, 2>().load<u16>() & mask;
            const usize replacement_pos  = prefix.view_exactly<4, 2>().load<u16>() & mask;
            const usize replacement_size = prefix.view_exactly<6, 2>().load<u16>() & mask;

            const cstrview needle      = input.view(needle_pos, needle_size);
            const cstrview replacement = input.view(replacement_pos, replacement_size);

            app::fuzz_replace<str>(input, needle, replacement);
            app::fuzz_replace<str>(input, needle_pos, needle_size, replacement_pos,
                                   replacement_size);

            app::fuzz_replace<strbuf>(input, needle, replacement);
            app::fuzz_replace<strbuf>(input, needle_pos, needle_size, replacement_pos,
                                      replacement_size);
        }

        return constant::exit::success;
    }
}
