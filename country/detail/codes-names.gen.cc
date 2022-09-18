// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/vec.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/view/element.hh"
#include "snn-core/string/range/split.hh"
#include <locale>

namespace snn
{
    int main(array_view<const env::argument>)
    {
        const auto data = file::read<strbuf>("tzdb/iso3166.tab").value();

        vec<pair::name_value<cstrview>> countries;

        for (const auto line : string::range::split{data, '\n'})
        {
            if (line && !line.has_front('#'))
            {
                auto rng = string::range::split{line, '\t'};

                const cstrview code = rng.pop_front().value();
                const cstrview name = rng.pop_front().value();

                countries.append_inplace(name, code);

                if (rng)
                {
                    fmt::print_error_line("Error: Line must contain exactly two columns.");
                    return constant::exit::failure;
                }
            }
        }

        std::locale loc{"en_US.UTF-8"};
        const auto& facet = std::use_facet<std::collate<char>>(loc);
        countries.sort([&facet](const auto& a, const auto& b) -> bool {
            return facet.compare(a.name.begin(), a.name.end(), b.name.begin(), b.name.end()) < 0;
        });

        strbuf hh;

        hh << "inline constexpr array<size_prefixed_string_literal, " << as_num(countries.count())
           << "> names{";
        for (const auto name : countries.range() | range::v::element<0>{})
        {
            hh << "\"\\";
            constexpr usize min_digits = 3;
            hh.append_integral<math::base::octal>(name.size(), min_digits);
            hh << name << "\", ";
        }
        hh << "};\n";

        hh << "inline constexpr array<country::code, " << as_num(countries.count()) << "> codes{";
        for (const auto code : countries.range() | range::v::element<1>{})
        {
            hh << '"' << code << '"' << ", ";
        }
        hh << "};\n";

        file::standard::out{} << hh;

        return constant::exit::success;
    }
}
