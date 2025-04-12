// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unit test entry point and `snn_require` macros

#pragma once

#include "snn-core/main.hh"
#include "snn-core/file/dir/change.hh"
#include "snn-core/fn/common.hh"
#include <cstdlib> // quick_exit

namespace snn
{
    // ## Functions

    // ### unittest

    // Forward declare, this function must be defined in each unit test.

    void unittest();

    // ### require_failed

    // Helper function for `snn_require` macros.

    [[noreturn]] inline void require_failed(const char* const desc, const char* const expr,
                                            const char* const file, const int line) noexcept
    {
        try
        {
            fmt::print_error("{}:{}: {} ({})\n",                        //
                             cstrview{file, promise::null_terminated},  //
                             line,                                      //
                             cstrview{desc, promise::null_terminated},  //
                             cstrview{expr, promise::null_terminated}); //
        }
        catch (...)
        {
        }

        std::quick_exit(constant::exit::failure);
    }

    // ### main

    // Calls the `unittest()` function.

    int main(const array_view<const env::argument> arguments)
    {
        if (arguments.count() > 1)
        {
            fmt::print_error_line("Error: Unit test executable does not take arguments.");

            if (arguments.view(1).all(fn::to{meta::type<cstrview>, fn::has_back{".cc"}}))
            {
                fmt::print_error_line("If you're using https://github.com/snncpp/build-tool, did "
                                      "you mean to use the \"runall\" command?");
            }

            return constant::exit::failure;
        }

        if (arguments)
        {
            // Change directory to that of the executable (invoked as).
            auto rng = arguments.front(promise::not_empty).to<cstrview>().range();
            rng.pop_back_while(fn::is{fn::not_equal_to{}, '/'});
            if (rng)
            {
                file::dir::change(str{rng}).or_throw();
            }
        }

        unittest();

        return constant::exit::success;
    }
}

// ## Macros

// ### snn_require

// The expression must evaluate to `true` (and not throw) to continue execution.

// To keep compilation times down and as more and more tests are compile-time only this macro no
// longer prints any information about caught exceptions.

// `if (__VA_ARGS__)` is used instead of `if (!(__VA_ARGS__))` to not suppress warnings.

#define snn_require(...)                                                                           \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            if (__VA_ARGS__)                                                                       \
            {                                                                                      \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                ::snn::require_failed("snn_require failed", #__VA_ARGS__, __FILE__, __LINE__);     \
            }                                                                                      \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::snn::require_failed("snn_require threw", #__VA_ARGS__, __FILE__, __LINE__);          \
        }                                                                                          \
    } while (false)

// ### snn_require_nothrow

// The expression must not throw to continue execution.

// `(void)(__VA_ARGS__)` is used to suppress unused result warnings.

#define snn_require_nothrow(...)                                                                   \
    do                                                                                             \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            (void)(__VA_ARGS__);                                                                   \
        }                                                                                          \
        catch (...)                                                                                \
        {                                                                                          \
            ::snn::require_failed("snn_require_nothrow failed", #__VA_ARGS__, __FILE__, __LINE__); \
        }                                                                                          \
    } while (false)

// ### snn_require_throws_code

// Ignored in a constant-evaluated context.

// The expression must throw a specific error code to continue execution.

// `(void)(expr)` is used to suppress unused result warnings.
// Variable names are picked to (hopefully) be unique in any context.

#define snn_require_throws_code(expr, ec)                                                          \
    do                                                                                             \
    {                                                                                              \
        if (!std::is_constant_evaluated())                                                         \
        {                                                                                          \
            bool ec8693 = false;                                                                   \
            try                                                                                    \
            {                                                                                      \
                (void)(expr);                                                                      \
            }                                                                                      \
            catch (const ::snn::exception& e9023)                                                  \
            {                                                                                      \
                if (e9023.error_code() == (ec))                                                    \
                {                                                                                  \
                    ec8693 = true;                                                                 \
                }                                                                                  \
            }                                                                                      \
            catch (...)                                                                            \
            {                                                                                      \
            }                                                                                      \
            if (!ec8693)                                                                           \
            {                                                                                      \
                ::snn::require_failed("snn_require_throws_code failed", #expr, __FILE__,           \
                                      __LINE__);                                                   \
            }                                                                                      \
        }                                                                                          \
    } while (false)

// ### snn_require_throws_type

// Ignored in a constant-evaluated context.

// The expression must throw a specific exception type to continue execution.

// `(void)(expr)` is used to suppress unused result warnings.
// Variable names are picked to (hopefully) be unique in any context.

#define snn_require_throws_type(expr, exception_type)                                              \
    do                                                                                             \
    {                                                                                              \
        if (!std::is_constant_evaluated())                                                         \
        {                                                                                          \
            bool ec8693 = false;                                                                   \
            try                                                                                    \
            {                                                                                      \
                (void)(expr);                                                                      \
            }                                                                                      \
            catch (const exception_type&)                                                          \
            {                                                                                      \
                ec8693 = true;                                                                     \
            }                                                                                      \
            catch (...)                                                                            \
            {                                                                                      \
            }                                                                                      \
            if (!ec8693)                                                                           \
            {                                                                                      \
                ::snn::require_failed("snn_require_throws_type failed", #expr, __FILE__,           \
                                      __LINE__);                                                   \
            }                                                                                      \
        }                                                                                          \
    } while (false)

// ### snn_static_require

// If supported, test the expression in both a constant-evaluated and a non-constant-evaluated
// context. (GCCs constexpr support is not as good as Clangs.)

#if defined(__clang__)
    #define snn_static_require(e)                                                                  \
        static_assert(e);                                                                          \
        snn_require(e)
#else
    #define snn_static_require(e) snn_require(e)
#endif
