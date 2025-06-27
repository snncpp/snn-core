// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Create temporary directory

// The directory will only be accessible by the owner (mode 0700).

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/strcore.hh"
#include "snn-core/result.hh"
#include "snn-core/env/get.hh"
#include "snn-core/file/error.hh"
#include "snn-core/random/number.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/system/error.hh"
#include <cerrno>     // E*
#include <sys/stat.h> // mkdir

namespace snn::file::dir
{
    // ## Functions

    // ### is_valid_temporary_prefix

    // Prefix must match: [a-z][a-z0-9_\-]{0,63}

    [[nodiscard]] constexpr bool is_valid_temporary_prefix(
        const transient<cstrview> prefix) noexcept
    {
        // Arbitrary size limit (a single component of the path must not exceed 255 characters).
        const cstrview s = prefix.get();
        if (s.size() <= 64)
        {
            auto rng = s.range();
            if (rng.drop_front_if(chr::is_alpha_lower))
            {
                rng.pop_front_while([](const char c) {
                    return chr::is_alphanumeric_lower(c) || c == '_' || c == '-';
                });

                return rng.is_empty();
            }
        }
        return false;
    }

    // ### create_temporary

    namespace detail
    {
        inline result<str> create_temporary(const cstrview prefix, const cstrview initial_path)
        {
            if (!is_valid_temporary_prefix(prefix))
            {
                return error::invalid_temporary_directory_prefix;
            }

            if (initial_path.is_empty())
            {
                return error::invalid_temporary_directory_path;
            }

            str path = initial_path;

            if (!path.has_back('/'))
            {
                path.append('/');
            }

            path.append(prefix);

            const usize path_size_before_random_suffix = path.size();

            u32 failed_count = 0;
            do
            {
                const auto n               = random::number<u32>();
                constexpr usize min_digits = sizeof(n) * 2;
                path.append_integral<math::base::hex>(n, min_digits);

                if (::mkdir(path.null_terminated().get(), 0700) == 0)
                {
                    return path;
                }
                else if (errno != EEXIST)
                {
                    // Note: `mkdir()` never returns `EINTR`.
                    return error_code{errno, system::error_category};
                }

                path.truncate(path_size_before_random_suffix);

                ++failed_count;
            } while (failed_count < 100); // Arbitrary limit.

            // This should never happen, u32 has over 4 billion unique values.
            return error::failed_to_create_unique_temporary_directory;
        }
    }

    // Create a temporary directory under `path`.

    [[nodiscard]] inline result<str> create_temporary(const transient<cstrview> prefix,
                                                      const transient<null_term<const char*>> path)
    {
        return detail::create_temporary(prefix.get(), path.get().to<cstrview>());
    }

    // Create a temporary directory under `TMPDIR` environment variable (if set) or "/tmp".

    // Note: The `TMPDIR` environment variable (if set) is used as is, it is not validated or
    // sanitized.

    [[nodiscard]] inline result<str> create_temporary(const transient<cstrview> prefix)
    {
        cstrview path;

        // https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html
        // TMPDIR "This variable shall represent a pathname of a directory made available for
        //         programs that need a place to create temporary files."
        const optional<cstrview> tmpdir = env::get<cstrview>("TMPDIR");
        if (tmpdir)
        {
            path = tmpdir.value(assume::has_value);
        }

        if (path.is_empty())
        {
            path = "/tmp";
        }

        return detail::create_temporary(prefix.get(), path);
    }
}
