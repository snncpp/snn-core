// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Process spawner

// If the `path` argument does not contain a slash "/" the filename is looked for in the directories
// specified in the `PATH` environment variable (or a default set of directories if not set). See
// `man 3 execvp` for more information.

#pragma once

#include "snn-core/defer.hh"
#include "snn-core/exception.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/vec.hh"
#include "snn-core/file/descriptor.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/system/error.hh"
#include <cerrno>      // errno, E*
#include <signal.h>    // sigemptyset, sigfillset, sigset_t
#include <spawn.h>     // posix_spawn*
#include <unistd.h>    // STD*_FILENO
#include <sys/types.h> // waitpid
#include <sys/wait.h>  // waitpid, W*

extern char** environ;

namespace snn::process
{
    // ## Classes

    // ### termination_status

    class termination_status final
    {
      public:
        // Only one of `with_exit_status()` and `by_signal()` will return `true`.

        [[nodiscard]] bool with_exit_status() const noexcept
        {
            return WIFEXITED(status_);
        }

        // If `with_exit_status()` returned `true`, get the exit status returned by `main()` or
        // passed to `exit()`.

        [[nodiscard]] int exit_status() const noexcept
        {
            return WEXITSTATUS(status_);
        }

        // Only one of `by_signal()` and `with_exit_status()` will return `true`.

        [[nodiscard]] bool by_signal() const noexcept
        {
            return WIFSIGNALED(status_);
        }

        // If `by_signal()` returned `true`, get the signal number that terminated the process.

        [[nodiscard]] int signal_number() const noexcept
        {
            return WTERMSIG(status_);
        }

      private:
        int status_;

        friend class handle;

        explicit termination_status(const int status) noexcept
            : status_{status}
        {
        }
    };

    // ### handle

    // Wrapper around a process id, will wait for the process to terminate on destruction.

    class handle final
    {
      public:
        // #### Non-copyable

        handle(const handle&)            = delete;
        handle& operator=(const handle&) = delete;

        // #### Movable

        handle(handle&& other) noexcept
            : pid_{std::exchange(other.pid_, -1)}
        {
        }

        handle& operator=(handle&& other) noexcept
        {
            std::swap(pid_, other.pid_);
            return *this;
        }

        // #### Destructor

        ~handle()
        {
            if (has_value())
            {
                wait().discard();
            }
        }

        // #### Explicit conversion operators

        explicit operator bool() const noexcept
        {
            return has_value();
        }

        // #### Value (process id)

        [[nodiscard]] bool has_value() const noexcept
        {
            return pid_ != -1;
        }

        [[nodiscard]] pid_t value() const
        {
            if (has_value())
            {
                return pid_;
            }
            throw_or_abort(generic::error::no_value);
        }

        [[nodiscard]] pid_t value(promise::has_value_t) const noexcept
        {
            snn_should(has_value());
            return pid_;
        }

        [[nodiscard]] pid_t value_or(const pid_t alt) const noexcept
        {
            if (has_value())
            {
                return pid_;
            }
            return alt;
        }

        // #### Wait

        [[nodiscard]] result<termination_status> wait() noexcept
        {
            if (has_value())
            {
                int status = 0;
                pid_t pid  = 0;

                do
                {
                    pid = ::waitpid(pid_, &status, 0);
                } while (pid == -1 && errno == EINTR);

                if (pid > 0)
                {
                    snn_should(pid == pid_);
                    pid_ = -1;
                    return termination_status{status};
                }

                snn_should(pid == -1); // 0 is only returned with WNOHANG option.
                return error_code{errno, system::error_category};
            }

            // No process id to wait for.
            return error_code{ECHILD, system::error_category};
        }

      private:
        pid_t pid_;

        friend class spawner;

        // #### Constructor

        explicit handle(const pid_t pid) noexcept
            : pid_{pid}
        {
            snn_should(pid_ != -1);
        }
    };

    // ### spawner

    class spawner final
    {
      public:
        // #### Constructors

        explicit spawner(str path) noexcept
            : path_{std::move(path)}
        {
        }

        explicit spawner(str path, vec<str> arguments) noexcept
            : path_{std::move(path)},
              arguments_{std::move(arguments)}
        {
        }

        // #### Non-copyable

        spawner(const spawner&)            = delete;
        spawner& operator=(const spawner&) = delete;

        // #### Non-movable

        spawner(spawner&&)            = delete;
        spawner& operator=(spawner&&) = delete;

        // #### Arguments

        [[nodiscard]] vec<str>& arguments() noexcept
        {
            return arguments_;
        }

        [[nodiscard]] const vec<str>& arguments() const noexcept
        {
            return arguments_;
        }

