// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/decoder.hh"

#include "snn-core/unittest.hh"
#include "snn-core/json/is_valid.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/dir/list.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            strbuf input{R"({"One" : 1, "Two":{"TwoTwo":22, "ThreeThree":  " 33 " }})"};

            json::decoder dec;
            const json::document doc = dec.decode_inplace(input.range()).value();
            const json::node& root   = doc.root();

            snn_require(root.is_object());
            snn_require(root.has_children());

            snn_require(root.get("One").is_unsigned_integral());
            snn_require(root.get("One").to<u64>().value() == 1);

            const json::node& two = root.get("Two");
            snn_require(two.is_object());
            snn_require(two.get("TwoTwo").is_unsigned_integral());
            snn_require(two.get("TwoTwo").to<u64>().value() == 22);
            snn_require(two.get("ThreeThree").is_string());
            snn_require(two.get("ThreeThree").to<cstrview>() == " 33 ");

            // Non-existing
            snn_require(root.get("Three").is_empty());
            snn_require(root.get("Three").get("Four").is_empty());

            return true;
        }

        constexpr bool test_decoder()
        {
            {
                str s{"true"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_boolean());
                snn_require(n.is_true());
                snn_require(!n.is_false());
                snn_require(n.to<str>() == "true");
                snn_require(n.to<cstrview>() == "true");
                snn_require(n.to<cstrview>().begin() == s.begin());
            }

            {
                str s{"\ntrue  "};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_boolean());
                snn_require(n.is_true());
                snn_require(!n.is_false());
                snn_require(n.to<cstrview>() == "true");
            }

            {
                str s{"false"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_boolean());
                snn_require(!n.is_true());
                snn_require(n.is_false());
                snn_require(n.to<cstrview>() == "false");
            }

            {
                str s{"null"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_null());
                snn_require(n.to<cstrview>() == "null");
            }

            {
                str s{R"("One")"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_string());
                snn_require(n.to<cstrview>() == "One");
            }

            {
                str s{R"("One\nTwo")"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_string());
                snn_require(n.to<cstrview>() == "One\nTwo");
            }

            {
                str s{"3892382682"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_number());
                snn_require(n.is_integral());
                snn_require(n.is_unsigned_integral());
                snn_require(!n.is_signed_integral());
                snn_require(n.to<u64>().value() == 3892382682);
            }

            {
                str s{"18446744073709551615"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_unsigned_integral());
                snn_require(n.to<u64>().value() == constant::limit<usize>::max);
            }

            {
                // Not invalid, but will overflow if converted to `u64`.
                str s{"18446744073709551616"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_unsigned_integral());
                snn_require(!n.to<u64>()); // Overflows
                snn_require(n.to<cstrview>() == "18446744073709551616");
            }

            {
                str s{"-123"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_number());
                snn_require(n.is_integral());
                snn_require(n.is_signed_integral());
                snn_require(!n.is_unsigned_integral());
                snn_require(n.to<i64>().value() == -123);
            }

            {
                str s{"-9223372036854775808"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_signed_integral());
                snn_require(n.to<i64>().value() == constant::limit<i64>::min);
            }

            {
                // Not invalid, but will overflow if converted to `i64`.
                str s{"-9223372036854775809"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_signed_integral());
                snn_require(!n.to<i64>()); // Overflows
                snn_require(n.to<cstrview>() == "-9223372036854775809");
            }

            {
                str s{"-123.9382"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_number());
                snn_require(n.is_floating_point());
                snn_require(!n.is_integral());
                snn_require(n.to<cstrview>() == "-123.9382");
            }

            {
                str s{"123.9382E+832"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_floating_point());
                snn_require(n.to<cstrview>() == "123.9382E+832");
            }

            {
                str s{"456e-9"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_floating_point());
                snn_require(n.to<cstrview>() == "456e-9");
            }

            // Unicode escape.
            {
                str s{R"("One \u2603")"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_string());
                snn_require(n.to<cstrview>() == "One \xE2\x98\x83");
            }
            {
                str s{R"("\u003cp\u003e\u0026nbsp;")"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_string());
                snn_require(n.to<cstrview>() == "<p>&nbsp;");
            }

            // Surrogate pair.
            {
                str s{R"("\ud83D\uDc19")"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_string());
                snn_require(n.to<cstrview>() == "\U0001F419");
                snn_require(n.to<cstrview>() == "\xF0\x9F\x90\x99");
            }

            // Surrogate pair.
            {
                str s{R"("1\ud83D\uDc192")"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_string());
                snn_require(n.to<cstrview>() == "1\U0001F4192");
                snn_require(n.to<cstrview>() == "\x31\xF0\x9F\x90\x99\x32");
            }

            // Array
            {
                str s{R"([])"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_array());
                snn_require(!n.has_children());
                snn_require(n.to<cstrview>() == "");

                usize count = 0;
                for (const auto& val : n)
                {
                    ignore_if_unused(val);
                    ++count;
                }
                snn_require(count == 0);
            }

            // Array
            {
                str s{R"(["One"])"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_array());
                snn_require(n.has_children());
                snn_require(n.to<cstrview>() == "");
                snn_require(n.get(0).is_string());
                snn_require(n.get(0).to<cstrview>() == "One");

                usize count = 0;
                for (const auto& val : n)
                {
                    snn_require(val.is_string());
                    snn_require(val.to<cstrview>() == "One");
                    ++count;
                }
                snn_require(count == 1);
            }

            // Array
            {
                str s{R"(["One", "Two"])"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_array());
                snn_require(n.has_children());
                snn_require(n.get(0).is_string());
                snn_require(n.get(0).to<cstrview>() == "One");
                snn_require(n.get(1).is_string());
                snn_require(n.get(1).to<cstrview>() == "Two");

                usize count = 0;
                for (const auto& val : n)
                {
                    snn_require(val.is_string());
                    if (count == 0)
                    {
                        snn_require(val.to<cstrview>() == "One");
                    }
                    else if (count == 1)
                    {
                        snn_require(val.to<cstrview>() == "Two");
                    }
                    ++count;
                }
                snn_require(count == 2);

                auto rng = n.range();
                count    = 0;
                while (rng)
                {
                    const auto& val = rng.front(promise::not_empty);

                    snn_require(val.is_string());
                    if (count == 0)
                    {
                        snn_require(val.to<cstrview>() == "One");
                    }
                    else if (count == 1)
                    {
                        snn_require(val.to<cstrview>() == "Two");
                    }
                    ++count;

                    rng.drop_front(promise::not_empty);
                }
                snn_require(count == 2);
            }

            // Array
            {
                str s{R"(["One", ["Two", "Three"]])"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_array());
                snn_require(n.has_children());
                snn_require(n.get(0).is_string());
                snn_require(n.get(0).to<cstrview>() == "One");
                snn_require(n.get(1).is_array());

                const auto& sub = n.get(1);
                snn_require(sub.get(0).is_string());
                snn_require(sub.get(0).to<cstrview>() == "Two");
                snn_require(sub.get(1).is_string());
                snn_require(sub.get(1).to<cstrview>() == "Three");
            }

            // Object
            {
                str s{R"({})"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_object());
                snn_require(!n.has_children());

                // Non-existing:
                snn_require(n.get("Key").is_empty());
                snn_require(n.get(0).is_empty());

                usize count = 0;
                for (const auto& key : n)
                {
                    ignore_if_unused(key);
                    ++count;
                }
                snn_require(count == 0);
            }

            // Object
            {
                str s{R"({"One" : 1})"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_object());
                snn_require(n.has_children());

                snn_require(n.get("One").is_unsigned_integral());
                snn_require(n.get("One").to<u64>().value() == 1);

                snn_require(n.get(0).is_string());
                snn_require(n.get(0).to<cstrview>() == "One");
                snn_require(n.get(0).get(0).is_unsigned_integral());
                snn_require(n.get(0).get(0).to<u64>().value() == 1);

                // Non-existing:
                snn_require(n.get(1).is_empty());

                usize count = 0;
                for (const auto& key : n)
                {
                    snn_require(key.is_string());
                    snn_require(key.to<cstrview>() == "One");

                    snn_require(key.has_children());

                    snn_require(key.child().is_unsigned_integral());
                    snn_require(key.child().to<u64>().value() == 1);

                    ++count;
                }
                snn_require(count == 1);
            }

            // Object
            {
                str s{R"({"One" : 1, "Two":{"TwoTwo":22, "ThreeThree":  " 33 " }})"};
                json::decoder d;
                const auto r = d.decode_inplace(s.range());
                snn_require(r);
                const auto& n = r.value().root();
                snn_require(n.is_object());
                snn_require(n.has_children());

                snn_require(n.get("One").is_unsigned_integral());
                snn_require(n.get("One").to<u64>().value() == 1);
                snn_require(n.get("Two").is_object());
                snn_require(n.get("Two").get("TwoTwo").is_unsigned_integral());
                snn_require(n.get("Two").get("TwoTwo").to<u64>().value() == 22);
                snn_require(n.get("Two").get("ThreeThree").is_string());
                snn_require(n.get("Two").get("ThreeThree").to<cstrview>() == " 33 ");

                // Non-existing:
                snn_require(n.get("Three").is_empty());
                snn_require(n.get("Three").get("Four").is_empty());
                snn_require(n.get("Two").get("FourFour").is_empty());

                snn_require(n.get(0).is_string());
                snn_require(n.get(0).to<cstrview>() == "One");
                snn_require(n.get(0).get(0).is_unsigned_integral());
                snn_require(n.get(0).get(0).to<u64>().value() == 1);
                snn_require(n.get(1).is_string());
                snn_require(n.get(1).to<cstrview>() == "Two");
                snn_require(n.get(1).child().is_object());

                // Non-existing:
                snn_require(n.get(2).is_empty());

                const auto& o = n.get(1).child();
                snn_require(o.has_children());
                snn_require(o.get(0).is_string());
                snn_require(o.get(0).to<cstrview>() == "TwoTwo");
                snn_require(o.get(0).child().is_unsigned_integral());
                snn_require(o.get(0).child().to<u64>().value() == 22);
                snn_require(o.get(1).is_string());
                snn_require(o.get(1).to<cstrview>() == "ThreeThree");
                snn_require(o.get(1).child().is_string());
                snn_require(o.get(1).child().to<cstrview>() == " 33 ");

                usize count = 0;
                for (const auto& key : n)
                {
                    snn_require(key.is_string());
                    if (count == 0)
                    {
                        snn_require(key.to<cstrview>() == "One");
                    }
                    else if (count == 1)
                    {
                        snn_require(key.to<cstrview>() == "Two");
                    }
                    ++count;
                }
                snn_require(count == 2);
            }

            // Invalid surrogate pair.
            {
                json::decoder d;
                str s{R"("\udc19\ud83d")"}; // Invalid surrogate pair (switched).
                const auto r = d.decode_inplace(s.range());
                snn_require(!r);
                snn_require(r.error_code() ==
                            make_error_code(json::error::invalid_codepoint_escape));
                snn_require(d.byte_position() == 7);
            }

            return true;
        }

        constexpr bool test_is_valid()
        {
            // Valid

            snn_require(json::is_valid(R"("\\")"));
            snn_require(json::is_valid(R"("\t")"));
            snn_require(json::is_valid("\"\U0001F419\""));
            snn_require(json::is_valid("\"\xF0\x9F\x90\x99\"")); // U+1F419
            snn_require(json::is_valid(R"("\ud83d\udc19")"));    // U+1F419
            snn_require(json::is_valid(R"("åäö")"));

            snn_require(json::is_valid("0"));
            snn_require(json::is_valid("3"));
            snn_require(json::is_valid("74"));
            snn_require(json::is_valid("-0"));
            snn_require(json::is_valid("-3"));
            snn_require(json::is_valid("-74"));

            snn_require(json::is_valid("0.0"));
            snn_require(json::is_valid("0.75"));
            snn_require(json::is_valid("3.1"));
            snn_require(json::is_valid("3.92"));
            snn_require(json::is_valid("74.3"));
            snn_require(json::is_valid("74.13"));
            snn_require(json::is_valid("-0.0"));
            snn_require(json::is_valid("-0.75"));
            snn_require(json::is_valid("-3.1"));
            snn_require(json::is_valid("-3.92"));
            snn_require(json::is_valid("-74.3"));
            snn_require(json::is_valid("-74.13"));

            snn_require(json::is_valid("0.0e0"));
            snn_require(json::is_valid("0.75E12"));
            snn_require(json::is_valid("3.1e3"));
            snn_require(json::is_valid("3.92E789"));
            snn_require(json::is_valid("74.3E2"));
            snn_require(json::is_valid("74.13e74"));
            snn_require(json::is_valid("-0.0e0"));
            snn_require(json::is_valid("-0.75E12"));
            snn_require(json::is_valid("-3.1e3"));
            snn_require(json::is_valid("-3.92E789"));
            snn_require(json::is_valid("-74.3E2"));
            snn_require(json::is_valid("-74.13e74"));

            snn_require(json::is_valid("0.0e+0"));
            snn_require(json::is_valid("0.75E-12"));
            snn_require(json::is_valid("3.1e-3"));
            snn_require(json::is_valid("3.92E+789"));
            snn_require(json::is_valid("74.3E+2"));
            snn_require(json::is_valid("74.13e-74"));
            snn_require(json::is_valid("-0.0e+0"));
            snn_require(json::is_valid("-0.75E-12"));
            snn_require(json::is_valid("-3.1e-3"));
            snn_require(json::is_valid("-3.92E+789"));
            snn_require(json::is_valid("-74.3E+2"));
            snn_require(json::is_valid("-74.13e-74"));

            snn_require(json::is_valid("7438362456912056172.132023637912397e-743749264712047"));
            snn_require(json::is_valid("-7438362456912056172.132023637912397e-743749264712047"));

            // Invalid

            snn_require(!json::is_valid(""));
            snn_require(!json::is_valid("fAlse"));
            snn_require(!json::is_valid("aaa"));
            snn_require(!json::is_valid("falsee"));
            snn_require(!json::is_valid("false false"));
            snn_require(!json::is_valid(R"(\)"));
            snn_require(!json::is_valid(R"("\")"));
            snn_require(!json::is_valid(R"("\)"));
            snn_require(!json::is_valid(R"("\ud83d\udc19)"));
            snn_require(!json::is_valid(R"("\ud83d\udc1)"));
            snn_require(!json::is_valid(R"("\ud83d\udc)"));
            snn_require(!json::is_valid(R"("\ud83d\ud)"));
            snn_require(!json::is_valid(R"("\ud83d\u)"));
            snn_require(!json::is_valid(R"("\ud83d\)"));
            snn_require(!json::is_valid(R"("\ud83d)"));
            snn_require(!json::is_valid(R"("\ud83)"));
            snn_require(!json::is_valid(R"("\ud8)"));
            snn_require(!json::is_valid(R"("\ud)"));
            snn_require(!json::is_valid(R"("\u)"));
            snn_require(!json::is_valid(R"("\a")"));
            snn_require(!json::is_valid("\"\U0001F419"));
            snn_require(!json::is_valid("\"\xe5\xe4\xf6\""));  // Latin1 (åäö).
            snn_require(!json::is_valid(R"("\ud83d\ud83d")")); // Invalid surrogate pair.
            snn_require(!json::is_valid(R"("\udc19\ud83d")")); // Invalid surrogate pair (switched).
            snn_require(!json::is_valid("\"\xf0\x82\x82\xac\"")); // Overlong

            snn_require(!json::is_valid("-"));
            snn_require(!json::is_valid("a"));
            snn_require(!json::is_valid("abc"));
            snn_require(!json::is_valid("00"));
            snn_require(!json::is_valid("3a"));
            snn_require(!json::is_valid("x3"));
            snn_require(!json::is_valid("74abc"));
            snn_require(!json::is_valid("-00"));
            snn_require(!json::is_valid("--3"));
            snn_require(!json::is_valid("a-74"));

            snn_require(!json::is_valid("00.0"));
            snn_require(!json::is_valid("0..75"));
            snn_require(!json::is_valid("3.1a"));
            snn_require(!json::is_valid("3.92abc"));
            snn_require(!json::is_valid("a74.3"));
            snn_require(!json::is_valid("abc74.13"));
            snn_require(!json::is_valid("--0.0"));
            snn_require(!json::is_valid("-0.75a"));
            snn_require(!json::is_valid("-3.1abc"));
            snn_require(!json::is_valid("-3.92.38"));
            snn_require(!json::is_valid("-74.13-"));

            snn_require(!json::is_valid("0.0f0"));
            snn_require(!json::is_valid("0.75Ee12"));
            snn_require(!json::is_valid("3.1ee3"));
            snn_require(!json::is_valid("3.92E789E123"));
            snn_require(!json::is_valid("74.3E2a"));
            snn_require(!json::is_valid("74.13e74ABC"));
            snn_require(!json::is_valid("-0.0f0"));
            snn_require(!json::is_valid("-0.75Ee12"));
            snn_require(!json::is_valid("-3.1ee3"));
            snn_require(!json::is_valid("-3.92E789E123"));
            snn_require(!json::is_valid("-74.3E2a"));
            snn_require(!json::is_valid("-74.13e74ABC"));

            snn_require(!json::is_valid("0.0e*0"));
            snn_require(!json::is_valid("0.75E--12"));
            snn_require(!json::is_valid("3.1e-3e-3"));
            snn_require(!json::is_valid("3.92E++789"));
            snn_require(!json::is_valid("74.3E+2a"));
            snn_require(!json::is_valid("74.13e-74abc"));
            snn_require(!json::is_valid("-0.0e*0"));
            snn_require(!json::is_valid("-0.75E--12"));
            snn_require(!json::is_valid("-3.1e-3e-3"));
            snn_require(!json::is_valid("-3.92E++789"));
            snn_require(!json::is_valid("-74.3E+2a"));
            snn_require(!json::is_valid("-74.13e-74abc"));

            // Limits

            snn_require(json::is_valid(R"(["One"])"));
            snn_require(json::is_valid(R"(["One"])", 1, 99));  // Depth
            snn_require(!json::is_valid(R"(["One"])", 0, 99)); // Depth
            snn_require(json::is_valid(R"(["One"])", 99, 2));  // Nodes
            snn_require(!json::is_valid(R"(["One"])", 99, 1)); // Nodes

            snn_require(json::is_valid(R"(["One", "Two"])"));
            snn_require(json::is_valid(R"(["One", "Two"])", 1, 99));  // Depth
            snn_require(!json::is_valid(R"(["One", "Two"])", 0, 99)); // Depth
            snn_require(json::is_valid(R"(["One", "Two"])", 99, 3));  // Nodes
            snn_require(!json::is_valid(R"(["One", "Two"])", 99, 2)); // Nodes

            snn_require(json::is_valid(R"({"One":1,"Two":{"TwoTwo":22,"ThreeThree":33}})"));
            // Depth
            snn_require(json::is_valid(R"({"One":1,"Two":{"TwoTwo":22,"ThreeThree":33}})", 2, 99));
            snn_require(!json::is_valid(R"({"One":1,"Two":{"TwoTwo":22,"ThreeThree":33}})", 1, 99));
            // Nodes
            snn_require(json::is_valid(R"({"One":1,"Two":{"TwoTwo":22,"ThreeThree":33}})", 99, 9));
            snn_require(!json::is_valid(R"({"One":1,"Two":{"TwoTwo":22,"ThreeThree":33}})", 99, 8));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_decoder());
        snn_static_require(app::test_is_valid());

        static_assert(sizeof(json::node) == 48);

        // JSON Parsing Test Suite
        // https://github.com/nst/JSONTestSuite
        {
            usize invalid_count = 0;
            usize valid_count   = 0;

            const str test_suite_dir = "decoder.test.data/";
            const auto entries       = file::dir::list(test_suite_dir).value();
            for (const auto& e : entries)
            {
                const auto name = e.name().view();
                if (name.has_back(".json"))
                {
                    const str path      = concat(test_suite_dir, name);
                    const auto contents = file::read<strbuf>(path).value();

                    if (name.has_front("y_"))
                    {
                        snn_require(json::is_valid(contents));
                        ++valid_count;
                    }
                    else if (name.has_front("n_"))
                    {
                        snn_require(!json::is_valid(contents));
                        ++invalid_count;
                    }
                    else if (name.has_front("i_number_"))
                    {
                        snn_require(json::is_valid(contents));
                        ++valid_count;
                    }
                    else if (name.has_front("i_"))
                    {
                        snn_require(!json::is_valid(contents));
                        ++invalid_count;
                    }
                    else
                    {
                        snn_require(false && "Unknown .json file");
                    }
                }
            }

            snn_require(invalid_count == 213);
            snn_require(valid_count == 105);
        }
    }
}
