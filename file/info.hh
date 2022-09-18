// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # File information

#pragma once

#include "snn-core/file/perm.hh"
#include "snn-core/file/type.hh"
#include "snn-core/math/common.hh"
#include <sys/stat.h> // mode_t, stat, S_IF*

static_assert(std::is_unsigned_v<mode_t>);

namespace snn::file
{
    namespace detail
    {
        constexpr type to_type(const mode_t mode) noexcept
        {
            switch (mode & S_IFMT)
            {
                case S_IFIFO:
                    return type::fifo;
                case S_IFCHR:
                    return type::character;
                case S_IFDIR:
                    return type::directory;
                case S_IFBLK:
                    return type::block;
                case S_IFREG:
                    return type::regular;
                case S_IFLNK:
                    return type::symlink;
                case S_IFSOCK:
                    return type::socket;
#if defined(S_IFWHT)
                case S_IFWHT:
                    return type::whiteout;
#endif
                default:
                    return type::unknown;
            }
        }
    }

    // ## Classes

    // ### info

    class info final
    {
      public:
        constexpr explicit info() noexcept
            : stat_{}
        {
        }

        explicit info(container::do_not_initialize_t) noexcept
        {
        }

        [[nodiscard]] constexpr bool is_block() const noexcept
        {
            return type() == type::block;
        }

        [[nodiscard]] constexpr bool is_character() const noexcept
        {
            return type() == type::character;
        }

        [[nodiscard]] constexpr bool is_directory() const noexcept
        {
            return type() == type::directory;
        }

        [[nodiscard]] constexpr bool is_fifo() const noexcept
        {
            return type() == type::fifo;
        }

        [[nodiscard]] constexpr bool is_regular() const noexcept
        {
            return type() == type::regular;
        }

        [[nodiscard]] constexpr bool is_socket() const noexcept
        {
            return type() == type::socket;
        }

        [[nodiscard]] constexpr bool is_symlink() const noexcept
        {
            return type() == type::symlink;
        }

        [[nodiscard]] constexpr bool is_unknown() const noexcept
        {
            return type() == type::unknown;
        }

        [[nodiscard]] constexpr bool is_whiteout() const noexcept
        {
            return type() == type::whiteout;
        }

        [[nodiscard]] constexpr usize optimal_io_block_size() const noexcept
        {
            return math::within_bounds_or<usize>(stat_.st_blksize, 0);
        }

        // Permission bits.
        [[nodiscard]] constexpr perm permission() const noexcept
        {
            return static_cast<perm>(stat_.st_mode & 07777);
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return math::within_bounds_or<usize>(stat_.st_size, 0);
        }

        [[nodiscard]] constexpr file::type type() const noexcept
        {
            return detail::to_type(stat_.st_mode);
        }

        [[nodiscard]] constexpr struct ::stat& internal() noexcept
        {
            return stat_;
        }

        [[nodiscard]] constexpr const struct ::stat& internal() const noexcept
        {
            return stat_;
        }

      private:
        struct ::stat stat_;
    };
}
