// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/hex/decode.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(hex::decode("").value() == "");
            snn_require(hex::decode("66").value() == "f");
            snn_require(hex::decode("666F").value() == "fo");
            snn_require(hex::decode("666f6f").value() == "foo");
            snn_require(hex::decode("666f6f62").value() == "foob");
            snn_require(hex::decode("666F6F6261").value() == "fooba");
            snn_require(hex::decode("666f6f626172").value() == "foobar");

            snn_require(hex::decode("c3a5").value() == "å");
            snn_require(hex::decode("C3A5c3a4").value() == "åä");
            snn_require(hex::decode("c3a5c3a4c3b6").value() == "åäö");

            snn_require(!hex::decode("abc"));  // Invalid size.
            snn_require(!hex::decode("abc.")); // Invalid character.

            return true;
        }

        constexpr bool test_decode()
        {
            {
                strbuf s;
                snn_require(hex::decode("c3a5", s, promise::no_overlap));
                snn_require(hex::decode("c3a5c3a4", s, promise::no_overlap));
                snn_require(hex::decode("c3a5c3a4c3b6", s, promise::no_overlap));
                snn_require(s == "ååäåäö");
            }

            {
                str s;
                s.reserve(256);
                snn_require(s.size() == 0);
                snn_require(s.capacity() >= 256);
                for (const auto b : range::integral<byte>{})
                {
                    s << to_char(b);
                }
                snn_require(s.size() == 256);
                snn_require(hex::decode("000102030405060708090a0b0c0d0e0f101112131415161718191a1b"
                                        "1c1d1e1f202122232425262728292a2b2c2d2e2f3031323334353637"
                                        "38393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f50515253"
                                        "5455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f"
                                        "707172737475767778797a7b7c7d7e7f808182838485868788898a8b"
                                        "8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7"
                                        "a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3"
                                        "c4c5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
                                        "E0E1E2E3E4E5E6E7E8E9EAEBECedeeeff0f1f2f3f4f5f6f7f8f9fafb"
                                        "fcfdfeff")
                                .value() == s);
            }

            {
                str dest{"One two three four?"};
                snn_require(dest.capacity() == str::default_capacity());

                str src{"c3a5c3a4c3b6"};
                snn_require(hex::decode(src, dest, promise::no_overlap));
                snn_require(dest == "One two three four?åäö");
                snn_require(dest.size() == 25);
                snn_require(dest.capacity() > str::default_capacity());

                const cstrview invalid_tail{"c3a5c3a4c3b6_!"};
                snn_require(invalid_tail.size() % 2 == 0);
                snn_require(!hex::decode(invalid_tail, dest, promise::no_overlap));
                snn_require(dest == "One two three four?åäö");
                snn_require(dest.size() == 25);
            }

            snn_require(hex::decode("66"));
            snn_require(hex::decode("ff"));
            snn_require(hex::decode("ba"));
            snn_require(hex::decode("ab"));

            snn_require(!hex::decode("6"));
            snn_require(!hex::decode("fff"));
            snn_require(!hex::decode("ga"));
            snn_require(!hex::decode("ag"));
            snn_require(!hex::decode("!!"));

            {
                auto r = hex::decode("6");
                snn_require(!r);
                snn_require(r.error_code() == hex::error::invalid_size);
            }

            {
                auto r = hex::decode("ag");
                snn_require(!r);
                snn_require(r.error_code() == hex::error::invalid_character);
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
        snn_static_require(app::test_decode());
    }
}
