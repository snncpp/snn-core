// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Seek in file (reposition read/write file offset)

#pragma once

#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <cerrno>   // errno, EINVAL
#include <unistd.h> // lseek, SEEK_*

static_assert(std::is_same_v<off_t, int64_t>); // Signed POSIX type.

namespace snn::file
{
    // ## Enums

    // ### whence

    enum class whence : int
    {
        set     = SEEK_SET,
        current = SEEK_CUR,
        end     = SEEK_END,
#if defined(SEEK_DATA)
        data = SEEK_DATA,
#endif
#if defined(SEEK_HOLE)
        hole = SEEK_HOLE,
#endif
    };

    // ## Functions

    // ### seek

    // With signed offset.

    template <strict_signed_integral SInt>
        requires(sizeof(SInt) <= sizeof(isize))
    [[nodiscard]] result<usize> seek(const int fd, const SInt offset,
                                     const whence w = whence::set) noexcept
    {
        const isize offs = ::lseek(fd, offset, static_cast<int>(w));
        if (offs >= 0)
        {
            return to_usize(offs);
        }
        return error_code{errno, system::error_category};
    }

    // With unsigned offset.

    template <strict_unsigned_integral UInt>
        requires(sizeof(UInt) <= sizeof(isize))
    [[nodiscard]] result<usize> seek(const int fd, const UInt offset,
                                     const whence w = whence::set) noexcept
    {
        if (offset < constant::limit<isize>::max)
        {
            const isize offs = ::lseek(fd, to_isize(offset), static_cast<int>(w));
            if (offs >= 0)
            {
                return to_usize(offs);
            }
            return error_code{errno, system::error_category};
        }
        return error_code{EINVAL, system::error_category};
    }
}
