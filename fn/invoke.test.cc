// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fn/invoke.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                fn::invoke clear_str{&str::clear};

                str s = "abc";
                snn_require(s == "abc");
                clear_str(s);
                snn_require(s == "");
            }
            {
                struct person
                {
                    int id{};
                    cstrview name;
                    cstrview title;
                };

                constexpr person a{123, "foo", "abc"};
                constexpr person b{456, "bar", "def"};

                static_assert(fn::invoke{fn::less_than{}, &person::id}(a, b));
                static_assert(!fn::invoke{fn::less_than{}, &person::id}(b, a));

                static_assert(!fn::invoke{fn::less_than{}, &person::name}(a, b));
                static_assert(fn::invoke{fn::less_than{}, &person::name}(b, a));

                static_assert(fn::invoke{fn::less_than{}, &person::title, &person::name}(a, b));
                static_assert(!fn::invoke{fn::less_than{}, &person::name, &person::title}(a, b));
            }

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
