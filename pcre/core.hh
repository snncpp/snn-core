// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Core functionality

#pragma once

#include "snn-core/core.hh"
#if !defined(PCRE2_CODE_UNIT_WIDTH)
    #define PCRE2_CODE_UNIT_WIDTH 0
#endif
#include <pcre2.h> // pcre2* [#lib:pcre2-8]
