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
        usize std_replace_inplace(std::string& s, const char needle, const char replacement,
                                  const usize start_pos)
        {
            usize replace_count = 0;
            char* const data    = s.data();
            const usize size    = s.size();
            for (usize i = start_pos; i < size; ++i)
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                if (data[i] == needle)
                {
                    data[i] = replacement;
                    ++replace_count;
                }

                SNN_DIAGNOSTIC_POP
            }
            return replace_count;
        }

        usize std_replace_inplace(std::string& s, const std::string_view needle,
                                  const std::string_view replacement, const usize start_pos)
        {
            usize replace_count = 0;

            usize pos = s.find(needle, start_pos);
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

                    if (needle.empty())
                    {
                        last_pos = pos;
                        ++pos;
                    }
                    else
                    {
                        pos += needle.size();
                        last_pos = pos;
                    }
                    pos = s.find(needle, pos);

                    ++replace_count;
                } while (pos != s.npos);

                if (s.size() > last_pos)
                {
                    tmp.append(s, last_pos, s.size() - last_pos);
                }

                s.swap(tmp);
            }

            return replace_count;
        }

        [[nodiscard]] std::string_view std_sub_view(const std::string_view s, const usize pos,
                                                    const usize size)
        {
            return s.substr(math::min(s.size(), pos), size);
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const char needle, const char replacement,
                          const usize start_pos)
        {
            Str s1{subject};
            const usize s1_count = s1.replace(needle, replacement, start_pos);

            std::string s2{subject.to<std::string_view>()};
            const usize s2_count = app::std_replace_inplace(s2, needle, replacement, start_pos);

            snn_assert(s1_count == s2_count);
            snn_assert(s1.template to<std::string_view>() == std::string_view{s2});
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const cstrview needle, const cstrview replacement,
                          const usize start_pos)

        {
            Str s1{subject};
            const usize s1_count = s1.replace(needle, replacement, start_pos);

            std::string s2{subject.to<std::string_view>()};
            const usize s2_count =
                app::std_replace_inplace(s2, needle.to<std::string_view>(),
                                         replacement.to<std::string_view>(), start_pos);

            snn_assert(s1_count == s2_count);
            snn_assert(s1.template to<std::string_view>() == std::string_view{s2});
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const usize needle_pos, const usize needle_size,
                          const usize replacement_pos, const usize replacement_size,
                          const usize start_pos)
        {
            Str s1{subject};
            const cstrview s1_needle      = s1.view(needle_pos, needle_size);
            const cstrview s1_replacement = s1.view(replacement_pos, replacement_size);
            const usize s1_count          = s1.replace(s1_needle, s1_replacement, start_pos);

            std::string s2{subject.to<std::string_view>()};
            const auto s2_needle      = app::std_sub_view(s2, needle_pos, needle_size);
            const auto s2_replacement = app::std_sub_view(s2, replacement_pos, replacement_size);
            const usize s2_count =
                app::std_replace_inplace(s2, s2_needle, s2_replacement, start_pos);

            snn_assert(s1_count == s2_count);
            snn_assert(s1.template to<std::string_view>() == std::string_view{s2});
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input.size() >= 10)
        {
            const auto prefix = input.view_exactly<0, 10>(assume::within_bounds);
            input.drop_front_n(10);

            constexpr usize mask = 0b0001'1111'1111;
            static_assert(mask == 511); // 0-511

            const usize needle_pos       = prefix.view_exactly<0, 2>().load<u16>() & mask;
            const usize needle_size      = prefix.view_exactly<2, 2>().load<u16>() & mask;
            const usize replacement_pos  = prefix.view_exactly<4, 2>().load<u16>() & mask;
            const usize replacement_size = prefix.view_exactly<6, 2>().load<u16>() & mask;
            usize start_pos              = prefix.view_exactly<8, 2>().load<u16>();

            if (start_pos == constant::limit<u16>::max)
            {
                start_pos = constant::npos;
            }

            const cstrview needle      = input.view(needle_pos, needle_size);
            const cstrview replacement = input.view(replacement_pos, replacement_size);

            app::fuzz_replace<str>(input, needle, replacement, start_pos);
            app::fuzz_replace<str>(input, needle_pos, needle_size, replacement_pos,
                                   replacement_size, start_pos);

            app::fuzz_replace<strbuf>(input, needle, replacement, start_pos);
            app::fuzz_replace<strbuf>(input, needle_pos, needle_size, replacement_pos,
                                      replacement_size, start_pos);

            if (needle && replacement)
            {
                const char n = needle.front(assume::not_empty);
                const char r = replacement.front(assume::not_empty);
                app::fuzz_replace<str>(input, n, r, start_pos);
                app::fuzz_replace<strbuf>(input, n, r, start_pos);
            }
        }

        return constant::exit::success;
    }
}
