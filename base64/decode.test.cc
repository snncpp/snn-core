// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/base64/decode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(base64::decode("").value() == "");
            snn_require(base64::decode("Zg==").value() == "f");
            snn_require(base64::decode("Zm8=").value() == "fo");
            snn_require(base64::decode("Zm9v").value() == "foo");
            snn_require(base64::decode("Zm9vYg==").value() == "foob");
            snn_require(base64::decode("Zm9vYmE=").value() == "fooba");
            snn_require(base64::decode("Zm9vYmFy").value() == "foobar");

            snn_require(!base64::decode("Zm9vYg"));
            snn_require(base64::decode("Zm9vYg", base64::padding_optional).value() == "foob");

            snn_require(!base64::decode("Zm9vYg..", base64::padding_required));
            snn_require(base64::decode("Zm9vYg..", base64::padding_required, '.').value() ==
                        "foob");

            snn_require(base64::decode("w6U=").value() == "å");
            snn_require(base64::decode("w6XDpA==").value() == "åä");
            snn_require(base64::decode("w6XDpMO2").value() == "åäö");

            strbuf s;
            snn_require(base64::decode("w6U=", s, assume::no_overlap));
            snn_require(base64::decode("w6XDpA==", s, assume::no_overlap));
            snn_require(base64::decode("w6XDpMO2", s, assume::no_overlap));
            snn_require(s == "ååäåäö");

            // Random data where the output contains '/' and '+'.

            snn_require(base64::decode("gH/+eHYKkktC").value() ==
                        "\x80\x7f\xfe\x78\x76\x0a\x92\x4b\x42");
            snn_require(base64::decode("Kd+nwkcJXLOiyth/MQ==").value() ==
                        "\x29\xdf\xa7\xc2\x47\x09\x5c\xb3\xa2\xca\xd8\x7f\x31");
            snn_require(base64::decode("HO/0+WjpWuI=").value() ==
                        "\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2");

            str dst{"One two three four?"};
            snn_require(dst.capacity() == str::default_capacity());
            str src{"HO/0+WjpWuI="};
            snn_require(base64::decode(src, dst, assume::no_overlap));
            snn_require(dst == "One two three four?\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2");
            snn_require(dst.capacity() > str::default_capacity());

            constexpr cstrview invalid_tail{"w6XDpMO,"};
            static_assert(invalid_tail.size() % 4 == 0);
            snn_require(!base64::decode(invalid_tail, dst, assume::no_overlap));
            snn_require(dst == "One two three four?\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2");
            snn_require(dst.size() == 27);

            // Longer text.
            snn_require(
                base64::decode(
                    "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGl"
                    "zIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2"
                    "YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29ud"
                    "GludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRz"
                    "IHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=")
                    .value() ==
                "Man is distinguished, not only by his reason, but by this singular "
                "passion from other animals, which is a lust of the mind, that by a "
                "perseverance of delight in the continued and indefatigable generation "
                "of knowledge, exceeds the short vehemence of any carnal pleasure.");

            snn_require(base64::decode(""));
            snn_require(base64::decode("Zm9v"));
            snn_require(base64::decode("Zm9vYg=="));
            snn_require(base64::decode("Zm9vYmE="));

            snn_require(!base64::decode("HO_0-WjpWuI=")); // URL encoding.

            // Invalid size.
            snn_require(!base64::decode("Zm9vYmF"));
            snn_require(!base64::decode("Zm9vYm"));
            snn_require(!base64::decode("Zm9vY"));
            snn_require(!base64::decode("Zm9"));
            snn_require(!base64::decode("Zm"));
            snn_require(!base64::decode("Z"));

            {
                auto r = base64::decode("Z");
                snn_require(!r);
                snn_require(r.error_code() == base64::error::invalid_size);
            }

            // Invalid char.
            snn_require(!base64::decode("-m9vYg=="));
            snn_require(!base64::decode("Z-9vYg=="));
            snn_require(!base64::decode("Zm-vYg=="));
            snn_require(!base64::decode("Zm9-Yg=="));
            snn_require(!base64::decode("Zm9v-g=="));
            snn_require(!base64::decode("Zm9vY-=="));
            snn_require(!base64::decode("Zm9vYg-="));
            snn_require(!base64::decode("Zm9vYg=-"));

            {
                auto r = base64::decode("Z-9vYg==");
                snn_require(!r);
                snn_require(r.error_code() == base64::error::invalid_character);
            }

            // Invalid padding char.
            snn_require(!base64::decode("Zm9vYg.="));
            snn_require(!base64::decode("Zm9vYg=."));
            snn_require(!base64::decode("Zm9vYg.."));

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
