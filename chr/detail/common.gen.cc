// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/range/integral.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        strbuf out;

        {
            out << "inline constexpr array<u8, 256> property_lookup{";

            constexpr u8 is_alpha              = 0b1000'0000;
            constexpr u8 is_alphanumeric       = 0b0100'0000;
            constexpr u8 is_alphanumeric_lower = 0b0010'0000;
            constexpr u8 is_alphanumeric_upper = 0b0001'0000;
            constexpr u8 is_ascii_control      = 0b0000'1000;
            // constexpr u8 unused             = 0b0000'0100;
            // constexpr u8 unused             = 0b0000'0010;
            // constexpr u8 unused             = 0b0000'0001;

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                u8 val = 0;

                if (b >= to_byte('0') && b <= to_byte('9'))
                {
                    val = val | is_alphanumeric;
                    val = val | is_alphanumeric_lower;
                    val = val | is_alphanumeric_upper;
                }

                if (b >= to_byte('a') && b <= to_byte('z'))
                {
                    val = val | is_alpha;
                    val = val | is_alphanumeric;
                    val = val | is_alphanumeric_lower;
                }

                if (b >= to_byte('A') && b <= to_byte('Z'))
                {
                    val = val | is_alpha;
                    val = val | is_alphanumeric;
                    val = val | is_alphanumeric_upper;
                }

                if (b < 0x20 || b == 0x7f)
                {
                    val = val | is_ascii_control;
                }

                out << as_num(val) << ", ";

                ++count;
            }
            snn_assert(count == 256);

            out << "};\n";
        }

        {
            out << "inline constexpr array<byte, 256> lowercase_lookup{";

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                if (b >= to_byte('A') && b <= to_byte('Z'))
                {
                    out << as_num(b + (to_byte('a') - to_byte('A'))) << ", ";
                }
                else
                {
                    out << as_num(b) << ", ";
                }

                ++count;
            }
            snn_assert(count == 256);

            out << "};\n";
        }

        {
            out << "inline constexpr array<byte, 256> uppercase_lookup{";

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                if (b >= to_byte('a') && b <= to_byte('z'))
                {
                    out << as_num(b - (to_byte('a') - to_byte('A'))) << ", ";
                }
                else
                {
                    out << as_num(b) << ", ";
                }

                ++count;
            }
            snn_assert(count == 256);

            out << "};\n";
        }

        {
            out << "inline constexpr array<u8, 256> hex_lookup{";

            usize count = 0;
            for (auto b : range::integral<byte>{})
            {
                if (b >= to_byte('0') && b <= to_byte('9'))
                {
                    b = b - to_byte('0');
                }
                else if (b >= to_byte('a') && b <= to_byte('f'))
                {
                    b = b - (to_byte('a') - 10);
                }
                else if (b >= to_byte('A') && b <= to_byte('F'))
                {
                    b = b - (to_byte('A') - 10);
                }
                else
                {
                    b = 255;
                }

                out << as_num(b) << ", ";

                ++count;
            }
            snn_assert(count == 256);

            out << "};\n";
        }

        file::standard::out{} << out;

        return constant::exit::success;
    }
}
