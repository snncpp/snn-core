// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/url/encode.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(url::encode("abc") == "abc");

            snn_require(url::encode("åäö") == "%C3%A5%C3%A4%C3%B6");
            snn_require(url::encode("åäö", hex::table::lower) == "%c3%a5%c3%a4%c3%b6");

            snn_require(url::encode("abc123åäö") == "abc123%C3%A5%C3%A4%C3%B6");

            snn_require(url::encode("~/this-and_that.txt") == "~%2Fthis-and_that.txt");

            str s;
            url::encode("abc", s, promise::no_overlap);
            url::encode("123åäö", s, promise::no_overlap);
            snn_require(s == "abc123%C3%A5%C3%A4%C3%B6");

            return true;
        }

        constexpr bool test_encode()
        {
            {
                snn_require(url::encode("") == "");
                snn_require(url::encode("f") == "f");
                snn_require(url::encode("fo") == "fo");
                snn_require(url::encode("foo") == "foo");
            }

            {
                str s;
                url::encode("foo", s, promise::no_overlap);
                url::encode(cstrview{"åäö"}, s, promise::no_overlap);
                snn_require(s == "foo%C3%A5%C3%A4%C3%B6");
            }

            {
                str s{"åäö"};
                snn_require(s.size() == 6);
                snn_require(url::encode(s) == "%C3%A5%C3%A4%C3%B6");

                snn_require(url::encode(str{"åäö"}, hex::table::lower) == "%c3%a5%c3%a4%c3%b6");
            }

            {
                str dest{"one"};
                strbuf src{"fooåäö"};
                url::encode(src, dest, promise::no_overlap);
                snn_require(dest == "onefoo%C3%A5%C3%A4%C3%B6");
            }

            {
                str dest{"one"};
                strbuf src{"fooåäö"};
                url::encode(src, hex::table::lower, dest, promise::no_overlap);
                snn_require(dest == "onefoo%c3%a5%c3%a4%c3%b6");
            }

            {
                str s{"fooåäö"};
                snn_require(s.size() == 9);
                snn_require(url::encode(s) == "foo%C3%A5%C3%A4%C3%B6");
            }

            {
                str dest{"One two & three"};
                snn_require(dest.capacity() == str::default_capacity());
                const cstrview src{"fooåäö"};
                url::encode(src, dest, promise::no_overlap);
                snn_require(dest == "One two & threefoo%C3%A5%C3%A4%C3%B6");
                snn_require(dest.capacity() > str::default_capacity());
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
                    url::encode(s) ==
                    "%00%01%02%03%04%05%06%07%08%09%0A%0B%0C%0D%0E%0F%10%11%12%13%14%15%16%17%18%"
                    "19%1A%1B%1C%1D%1E%1F%20%21%22%23%24%25%26%27%28%29%2A%2B%2C-.%2F0123456789%"
                    "3A%3B%3C%3D%3E%3F%40ABCDEFGHIJKLMNOPQRSTUVWXYZ%5B%5C%5D%5E_%"
                    "60abcdefghijklmnopqrstuvwxyz%7B%7C%7D~%7F%80%81%82%83%84%85%86%87%88%89%8A%"
                    "8B%8C%8D%8E%8F%90%91%92%93%94%95%96%97%98%99%9A%9B%9C%9D%9E%9F%A0%A1%A2%A3%"
                    "A4%A5%A6%A7%A8%A9%AA%AB%AC%AD%AE%AF%B0%B1%B2%B3%B4%B5%B6%B7%B8%B9%BA%BB%BC%"
                    "BD%BE%BF%C0%C1%C2%C3%C4%C5%C6%C7%C8%C9%CA%CB%CC%CD%CE%CF%D0%D1%D2%D3%D4%D5%"
                    "D6%D7%D8%D9%DA%DB%DC%DD%DE%DF%E0%E1%E2%E3%E4%E5%E6%E7%E8%E9%EA%EB%EC%ED%EE%"
                    "EF%F0%F1%F2%F3%F4%F5%F6%F7%F8%F9%FA%FB%FC%FD%FE%FF");
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
