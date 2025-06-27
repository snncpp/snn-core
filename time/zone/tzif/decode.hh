// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Decode TZif

// https://datatracker.ietf.org/doc/html/rfc8536

// tzhead struct and validation logic from tzfile.h and localtime.c in (public domain):
// http://www.iana.org/time-zones/repository/releases/tzcode2015g.tar.gz

#pragma once

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/vec.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/time/error.hh"
#include "snn-core/time/zone/offset.hh"
#include "snn-core/time/zone/transition.hh"

namespace snn::time::zone::tzif
{
    // ## Functions

    // ### decode

    namespace detail
    {
        struct counts final
        {
            u32 isutc_count;
            u32 isstd_count;
            u32 leap_count;
            u32 transition_count;
            u32 offset_count;
            u32 abbr_block_size;

            static constexpr usize offset_size = 6;

            // Block sizes.

            template <signed_integral Int>
            [[nodiscard]] constexpr usize data_block_size() const noexcept
            {
                static_assert(sizeof(Int) == 4 || sizeof(Int) == 8);
                return transition_block_size<Int>() // Transitions block.
                     + transition_count             // Single byte indexes into offsets.
                     + offset_block_size()          // Offset block.
                     + abbr_block_size              // Total bytes for abbreviations.
                     + leap_block_size<Int>()       // Leap seconds block.
                     + isstd_count                  // Single byte standard/wall indicators.
                     + isutc_count;                 // Single byte UTC/local indicators.
            }

            template <signed_integral Int>
            [[nodiscard]] constexpr usize leap_block_size() const noexcept
            {
                static_assert(sizeof(Int) == 4 || sizeof(Int) == 8);
                // Occurrence(Int) + correction(u32/i32).
                constexpr usize leap_second_size = sizeof(Int) + sizeof(u32);
                return leap_count * leap_second_size;
            }

            [[nodiscard]] constexpr usize offset_block_size() const noexcept
            {
                return offset_count * offset_size;
            }

            template <signed_integral Int>
            [[nodiscard]] constexpr usize transition_block_size() const noexcept
            {
                static_assert(sizeof(Int) == 4 || sizeof(Int) == 8);
                return transition_count * sizeof(Int); // Each transition is an Int.
            }

            // Validation

            [[nodiscard]] constexpr bool is_valid() const noexcept
            {
                // Use the same limits as tzfile.h.
                constexpr u32 transitions_max     = 2'000;
                constexpr u32 offsets_max         = 256; // Limited by what u8 can hold.
                constexpr u32 abbr_block_max_size = 50;
                constexpr u32 leaps_max           = 50;

                // Use the same validation as localtime.c. Later versions of localtime.c seems to
                // only allow values less than max, so do the same here.
                return leap_count < leaps_max && offset_count > 0 && offset_count < offsets_max &&
                       transition_count < transitions_max &&
                       abbr_block_size < abbr_block_max_size &&
                       (isstd_count == offset_count || isstd_count == 0) &&
                       (isutc_count == offset_count || isutc_count == 0);
            }
        };

        struct header final
        {
            array<char, 4> magic;     // "TZif"
            char version{};           // '\0', '2' or '3'.
            array<char, 15> reserved; // Reserved, must be zero.
            array<char, 4> isutc_count;
            array<char, 4> isstd_count;
            array<char, 4> leap_count;
            array<char, 4> transition_count;
            array<char, 4> offset_count;
            array<char, 4> abbr_block_size;

            [[nodiscard]] constexpr detail::counts counts() const noexcept
            {
                return detail::counts{
                    .isutc_count      = isutc_count.view<>().load_swap<u32>(),
                    .isstd_count      = isstd_count.view<>().load_swap<u32>(),
                    .leap_count       = leap_count.view<>().load_swap<u32>(),
                    .transition_count = transition_count.view<>().load_swap<u32>(),
                    .offset_count     = offset_count.view<>().load_swap<u32>(),
                    .abbr_block_size  = abbr_block_size.view<>().load_swap<u32>(),
                };
            }

            [[nodiscard]] constexpr bool is_valid() const noexcept
            {
                return magic.view() == "TZif" &&
                       (version == '\0' || version == '2' || version == '3');
            }
        };
        static_assert(sizeof(header) == 44);

