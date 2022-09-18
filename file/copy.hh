// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Copy a file

// The destination file (if it exists) will be overwritten by default.
// Both source and destination (if it exists) must be regular files.
// If the destination file is created and an error occurs, it is not removed.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/file/is_same.hh"
#include "snn-core/file/reader_writer.hh"
#include "snn-core/math/common.hh"
#include "snn-core/system/error.hh"
#include <cerrno> // E*

namespace snn::file
{
    template <typename Io = io::blocking>
    [[nodiscard]] result<void> copy(const transient<null_term<const char*>> source,
                                    const transient<null_term<const char*>> destination,
                                    const option options    = option::create_or_truncate,
                                    const perm permissions  = perm::regular_default,
                                    const usize buffer_size = 128 * constant::size::kibibyte<usize>,
                                    Io io                   = Io{})
    {
        constexpr usize buf_min_size = 4 * constant::size::kibibyte<usize>;
        constexpr usize buf_max_size = 1 * constant::size::mebibyte<usize>;
        static_assert(buf_min_size < buf_max_size);

        // Source

        file::reader_writer<Io> src{io};
        if (const auto r = src.open_for_reading(source); !r)
        {
            return r.error_code();
        }

        file::info src_info{container::do_not_initialize};
        if (const auto r = src.status(src_info); !r)
        {
            // The only reasonable error here is `EIO`.
            return r.error_code();
        }

        if (!src_info.is_regular())
        {
            return error_code{EINVAL, system::error_category};
        }

        // Destination (don't truncate yet).

        file::reader_writer<Io> dst{io};
        const option opts_no_truncate = options & ~option::truncate;
        if (const auto r = dst.open_for_writing(destination, opts_no_truncate, permissions); !r)
        {
            return r.error_code();
        }

        file::info dst_info{container::do_not_initialize};
        if (const auto r = dst.status(dst_info); !r)
        {
            // The only reasonable error here is `EIO`.
            return r.error_code();
        }

        if (!dst_info.is_regular())
        {
            return error_code{EINVAL, system::error_category};
        }

        // Same?

        if (is_same(src_info, dst_info))
        {
            if ((options & option::append) == option::none)
            {
                // The file has not been modified in any way, so it should be safe to return early
                // and ignore the return value from `close()`.
                return {};
            }
            else
            {
                // Appending a file to itself is not supported.
                return error_code{EINVAL, system::error_category};
            }
        }

        // Truncate destination?

        if ((options & option::truncate) == option::truncate)
        {
            // This should never fail.
            if (const auto r = dst.truncate(); !r)
            {
                return r.error_code();
            }

            // The file was just opened so the file offset is zero. (If the file was opened with
            // `option::append`, writing takes place at the new file size, which is zero.)
        }

        // Copy contents if not empty.

        const usize src_size = src_info.size();
        if (src_size > 0)
        {
            // TODO
            // Use `copy_file_range()`?

            // Buffer

            const usize buf_size =
                math::min(src_size, math::clamp(buffer_size, buf_min_size, buf_max_size));
            snn_should(buf_size > 0);

            strbuf buf;
            strview buffer = buf.resize_uninitialized(buf_size);

            // Read & write loop.

            while (true)
            {
                const auto res = src.read_some(buffer);
                if (res)
                {
                    const usize read_size = res.value(promise::has_value);
                    if (read_size > 0)
                    {
                        if (const auto r = dst.write_all(buffer.view(0, read_size)); !r)
                        {
                            return r.error_code();
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    return res.error_code();
                }
            }
        }

        // Close (explicitly to catch any delayed errors).

        if (const auto r = src.close(); !r)
        {
            return r.error_code();
        }

        return dst.close();
    }
}
