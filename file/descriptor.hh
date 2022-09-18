// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # File descriptor wrapper

// The descriptor is implicitly closed on destruction but it is recommended to explicitly call
// `close()` and check the return value.

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/result.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/math/common.hh"
#include "snn-core/system/error.hh"
#include <cerrno>   // errno, E*
#include <unistd.h> // close

namespace snn::file
{
    // ## Classes

    // ### descriptor

    class descriptor final
    {
      public:
        // #### Types

        using trivially_relocatable_type = descriptor;

        // #### Constructors & assignment operators

        explicit descriptor() noexcept
            : fd_{-1}
        {
        }

        explicit descriptor(const int fd) noexcept
            : fd_{math::max(fd, -1)}
        {
        }

        // Non-copyable

        descriptor(const descriptor&)            = delete;
        descriptor& operator=(const descriptor&) = delete;

        // Movable

        descriptor(descriptor&& other) noexcept
            : fd_{std::exchange(other.fd_, -1)}
        {
        }

        descriptor& operator=(descriptor&& other) noexcept
        {
            std::swap(fd_, other.fd_);
            return *this;
        }

        // #### Destructor

        ~descriptor()
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
                const int ret = ::close(std::exchange(fd_, -1));
                if (ret == 0)
                {
                    return {};
                }
                snn_should(errno != EBADF); // Programmer error.
                // Note: `close()` should never be retried on EINTR (FreeBSD/Linux),
                // see `man 2 close`.
                return error_code{errno, system::error_category};
            }
            return generic::error::no_value;
        }

        // #### Swap

        void swap(descriptor& other) noexcept
        {
            std::swap(fd_, other.fd_);
        }

        // #### Value

        [[nodiscard]] bool has_value() const noexcept
        {
            return fd_ != -1;
        }

        [[nodiscard]] int value() const
        {
            if (has_value())
            {
                return fd_;
            }
            throw_or_abort(generic::error::no_value);
        }

        [[nodiscard]] int value(promise::has_value_t) const noexcept
        {
            snn_should(has_value());
            return fd_;
        }

        [[nodiscard]] int value_or(const int alt) const noexcept
        {
            if (has_value())
            {
                return fd_;
            }
            return alt;
        }

      private:
        int fd_;
    };
}
