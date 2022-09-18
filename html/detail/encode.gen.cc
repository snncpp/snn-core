// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/range/integral.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        {
            strbuf table;

            table << "inline constexpr array<bool, 256> encode_unreserved_lookup{";

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                switch (b)
                {
                    case '<':
                    case '>':
                    case '&':
                    case '"':
                    case '\'':
                        table << "0, ";
                        break;

                    default:
                        table << "1, ";
                        break;
                }

                ++count;
            }
            snn_assert(count == 256);

            table << "};\n";

            file::standard::out{} << table;
        }

        {
            strbuf table;

            table << "inline constexpr array<bool, 256> encode_unreserved_attribute_lookup{";

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                switch (b)
                {
                    case '&':
                    case '"':
                    case 0xc2:
                        table << "0, ";
                        break;

                    default:
                        table << "1, ";
                        break;
                }

                ++count;
            }
            snn_assert(count == 256);

            table << "};\n";

            file::standard::out{} << table;
        }

        {
            strbuf table;

            table << "inline constexpr array<bool, 256> encode_unreserved_text_lookup{";

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                switch (b)
                {
                    case '<':
                    case '>':
                    case '&':
                    case 0xc2:
                        table << "0, ";
                        break;

                    default:
                        table << "1, ";
                        break;
                }

                ++count;
            }
            snn_assert(count == 256);

            table << "};\n";

            file::standard::out{} << table;
        }

        return constant::exit::success;
    }
}
