// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/country/code.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            country::code cc{"SE"};
            snn_require(cc == "SE");
            snn_require(cc.view() == "SE");
            snn_require(algo::is_equal(cc.range(), {'S', 'E'}));

            return true;
        }

        constexpr bool test_code()
        {
            static_assert(sizeof(country::code) == 2);
            static_assert(is_trivially_relocatable_v<country::code>);

            {
                country::code cc = "SE";
                snn_require(cc == "SE");
            }
            {
                cstrview se{"SE"};
                country::code cc{se};
                snn_require(cc == "SE");
            }
            {
                country::code cc{"UK"};
                snn_require(cc == "UK");
                cc = "US";
                snn_require(cc != "UK");
                snn_require(cc == "US");
            }
            {
                country::code ae{"AE"};
                country::code be{"BE"};

                snn_require(ae == ae);
                snn_require(!(ae != ae));
                snn_require(ae != be);
                snn_require(!(ae == be));
                snn_require(ae < be);
                snn_require(!(ae < ae));
                snn_require(!(be < ae));
                snn_require(be > ae);
                snn_require(!(be > be));
                snn_require(!(ae > be));

                static_assert(std::is_same_v<decltype(ae <=> be), std::strong_ordering>);
            }
            {
                country::code ad{"AD"};
                country::code ae{"AE"};

                snn_require(ad != "");
                snn_require(ad != "A");
                snn_require(ad != "AE");
                snn_require(ad != "ADE");

                snn_require("AE" == ae);

                snn_require(ad == ad);
                snn_require(!(ad != ad));
                snn_require(ad != ae);
                snn_require(!(ad == ae));
                snn_require(ad < ae);
                snn_require(ad < "AEX");
                snn_require(!(ad < ad));
                snn_require(!(ad < "AD"));
                snn_require(!(ae < ad));
                snn_require(ae > ad);
                snn_require(ae > "ADX");
                snn_require(!(ae > ae));
                snn_require(!(ae > "AE"));
                snn_require(!(ad > ae));

                static_assert(std::is_same_v<decltype(ae <=> cstrview{}), std::strong_ordering>);
            }

            snn_require(country::code::is_valid("SE"));
            snn_require(country::code::is_valid("XX"));
            snn_require(!country::code::is_valid(""));
            snn_require(!country::code::is_valid("s"));
            snn_require(!country::code::is_valid("S"));
            snn_require(!country::code::is_valid("se"));
            snn_require(!country::code::is_valid("Se"));
            snn_require(!country::code::is_valid("sE"));
            snn_require(!country::code::is_valid("abc"));
            snn_require(!country::code::is_valid("ABC"));

            snn_require_throws_code(country::code{cstrview{""}},
                                    country::error::invalid_two_letter_code);
            snn_require_throws_code(country::code{cstrview{"A"}},
                                    country::error::invalid_two_letter_code);
            snn_require_throws_code(country::code{cstrview{"Se"}},
                                    country::error::invalid_two_letter_code);
            snn_require_throws_code(country::code{cstrview{"sE"}},
                                    country::error::invalid_two_letter_code);
            snn_require_throws_code(country::code{cstrview{".."}},
                                    country::error::invalid_two_letter_code);
            snn_require_throws_code(country::code{cstrview{"ABC"}},
                                    country::error::invalid_two_letter_code);
            snn_require_throws_code(country::code{cstrview{"ABCD"}},
                                    country::error::invalid_two_letter_code);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_code());
    }
}
