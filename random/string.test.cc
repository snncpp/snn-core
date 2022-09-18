// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/random/string.hh"

#include "snn-core/unittest.hh"
#include "snn-core/string/repeat.hh"
#include "snn-core/string/range/chunk.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const str a = random::string(20);
            const str b = random::string(20);
            snn_require(a.size() == 20);
            snn_require(b.size() == 20);
            snn_require(a != b);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        // void string(const usize size, strcore<Buf>& append_to)
        {
            str s{"abcdef"};
            random::string(16, s);

            snn_require(s.size() == 22);

            const auto suffix = s.view(6);
            snn_require(suffix.size() == 16);

            byte combined = 0;
            for (const char c : suffix)
            {
                combined = combined | to_byte(c);
            }
            snn_require(combined > 0);
        }
        {
            auto buf = string::repeat<strbuf>("abcdefgh", 40);
            snn_require(buf.size() == 320);

            for (const auto s : string::range::chunk{buf, 8})
            {
                snn_require(s == "abcdefgh");
            }

            buf.clear();
            random::string(320, buf);
            snn_require(buf.size() == 320);

            for (const auto s : string::range::chunk{buf, 8})
            {
                snn_require(s != "abcdefgh");
            }
        }

        // Str string(const usize size)
        {
            const str s = random::string(16);
            snn_require(s.size() == 16);
            byte combined = 0;
            for (const char c : s)
            {
                combined = combined | to_byte(c);
            }
            snn_require(combined > 0);
        }

        {
            str last{container::fill, 8, 'a'};
            snn_require(last.size() == 8);
            for (loop::count lc{10}; lc--;)
            {
                const str s = random::string(8);
                snn_require(s.size() == last.size());
                snn_require(s != last);
                last = s;
            }
        }
    }
}
