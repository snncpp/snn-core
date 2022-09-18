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

        [[nodiscard]] std::string_view std_view(const cstrview s)
        {
            return std::string_view{s.cbegin(), s.size()};
        }

        template <typename Str>
        void fuzz_insert(const cstrview subject, const usize pos, const cstrview insertion)
        {
            Str s1{subject};

            bool exception1 = false;
            try
            {
                s1.insert_at(pos, insertion);
            }
            catch (const exception&)
            {
                exception1 = true;
            }

            std::string s2{app::std_view(subject)};

            bool exception2 = false;
            try
            {
                s2.insert(pos, app::std_view(insertion));
            }
            catch (const std::out_of_range&)
            {
                exception2 = true;
            }

            snn_assert(exception1 == exception2);
            snn_assert(app::std_view(s1) == std::string_view{s2});
        }

        template <typename Str>
        void fuzz_insert(const cstrview subject, const usize pos, const usize insertion_pos,
                         const usize insertion_size)
        {
            Str s1{subject};
            const cstrview insertion1 = s1.view(insertion_pos, insertion_size);

            bool exception1 = false;
            try
            {
                s1.insert_at(pos, insertion1);
            }
            catch (const exception&)
            {
                exception1 = true;
            }

            std::string s2{app::std_view(subject)};

            const std::string_view insertion2 =
                app::std_sub_view(s2, insertion_pos, insertion_size);

            bool exception2 = false;
            try
            {
                s2.insert(pos, insertion2);
            }
            catch (const std::out_of_range&)
            {
                exception2 = true;
            }

            snn_assert(exception1 == exception2);
            snn_assert(app::std_view(s1) == std::string_view{s2});
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input.size() > 5)
        {
            const usize pos            = to_byte(input.at(0).value());
            const usize subject_pos    = to_byte(input.at(1).value());
            const usize subject_size   = to_byte(input.at(2).value());
            const usize insertion_pos  = to_byte(input.at(3).value());
            const usize insertion_size = to_byte(input.at(4).value());

            const cstrview subject   = input.view(subject_pos, subject_size);
            const cstrview insertion = input.view(insertion_pos, insertion_size);

            app::fuzz_insert<str>(subject, pos, insertion);
            app::fuzz_insert<str>(subject, pos, insertion_pos, insertion_size);

            app::fuzz_insert<strbuf>(subject, pos, insertion);
            app::fuzz_insert<strbuf>(subject, pos, insertion_pos, insertion_size);
        }

        return constant::exit::success;
    }
}
