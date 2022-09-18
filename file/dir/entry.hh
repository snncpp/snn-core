// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Directory entry

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/file/type.hh"
#include "snn-core/file/dir/transient_entry.hh"

namespace snn::file::dir
{
    // ## Classes

    // ### entry

    struct entry final
    {
        // #### Data

        // Public data for (lazy) structured binding.

        str n;        // Name
        file::type t; // Type

        // #### Types

        using trivially_relocatable_type = trivially_relocatable_if_t<entry, str>;

        // #### Explicit constructors

        constexpr explicit entry(const cstrview name, const file::type type)
            : n{name},
              t{type}
        {
        }

        constexpr explicit entry(const transient_entry e)
            : n{e.name<str>()},
              t{e.type()}
        {
        }

        // #### Name

        [[nodiscard]] constexpr const str& name() const& noexcept
        {
            return n;
        }

        [[nodiscard]] constexpr str&& name() && noexcept
        {
            return std::move(n);
        }

        // #### Type

        [[nodiscard]] constexpr bool is_block() const noexcept
        {
            return t == file::type::block;
        }

        [[nodiscard]] constexpr bool is_character() const noexcept
        {
            return t == file::type::character;
        }

        [[nodiscard]] constexpr bool is_directory() const noexcept
        {
            return t == file::type::directory;
        }

        [[nodiscard]] constexpr bool is_fifo() const noexcept
        {
            return t == file::type::fifo;
        }

        [[nodiscard]] constexpr bool is_regular() const noexcept
        {
            return t == file::type::regular;
        }

        [[nodiscard]] constexpr bool is_socket() const noexcept
        {
            return t == file::type::socket;
        }

        [[nodiscard]] constexpr bool is_symlink() const noexcept
        {
            return t == file::type::symlink;
        }

        [[nodiscard]] constexpr bool is_unknown() const noexcept
        {
            return t == file::type::unknown;
        }

        [[nodiscard]] constexpr bool is_whiteout() const noexcept
        {
            return t == file::type::whiteout;
        }

        [[nodiscard]] constexpr file::type type() const noexcept
        {
            return t;
        }

        // #### Comparison

        constexpr bool operator==(const entry& other) const noexcept
        {
            // Comparing the type first is faster.
            return t == other.t && n == other.n;
        }

        constexpr std::strong_ordering operator<=>(const entry& other) const noexcept = default;
    };
}
