// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Split path into directory, base and extension

#pragma once

#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/system/error.hh"
#include <cerrno>  // ENAMETOOLONG
#include <climits> // PATH_MAX

namespace snn::file::path
{
    // ## Classes

    // ### parts

    template <typename String>
    class parts final
    {
      public:
        // #### Types

        using trivially_relocatable_type = trivially_relocatable_if_t<parts, String>;

        // #### Constructors

        // ##### Default

        constexpr parts() noexcept(std::is_nothrow_default_constructible_v<String>)
            : combined_{},
              base_pos_{},
              ext_pos_{}
        {
        }

        // ##### Explicit

        constexpr explicit parts(String&& combined, const u32 base_pos, const u32 ext_pos) noexcept
            : combined_{std::move(combined)},
              base_pos_{base_pos},
              ext_pos_{ext_pos}
        {
        }

        // #### Get

        template <usize Index>
            requires(Index < 3)
        [[nodiscard]] constexpr cstrview get() const noexcept
        {
            if constexpr (Index == 0)
                return directory();
            else if constexpr (Index == 1)
                return base();
            else
                return extension();
        }

        // #### Parts

        [[nodiscard]] constexpr cstrview directory() const noexcept
        {
            return combined_.view(0, base_pos_);
        }

        [[nodiscard]] constexpr cstrview base() const noexcept
        {
            return combined_.view(base_pos_, ext_pos_ - base_pos_);
        }

        [[nodiscard]] constexpr cstrview extension() const noexcept
        {
            return combined_.view(ext_pos_);
        }

        // #### Combined

        [[nodiscard]] constexpr const String& combined() const noexcept
        {
            return combined_;
        }

      private:
        String combined_;
        u32 base_pos_;
        u32 ext_pos_;
    };

    // ## Functions

    // ### get

    template <usize Index, typename String>
        requires(Index < 3)
    [[nodiscard]] constexpr cstrview get(const parts<String>& p) noexcept
    {
        return p.template get<Index>();
    }

    // ### split

    template <typename String = str>
    [[nodiscard]] constexpr result<parts<String>> split(const cstrview path)
        noexcept(std::is_nothrow_constructible_v<String, cstrview>)
    {
        usize base_pos = 0;
        usize ext_pos  = 0;

        if (path)
        {
            // PATH_MAX is 1024 in FreeBSD and 4096 in Linux (POSIX min: 256).
            static_assert(PATH_MAX >= 1024 && PATH_MAX < constant::limit<u32>::max);
            // "PATH_MAX - Maximum number of bytes the implementation will store as a pathname in a
            // user-supplied buffer of unspecified size, including the terminating null character."
            // https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/limits.h.html
            if (path.size() >= PATH_MAX) // PATH_MAX includes trailing zero byte.
            {
                return error_code{ENAMETOOLONG, system::error_category};
            }

            if (path.contains('\0'))
            {
                return generic::error::unexpected_null_character;
            }

            base_pos = path.find_in_reverse('/').value_or_npos();
            if (base_pos != constant::npos)
            {
                ++base_pos; // Skip the slash.
            }
            else
            {
                base_pos = 0;
            }

            ext_pos = path.view(base_pos).find_in_reverse('.').value_or_npos();
            if (ext_pos != constant::npos)
            {
                if (ext_pos > 0)
                {
                    ext_pos += base_pos;
                }
                else
                {
                    ext_pos = constant::npos;
                }
            }
        }

        return parts<String>{String{path}, static_cast<u32>(base_pos), static_cast<u32>(ext_pos)};
    }
}

// ## Specializations

// ### std::tuple_element

template <std::size_t Index, typename String>
    requires(Index < 3)
struct std::tuple_element<Index, snn::file::path::parts<String>>
{
    using type = snn::cstrview;
};

// ### std::tuple_size

template <typename String>
struct std::tuple_size<snn::file::path::parts<String>>
    : public std::integral_constant<std::size_t, 3>
{
};
