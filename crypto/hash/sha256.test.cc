// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/crypto/hash/sha256.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            crypto::hash::sha256 h;

            h.update("abc");
            str digest = h.final_hex();
            snn_require(digest ==
                        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

            h.reset();

            h << "a" << "b" << "c";
            digest = h.final_hex();
            snn_require(digest ==
                        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

            return true;
        }

        void test_hex(const cstrview input, const cstrview output)
        {
            crypto::hash::sha256 h;
            h.update(input);
            const str s = h.final_hex();
            snn_require(s.size() == 64);
            snn_require(s == output);
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        static_assert(crypto::hash::sha256::block_size == 64);
        static_assert(crypto::hash::sha256::output_size == 32);

        app::test_hex("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
        app::test_hex("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
        app::test_hex("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
                      "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
        app::test_hex(
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmno"
            "pqrlmnopqrsmnopqrstnopqrstu",
            "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");

        {
            crypto::hash::sha256 h;
            str s{"One Two Three"};
            h.final_hex(s);
            snn_require(s.size() == 77);
            snn_require(
                s ==
                "One Two Threee3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
        }

        {
            crypto::hash::sha256 h;
            str s = h.final();
            snn_require(s.size() == 32);
            snn_require(s == "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24"
                             "\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55");
        }

        {
            crypto::hash::sha256 h;
            h << str{init::fill, 900'000, 'a'} << str{init::fill, 100'000, 'a'};
            auto s = h.final_hex<strbuf>();
            snn_require(s.size() == 64);
            snn_require(s == "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
        }
    }
}
