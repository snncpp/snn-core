// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/transient_entry.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_transient_entry()
        {
            static_assert(is_trivially_relocatable_v<file::dir::transient_entry>);
            static_assert(sizeof(file::dir::transient_entry) == 16);

            {
                file::dir::transient_entry ent{"hello.txt", file::type::regular};
                snn_require(ent.name<cstrview>() == "hello.txt");
                snn_require(ent.is_regular());
                snn_require(ent.type() == file::type::regular);
            }
            {
                file::dir::transient_entry ent{"home", file::type::directory};
                snn_require(ent.name<str>() == "home");
                snn_require(ent.is_directory());
                snn_require(ent.type() == file::type::directory);
            }
            {
                const array name{'A', 'b', 'c', '\0'};
                file::dir::transient_entry ent{null_term{name.data()}, file::type::regular};
                snn_require(ent.name<cstrview>().size() == 3);
                snn_require(ent.name<cstrview>() == "Abc");
                snn_require(ent.name<null_term<const char*>>().get() == name.begin());
            }
            {
                const array name{'A', 'b', 'c', '\0'};
                file::dir::transient_entry ent{null_term{name.data()}, 3, file::type::regular};
                snn_require(ent.name<cstrview>().size() == 3);
                snn_require(ent.name<cstrview>() == "Abc");
                snn_require(ent.name<null_term<const char*>>().get() == name.begin());
            }
            {
                const array name{'\0', 'A', 'b', 'c'};
                file::dir::transient_entry ent{null_term{name.data()}, file::type::regular};
                snn_require(ent.name<cstrview>().size() == 0);
                snn_require(ent.name<cstrview>() == "");
                snn_require(ent.name<null_term<const char*>>().get() == name.begin());
            }
            {
                const array name{'\0', 'A', 'b', 'c'};
                file::dir::transient_entry ent{null_term{name.data()}, 0, file::type::regular};
                snn_require(ent.name<cstrview>().size() == 0);
                snn_require(ent.name<cstrview>() == "");
                snn_require(ent.name<null_term<const char*>>().get() == name.begin());
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_transient_entry());
    }
}
