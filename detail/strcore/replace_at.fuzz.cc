// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/strcore.hh"

#include "snn-core/fuzz.hh"
#include <string>

namespace snn::app
{
    namespace
    {
        [[nodiscard]] std::string_view std_sub_view(const std::string_view s, const usize pos,
                                                    const usize size)
        {
            return s.substr(math::min(s.size(), pos), size);
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const usize pos, const usize size,
                          const cstrview replacement)
        {
            Str s1{subject};

            bool exception1 = false;
            try
            {
                s1.replace_at(pos, size, replacement);
            }
            catch (const exception&)
            {
                exception1 = true;
            }

            std::string s2{subject.to<std::string_view>()};

            bool exception2 = false;
            try
            {
                s2.replace(pos, size, replacement.to<std::string_view>());
            }
            catch (const std::out_of_range&)
            {
                exception2 = true;
            }

            snn_assert(exception1 == exception2);
            snn_assert(s1.template to<std::string_view>() == std::string_view{s2});
        }

        template <typename Str>
        void fuzz_replace(const cstrview subject, const usize pos, const usize size,
                          const usize replacement_pos, const usize replacement_size)
        {
            Str s1{subject};
            const cstrview replacement1 = s1.view(replacement_pos, replacement_size);

            bool exception1 = false;
            try
            {
                s1.replace_at(pos, size, replacement1);
            }
            catch (const exception&)
            {
                exception1 = true;
            }

            std::string s2{subject.to<std::string_view>()};

            const std::string_view replacement2 =
                app::std_sub_view(s2, replacement_pos, replacement_size);

            bool exception2 = false;
            try
            {
                s2.replace(pos, size, replacement2);
            }
            catch (const std::out_of_range&)
            {
                exception2 = true;
            }

            snn_assert(exception1 == exception2);
            snn_assert(s1.template to<std::string_view>() == std::string_view{s2});
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input.size() > 6)
        {
            const usize pos              = to_byte(input.at(0).value());
            const usize size             = to_byte(input.at(1).value());
            const usize subject_pos      = to_byte(input.at(2).value());
            const usize subject_size     = to_byte(input.at(3).value());
            const usize replacement_pos  = to_byte(input.at(4).value());
            const usize replacement_size = to_byte(input.at(5).value());

            const cstrview subject     = input.view(subject_pos, subject_size);
            const cstrview replacement = input.view(replacement_pos, replacement_size);

            app::fuzz_replace<str>(subject, pos, size, replacement);
            app::fuzz_replace<str>(subject, pos, size, replacement_pos, replacement_size);

            app::fuzz_replace<strbuf>(subject, pos, size, replacement);
            app::fuzz_replace<strbuf>(subject, pos, size, replacement_pos, replacement_size);
        }

        return constant::exit::success;
    }
}
