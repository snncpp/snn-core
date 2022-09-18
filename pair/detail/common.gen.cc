// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/array.hh"
#include "snn-core/main.hh"
#include "snn-core/strcore.hh"
#include "snn-core/ascii/pad.hh"
#include "snn-core/file/write.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/time/point.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        static constexpr auto pairs = to_array<pair::first_second<cstrview>>({
            {"first", "last"},
            {"first", "second"},
            {"index", "value"},
            {"key", "value"},
            {"line", "column"},
            {"name", "value"},
            {"value", "count"},
        });

        strbuf hh{container::reserve, 24 * constant::size::kibibyte<usize>};

        hh << "// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.\n";
        hh << "// SPDX-License-Identifier: BSL-1.0\n";

        hh << "\n";
        hh << "// This file is generated, DO NOT EDIT MANUALLY.\n\n";

        hh << "// # Common pairs\n\n";

        for (const auto [a_name, b_name] : pairs)
        {
            const str name = concat(a_name, '_', b_name);
            hh << "// `" << name << "`\n";
        }
        hh << "\n";

        hh << "// Const members tend to break \"sane\" requirements (`sane` concept) and are "
              "disabled.\n";
        hh << "// Use a wrapper like `semi_const` if const-like behavior is needed.\n\n";

        hh << "// Does not propagate const (breaks structured binding declarations).\n\n";

        hh << "#pragma once\n\n";

        hh << "#include \"snn-core/pair/core.hh\"\n";

        hh << "\n";

        hh << "namespace snn::pair\n";
        hh << "{\n";

        hh << "    // ## Classes\n\n";

        for (const auto [a_name, b_name] : pairs)
        {
            const str name = concat(a_name, '_', b_name);

            const usize max_size = math::max(a_name.size() + string_size("_type"),
                                             b_name.size() + string_size("_type"));
            const str a_type     = ascii::pad_right(concat(a_name, "_type"), max_size, ' ');
            const str b_type     = ascii::pad_right(concat(b_name, "_type"), max_size, ' ');

            hh << "    // ### " << name << "\n\n";

            hh << "    template <typename A, typename B = A>\n";
            hh << "        requires(meta::none_v<std::is_const, A, B>)\n";
            hh << "    struct " << name << " final\n";
            hh << "    {\n";
            hh << "        // #### Data\n\n";

            hh << "        A " << a_name << ";\n";
            hh << "        B " << b_name << ";\n\n";

            hh << "        // #### Types\n\n";

            hh << "        // For `" << name << "` only.\n\n";

            hh << "        using " << a_type << " = A;\n";
            hh << "        using " << b_type << " = B;\n\n";

            hh << "        // For all pairs.\n\n";

            hh << "        using a_type = A;\n";
            hh << "        using b_type = B;\n\n";

            hh << "        using trivially_relocatable_type = trivially_relocatable_if_t<" << name
               << ", A, B>;\n\n";

            hh << "        // #### Default constructor\n\n";

            hh << "        // Conditionally explicit.\n\n";

            hh << "        // clang-format off\n";
            hh << "        explicit(explicitly_default_constructible<A> || "
                  "explicitly_default_constructible<B>)\n";
            hh << "        constexpr " << name
               << "() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)\n";
            hh << "            requires(meta::all_v<std::is_default_constructible, A, B>)\n";
            hh << "            : " << a_name << "{},\n";
            hh << "              " << b_name << "{}\n";
            hh << "        {\n";
            hh << "        }\n";
            hh << "        // clang-format on\n\n";

            hh << "        // #### Converting constructors\n\n";

            hh << "        constexpr " << name << "(A a, B b) noexcept\n";
            hh << "            : " << a_name << "{std::forward<A>(a)},\n";
            hh << "              " << b_name << "{std::forward<B>(b)}\n";
            hh << "        {\n";
            hh << "        }\n\n";

            hh << "        // Prevent lvalue references from binding to rvalues.\n\n";

            hh << "        // clang-format off\n";
            hh << "        template <typename T>\n";
            hh << "        " << name << "(const std::remove_reference_t<A>&&, T&&)\n";
            hh << "            requires(std::is_lvalue_reference_v<A>) = delete;\n\n";

            hh << "        template <typename T>\n";
            hh << "        " << name << "(T&&, const std::remove_reference_t<B>&&)\n";
            hh << "            requires(std::is_lvalue_reference_v<B>) = delete;\n";
            hh << "        // clang-format on\n\n";

            hh << "        // #### get\n\n";

            hh << "        template <usize Index>\n";
            hh << "            requires(Index < 2)\n";
            hh << "        [[nodiscard]] constexpr decltype(auto) get() & noexcept\n";
            hh << "        {\n";
            hh << "            if constexpr (Index == 0)\n";
            hh << "                return std::forward<A&>(" << a_name << ");\n";
            hh << "            else\n";
            hh << "                return std::forward<B&>(" << b_name << ");\n";
            hh << "        }\n\n";

            hh << "        template <usize Index>\n";
            hh << "            requires(Index < 2)\n";
            hh << "        [[nodiscard]] constexpr decltype(auto) get() const& noexcept\n";
            hh << "        {\n";
            hh << "            if constexpr (Index == 0)\n";
            hh << "                return std::forward<const A&>(" << a_name << ");\n";
            hh << "            else\n";
            hh << "                return std::forward<const B&>(" << b_name << ");\n";
            hh << "        }\n\n";

            hh << "        template <usize Index>\n";
            hh << "            requires(Index < 2)\n";
            hh << "        [[nodiscard]] constexpr decltype(auto) get() && noexcept\n";
            hh << "        {\n";
            hh << "            if constexpr (Index == 0)\n";
            hh << "                return std::forward<A>(" << a_name << ");\n";
            hh << "            else\n";
            hh << "                return std::forward<B>(" << b_name << ");\n";
            hh << "        }\n\n";

            hh << "        template <usize Index>\n";
            hh << "            requires(Index < 2)\n";
            hh << "        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept\n";
            hh << "        {\n";
            hh << "            if constexpr (Index == 0)\n";
            hh << "                return std::forward<const A>(" << a_name << ");\n";
            hh << "            else\n";
            hh << "                return std::forward<const B>(" << b_name << ");\n";
            hh << "        }\n";
            hh << "    };\n\n";

            hh << "    // #### Specializations\n\n";

            hh << "    template <typename A, typename B>\n";
            hh << "    struct is_any_strict<" << name << "<A, B>> : public std::true_type\n";
            hh << "    {\n";
            hh << "    };\n\n";
        }

        hh.drop_back_n(string_size("\n"));

        hh << "}\n";

        file::write("common.hh", hh).or_throw();

        return constant::exit::success;
    }
}
