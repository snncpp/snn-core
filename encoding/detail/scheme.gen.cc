// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/json/decoder.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/map/sorted.hh"
#include "snn-core/regex/pattern.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        auto json_data = file::read<strbuf>("encodings.json").value_or_default();

        if (!json_data)
        {
            fmt::print_error("Error, could not read: encodings.json\nDownload here:\n"
                             "https://encoding.spec.whatwg.org/encodings.json\n");
            return constant::exit::failure;
        }

        map::sorted<str, str> encodings;
        encodings.insert("iso8859_1", "ISO-8859-1"); // Not used by WHATWG.

        regex::pattern alpha_underscore_num{"([a-z])_([0-9])"};
        regex::pattern num_underscore_alpha{"([0-9])_([a-z])"};

        json::decoder d;
        const auto doc = d.decode_inplace(json_data.range()).value();
        for (const auto& group : doc.root())
        {
            for (const auto& enc : group.get("encodings"))
            {
                str name = enc.get("name").to<cstrview>();
                str var  = name;

                // Use consistent naming.
                var.transform(chr::to_alpha_lower);
                var.replace('-', '_');
                var = alpha_underscore_num.replace(var, "$1$2");
                var = num_underscore_alpha.replace(var, "$1$2");

                if (!encodings.insert(std::move(var), std::move(name)))
                {
                    fmt::print_error_line("Error: Duplicate encoding?");
                    return constant::exit::failure;
                }
            }
        }

        const usize encoding_count = encodings.count() + 1; // +1 for unknown.

        strbuf hh{container::reserve, constant::size::kibibyte<usize>};

        // scheme.hh

        hh << "// scheme.hh\n\n";

        hh << "    // ## Enums\n\n";

        hh << "    // ### scheme\n\n";

        hh << "    enum class scheme : u8\n";
        hh << "    {\n";
        hh << "        unknown = 0,\n";

        str last_encoding;
        for (const auto& [var, name] : encodings)
        {
            hh << "        " << var << ',';
            if (var == "iso8859_1")
            {
                hh << " // Not used by the WHATWG Encoding Standard (maps to windows1252).";
            }
            hh << "\n";
            last_encoding = var;
        }

        hh.drop_back_n(string_size("\n"));

        hh << " // Last (used below).\n";
        hh << "    };\n\n";

        hh << "    // ## Constants\n\n";

        hh << "    // ### scheme_count\n\n";

        hh << "    inline constexpr usize scheme_count = " << as_num(encoding_count) << ";\n";
        hh << "    static_assert(to_underlying(scheme::" << last_encoding
           << ") == (scheme_count - 1));\n\n";

        // schemes.hh

        hh << "// schemes.hh\n\n";

        hh << "    // ## Arrays\n\n";

        hh << "    // ### schemes\n\n";

        hh << "    // clang-format off\n";
        hh << "    inline constexpr array<scheme, scheme_count> schemes{\n";
        hh << "        scheme::unknown,\n";
        for (const auto& [var, name] : encodings)
        {
            hh << "        scheme::" << var << ",";
            if (var == "iso8859_1")
            {
                hh << " // Not used by the WHATWG Encoding Standard.";
            }
            hh << "\n";
        }
        hh << "    };\n";
        hh << "    // clang-format on\n\n";

        // scheme_names.hh

        hh << "// scheme_names.hh\n\n";

        hh << "    // ## Arrays\n\n";

        hh << "    // ### scheme_names\n\n";

        hh << "    // clang-format off\n";
        hh << "    inline constexpr array<size_prefixed_string_literal, scheme_count> "
              "scheme_names{\n";
        hh << "        \"\\007Unknown\",\n";
        for (const auto& [var, name] : encodings)
        {
            hh << "        \"\\";
            constexpr usize min_digits = 3;
            hh.append_integral<math::base::octal>(name.size(), min_digits);
            hh << name << "\",";
            if (var == "iso8859_1")
            {
                hh << " // Not used by the WHATWG Encoding Standard.";
            }
            hh << "\n";
        }
        hh << "    };\n";
        hh << "    // clang-format on\n\n";

        file::standard::out{} << hh;

        return constant::exit::success;
    }
}
