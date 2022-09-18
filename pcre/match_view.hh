// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Match view

#pragma once

#include "snn-core/array_view.hh"

namespace snn::pcre
{
    // ## Classes

    // ### match_view

    class match_view final
    {
      public:
        constexpr explicit match_view(const cstrview subject, const usize position,
                                      const usize size) noexcept
            : subject_{subject},
              position_{position},
              size_{size}
        {
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return size_;
        }

        [[nodiscard]] constexpr usize position() const noexcept
        {
            return position_;
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return size_;
        }

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return subject_.view(position_, size_);
        }

      private:
        cstrview subject_;
        usize position_;
        usize size_;
    };
}
