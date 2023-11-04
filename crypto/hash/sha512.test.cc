// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/crypto/hash/sha512.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            crypto::hash::sha512 h;

            h.update("abc");
            str digest = h.final_hex();
            snn_require(digest.size() == 128);
            snn_require(digest == "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea2"
                                  "0a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd"
                                  "454d4423643ce80e2a9ac94fa54ca49f");

            h.reset();

            h << "a" << "b" << "c";
            digest = h.final_hex();
            snn_require(digest == "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea2"
                                  "0a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd"
                                  "454d4423643ce80e2a9ac94fa54ca49f");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        static_assert(crypto::hash::sha512::block_size == 128);
        static_assert(crypto::hash::sha512::output_size == 64);

        {
            crypto::hash::sha512 h;
            h.update("abc");
            str s = h.final_hex();
            snn_require(s.size() == 128);
            snn_require(s == "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a21929"
                             "92a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");
        }

        {
            crypto::hash::sha512 h;
            str s = h.final_hex();
            snn_require(s.size() == 128);
            snn_require(s == "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d"
                             "13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
        }

        {
            crypto::hash::sha512 h;
            h << "";
            str s = h.final_hex();
            snn_require(s.size() == 128);
            snn_require(s == "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d"
                             "13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
        }

        {
            crypto::hash::sha512 h;
            h << "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
            str s = h.final_hex();
            snn_require(s.size() == 128);
            snn_require(s == "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd1"
                             "5c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445");
        }

        {
            crypto::hash::sha512 h;
            h << "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqk"
                 "lmnopqrlmnopqrsmnopqrstnopqrstu";
            str s = h.final_hex();
            snn_require(s.size() == 128);
            snn_require(s == "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d2"
                             "89e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909");
        }

        {
            crypto::hash::sha512 h;
            h << str{container::fill, 1'000'000, 'a'};
            str s = h.final_hex();
            snn_require(s.size() == 128);
            snn_require(s == "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff"
                             "244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b");
        }

        {
            crypto::hash::sha512 h;
            h << str{container::fill, 100'000, 'a'} << str{container::fill, 900'000, 'a'};
            auto s = h.final_hex<strbuf>();
            snn_require(s.size() == 128);
            snn_require(s == "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff"
                             "244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b");
        }
    }
}
