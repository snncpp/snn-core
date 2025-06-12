// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pipe for interprocess communication

// Wrapper around `pipe2()` system call.

#pragma once

#include "snn-core/result.hh"
#include "snn-core/file/descriptor.hh"
#include "snn-core/system/error.hh"
#include <cerrno>   // errno
#include <fcntl.h>  // O_CLOEXEC
#include <unistd.h> // pipe2

namespace snn::process
{
    // ## Classes

    // ### pipe

    class pipe final
    {
      public:
        // #### Types

        using trivially_relocatable_type = trivially_relocatable_if_t<pipe, file::descriptor>;

        // #### Constructor

        // Throws on error. `make()` is a non-throwing alternative.

        explicit pipe()
            : pipe{make().value()}
        {
        }

        // #### Make

        [[nodiscard]] static result<pipe> make() noexcept
        {
            int fildes[2];
            if (::pipe2(fildes, O_CLOEXEC) == 0)
            {
                return pipe{fildes, init::internal};
            }
            return error_code{errno, system::error_category};
        }

        // #### Reading end

        [[nodiscard]] file::descriptor& reading_end() noexcept
        {
            return reading_end_;
        }

        [[nodiscard]] const file::descriptor& reading_end() const noexcept
        {
            return reading_end_;
        }

        // #### Writing end

        [[nodiscard]] file::descriptor& writing_end() noexcept
        {
            return writing_end_;
        }

        [[nodiscard]] const file::descriptor& writing_end() const noexcept
        {
            return writing_end_;
        }

      private:
        file::descriptor reading_end_;
        file::descriptor writing_end_;

        explicit pipe(const int (&fildes)[2], init::internal_t) noexcept
            : reading_end_{fildes[0]},
              writing_end_{fildes[1]}
        {
        }
    };
}
