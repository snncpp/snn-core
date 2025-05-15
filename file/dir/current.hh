// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get working directory pathname

#pragma once

#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/env/get.hh"
#include "snn-core/file/is_same.hh"
#include "snn-core/file/path/looks_canonical.hh"
#include "snn-core/system/error.hh"
#include <climits>     // PATH_MAX
#include <sys/param.h> // [MAXPATHLEN]
#include <unistd.h>    // getcwd

namespace snn::file::dir
{
    // ## Functions

    // ### current

    // Get absolute current working directory pathname (using `getcwd`). All symbolic links are
    // resolved.

    template <any_strcore Str = str>
    [[nodiscard]] result<Str> current()
    {
#if defined(__FreeBSD__)
        // FreeBSD uses `MAXPATHLEN` with `getcwd`, but it is defined to `PATH_MAX`.
        static_assert(MAXPATHLEN == PATH_MAX);
#endif
        Str path{container::size_for_overwrite, PATH_MAX};

        if (::getcwd(path.begin(), path.size()) == nullptr)
        {
            return error_code{errno, system::error_category};
        }

        path.truncate(path.find('\0').value_or_npos());

        return path;
    }

    // ### current_logical

    // Get absolute logical current working directory pathname (using `PWD` environment variable if
    // available and correct, with `getcwd` as fallback).

    template <any_strcore Str = str>
    [[nodiscard]] result<Str> current_logical()
    {
        if (auto pwd_maybe = env::get<null_term<const char*>>("PWD"))
        {
            const auto pwd  = pwd_maybe.value(promise::has_value);
            const auto path = pwd.to<cstrview>();
            if (file::path::looks_canonical(path))
            {
                if (file::is_same(pwd, "."))
                {
                    return Str{path};
                }
            }
        }

        // Fallback
        return current<Str>();
    }
}
