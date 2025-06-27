// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # File pointer wrapper

// The file pointer is implicitly closed on destruction but it is recommended to explicitly call
// `close()` and check the return value.

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/optional.hh"
#include "snn-core/result.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/system/error.hh"
#include <cerrno> // errno, E*
#include <cstdio> // fclose, fileno, FILE

namespace snn::file
{
    // ## Classes

    // ### pointer

    class pointer final
    {
      public:
        // #### Types

        using trivially_relocatable_type = pointer;

        // #### Constructors & assignment operators

        pointer() noexcept
            : fp_{nullptr}
        {
        }

        explicit pointer(std::FILE* const fp) noexcept
            : fp_{fp}
        {
        }

        // #### Non-copyable

        pointer(const pointer&)            = delete;
        pointer& operator=(const pointer&) = delete;

        // #### Movable

        pointer(pointer&& other) noexcept
            : fp_{std::exchange(other.fp_, nullptr)}
        {
        }

        pointer& operator=(pointer&& other) noexcept
        {
            swap(other);
            return *this;
        }

        // #### Destructor

        ~pointer()
        {
            close().discard();
        }

        // #### Explicit conversion operators

        explicit operator bool() const noexcept
        {
            return has_value();
        }

        // #### Close

        [[nodiscard]] result<void> close() noexcept
        {
            if (has_value())
            {
                const int ret = std::fclose(std::exchange(fp_, nullptr));
                if (ret == 0)
                {
                    return {};
                }
                return error_code{errno, system::error_category};
            }
            return generic::error::no_value;
        }

        // #### Descriptor

        [[nodiscard]] optional<int> integer_descriptor() const noexcept
        {
            if (has_value())
            {
                return ::fileno(fp_);
            }
            return nullopt;
        }

        // #### Swap

        void swap(pointer& other) noexcept
        {
            std::swap(fp_, other.fp_);
        }

        // #### Value

        [[nodiscard]] bool has_value() const noexcept
        {
            return fp_ != nullptr;
        }

        [[nodiscard]] std::FILE* value() const
        {
            if (has_value())
            {
                return fp_;
            }
            throw_or_abort(generic::error::no_value);
        }

        [[nodiscard]] std::FILE* value(assume::has_value_t) const noexcept
        {
            snn_should(has_value());
            return fp_;
        }

        [[nodiscard]] std::FILE* value_or(std::FILE* const alt) const noexcept
        {
            if (has_value())
            {
                return fp_;
            }
            return alt;
        }

      private:
        std::FILE* fp_;
    };

    inline void swap(pointer& a, pointer& b) noexcept
    {
        a.swap(b);
    }
}
