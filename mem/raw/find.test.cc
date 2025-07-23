// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/find.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/random/number.hh"
#include "snn-core/random/string.hh"
#include <string.h> // memchr, memmem

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Find a single byte (`memchr`).
            {
                const array<byte, 4> a{182, 0, 34, 182};
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{1}) == nullptr);
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{182}) == &a.get<0>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{0}) == &a.get<1>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), byte{34}) == &a.get<2>());
            }

            // Find a byte substring (`memmem`).
            {
                const array<byte, 4> a{182, 0, 34, 182};
                snn_require(mem::raw::find(a.data(), a.byte_size(), a.data(),
                                           not_zero{a.byte_size()}) == &a.get<0>());
            }
            {
                const array<byte, 4> haystack{182, 0, 34, 182};
                const array<byte, 2> needle{34, 182};
                snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), needle.data(),
                                           not_zero{needle.byte_size()}) == &haystack.get<2>());
            }
            {
                const array<byte, 4> haystack{182, 0, 34, 182};
                const array<byte, 3> needle{34, 182, 182};
                snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), needle.data(),
                                           not_zero{needle.byte_size()}) == nullptr);
            }

            return true;
        }

        constexpr bool test_find()
        {
            {
                array<i8, 4> a{-128, 0, 34, -128};

                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{1}) == nullptr);
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{-128}) == &a.get<0>());
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{0}) == &a.get<1>());
                snn_require(mem::raw::find(a.writable(), a.byte_size(), i8{34}) == &a.get<2>());

                array<i8, 2> n{-128, 0};
                snn_require(mem::raw::find(a.writable(), a.byte_size(), n.data(),
                                           not_zero{n.byte_size()}) == &a.get<0>());
            }
            {
                array<char, 5> a{'a', '\0', to_char(174), '\n', '\0'};

                snn_require(mem::raw::find(a.data(), a.byte_size(), ' ') == nullptr);
                snn_require(mem::raw::find(a.data(), a.byte_size(), 'a') == &a.get<0>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), '\0') == &a.get<1>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), to_char(0xAE)) == &a.get<2>());
                snn_require(mem::raw::find(a.data(), a.byte_size(), '\n') == &a.get<3>());

                array<char, 3> n{'\0', to_char(174), '\n'};
                snn_require(mem::raw::find(a.writable(), a.byte_size(), n.data(),
                                           not_zero{n.byte_size()}) == &a.get<1>());
            }

            return true;
        }

        bool test_find_rnd()
        {
            for (loop::count lc{10}; lc--;)
            {
                const auto haystack = random::string(random::number<usize>(0, 10'000));

                // Find a single byte (`memchr`).

                if (haystack)
                {
                    const char needle = haystack.back(assume::not_empty);

                    auto* p = static_cast<const char*>(::memchr(haystack.begin(), to_byte(needle),
                                                                haystack.size()));
                    snn_require(p != nullptr);
                    snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), needle) == p);
                    snn_require(mem::raw::detail::find_slow(haystack.data(), haystack.byte_size(),
                                                            needle) == p);
                }

                {
                    auto* p =
                        static_cast<const char*>(::memchr(haystack.begin(), '\n', haystack.size()));
                    snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), '\n') == p);
                    snn_require(mem::raw::detail::find_slow(haystack.data(), haystack.byte_size(),
                                                            '\n') == p);
                }
                {
                    auto* p =
                        static_cast<const char*>(::memchr(haystack.begin(), '\0', haystack.size()));
                    snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), '\0') == p);
                    snn_require(mem::raw::detail::find_slow(haystack.data(), haystack.byte_size(),
                                                            '\0') == p);
                }
                {
                    auto* p =
                        static_cast<const char*>(::memchr(haystack.begin(), 0xFF, haystack.size()));
                    snn_require(mem::raw::find(haystack.data(), haystack.byte_size(),
                                               to_char(0xFF)) == p);
                    snn_require(mem::raw::detail::find_slow(haystack.data(), haystack.byte_size(),
                                                            to_char(0xFF)) == p);
                }

                // Find a byte substring (`memmem`).

#if defined(__FreeBSD__) || defined(__linux__)
                if (haystack)
                {
                    const auto needle = haystack.view_offset(-20);
                    snn_require(needle);

                    auto* p = static_cast<const char*>(::memmem(haystack.begin(), haystack.size(),
                                                                needle.begin(), needle.size()));
                    snn_require(p != nullptr);
                    snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), needle.data(),
                                               not_zero{needle.byte_size()}) == p);
                }

                {
                    const auto needle = random::string(random::number<usize>(1, 3));
                    snn_require(needle);

                    auto* p = static_cast<const char*>(::memmem(haystack.begin(), haystack.size(),
                                                                needle.begin(), needle.size()));
                    snn_require(mem::raw::find(haystack.data(), haystack.byte_size(), needle.data(),
                                               not_zero{needle.byte_size()}) == p);
                }
#endif
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
        snn_static_require(app::test_find());
        snn_require(app::test_find_rnd());
    }
}
