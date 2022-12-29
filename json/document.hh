// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Document (pool of nodes)

#pragma once

#include "snn-core/json/node.hh"
#include "snn-core/pool/append_only.hh"

namespace snn::json
{
    // ## Classes

    // ### document

    class document final
    {
      public:
        document() = delete;

        // Non-copyable
        document(const document&)            = delete;
        document& operator=(const document&) = delete;

        // Move constructor.
        constexpr document(document&& other) noexcept = default;

        // Move assignment operator.
        document& operator=(document&&) = delete;

        ~document() = default; // "Rule of five".

        [[nodiscard]] constexpr usize node_count() const noexcept
        {
            return pool_.count();
        }

        [[nodiscard]] constexpr const node& root() const noexcept
        {
            return root_;
        }

      private:
        pool::append_only<node> pool_;
        node& root_;

        friend class decoder;

        constexpr document(pool::append_only<node>&& pool, node& root) noexcept
            : pool_{std::move(pool)},
              root_{root}
        {
        }
    };
}
