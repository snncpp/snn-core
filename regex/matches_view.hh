// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Matches view

#pragma once

#include "snn-core/regex/matches.hh"

namespace snn::regex
{
    namespace range
    {
        // Forward declare.
        class match;
    }

    // ## Classes

    // ### matches_view

    class matches_view final : public matches_base<matches_view>
    {
      private:
        const std::cmatch& cmatch_;

        friend class matches_base<matches_view>;
        friend class pattern;
        friend class range::match;

        explicit matches_view(const std::cmatch& cmatch)
            : cmatch_{cmatch}
        {
        }

        explicit matches_view(const std::cmatch&& cmatch) = delete;

        const std::cmatch& cmatch() const noexcept
        {
            return cmatch_;
        }
    };
}
