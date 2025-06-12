// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/crypto/kdf/pbkdf2_hmac.hh"

#include "snn-core/unittest.hh"
#include "snn-core/crypto/hash/sha1.hh"
#include "snn-core/crypto/hash/sha256.hh"
#include "snn-core/crypto/hash/sha512.hh"
#include "snn-core/hex/encode.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            // Derive a key from a password, a salt and an iteration count. (The salt should be
            // randomly generated for each user.)

            constexpr cstrview passphrase   = "tipped vaporizer demanding crabgrass";
            constexpr cstrview salt         = "EZHMU7vL2JraXhor";
            constexpr usize iteration_count = 1'000'000;
            const str key =
                crypto::kdf::pbkdf2_hmac<crypto::hash::sha256>(passphrase, salt, iteration_count)
                    .value();
            snn_require(hex::encode(key) ==
                        "d9c3aa491290ea1e2cabb0210d16044db4d16546c199c53dbb0cff1ea80ca1de");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        using crypto::hash::sha1;
        using crypto::hash::sha256;
        using crypto::hash::sha512;
        using crypto::kdf::pbkdf2_hmac;

        str key;

        // SHA1 (PHP used for verification).

        key = pbkdf2_hmac<sha1>("123456", "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) == "3681fd29840b1317d7174675f571d49ae4fc7549");

        key = pbkdf2_hmac<sha1>(str{init::fill, 30, 'a'}, "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) == "3a08a286505f7d4afd1fc89bdaf0edc997bbde78");

        key = pbkdf2_hmac<sha1>(str{init::fill, 200, 'a'}, "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) == "b1354d64577dad2cea24f44e6f054c1cae72fa83");

        key = pbkdf2_hmac<sha1, str, 10>("x", "aaaaaaaa", 4'000).value();
        snn_require(hex::encode(key) == "d572a8b335a0a7b3a30c");

        // SHA256 (PHP used for verification).

        key = pbkdf2_hmac<sha256>("123456", "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) ==
                    "41a8b2113c0ad72acb9d9dc01a443bdfb839da768ea279055d10ef6bd93eb6e6");

        key = pbkdf2_hmac<sha256>(str{init::fill, 30, 'a'}, "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) ==
                    "3c9be1c1d1355dea9441a5bf2d4964259b0293c765df605c9187ad06690591ef");

        key = pbkdf2_hmac<sha256>(str{init::fill, 200, 'a'}, "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) ==
                    "1747615168b6c926e9bcb671593834e03b2305115879db4c74bbf79a8000c414");

        key = pbkdf2_hmac<sha256, str, 16>("x", "aaaaaaaa", 4'000).value();
        snn_require(hex::encode(key) == "1ca36fd543448f9c660d386957dd9150");

        // SHA512 (PHP used for verification).

        key = pbkdf2_hmac<sha512>("123456", "abcdefghijklmnop", 1'000).value();
        snn_require(hex::encode(key) ==
                    "d3af5a1da474c608a7923101b159851ec7f08a785f295fb17b7c2901a7d253ffc99e500d409"
                    "8101887e09e3eb3998de7f4a7c67fecd599223c7af5ee6b7bb8d3");

        key = pbkdf2_hmac<sha512>(str{init::fill, 30, 'a'}, "abcdefghijklmnop", 1'000).value();
        snn_require(
            hex::encode(key) ==
            "8896b7d63f66a1d779cd6e06ebbf832d9b020d61e9a7b5829b1fa4eb273e04f93a48e98345e430baa20"
            "d6bb80ca3144aead3cd647bb71393e5933edb40705795");

        key = pbkdf2_hmac<sha512>(str{init::fill, 200, 'a'}, "abcdefghijklmnop", 1'000).value();
        snn_require(
            hex::encode(key) ==
            "c170b08338d3b3aeaa100014fec5fe0bcb24279d661fc62c575eb4c0f9d47707db270ab3d7ff7521005"
            "8f6c871f9cc45bfcc59e5ec8fe8d8fc0808808179672e");

        key = pbkdf2_hmac<sha512, strbuf, 32>("x", "aaaaaaaa", 4'000).value();
        snn_require(hex::encode(key) ==
                    "5f29cf594efa5c8e2ec79be0b092da9735e4410bb5a33761259edd6919494682");

        // Iteration count is out of range.
        snn_require_throws_code(pbkdf2_hmac<sha1>("abc", "aaaaaaaa", 999).value(),
                                make_error_code(crypto::error::invalid_iteration_count));
        snn_require_throws_code(pbkdf2_hmac<sha1>("abc", "aaaaaaaa", constant::npos).value(),
                                make_error_code(crypto::error::invalid_iteration_count));

        // Password size is invalid.
        snn_require_throws_code(
            pbkdf2_hmac<sha1>(str{init::fill, 4097, 'a'}, "aaaaaaaa", 1'000).value(),
            make_error_code(crypto::error::invalid_password_size));

        // Salt size is invalid.
        snn_require_throws_code(pbkdf2_hmac<sha1>("abc", "aaaaaaa", 1'000).value(),
                                make_error_code(crypto::error::invalid_salt_size));
        snn_require_throws_code(pbkdf2_hmac<sha1>("abc", str{init::fill, 33, 'a'}, 1'000).value(),
                                make_error_code(crypto::error::invalid_salt_size));
    }
}
