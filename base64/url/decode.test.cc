// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/base64/url/decode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // With or without padding (short padding is allowed).

            snn_require(base64::url::decode("").value() == "");
            snn_require(base64::url::decode("Zg").value() == "f");
            snn_require(base64::url::decode("Zg=").value() == "f"); // Short padding.
            snn_require(base64::url::decode("Zg==").value() == "f");
            snn_require(base64::url::decode("Zm8").value() == "fo");
            snn_require(base64::url::decode("Zm8=").value() == "fo");
            snn_require(base64::url::decode("Zm9v").value() == "foo");
            snn_require(base64::url::decode("Zm9vYg").value() == "foob");
            snn_require(base64::url::decode("Zm9vYg=").value() == "foob"); // Short padding.
            snn_require(base64::url::decode("Zm9vYg==").value() == "foob");
            snn_require(base64::url::decode("Zm9vYmE").value() == "fooba");
            snn_require(base64::url::decode("Zm9vYmE=").value() == "fooba");
            snn_require(base64::url::decode("Zm9vYmFy").value() == "foobar");

            snn_require(base64::url::decode("w6U").value() == "å");
            snn_require(base64::url::decode("w6U=").value() == "å");
            snn_require(base64::url::decode("w6XDpA").value() == "åä");
            snn_require(base64::url::decode("w6XDpA=").value() == "åä"); // Short padding.
            snn_require(base64::url::decode("w6XDpA==").value() == "åä");
            snn_require(base64::url::decode("w6XDpMO2").value() == "åäö");

            strbuf s;
            snn_require(base64::url::decode("w6U", s, promise::no_overlap));
            snn_require(base64::url::decode("w6XDpA", s, promise::no_overlap));
            snn_require(base64::url::decode("w6XDpMO2", s, promise::no_overlap));
            snn_require(s == "ååäåäö");
            s.clear();
            snn_require(base64::url::decode("w6U=", s, promise::no_overlap));
            snn_require(base64::url::decode("w6XDpA==", s, promise::no_overlap));
            snn_require(base64::url::decode("w6XDpMO2", s, promise::no_overlap));
            snn_require(s == "ååäåäö");

            // Random data where the output contains '-' and '_' ('+' and '/' in standard encoding).

            snn_require(base64::url::decode("gH_-eHYKkktC").value() ==
                        "\x80\x7f\xfe\x78\x76\x0a\x92\x4b\x42");
            snn_require(base64::url::decode("Kd-nwkcJXLOiyth_MQ").value() ==
                        "\x29\xdf\xa7\xc2\x47\x09\x5c\xb3\xa2\xca\xd8\x7f\x31");
            snn_require(base64::url::decode("HO_0-WjpWuI").value() ==
                        "\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2");
            snn_require(!base64::url::decode("HO/0+WjpWuI")); // Standard encoding.

            str dst{"One two three four?"};
            snn_require(dst.capacity() == str::default_capacity());
            str src{"HO_0-WjpWuI"};
            snn_require(base64::url::decode(src, dst, promise::no_overlap));
            snn_require(dst == "One two three four?\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2");
            snn_require(dst.capacity() > str::default_capacity());

            // Longer text (without padding).
            snn_require(base64::url::decode(
                            "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBi"
                            "eSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBp"
                            "cyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVs"
                            "aWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24g"
                            "b2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNh"
                            "cm5hbCBwbGVhc3VyZS4")
                            .value() ==
                        "Man is distinguished, not only by his reason, but by this singular "
                        "passion from other animals, which is a lust of the mind, that by a "
                        "perseverance of delight in the continued and indefatigable generation "
                        "of knowledge, exceeds the short vehemence of any carnal pleasure.");

            // Invalid padding or size (single trailing character).
            snn_require(!base64::url::decode("Z"));
            snn_require(!base64::url::decode("Zm9vv"));
            snn_require(!base64::url::decode("Zg==="));
            snn_require(!base64::url::decode("Zm9v="));
            snn_require(!base64::url::decode("Zm9v=="));
            snn_require(!base64::url::decode("Zm9v==="));
            snn_require(!base64::url::decode("Zm9v===="));
            snn_require(!base64::url::decode("Zm9v====="));
            snn_require(!base64::url::decode("Zm9v======"));
            snn_require(!base64::url::decode("Zm9v======="));
            snn_require(!base64::url::decode("Zm9v========"));

            // Invalid characters or size.
            snn_require(!base64::url::decode("!"));
            snn_require(!base64::url::decode("Z!"));
            snn_require(!base64::url::decode("Zg!"));
            snn_require(!base64::url::decode("Zg=!"));
            snn_require(!base64::url::decode("!m8"));
            snn_require(!base64::url::decode("Z!8="));
            snn_require(!base64::url::decode("Zm!v"));
            snn_require(!base64::url::decode("Zm9!Yg"));
            snn_require(!base64::url::decode("Zm9v!g="));
            snn_require(!base64::url::decode("Zm9vY!=="));
            snn_require(!base64::url::decode("Zm9vYm!"));
            snn_require(!base64::url::decode("Zm9vYmE!"));
            snn_require(!base64::url::decode("!m9vYmFy"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
