// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/find.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/random/number.hh"
#include "snn-core/random/string.hh"
#include <cstring> // memchr

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                const array<byte, 4> a{182, 0, 34, 182};
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{1}) == nullptr);
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{182}) == &a.get<0>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{0}) == &a.get<1>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{34}) == &a.get<2>());
            }
            {
                array<i8, 4> a{-128, 0, 34, -128};
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{1}) == nullptr);
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{-128}) == &a.get<0>());
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{0}) == &a.get<1>());
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{34}) == &a.get<2>());
            }
            {
                array<char, 5> a{'a', '\0', to_char(174), '\n', '\0'};
                snn_require(mem::raw::find(a.data(), a.byte_size(), ' ') == nullptr);
                snn_require(mem::raw::find(a.data(), a.byte_size(), 'a') == &a.get<0>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), '\0') == &a.get<1>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), to_char(0xAE)) == &a.get<2>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), '\n') == &a.get<3>());
            }

            return true;
        }

        bool test_find()
        {
            for (loop::count lc{100}; lc--;)
            {
                const auto haystack = random::string(random::number<usize>(0, 50));

                if (haystack)
                {
                    const char needle = haystack.back(assume::not_empty);
                    const char* const p =
                        mem::raw::find(haystack.data(), haystack.byte_size(), needle);
                    snn_require(p != nullptr);
                    snn_require(p == static_cast<const char*>(std::memchr(
                                         haystack.begin(), to_byte(needle), haystack.size())));
                }

                snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), '\n') ==
                            static_cast<const char*>(std::memchr(haystack.begin(), '\n',
                                                                 haystack.size())));
                snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), '\0') ==
                            static_cast<const char*>(std::memchr(haystack.begin(), '\0',
                                                                 haystack.size())));
                snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), to_char(0xFF)) ==
                            static_cast<const char*>(std::memchr(haystack.begin(), 0xFF,
                                                                 haystack.size())));
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
        snn_require(app::test_find());
    }
}
