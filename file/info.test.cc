// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/info.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        static_assert(file::detail::to_type(0) == file::type::unknown);
        static_assert(file::detail::to_type(99) == file::type::unknown);

        static_assert(file::detail::to_type(S_IFIFO) == file::type::fifo);
        static_assert(file::detail::to_type(S_IFCHR) == file::type::character);
        static_assert(file::detail::to_type(S_IFDIR) == file::type::directory);
        static_assert(file::detail::to_type(S_IFBLK) == file::type::block);
        static_assert(file::detail::to_type(S_IFREG) == file::type::regular);
        static_assert(file::detail::to_type(S_IFLNK) == file::type::symlink);
        static_assert(file::detail::to_type(S_IFSOCK) == file::type::socket);
#if defined(S_IFWHT)
        static_assert(file::detail::to_type(S_IFWHT) == file::type::whiteout);
#endif

        // Permission mask.
        static_assert(static_cast<u16>(file::perm::all_all | file::perm::setuid |
                                       file::perm::setgid | file::perm::sticky) == 07777);

        {
            constexpr file::info info;

            static_assert(!info.is_block());
            static_assert(!info.is_character());
            static_assert(!info.is_directory());
            static_assert(!info.is_fifo());
            static_assert(!info.is_regular());
            static_assert(!info.is_socket());
            static_assert(!info.is_symlink());
            static_assert(info.is_unknown());
            static_assert(!info.is_whiteout());

            static_assert(info.optimal_io_block_size() == 0);
            static_assert(info.permission() == file::perm::none);
            static_assert(info.size() == 0);
            static_assert(info.type() == file::type::unknown);
        }
    }
}
