// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Replace invalid bytes

// Invalid bytes are replaced with U+FFFD: �

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/utf8/skip.hh"
#include "snn-core/utf8/core.hh"

namespace snn::utf8
{
    // ## Functions

    // ### replace_invalid

    namespace detail
    {
        template <typename Buf>
        constexpr void replace_invalid(const cstrview s, strcore<Buf>& append_to)
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
                    append_to.append(utf8::replacement_string());
                }
            }
        }
    }

    template <typename Buf>
    constexpr void replace_invalid(const transient<cstrview> s, strcore<Buf>& append_to,
                                   assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::replace_invalid(s.get(), append_to);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str replace_invalid(const transient<cstrview> s)
    {
        Str append_to;
        detail::replace_invalid(s.get(), append_to);
        return append_to;
    }
}
