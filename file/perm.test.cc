// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/perm.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_perm()
        {
            static_assert(strict_unsigned_integral<std::underlying_type_t<file::perm>>);

            auto reg_def = file::perm::owner_read | file::perm::owner_write |
                           file::perm::group_read | file::perm::group_write |
                           file::perm::other_read | file::perm::other_write;

            static_assert(std::is_same_v<decltype(reg_def), file::perm>);

            snn_require(reg_def == file::perm::regular_default);
            snn_require((reg_def & file::perm::group_read) == file::perm::group_read);
            snn_require((reg_def & file::perm::group_exec) == file::perm::none);

            snn_require(to_underlying(file::perm::regular_default) == 0666);
            snn_require(to_underlying(file::perm::regular_default & ~file::perm::other_write) ==
                        0664);

            // At most 12 bits are used for permissions.
            snn_require(to_underlying(file::perm::mask) == 0b111'111'111'111);

            snn_require(to_underlying(file::perm::all_read) == 0b000'100'100'100);
            snn_require(to_underlying(file::perm::all_write) == 0b000'010'010'010);
            snn_require(to_underlying(file::perm::all_exec) == 0b000'001'001'001);
            snn_require(to_underlying(file::perm::all_all) == 0b000'111'111'111);

            snn_require(to_underlying(file::perm::setuid) == 0b100'000'000'000);
            snn_require(to_underlying(file::perm::setgid) == 0b010'000'000'000);
            snn_require(to_underlying(file::perm::sticky) == 0b001'000'000'000);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_perm());
    }
}
