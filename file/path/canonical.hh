// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Canonical absolute path

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/system/error.hh"
#include <climits> // PATH_MAX
#include <cstdlib> // realpath

namespace snn::file::path
{
    // ## Functions

    // ### canonical

    template <any_strcore Str = str>
    [[nodiscard]] result<Str> canonical(const transient<null_term<const char*>> path)
    {
        // PATH_MAX is 1024 in FreeBSD and 4096 in Linux (POSIX min: 256).
        static_assert(PATH_MAX >= 1024);
        // "PATH_MAX - Maximum number of bytes the implementation will store as a pathname in a
        // user-supplied buffer of unspecified size, including the terminating null character."
        // https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/limits.h.html
        Str s;
        char* const buffer = s.resize_for_overwrite(PATH_MAX).begin();
        if (::realpath(path.get().get(), buffer) != nullptr)
        {
            s.truncate(s.find('\0').value_or_npos());
            return s;
        }
        return error_code{errno, system::error_category};
    }
}
