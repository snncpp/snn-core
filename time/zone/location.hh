// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Location

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/algo/find_greater_than.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/time/zone/db/entry.hh"

namespace snn::time::zone
{
    // ## Classes

    // ### location

    class location final
    {
      public:
        constexpr explicit location(const db::entry& entry) noexcept
            : entry_{&entry}
        {
            snn_should(!entry_->offsets.is_empty());
        }

        explicit location(const db::entry&&) = delete;

        [[nodiscard]] constexpr cstrview name() const noexcept
        {
            return entry_->name;
        }

        [[nodiscard]] constexpr cstrview comment() const noexcept
        {
            return entry_->comment;
        }

        [[nodiscard]] constexpr cstrview coordinates() const noexcept
        {
            return entry_->coordinates;
        }

        [[nodiscard]] constexpr array_view<const country::code> country_codes() const noexcept
        {
            return entry_->country_codes;
        }

        [[nodiscard]] constexpr zone::offset offset(const i64 timestamp) noexcept
        {
            const auto offsets     = entry_->offsets;
            const auto transitions = entry_->transitions;

            if (timestamp >= cache_when_ && timestamp < cache_next_) [[likely]]
            {
                return offsets.at(cache_offset_index_, promise::within_bounds);
            }

            if (transitions.is_empty()) [[unlikely]]
            {
                cache_when_         = constant::limit<i64>::min;
                cache_next_         = constant::limit<i64>::max;
                cache_offset_index_ = 0; // First offset is the fallback from tzdb-2018f and later.
                return offsets.at(cache_offset_index_, promise::within_bounds);
            }

            const auto first_transition = transitions.front(promise::not_empty);

            if (timestamp < first_transition.when) [[unlikely]]
            {
                cache_when_         = constant::limit<i64>::min;
                cache_next_         = first_transition.when;
                cache_offset_index_ = 0; // First offset is the fallback from tzdb-2018f and later.
                return offsets.at(cache_offset_index_, promise::within_bounds);
            }

            // Find the first transition that is greater than timestamp.
            usize index = algo::find_greater_than(
                              transitions.range(), timestamp,
                              [](auto time, auto t) { return time < t.when; }, assume::is_sorted)
                              .value_or_npos();

            // The first transition can't be returned at this point.
            snn_should(index > 0);

            if (index < transitions.count())
            {
                cache_next_ = transitions.at(index, promise::within_bounds).when;
            }
            else
            {
                index       = transitions.count();
                cache_next_ = constant::limit<i64>::max;
            }

            --index; // Step back to the transition that will be in effect.
            const auto t        = transitions.at(index, promise::within_bounds);
            cache_when_         = t.when;
            cache_offset_index_ = t.offset_index;
            return offsets.at(cache_offset_index_, promise::within_bounds);
        }

        [[nodiscard]] static constexpr bool is_valid_name(const transient<cstrview> name) noexcept
        {
            const cstrview s = name.get();
            if (s && s.size() < 50) // Longest in tzdb-2022a is 30 characters.
            {
                auto rng = s.range();
                while (rng.pop_front_while(chr::is_alpha))
                {
                    while (rng.drop_front_if(fn::in_array{'_', '-'}))
                    {
                        if (!rng.pop_front_while(chr::is_alpha))
                        {
                            return false;
                        }
                    }

                    if (rng.drop_front('/'))
                    {
                        if (rng.is_empty())
                        {
                            return false;
                        }
                    }
                }
                return rng.is_empty();
            }
            return false;
        }

        [[nodiscard]] constexpr i64 last_offset_from() const noexcept
        {
            return cache_when_;
        }

        [[nodiscard]] constexpr i64 last_offset_to() const noexcept
        {
            return cache_next_;
        }

      private:
        const db::entry* entry_;
        i64 cache_when_        = constant::limit<i64>::max;
        i64 cache_next_        = constant::limit<i64>::max;
        u8 cache_offset_index_ = 0;
    };
}
