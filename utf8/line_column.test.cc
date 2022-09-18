// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/line_column.hh"

#include "snn-core/unittest.hh"
#include "snn-core/utf8/count.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            const cstrview s{"Å\0äö\r\n"
                             "abc\n"
                             "\xF0\x9F\x8D\x83" // U+1F343
                             "t"};

            snn_require(s.size() == 18);
            snn_require(utf8::count(s).value() == 12);
            snn_require(utf8::line_column(s, 0) == pair::line_column<usize>{1, 1});  // Å
            snn_require(utf8::line_column(s, 1) == pair::line_column<usize>{1, 1});  // Å
            snn_require(utf8::line_column(s, 2) == pair::line_column<usize>{1, 2});  // \0
            snn_require(utf8::line_column(s, 3) == pair::line_column<usize>{1, 3});  // ä
            snn_require(utf8::line_column(s, 4) == pair::line_column<usize>{1, 3});  // ä
            snn_require(utf8::line_column(s, 5) == pair::line_column<usize>{1, 4});  // ö
            snn_require(utf8::line_column(s, 6) == pair::line_column<usize>{1, 4});  // ö
            snn_require(utf8::line_column(s, 7) == pair::line_column<usize>{2, 1});  // \r
            snn_require(utf8::line_column(s, 8) == pair::line_column<usize>{2, 1});  // \n
            snn_require(utf8::line_column(s, 9) == pair::line_column<usize>{2, 1});  // a
            snn_require(utf8::line_column(s, 10) == pair::line_column<usize>{2, 2}); // b
            snn_require(utf8::line_column(s, 11) == pair::line_column<usize>{2, 3}); // c
            snn_require(utf8::line_column(s, 12) == pair::line_column<usize>{3, 1}); // \n
            snn_require(utf8::line_column(s, 13) == pair::line_column<usize>{3, 1}); // U+1F343
            snn_require(utf8::line_column(s, 14) == pair::line_column<usize>{3, 1}); // U+1F343
            snn_require(utf8::line_column(s, 15) == pair::line_column<usize>{3, 1}); // U+1F343
            snn_require(utf8::line_column(s, 16) == pair::line_column<usize>{3, 1}); // U+1F343
            snn_require(utf8::line_column(s, 17) == pair::line_column<usize>{3, 2}); // t

            // Position is invalid.
            snn_require(utf8::line_column(s, 18) == pair::line_column<usize>{3, 3});
            snn_require(utf8::line_column(s, 19) == pair::line_column<usize>{3, 3});
            snn_require(utf8::line_column(s, 999) == pair::line_column<usize>{3, 3});
            snn_require(utf8::line_column(s, constant::npos) == pair::line_column<usize>{3, 3});

            // String is empty (position is always invalid).
            snn_require(utf8::line_column("", 0) == pair::line_column<usize>{1, 1});
            snn_require(utf8::line_column("", 1) == pair::line_column<usize>{1, 1});
            snn_require(utf8::line_column("", 2) == pair::line_column<usize>{1, 1});
            snn_require(utf8::line_column("", constant::npos) == pair::line_column<usize>{1, 1});

            // Can produce incorrect result for invalid UTF-8.
            snn_require(utf8::line_column("\x80\x80\x80\x80\x80\x80", 0) ==
                        pair::line_column<usize>{1, 4});

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
