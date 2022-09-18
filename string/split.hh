// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Split string

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/vec.hh"

namespace snn::string
{
    // ## Enums

    // ### empty_elements

    enum class empty_elements : u8
    {
        discard,
        keep,
    };

    // ## Functions

    // ### split

    // If subject or delimiter is empty no action will be performed.
    // If limit is greater than 0, a maximum of limit elements will be added to the container,
    // with the last element containing the rest of the subject.
    // If limit is 1 a single element with the entire subject will be added to the container.
    // UTF-8 safe if both subject and delimiter are valid UTF-8.

    template <has_append_inplace Container>
    constexpr void split(const cstrview subject, const transient<cstrview> delimiter,
                         const empty_elements policy, const usize limit, Container& parts,
                         promise::no_overlap_t)
    {
        const usize subject_size   = subject.size();
        const usize delimiter_size = delimiter.get().size();
        if (subject_size > 0 && delimiter_size > 0)
        {
            usize limit_count = 1;
            usize start_pos   = 0;
            usize pos         = 0;

            do
            {
                if (limit_count == limit)
                {
                    parts.append_inplace(subject.view(start_pos));
                    break;
                }

                pos = subject.find(delimiter, start_pos).value_or_npos();
                if (pos != constant::npos)
                {
                    if (pos > start_pos)
                    {
                        parts.append_inplace(subject.view(start_pos, pos - start_pos));
                        ++limit_count;
                    }
                    else if (policy == empty_elements::keep)
                    {
                        parts.append_inplace();
                        ++limit_count;
                    }

                    start_pos = pos + delimiter_size;
                }
                else
                {
                    parts.append_inplace(subject.view(start_pos));
                    break;
                }
            } while (start_pos < subject_size);

            if (policy == empty_elements::keep && start_pos == subject_size && limit_count != limit)
            {
                parts.append_inplace();
            }
        }
    }

    template <typename String = str>
    [[nodiscard]] constexpr vec<String> split(const cstrview subject,
                                              const transient<cstrview> delimiter,
                                              const empty_elements policy = empty_elements::keep,
                                              const usize limit           = 0)
    {
        vec<String> parts;
        split(subject, delimiter, policy, limit, parts, promise::no_overlap);
        return parts;
    }

    // If subject is empty the returned vector will be empty.
    // UTF-8 safe if both subject and delimiter are valid UTF-8.

    template <has_append_inplace Container, character Char>
    constexpr void split(const cstrview subject, const Char delimiter, Container& parts,
                         promise::no_overlap_t)
    {
        if (subject)
        {
            usize start_pos = 0;
            do
            {
                snn_should(start_pos < subject.size());
                const usize pos = subject.find(delimiter, start_pos).value_or_npos();
                if (pos != constant::npos)
                {
                    snn_should(pos >= start_pos);
                    parts.append_inplace(subject.view(start_pos, pos - start_pos));
                    start_pos = pos + 1;
                }
                else
                {
                    parts.append_inplace(subject.view(start_pos));
                    break;
                }
            } while (start_pos < subject.size());

            if (start_pos == subject.size())
            {
                parts.append_inplace();
            }
        }
    }

    template <typename String = str, character Char>
    [[nodiscard]] constexpr vec<String> split(const cstrview subject, const Char delimiter)
    {
        vec<String> parts;
        split(subject, delimiter, parts, promise::no_overlap);
        return parts;
    }
}
