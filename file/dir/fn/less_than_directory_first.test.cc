// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/fn/less_than_directory_first.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/sort.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array entries{
                file::dir::entry{"def", file::type::directory},
                file::dir::entry{"ghi", file::type::regular},
                file::dir::entry{"abc", file::type::regular},
                file::dir::entry{"jkl", file::type::directory},
            };

            entries.sort();

            snn_require(entries.get<0>() == file::dir::entry{"abc", file::type::regular});
            snn_require(entries.get<1>() == file::dir::entry{"def", file::type::directory});
            snn_require(entries.get<2>() == file::dir::entry{"ghi", file::type::regular});
            snn_require(entries.get<3>() == file::dir::entry{"jkl", file::type::directory});

            entries.sort(file::dir::fn::less_than_directory_first{});

            snn_require(entries.get<0>() == file::dir::entry{"def", file::type::directory});
            snn_require(entries.get<1>() == file::dir::entry{"jkl", file::type::directory});
            snn_require(entries.get<2>() == file::dir::entry{"abc", file::type::regular});
            snn_require(entries.get<3>() == file::dir::entry{"ghi", file::type::regular});

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
