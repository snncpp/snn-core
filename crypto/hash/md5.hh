// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # MD5

#pragma once

#if defined(SNN_BEARSSL_HASH)
    #include "snn-core/crypto/hash/impl/md5.bearssl.hh"
#else
    #include "snn-core/crypto/hash/impl/md5.openssl.hh"
#endif
