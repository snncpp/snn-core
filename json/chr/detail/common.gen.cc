// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/strcore.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/range/integral.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        strbuf table;

        table << "inline constexpr array<u8, 256> property_lookup{";

        constexpr u8 is_non_special_ascii = 0b0000'0001;
        constexpr u8 is_special           = 0b0000'0010;
        constexpr u8 is_whitespace        = 0b0000'0100;

        usize count = 0;
        for (const auto b : range::integral<byte>{})
        {
            u8 val = 0;

            if (b < 0x80 && b >= ' ' && b != '"' && b != '\\')
            {
                val = val | is_non_special_ascii;
            }

            if (b < ' ' || b == '"' || b == '\\')
            {
                val = val | is_special;
            }

            if (b == ' ' || b == '\t' || b == '\n' || b == '\r')
            {
                val = val | is_whitespace;
            }

            table << as_num(val) << ", ";

            ++count;
        }
        snn_assert(count == 256);

        table << "};\n";

        file::standard::out{} << table;

        return constant::exit::success;
    }
}
