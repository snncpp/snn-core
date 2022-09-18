// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/range/integral.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        strbuf table;

        table << "inline constexpr array<bool, 256> encode_unreserved_lookup{";

        usize count = 0;
        for (const auto b : range::integral<byte>{})
        {
            // https://www.rfc-editor.org/rfc/rfc3986#section-2.3
            // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
            if (chr::is_alphanumeric(to_char(b)) || b == '-' || b == '.' || b == '_' || b == '~')
            {
                table << "1, ";
            }
            else
            {
                table << "0, ";
            }

            ++count;
        }
        snn_assert(count == 256);

        table << "};\n";

        file::standard::out{} << table;

        return constant::exit::success;
    }
}
