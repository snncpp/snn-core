// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Matches (`std::cmatch` wrapper)

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/range/forward.hh"
#include "snn-core/regex/match_view.hh"
#include <regex>

namespace snn::regex
{
    // ## Classes

    // ### matches_iterator

    class matches_iterator final
    {
      public:
        match_view operator*() const noexcept
        {
            return match_view{cmatch_, index_};
        }

        matches_iterator& operator++() noexcept
        {
            ++index_;
            return *this;
        }

        bool operator==(const matches_iterator& other) const noexcept
        {
            return index_ == other.index_;
        }

        bool operator!=(const matches_iterator& other) const noexcept
        {
            return index_ != other.index_;
        }

      private:
        const std::cmatch& cmatch_;
        usize index_;

        template <typename>
        friend class matches_base;

        explicit matches_iterator(const std::cmatch& matches, const usize index) noexcept
            : cmatch_{matches},
              index_{index}
        {
        }
    };

    // ## Classes

    // ### matches_base

    template <typename Derived>
    class matches_base
    {
      public:
        // Explicit conversion operators.

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // Single element access.

        [[nodiscard]] optional<match_view> at(const usize pos) const noexcept
        {
            if (pos < count())
            {
                return match_view{cmatch_(), pos};
            }
            return nullopt;
        }

        [[nodiscard]] match_view at(const usize pos, promise::within_bounds_t) const noexcept
        {
            snn_assert(pos < count());
            return match_view{cmatch_(), pos};
        }

        // Iterators

        [[nodiscard]] auto begin() const noexcept
        {
            return matches_iterator{cmatch_(), 0};
        }

        [[nodiscard]] auto end() const noexcept
        {
            return matches_iterator{cmatch_(), cmatch_().size()};
        }

        // Capacity

        [[nodiscard]] usize count() const noexcept
        {
            return cmatch_().size();
        }

        [[nodiscard]] bool is_empty() const noexcept
        {
            return count() == 0;
        }

        // Range

        [[nodiscard]] auto range() const noexcept
        {
            return snn::range::forward{meta::iterators, begin(), end()};
        }

      protected:
        matches_base() = default;

      private:
        const std::cmatch& cmatch_() const noexcept
        {
            return static_cast<const Derived*>(this)->cmatch();
        }
    };

    // ## Classes

    // ### matches

    class matches final : public matches_base<matches>
    {
      public:
        explicit matches()
            : cmatch_{}
        {
        }

        explicit matches(std::cmatch cmatch)
            : cmatch_{std::move(cmatch)}
        {
        }

      private:
        std::cmatch cmatch_;

        friend class matches_base<matches>;
        friend class pattern;

        std::cmatch& cmatch() noexcept
        {
            return cmatch_;
        }

        const std::cmatch& cmatch() const noexcept
        {
            return cmatch_;
        }
    };
}
