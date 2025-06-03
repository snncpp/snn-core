// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/exception.hh"
#include "snn-core/main.hh"
#include "snn-core/vec.hh"
#include "snn-core/algo/join.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/country/code.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/write.hh"
#include "snn-core/file/dir/create_recursive.hh"
#include "snn-core/file/dir/list.hh"
#include "snn-core/file/path/append.hh"
#include "snn-core/file/path/join.hh"
#include "snn-core/fmt/print.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/map/unsorted.hh"
#include "snn-core/range/view/drop_last.hh"
#include "snn-core/range/view/enumerate.hh"
#include "snn-core/set/sorted.hh"
#include "snn-core/set/unsorted.hh"
#include "snn-core/string/normalize_line_endings.hh"
#include "snn-core/string/range/split.hh"
#include "snn-core/time/zone/location.hh"
#include "snn-core/time/zone/offset.hh"
#include "snn-core/time/zone/transition.hh"
#include "snn-core/time/zone/tzif/decode.hh"

namespace snn::app
{
    struct zone_tab_metadata final
    {
        str comment;
        str coordinates;
        vec<str> country_codes;
    };

    namespace
    {
        bool ignore_link(const cstrview name)
        {
            [[clang::no_destroy]] static const set::unsorted<cstrview> ignore{
                // From backward file: "Alternate names for the same location"
                "Africa/Asmera",
                "America/Godthab",
                "Asia/Ashkhabad",
                "Asia/Calcutta",
                "Asia/Chungking",
                "Asia/Dacca",
                "Asia/Istanbul",
                "Asia/Katmandu",
                "Asia/Macao",
                "Asia/Rangoon",
                "Asia/Saigon",
                "Asia/Thimbu",
                "Asia/Ujung_Pandang",
                "Asia/Ulan_Bator",
                "Atlantic/Faeroe",
                "Europe/Kiev",
                "Europe/Nicosia",
                "Pacific/Ponape",
                "Pacific/Truk",

                // From backward file: "Two-part names that were renamed mostly to three-part names
                // in 1995"
                "America/Buenos_Aires",
                "America/Catamarca",
                "America/Cordoba",
                "America/Indianapolis",
                "America/Jujuy",
                "America/Knox_IN",
                "America/Louisville",
                "America/Mendoza",
                "America/Virgin",
                "Pacific/Samoa",

                // From backward file: "Pre-1993 naming conventions"
                "Australia/ACT",
                "Australia/LHI",
                "Australia/NSW",
                "Australia/North",
                "Australia/Queensland",
                "Australia/South",
                "Australia/Tasmania",
                "Australia/Victoria",
                "Australia/West",
                "Australia/Yancowinna",
                "Brazil/Acre",
                "Brazil/DeNoronha",
                "Brazil/East",
                "Brazil/West",
                "Canada/Atlantic",
                "Canada/Central",
                "Canada/Eastern",
                "Canada/Mountain",
                "Canada/Newfoundland",
                "Canada/Pacific",
                "Canada/Saskatchewan",
                "Canada/Yukon",
                "Chile/Continental",
                "Chile/EasterIsland",
                "Mexico/BajaNorte",
                "Mexico/BajaSur",
                "Mexico/General",
                "US/Alaska",
                "US/Aleutian",
                "US/Arizona",
                "US/Central",
                "US/East-Indiana",
                "US/Eastern",
                "US/Hawaii",
                "US/Indiana-Starke",
                "US/Michigan",
                "US/Mountain",
                "US/Pacific",
                "US/Samoa",

                // From backward file: "Non-zone.tab locations with timestamps since 1970 that
                // duplicate those of an existing location"
                "Africa/Timbuktu",
                "America/Argentina/ComodRivadavia",
                "America/Atka",
                "America/Coral_Harbour",
                "America/Ensenada",
                "America/Fort_Wayne",
                "America/Montreal",
                "America/Nipigon",
                "America/Porto_Acre",
                "America/Rainy_River",
                "America/Rosario",
                "America/Santa_Isabel",
                "America/Shiprock",
                "America/Thunder_Bay",
                "Antarctica/South_Pole",
                "Asia/Chongqing",
                "Asia/Harbin",
                "Asia/Kashgar",
                "Asia/Tel_Aviv",
                "Atlantic/Jan_Mayen",
                "Australia/Canberra",
                "Australia/Currie",
                "Europe/Belfast",
                "Europe/Tiraspol",
                "Europe/Uzhgorod",
                "Europe/Zaporozhye",
                "Pacific/Enderbury",
                "Pacific/Johnston",
                "Pacific/Yap",
            };
            return ignore.contains(name);
        }

