// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Transient directory entry

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/file/type.hh"
#include "snn-core/string/size.hh"

namespace snn::file::dir
{
    // ## Classes

    // ### transient_entry

    class transient_entry final
    {
      public:
        // #### Explicit constructors

        constexpr explicit transient_entry(const null_term<const char*> name, const file::type type)
            : name_data_{name},
              name_size_maybe_{constant::limit<u32>::max},
              type_{type}
        {
        }

        constexpr explicit transient_entry(const null_term<const char*> name, const u16 name_size,
                                           const file::type type)
            : name_data_{name},
              name_size_maybe_{name_size},
              type_{type}
        {
            snn_should(string::size(name_data_.as_not_null(), assume::null_terminated) ==
                       name_size_maybe_);
        }

        // #### Name

        template <typename String>
        [[nodiscard]] constexpr String name() const
            noexcept(std::is_nothrow_constructible_v<String, not_null<const char*>, usize>)
        {
            return String{name_data_.as_not_null(), name_size_()};
        }

        template <same_as<null_term<const char*>> NullTermConstChar>
        [[nodiscard]] constexpr NullTermConstChar name() const noexcept
        {
            return name_data_;
        }

        // #### Type

        [[nodiscard]] constexpr bool is_block() const noexcept
        {
            return type_ == file::type::block;
        }

        [[nodiscard]] constexpr bool is_character() const noexcept
        {
            return type_ == file::type::character;
        }

        [[nodiscard]] constexpr bool is_directory() const noexcept
        {
            return type_ == file::type::directory;
        }

        [[nodiscard]] constexpr bool is_fifo() const noexcept
        {
            return type_ == file::type::fifo;
        }

        [[nodiscard]] constexpr bool is_regular() const noexcept
        {
            return type_ == file::type::regular;
        }

        [[nodiscard]] constexpr bool is_socket() const noexcept
        {
            return type_ == file::type::socket;
        }

        [[nodiscard]] constexpr bool is_symlink() const noexcept
        {
            return type_ == file::type::symlink;
        }

        [[nodiscard]] constexpr bool is_unknown() const noexcept
        {
            return type_ == file::type::unknown;
        }

        [[nodiscard]] constexpr bool is_whiteout() const noexcept
        {
            return type_ == file::type::whiteout;
        }

        [[nodiscard]] constexpr file::type type() const noexcept
        {
            return type_;
        }

      private:
        null_term<const char*> name_data_;
        u32 name_size_maybe_;
        file::type type_;

        constexpr usize name_size_() const noexcept
        {
            if (name_size_maybe_ != constant::limit<u32>::max)
            {
                return name_size_maybe_;
            }

            return string::size(name_data_.as_not_null(), assume::null_terminated);
        }
    };
}
