// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/random/element.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            str s{"abc"};
            do
            {
                char& c = random::element(s).value();
                c       = '.';
            } while (s != "...");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        {
            str s;
            snn_require(s.is_empty());
            auto opt = random::element(s);
            snn_require(!opt);
            snn_require(!opt.has_value());
        }

        {
            str s{"a"};
            decltype(auto) opt = random::element(s);
            static_assert(std::is_same_v<decltype(opt), optional<char&>>);
            snn_require(opt.value() == 'a');
        }

        {
            const str s{"a"};
            decltype(auto) opt = random::element(s);
            static_assert(std::is_same_v<decltype(opt), optional<const char&>>);
            snn_require(opt.value() == 'a');
        }
    }
}
