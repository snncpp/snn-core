// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Append iterator

#pragma once

#include "snn-core/core.hh"

namespace snn::string
{
    // ## Classes

    // ### append_iterator

    template <typename T>
    class append_iterator final
    {
      public:
        constexpr explicit append_iterator(T& s) noexcept
            : s_{&s}
        {
        }

        explicit append_iterator(const T&) = delete;

        constexpr append_iterator& operator=(const char c)
        {
            s_->append(c);
            return *this;
        }

        constexpr append_iterator& operator*() noexcept
        {
            return *this;
        }

        constexpr append_iterator& operator++() noexcept
        {
            return *this;
        }

        constexpr append_iterator operator++(int) noexcept
        {
            return *this;
        }

      private:
        T* s_; // Can't be a reference, this class must be copy assignable.
    };
}
