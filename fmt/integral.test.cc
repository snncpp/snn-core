// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/integral.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(fmt::integral(-922337, ' ') == "-922 337");
            snn_require(fmt::integral(-922337, "&nbsp;") == "-922&nbsp;337");

            snn_require(fmt::integral(1234, "...") == "1...234");
            snn_require(fmt::integral(1234, "...", 1) == "1...2...3...4");

            snn_require(fmt::integral<str, math::base::binary>(0x7F, " ", 4) == "111 1111");
            snn_require(fmt::integral<str, math::base::binary>(0x7F, "__", 4, 8) == "0111__1111");

            snn_require(fmt::integral<str, math::base::hex>(57610, "...", 2, 5) == "0...e1...0a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "...", 2, 6,
                                                            hex::table::upper) == "00...E1...0A");

            return true;
        }

        constexpr bool test_integral()
        {
            // Format decimal with thousands separator.

            // Unsigned

            constexpr auto integral_unsigned_max = constant::limit<u64>::max;
            snn_require(fmt::integral(0u, ' ') == "0");
            snn_require(fmt::integral(1u, ' ') == "1");
            snn_require(fmt::integral(18u, ' ') == "18");
            snn_require(fmt::integral(184u, ' ') == "184");
            snn_require(fmt::integral(1844u, ' ') == "1 844");
            snn_require(fmt::integral(18446u, ' ') == "18 446");
            snn_require(fmt::integral(184467u, ' ') == "184 467");
            snn_require(fmt::integral(1844674u, ' ') == "1 844 674");
            snn_require(fmt::integral(18446744u, ' ') == "18 446 744");
            snn_require(fmt::integral(184467440u, ' ') == "184 467 440");
            snn_require(fmt::integral(1844674407u, ' ') == "1 844 674 407");
            snn_require(fmt::integral(18446744073u, ' ') == "18 446 744 073");
            snn_require(fmt::integral(184467440737u, ' ') == "184 467 440 737");
            snn_require(fmt::integral(1844674407370u, ' ') == "1 844 674 407 370");
            snn_require(fmt::integral(18446744073709u, ' ') == "18 446 744 073 709");
            snn_require(fmt::integral(184467440737095u, ' ') == "184 467 440 737 095");
            snn_require(fmt::integral(1844674407370955u, ' ') == "1 844 674 407 370 955");
            snn_require(fmt::integral(18446744073709551u, ' ') == "18 446 744 073 709 551");
            snn_require(fmt::integral(184467440737095516u, ' ') == "184 467 440 737 095 516");
            snn_require(fmt::integral(1844674407370955161u, ' ') == "1 844 674 407 370 955 161");
            snn_require(fmt::integral(integral_unsigned_max, ' ') == "18 446 744 073 709 551 615");

            snn_require(fmt::integral(constant::limit<u8>::max, ' ') == "255");
            snn_require(fmt::integral(constant::limit<u16>::max, ' ') == "65 535");
            snn_require(fmt::integral(constant::limit<u32>::max, ' ') == "4 294 967 295");

            // Signed

            constexpr auto int_egral_signed_min  = constant::limit<i64>::min;
            constexpr auto int_e_gral_signed_max = constant::limit<i64>::max;
            snn_require(fmt::integral(-0, ' ') == "0");
            snn_require(fmt::integral(-9, ' ') == "-9");
            snn_require(fmt::integral(-92, ' ') == "-92");
            snn_require(fmt::integral(-922, ' ') == "-922");
            snn_require(fmt::integral(-9223, ' ') == "-9 223");
            snn_require(fmt::integral(-92233, ' ') == "-92 233");
            snn_require(fmt::integral(-922337, ' ') == "-922 337");
            snn_require(fmt::integral(-9223372, ' ') == "-9 223 372");
            snn_require(fmt::integral(-92233720, ' ') == "-92 233 720");
            snn_require(fmt::integral(-922337203, ' ') == "-922 337 203");
            snn_require(fmt::integral(-9223372036, ' ') == "-9 223 372 036");
            snn_require(fmt::integral(-92233720368, ' ') == "-92 233 720 368");
            snn_require(fmt::integral(-922337203685, ' ') == "-922 337 203 685");
            snn_require(fmt::integral(-9223372036854, ' ') == "-9 223 372 036 854");
            snn_require(fmt::integral(-92233720368547, ' ') == "-92 233 720 368 547");
            snn_require(fmt::integral(-922337203685477, ' ') == "-922 337 203 685 477");
            snn_require(fmt::integral(-9223372036854775, ' ') == "-9 223 372 036 854 775");
            snn_require(fmt::integral(-92233720368547758, ' ') == "-92 233 720 368 547 758");
            snn_require(fmt::integral(-922337203685477580, ' ') == "-922 337 203 685 477 580");
            snn_require(fmt::integral(int_egral_signed_min, ' ') == "-9 223 372 036 854 775 808");
            snn_require(fmt::integral(int_e_gral_signed_max, ' ') == "9 223 372 036 854 775 807");

            snn_require(fmt::integral(constant::limit<i8>::min, ' ') == "-128");
            snn_require(fmt::integral(constant::limit<i16>::min, ' ') == "-32 768");
            snn_require(fmt::integral(constant::limit<i32>::min, ' ') == "-2 147 483 648");
            snn_require(fmt::integral(constant::limit<i8>::max, ' ') == "127");
            snn_require(fmt::integral(constant::limit<i16>::max, ' ') == "32 767");
            snn_require(fmt::integral(constant::limit<i32>::max, ' ') == "2 147 483 647");

            snn_require(fmt::integral<strbuf>(constant::limit<i16>::max, ' ') == "32 767");

#if SNN_INT128_ENABLED
            snn_require(fmt::integral(constant::limit<i128>::min, ' ') ==
                        "-170 141 183 460 469 231 731 687 303 715 884 105 728");
            snn_require(fmt::integral(constant::limit<i128>::max, ' ') ==
                        "170 141 183 460 469 231 731 687 303 715 884 105 727");
            snn_require(fmt::integral(constant::limit<u128>::min, ' ') == "0");
            snn_require(fmt::integral(constant::limit<u128>::max, ' ') ==
                        "340 282 366 920 938 463 463 374 607 431 768 211 455");
#endif

            snn_require(fmt::integral(constant::limit<i32>::min, "") == "-2147483648");
            snn_require(fmt::integral(constant::limit<i32>::min, ".") == "-2.147.483.648");
            snn_require(fmt::integral(constant::limit<i32>::min, "..") == "-2..147..483..648");
            snn_require(fmt::integral(constant::limit<i32>::min, "...") == "-2...147...483...648");

            snn_require(fmt::integral<strbuf>(constant::limit<i32>::min, "..") ==
                        "-2..147..483..648");

            {
                strbuf append_to = "abcdefghijklmnopqrstuvwxyz";
                fmt::integral(constant::limit<i32>::min, "..", append_to, assume::no_overlap);
                snn_require(append_to == "abcdefghijklmnopqrstuvwxyz-2..147..483..648");
            }

            // Extended integral formatting.

            snn_require(fmt::integral(1234, "", 0) == "1234");
            snn_require(fmt::integral(1234, "", 1) == "1234");
            snn_require(fmt::integral(1234, "", 2) == "1234");
            snn_require(fmt::integral(1234, "...", 0) == "1234");
            snn_require(fmt::integral(1234, "...", 1) == "1...2...3...4");
            snn_require(fmt::integral(1234, "...", 2) == "12...34");
            snn_require(fmt::integral(1234, ",", 0) == "1234");
            snn_require(fmt::integral(1234, ",", 1) == "1,2,3,4");
            snn_require(fmt::integral(1234, ",", 1, 4) == "1,2,3,4");
            snn_require(fmt::integral(1234, ",", 1, 5) == "0,1,2,3,4");
            snn_require(fmt::integral(1234, ",", 1, 6) == "0,0,1,2,3,4");

            snn_require(fmt::integral(-1234, "", 2) == "-1234");
            snn_require(fmt::integral(-1234, "...", 2) == "-12...34");
            snn_require(fmt::integral(-1234, ",", 1) == "-1,2,3,4");
            snn_require(fmt::integral(-1234, ",", 1, 4) == "-1,2,3,4");
            snn_require(fmt::integral(-1234, ",", 1, 5) == "-0,1,2,3,4");
            snn_require(fmt::integral(-1234, ",", 1, 6) == "-0,0,1,2,3,4");

            snn_require(fmt::integral<str, math::base::hex>(57610, "", 0) == "e10a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "", 2) == "e10a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "", 2, 6) == "00e10a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "...", 0) == "e10a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "...", 2) == "e1...0a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "...", 2, 5) == "0...e1...0a");
            snn_require(fmt::integral<str, math::base::hex>(57610, "...", 2, 6,
                                                            hex::table::upper) == "00...E1...0A");

            snn_require(fmt::integral<strbuf, math::base::hex>(57610, "...", 2) == "e1...0a");

            {
                str append_to = "\\x";
                fmt::integral<math::base::hex>(57610, "\\x", 2, 0, hex::table::upper, append_to,
                                               assume::no_overlap);
                snn_require(append_to == "\\xE1\\x0A");
            }

            snn_require(fmt::integral<str, math::base::binary>(0x7F, "", 4) == "1111111");
            snn_require(fmt::integral<str, math::base::binary>(0x80, "", 4) == "10000000");
            snn_require(fmt::integral<str, math::base::binary>(0x7F, "", 4, 8) == "01111111");
            snn_require(fmt::integral<str, math::base::binary>(0x80, "", 4, 8) == "10000000");

            snn_require(fmt::integral<str, math::base::binary>(0x7F, " ", 4) == "111 1111");
            snn_require(fmt::integral<str, math::base::binary>(0x80, " ", 4) == "1000 0000");
            snn_require(fmt::integral<str, math::base::binary>(0x7F, " ", 4, 8) == "0111 1111");
            snn_require(fmt::integral<str, math::base::binary>(0x80, " ", 4, 8) == "1000 0000");

            snn_require(fmt::integral<str, math::base::binary>(0x7F, "__", 4) == "111__1111");
            snn_require(fmt::integral<str, math::base::binary>(0x80, "__", 4) == "1000__0000");
            snn_require(fmt::integral<str, math::base::binary>(0x7F, "__", 4, 8) == "0111__1111");
            snn_require(fmt::integral<str, math::base::binary>(0x80, "__", 4, 8) == "1000__0000");

            snn_require(fmt::integral<str, math::base::octal>(1234, "", 2) == "2322");
            snn_require(fmt::integral<str, math::base::octal>(1234, "...", 2) == "23...22");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_integral());
    }
}
