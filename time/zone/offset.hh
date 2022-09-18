// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Offset

#pragma once

#include "snn-core/time/zone/abbreviation.hh"

namespace snn::time::zone
{
    // ## Classes

    // ### offset

    class offset final
    {
      public:
        // #### Converting constructors

        constexpr offset(const zone::abbreviation abbr, const i32 seconds,
                         const bool is_dst) noexcept
            : seconds_{seconds},
              abbr_{abbr},
              is_dst_{is_dst}
        {
        }

        // #### Observers

        [[nodiscard]] constexpr zone::abbreviation abbreviation() const noexcept
        {
            return abbr_;
        }

        [[nodiscard]] constexpr zone::abbreviation abbr() const noexcept
        {
            return abbr_;
        }

        [[nodiscard]] constexpr bool is_daylight_saving_time() const noexcept
        {
            return is_dst_;
        }

        [[nodiscard]] constexpr bool is_dst() const noexcept
        {
            return is_dst_;
        }

        [[nodiscard]] constexpr i32 seconds() const noexcept
        {
            return seconds_;
        }

        // #### Factories

        [[nodiscard]] static consteval offset utc() noexcept
        {
            return offset{zone::abbreviation::utc(), 0, false};
        }

      private:
        i32 seconds_;
        zone::abbreviation abbr_;
        bool is_dst_;
    };
}
