// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Does a range start with another range

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/range/unchecked/forward.hh"

namespace snn::algo
{
    // ## Functions

    // ### starts_with

    template <input_range SubjectRng, input_range NeedleRng, typename TwoArgPred = fn::equal_to>
    [[nodiscard]] constexpr bool starts_with(SubjectRng subject, NeedleRng needle,
                                             TwoArgPred is_equal = TwoArgPred{})
    {
        if constexpr (random_access_range<SubjectRng> && legacy_iterable<SubjectRng> &&
                      random_access_range<NeedleRng>)
        {
            if (subject.count() < needle.count())
            {
                return false;
            }

            range::unchecked::forward usubject{meta::iterators, subject.begin(), subject.end()};
            while (needle)
            {
                if (!is_equal(usubject.front(promise::not_empty), needle.front(promise::not_empty)))
                {
                    return false;
                }
                usubject.drop_front(promise::not_empty);
                needle.drop_front(promise::not_empty);
            }

            return true;
        }
        else
        {
            while (subject && needle)
            {
                if (!is_equal(subject.front(promise::not_empty), needle.front(promise::not_empty)))
                {
                    return false;
                }
                subject.drop_front(promise::not_empty);
                needle.drop_front(promise::not_empty);
            }
            return needle.is_empty();
        }
    }
}
