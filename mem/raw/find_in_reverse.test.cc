// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/find_in_reverse.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/random/number.hh"
#include "snn-core/random/string.hh"
#include <string.h> // memrchr

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            const array<byte, 4> a{182, 0, 7, 0};
            snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), byte{1}) == nullptr);
            snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), byte{0}) == &a.get<3>());
            snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), byte{7}) == &a.get<2>());

            return true;
        }

        constexpr bool test_find_in_reverse()
        {
            {
                array<i8, 4> a{-128, 0, 34, -128};
                snn_require(mem::raw::find_in_reverse(a.writable(), a.byte_size(), i8{1}) ==
                            nullptr);
                snn_require(mem::raw::find_in_reverse(a.writable(), a.byte_size(), i8{-128}) ==
                            &a.get<3>());
                snn_require(mem::raw::find_in_reverse(a.writable(), a.byte_size(), i8{0}) ==
                            &a.get<1>());
                snn_require(mem::raw::find_in_reverse(a.writable(), a.byte_size(), i8{34}) ==
                            &a.get<2>());
            }
            {
                array<char, 5> a{'a', '\0', to_char(174), '\n', '\0'};
                snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), ' ') == //
                            nullptr);
                snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), 'a') == //
                            &a.get<0>());
                snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), '\0') ==
                            &a.get<4>());
                snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), to_char(0xAE)) ==
                            &a.get<2>());
                snn_require(mem::raw::find_in_reverse(a.data(), a.byte_size(), '\n') ==
                            &a.get<3>());
            }

            return true;
        }

        bool test_find_in_reverse_rnd()
        {
#if defined(__FreeBSD__) || defined(__linux__)
            for (loop::count lc{100}; lc--;)
            {
                const auto haystack = random::string(random::number<usize>(0, 50));

                if (haystack)
                {
                    const char needle = haystack.front(assume::not_empty);

                    auto* p = static_cast<const char*>(::memrchr(haystack.begin(), to_byte(needle),
                                                                 haystack.size()));
                    snn_require(p != nullptr);
                    snn_require(mem::raw::find_in_reverse(haystack.data(), haystack.byte_size(),
                                                          needle) == p);
                    snn_require(mem::raw::detail::find_in_reverse(
                                    haystack.data(), haystack.byte_size(), needle) == p);
                }

                {
                    auto* p = static_cast<const char*>(::memrchr(haystack.begin(), '\n',
                                                                 haystack.size()));
                    snn_require(mem::raw::find_in_reverse(haystack.data(), haystack.byte_size(),
                                                          '\n') == p);
                    snn_require(mem::raw::detail::find_in_reverse(haystack.data(),
                                                                  haystack.byte_size(), '\n') == p);
                }
                {
                    auto* p = static_cast<const char*>(::memrchr(haystack.begin(), '\0',
                                                                 haystack.size()));
                    snn_require(mem::raw::find_in_reverse(haystack.data(), haystack.byte_size(),
                                                          '\0') == p);
                    snn_require(mem::raw::detail::find_in_reverse(haystack.data(),
                                                                  haystack.byte_size(), '\0') == p);
                }
                {
                    auto* p = static_cast<const char*>(::memrchr(haystack.begin(), 0xFF,
                                                                 haystack.size()));
                    snn_require(mem::raw::find_in_reverse(haystack.data(), haystack.byte_size(),
                                                          to_char(0xFF)) == p);
                    snn_require(mem::raw::detail::find_in_reverse(
                                    haystack.data(), haystack.byte_size(), to_char(0xFF)) == p);
                }
            }
#endif

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_find_in_reverse());
        snn_require(app::test_find_in_reverse_rnd());
    }
}
