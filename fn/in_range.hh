// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # In-range (binary search)

// Check if a value is in a sorted range via binary search.

#pragma once

#include "snn-core/array.hh"
#include "snn-core/algo/find_greater_than_or_equal_to.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::fn
{
    // ## Classes

    // ### in_range

    template <random_access_range RandomAccessRng>
    class in_range final
    {
      public:
        constexpr explicit in_range(RandomAccessRng rng, assume::is_sorted_t) noexcept
            : rng_{std::move(rng)}
        {
        }

        template <typename T, usize Count>
        constexpr explicit in_range(const array<T, Count>& arr, assume::is_sorted_t) noexcept
            : rng_{arr.range()}
        {
        }

        template <typename T, usize Count>
        explicit in_range(const array<T, Count>&&,
                          assume::is_sorted_t) = delete; // Temporary, use range() if safe.

        template <typename V>
        [[nodiscard]] constexpr bool operator()(const V& v) const
        {
            const usize index =
                algo::find_greater_than_or_equal_to(rng_, v, assume::is_sorted).value_or_npos();
            if (index < rng_.count())
            {
                return rng_.at(index, promise::within_bounds) == v;
            }
            return false;
        }

      private:
        RandomAccessRng rng_;
    };

    // Deduction guides.
    template <typename T, usize Count>
    in_range(const array<T, Count>&, assume::is_sorted_t) -> in_range<range::contiguous<const T*>>;
}
