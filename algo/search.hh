// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Search a range for another range

#pragma once

#include "snn-core/val_or_ref.hh"
#include "snn-core/algo/starts_with.hh"
#include "snn-core/fn/common.hh"
#include <algorithm> // search

namespace snn::algo
{
    // ## Functions

    // ### search

    // Search subject range for needle range.
    // Returns subject range suffix starting from needle.
    // If the needle range is empty the entire subject range is returned.
    // If the needle range is not found an empty subject range is returned.

    template <forward_range SubjectRng, forward_range NeedleRng, typename TwoArgPred = fn::equal_to>
    [[nodiscard]] constexpr SubjectRng search(SubjectRng subject, NeedleRng needle,
                                              TwoArgPred is_equal = TwoArgPred{})
    {
        if constexpr (legacy_iterable<SubjectRng> && constructible_from_iterators<SubjectRng> &&
                      legacy_iterable<NeedleRng>)
        {
            const auto it = std::search(subject.begin(), subject.end(), needle.begin(),
                                        needle.end(), std::move(is_equal));
            return SubjectRng{init::from, it, subject.end()};
        }
        else
        {
            if (needle)
            {
                const val_or_ref needle_front{needle.front(promise::not_empty)};
                needle.drop_front(promise::not_empty);

                while (true)
                {
                    // Search for needle_front.
                    while (subject &&
                           !is_equal(subject.front(promise::not_empty), needle_front.get()))
                    {
                        subject.drop_front(promise::not_empty);
                    }

                    const auto remaining = subject;
                    if (subject)
                    {
                        // Drop element that matched "needle_front".
                        subject.drop_front(promise::not_empty);

                        if (starts_with(subject, needle, is_equal))
                        {
                            return remaining;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            return subject;
        }
    }
}
