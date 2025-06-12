// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/crypto/mac/hmac.hh"

#include "snn-core/unittest.hh"
#include "snn-core/crypto/hash/sha1.hh"
#include "snn-core/crypto/hash/sha256.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            crypto::mac::hmac<crypto::hash::sha256> h{"Jefe"};
            h << "what do ya want" << " for nothing?";
            const str mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        // SHA1
        // Test vectors from: https://www.rfc-editor.org/rfc/rfc2202.html

        {
            crypto::mac::hmac<crypto::hash::sha1> h{str{init::fill, 20, '\x0b'}};
            h << "Hi There";
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "b617318655057264e28bc0b6fb378c8ef146be00");
        }

        {
            crypto::mac::hmac<crypto::hash::sha1> h{"Jefe"};
            h.update("what do ya want for nothing?");
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79");
        }

        {
            crypto::mac::hmac<crypto::hash::sha1> h{str{init::fill, 20, '\xaa'}};
            h << str{init::fill, 50, '\xdd'};
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "125d7342b9ac11cd91a39af48aa17b4f63f175d3");
        }

        {
            crypto::mac::hmac<crypto::hash::sha1> h{"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b"
                                                    "\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16"
                                                    "\x17\x18\x19"};
            h << str{init::fill, 50, '\xcd'};
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "4c9007f4026250c6bc8414f9bf50c86c2d7235da");
        }

        {
            crypto::mac::hmac<crypto::hash::sha1> h{str{init::fill, 20, '\x0c'}};
            h << "Test With Truncation";
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04");
        }

        {
            crypto::mac::hmac<crypto::hash::sha1> h{str{init::fill, 80, '\xaa'}};
            h << "Test Using Larger Than Block-Size Key - Hash Key First";
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "aa4ae5e15272d00e95705637ce8a3b55ed402112");
        }

        {
            crypto::mac::hmac<crypto::hash::sha1> h{str{init::fill, 80, '\xaa'}};
            h << "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data";
            str mac = h.final_hex();
            snn_require(mac.size() == 40);
            snn_require(mac == "e8e99d0f45237d786d6bbaa7965c7808bbff1a91");
        }

        // SHA256
        // Test vectors from: https://www.rfc-editor.org/rfc/rfc4231

        {
            crypto::mac::hmac<crypto::hash::sha256> h{str{init::fill, 20, '\x0b'}};
            h << "Hi There";
            str mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{"Jefe"};
            h << "what do ya want for nothing?";
            str mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{str{init::fill, 20, '\xaa'}};
            h << str{init::fill, 50, '\xdd'};
            str mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a"
                                                      "\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14"
                                                      "\x15\x16\x17\x18\x19"};
            h << str{init::fill, 50, '\xcd'};
            str mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a"
                                                      "\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14"
                                                      "\x15\x16\x17\x18\x19"};
            h << str{init::fill, 50, '\xcd'};
            str raw_mac = h.final();
            snn_require(raw_mac.size() == 32);
            snn_require(raw_mac == "\x82\x55\x8a\x38\x9a\x44\x3c\x0e\xa4\xcc\x81\x98\x99\xf2\x08"
                                   "\x3a\x85\xf0\xfa\xa3\xe5\x78\xf8\x07\x7a\x2e\x3f\xf4\x67\x29"
                                   "\x66\x5b");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a"
                                                      "\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14"
                                                      "\x15\x16\x17\x18\x19"};
            h << str{init::fill, 50, '\xcd'};
            array<char, decltype(h)::output_size> digest;
            h.final(digest);
            snn_require(digest.view() == "\x82\x55\x8a\x38\x9a\x44\x3c\x0e\xa4\xcc\x81\x98\x99\xf2"
                                         "\x08\x3a\x85\xf0\xfa\xa3\xe5\x78\xf8\x07\x7a\x2e\x3f\xf4"
                                         "\x67\x29\x66\x5b");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{str{init::fill, 20, '\x0c'}};
            h << "Test With Truncation";
            str mac = h.final_hex();
            snn_require(mac.size() == 64);
            mac.truncate(32);
            snn_require(mac == "a3b6167473100ee06e0c796c2955552b");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{str{init::fill, 131, '\xaa'}};
            h << "Test Using Larger Than Block-Size Key - Hash Key First";
            str mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54");
        }

        {
            crypto::mac::hmac<crypto::hash::sha256> h{str{init::fill, 131, '\xaa'}};
            h << "This is a test using a larger than block-size key"
              << " and a larger than block-size data. "
              << "The key needs to be hashed before being used by the HMAC algorithm.";
            const auto mac = h.final_hex();
            snn_require(mac.size() == 64);
            snn_require(mac == "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2");
        }

        // Empty key (must work, but is of course not recommended). Verified with PHP and Go.
        {
            crypto::mac::hmac<crypto::hash::sha256> h{""};
            const auto mac = h.final_hex<strbuf>();
            snn_require(mac.size() == 64);
            snn_require(mac == "b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad");
        }
    }
}
