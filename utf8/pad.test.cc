// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/pad.hh"

#include "snn-core/unittest.hh"
#include "snn-core/utf8/count.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                constexpr u32 inverted_question_mark = 0xBF;
                constexpr u32 paragraph_sign         = 0xB6;

                str s = "åäö";

                utf8::pad_inplace(s, 3, inverted_question_mark);
                snn_require(utf8::count(s).value() == 3);
                snn_require(s.size() == 6);
                snn_require(s == "åäö"); // Not changed.

                utf8::pad_inplace(s, 4, inverted_question_mark);
                snn_require(utf8::count(s).value() == 4);
                snn_require(s.size() == 8);
                snn_require(s == "åäö¿");

                utf8::pad_inplace(s, 7, paragraph_sign);
                snn_require(utf8::count(s).value() == 7);
                snn_require(s.size() == 14);
                snn_require(s == "¶åäö¿¶¶");
            }
            {
                strbuf s = "ÅÄÖ";
                utf8::pad_inplace(s, 15, "¦»»", assume::no_overlap);
                snn_require(utf8::count(s).value() == 15);
                snn_require(s.size() == 30);
                snn_require(s == "¦»»¦»»ÅÄÖ¦»»¦»»");
            }

            return true;
        }

        constexpr bool test_pad()
        {
            // [pad_inplace_char]:
            {
                str s = "åäö";
                utf8::pad_inplace(s, 3, 'a');
                snn_require(s == "åäö");
                snn_require(utf8::count(s).value() == 3);
                snn_require(s.size() == 6);

                utf8::pad_inplace(s, 4, 'a');
                snn_require(utf8::count(s).value() == 4);
                snn_require(s.size() == 7);
                snn_require(s == "åäöa");

                utf8::pad_inplace(s, 7, 'b');
                snn_require(utf8::count(s).value() == 7);
                snn_require(s.size() == 10);
                snn_require(s == "båäöabb");
            }

            // [pad_inplace_string]:
            {
                str s;
                utf8::pad_inplace(s, 0, "x", assume::no_overlap);
                snn_require(s.is_empty());

                utf8::pad_inplace(s, 3, "y", assume::no_overlap);
                snn_require(utf8::count(s).value() == 3);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                utf8::pad_inplace(s, 3, "åäö", assume::no_overlap);
                snn_require(utf8::count(s).value() == 3);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                utf8::pad_inplace(s, 6, "åäö", assume::no_overlap);
                snn_require(utf8::count(s).value() == 6);
                snn_require(s.size() == 9);
                snn_require(s == "åyyyåä");

                utf8::pad_inplace(s, 15, "AÄÖ", assume::no_overlap);
                snn_require(utf8::count(s).value() == 15);
                snn_require(s.size() == 23);
                snn_require(s == "AÄÖAåyyyåäAÄÖAÄ");

                // Pad string must not be empty.
                snn_require_throws_code(utf8::pad_inplace(s, 6, "", assume::no_overlap),
                                        utf8::error::empty_pad_string);
            }

            // [pad_left_inplace_codepoint]:
            {
                constexpr u32 inverted_question_mark = 0xbf;
                constexpr u32 paragraph_sign         = 0xb6;

                str s;

                utf8::pad_left_inplace(s, 0, inverted_question_mark);
                snn_require(s.is_empty());

                utf8::pad_left_inplace(s, 4, inverted_question_mark);
                snn_require(s == "¿¿¿¿");

                utf8::pad_left_inplace(s, 4, inverted_question_mark);
                snn_require(s == "¿¿¿¿");

                utf8::pad_left_inplace(s, 2, inverted_question_mark);
                snn_require(s == "¿¿¿¿");

                utf8::pad_left_inplace(s, 6, paragraph_sign);
                snn_require(s == "¶¶¿¿¿¿");
            }
            {
                str s{"One åäö Three Four"};

                snn_require(utf8::count(s).value() == 18);
                snn_require(s.size() == 21);
                snn_require(s.capacity() == str::default_capacity());

                utf8::pad_left_inplace(s, 26, u32{0xD6});

                snn_require(utf8::count(s).value() == 26);
                snn_require(s.size() == 37);
                snn_require(s.capacity() > str::default_capacity());
                snn_require(s == "ÖÖÖÖÖÖÖÖOne åäö Three Four");
            }

            // [pad_left_inplace_char]:
            {
                str s;

                utf8::pad_left_inplace(s, 0, 'a');
                snn_require(s.is_empty());

                utf8::pad_left_inplace(s, 4, 'a');
                snn_require(s == "aaaa");

                utf8::pad_left_inplace(s, 4, 'a');
                snn_require(s == "aaaa");

                utf8::pad_left_inplace(s, 2, 'a');
                snn_require(s == "aaaa");

                utf8::pad_left_inplace(s, 6, '\xE4');
                snn_require(s == "ääaaaa");
            }
            {
                str s{"One åäö Three Four"};

                snn_require(utf8::count(s).value() == 18);
                snn_require(s.size() == 21);
                snn_require(s.capacity() == str::default_capacity());

                utf8::pad_left_inplace(s, 26, ' ');

                snn_require(utf8::count(s).value() == 26);
                snn_require(s.size() == 29);
                snn_require(s.capacity() > str::default_capacity());
                snn_require(s == "        One åäö Three Four");
            }

            // [pad_left_inplace_string]:
            {
                str s;
                utf8::pad_left_inplace(s, 0, "yy", assume::no_overlap);
                snn_require(s.is_empty());

                utf8::pad_left_inplace(s, 3, "yy", assume::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                utf8::pad_left_inplace(s, 2, "åäö", assume::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                utf8::pad_left_inplace(s, 5, "åäö", assume::no_overlap);
                snn_require(utf8::count(s).value() == 5);
                snn_require(s.size() == 7);
                snn_require(s == "åäyyy");

                utf8::pad_left_inplace(s, 10, "åäö", assume::no_overlap);
                snn_require(utf8::count(s).value() == 10);
                snn_require(s.size() == 17);
                snn_require(s == "åäöåäåäyyy");

                // Pad string must not be empty.
                snn_require_throws_code(utf8::pad_left_inplace(s, 5, "", assume::no_overlap),
                                        utf8::error::empty_pad_string);
            }

            // [pad_right_inplace_codepoint]:
            {
                constexpr u32 inverted_question_mark = 0xbf;
                constexpr u32 paragraph_sign         = 0xb6;

                str s;

                utf8::pad_right_inplace(s, 0, inverted_question_mark);
                snn_require(s.is_empty());

                utf8::pad_right_inplace(s, 4, inverted_question_mark);
                snn_require(s == "¿¿¿¿");

                utf8::pad_right_inplace(s, 4, inverted_question_mark);
                snn_require(s == "¿¿¿¿");

                utf8::pad_right_inplace(s, 2, inverted_question_mark);
                snn_require(s == "¿¿¿¿");

                utf8::pad_right_inplace(s, 6, paragraph_sign);
                snn_require(s == "¿¿¿¿¶¶");
            }
            {
                str s{"One åäö Three Four"};

                snn_require(utf8::count(s).value() == 18);
                snn_require(s.size() == 21);
                snn_require(s.capacity() == str::default_capacity());

                utf8::pad_right_inplace(s, 26, u32{0xD6});

                snn_require(utf8::count(s).value() == 26);
                snn_require(s.size() == 37);
                snn_require(s.capacity() > str::default_capacity());
                snn_require(s == "One åäö Three FourÖÖÖÖÖÖÖÖ");
            }

            // [pad_right_inplace_char]:
            {
                str s;

                utf8::pad_right_inplace(s, 0, 'a');
                snn_require(s.is_empty());

                utf8::pad_right_inplace(s, 4, 'a');
                snn_require(s == "aaaa");

                utf8::pad_right_inplace(s, 4, 'a');
                snn_require(s == "aaaa");

                utf8::pad_right_inplace(s, 2, 'a');
                snn_require(s == "aaaa");

                utf8::pad_right_inplace(s, 6, '\xE4');
                snn_require(s == "aaaaää");
            }
            {
                str s{"One åäö Three Four"};

                snn_require(utf8::count(s).value() == 18);
                snn_require(s.size() == 21);
                snn_require(s.capacity() == str::default_capacity());

                utf8::pad_right_inplace(s, 26, ' ');

                snn_require(utf8::count(s).value() == 26);
                snn_require(s.size() == 29);
                snn_require(s.capacity() > str::default_capacity());
                snn_require(s == "One åäö Three Four        ");
            }

            // [pad_right_inplace_string]:
            {
                str s;
                utf8::pad_right_inplace(s, 0, "yy", assume::no_overlap);
                snn_require(s.is_empty());

                utf8::pad_right_inplace(s, 3, "yy", assume::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                utf8::pad_right_inplace(s, 2, "åäö", assume::no_overlap);
                snn_require(s.size() == 3);
                snn_require(s == "yyy");

                utf8::pad_right_inplace(s, 5, "åäö", assume::no_overlap);
                snn_require(utf8::count(s).value() == 5);
                snn_require(s.size() == 7);
                snn_require(s == "yyyåä");

                utf8::pad_right_inplace(s, 10, "åäö", assume::no_overlap);
                snn_require(utf8::count(s).value() == 10);
                snn_require(s.size() == 17);
                snn_require(s == "yyyåäåäöåä");

                // Pad string must not be empty.
                snn_require_throws_code(utf8::pad_right_inplace(s, 5, "", assume::no_overlap),
                                        utf8::error::empty_pad_string);
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

        static_assert(utf8::detail::pad_fill_size(0, "a", assume::not_empty) == 0);
        static_assert(utf8::detail::pad_fill_size(1, "a", assume::not_empty) == 1);
        static_assert(utf8::detail::pad_fill_size(2, "a", assume::not_empty) == 2);
        static_assert(utf8::detail::pad_fill_size(3, "a", assume::not_empty) == 3);
        static_assert(utf8::detail::pad_fill_size(4, "a", assume::not_empty) == 4);

        static_assert(utf8::detail::pad_fill_size(0, "abc", assume::not_empty) == 0);
        static_assert(utf8::detail::pad_fill_size(1, "abc", assume::not_empty) == 1);
        static_assert(utf8::detail::pad_fill_size(2, "abc", assume::not_empty) == 2);
        static_assert(utf8::detail::pad_fill_size(3, "abc", assume::not_empty) == 3);
        static_assert(utf8::detail::pad_fill_size(4, "abc", assume::not_empty) == 4);

        static_assert(utf8::detail::pad_fill_size(0, "ä", assume::not_empty) == 0);
        static_assert(utf8::detail::pad_fill_size(1, "ä", assume::not_empty) == 2);
        static_assert(utf8::detail::pad_fill_size(2, "ä", assume::not_empty) == 4);
        static_assert(utf8::detail::pad_fill_size(3, "ä", assume::not_empty) == 6);
        static_assert(utf8::detail::pad_fill_size(4, "ä", assume::not_empty) == 8);

        static_assert(utf8::detail::pad_fill_size(0, "oä", assume::not_empty) == 0);
        static_assert(utf8::detail::pad_fill_size(1, "oä", assume::not_empty) == 1);
        static_assert(utf8::detail::pad_fill_size(2, "oä", assume::not_empty) == 3);
        static_assert(utf8::detail::pad_fill_size(3, "oä", assume::not_empty) == 4);
        static_assert(utf8::detail::pad_fill_size(4, "oä", assume::not_empty) == 6);

        static_assert(utf8::detail::pad_fill_size(0, "äo", assume::not_empty) == 0);
        static_assert(utf8::detail::pad_fill_size(1, "äo", assume::not_empty) == 2);
        static_assert(utf8::detail::pad_fill_size(2, "äo", assume::not_empty) == 3);
        static_assert(utf8::detail::pad_fill_size(3, "äo", assume::not_empty) == 5);
        static_assert(utf8::detail::pad_fill_size(4, "äo", assume::not_empty) == 6);

        static_assert(utf8::detail::pad_fill_size(0, "äö", assume::not_empty) == 0);
        static_assert(utf8::detail::pad_fill_size(1, "äö", assume::not_empty) == 2);
        static_assert(utf8::detail::pad_fill_size(2, "äö", assume::not_empty) == 4);
        static_assert(utf8::detail::pad_fill_size(3, "äö", assume::not_empty) == 6);
        static_assert(utf8::detail::pad_fill_size(4, "äö", assume::not_empty) == 8);
    }
}