        bool ignore_name(const cstrview name)
        {
            return name.has_front("Etc/") || !name.contains('/');
        }

        bool is_valid_identifier(const cstrview s) noexcept
        {
            // Match: [a-z][a-z0-9_]*
            auto rng = s.range();
            if (rng.drop_front_if(chr::is_alpha_lower))
            {
                rng.pop_front_while(fn::is_any_of{chr::is_alphanumeric_lower, fn::in_array{'_'}});
                return rng.is_empty();
            }
            return false;
        }

        str make_relative_header_path(const vec<str>& identifiers)
        {
            str path;
            identifiers.each([&](auto& s) { file::path::append(s, path, promise::no_overlap); });
            path.append(".hh");
            return path;
        }

        vec<str> split_into_identifiers(const cstrview name)
        {
            vec<str> identifiers;
            for (const cstrview name_part : string::range::split{name, '/'})
            {
                str ident{name_part};
                ident.replace('-', '_');
                ident.transform(chr::to_alpha_lower);
                if (app::is_valid_identifier(ident))
                {
                    identifiers.append(std::move(ident));
                }
                else
                {
                    throw_or_abort(fmt::format("Invalid identifier: {}", ident));
                }
            }
            return identifiers;
        }

        void generate_header(const str& name, const str& links_to, const zone_tab_metadata& meta,
                             const vec<str>& identifiers, const str& rel_include_path,
                             const str& zoneinfo_path, const str& output_path)
        {
            vec<time::zone::offset> offsets;
            vec<time::zone::transition> transitions;
            str tzif_footer;

            const auto tzif_path = file::path::join(zoneinfo_path, name);
            const auto tzif_data = file::read<strbuf>(tzif_path).value_or_default();

            if (!links_to)
            {
                if (!time::zone::tzif::decode(tzif_data.range(), offsets, transitions, tzif_footer))
                {
                    throw_or_abort(fmt::format("Couldn't decode TZif file: {}", tzif_path));
                }

                if (offsets.is_empty())
                {
                    throw_or_abort(fmt::format("No time zone offsets in file: {}", tzif_path));
                }

                if (transitions.is_empty())
                {
                    throw_or_abort(fmt::format("No time zone transitions in file: {}", tzif_path));
                }
            }
            else
            {
                const auto source_path = file::path::join(zoneinfo_path, links_to);
                const auto source_data = file::read<strbuf>(source_path).value();
                if (tzif_data != source_data)
                {
                    throw_or_abort(fmt::format("{} and {} data differs", name, links_to));
                }
            }

            const str& tail_ident = identifiers.back().value();

            // Will be empty if `links_to` is empty.
            const vec<str> source_identifiers = app::split_into_identifiers(links_to);

            strbuf hh{container::reserve, 10'000};

            hh << "// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.\n";
            hh << "// SPDX-License-Identifier: BSL-1.0\n";

            hh << '\n';

            hh << "// This file is generated, DO NOT EDIT MANUALLY.\n\n";

            hh << "// IANA Time Zone Database (public domain).\n\n";

            hh << "#pragma once\n\n";

            hh << "#include \"snn-core/array.hh\"\n";
            hh << "#include \"snn-core/country/code.hh\"\n";
            hh << "#include \"snn-core/time/zone/offset.hh\"\n";
            hh << "#include \"snn-core/time/zone/transition.hh\"\n";
            hh << "#include \"snn-core/time/zone/db/entry.hh\"\n";

            if (links_to)
            {
                hh << "#include \"snn-core/time/zone/db/"
                   << app::make_relative_header_path(source_identifiers) << "\"\n";
            }

            hh << "\n";

            hh << "namespace snn::time::zone::db";
            for (const auto& ns : identifiers.range() | range::v::drop_last{1})
            {
                hh << "::" << ns;
            }
            hh << "\n";
            hh << "{\n";

            // Disable clang-format.

            hh << "    // clang-format off\n\n";

            // Detail namespace.

            hh << "    namespace detail::" << tail_ident << "\n";
            hh << "    {\n";

            // Country codes.

            hh << "        inline constexpr array<country::code, "
               << as_num(meta.country_codes.count()) << "> country_codes{\n";
            for (const auto& cc : meta.country_codes)
            {
                hh << "            \"" << cc << "\",\n";
            }
            hh << "        };\n";

            if (!links_to)
            {
                // Offsets

                hh << "\n        " << "inline constexpr array<zone::offset, "
                   << as_num(offsets.count()) << "> offsets{{\n";
                for (const auto& offset : offsets)
                {
                    hh << "            " << "{\"" << offset.abbr().view() << "\", "
                       << as_num(offset.seconds()) << ", ";
                    if (offset.is_dst())
                    {
                        hh << "true";
                    }
                    else
                    {
                        hh << "false";
                    }
                    hh << "},\n";
                }
                hh << "        }};\n";

                // Transitions

                hh << "\n        " << "inline constexpr array<zone::transition, "
                   << as_num(transitions.count()) << "> transitions{{\n";
                for (const auto& t : transitions)
                {
                    hh << "            " << "{" << as_num(t.offset_index) << ", " << as_num(t.when)
                       << "},\n";
                }
                hh << "        }};\n";
            }

            // End detail namespace.

            hh << "    }\n\n";

            // DB entry.

            const strbuf indentation{container::fill, tail_ident.size() + 32, ' '};

            hh << "    inline constexpr db::entry " << tail_ident << "{\"" << name << "\",\n";
            hh << indentation << "\"" << meta.comment << "\",\n";
            hh << indentation << "\"" << meta.coordinates << "\",\n";
            hh << indentation << "detail::" << tail_ident << "::country_codes,\n";

            if (!links_to)
            {
                hh << indentation << "detail::" << tail_ident << "::offsets,\n";
                hh << indentation << "detail::" << tail_ident << "::transitions};\n";
            }
            else
            {
                snn_assert(source_identifiers.count() >= 2);
                auto src_rng          = source_identifiers.range();
                const str& src_tail   = src_rng.pop_back(promise::not_empty);
                const auto src_prefix = algo::join<str>(src_rng, "::");
                hh << indentation << src_prefix << "::detail::" << src_tail << "::offsets,\n";
                hh << indentation << src_prefix << "::detail::" << src_tail << "::transitions};\n";
            }

            hh << '\n';

            // End disable clang-format.

            hh << "    // clang-format on\n";

            // End snn namespace.

            hh << "}\n";

            const auto header_path = file::path::join(output_path, rel_include_path);
            fmt::print_line("... {}", header_path);
            file::dir::create_recursive(file::path::parent(header_path)).discard();
            file::write(header_path, hh).or_throw();
        }

        void list_recursive(const str& path, const str& rel_path, set::sorted<str>& files)
        {
            file::dir::list(path, [&](const file::dir::transient_entry e) {
                const auto entry_name = e.name<cstrview>();
                str entry_rel_path    = file::path::join(rel_path, entry_name);
                if (e.is_directory())
                {
                    const str entry_full_path = file::path::join(path, entry_name);
                    list_recursive(entry_full_path, entry_rel_path, files);
                }
                else if (!files.insert(std::move(entry_rel_path)))
                {
                    throw_or_abort("Duplicate file entry"); // Should not be possible.
                }
            }).or_throw();
        }

        set::sorted<str> list_recursive(const str& path)
        {
            set::sorted<str> files;
            list_recursive(path, "", files);
            return files;
        }

        const zone_tab_metadata& metadata(const str& name, const str& links_to,
                                          const map::unsorted<str, zone_tab_metadata>& primary,
                                          const map::unsorted<str, zone_tab_metadata>& secondary)
        {
            if (auto opt = primary.get(name))
            {
                return opt.value();
            }

            if (auto opt = secondary.get(name))
            {
                return opt.value();
            }

            fmt::print_line("Using {} metadata for {}.", links_to, name);

            if (auto opt = primary.get(links_to))
            {
                return opt.value();
            }

            if (auto opt = secondary.get(links_to))
            {
                return opt.value();
            }

            throw_or_abort(fmt::format("No metadata (zone[1970].tab data) for: {} ({})", name,
                                       links_to));
        }

        map::unsorted<str, str> parse_tzdata_zi_links(strbuf contents)
        {
            // Name -> Links-to
            map::unsorted<str, str> map;

            string::normalize_line_endings_inplace(contents);

            for (const auto [line_number, line] :
                 string::range::split{contents, '\n'} | range::v::enumerate{1})
            {
                auto rng = line.range();

                if (!rng.drop_front("L "))
                {
                    continue;
                }

                str links_to{rng.pop_front_while(fn::is{fn::ne{}, ' '})};
                rng.drop_front(' ');
                str name{rng.pop_front_while(fn::is{fn::ne{}, ' '})};

                if (rng)
                {
                    throw_or_abort(fmt::format("Invalid tzdata.zi, trailing characters on line: {}",
                                               line_number));
                }

                if (name.is_empty() || links_to.is_empty())
                {
                    throw_or_abort(fmt::format("Invalid tzdata.zi, invalid link on line: {}",
                                               line_number));
                }

                if (!map.insert(std::move(name), std::move(links_to)))
                {
                    throw_or_abort(fmt::format("Invalid tzdata.zi, duplicate link on line: {}",
                                               line_number));
                }
            }

            return map;
        }

        map::unsorted<str, zone_tab_metadata> parse_zone_tab(const cstrview prefix, strbuf contents)
        {
            // Name -> Metadata
            map::unsorted<str, zone_tab_metadata> map;

            string::normalize_line_endings_inplace(contents);

            for (const auto [line_number, line] :
                 string::range::split{contents, '\n'} | range::v::enumerate{1})
            {
                if (line.is_empty() || line.has_front('#'))
                {
                    continue;
                }

                str name;
                zone_tab_metadata meta;

                auto line_rng = line.range();

                // Country codes.

                for (auto rng = line_rng.pop_front_while(fn::is{fn::ne{}, '\t'}); rng;)
                {
                    meta.country_codes.append(rng.pop_front_while(fn::is{fn::ne{}, ','}).view());
                    rng.drop_front(',');
                }

                // Coordinates

                if (!line_rng.drop_front('\t'))
                {
                    throw_or_abort(fmt::format("Invalid {}.tab, expected tab on line: {}", prefix,
                                               line_number));
                }

                meta.coordinates = line_rng.pop_front_while(fn::is{fn::ne{}, '\t'}).view();

                // Name

                if (!line_rng.drop_front('\t'))
                {
                    throw_or_abort(fmt::format("Invalid {}.tab, expected tab on line: {}", prefix,
                                               line_number));
                }

                name = line_rng.pop_front_while(fn::is{fn::ne{}, '\t'}).view();

                // Comment (optional)

                if (line_rng.drop_front('\t'))
                {
                    meta.comment = line_rng.pop_front_while(fn::is{fn::ne{}, '\t'}).view();
                }

                if (line_rng)
                {
                    throw_or_abort(fmt::format("Invalid {}.tab, trailing characters on line: {}",
                                               prefix, line_number));
                }

                if (!map.insert(std::move(name), std::move(meta)))
                {
                    throw_or_abort(fmt::format("Invalid {}.tab, duplicate TZ on line: {}", prefix,
                                               line_number));
                }
            }

            return map;
        }

        strbuf read_file(str path)
        {
            auto data = file::read<strbuf>(path).value_or_default();
            if (!data)
            {
                throw_or_abort(fmt::format("Error: Empty/unreadable: {}", path));
            }
            return data;
        }
    }
}

namespace snn
{
    int main(const array_view<const env::argument> arguments)
    {
        if (arguments.count() != 3)
        {
            const auto program_name = arguments.front().value_or_default().to<cstrview>();
            fmt::print_error_line("Usage: {} version fat-zoneinfo-dir", program_name);
            fmt::print_error_line();
            fmt::print_error_line("Version is tzdb version, e.g.: 2022a");
            fmt::print_error_line("See README.md for how to build a \"fat\" zoneinfo directory.");
            return constant::exit::failure;
        }

        const auto version       = arguments.at(1).value().to<str>();
        const auto zoneinfo_path = arguments.at(2).value().to<str>();
        const str output_path    = "db-new";

        if (version.is_empty() || !version.all(chr::is_alphanumeric_lower))
        {
            fmt::print_error_line("Error: Invalid version: {}", version);
            return constant::exit::failure;
        }

        if (file::is_something(output_path))
        {
            fmt::print_error_line("Error: Output directory already exist: {}", output_path);
            return constant::exit::failure;
        }

        const auto files = app::list_recursive(zoneinfo_path);

        const auto new_zone_tab =
            app::parse_zone_tab("zone1970",
                                app::read_file(file::path::join(zoneinfo_path, "zone1970.tab")));

        const auto old_zone_tab =
            app::parse_zone_tab("zone",
                                app::read_file(file::path::join(zoneinfo_path, "zone.tab")));

        const auto links = app::parse_tzdata_zi_links(
            app::read_file(file::path::join(zoneinfo_path, "tzdata.zi")));

        // Generate a header file for each location.

        vec<str> entries;
        vec<str> include_paths;
        vec<str> locations;

        for (const auto& name : files)
        {
            if (app::ignore_name(name))
            {
                fmt::print_line("Ignoring: {}", name);
                continue;
            }

            if (!time::zone::location::is_valid_name(name))
            {
                fmt::print_error_line("Error: Invalid name: {}", name);
                return constant::exit::failure;
            }

            // Link

            str links_to;

            if (auto opt = links.get(name))
            {
                links_to = opt.value();

                if (app::ignore_link(name))
                {
                    fmt::print_line("Ignoring link: {}", name);
                    continue;
                }

                fmt::print_line("Link: {} -> {}", name, links_to);

                if (!files.contains(links_to))
                {
                    fmt::print_error_line("Error: No file for: {}", links_to);
                    return constant::exit::failure;
                }

                if (app::ignore_name(links_to))
                {
                    fmt::print_error_line("Error: {} links to ignored name: {}", name, links_to);
                    return constant::exit::failure;
                }

                if (!time::zone::location::is_valid_name(links_to))
                {
                    fmt::print_error_line("Error: Invalid name: {}", links_to);
                    return constant::exit::failure;
                }
            }

            // Metadata

            const app::zone_tab_metadata& meta =
                app::metadata(name, links_to, new_zone_tab, old_zone_tab);

            if (meta.country_codes.is_empty())
            {
                fmt::print_error_line("Error: No country codes for name: {}", name);
                return constant::exit::failure;
            }

            for (const auto& s : meta.country_codes)
            {
                if (!country::code::is_valid(s))
                {
                    fmt::print_error_line("Error: Invalid country code: {}", s);
                    return constant::exit::failure;
                }
            }

            constexpr auto is_valid_meta_character =
                fn::is_none_of{chr::is_ascii_control, fn::in_array{'\\', '"'}};

            if (!meta.comment.all(is_valid_meta_character))
            {
                fmt::print_error_line("Error: Invalid character in comment for: {}", name);
                return constant::exit::failure;
            }

            if (!meta.coordinates.all(is_valid_meta_character))
            {
                fmt::print_error_line("Error: Invalid character in coordinates for: {}", name);
                return constant::exit::failure;
            }

            // Generate

            const vec<str> identifiers = app::split_into_identifiers(name);

            entries.append(algo::join<str>(identifiers.range(), "::"));

            const str rel_include_path = app::make_relative_header_path(identifiers);
            include_paths.append(rel_include_path);

            locations.append(name);

            app::generate_header(name, links_to, meta, identifiers, rel_include_path, zoneinfo_path,
                                 output_path);
        }

        // world.hh

        strbuf hh{container::reserve, 24'000};

        hh << "// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.\n";
        hh << "// SPDX-License-Identifier: BSL-1.0\n";

        hh << '\n';

        hh << "// This file is generated, DO NOT EDIT MANUALLY.\n\n";

        hh << "// IANA Time Zone Database (public domain).\n\n";

        hh << "#pragma once\n\n";

        for (const auto& inc : include_paths)
        {
            hh << "#include \"snn-core/time/zone/db/" << inc << "\"\n";
        }

        const str world_hh_path = file::path::join(output_path, "world.hh");
        fmt::print("... {}\n", world_hh_path);
        file::write(world_hh_path, hh).or_throw();

        // locations.hh

        hh.clear();

        hh << "// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.\n";
        hh << "// SPDX-License-Identifier: BSL-1.0\n";

        hh << '\n';

        hh << "// This file is generated, DO NOT EDIT MANUALLY.\n\n";

        hh << "// IANA Time Zone Database (public domain).\n\n";

        hh << "#pragma once\n\n";

        hh << "#include \"snn-core/array.hh\"\n";
        hh << "#include \"snn-core/size_prefixed_string_literal.hh\"\n";

        hh << "\n";

        hh << "namespace snn::time::zone::db\n";
        hh << "{\n";

        hh << "    inline constexpr array<size_prefixed_string_literal, "
           << as_num(locations.count()) << "> locations{\n";
        for (const auto& name : locations)
        {
            hh << "        \"\\";
            constexpr usize min_digits = 3;
            hh.append_integral<math::base::octal>(name.size(), min_digits);
            hh << name << "\",\n";
        }
        hh << "    };\n";

        hh << "}\n";

        const str locations_hh_path = file::path::join(output_path, "locations.hh");
        fmt::print("... {}\n", locations_hh_path);
        file::write(locations_hh_path, hh).or_throw();

        // entries.hh

        hh.clear();

        hh << "// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.\n";
        hh << "// SPDX-License-Identifier: BSL-1.0\n";

        hh << '\n';

        hh << "// This file is generated, DO NOT EDIT MANUALLY.\n\n";

        hh << "// IANA Time Zone Database (public domain).\n\n";

        hh << "#pragma once\n\n";

        hh << "#include \"snn-core/array.hh\"\n";
        hh << "#include \"snn-core/time/zone/db/entry.hh\"\n";
        hh << "#include \"snn-core/time/zone/db/world.hh\"\n";

        hh << "\n";

        hh << "namespace snn::time::zone::db\n";
        hh << "{\n";

        hh << "    inline constexpr array<const entry*, " << as_num(entries.count())
           << "> entries{\n";
        for (const auto& entry : entries)
        {
            hh << "        &" << entry << ",\n";
        }
        hh << "    };\n";

        hh << "}\n";

        const str entries_hh_path = file::path::join(output_path, "entries.hh");
        fmt::print("... {}\n", entries_hh_path);
        file::write(entries_hh_path, hh).or_throw();

        // version.hh

        hh.clear();

        hh << "// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.\n";
        hh << "// SPDX-License-Identifier: BSL-1.0\n";

        hh << '\n';

        hh << "// This file is generated, DO NOT EDIT MANUALLY.\n\n";

        hh << "// IANA Time Zone Database (public domain).\n\n";

        hh << "#pragma once\n\n";

        hh << "#include \"snn-core/array_view.hh\"\n";

        hh << "\n";

        hh << "namespace snn::time::zone::db\n";
        hh << "{\n";
        hh << "    inline constexpr cstrview version{\"" << version << "\"};\n";
        hh << "}\n";

        const str version_hh_path = file::path::join(output_path, "version.hh");
        fmt::print("... {}\n", version_hh_path);
        file::write(version_hh_path, hh).or_throw();

        return constant::exit::success;
    }
}