        template <signed_integral Int, typename Buf>
        constexpr result<void> parse_extract(cstrrng rng, const counts& c,
                                             vec<zone::offset>& offsets,
                                             vec<zone::transition>& transitions,
                                             strcore<Buf>& footer)
        {
            using UInt = std::make_unsigned_t<Int>;

            static_assert(sizeof(Int) == 4 || sizeof(Int) == 8);
            static_assert(sizeof(Int) == sizeof(UInt));

            if (rng.size() < c.data_block_size<Int>())
            {
                return error::invalid_tzif_data;
            }

            cstrrng transitions_rng      = rng.pop_front_n(c.transition_block_size<Int>());
            cstrrng offset_indexes_rng   = rng.pop_front_n(c.transition_count);
            cstrrng offsets_rng          = rng.pop_front_n(c.offset_block_size());
            const cstrview abbreviations = rng.pop_front_n(c.abbr_block_size).view();

            snn_should(abbreviations.size() == c.abbr_block_size);

            // Offsets
            constexpr usize offset_size = counts::offset_size;
            while (offsets_rng.size() >= offset_size)
            {
                const auto sub_rng = offsets_rng.pop_front_n(offset_size);
                const auto v       = sub_rng.view_exactly<0, offset_size>(assume::within_bounds);

                const auto offset    = to_i32(v.load_swap<u32>());
                const bool is_dst    = static_cast<bool>(v.get<4>());
                const usize abbr_pos = to_byte(v.get<5>());

                const usize zero_pos =
                    abbreviations.find_if(fn::is_zero{}, abbr_pos).value_or_npos();
                if (zero_pos != constant::npos && zero_pos > abbr_pos)
                {
                    const auto abbr = abbreviations.view(abbr_pos, zero_pos - abbr_pos);
                    if (zone::abbr::is_valid(abbr))
                    {
                        offsets.append({zone::abbr{abbr, promise::is_valid}, offset, is_dst});
                    }
                    else
                    {
                        return error::invalid_tzif_data;
                    }
                }
                else
                {
                    return error::invalid_tzif_data;
                }
            }
            snn_should(offsets_rng.is_empty());

            // Transitions.
            constexpr usize transition_size = sizeof(Int);
            while (transitions_rng.size() >= transition_size)
            {
                const auto sub_rng = transitions_rng.pop_front_n(transition_size);
                const auto v = sub_rng.view_exactly<0, transition_size>(assume::within_bounds);

                const auto when = static_cast<Int>(v.template load_swap<UInt>());

                // `offset_indexes_rng` is never empty here, but the optimizer doesn't know that.
                snn_should(offset_indexes_rng);
                if (offset_indexes_rng)
                {
                    const auto offset_index =
                        to_byte(offset_indexes_rng.pop_front(promise::not_empty));
                    if (offset_index < c.offset_count &&
                        time::zone::transition::is_valid(offset_index, when))
                    {
                        transitions.append({offset_index, when});
                    }
                    else
                    {
                        return error::invalid_tzif_data;
                    }
                }
                else
                {
                    return error::invalid_tzif_data;
                }
            }
            snn_should(transitions_rng.is_empty());
            snn_should(offset_indexes_rng.is_empty());

            // At least one offset must exist if the header/counts passed validation.
            snn_should(offsets);

            // Skip leap seconds.
            rng.pop_front_n(c.leap_block_size<Int>());

            // Skip isstd.
            rng.pop_front_n(c.isstd_count);

            // Skip isutc.
            rng.pop_front_n(c.isutc_count);

            // Footer (version 2 and 3 only).
            if (rng.drop_front('\n'))
            {
                const auto footer_rng = rng.pop_front_while(fn::is{fn::not_equal_to{}, '\n'});

                if (!rng.drop_front('\n'))
                {
                    return error::invalid_tzif_data;
                }

                if (!rng.is_empty())
                {
                    return error::invalid_tzif_data;
                }

                // Don't touch anything related to `rng` after this (in case of overlap).
                footer.append(footer_rng.view());

                return {};
            }

            // No footer.
            if (!rng.is_empty())
            {
                return error::invalid_tzif_data;
            }

            return {};
        }
    }

    template <typename Buf>
    [[nodiscard]] constexpr result<void> decode(cstrrng rng, vec<zone::offset>& offsets,
                                                vec<zone::transition>& transitions,
                                                strcore<Buf>& footer)
    {
        detail::header header;

        if (!rng.drop_front_read(header))
        {
            return error::invalid_tzif_data;
        }

        if (!header.is_valid())
        {
            return error::invalid_tzif_data;
        }

        detail::counts counts = header.counts();
        if (!counts.is_valid())
        {
            return error::invalid_tzif_data;
        }

        // Parse second set of data if possible.
        if (header.version != '\0')
        {
            // Skip the first data set.
            rng.pop_front_n(counts.data_block_size<i32>());

            if (!rng.drop_front_read(header))
            {
                return error::invalid_tzif_data;
            }

            if (!header.is_valid())
            {
                return error::invalid_tzif_data;
            }

            counts = header.counts();
            if (!counts.is_valid())
            {
                return error::invalid_tzif_data;
            }

            return detail::parse_extract<i64>(rng, counts, offsets, transitions, footer);
        }
        else
        {
            // Parse first set of data.
            return detail::parse_extract<i32>(rng, counts, offsets, transitions, footer);
        }
    }
}
