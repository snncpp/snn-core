// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Command line options

// Define and parse command line options and arguments.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/exception.hh"
#include "snn-core/optional.hh"
#include "snn-core/strcore.hh"
#include "snn-core/vec.hh"
#include "snn-core/algo/find_greater_than_or_equal_to.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/env/argument.hh"
#include "snn-core/env/error.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::env
{
    // ## Classes

    // ### option

    class option final
    {
      public:
        // #### Types

        using trivially_relocatable_type = option;

        enum type : u8
        {
            boolean      = 0,
            takes_values = 1,
        };

        // #### Constructor

        constexpr option(const cstrview long_flag, const optional<char> short_flag = nullopt,
                         const type t = boolean)
            : values_{},
              long_flag_{long_flag},
              count_{0},
              short_flag_{short_flag},
              type_{t}
        {
            if (!is_valid_long_flag_name(long_flag_))
            {
                throw_or_abort(env::error::invalid_flag_name);
            }

            if (short_flag_ && !is_valid_short_flag_name(short_flag_.value(promise::has_value)))
            {
                throw_or_abort(env::error::invalid_flag_name);
            }
        }

        // #### Information

        [[nodiscard]] constexpr u32 count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] constexpr cstrview long_flag() const noexcept
        {
            return long_flag_;
        }

        [[nodiscard]] constexpr optional<char> short_flag() const noexcept
        {
            return short_flag_;
        }

        [[nodiscard]] constexpr bool is_set() const noexcept
        {
            return count_ > 0;
        }

        [[nodiscard]] constexpr bool is_boolean() const noexcept
        {
            return type_ == boolean;
        }

        [[nodiscard]] constexpr const vec<cstrview>& values() const noexcept
        {
            return values_;
        }

        // #### Validation

        [[nodiscard]] static constexpr bool is_valid_long_flag_name(
            const transient<cstrview> flag) noexcept
        {
            auto rng = flag.get().range();
            if (rng.count() >= 2)
            {
                if (rng.pop_front_while(chr::is_alphanumeric))
                {
                    while (rng.drop_front('-'))
                    {
                        if (!rng.pop_front_while(chr::is_alphanumeric))
                        {
                            return false;
                        }
                    }
                    return rng.is_empty();
                }
            }
            return false;
        }

        [[nodiscard]] static constexpr bool is_valid_short_flag_name(const char flag) noexcept
        {
            return chr::is_alphanumeric(flag);
        }

        // #### Comparison (for sorting)

        constexpr bool operator<(const option& other) const noexcept
        {
            return long_flag_ < other.long_flag_;
        }

        constexpr bool operator<(const cstrview long_flag) const noexcept
        {
            return long_flag_ < long_flag;
        }

      private:
        vec<cstrview> values_;
        cstrview long_flag_;
        u32 count_;
        optional<char> short_flag_;
        type type_;

        friend class options;

        constexpr void add_value(const cstrview value)
        {
            snn_should(!is_boolean());
            values_.append(value);
        }

        constexpr void increment_count() noexcept
        {
            ++count_;
        }
    };

    // ### options

    class options final
    {
      public:
        // #### Constructors

        constexpr options(const array_view<const env::argument> arguments,
                          const init_list<env::option> options)
            : options_{options},
              error_message_{},
              args_{arguments},
              program_name_{}
        {
            options_.sort();
            parse_or_clear_();
        }

        constexpr options(const array_view<const env::argument> arguments,
                          const init_list<env::option> options, promise::is_sorted_t)
            : options_{options},
              error_message_{},
              args_{arguments},
              program_name_{}
        {
            snn_should(options_.is_sorted());
            parse_or_clear_();
        }

        // #### Non-copyable

        options(const options&)            = delete;
        options& operator=(const options&) = delete;

        // #### Non-movable

        options(options&&)            = delete;
        options& operator=(options&&) = delete;

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return error_ == error::no_error;
        }

        // #### Arguments

        [[nodiscard]] constexpr array_view<const env::argument> arguments() const noexcept
        {
            return args_;
        }

        // #### Program name

        [[nodiscard]] constexpr cstrview program_name() const noexcept
        {
            return program_name_;
        }

        // #### Get option from flag (non-throwing)

        [[nodiscard]] constexpr optional<env::option&> get(
            const same_as<char> auto short_flag) noexcept
        {
            for (auto& o : options_)
            {
                if (o.short_flag() == short_flag)
                {
                    return o;
                }
            }
            return nullopt;
        }

        [[nodiscard]] constexpr optional<env::option&> get(const cstrview long_flag) noexcept
        {
            const usize pos =
                algo::find_greater_than_or_equal_to(options_.range(), long_flag, promise::is_sorted)
                    .value_or_npos();
            if (pos < options_.count())
            {
                auto& o = options_.at(pos, promise::within_bounds);
                if (o.long_flag() == long_flag)
                {
                    return o;
                }
            }
            return nullopt;
        }

        // #### Get option from flag (throwing)

        [[nodiscard]] constexpr env::option& option(const same_as<char> auto short_flag)
        {
            auto opt = get(short_flag);
            if (opt)
            {
                return opt.value(promise::has_value);
            }
            throw_or_abort(env::error::unknown_option);
        }

        [[nodiscard]] constexpr env::option& option(const cstrview long_flag)
        {
            auto opt = get(long_flag);
            if (opt)
            {
                return opt.value(promise::has_value);
            }
            throw_or_abort(env::error::unknown_option);
        }

        // #### Error

        [[nodiscard]] constexpr snn::error_code error_code() const noexcept
        {
            if (error_ != error::no_error)
            {
                return make_error_code(error_);
            }
            return snn::error_code{};
        }

        [[nodiscard]] constexpr const str& error_message() const noexcept
        {
            return error_message_;
        }

      private:
        vec<env::option> options_;
        str error_message_;
        array_view<const env::argument> args_;
        cstrview program_name_;
        env::error error_{error::no_error};

        [[nodiscard]] constexpr env::error parse_()
        {
            if (args_)
            {
                program_name_ = args_.front(promise::not_empty).to<cstrview>();
                args_.drop_front_n(1);
            }

            auto rng = args_.range();
            while (rng)
            {
                auto arg = rng.pop_front(promise::not_empty).to<cstrview>();

                if (!arg.has_front('-') || arg == "-")
                {
                    // Not a flag.
                    break;
                }

                args_.drop_front_n(1);

                if (arg == "--")
                {
                    // End of flags.
                    break;
                }

                // Long flag?
                if (arg.has_front("--"))
                {
                    arg.drop_front_n(string_size("--"));
                    snn_should(arg);

                    if (auto opt = get(arg))
                    {
                        auto& o = opt.value(promise::has_value);
                        o.increment_count();
                        if (!o.is_boolean())
                        {
                            if (rng)
                            {
                                // The next argument is the value.
                                o.add_value(rng.pop_front(promise::not_empty).to<cstrview>());
                                args_.drop_front_n(1);
                            }
                            else
                            {
                                error_message_ << "Option \"--" << arg << "\" requires a value";
                                return error::option_requires_value;
                            }
                        }
                    }
                    else
                    {
                        error_message_ << "Unknown option \"--" << arg << '"';
                        return error::unknown_option;
                    }
                }
                // Short flag(s).
                else
                {
                    arg.drop_front_n(string_size("-"));
                    snn_should(arg);

                    // Every character is a potential flag.
                    while (arg)
                    {
                        const char c = arg.front(promise::not_empty);
                        arg.drop_front_n(1);

                        if (auto opt = get(c))
                        {
                            auto& o = opt.value(promise::has_value);
                            o.increment_count();
                            if (!o.is_boolean())
                            {
                                if (arg)
                                {
                                    // What's left in `arg` is the value.
                                    o.add_value(arg);
                                    arg.clear();
                                }
                                else if (rng)
                                {
                                    // The next argument is the value.
                                    o.add_value(rng.pop_front(promise::not_empty).to<cstrview>());
                                    args_.drop_front_n(1);
                                }
                                else
                                {
                                    error_message_ << "Option \"-" << c << "\" requires a value";
                                    return error::option_requires_value;
                                }
                            }
                        }
                        else
                        {
                            error_message_ << "Unknown option \"-" << c << '"';
                            return error::unknown_option;
                        }
                    }
                }
            }

            return error::no_error;
        }

        constexpr void parse_or_clear_()
        {
            error_ = parse_();
            if (error_ != error::no_error)
            {
                options_.clear();
                args_.clear();
            }
        }
    };
}
