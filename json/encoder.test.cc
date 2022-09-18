// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/encoder.hh"

#include "snn-core/unittest.hh"
#include "snn-core/json/decoder.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            strbuf input{R"({"One" : 1, "Two":{"TwoTwo":22, "ThreeThree":  " 33 " }})"};

            json::decoder dec;
            const json::document doc = dec.decode_inplace(input.range()).value();

            json::encoder enc;
            snn_require(enc.encode(doc) == R"({"One":1,"Two":{"TwoTwo":22,"ThreeThree":" 33 "}})");

            return true;
        }

        [[nodiscard]] constexpr bool decode_encode(const cstrview before)
        {
            json::decoder d;
            strbuf buf{before};
            const auto r = d.decode_inplace(buf.range());
            if (!r)
            {
                return false;
            }
            json::encoder e;
            const auto after = e.encode<strbuf>(r.value());
            return before == after;
        }

        [[nodiscard]] constexpr bool pretty_decode_encode(const cstrview before)
        {
            json::decoder d;
            strbuf buf{before};
            const auto r = d.decode_inplace(buf.range());
            if (!r)
            {
                return false;
            }
            json::encoder e;
            const auto after = e.pretty_encode<strbuf>(r.value());
            return before == after;
        }

        constexpr bool test_encoder()
        {
            snn_require(!decode_encode(""));
            snn_require(!decode_encode("abc"));
            snn_require(!decode_encode(" null"));
            snn_require(!decode_encode("null "));
            snn_require(!decode_encode(R"("\udc19\ud83d")"));

            snn_require(decode_encode("null"));
            snn_require(decode_encode("true"));
            snn_require(decode_encode("false"));
            snn_require(decode_encode("3856294"));
            snn_require(decode_encode("-3856294"));
            snn_require(decode_encode("-38.23"));

            snn_require(decode_encode(R"("åäö")"));
            snn_require(decode_encode(R"("\u2028\u2029")"));
            snn_require(decode_encode(R"("\ud83d\udc19")"));

            snn_require(decode_encode(R"(["FooBar",true,3856294])"));

            snn_require(decode_encode(R"({"One":"FooBar","Two":true)"
                                      R"(,"Three":-123,"Four":3856294})"));

            snn_require(decode_encode(R"({"One":"FooBar","Two":true,"Three":-123,"Four":)"
                                      R"(3856294,"Five":["FooBar",true,-123,3856294]})"));

            snn_require(decode_encode(R"({"One":"1","Two":"22","Three":"333","Four":)"
                                      R"("4444","Five":["A","B","C"]})"));

            snn_require(!pretty_decode_encode(R"(["FooBar",true,3856294])"));

            snn_require(pretty_decode_encode(R"("åäö")"));
            snn_require(pretty_decode_encode(R"("\u2028\u2029")"));
            snn_require(pretty_decode_encode(R"("\ud83d\udc19")"));

            snn_require(pretty_decode_encode("[\n"
                                             "  \"FooBar\",\n"
                                             "  true,\n"
                                             "  3856294\n"
                                             "]"));

            snn_require(pretty_decode_encode("{\n"
                                             "  \"One\": \"FooBar\",\n"
                                             "  \"Two\": true,\n"
                                             "  \"Three\": -123,\n"
                                             "  \"Four\": 3856294,\n"
                                             "  \"Five\": [\n"
                                             "    \"FooBar\",\n"
                                             "    true,\n"
                                             "    -123,\n"
                                             "    3856294\n"
                                             "  ]\n"
                                             "}"));

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
