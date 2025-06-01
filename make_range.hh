// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Make range function

#pragma once

#include "snn-core/range/bidirectional.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/range/forward.hh"
#include "snn-core/range/random_access.hh"
#include <iterator> // begin, end, iterator_traits, *_iterator_tag

namespace snn
{
    // ## Functions

    // ### make_range

    template <typename It>
    [[nodiscard]] constexpr auto make_range(It first, It last, std::forward_iterator_tag) noexcept
    {
        return range::forward{meta::iterators, std::move(first), std::move(last)};
    }

    template <typename It>
    [[nodiscard]] constexpr auto make_range(It first, It last,
                                            std::bidirectional_iterator_tag) noexcept
    {
        return range::bidirectional{meta::iterators, std::move(first), std::move(last)};
    }

    template <typename It>
    [[nodiscard]] constexpr auto make_range(It first, It last,
                                            std::random_access_iterator_tag) noexcept
    {
        return range::random_access{meta::iterators, std::move(first), std::move(last)};
    }

    template <typename It>
    [[nodiscard]] constexpr auto make_range(It first, It last) noexcept
    {
        return make_range(std::move(first), std::move(last),
                          typename std::iterator_traits<It>::iterator_category{});
    }

    template <pointer Ptr>
    [[nodiscard]] constexpr auto make_range(Ptr first, Ptr last) noexcept
    {
        return range::contiguous{meta::iterators, first, last};
    }

    template <same_as<const char> ConstChar, usize N>
    [[nodiscard]] constexpr auto make_range(ConstChar (&s)[N]) noexcept
    {
        return cstrrng{s};
    }

    template <typename Container>
    [[nodiscard]] constexpr auto make_range(Container& c) noexcept
    {
        return make_range(std::begin(c), std::end(c));
    }

    template <typename Container>
    void make_range(const Container&&) = delete;
}
