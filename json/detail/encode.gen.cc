// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/strcore.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool is_special(const byte b) noexcept
        {
            // Non-ascii, control or reserved.
            return b >= 0x80 || b < ' ' || b == '"' || b == '\\';
        }

        constexpr bool is_special_with_options(const byte b) noexcept
        {
            return b == '<' || b == '>' || b == '&';
        }
    }
}

namespace snn
{
    int main(array_view<const env::argument>)
    {
        strbuf table;

        // encode_passthrough_lookup

        table << "inline constexpr array<bool, 256> encode_passthrough_lookup{";

        usize count = 0;
        for (const auto b : range::integral<byte>{})
        {
            if (app::is_special(b))
            {
                table << "0, ";
            }
            else
            {
                table << "1, ";
            }

            ++count;
        }
        snn_assert(count == 256);

        table << "};\n";

        // encode_passthrough_opt_lookup

        table << "inline constexpr array<bool, 256> encode_passthrough_opt_lookup{";

        count = 0;
        for (const auto b : range::integral<byte>{})
        {
            if (app::is_special(b) || app::is_special_with_options(b))
            {
                table << "0, ";
            }
            else
            {
                table << "1, ";
            }

            ++count;
        }
        snn_assert(count == 256);

        table << "};\n";

        file::standard::out{} << table;

        return constant::exit::success;
    }
}
