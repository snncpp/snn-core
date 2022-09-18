// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/pad.hh"

#include "snn-core/unittest.hh"
#include "snn-core/utf8/count.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::pad("abc", 7, ' ') == "  abc  ");
            snn_require(ascii::pad_left("abc", 7, ' ') == "    abc");
            snn_require(ascii::pad_right("abc", 7, ' ') == "abc    ");

            snn_require(ascii::pad("abc", 7, "-~") == "-~abc-~");
            snn_require(ascii::pad_left("abc", 7, "-~") == "-~-~abc");
            snn_require(ascii::pad_right("abc", 7, "-~") == "abc-~-~");

            str s;
            ascii::pad_inplace(s, 4, 'a');
            snn_require(s.size() == 4);
            snn_require(s == "aaaa");
            ascii::pad_inplace(s, 7, 'b');
            snn_require(s.size() == 7);
            snn_require(s == "baaaabb");

            return true;
        }

        constexpr bool test_pad()
        {
            // [pad_char]:
            {
                snn_require(ascii::pad("", 0, ' ') == "");
                snn_require(ascii::pad("", 3, ' ') == "   ");

                snn_require(ascii::pad("abc", 0, ' ') == "abc");
                snn_require(ascii::pad("abc", 3, ' ') == "abc");
                snn_require(ascii::pad("abc", 4, ' ') == "abc ");
                snn_require(ascii::pad("abc", 5, ' ') == " abc ");
                snn_require(ascii::pad("abc", 6, ' ') == " abc  ");
                snn_require(ascii::pad("abc", 7, ' ') == "  abc  ");

                // This function pads to byte size.
                snn_require(ascii::pad("åäö", 0, ' ') == "åäö");   // 6 bytes.
                snn_require(ascii::pad("åäö", 3, ' ') == "åäö");   // 6 bytes.
                snn_require(ascii::pad("åäö", 6, ' ') == "åäö");   // 6 bytes.
                snn_require(ascii::pad("åäö", 7, ' ') == "åäö ");  // 7 bytes.
                snn_require(ascii::pad("åäö", 8, ' ') == " åäö "); // 8 bytes.
            }

            // [pad_string]:
            {
                snn_require(ascii::pad("abc", 0, "-~") == "abc");
                snn_require(ascii::pad("abc", 3, "-~") == "abc");
                snn_require(ascii::pad("abc", 4, "-~") == "abc-");
                snn_require(ascii::pad("abc", 5, "-~") == "-abc-");
                snn_require(ascii::pad("abc", 6, "-~") == "-abc-~");
                snn_require(ascii::pad("abc", 7, "-~") == "-~abc-~");
                snn_require(ascii::pad("abc", 8, "-~") == "-~abc-~-");
                snn_require(ascii::pad("abc", 9, "-~") == "-~-abc-~-");

                snn_require(ascii::pad("abc", 0, " ") == "abc");
                snn_require(ascii::pad("abc", 3, " ") == "abc");
                snn_require(ascii::pad("abc", 4, " ") == "abc ");
                snn_require(ascii::pad("abc", 5, " ") == " abc ");
                snn_require(ascii::pad("abc", 6, " ") == " abc  ");
                snn_require(ascii::pad("abc", 7, " ") == "  abc  ");
                snn_require(ascii::pad("abc", 8, " ") == "  abc   ");
                snn_require(ascii::pad("abc", 9, " ") == "   abc   ");

                // Pad string must not be empty.
                snn_require_throws_code(ascii::pad("abc", 7, ""),
                                        ascii::error::pad_string_can_not_be_empty);

                // This function pads to byte size.
                snn_require(ascii::pad("åäö", 6, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad("åäö", 7, "-~") == "åäö-"); // 7 bytes.
            }

            // [pad_inplace_char]:
            {
                str s;
                ascii::pad_inplace(s, 0, 'a');
                snn_require(s.is_empty());

                ascii::pad_inplace(s, 4, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_inplace(s, 4, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_inplace(s, 2, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_inplace(s, 7, 'b');
                snn_require(s.size() == 7);
                snn_require(s == "baaaabb");
            }
            {
                // This function pads to byte size.
                str s{"UTF-8 with åäö."};
                snn_require(s.size() == 18);
                snn_require(utf8::count(s).value() == 15);

                ascii::pad_inplace(s, 25, '-');
                snn_require(s.size() == 25);
                snn_require(s == "---UTF-8 with åäö.----");
                snn_require(utf8::count(s).value() == 22);
            }

            // [pad_inplace_string]:
            {
                str s;
                ascii::pad_inplace(s, 0, "x", promise::no_overlap);
                snn_require(s.is_empty());

                ascii::pad_inplace(s, 3, "y", promise::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                ascii::pad_inplace(s, 2, "zz", promise::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                ascii::pad_inplace(s, 6, "abc", promise::no_overlap);
                snn_require(s.size() == 6);
                snn_require(s == "ayyyab");

                // Pad string must not be empty.
                snn_require_throws_code(ascii::pad_inplace(s, 6, "", promise::no_overlap),
                                        ascii::error::pad_string_can_not_be_empty);
            }
            {
                // This function pads to byte size.
                str s{"UTF-8 with åäö."};
                snn_require(s.size() == 18);
                snn_require(utf8::count(s).value() == 15);

                ascii::pad_inplace(s, 25, "-~", promise::no_overlap);
                snn_require(s.size() == 25);
                snn_require(s == "-~-UTF-8 with åäö.-~-~");
                snn_require(utf8::count(s).value() == 22);
            }

            // [pad_left_char]:
            {
                snn_require(ascii::pad_left("", 0, ' ') == "");
                snn_require(ascii::pad_left("", 3, ' ') == "   ");

                snn_require(ascii::pad_left("abc", 0, ' ') == "abc");
                snn_require(ascii::pad_left("abc", 3, ' ') == "abc");
                snn_require(ascii::pad_left("abc", 4, ' ') == " abc");
                snn_require(ascii::pad_left("abc", 7, ' ') == "    abc");

                // This function pads to byte size.
                snn_require(ascii::pad_left("åäö", 0, ' ') == "åäö");  // 6 bytes.
                snn_require(ascii::pad_left("åäö", 3, ' ') == "åäö");  // 6 bytes.
                snn_require(ascii::pad_left("åäö", 6, ' ') == "åäö");  // 6 bytes.
                snn_require(ascii::pad_left("åäö", 7, ' ') == " åäö"); // 7 bytes.
            }

            // [pad_left_string]:
            {
                snn_require(ascii::pad_left("", 0, " ") == "");
                snn_require(ascii::pad_left("", 3, " ") == "   ");

                snn_require(ascii::pad_left("abc", 0, "-~") == "abc");
                snn_require(ascii::pad_left("abc", 3, "-~") == "abc");
                snn_require(ascii::pad_left("abc", 4, "-~") == "-abc");
                snn_require(ascii::pad_left("abc", 7, "-~") == "-~-~abc");

                // Pad string must not be empty.
                snn_require_throws_code(ascii::pad_left("abc", 7, ""),
                                        ascii::error::pad_string_can_not_be_empty);

                // This function pads to byte size.
                snn_require(ascii::pad_left("åäö", 0, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad_left("åäö", 3, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad_left("åäö", 6, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad_left("åäö", 7, "-~") == "-åäö"); // 7 bytes.
            }

            // [pad_left_inplace_char]:
            {
                str s;
                ascii::pad_left_inplace(s, 0, 'a');
                snn_require(s.is_empty());

                ascii::pad_left_inplace(s, 4, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_left_inplace(s, 4, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_left_inplace(s, 2, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_left_inplace(s, 6, 'b');
                snn_require(s.size() == 6);
                snn_require(s == "bbaaaa");
            }
            {
                // This function pads to byte size.
                str s{"UTF-8 with åäö."};
                snn_require(s.size() == 18);
                snn_require(utf8::count(s).value() == 15);

                ascii::pad_left_inplace(s, 25, '-');
                snn_require(s.size() == 25);
                snn_require(s == "-------UTF-8 with åäö.");
                snn_require(utf8::count(s).value() == 22);
            }

            // [pad_left_inplace_string]:
            {
                str s;
                ascii::pad_left_inplace(s, 0, "x", promise::no_overlap);
                snn_require(s.is_empty());

                ascii::pad_left_inplace(s, 3, "y", promise::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                ascii::pad_left_inplace(s, 2, "zz", promise::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                ascii::pad_left_inplace(s, 5, "abc", promise::no_overlap);
                snn_require(s.size() == 5);
                snn_require(s == "abyyy");

                // Pad string must not be empty.
                snn_require_throws_code(ascii::pad_left_inplace(s, 5, "", promise::no_overlap),
                                        ascii::error::pad_string_can_not_be_empty);
            }
            {
                // This function pads to byte size.
                str s{"UTF-8 with åäö."};
                snn_require(s.size() == 18);
                snn_require(utf8::count(s).value() == 15);

                ascii::pad_left_inplace(s, 25, "-~", promise::no_overlap);
                snn_require(s.size() == 25);
                snn_require(s == "-~-~-~-UTF-8 with åäö.");
                snn_require(utf8::count(s).value() == 22);
            }

            // [pad_right_char]:
            {
                snn_require(ascii::pad_right("", 0, ' ') == "");
                snn_require(ascii::pad_right("", 3, ' ') == "   ");

                snn_require(ascii::pad_right("abc", 0, ' ') == "abc");
                snn_require(ascii::pad_right("abc", 3, ' ') == "abc");
                snn_require(ascii::pad_right("abc", 4, ' ') == "abc ");
                snn_require(ascii::pad_right("abc", 7, ' ') == "abc    ");

                // This function pads to byte size.
                snn_require(ascii::pad_right("åäö", 0, ' ') == "åäö");  // 6 bytes.
                snn_require(ascii::pad_right("åäö", 3, ' ') == "åäö");  // 6 bytes.
                snn_require(ascii::pad_right("åäö", 6, ' ') == "åäö");  // 6 bytes.
                snn_require(ascii::pad_right("åäö", 7, ' ') == "åäö "); // 7 bytes.
            }

            // [pad_right_string]:
            {
                snn_require(ascii::pad_right("", 0, " ") == "");
                snn_require(ascii::pad_right("", 3, " ") == "   ");

                snn_require(ascii::pad_right("abc", 0, "-~") == "abc");
                snn_require(ascii::pad_right("abc", 3, "-~") == "abc");
                snn_require(ascii::pad_right("abc", 4, "-~") == "abc-");
                snn_require(ascii::pad_right("abc", 7, "-~") == "abc-~-~");

                // Pad string must not be empty.
                snn_require_throws_code(ascii::pad_right("abc", 7, ""),
                                        ascii::error::pad_string_can_not_be_empty);

                // This function pads to byte size.
                snn_require(ascii::pad_right("åäö", 0, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad_right("åäö", 3, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad_right("åäö", 6, "-~") == "åäö");  // 6 bytes.
                snn_require(ascii::pad_right("åäö", 7, "-~") == "åäö-"); // 7 bytes.
            }

            // [pad_right_inplace_char]:
            {
                str s;
                ascii::pad_right_inplace(s, 0, 'a');
                snn_require(s.is_empty());

                ascii::pad_right_inplace(s, 4, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_right_inplace(s, 4, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_right_inplace(s, 2, 'a');
                snn_require(s.size() == 4);
                snn_require(s == "aaaa");

                ascii::pad_right_inplace(s, 6, 'b');
                snn_require(s.size() == 6);
                snn_require(s == "aaaabb");
            }
            {
                // This function pads to byte size.
                str s{"UTF-8 with åäö."};
                snn_require(s.size() == 18);
                snn_require(utf8::count(s).value() == 15);

                ascii::pad_right_inplace(s, 25, '-');
                snn_require(s.size() == 25);
                snn_require(s == "UTF-8 with åäö.-------");
                snn_require(utf8::count(s).value() == 22);
            }

            // [pad_right_inplace_string]:
            {
                str s;
                ascii::pad_right_inplace(s, 0, "x", promise::no_overlap);
                snn_require(s.is_empty());

                ascii::pad_right_inplace(s, 3, "y", promise::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                ascii::pad_right_inplace(s, 2, "zz", promise::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                ascii::pad_right_inplace(s, 5, "abc", promise::no_overlap);
                snn_require(s.size() == 5);
                snn_require(s == "yyyab");

                // Pad string must not be empty.
                snn_require_throws_code(ascii::pad_right_inplace(s, 5, "", promise::no_overlap),
                                        ascii::error::pad_string_can_not_be_empty);
            }
            {
                // This function pads to byte size.
                str s{"UTF-8 with åäö."};
                snn_require(s.size() == 18);
                snn_require(utf8::count(s).value() == 15);

                ascii::pad_right_inplace(s, 25, "-~", promise::no_overlap);
                snn_require(s.size() == 25);
                snn_require(s == "UTF-8 with åäö.-~-~-~-");
                snn_require(utf8::count(s).value() == 22);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_pad());
    }
}
