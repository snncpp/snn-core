// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/range/wrap.hh"

#include "snn-core/make_range.hh"
#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            constexpr cstrview dependencies{
                "file.o: file.cc inc/one.hh inc/two.hh\n"
                "another-file.o: another-file.cc inc/three.hh inc/four.hh inc/five.hh\n"};

            strbuf output;
            for (const auto [part, delim] : string::range::wrap{dependencies, 30, " \\\n  "})
            {
                output << part << delim;
            }
            snn_require(output == "file.o: file.cc inc/one.hh \\\n"
                                  "  inc/two.hh\n"
                                  "another-file.o: \\\n"
                                  "  another-file.cc inc/three.hh \\\n"
                                  "  inc/four.hh inc/five.hh\n");

            return true;
        }

        constexpr bool wraps_to(const cstrview subject, const usize line_length,
                                const cstrview delimiter,
                                const init_list<pair::first_second<cstrview>> expected) noexcept
        {
            auto rng = make_range(expected);
            for (const auto p : string::range::wrap{subject, line_length, delimiter})
            {
                if (rng.pop_front() != p)
                {
                    return false;
                }
            }
            return rng.is_empty();
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        {
            string::range::wrap rng{"one two\n", 6, "-"};

            snn_require(rng);
            snn_require(!rng.is_empty());

            using cstrview_pair = pair::first_second<cstrview>;

            snn_require(rng.front(assume::not_empty) == cstrview_pair{"one", "-"});
            snn_require(rng.front().value() == cstrview_pair{"one", "-"});

            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == cstrview_pair{"two", "\n"});
            snn_require(rng.front().value() == cstrview_pair{"two", "\n"});

            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        static_assert(app::wraps_to("", 0, "LF", {}));
        static_assert(app::wraps_to("", 1, "LF", {}));

        static_assert(app::wraps_to("\n", 0, "LF", {{"", "\n"}}));
        static_assert(app::wraps_to("\n", 1, "LF", {{"", "\n"}}));

        static_assert(app::wraps_to("\n\n", 0, "LF", {{"", "\n"}, {"", "\n"}}));
        static_assert(app::wraps_to("\n\n", 1, "LF", {{"", "\n"}, {"", "\n"}}));

        static_assert(app::wraps_to("a", 0, "LF", {{"a", ""}}));
        static_assert(app::wraps_to("a", 1, "LF", {{"a", ""}}));
        static_assert(app::wraps_to("a", 2, "LF", {{"a", ""}}));
        static_assert(app::wraps_to("a\n", 0, "LF", {{"a", "\n"}}));
        static_assert(app::wraps_to("a\n", 1, "LF", {{"a", "\n"}}));
        static_assert(app::wraps_to("a\n", 2, "LF", {{"a", "\n"}}));

        static_assert(app::wraps_to("ab", 0, "LF", {{"ab", ""}}));
        static_assert(app::wraps_to("ab", 1, "LF", {{"ab", ""}}));
        static_assert(app::wraps_to("ab", 2, "LF", {{"ab", ""}}));
        static_assert(app::wraps_to("ab\n", 0, "LF", {{"ab", "\n"}}));
        static_assert(app::wraps_to("ab\n", 1, "LF", {{"ab", "\n"}}));
        static_assert(app::wraps_to("ab\n", 2, "LF", {{"ab", "\n"}}));

        static_assert(app::wraps_to("a b", 0, "LF", {{"a", "LF"}, {"b", ""}}));
        static_assert(app::wraps_to("a b", 1, "LF", {{"a", "LF"}, {"b", ""}}));
        static_assert(app::wraps_to("a b", 2, "LF", {{"a", "LF"}, {"b", ""}}));
        static_assert(app::wraps_to("a b", 3, "LF", {{"a b", ""}}));
        static_assert(app::wraps_to("a b\n", 0, "LF", {{"a", "LF"}, {"b", "\n"}}));
        static_assert(app::wraps_to("a b\n", 1, "LF", {{"a", "LF"}, {"b", "\n"}}));
        static_assert(app::wraps_to("a b\n", 2, "LF", {{"a", "LF"}, {"b", "\n"}}));
        static_assert(app::wraps_to("a b\n", 3, "LF", {{"a b", "\n"}}));

        static_assert(!app::wraps_to("a b", 0, "LF", {{"a", " "}, {"b", ""}}));
        static_assert(!app::wraps_to("a b", 1, "LF", {{"a", "LF"}}));
        static_assert(!app::wraps_to("a b", 2, "LF", {{"a", "LF"}, {"b", ""}, {"", ""}}));
        static_assert(!app::wraps_to("a b", 3, "LF", {{"a b", "LF"}}));

        static_assert(app::wraps_to("å ä", 0, "LF", {{"å", "LF"}, {"ä", ""}}));
        static_assert(app::wraps_to("å ä", 1, "LF", {{"å", "LF"}, {"ä", ""}}));
        static_assert(app::wraps_to("å ä", 2, "LF", {{"å", "LF"}, {"ä", ""}}));
        static_assert(app::wraps_to("å ä", 3, "LF", {{"å ä", ""}}));
        static_assert(app::wraps_to("å ä\n", 0, "LF", {{"å", "LF"}, {"ä", "\n"}}));
        static_assert(app::wraps_to("å ä\n", 1, "LF", {{"å", "LF"}, {"ä", "\n"}}));
        static_assert(app::wraps_to("å ä\n", 2, "LF", {{"å", "LF"}, {"ä", "\n"}}));
        static_assert(app::wraps_to("å ä\n", 3, "LF", {{"å ä", "\n"}}));

        static_assert(app::wraps_to("åa ÖO", 0, "LF", {{"åa", "LF"}, {"ÖO", ""}}));
        static_assert(app::wraps_to("åa ÖO", 1, "LF", {{"åa", "LF"}, {"ÖO", ""}}));
        static_assert(app::wraps_to("åa ÖO", 2, "LF", {{"åa", "LF"}, {"ÖO", ""}}));
        static_assert(app::wraps_to("åa ÖO", 3, "LF", {{"åa", "LF"}, {"ÖO", ""}}));
        static_assert(app::wraps_to("åa ÖO", 4, "LF", {{"åa", "LF"}, {"ÖO", ""}}));
        static_assert(app::wraps_to("åa ÖO", 5, "LF", {{"åa ÖO", ""}}));

        static_assert(app::wraps_to("åa ÖO\n", 0, "LF", {{"åa", "LF"}, {"ÖO", "\n"}}));
        static_assert(app::wraps_to("åa ÖO\n", 1, "LF", {{"åa", "LF"}, {"ÖO", "\n"}}));
        static_assert(app::wraps_to("åa ÖO\n", 2, "LF", {{"åa", "LF"}, {"ÖO", "\n"}}));
        static_assert(app::wraps_to("åa ÖO\n", 3, "LF", {{"åa", "LF"}, {"ÖO", "\n"}}));
        static_assert(app::wraps_to("åa ÖO\n", 4, "LF", {{"åa", "LF"}, {"ÖO", "\n"}}));
        static_assert(app::wraps_to("åa ÖO\n", 5, "LF", {{"åa ÖO", "\n"}}));

        // Space prefix (doesn't wrap if space is the first character unless line length is zero).
        static_assert(app::wraps_to(" ab", 0, "LF", {{"", "LF"}, {"ab", ""}}));
        static_assert(app::wraps_to(" ab", 1, "LF", {{" ab", ""}}));
        static_assert(app::wraps_to(" ab", 2, "LF", {{" ab", ""}}));

        // Spaces only.
        static_assert(app::wraps_to("   ", 0, "LF", {{"", "LF"}, {"", "LF"}, {"", "LF"}}));
        static_assert(app::wraps_to("   ", 1, "LF", {{" ", "LF"}, {" ", ""}}));
    }
}