        // #### File descriptors

        [[nodiscard]] file::descriptor& standard_in() noexcept
        {
            return standard_in_;
        }

        [[nodiscard]] const file::descriptor& standard_in() const noexcept
        {
            return standard_in_;
        }

        [[nodiscard]] file::descriptor& standard_out() noexcept
        {
            return standard_out_;
        }

        [[nodiscard]] const file::descriptor& standard_out() const noexcept
        {
            return standard_out_;
        }

        [[nodiscard]] file::descriptor& standard_error() noexcept
        {
            return standard_error_;
        }

        [[nodiscard]] const file::descriptor& standard_error() const noexcept
        {
            return standard_error_;
        }

        void set_standard_in(file::descriptor fd) noexcept
        {
            standard_in_ = std::move(fd);
        }

        void set_standard_out(file::descriptor fd) noexcept
        {
            standard_out_ = std::move(fd);
        }

        void set_standard_error(file::descriptor fd) noexcept
        {
            standard_error_ = std::move(fd);
        }

        // #### Spawn

        [[nodiscard]] result<handle> spawn()
        {
            // File actions.

            posix_spawn_file_actions_t file_actions{};
            posix_spawn_file_actions_t* fact = nullptr;

            if (standard_in_ || standard_out_ || standard_error_)
            {
                if (const int err = ::posix_spawn_file_actions_init(&file_actions); err != 0)
                {
                    return error_code{err, system::error_category};
                }

                // Initialized, update pointer.
                fact = &file_actions;
            }

            defer destroy_fact{[fact] {
                if (fact != nullptr)
                {
                    const int res = ::posix_spawn_file_actions_destroy(fact);
                    ignore_if_unused(res);
                    snn_should(res == 0);
                }
            }};

            if (standard_in_)
            {
                const int fd    = standard_in_.value(promise::has_value);
                const int newfd = STDIN_FILENO;
                snn_should(fact != nullptr);
                if (const int err = ::posix_spawn_file_actions_adddup2(fact, fd, newfd); err != 0)
                {
                    return error_code{err, system::error_category};
                }
            }

            if (standard_out_)
            {
                const int fd    = standard_out_.value(promise::has_value);
                const int newfd = STDOUT_FILENO;
                snn_should(fact != nullptr);
                if (const int err = ::posix_spawn_file_actions_adddup2(fact, fd, newfd); err != 0)
                {
                    return error_code{err, system::error_category};
                }
            }

            if (standard_error_)
            {
                const int fd    = standard_error_.value(promise::has_value);
                const int newfd = STDERR_FILENO;
                snn_should(fact != nullptr);
                if (const int err = ::posix_spawn_file_actions_adddup2(fact, fd, newfd); err != 0)
                {
                    return error_code{err, system::error_category};
                }
            }

            // Attributes

            posix_spawnattr_t attributes{};
            const not_null<posix_spawnattr_t*> attr{&attributes};

            if (const int err = ::posix_spawnattr_init(attr.get()); err != 0)
            {
                return error_code{err, system::error_category};
            }

            defer destroy_attr{[attr] {
                const int res = ::posix_spawnattr_destroy(attr.get());
                ignore_if_unused(res);
                snn_should(res == 0);
            }};

            // Set all signals to default action & set an empty signal mask.

            ::posix_spawnattr_setflags(attr.get(), POSIX_SPAWN_SETSIGDEF | POSIX_SPAWN_SETSIGMASK);

            sigset_t signals{};

            ::sigfillset(&signals);
            ::posix_spawnattr_setsigdefault(attr.get(), &signals);

            ::sigemptyset(&signals);
            ::posix_spawnattr_setsigmask(attr.get(), &signals);

            // Arguments (argv).

            // A null byte is a potential security vulnerability.
            if (path_.contains('\0') || arguments_.any(fn::contains{'\0'}))
            {
                return generic::error::unexpected_null_character;
            }

            vec<char*> argv{container::reserve, arguments_.count() + 2};
            argv.append(path_.writable().get());
            for (str& arg : arguments_)
            {
                argv.append(arg.writable().get());
            }
            argv.append(nullptr);

            // Spawn

            pid_t pid     = 0;
            const int err = ::posix_spawnp(&pid, path_.writable().get(), fact, attr.get(),
                                           argv.writable(), environ);
            if (err == 0)
            {
                snn_should(pid > 0); // Process id 0 is reserved for kernel/scheduler.
                return handle{pid};
            }
            return error_code{err, system::error_category};
        }

      private:
        str path_;
        vec<str> arguments_;
        file::descriptor standard_in_;
        file::descriptor standard_out_;
        file::descriptor standard_error_;
    };
}
