// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # List directory entries

#pragma once

#include "snn-core/defer.hh"
#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/vec.hh"
#include "snn-core/file/dir/entry.hh"
#include "snn-core/file/dir/transient_entry.hh"
#include "snn-core/system/error.hh"
#include <dirent.h> // closedir, opendir, readdir, DT_*

namespace snn::file::dir
{
    namespace detail
    {
        constexpr type to_type(const u8 t) noexcept
        {
            switch (t)
            {
                case DT_FIFO:
                    return type::fifo;
                case DT_CHR:
                    return type::character;
                case DT_DIR:
                    return type::directory;
                case DT_BLK:
                    return type::block;
                case DT_REG:
                    return type::regular;
                case DT_LNK:
                    return type::symlink;
                case DT_SOCK:
                    return type::socket;
#if defined(DT_WHT)
                case DT_WHT:
                    return type::whiteout;
#endif
                default:
                    return type::unknown;
            }
        }

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        constexpr bool skip_name(const char* name) noexcept
        {
            // Skip "." and "..".
            return name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
        }

        SNN_DIAGNOSTIC_POP
    }

    // ## Functions

    // ### list

    template <callable<transient_entry> Callback>
    [[nodiscard]] result<void> list(const transient<null_term<const char*>> path, Callback process)
    {
        DIR* dirp = ::opendir(path.get().get());
        if (dirp == nullptr)
        {
            return error_code{errno, system::error_category};
        }
        defer close_dirp{[dirp] { ::closedir(dirp); }};

        errno = 0;

        // readdir_r() is deprecated, readdir() is thread-safe as long as DIR* isn't shared.
        struct dirent* de;
        while ((de = ::readdir(dirp)) != nullptr)
        {
            if (!detail::skip_name(de->d_name))
            {
                const auto name = null_term{not_null<const char*>{de->d_name}};
                const auto type = detail::to_type(de->d_type);
#if defined(__FreeBSD__)
                process(transient_entry{name, de->d_namlen, type});
#else
                process(transient_entry{name, type});
#endif

                errno = 0; // In case process() changed it.
            }
        }

        if (errno != 0)
        {
            return error_code{errno, system::error_category};
        }

        return {};
    }

    template <has_append_inplace Container>
    [[nodiscard]] result<void> list(const transient<null_term<const char*>> path,
                                    Container& entries)
    {
        return list(path, [&entries](const transient_entry e) {
            entries.append_inplace(e.name<str>(), e.type());
        });
    }

    [[nodiscard]] inline result<vec<entry>> list(const transient<null_term<const char*>> path)
    {
        vec<entry> entries;
        const auto res = list(path, entries);
        if (res)
        {
            return entries;
        }
        return res.error_code();
    }
}
