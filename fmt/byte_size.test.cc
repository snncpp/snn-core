// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/byte_size.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fmt/format.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // SI units by default (powers of 1000).
            snn_require(fmt::byte_size(byte_size{0}) == "0 B");
            snn_require(fmt::byte_size(byte_size{123}) == "123 B");
            snn_require(fmt::byte_size(byte_size{1000}) == "1.0 kB");
            snn_require(fmt::byte_size(byte_size{1024}) == "1.0 kB");
            snn_require(fmt::byte_size(byte_size{999999}) == "1.0 MB");
            snn_require(fmt::byte_size(byte_size{1048576}) == "1.0 MB");
            snn_require(fmt::byte_size(byte_size{1073741824}) == "1.1 GB");
            snn_require(fmt::byte_size(byte_size{1099511627776}) == "1.1 TB");
            snn_require(fmt::byte_size(byte_size{1125899906842624}) == "1.1 PB");
            snn_require(fmt::byte_size(byte_size{1152921504606846976}) == "1.2 EB");
            snn_require(fmt::byte_size(byte_size{constant::limit<usize>::max}) == "18 EB");

            return true;
        }

        bool test_format_byte_size()
        {
            // Default (empty formatting string).
            snn_require(fmt::format("{}", byte_size{0}) == "0 B");
            snn_require(fmt::format("{}", byte_size{999'999}) == "1.0 MB");
            snn_require(fmt::format("{}", byte_size{5'555'555'555}) == "5.6 GB");

            // Unit (no spacing).
            snn_require(fmt::format("{:k}", byte_size{999'999}) == "1.0M");           // SI-short
            snn_require(fmt::format("{:k}", byte_size{5'555'555'555}) == "5.6G");     // SI-short
            snn_require(fmt::format("{:kB}", byte_size{999'999}) == "1.0MB");         // SI
            snn_require(fmt::format("{:kB}", byte_size{5'555'555'555}) == "5.6GB");   // SI
            snn_require(fmt::format("{:K}", byte_size{999'999}) == "977K");           // IEC-short
            snn_require(fmt::format("{:K}", byte_size{5'555'555'555}) == "5.2G");     // IEC-short
            snn_require(fmt::format("{:KiB}", byte_size{999'999}) == "977KiB");       // IEC
            snn_require(fmt::format("{:KiB}", byte_size{5'555'555'555}) == "5.2GiB"); // IEC

            // Only "k", "kB", "K" and "KiB" are recognized as units.
            // Here "MB" is parsed as spacing.
            snn_require(fmt::format("{:MB}", byte_size{5'555'555'555}) == "5.6MBGB");

            // Spacing (default unit)
            snn_require(fmt::format("{: }", byte_size{999'999}) == "1.0 MB");
            snn_require(fmt::format("{:  }", byte_size{999'999}) == "1.0  MB");
            snn_require(fmt::format("{:&nbsp;}", byte_size{999'999}) == "1.0&nbsp;MB");

            // Spacing + unit.
            snn_require(fmt::format("{: k}", byte_size{999'999}) == "1.0 M");
            snn_require(fmt::format("{: kB}", byte_size{999'999}) == "1.0 MB");
            snn_require(fmt::format("{: K}", byte_size{999'999}) == "977 K");
            snn_require(fmt::format("{: KiB}", byte_size{999'999}) == "977 KiB");
            snn_require(fmt::format("{:&nbsp;k}", byte_size{999'999}) == "1.0&nbsp;M");
            snn_require(fmt::format("{:&nbsp;kB}", byte_size{999'999}) == "1.0&nbsp;MB");
            snn_require(fmt::format("{:&nbsp;K}", byte_size{999'999}) == "977&nbsp;K");
            snn_require(fmt::format("{:&nbsp;KiB}", byte_size{999'999}) == "977&nbsp;KiB");

            // Decimal point (no spacing, default unit).
            snn_require(fmt::format("{:,0}", byte_size{999'999}) == "1,0MB");
            snn_require(fmt::format("{:，0}", byte_size{999'999}) == "1，0MB"); // U+FF0C
            snn_require(fmt::format("{:,0}", byte_size{5'555'555'555}) == "5,6GB");
            snn_require(fmt::format("{:，0}", byte_size{5'555'555'555}) == "5，6GB"); // U+FF0C

            // Only '0' can be used as a separator between decimal point and spacing.
            snn_require_throws_code(fmt::format("{:,9}", byte_size{0}),
                                    fmt::error::unexpected_character_in_format_string);

            // '0' separator but empty decimal point.
            snn_require_throws_code(fmt::format("{:0}", byte_size{0}),
                                    fmt::error::unexpected_character_in_format_string);

            // Decimal point + spacing (default unit).
            snn_require(fmt::format("{:,0 }", byte_size{999'999}) == "1,0 MB");
            snn_require(fmt::format("{:，0 }", byte_size{999'999}) == "1，0 MB"); // U+FF0C

            // Decimal point + unit (no spacing).
            snn_require(fmt::format("{:,0k}", byte_size{999'999}) == "1,0M");
            snn_require(fmt::format("{:,0K}", byte_size{999'999}) == "977K");

            // Decimal point + spacing + unit.
            snn_require(fmt::format("{:,0 k}", byte_size{999'999}) == "1,0 M");
            snn_require(fmt::format("{:,0 kB}", byte_size{999'999}) == "1,0 MB");
            snn_require(fmt::format("{:,0 K}", byte_size{999'999}) == "977 K");
            snn_require(fmt::format("{:,0 KiB}", byte_size{999'999}) == "977 KiB");
            snn_require(fmt::format("{:,0&nbsp;k}", byte_size{999'999}) == "1,0&nbsp;M");
            snn_require(fmt::format("{:,0&nbsp;kB}", byte_size{999'999}) == "1,0&nbsp;MB");
            snn_require(fmt::format("{:,0&nbsp;K}", byte_size{999'999}) == "977&nbsp;K");
            snn_require(fmt::format("{:,0&nbsp;KiB}", byte_size{999'999}) == "977&nbsp;KiB");

            return true;
        }

        constexpr bool test_byte_size_generated()
        {
            // Generated by: detail/byte_size.test.gen.cc

            // 1000 B,kB,MB,GB...
            // 1000 B,k,M,G...

            using fmt::table::byte_size::si;
            using fmt::table::byte_size::si_short;

            snn_require(fmt::byte_size<1000>(byte_size{0}, si) == "0 B");
            snn_require(fmt::byte_size<1000>(byte_size{0}, si_short) == "0 B");
            snn_require(fmt::byte_size<1000>(byte_size{1}, si) == "1 B");
            snn_require(fmt::byte_size<1000>(byte_size{1}, si_short) == "1 B");
            snn_require(fmt::byte_size<1000>(byte_size{10}, si) == "10 B");
            snn_require(fmt::byte_size<1000>(byte_size{10}, si_short) == "10 B");
            snn_require(fmt::byte_size<1000>(byte_size{100}, si) == "100 B");
            snn_require(fmt::byte_size<1000>(byte_size{100}, si_short) == "100 B");
            snn_require(fmt::byte_size<1000>(byte_size{1000}, si) == "1.0 kB");
            snn_require(fmt::byte_size<1000>(byte_size{1000}, si_short) == "1.0 k");
            snn_require(fmt::byte_size<1000>(byte_size{10000}, si) == "10 kB");
            snn_require(fmt::byte_size<1000>(byte_size{10000}, si_short) == "10 k");
            snn_require(fmt::byte_size<1000>(byte_size{100000}, si) == "100 kB");
            snn_require(fmt::byte_size<1000>(byte_size{100000}, si_short) == "100 k");
            snn_require(fmt::byte_size<1000>(byte_size{1000000}, si) == "1.0 MB");
            snn_require(fmt::byte_size<1000>(byte_size{1000000}, si_short) == "1.0 M");
            snn_require(fmt::byte_size<1000>(byte_size{10000000}, si) == "10 MB");
            snn_require(fmt::byte_size<1000>(byte_size{10000000}, si_short) == "10 M");
            snn_require(fmt::byte_size<1000>(byte_size{100000000}, si) == "100 MB");
            snn_require(fmt::byte_size<1000>(byte_size{100000000}, si_short) == "100 M");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000}, si) == "1.0 GB");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000}, si_short) == "1.0 G");
            snn_require(fmt::byte_size<1000>(byte_size{10000000000}, si) == "10 GB");
            snn_require(fmt::byte_size<1000>(byte_size{10000000000}, si_short) == "10 G");
            snn_require(fmt::byte_size<1000>(byte_size{100000000000}, si) == "100 GB");
            snn_require(fmt::byte_size<1000>(byte_size{100000000000}, si_short) == "100 G");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000000}, si) == "1.0 TB");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000000}, si_short) == "1.0 T");
            snn_require(fmt::byte_size<1000>(byte_size{10000000000000}, si) == "10 TB");
            snn_require(fmt::byte_size<1000>(byte_size{10000000000000}, si_short) == "10 T");
            snn_require(fmt::byte_size<1000>(byte_size{100000000000000}, si) == "100 TB");
            snn_require(fmt::byte_size<1000>(byte_size{100000000000000}, si_short) == "100 T");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000000000}, si) == "1.0 PB");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000000000}, si_short) == "1.0 P");
            snn_require(fmt::byte_size<1000>(byte_size{10000000000000000}, si) == "10 PB");
            snn_require(fmt::byte_size<1000>(byte_size{10000000000000000}, si_short) == "10 P");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000000000000}, si) == "1.0 EB");
            snn_require(fmt::byte_size<1000>(byte_size{1000000000000000000}, si_short) == "1.0 E");

            snn_require(fmt::byte_size<1000>(byte_size{2}, si) == "2 B");
            snn_require(fmt::byte_size<1000>(byte_size{2}, si_short) == "2 B");
            snn_require(fmt::byte_size<1000>(byte_size{22}, si) == "22 B");
            snn_require(fmt::byte_size<1000>(byte_size{22}, si_short) == "22 B");
            snn_require(fmt::byte_size<1000>(byte_size{222}, si) == "222 B");
            snn_require(fmt::byte_size<1000>(byte_size{222}, si_short) == "222 B");
            snn_require(fmt::byte_size<1000>(byte_size{2222}, si) == "2.2 kB");
            snn_require(fmt::byte_size<1000>(byte_size{2222}, si_short) == "2.2 k");
            snn_require(fmt::byte_size<1000>(byte_size{22222}, si) == "22 kB");
            snn_require(fmt::byte_size<1000>(byte_size{22222}, si_short) == "22 k");
            snn_require(fmt::byte_size<1000>(byte_size{222222}, si) == "222 kB");
            snn_require(fmt::byte_size<1000>(byte_size{222222}, si_short) == "222 k");
            snn_require(fmt::byte_size<1000>(byte_size{2222222}, si) == "2.2 MB");
            snn_require(fmt::byte_size<1000>(byte_size{2222222}, si_short) == "2.2 M");
            snn_require(fmt::byte_size<1000>(byte_size{22222222}, si) == "22 MB");
            snn_require(fmt::byte_size<1000>(byte_size{22222222}, si_short) == "22 M");
            snn_require(fmt::byte_size<1000>(byte_size{222222222}, si) == "222 MB");
            snn_require(fmt::byte_size<1000>(byte_size{222222222}, si_short) == "222 M");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222}, si) == "2.2 GB");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222}, si_short) == "2.2 G");
            snn_require(fmt::byte_size<1000>(byte_size{22222222222}, si) == "22 GB");
            snn_require(fmt::byte_size<1000>(byte_size{22222222222}, si_short) == "22 G");
            snn_require(fmt::byte_size<1000>(byte_size{222222222222}, si) == "222 GB");
            snn_require(fmt::byte_size<1000>(byte_size{222222222222}, si_short) == "222 G");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222222}, si) == "2.2 TB");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222222}, si_short) == "2.2 T");
            snn_require(fmt::byte_size<1000>(byte_size{22222222222222}, si) == "22 TB");
            snn_require(fmt::byte_size<1000>(byte_size{22222222222222}, si_short) == "22 T");
            snn_require(fmt::byte_size<1000>(byte_size{222222222222222}, si) == "222 TB");
            snn_require(fmt::byte_size<1000>(byte_size{222222222222222}, si_short) == "222 T");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222222222}, si) == "2.2 PB");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222222222}, si_short) == "2.2 P");
            snn_require(fmt::byte_size<1000>(byte_size{22222222222222222}, si) == "22 PB");
            snn_require(fmt::byte_size<1000>(byte_size{22222222222222222}, si_short) == "22 P");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222222222222}, si) == "2.2 EB");
            snn_require(fmt::byte_size<1000>(byte_size{2222222222222222222}, si_short) == "2.2 E");

            snn_require(fmt::byte_size<1000>(byte_size{5}, si) == "5 B");
            snn_require(fmt::byte_size<1000>(byte_size{5}, si_short) == "5 B");
            snn_require(fmt::byte_size<1000>(byte_size{55}, si) == "55 B");
            snn_require(fmt::byte_size<1000>(byte_size{55}, si_short) == "55 B");
            snn_require(fmt::byte_size<1000>(byte_size{555}, si) == "555 B");
            snn_require(fmt::byte_size<1000>(byte_size{555}, si_short) == "555 B");
            snn_require(fmt::byte_size<1000>(byte_size{5555}, si) == "5.6 kB");
            snn_require(fmt::byte_size<1000>(byte_size{5555}, si_short) == "5.6 k");
            snn_require(fmt::byte_size<1000>(byte_size{55555}, si) == "56 kB");
            snn_require(fmt::byte_size<1000>(byte_size{55555}, si_short) == "56 k");
            snn_require(fmt::byte_size<1000>(byte_size{555555}, si) == "556 kB");
            snn_require(fmt::byte_size<1000>(byte_size{555555}, si_short) == "556 k");
            snn_require(fmt::byte_size<1000>(byte_size{5555555}, si) == "5.6 MB");
            snn_require(fmt::byte_size<1000>(byte_size{5555555}, si_short) == "5.6 M");
            snn_require(fmt::byte_size<1000>(byte_size{55555555}, si) == "56 MB");
            snn_require(fmt::byte_size<1000>(byte_size{55555555}, si_short) == "56 M");
            snn_require(fmt::byte_size<1000>(byte_size{555555555}, si) == "556 MB");
            snn_require(fmt::byte_size<1000>(byte_size{555555555}, si_short) == "556 M");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555}, si) == "5.6 GB");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555}, si_short) == "5.6 G");
            snn_require(fmt::byte_size<1000>(byte_size{55555555555}, si) == "56 GB");
            snn_require(fmt::byte_size<1000>(byte_size{55555555555}, si_short) == "56 G");
            snn_require(fmt::byte_size<1000>(byte_size{555555555555}, si) == "556 GB");
            snn_require(fmt::byte_size<1000>(byte_size{555555555555}, si_short) == "556 G");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555555}, si) == "5.6 TB");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555555}, si_short) == "5.6 T");
            snn_require(fmt::byte_size<1000>(byte_size{55555555555555}, si) == "56 TB");
            snn_require(fmt::byte_size<1000>(byte_size{55555555555555}, si_short) == "56 T");
            snn_require(fmt::byte_size<1000>(byte_size{555555555555555}, si) == "556 TB");
            snn_require(fmt::byte_size<1000>(byte_size{555555555555555}, si_short) == "556 T");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555555555}, si) == "5.6 PB");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555555555}, si_short) == "5.6 P");
            snn_require(fmt::byte_size<1000>(byte_size{55555555555555555}, si) == "56 PB");
            snn_require(fmt::byte_size<1000>(byte_size{55555555555555555}, si_short) == "56 P");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555555555555}, si) == "5.6 EB");
            snn_require(fmt::byte_size<1000>(byte_size{5555555555555555555}, si_short) == "5.6 E");

            snn_require(fmt::byte_size<1000>(byte_size{9}, si) == "9 B");
            snn_require(fmt::byte_size<1000>(byte_size{9}, si_short) == "9 B");
            snn_require(fmt::byte_size<1000>(byte_size{99}, si) == "99 B");
            snn_require(fmt::byte_size<1000>(byte_size{99}, si_short) == "99 B");
            snn_require(fmt::byte_size<1000>(byte_size{999}, si) == "999 B");
            snn_require(fmt::byte_size<1000>(byte_size{999}, si_short) == "999 B");
            snn_require(fmt::byte_size<1000>(byte_size{9999}, si) == "10 kB");
            snn_require(fmt::byte_size<1000>(byte_size{9999}, si_short) == "10 k");
            snn_require(fmt::byte_size<1000>(byte_size{99999}, si) == "100 kB");
            snn_require(fmt::byte_size<1000>(byte_size{99999}, si_short) == "100 k");
            snn_require(fmt::byte_size<1000>(byte_size{999999}, si) == "1.0 MB");
            snn_require(fmt::byte_size<1000>(byte_size{999999}, si_short) == "1.0 M");
            snn_require(fmt::byte_size<1000>(byte_size{9999999}, si) == "10 MB");
            snn_require(fmt::byte_size<1000>(byte_size{9999999}, si_short) == "10 M");
            snn_require(fmt::byte_size<1000>(byte_size{99999999}, si) == "100 MB");
            snn_require(fmt::byte_size<1000>(byte_size{99999999}, si_short) == "100 M");
            snn_require(fmt::byte_size<1000>(byte_size{999999999}, si) == "1.0 GB");
            snn_require(fmt::byte_size<1000>(byte_size{999999999}, si_short) == "1.0 G");
            snn_require(fmt::byte_size<1000>(byte_size{9999999999}, si) == "10 GB");
            snn_require(fmt::byte_size<1000>(byte_size{9999999999}, si_short) == "10 G");
            snn_require(fmt::byte_size<1000>(byte_size{99999999999}, si) == "100 GB");
            snn_require(fmt::byte_size<1000>(byte_size{99999999999}, si_short) == "100 G");
            snn_require(fmt::byte_size<1000>(byte_size{999999999999}, si) == "1.0 TB");
            snn_require(fmt::byte_size<1000>(byte_size{999999999999}, si_short) == "1.0 T");
            snn_require(fmt::byte_size<1000>(byte_size{9999999999999}, si) == "10 TB");
            snn_require(fmt::byte_size<1000>(byte_size{9999999999999}, si_short) == "10 T");
            snn_require(fmt::byte_size<1000>(byte_size{99999999999999}, si) == "100 TB");
            snn_require(fmt::byte_size<1000>(byte_size{99999999999999}, si_short) == "100 T");
            snn_require(fmt::byte_size<1000>(byte_size{999999999999999}, si) == "1.0 PB");
            snn_require(fmt::byte_size<1000>(byte_size{999999999999999}, si_short) == "1.0 P");
            snn_require(fmt::byte_size<1000>(byte_size{9999999999999999}, si) == "10 PB");
            snn_require(fmt::byte_size<1000>(byte_size{9999999999999999}, si_short) == "10 P");
            snn_require(fmt::byte_size<1000>(byte_size{99999999999999999}, si) == "100 PB");
            snn_require(fmt::byte_size<1000>(byte_size{99999999999999999}, si_short) == "100 P");
            snn_require(fmt::byte_size<1000>(byte_size{9223372036854775807}, si) == "9.2 EB");
            snn_require(fmt::byte_size<1000>(byte_size{9223372036854775807}, si_short) == "9.2 E");

            snn_require(fmt::byte_size<1000>(byte_size{1024}, si) == "1.0 kB");
            snn_require(fmt::byte_size<1000>(byte_size{1024}, si_short) == "1.0 k");
            snn_require(fmt::byte_size<1000>(byte_size{1048576}, si) == "1.0 MB");
            snn_require(fmt::byte_size<1000>(byte_size{1048576}, si_short) == "1.0 M");
            snn_require(fmt::byte_size<1000>(byte_size{1073741824}, si) == "1.1 GB");
            snn_require(fmt::byte_size<1000>(byte_size{1073741824}, si_short) == "1.1 G");
            snn_require(fmt::byte_size<1000>(byte_size{1099511627776}, si) == "1.1 TB");
            snn_require(fmt::byte_size<1000>(byte_size{1099511627776}, si_short) == "1.1 T");
            snn_require(fmt::byte_size<1000>(byte_size{1125899906842624}, si) == "1.1 PB");
            snn_require(fmt::byte_size<1000>(byte_size{1125899906842624}, si_short) == "1.1 P");
            snn_require(fmt::byte_size<1000>(byte_size{1152921504606846976}, si) == "1.2 EB");
            snn_require(fmt::byte_size<1000>(byte_size{1152921504606846976}, si_short) == "1.2 E");

            snn_require(fmt::byte_size<1000>(byte_size{1001}, si) == "1.0 kB");
            snn_require(fmt::byte_size<1000>(byte_size{1001}, si_short) == "1.0 k");
            snn_require(fmt::byte_size<1000>(byte_size{1023}, si) == "1.0 kB");
            snn_require(fmt::byte_size<1000>(byte_size{1023}, si_short) == "1.0 k");
            snn_require(fmt::byte_size<1000>(byte_size{1047552}, si) == "1.0 MB");
            snn_require(fmt::byte_size<1000>(byte_size{1047552}, si_short) == "1.0 M");
            snn_require(fmt::byte_size<1000>(byte_size{1048575}, si) == "1.0 MB");
            snn_require(fmt::byte_size<1000>(byte_size{1048575}, si_short) == "1.0 M");

            // 1024 B,K,M,G...
            // 1024 B,KiB,MiB,GiB...

            using fmt::table::byte_size::iec;
            using fmt::table::byte_size::iec_short;

            snn_require(fmt::byte_size<1024>(byte_size{0}, iec) == "0 B");
            snn_require(fmt::byte_size<1024>(byte_size{0}, iec_short) == "0 B");
            snn_require(fmt::byte_size<1024>(byte_size{1}, iec) == "1 B");
            snn_require(fmt::byte_size<1024>(byte_size{1}, iec_short) == "1 B");
            snn_require(fmt::byte_size<1024>(byte_size{10}, iec) == "10 B");
            snn_require(fmt::byte_size<1024>(byte_size{10}, iec_short) == "10 B");
            snn_require(fmt::byte_size<1024>(byte_size{100}, iec) == "100 B");
            snn_require(fmt::byte_size<1024>(byte_size{100}, iec_short) == "100 B");
            snn_require(fmt::byte_size<1024>(byte_size{1000}, iec) == "1.0 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{1000}, iec_short) == "1.0 K");
            snn_require(fmt::byte_size<1024>(byte_size{10000}, iec) == "9.8 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{10000}, iec_short) == "9.8 K");
            snn_require(fmt::byte_size<1024>(byte_size{100000}, iec) == "98 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{100000}, iec_short) == "98 K");
            snn_require(fmt::byte_size<1024>(byte_size{1000000}, iec) == "977 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{1000000}, iec_short) == "977 K");
            snn_require(fmt::byte_size<1024>(byte_size{10000000}, iec) == "9.5 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{10000000}, iec_short) == "9.5 M");
            snn_require(fmt::byte_size<1024>(byte_size{100000000}, iec) == "95 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{100000000}, iec_short) == "95 M");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000}, iec) == "954 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000}, iec_short) == "954 M");
            snn_require(fmt::byte_size<1024>(byte_size{10000000000}, iec) == "9.3 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{10000000000}, iec_short) == "9.3 G");
            snn_require(fmt::byte_size<1024>(byte_size{100000000000}, iec) == "93 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{100000000000}, iec_short) == "93 G");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000000}, iec) == "931 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000000}, iec_short) == "931 G");
            snn_require(fmt::byte_size<1024>(byte_size{10000000000000}, iec) == "9.1 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{10000000000000}, iec_short) == "9.1 T");
            snn_require(fmt::byte_size<1024>(byte_size{100000000000000}, iec) == "91 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{100000000000000}, iec_short) == "91 T");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000000000}, iec) == "909 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000000000}, iec_short) == "909 T");
            snn_require(fmt::byte_size<1024>(byte_size{10000000000000000}, iec) == "8.9 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{10000000000000000}, iec_short) == "8.9 P");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000000000000}, iec) == "888 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{1000000000000000000}, iec_short) == "888 P");

            snn_require(fmt::byte_size<1024>(byte_size{2}, iec) == "2 B");
            snn_require(fmt::byte_size<1024>(byte_size{2}, iec_short) == "2 B");
            snn_require(fmt::byte_size<1024>(byte_size{22}, iec) == "22 B");
            snn_require(fmt::byte_size<1024>(byte_size{22}, iec_short) == "22 B");
            snn_require(fmt::byte_size<1024>(byte_size{222}, iec) == "222 B");
            snn_require(fmt::byte_size<1024>(byte_size{222}, iec_short) == "222 B");
            snn_require(fmt::byte_size<1024>(byte_size{2222}, iec) == "2.2 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{2222}, iec_short) == "2.2 K");
            snn_require(fmt::byte_size<1024>(byte_size{22222}, iec) == "22 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{22222}, iec_short) == "22 K");
            snn_require(fmt::byte_size<1024>(byte_size{222222}, iec) == "217 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{222222}, iec_short) == "217 K");
            snn_require(fmt::byte_size<1024>(byte_size{2222222}, iec) == "2.1 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{2222222}, iec_short) == "2.1 M");
            snn_require(fmt::byte_size<1024>(byte_size{22222222}, iec) == "21 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{22222222}, iec_short) == "21 M");
            snn_require(fmt::byte_size<1024>(byte_size{222222222}, iec) == "212 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{222222222}, iec_short) == "212 M");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222}, iec) == "2.1 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222}, iec_short) == "2.1 G");
            snn_require(fmt::byte_size<1024>(byte_size{22222222222}, iec) == "21 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{22222222222}, iec_short) == "21 G");
            snn_require(fmt::byte_size<1024>(byte_size{222222222222}, iec) == "207 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{222222222222}, iec_short) == "207 G");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222222}, iec) == "2.0 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222222}, iec_short) == "2.0 T");
            snn_require(fmt::byte_size<1024>(byte_size{22222222222222}, iec) == "20 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{22222222222222}, iec_short) == "20 T");
            snn_require(fmt::byte_size<1024>(byte_size{222222222222222}, iec) == "202 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{222222222222222}, iec_short) == "202 T");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222222222}, iec) == "2.0 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222222222}, iec_short) == "2.0 P");
            snn_require(fmt::byte_size<1024>(byte_size{22222222222222222}, iec) == "20 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{22222222222222222}, iec_short) == "20 P");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222222222222}, iec) == "1.9 EiB");
            snn_require(fmt::byte_size<1024>(byte_size{2222222222222222222}, iec_short) == "1.9 E");

            snn_require(fmt::byte_size<1024>(byte_size{5}, iec) == "5 B");
            snn_require(fmt::byte_size<1024>(byte_size{5}, iec_short) == "5 B");
            snn_require(fmt::byte_size<1024>(byte_size{55}, iec) == "55 B");
            snn_require(fmt::byte_size<1024>(byte_size{55}, iec_short) == "55 B");
            snn_require(fmt::byte_size<1024>(byte_size{555}, iec) == "555 B");
            snn_require(fmt::byte_size<1024>(byte_size{555}, iec_short) == "555 B");
            snn_require(fmt::byte_size<1024>(byte_size{5555}, iec) == "5.4 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{5555}, iec_short) == "5.4 K");
            snn_require(fmt::byte_size<1024>(byte_size{55555}, iec) == "54 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{55555}, iec_short) == "54 K");
            snn_require(fmt::byte_size<1024>(byte_size{555555}, iec) == "543 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{555555}, iec_short) == "543 K");
            snn_require(fmt::byte_size<1024>(byte_size{5555555}, iec) == "5.3 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{5555555}, iec_short) == "5.3 M");
            snn_require(fmt::byte_size<1024>(byte_size{55555555}, iec) == "53 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{55555555}, iec_short) == "53 M");
            snn_require(fmt::byte_size<1024>(byte_size{555555555}, iec) == "530 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{555555555}, iec_short) == "530 M");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555}, iec) == "5.2 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555}, iec_short) == "5.2 G");
            snn_require(fmt::byte_size<1024>(byte_size{55555555555}, iec) == "52 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{55555555555}, iec_short) == "52 G");
            snn_require(fmt::byte_size<1024>(byte_size{555555555555}, iec) == "517 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{555555555555}, iec_short) == "517 G");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555555}, iec) == "5.1 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555555}, iec_short) == "5.1 T");
            snn_require(fmt::byte_size<1024>(byte_size{55555555555555}, iec) == "51 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{55555555555555}, iec_short) == "51 T");
            snn_require(fmt::byte_size<1024>(byte_size{555555555555555}, iec) == "505 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{555555555555555}, iec_short) == "505 T");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555555555}, iec) == "4.9 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555555555}, iec_short) == "4.9 P");
            snn_require(fmt::byte_size<1024>(byte_size{55555555555555555}, iec) == "49 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{55555555555555555}, iec_short) == "49 P");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555555555555}, iec) == "4.8 EiB");
            snn_require(fmt::byte_size<1024>(byte_size{5555555555555555555}, iec_short) == "4.8 E");

            snn_require(fmt::byte_size<1024>(byte_size{9}, iec) == "9 B");
            snn_require(fmt::byte_size<1024>(byte_size{9}, iec_short) == "9 B");
            snn_require(fmt::byte_size<1024>(byte_size{99}, iec) == "99 B");
            snn_require(fmt::byte_size<1024>(byte_size{99}, iec_short) == "99 B");
            snn_require(fmt::byte_size<1024>(byte_size{999}, iec) == "999 B");
            snn_require(fmt::byte_size<1024>(byte_size{999}, iec_short) == "999 B");
            snn_require(fmt::byte_size<1024>(byte_size{9999}, iec) == "9.8 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{9999}, iec_short) == "9.8 K");
            snn_require(fmt::byte_size<1024>(byte_size{99999}, iec) == "98 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{99999}, iec_short) == "98 K");
            snn_require(fmt::byte_size<1024>(byte_size{999999}, iec) == "977 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{999999}, iec_short) == "977 K");
            snn_require(fmt::byte_size<1024>(byte_size{9999999}, iec) == "9.5 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{9999999}, iec_short) == "9.5 M");
            snn_require(fmt::byte_size<1024>(byte_size{99999999}, iec) == "95 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{99999999}, iec_short) == "95 M");
            snn_require(fmt::byte_size<1024>(byte_size{999999999}, iec) == "954 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{999999999}, iec_short) == "954 M");
            snn_require(fmt::byte_size<1024>(byte_size{9999999999}, iec) == "9.3 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{9999999999}, iec_short) == "9.3 G");
            snn_require(fmt::byte_size<1024>(byte_size{99999999999}, iec) == "93 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{99999999999}, iec_short) == "93 G");
            snn_require(fmt::byte_size<1024>(byte_size{999999999999}, iec) == "931 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{999999999999}, iec_short) == "931 G");
            snn_require(fmt::byte_size<1024>(byte_size{9999999999999}, iec) == "9.1 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{9999999999999}, iec_short) == "9.1 T");
            snn_require(fmt::byte_size<1024>(byte_size{99999999999999}, iec) == "91 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{99999999999999}, iec_short) == "91 T");
            snn_require(fmt::byte_size<1024>(byte_size{999999999999999}, iec) == "909 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{999999999999999}, iec_short) == "909 T");
            snn_require(fmt::byte_size<1024>(byte_size{9999999999999999}, iec) == "8.9 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{9999999999999999}, iec_short) == "8.9 P");
            snn_require(fmt::byte_size<1024>(byte_size{99999999999999999}, iec) == "89 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{99999999999999999}, iec_short) == "89 P");
            snn_require(fmt::byte_size<1024>(byte_size{9223372036854775807}, iec) == "8.0 EiB");
            snn_require(fmt::byte_size<1024>(byte_size{9223372036854775807}, iec_short) == "8.0 E");

            snn_require(fmt::byte_size<1024>(byte_size{1024}, iec) == "1.0 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{1024}, iec_short) == "1.0 K");
            snn_require(fmt::byte_size<1024>(byte_size{1048576}, iec) == "1.0 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{1048576}, iec_short) == "1.0 M");
            snn_require(fmt::byte_size<1024>(byte_size{1073741824}, iec) == "1.0 GiB");
            snn_require(fmt::byte_size<1024>(byte_size{1073741824}, iec_short) == "1.0 G");
            snn_require(fmt::byte_size<1024>(byte_size{1099511627776}, iec) == "1.0 TiB");
            snn_require(fmt::byte_size<1024>(byte_size{1099511627776}, iec_short) == "1.0 T");
            snn_require(fmt::byte_size<1024>(byte_size{1125899906842624}, iec) == "1.0 PiB");
            snn_require(fmt::byte_size<1024>(byte_size{1125899906842624}, iec_short) == "1.0 P");
            snn_require(fmt::byte_size<1024>(byte_size{1152921504606846976}, iec) == "1.0 EiB");
            snn_require(fmt::byte_size<1024>(byte_size{1152921504606846976}, iec_short) == "1.0 E");

            snn_require(fmt::byte_size<1024>(byte_size{1001}, iec) == "1.0 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{1001}, iec_short) == "1.0 K");
            snn_require(fmt::byte_size<1024>(byte_size{1023}, iec) == "1.0 KiB");
            snn_require(fmt::byte_size<1024>(byte_size{1023}, iec_short) == "1.0 K");
            snn_require(fmt::byte_size<1024>(byte_size{1047552}, iec) == "1.0 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{1047552}, iec_short) == "1.0 M");
            snn_require(fmt::byte_size<1024>(byte_size{1048575}, iec) == "1.0 MiB");
            snn_require(fmt::byte_size<1024>(byte_size{1048575}, iec_short) == "1.0 M");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_byte_size_generated());
        snn_require(app::test_format_byte_size());
    }
}
