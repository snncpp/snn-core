// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/stream/encoder.hh"

#include "snn-core/unittest.hh"
#include "snn-core/json/is_valid.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            namespace js = json::stream;

            strbuf buf;

            // Promise up front that nothing added to this object will overlap the buffer.
            js::encoder enc{buf, promise::no_overlap};

            enc << js::object;

            enc << "One" << js::colon << js::as_bool(true) << js::comma;
            enc << "Tw\0" << js::colon << js::as_num(2) << js::comma;

            enc << "Three";
            enc << js::colon;

            enc << js::array;
            enc << "3" << js::comma;
            enc << js::as_num("333") << js::comma;
            enc << js::array_end;

            enc << js::comma;
            enc << js::object_end;

            snn_require(buf == R"({"One":true,"Tw\u0000":2,"Three":["3",333]})");
            snn_require(json::is_valid(buf));

            return true;
        }

        constexpr bool test_encoder()
        {
            {
                namespace js = json::stream;

                strbuf buf;
                js::encoder enc{buf, promise::no_overlap};

                enc << js::array << js::array_end;

                snn_require(buf == "[]");
                snn_require(json::is_valid(buf));
            }

            {
                namespace js = json::stream;

                strbuf buf;
                js::encoder enc{buf, promise::no_overlap};

                enc << js::object << js::object_end;

                snn_require(buf == "{}");
                snn_require(json::is_valid(buf));
            }

            {
                namespace js = json::stream;

                strbuf buf;
                js::encoder enc{buf, promise::no_overlap};

                enc << js::array;
                for (auto i : {1, 2, 7, 28, 231})
                {
                    enc << js::as_num(i) << js::comma;
                }
                enc << js::array_end;

                // The trailing comma is removed by array_end.
                snn_require(buf == "[1,2,7,28,231]");
                snn_require(json::is_valid(buf));
            }

            {
                namespace js = json::stream;

                strbuf buf;
                js::encoder enc{buf, promise::no_overlap};

                enc << js::object;

                enc << "One" << js::colon << '1' << js::comma;
                enc << "Two" << js::colon << js::as_bool(true) << js::comma;
                enc << cstrview{"Three"} << js::colon << js::as_num(-123) << js::comma;
                enc << "Four" << js::colon << js::as_num(3856294) << js::comma;
                enc << "Five" << js::colon;

                enc << js::array;
                enc << "FooBar" << js::comma;
                enc << js::as_bool(cstrview{"true"}) << js::comma;
                enc << js::as_num("-123") << js::comma;
                enc << js::as_null("null") << js::comma;
                enc << js::array_end;

                enc << js::comma;

                enc << js::object_end;

                // The trailing commas are removed by array_end and object_end.
                snn_require(buf == R"({"One":"1","Two":true,"Three":-123,"Four":)"
                                   R"(3856294,"Five":["FooBar",true,-123,null]})");
                snn_require(json::is_valid(buf));
            }

            {
                namespace js = json::stream;

                strbuf buf;
                js::encoder enc{buf, promise::no_overlap};

                enc << js::array;

                enc << js::as_bool(true) << js::comma;
                enc << js::as_bool("true") << js::comma;
                enc << js::as_bool(false) << js::comma;
                enc << js::as_bool("false") << js::comma;

                enc << js::as_null(nullptr) << js::comma;
                enc << js::as_null("null") << js::comma;

                enc << js::as_num(82819) << js::comma;
                enc << js::as_num("-123") << js::comma;
                enc << js::as_num("1.09") << js::comma;

                enc << js::array_end;

                snn_require(buf == "[true,true,false,false,null,null,82819,-123,1.09]");
                snn_require(json::is_valid(buf));
            }

            {
                namespace js = json::stream;

                strbuf buf;
                js::encoder enc{buf, promise::no_overlap};

                enc << js::array;

                enc << "" << js::comma;
                enc << "abc" << js::comma;
                enc << "a\0c" << js::comma;
                enc << "ÅÄÖ\U0001f348 \U0001f349 " << js::comma;

                enc << js::array_end;

                snn_require(buf == R"(["","abc","a\u0000c","ÅÄÖ\ud83c\udf48 \ud83c\udf49 "])");
                snn_require(json::is_valid(buf));
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
        snn_static_require(app::test_encoder());
    }
}
