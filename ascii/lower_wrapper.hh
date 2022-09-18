// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Lower wrapper and `as_lower()` function

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/chr/common.hh"

namespace snn::ascii
{
    // ## Classes

    // ### lower_wrapper

    template <typename CharView>
    class lower_wrapper final
    {
      public:
        template <character Char>
        constexpr explicit lower_wrapper(array_view<Char> s) noexcept
            : s_{s}
        {
            snn_should(s_.none(chr::is_alpha_upper));
        }

        [[nodiscard]] constexpr auto get() noexcept
        {
            return s_;
        }

        [[nodiscard]] constexpr auto get() const noexcept
        {
            return s_.view();
        }

      private:
        CharView s_;
    };

    // ## Functions

    // ### as_lower

    template <character Char>
    [[nodiscard]] constexpr auto as_lower(array_view<Char> s) noexcept
    {
        return lower_wrapper<array_view<Char>>{s};
    }

    template <typename CharView>
    [[nodiscard]] constexpr auto as_lower(not_deduced_t<CharView> s) noexcept
    {
        return lower_wrapper<CharView>{s};
    }

    template <same_as<const char> ConstChar, usize N>
    [[nodiscard]] constexpr auto as_lower(ConstChar (&string)[N]) noexcept
    {
        using CharView = array_view<ConstChar>;
        return lower_wrapper<CharView>{CharView{string}};
    }
}
