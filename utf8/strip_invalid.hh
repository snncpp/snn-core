// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Strip invalid bytes

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/utf8/skip.hh"

namespace snn::utf8
{
    // ## Functions

    // ### strip_invalid

    namespace detail
    {
        template <typename Buf>
        constexpr void strip_invalid(const cstrview s, strcore<Buf>& append_to)
        {
            append_to.reserve_append(s.size()); // Approximate

            auto rng = s.range();
            while (rng)
            {
                const auto initial_rng = rng;
                rng                    = utf8::skip(rng);
                const auto skipped_rng = initial_rng.without_suffix(rng, promise::is_valid);
                if (skipped_rng)
                {
                    append_to.append(skipped_rng.view());
                }

                if (rng)
                {
                    // Invalid
                    rng.drop_front(promise::not_empty);
                }
            }
        }
    }

    template <typename Buf>
    constexpr void strip_invalid(const transient<cstrview> s, strcore<Buf>& append_to,
                                 assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::strip_invalid(s.get(), append_to);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str strip_invalid(const transient<cstrview> s)
    {
        Str append_to;
        detail::strip_invalid(s.get(), append_to);
        return append_to;
    }
}
