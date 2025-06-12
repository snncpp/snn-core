// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/crypto/hash/md5.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            crypto::hash::md5 h;

            h.update("abc");
            str digest = h.final_hex();
            snn_require(digest.size() == 32);
            snn_require(digest == "900150983cd24fb0d6963f7d28e17f72");

            h.reset();

            h << "a" << "b" << "c";
            digest = h.final_hex();
            snn_require(digest == "900150983cd24fb0d6963f7d28e17f72");

            return true;
        }

        void test_hex(const cstrview input, const cstrview output)
        {
            crypto::hash::md5 h;
            h.update(input);
            str s = h.final_hex();
            snn_require(s.size() == 32);
            snn_require(s == output);
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        static_assert(crypto::hash::md5::block_size == 64);
        static_assert(crypto::hash::md5::output_size == 16);

        app::test_hex("", "d41d8cd98f00b204e9800998ecf8427e");
        app::test_hex("a", "0cc175b9c0f1b6a831c399e269772661");
        app::test_hex("abc", "900150983cd24fb0d6963f7d28e17f72");
        app::test_hex("message digest", "f96b697d7cb7938d525a2f31aaf161d0");
        app::test_hex("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
        app::test_hex("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
                      "d174ab98d277d9f5a5611c2c9f419d9f");
        app::test_hex(
            "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "57edf4a22be3c955ac49da2e2107b67a");
        app::test_hex("The quick brown fox jumps over the lazy dog",
                      "9e107d9d372bb6826bd81d3542a419d6");
        app::test_hex("The quick brown fox jumps over the lazy dog.",
                      "e4d909c290d0fb1ca068ffaddf22cbd0");
        app::test_hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                      "8215ef0796a20bcaaae116d3876c664a");

        {
            crypto::hash::md5 h;
            str s{"One Two Three"};
            h.final_hex(s);
            snn_require(s.size() == 45);
            snn_require(s == "One Two Threed41d8cd98f00b204e9800998ecf8427e");
        }

        {
            crypto::hash::md5 h;
            str s = h.final();
            snn_require(s.size() == 16);
            snn_require(s == "\xd4\x1d\x8c\xd9\x8f\x00\xb2\x04\xe9\x80\x09\x98\xec\xf8\x42\x7e");
        }

        {
            crypto::hash::md5 h;
            h << str{init::fill, 900'000, 'a'} << str{init::fill, 100'000, 'a'};
            auto s = h.final_hex<strbuf>();
            snn_require(s.size() == 32);
            snn_require(s == "7707d6ae4e027c70eea2a935c2296f21");
        }
    }
}
