// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/fn/print.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        fmt::fn::print print{"{}, {}, {}\n"};
        print('a', 123, "abc");
        print(1, 2, 3);

        fmt::fn::print_error print_error{"{}: {}\n"};
        print_error('a', 'b');
        print_error(7, 8);
    }
}
