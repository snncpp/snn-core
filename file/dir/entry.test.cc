// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/entry.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_entry()
        {
            static_assert(is_trivially_relocatable_v<file::dir::entry>);

            {
                file::dir::entry ent{"hello.txt", file::type::regular};
                snn_require(ent.name() == "hello.txt");
                snn_require(ent.is_regular());
                snn_require(ent.type() == file::type::regular);

                // Structured binding.
                const auto [name, type] = ent;
                snn_require(name == "hello.txt");
                snn_require(type == file::type::regular);
            }
            {
                file::dir::entry ent{"home", file::type::directory};
                snn_require(ent.name() == "home");
                snn_require(ent.is_directory());
                snn_require(ent.type() == file::type::directory);
            }
            {
                file::dir::transient_entry tmp{"home", file::type::directory};
                file::dir::entry ent{tmp};
                snn_require(ent.name() == "home");
                snn_require(ent.is_directory());
                snn_require(ent.type() == file::type::directory);
            }

            {
                file::dir::entry a_reg{"a", file::type::regular};
                file::dir::entry b_reg{"b", file::type::regular};
                file::dir::entry c_reg{"c", file::type::regular};
                file::dir::entry aa_reg{"aa", file::type::regular};
                file::dir::entry empty_reg{"", file::type::regular};
                file::dir::entry a_unknown{"a", file::type::unknown};

                snn_require(a_reg == a_reg);
                snn_require(!(a_reg == b_reg));

                snn_require(a_reg != b_reg);
                snn_require(a_reg != a_unknown);
                snn_require(!(a_reg != a_reg));

                snn_require(!(a_reg < a_reg));
                snn_require(a_reg < aa_reg);
                snn_require(a_reg < b_reg);
                snn_require(empty_reg < a_reg);

                snn_require(c_reg > b_reg);

                // This would fail if the name and type members were reordered.
                snn_require(empty_reg < a_unknown);

                static_assert(noexcept(a_reg <=> b_reg));
                static_assert(std::is_same_v<decltype(a_reg <=> b_reg), std::strong_ordering>);
            }

            return true;
        }

    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_entry());
    }
}
