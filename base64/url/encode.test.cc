// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/base64/url/encode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                // Encodes with URL safe characters and without padding.

                snn_require(base64::url::encode("") == "");
                snn_require(base64::url::encode("f") == "Zg");
                snn_require(base64::url::encode("fo") == "Zm8");
                snn_require(base64::url::encode("foo") == "Zm9v");
                snn_require(base64::url::encode("foob") == "Zm9vYg");
                snn_require(base64::url::encode("fooba") == "Zm9vYmE");
                snn_require(base64::url::encode("foobar") == "Zm9vYmFy");

                // Random data where the output contains '-' and '_' ('+' and '/' in standard
                // encoding).

                snn_require(base64::url::encode("\x80\x7f\xfe\x78\x76\x0a\x92\x4b\x42") ==
                            "gH_-eHYKkktC");
                snn_require(
                    base64::url::encode("\x29\xdf\xa7\xc2\x47\x09\x5c\xb3\xa2\xca\xd8\x7f\x31") ==
                    "Kd-nwkcJXLOiyth_MQ");
                snn_require(base64::url::encode("\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2") ==
                            "HO_0-WjpWuI");
            }

            {
                str s{"one two three"};
                base64::url::encode("foob", s, assume::no_overlap);
                base64::url::encode("\x1c\xef\xf4\xf9\x68\xe9\x5a\xe2", s, assume::no_overlap);
                snn_require(s == "one two three"
                                 "Zm9vYg"
                                 "HO_0-WjpWuI");
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
    }
}
