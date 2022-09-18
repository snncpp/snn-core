// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/cmp/common.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/less_than_icase.hh"
#include <memory> // addressof

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            int a = 123;
            int b = 456;

            snn_require(cmp::max(a, b) == 456);
            snn_require(cmp::max(b, a) == 456);
            snn_require(std::addressof(cmp::max(a, b)) == std::addressof(b));
            snn_require(std::addressof(cmp::max(b, a)) == std::addressof(b));

            snn_require(cmp::min(a, b) == 123);
            snn_require(cmp::min(b, a) == 123);
            snn_require(std::addressof(cmp::min(a, b)) == std::addressof(a));
            snn_require(std::addressof(cmp::min(b, a)) == std::addressof(a));

            return true;
        }

        constexpr bool test_max()
        {
            // decltype(auto) max(A&& a, B&& b)
            {
                str a            = "abc";
                const str b      = "def";
                decltype(auto) s = cmp::max(a, b);
                static_assert(std::is_same_v<decltype(s), const str&>);
                snn_require(s == "def");
                snn_require(std::addressof(s) == std::addressof(b));
            }
            {
                str a            = "abc";
                str b            = "def";
                decltype(auto) s = cmp::max(b, a);
                static_assert(std::is_same_v<decltype(s), str&>);
                snn_require(s == "def");
                snn_require(std::addressof(s) == std::addressof(b));
            }

            // decltype(auto) max(A&& a, B&& b, TwoArgPred is_less)
            {
                cstrview a{"abc"};
                cstrview b{"DEF"};

                snn_require(!(a < b));
                snn_require(b < a);

                snn_require(cmp::max(a, b, ascii::fn::less_than_icase{}) == "DEF");
                snn_require(cmp::max(b, a, ascii::fn::less_than_icase{}) == "DEF");

                snn_require(std::addressof(cmp::max(a, b, ascii::fn::less_than_icase{})) ==
                            std::addressof(b));
                snn_require(std::addressof(cmp::max(b, a, ascii::fn::less_than_icase{})) ==
                            std::addressof(b));
            }
            {
                str a            = "abc";
                const str b      = "DEF";
                decltype(auto) s = cmp::max(a, b, ascii::fn::less_than_icase{});
                static_assert(std::is_same_v<decltype(s), const str&>);
                snn_require(s == "DEF");
                snn_require(std::addressof(s) == std::addressof(b));
            }
            {
                str a            = "abc";
                str b            = "DEF";
                decltype(auto) s = cmp::max(b, a, ascii::fn::less_than_icase{});
                static_assert(std::is_same_v<decltype(s), str&>);
                snn_require(s == "DEF");
                snn_require(std::addressof(s) == std::addressof(b));
            }

            return true;
        }

        constexpr bool test_min()
        {
            // decltype(auto) min(A&& a, B&& b)
            {
                str a            = "abc";
                const str b      = "def";
                decltype(auto) s = cmp::min(a, b);
                static_assert(std::is_same_v<decltype(s), const str&>);
                snn_require(s == "abc");
                snn_require(std::addressof(s) == std::addressof(a));
            }
            {
                str a            = "abc";
                str b            = "def";
                decltype(auto) s = cmp::min(b, a);
                static_assert(std::is_same_v<decltype(s), str&>);
                snn_require(s == "abc");
                snn_require(std::addressof(s) == std::addressof(a));
            }

            // decltype(auto) min(A&& a, B&& b, TwoArgPred is_less)
            {
                cstrview a{"abc"};
                cstrview b{"DEF"};

                snn_require(!(a < b));
                snn_require(b < a);

                snn_require(cmp::min(a, b, ascii::fn::less_than_icase{}) == "abc");
                snn_require(cmp::min(b, a, ascii::fn::less_than_icase{}) == "abc");

                snn_require(std::addressof(cmp::min(a, b, ascii::fn::less_than_icase{})) ==
                            std::addressof(a));
                snn_require(std::addressof(cmp::min(b, a, ascii::fn::less_than_icase{})) ==
                            std::addressof(a));
            }
            {
                str a            = "abc";
                const str b      = "DEF";
                decltype(auto) s = cmp::min(a, b, ascii::fn::less_than_icase{});
                static_assert(std::is_same_v<decltype(s), const str&>);
                snn_require(s == "abc");
                snn_require(std::addressof(s) == std::addressof(a));
            }
            {
                str a            = "abc";
                str b            = "DEF";
                decltype(auto) s = cmp::min(b, a, ascii::fn::less_than_icase{});
                static_assert(std::is_same_v<decltype(s), str&>);
                snn_require(s == "abc");
                snn_require(std::addressof(s) == std::addressof(a));
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
        snn_static_require(app::test_max());
        snn_static_require(app::test_min());
    }
}
