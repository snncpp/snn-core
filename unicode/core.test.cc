// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/unicode/core.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(unicode::codepoint::max == 0x10FFFF);
            snn_require(unicode::codepoint::replacement == 0xFFFD);
            snn_require(unicode::codepoint::leading_surrogate_min == 0xD800);
            snn_require(unicode::codepoint::leading_surrogate_max == 0xDBFF);
            snn_require(unicode::codepoint::trailing_surrogate_min == 0xDC00);
            snn_require(unicode::codepoint::trailing_surrogate_max == 0xDFFF);

            snn_require(unicode::is_ascii(0x20));
            snn_require(!unicode::is_ascii(0xE5));
            snn_require(!unicode::is_ascii(0x1F349));

            snn_require(unicode::is_plane_0(0x20));
            snn_require(unicode::is_plane_0(0xE5));
            snn_require(!unicode::is_plane_0(0x1F349));

            snn_require(unicode::is_noncharacter(0xFFFF));
            snn_require(!unicode::is_noncharacter(0x20));
            snn_require(!unicode::is_noncharacter(0xE5));
            snn_require(!unicode::is_noncharacter(0x1F349));

            snn_require(unicode::is_leading_surrogate(0xD83C));
            snn_require(!unicode::is_leading_surrogate(0xDF49));

            snn_require(unicode::is_trailing_surrogate(0xDF49));
            snn_require(!unicode::is_trailing_surrogate(0xD83C));

            snn_require(unicode::decode_surrogate_pair(0xD83C, 0xDF49) == 0x1F349);

            snn_require(unicode::encode_leading_surrogate(0x1F349) == 0xD83C);
            snn_require(unicode::encode_trailing_surrogate(0x1F349) == 0xDF49);

            return true;
        }

        bool test_all()
        {
            usize count = 0;

            constexpr u32 codepoint_max_plus_one = unicode::codepoint::max + 1;
            for (const auto cp : range::step<u32>{0, codepoint_max_plus_one + 1})
            {
                if (cp < 128)
                {
                    snn_require(unicode::is_ascii(cp));
                }
                else
                {
                    snn_require(!unicode::is_ascii(cp));
                }

                if (cp >= 0xFDD0 && cp <= 0xFDEF)
                {
                    snn_require(unicode::is_noncharacter(cp));
                }
                else if (cp == 0xFFFE || cp == 0xFFFF || cp == 0x1FFFE || cp == 0x1FFFF ||
                         cp == 0x2FFFE || cp == 0x2FFFF || cp == 0x3FFFE || cp == 0x3FFFF ||
                         cp == 0x4FFFE || cp == 0x4FFFF || cp == 0x5FFFE || cp == 0x5FFFF ||
                         cp == 0x6FFFE || cp == 0x6FFFF || cp == 0x7FFFE || cp == 0x7FFFF ||
                         cp == 0x8FFFE || cp == 0x8FFFF || cp == 0x9FFFE || cp == 0x9FFFF ||
                         cp == 0xAFFFE || cp == 0xAFFFF || cp == 0xBFFFE || cp == 0xBFFFF ||
                         cp == 0xCFFFE || cp == 0xCFFFF || cp == 0xDFFFE || cp == 0xDFFFF ||
                         cp == 0xEFFFE || cp == 0xEFFFF || cp == 0xFFFFE || cp == 0xFFFFF ||
                         cp == 0x10FFFE || cp == 0x10FFFF)
                {
                    snn_require(unicode::is_noncharacter(cp));
                }
                else
                {
                    snn_require(!unicode::is_noncharacter(cp));
                }

                if (cp < 0x10000)
                {
                    snn_require(unicode::is_plane_0(cp));
                }
                else
                {
                    snn_require(!unicode::is_plane_0(cp));
                }

                if (cp >= 0xD800 && cp <= 0xDFFF)
                {
                    if (cp <= 0xDBFF)
                    {
                        snn_require(unicode::is_leading_surrogate(cp));
                        snn_require(!unicode::is_trailing_surrogate(cp));
                    }
                    else
                    {
                        snn_require(!unicode::is_leading_surrogate(cp));
                        snn_require(unicode::is_trailing_surrogate(cp));
                    }
                }
                else
                {
                    snn_require(!unicode::is_leading_surrogate(cp));
                    snn_require(!unicode::is_trailing_surrogate(cp));
                }

                ++count;
            }

            snn_require(count == 1'114'113);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_require(app::test_all());
    }
}
