// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encode string

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/chr/fn/lookup.hh"
#include "snn-core/utf8/is_valid.hh"

namespace snn::html
{
    namespace detail
    {
        // Generated with: detail/encode.gen.cc
        inline constexpr array<bool, 256> encode_unreserved_lookup{
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        };
        inline constexpr array<bool, 256> encode_unreserved_attribute_lookup{
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        };
        inline constexpr array<bool, 256> encode_unreserved_text_lookup{
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        };

        template <typename Buf>
        constexpr void encode(const cstrview s, strcore<Buf>& append_to,
                              const array<bool, 256>& unreserved_lookup)
        {
            append_to.reserve_append(s.size()); // Reserve minimal size.

            const auto is_unreserved = snn::chr::fn::lookup{unreserved_lookup};

            auto rng = s.range();
            while (rng)
            {
                const auto unreserved_rng = rng.pop_front_while(is_unreserved);
                if (unreserved_rng)
                {
                    append_to.append(cstrview{unreserved_rng});
                }

                if (rng)
                {
                    const char c = rng.pop_front(assume::not_empty);

                    snn_should(!is_unreserved(c));

                    switch (c)
                    {
                        case '"':
                            append_to << "&quot;";
                            break;

                        case '\'':
                            append_to << "&#39;"; // Better supported than "&apos;".
                            break;

                        case '&':
                            append_to << "&amp;";
                            break;

                        case '<':
                            append_to << "&lt;";
                            break;

                        case '>':
                            append_to << "&gt;";
                            break;

                        // "No(n)-Break Space" U+00A0 (UTF-8 encoding: 0xC2 0xA0).
                        // Only for strict HTML serialization (which requires UTF-8).
                        case to_char(0xC2):
                            if (rng.drop_front(to_char(0xA0)))
                            {
                                append_to << "&nbsp;";
                            }
                            else
                            {
                                append_to << c;
                            }
                            break;

                        default:
                            snn_should(false && "Lookup table is incorrect.");
                            break;
                    }
                }
            }
        }
    }

    // ## Functions

    // ### encode

    // Encodes 5 characters: < > & " '
    // Supports both HTML and XML.

    template <typename Buf>
    constexpr void encode(const transient<cstrview> s, strcore<Buf>& append_to,
                          assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), append_to, detail::encode_unreserved_lookup);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> s)
    {
        Str append_to;
        detail::encode(s.get(), append_to, detail::encode_unreserved_lookup);
        return append_to;
    }

    // The following functions are for strict HTML serialization:
    // https://html.spec.whatwg.org/multipage/parsing.html#escapingString

    // These functions expect the input string to be encoded in UTF-8.

    // ### encode_attribute_value

    // Encodes 3 characters: & " U+00A0

    template <typename Buf>
    constexpr void encode_attribute_value(const transient<cstrview> s, strcore<Buf>& append_to,
                                          assume::is_utf8_t, assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        snn_should_if_not_fuzzing(utf8::is_valid(s));
        detail::encode(s.get(), append_to, detail::encode_unreserved_attribute_lookup);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode_attribute_value(const transient<cstrview> s,
                                                       assume::is_utf8_t)
    {
        snn_should_if_not_fuzzing(utf8::is_valid(s));
        Str append_to;
        detail::encode(s.get(), append_to, detail::encode_unreserved_attribute_lookup);
        return append_to;
    }

    // ### encode_text_value

    // Encodes 4 characters: < > & U+00A0

    template <typename Buf>
    constexpr void encode_text_value(const transient<cstrview> s, strcore<Buf>& append_to,
                                     assume::is_utf8_t, assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        snn_should_if_not_fuzzing(utf8::is_valid(s));
        detail::encode(s.get(), append_to, detail::encode_unreserved_text_lookup);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode_text_value(const transient<cstrview> s, assume::is_utf8_t)
    {
        snn_should_if_not_fuzzing(utf8::is_valid(s));
        Str append_to;
        detail::encode(s.get(), append_to, detail::encode_unreserved_text_lookup);
        return append_to;
    }
}
