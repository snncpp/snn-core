// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/permission/mask.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool test_mask()
        {
            constexpr auto default_mask = file::perm::group_write | file::perm::other_write;
            static_assert(static_cast<mode_t>(default_mask) == 022);

            const auto original_mask = file::permission::mask(default_mask);

            auto old_mask = file::permission::mask(file::perm::none);
            snn_require(old_mask == default_mask);

            old_mask = file::permission::mask(original_mask);
            snn_require(old_mask == file::perm::none);

            return true;
        }

    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::test_mask());
    }
}
