// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/upper.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::upper("AbCdef123") == "ABCDEF123");
            // Only ASCII characters are converted.
            snn_require(ascii::upper("AbCdef123Åäö") == "ABCDEF123Åäö");

            str s;
            ascii::upper("AbC", s);
            ascii::upper("def123Åäö", s);
            snn_require(s == "ABCDEF123Åäö");

            return true;
        }

        constexpr bool test_upper()
        {
            str all;
            for (const auto b : range::integral<byte>{})
            {
                all << to_char(b);
            }
            snn_require(all.size() == 256);
            snn_require(
                all ==
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12"
                "\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f "
                "!\"#$%&'()*+,-./"
                "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}"
                "~\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91"
                "\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4"
                "\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7"
                "\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca"
                "\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd"
                "\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0"
                "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff");

            snn_require(ascii::upper("") == "");
            snn_require(ascii::upper("a") == "A");
            snn_require(ascii::upper("A") == "A");

            // Only a-z gets uppercased.
            snn_require(
                ascii::upper(all) ==
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12"
                "\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f "
                "!\"#$%&'()*+,-./"
                "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}"
                "~\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91"
                "\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4"
                "\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7"
                "\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca"
                "\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd"
                "\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0"
                "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff");

            // UTF-8 (does nothing).
            snn_require(ascii::upper("Åäö") == "\xc3\x85\xc3\xa4\xc3\xb6");

            str abc{"abcdefghijklmnopqrst"};
            snn_require(abc.capacity() == abc.default_capacity());
            ascii::upper("", abc);
            snn_require(abc == "abcdefghijklmnopqrst");
            ascii::upper("u", abc);
            snn_require(abc == "abcdefghijklmnopqrstU");
            ascii::upper("vwxyz0123456789", abc);
            snn_require(abc == "abcdefghijklmnopqrstUVWXYZ0123456789");
            snn_require(abc.capacity() > abc.default_capacity());

            // From self.
            ascii::upper(abc.view(), abc);
            snn_require(abc == "abcdefghijklmnopqrstUVWXYZ0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_upper());
    }
}
