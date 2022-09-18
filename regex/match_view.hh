// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Match view

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/math/common.hh"
#include <regex>

namespace snn::regex
{
    // ## Classes

    // ### match_view

    class match_view final
    {
      public:
        [[nodiscard]] usize count() const
        {
            return math::within_bounds_or<usize>(matches_.length(index_), 0);
        }

        [[nodiscard]] usize position() const
        {
            return math::within_bounds_or<usize>(matches_.position(index_), 0);
        }

        [[nodiscard]] cstrview view() const
        {
            const std::csub_match& sub = matches_[index_];
            snn_should(sub.matched);
            return cstrview{meta::iterators, sub.first, sub.second};
        }

        [[nodiscard]] usize size() const
        {
            return count();
        }

      private:
        const std::cmatch& matches_;
        usize index_;

        template <typename>
        friend class matches_base;

        friend class matches_iterator;

        explicit match_view(const std::cmatch& matches, const usize index) noexcept
            : matches_{matches},
              index_{index}
        {
            snn_should(index_ < matches_.size());
        }
    };
}
