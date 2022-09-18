// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/base64/encode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Test vectors from: https://www.rfc-editor.org/rfc/rfc4648

            snn_require(base64::encode("") == "");
            snn_require(base64::encode("f") == "Zg==");
            snn_require(base64::encode("fo") == "Zm8=");
            snn_require(base64::encode("foo") == "Zm9v");
            snn_require(base64::encode("foob") == "Zm9vYg==");
            snn_require(base64::encode("fooba") == "Zm9vYmE=");
            snn_require(base64::encode("foobar") == "Zm9vYmFy");

            snn_require(base64::encode("", base64::with_padding) == "");
            snn_require(base64::encode("f", base64::with_padding) == "Zg==");
            snn_require(base64::encode("fo", base64::with_padding) == "Zm8=");
            snn_require(base64::encode("foo", base64::with_padding) == "Zm9v");
            snn_require(base64::encode("foob", base64::with_padding) == "Zm9vYg==");
            snn_require(base64::encode("fooba", base64::with_padding) == "Zm9vYmE=");
            snn_require(base64::encode("foobar", base64::with_padding) == "Zm9vYmFy");

            snn_require(base64::encode("", base64::with_padding, '.') == "");
            snn_require(base64::encode("f", base64::with_padding, '.') == "Zg..");
            snn_require(base64::encode("fo", base64::with_padding, '.') == "Zm8.");
            snn_require(base64::encode("foo", base64::with_padding, '.') == "Zm9v");
            snn_require(base64::encode("foob", base64::with_padding, '.') == "Zm9vYg..");
            snn_require(base64::encode("fooba", base64::with_padding, '.') == "Zm9vYmE.");
            snn_require(base64::encode("foobar", base64::with_padding, '.') == "Zm9vYmFy");

            snn_require(base64::encode("", base64::without_padding) == "");
            snn_require(base64::encode("f", base64::without_padding) == "Zg");
            snn_require(base64::encode("fo", base64::without_padding) == "Zm8");
            snn_require(base64::encode("foo", base64::without_padding) == "Zm9v");
            snn_require(base64::encode("foob", base64::without_padding) == "Zm9vYg");
            snn_require(base64::encode("fooba", base64::without_padding) == "Zm9vYmE");
            snn_require(base64::encode("foobar", base64::without_padding) == "Zm9vYmFy");

            return true;
        }

        constexpr bool test_encode()
        {
            snn_require(base64::encode("å") == "w6U=");
            snn_require(base64::encode("åä") == "w6XDpA==");
            snn_require(base64::encode("åäö") == "w6XDpMO2");

            strbuf s;
            base64::encode("å", s, promise::no_overlap);
            base64::encode(cstrview{"åä"}, s, promise::no_overlap);
            base64::encode("åäö", s, promise::no_overlap);
            snn_require(s == "w6U=w6XDpA==w6XDpMO2");

            // Random data where the output contains '/' and '+'.
            snn_require(base64::encode("\x80\x7f\xfe\x78\x76\x0a\x92\x4b\x42") == "gH/+eHYKkktC");
            snn_require(base64::encode("\x29\xdf\xa7\xc2\x47\x09\x5c\xb3\xa2\xca\xd8\x7f\x31") ==
                        "Kd+nwkcJXLOiyth/MQ==");
            snn_require(base64::encode("\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2") == "HO/0+WjpWuI=");

            snn_require(
                base64::encode(
                    "Man is distinguished, not only by his reason, but by this singular "
                    "passion from other animals, which is a lust of the mind, that by a "
                    "perseverance of delight in the continued and indefatigable generation "
                    "of knowledge, exceeds the short vehemence of any carnal pleasure.") ==
                "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
                "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
                "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
                "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
                "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");

            str dest{"One two three four?"};
            snn_require(dest.capacity() == str::default_capacity());
            str src{"åäö"};
            base64::encode(src, dest, promise::no_overlap);
            snn_require(dest == "One two three four?w6XDpMO2");
            snn_require(dest.capacity() > str::default_capacity());

            return true;
        }

        constexpr bool test_encoded_size()
        {
            snn_require(base64::detail::encoded_size(0, base64::with_padding) == 0);
            snn_require(base64::detail::encoded_size(1, base64::with_padding) == 4);
            snn_require(base64::detail::encoded_size(2, base64::with_padding) == 4);
            snn_require(base64::detail::encoded_size(3, base64::with_padding) == 4);
            snn_require(base64::detail::encoded_size(4, base64::with_padding) == 8);
            snn_require(base64::detail::encoded_size(5, base64::with_padding) == 8);
            snn_require(base64::detail::encoded_size(6, base64::with_padding) == 8);
            snn_require(base64::detail::encoded_size(7, base64::with_padding) == 12);
            snn_require(base64::detail::encoded_size(8, base64::with_padding) == 12);
            snn_require(base64::detail::encoded_size(9, base64::with_padding) == 12);

            snn_require(base64::detail::encoded_size(0, base64::without_padding) == 0);
            snn_require(base64::detail::encoded_size(1, base64::without_padding) == 2);
            snn_require(base64::detail::encoded_size(2, base64::without_padding) == 3);
            snn_require(base64::detail::encoded_size(3, base64::without_padding) == 4);
            snn_require(base64::detail::encoded_size(4, base64::without_padding) == 6);
            snn_require(base64::detail::encoded_size(5, base64::without_padding) == 7);
            snn_require(base64::detail::encoded_size(6, base64::without_padding) == 8);
            snn_require(base64::detail::encoded_size(7, base64::without_padding) == 10);
            snn_require(base64::detail::encoded_size(8, base64::without_padding) == 11);
            snn_require(base64::detail::encoded_size(9, base64::without_padding) == 12);

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
        snn_static_require(app::test_encoded_size());
    }
}
