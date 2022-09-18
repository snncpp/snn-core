// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # SHA-1

#pragma once

#if defined(SNN_BEARSSL_HASH)
#include "snn-core/crypto/hash/impl/sha1.bearssl.hh"
#else
#include "snn-core/crypto/hash/impl/sha1.openssl.hh"
#endif
