// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/crypto/hash/sha1.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            crypto::hash::sha1 h;

            h.update("abc");
            str digest = h.final_hex();
            snn_require(digest.size() == 40);
            snn_require(digest == "a9993e364706816aba3e25717850c26c9cd0d89d");

            h.reset();

            h << "a" << "b" << "c";
            digest = h.final_hex();
            snn_require(digest == "a9993e364706816aba3e25717850c26c9cd0d89d");

            return true;
        }

        void test_hex(const cstrview input, const cstrview output)
        {
            crypto::hash::sha1 h;
            h.update(input);
            str s = h.final_hex();
            snn_require(s.size() == 40);
            snn_require(s == output);
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        static_assert(crypto::hash::sha1::block_size == 64);
        static_assert(crypto::hash::sha1::output_size == 20);

        app::test_hex("", "da39a3ee5e6b4b0d3255bfef95601890afd80709");
        app::test_hex("abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
        app::test_hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                      "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
        app::test_hex("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklm"
                      "nopqklmnopqrlmnopqrsmnopqrstnopqrstu",
                      "a49b2446a02c645bf419f995b67091253a04a259");
        app::test_hex("The quick brown fox jumps over the lazy dog",
                      "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");
        app::test_hex("The quick brown fox jumps over the lazy cog",
                      "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3");

        {
            crypto::hash::sha1 h;
            str s{"One Two Three"};
            h.final_hex(s);
            snn_require(s.size() == 53);
            snn_require(s == "One Two Threeda39a3ee5e6b4b0d3255bfef95601890afd80709");
        }

        {
            crypto::hash::sha1 h;
            str s = h.final();
            snn_require(s.size() == 20);
            snn_require(s == "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55"
                             "\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09");
        }

        {
            crypto::hash::sha1 h;
            h << str{container::fill, 900'000, 'a'} << str{container::fill, 100'000, 'a'};
            auto s = h.final_hex<strbuf>();
            snn_require(s.size() == 40);
            snn_require(s == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
        }
    }
}
