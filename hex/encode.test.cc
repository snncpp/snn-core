// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/hex/encode.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(hex::encode("") == "");
            snn_require(hex::encode("f") == "66");
            snn_require(hex::encode("fo") == "666f");
            snn_require(hex::encode("foo") == "666f6f");
            snn_require(hex::encode("foob") == "666f6f62");
            snn_require(hex::encode("fooba") == "666f6f6261");
            snn_require(hex::encode("foobar") == "666f6f626172");

            snn_require(hex::encode("foobar", hex::table::upper) == "666F6F626172");

            snn_require(hex::encode("å") == "c3a5");
            snn_require(hex::encode("åä") == "c3a5c3a4");
            snn_require(hex::encode("åäö") == "c3a5c3a4c3b6");

            snn_require(hex::encode("åäö", hex::table::upper) == "C3A5C3A4C3B6");

            return true;
        }

        constexpr bool test_encode()
        {
            {
                strbuf s;
                hex::encode("å", s, promise::no_overlap);
                hex::encode("åä", s, promise::no_overlap);
                hex::encode("åäö", s, promise::no_overlap);
                snn_require(s == "c3a5c3a5c3a4c3a5c3a4c3b6");

                s.clear();
                hex::encode("å", hex::table::upper, s, promise::no_overlap);
                hex::encode("åä", hex::table::upper, s, promise::no_overlap);
                hex::encode("åäö", hex::table::upper, s, promise::no_overlap);
                snn_require(s == "C3A5C3A5C3A4C3A5C3A4C3B6");
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
                snn_require(
                    hex::encode<strbuf>(s) ==
                    "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2021222"
                    "32425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f40414243444546"
                    "4748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696"
                    "a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d"
                    "8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b"
                    "1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4"
                    "d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f"
                    "8f9fafbfcfdfeff");
            }
            {
                str dest{"One two three?"};
                snn_require(dest.capacity() == str::default_capacity());
                str src{"åäö"};
                hex::encode(src, dest, promise::no_overlap);
                snn_require(dest == "One two three?c3a5c3a4c3b6");
                snn_require(dest.capacity() > str::default_capacity());
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
        snn_static_require(app::test_encode());
    }
}
