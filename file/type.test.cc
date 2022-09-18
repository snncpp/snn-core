// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/type.hh"

#include "snn-core/unittest.hh"
#include <dirent.h>   // DT_*
#include <sys/stat.h> // S_IF*

namespace snn
{
    void unittest()
    {
#if defined(__FreeBSD__)
        static_assert(S_IFMT == 0b1111'0000'0000'0000); // "Type of file mask".
        static_assert(static_cast<int>(file::type::fifo) == S_IFIFO >> 12);
        static_assert(static_cast<int>(file::type::character) == S_IFCHR >> 12);
        static_assert(static_cast<int>(file::type::directory) == S_IFDIR >> 12);
        static_assert(static_cast<int>(file::type::block) == S_IFBLK >> 12);
        static_assert(static_cast<int>(file::type::regular) == S_IFREG >> 12);
        static_assert(static_cast<int>(file::type::symlink) == S_IFLNK >> 12);
        static_assert(static_cast<int>(file::type::socket) == S_IFSOCK >> 12);
        static_assert(static_cast<int>(file::type::whiteout) == S_IFWHT >> 12);

        static_assert(static_cast<int>(file::type::unknown) == DT_UNKNOWN);
        static_assert(static_cast<int>(file::type::fifo) == DT_FIFO);
        static_assert(static_cast<int>(file::type::character) == DT_CHR);
        static_assert(static_cast<int>(file::type::directory) == DT_DIR);
        static_assert(static_cast<int>(file::type::block) == DT_BLK);
        static_assert(static_cast<int>(file::type::regular) == DT_REG);
        static_assert(static_cast<int>(file::type::symlink) == DT_LNK);
        static_assert(static_cast<int>(file::type::socket) == DT_SOCK);
        static_assert(static_cast<int>(file::type::whiteout) == DT_WHT);
#endif
    }
}
