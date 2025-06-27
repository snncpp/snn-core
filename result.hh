// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Result with a value/reference or an error code

#pragma once

#include "snn-core/error_code.hh"
#include "snn-core/exception.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/mem/construct.hh"

namespace snn
{
    // ## Classes

    // ### result

    // Forward declare.

    template <typename T, typename ErrorStorage = error_code>
    class result;

    // ## Type traits

    // ### is_result

    namespace detail
    {
        template <typename>
        struct is_result_strict : public std::false_type
        {
        };

        template <typename T, typename ErrorStorage>
        struct is_result_strict<snn::result<T, ErrorStorage>> : public std::true_type
        {
        };
    }

    template <typename T>
    struct is_result : public detail::is_result_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_result_v = is_result<T>::value;

    // ### result_compat

    // Make T a type compatible with result<...>.

    template <typename T>
    struct result_compat : public remove_cv_rvalue_ref<T>
    {
    };

    template <typename T>
    using result_compat_t = typename result_compat<T>::type;

    // ### result_wrap

    // Wrap type T in a result<T>, but only if T is not already a result<...>.

    template <typename T, typename ErrorStorage = error_code>
    struct result_wrap
    {
        using type = result<result_compat_t<T>, ErrorStorage>;
    };

    template <typename T, typename E, typename ErrorStorage>
    struct result_wrap<result<T, E>, ErrorStorage>
    {
        using type = result<T, E>; // Keep original error storage.
    };

    template <typename T, typename ErrorStorage = error_code>
    using result_wrap_t = typename result_wrap<T, ErrorStorage>::type;

    // ## Classes

    // ### Result unpacked

    template <typename T>
    struct result_unpacked final
    {
        T value;
        snn::error_code error_code;
    };

    // ### result<T> {#result}

    namespace detail
    {
        // Non-trivial types.
        template <typename T, typename ErrorStorage, bool IsTriviallyCopyable>
        class result_storage final
        {
          public:
            constexpr result_storage(const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>)
                : storage_{v}, // First member.
                  has_value_{true}
            {
            }

            constexpr result_storage(T&& v) noexcept
                : storage_{std::move(v)}, // First member.
                  has_value_{true}
            {
            }

            constexpr result_storage(snn::error_code ec) noexcept
                : storage_{.error = ErrorStorage{ec}},
                  has_value_{false}
            {
            }

            // Copy constructor.
            constexpr result_storage(const result_storage& other)
                noexcept(std::is_nothrow_copy_constructible_v<T>)
                : storage_{.error = ErrorStorage{}},
                  has_value_{other.has_value_}
            {
                if (other.has_value_)
                    set_value_(other.get());
                else
                    set_error_storage_(other.storage_.error);
            }

            // Copy assignment operator.
            constexpr result_storage& operator=(const result_storage& other)
                noexcept(std::is_nothrow_copy_assignable_v<T> &&
                         std::is_nothrow_copy_constructible_v<T>)
            {
                // This works even if this == &other.

                if (has_value_ && other.has_value_)
                {
                    get() = other.get();
                }
                else if (has_value_ && !other.has_value_)
                {
                    get().~T();
                    has_value_ = false;
                    set_error_storage_(other.storage_.error);
                }
                else if (!has_value_ && other.has_value_)
                {
                    ErrorStorage tmp = storage_.error;
                    try
                    {
                        set_value_(other.get());
                        has_value_ = true;
                    }
                    catch (...)
                    {
                        snn_should(!has_value_);
                        set_error_storage_(tmp);
                        throw;
                    }
                }
                else
                {
                    snn_should(!has_value_ && !other.has_value_);
                    set_error_storage_(other.storage_.error);
                }
                return *this;
            }

            // Move constructor.
            constexpr result_storage(result_storage&& other) noexcept
                : storage_{.error = ErrorStorage{}},
                  has_value_{other.has_value_}
            {
                if (other.has_value_)
                    set_value_(std::move(other.get()));
                else
                    set_error_storage_(other.storage_.error);
            }

            // Move assignment operator.
            constexpr result_storage& operator=(result_storage&& other) noexcept
            {
                // This works even if this == &other.

                if (has_value_ && other.has_value_)
                {
                    get() = std::move(other.get());
                }
                else if (has_value_ && !other.has_value_)
                {
                    get().~T();
                    has_value_ = false;
                    set_error_storage_(other.storage_.error);
                }
                else if (!has_value_ && other.has_value_)
                {
                    set_value_(std::move(other.get()));
                    has_value_ = true;
                }
                else
                {
                    snn_should(!has_value_ && !other.has_value_);
                    set_error_storage_(other.storage_.error);
                }
                return *this;
            }

            constexpr ~result_storage()
            {
                if (has_value_)
                {
                    get().~T();
                }
            }

            constexpr void assign(const T& v) noexcept(std::is_nothrow_copy_assignable_v<T> &&
                                                       std::is_nothrow_copy_constructible_v<T>)
            {
                if (has_value_)
                {
                    get() = v;
                }
                else
                {
                    ErrorStorage tmp = storage_.error;
                    try
                    {
                        set_value_(v);
                        has_value_ = true;
                    }
                    catch (...)
                    {
                        snn_should(!has_value_);
                        set_error_storage_(tmp);
                        throw;
                    }
                }
            }

            constexpr void assign(T&& v) noexcept
            {
                if (has_value_)
                {
                    get() = std::move(v);
                }
                else
                {
                    set_value_(std::move(v));
                    has_value_ = true;
                }
            }

            constexpr void assign(snn::error_code ec) noexcept
            {
                if (has_value_)
                {
                    get().~T();
                    has_value_ = false;
                }
                set_error_storage_(ErrorStorage{ec});
            }

            constexpr snn::error_code error_code() const noexcept
            {
                snn_should(!has_value_);
                return storage_.error; // Implicitly converts if needed.
            }

            constexpr T& get() noexcept
            {
                snn_should(has_value_);
                return storage_.value;
            }

            constexpr const T& get() const noexcept
            {
                snn_should(has_value_);
                return storage_.value;
            }

            constexpr bool has_value() const noexcept
            {
                return has_value_;
            }

          private:
            union storage
            {
                T value; // Must be first.
                ErrorStorage error;

                // If `T` has a non-trivial destructor then this union will have a deleted
                // destructor by default.
                constexpr ~storage()
                {
                }
            };
            storage storage_;
            bool has_value_;

            constexpr void set_value_(const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>)
            {
                mem::construct(not_null{&storage_.value}, v);
            }

            constexpr void set_value_(T&& v) noexcept
            {
                mem::construct(not_null{&storage_.value}, std::move(v));
            }

            constexpr void set_error_storage_(const ErrorStorage e) noexcept
            {
                mem::construct(not_null{&storage_.error}, e);
            }
        };

        // Trivial types.
        template <typename T, typename ErrorStorage>
        class result_storage<T, ErrorStorage, true> final
        {
          public:
            constexpr result_storage(const T& v) noexcept
                : value_{v},
                  has_value_{true}
            {
            }

            constexpr result_storage(snn::error_code ec) noexcept
                : error_storage_{ec},
                  has_value_{false}
            {
            }

            constexpr void assign(const T& v) noexcept
            {
                *this = result_storage{v};
            }

            constexpr void assign(snn::error_code ec) noexcept
            {
                *this = result_storage{ec};
            }

            constexpr snn::error_code error_code() const noexcept
            {
                snn_should(!has_value_);
                return error_storage_;
            }

            constexpr T& get() noexcept
            {
                snn_should(has_value_);
                return value_;
            }

            constexpr const T& get() const noexcept
            {
                snn_should(has_value_);
                return value_;
            }

            constexpr bool has_value() const noexcept
            {
                return has_value_;
            }

          private:
            union
            {
                T value_;
                ErrorStorage error_storage_;
            };
            bool has_value_;
        };
    }

    template <typename T, typename ErrorStorage>
    class result final
    {
      public:
        static_assert(std::is_same_v<std::remove_cvref_t<T>, T>, "Invalid type for result<T>.");
        static_assert(std::is_nothrow_move_constructible_v<T>);
        static_assert(std::is_trivially_copyable_v<ErrorStorage>);
        static_assert(std::is_convertible_v<ErrorStorage, snn::error_code>);

        // #### Types

        using trivially_relocatable_type = trivially_relocatable_if_t<result, T>;

        using value_type = T;

        // #### Constructors

        result() = delete; // For better compilation errors.

        constexpr result(const not_deduced_t<T>& v)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            : storage_{v}
        {
        }

        constexpr result(not_deduced_t<T>&& v) noexcept
            : storage_{std::move(v)}
        {
        }

        template <same_as<snn::error_code> ErrorCode>
        constexpr result(const ErrorCode ec) noexcept
            : storage_{ec}
        {
        }

        template <error_code_enum Enum>
        constexpr result(const Enum e) noexcept
            : storage_{make_error_code(e)}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        // #### Assignment

        // Value assignment via operator=() is error-prone and therefore disabled.
        // Use assign_or_rebind() or the implicit copy/move assignment operators.
        template <typename U>
        void operator=(const U&) = delete;

        constexpr void assign_or_rebind(const T& v)
            noexcept(std::is_nothrow_copy_assignable_v<T> &&
                     std::is_nothrow_copy_constructible_v<T>)
        {
            storage_.assign(v);
        }

        constexpr void assign_or_rebind(T&& v) noexcept
        {
            storage_.assign(std::move(v));
        }

        template <same_as<snn::error_code> ErrorCode>
        constexpr void assign_or_rebind(const ErrorCode ec) noexcept
        {
            storage_.assign(ec);
        }

        template <error_code_enum Enum>
        constexpr void assign_or_rebind(const Enum e) noexcept
        {
            storage_.assign(make_error_code(e));
        }

        // #### Observers

        template <typename F>
        void and_then(F&&) const& = delete;

        template <typename F>
        [[nodiscard]] constexpr auto and_then(F&& f) && noexcept(
            noexcept(std::forward<F>(f)(std::move(storage_.get()))))
            -> result_wrap_t<decltype(std::forward<F>(f)(std::declval<T>())), ErrorStorage>
        {
            if (has_value())
            {
                using R = decltype(std::forward<F>(f)(std::move(storage_.get())));
                if constexpr (std::is_same_v<R, void>)
                {
                    std::forward<F>(f)(std::move(storage_.get()));
                    return {};
                }
                else
                {
                    return std::forward<F>(f)(std::move(storage_.get()));
                }
            }
            return storage_.error_code();
        }

        constexpr void discard() const noexcept
        {
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return storage_.has_value();
        }

        [[nodiscard]] constexpr snn::error_code error_code() const noexcept
        {
            if (!has_value())
            {
                return storage_.error_code();
            }
            return snn::error_code{};
        }

        [[nodiscard]] static constexpr snn::error_code default_error_code() noexcept
        {
            return ErrorStorage::default_error_code();
        }

        void unpack() const& = delete;

        [[nodiscard]] constexpr result_unpacked<T> unpack() && noexcept(
            std::is_nothrow_default_constructible_v<T>)
        {
            if (has_value())
            {
                return result_unpacked<T>{T{std::move(storage_.get())}, snn::error_code{}};
            }
            // The `error_code` must not evaluate as `false`. This can also help the compiler
            // optimize away a second error check.
            return result_unpacked<T>{
                T{},
                storage_.error_code().non_zero_or(
                    not_zero<i32>{to_underlying(generic::error::unexpected_zero_value_error_code)},
                    generic::error_category)};
        }

        [[nodiscard]] constexpr T& value() &
        {
            if (has_value())
            {
                return storage_.get();
            }
            throw_or_abort(storage_.error_code());
        }

        [[nodiscard]] constexpr const T& value() const&
        {
            if (has_value())
            {
                return storage_.get();
            }
            throw_or_abort(storage_.error_code());
        }

        [[nodiscard]] constexpr T&& value() &&
        {
            if (has_value())
            {
                return std::move(storage_.get());
            }
            throw_or_abort(storage_.error_code());
        }

        void value() const&& = delete;

        [[nodiscard]] constexpr T& value(assume::has_value_t) & noexcept
        {
            snn_assert(has_value());
            return storage_.get();
        }

        [[nodiscard]] constexpr const T& value(assume::has_value_t) const& noexcept
        {
            snn_assert(has_value());
            return storage_.get();
        }

        void value(assume::has_value_t) const&& = delete;

        [[nodiscard]] constexpr T value_or(const T alt) const noexcept
            requires(std::is_arithmetic_v<T>)
        {
            if (has_value())
            {
                return storage_.get();
            }
            return alt;
        }

        template <typename U>
        [[nodiscard]] constexpr T value_or(U&& alt) const& noexcept(
            std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_constructible_v<T, U>)
            requires(!std::is_arithmetic_v<T>)
        {
            if (has_value())
            {
                return T{storage_.get()};
            }
            return T{std::forward<U>(alt)};
        }

        template <typename U>
        [[nodiscard]] constexpr T value_or(U&& alt) && noexcept(
            std::is_nothrow_constructible_v<T, U>)
            requires(!std::is_arithmetic_v<T>)
        {
            if (has_value())
            {
                return T{std::move(storage_.get())};
            }
            return T{std::forward<U>(alt)};
        }

        [[nodiscard]] constexpr T value_or_default() const& noexcept(
            std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_default_constructible_v<T>)
        {
            if (has_value())
            {
                return T{storage_.get()};
            }
            return T{};
        }

        [[nodiscard]] constexpr T value_or_default() && noexcept(
            std::is_nothrow_default_constructible_v<T>)
        {
            if (has_value())
            {
                return T{std::move(storage_.get())};
            }
            return T{};
        }

      private:
        detail::result_storage<T, ErrorStorage, std::is_trivially_copyable_v<T>> storage_;
    };

    // ### result<T&> {#result-reference}

    namespace detail
    {
        // Pointer with error code stored.
        template <typename T, typename ErrorStorage, bool HasBooleanStateErrorStorage>
        class result_ptr_storage final
        {
          public:
            constexpr result_ptr_storage(T& v) noexcept
                : value_ptr_{&v},
                  error_storage_{}
            {
            }

            constexpr result_ptr_storage(snn::error_code ec) noexcept
                : value_ptr_{nullptr},
                  error_storage_{ec}
            {
            }

            constexpr snn::error_code error_code() const noexcept
            {
                snn_should(!has_value());
                return error_storage_;
            }

            constexpr T& get() noexcept
            {
                snn_should(has_value());
                return *value_ptr_;
            }

            constexpr const T& get() const noexcept
            {
                snn_should(has_value());
                return *value_ptr_;
            }

            constexpr bool has_value() const noexcept
            {
                return value_ptr_ != nullptr;
            }

          private:
            T* value_ptr_;
            ErrorStorage error_storage_;
        };

        // Pointer without error code stored.
        template <typename T, typename ErrorStorage>
        class result_ptr_storage<T, ErrorStorage, true> final
        {
          public:
            constexpr result_ptr_storage(T& v) noexcept
                : value_ptr_{&v}
            {
            }

            constexpr result_ptr_storage(snn::error_code) noexcept
                : value_ptr_{nullptr}
            {
            }

            constexpr snn::error_code error_code() const noexcept
            {
                snn_should(!has_value());
                return ErrorStorage::default_error_code();
            }

            constexpr T& get() noexcept
            {
                snn_should(has_value());
                return *value_ptr_;
            }

            constexpr const T& get() const noexcept
            {
                snn_should(has_value());
                return *value_ptr_;
            }

            constexpr bool has_value() const noexcept
            {
                return value_ptr_ != nullptr;
            }

          private:
            T* value_ptr_;
        };
    }

    template <typename T, typename ErrorStorage>
    class result<T&, ErrorStorage> final
    {
      public:
        static_assert(std::is_trivially_copyable_v<ErrorStorage>);
        static_assert(std::is_convertible_v<ErrorStorage, snn::error_code>);

        // #### Types

        using value_type = std::remove_cv_t<T>;

        // #### Constructors

        result() = delete; // For better compilation errors.

        constexpr result(not_deduced_t<T>& v) noexcept
            : storage_{v}
        {
        }

        result(const not_deduced_t<T>&&) = delete;

        template <same_as<snn::error_code> ErrorCode>
        constexpr result(const ErrorCode ec) noexcept
            : storage_{ec}
        {
        }

        template <error_code_enum Enum>
        constexpr result(const Enum e) noexcept
            : storage_{make_error_code(e)}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        // #### Assignment

        // Value assignment via operator=() is error-prone and therefore disabled.
        // Use assign_or_rebind() or the implicit copy/move assignment operators.
        template <typename U>
        void operator=(const U&) = delete;

        constexpr void assign_or_rebind(T& v) noexcept
        {
            *this = result{v};
        }

        void assign_or_rebind(const T&&) = delete;

        template <same_as<snn::error_code> ErrorCode>
        constexpr void assign_or_rebind(const ErrorCode ec) noexcept
        {
            *this = result{ec};
        }

        template <error_code_enum Enum>
        constexpr void assign_or_rebind(const Enum e) noexcept
        {
            *this = result{e};
        }

        // #### Observers

        template <typename F>
        void and_then(F&&) const& = delete;

        template <typename F>
        [[nodiscard]] constexpr auto and_then(F&& f) && noexcept(
            noexcept(std::forward<F>(f)(storage_.get())))
            -> result_wrap_t<decltype(std::forward<F>(f)(std::declval<T&>())), ErrorStorage>
        {
            if (has_value())
            {
                using R = decltype(std::forward<F>(f)(storage_.get()));
                if constexpr (std::is_same_v<R, void>)
                {
                    std::forward<F>(f)(storage_.get());
                    return {};
                }
                else
                {
                    return std::forward<F>(f)(storage_.get());
                }
            }
            return storage_.error_code();
        }

        constexpr void discard() const noexcept
        {
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return storage_.has_value();
        }

        [[nodiscard]] constexpr snn::error_code error_code() const noexcept
        {
            if (!has_value())
            {
                return storage_.error_code();
            }
            return snn::error_code{};
        }

        [[nodiscard]] static constexpr snn::error_code default_error_code() noexcept
        {
            return ErrorStorage::default_error_code();
        }

        [[nodiscard]] constexpr T& value()
        {
            if (has_value())
            {
                return storage_.get();
            }
            throw_or_abort(storage_.error_code());
        }

        [[nodiscard]] constexpr const T& value() const
        {
            if (has_value())
            {
                return storage_.get();
            }
            throw_or_abort(storage_.error_code());
        }

        [[nodiscard]] constexpr T& value(assume::has_value_t) & noexcept
        {
            snn_assert(has_value());
            return storage_.get();
        }

        [[nodiscard]] constexpr const T& value(assume::has_value_t) const& noexcept
        {
            snn_assert(has_value());
            return storage_.get();
        }

        void value(assume::has_value_t) const&& = delete;

        [[nodiscard]] constexpr value_type value_or(const value_type alt) const noexcept
            requires(std::is_arithmetic_v<value_type>)
        {
            if (has_value())
            {
                return storage_.get();
            }
            return alt;
        }

        template <typename U>
        [[nodiscard]] constexpr value_type value_or(U&& alt) const
            noexcept(std::is_nothrow_copy_constructible_v<value_type> &&
                     std::is_nothrow_constructible_v<value_type, U>)
            requires(!std::is_arithmetic_v<value_type>)
        {
            if (has_value())
            {
                return value_type{storage_.get()};
            }
            return value_type{std::forward<U>(alt)};
        }

        [[nodiscard]] constexpr value_type value_or_default() const
            noexcept(std::is_nothrow_copy_constructible_v<value_type> &&
                     std::is_nothrow_default_constructible_v<value_type>)
        {
            if (has_value())
            {
                return value_type{storage_.get()};
            }
            return value_type{};
        }

      private:
        detail::result_ptr_storage<T, ErrorStorage, ErrorStorage::has_boolean_state()> storage_;
    };

    // ### result<void> {#result-void}

    namespace detail
    {
        // Default void error storage.
        template <typename ErrorStorage>
        class result_void_storage final
        {
          public:
            constexpr result_void_storage() noexcept = default;

            constexpr result_void_storage(snn::error_code ec) noexcept
                : error_storage_{ec}
            {
            }

            constexpr snn::error_code error_code() const noexcept
            {
                snn_should(!has_value());
                return error_storage_;
            }

            constexpr bool has_value() const noexcept
            {
                return !error_storage_;
            }

          private:
            ErrorStorage error_storage_{};
        };

        // Specialization for `error_code` (this specialization is a friend of `error_code`).
        template <>
        class result_void_storage<error_code> final
        {
          public:
            constexpr result_void_storage() noexcept = default;

            constexpr result_void_storage(snn::error_code ec) noexcept
                : error_category_{ec.category_},
                  error_value_{ec.value_},
                  has_error_{true}
            {
            }

            constexpr snn::error_code error_code() const noexcept
            {
                snn_should(!has_value());
                return snn::error_code{init::internal, error_value_, error_category_};
            }

            constexpr bool has_value() const noexcept
            {
                return !has_error_;
            }

          private:
            // These are all zero on success (for better machine code).
            const error_category* error_category_{nullptr};
            i32 error_value_{0};
            u32 has_error_{false};
        };
    }

    template <typename ErrorStorage>
    class result<void, ErrorStorage> final
    {
      public:
        static_assert(std::is_trivially_copyable_v<ErrorStorage>);

        // #### Constructors

        constexpr result() noexcept = default;

        template <same_as<snn::error_code> ErrorCode>
        constexpr result(const ErrorCode ec) noexcept
            : storage_{ec}
        {
        }

        template <error_code_enum Enum>
        constexpr result(const Enum e) noexcept
            : storage_{make_error_code(e)}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        // #### Assignment

        // Value assignment via operator=() is error-prone and therefore disabled.
        // Use assign_or_rebind() or the implicit copy/move assignment operators.
        template <typename U>
        void operator=(const U&) = delete;

        constexpr void assign_or_rebind() noexcept
        {
            *this = result{};
        }

        template <same_as<snn::error_code> ErrorCode>
        constexpr void assign_or_rebind(const ErrorCode ec) noexcept
        {
            *this = result{ec};
        }

        template <error_code_enum Enum>
        constexpr void assign_or_rebind(const Enum e) noexcept
        {
            *this = result{e};
        }

        // #### Observers

        template <typename F>
        void and_then(F&&) const& = delete;

        template <typename F>
        [[nodiscard]] constexpr auto and_then(F&& f) && noexcept(noexcept(std::forward<F>(f)())) //
            -> result_wrap_t<decltype(std::forward<F>(f)()), ErrorStorage>
        {
            if (has_value())
            {
                using R = decltype(std::forward<F>(f)());
                if constexpr (std::is_same_v<R, void>)
                {
                    std::forward<F>(f)();
                    return {};
                }
                else
                {
                    return std::forward<F>(f)();
                }
            }
            return storage_.error_code();
        }

        constexpr void discard() const noexcept
        {
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return storage_.has_value();
        }

        [[nodiscard]] constexpr snn::error_code error_code() const noexcept
        {
            if (!has_value())
            {
                return storage_.error_code();
            }
            return snn::error_code{};
        }

        [[nodiscard]] static constexpr snn::error_code default_error_code() noexcept
        {
            return ErrorStorage::default_error_code();
        }

        constexpr void or_throw() const
        {
            if (!has_value())
            {
                throw_or_abort(storage_.error_code());
            }
        }

      private:
        detail::result_void_storage<ErrorStorage> storage_;
    };

    // ## Functions

    // ### Comparison

    template <typename T, typename Err, typename U, typename E>
    constexpr bool operator==(const result<T, Err>& left, const result<U, E>& right)
    {
        if (left.has_value() && right.has_value()) [[likely]]
        {
            return left.value(assume::has_value) == right.value(assume::has_value);
        }
        if (!left.has_value() && !right.has_value())
        {
            return left.error_code() == right.error_code();
        }
        return false;
    }

    template <typename Err, typename E>
    constexpr bool operator==(const result<void, Err>& left, const result<void, E>& right) noexcept
    {
        if (left.has_value() && right.has_value()) [[likely]]
        {
            return true;
        }
        if (!left.has_value() && !right.has_value())
        {
            return left.error_code() == right.error_code();
        }
        return false;
    }

    template <typename T, typename Err, typename U, typename E>
    constexpr auto operator<=>(const result<T, Err>& left, const result<U, E>& right)
        -> decltype(left.value(assume::has_value) <=> right.value(assume::has_value))
    {
        if (left.has_value() && right.has_value()) [[likely]]
        {
            return left.value(assume::has_value) <=> right.value(assume::has_value);
        }
        if (!left.has_value() && !right.has_value())
        {
            return left.error_code() <=> right.error_code();
        }
        if (!left.has_value())
        {
            return std::strong_ordering::less;
        }
        return std::strong_ordering::greater;
    }

    template <typename Err, typename E>
    constexpr std::strong_ordering operator<=>(const result<void, Err>& left,
                                               const result<void, E>& right) noexcept
    {
        if (left.has_value() && right.has_value()) [[likely]]
        {
            return std::strong_ordering::equal;
        }
        if (!left.has_value() && !right.has_value())
        {
            return left.error_code() <=> right.error_code();
        }
        if (!left.has_value())
        {
            return std::strong_ordering::less;
        }
        return std::strong_ordering::greater;
    }

    // ### Comparison with a value

    template <typename T, typename Err, typename V>
        requires(!is_result_v<V>)
    constexpr bool operator==(const result<T, Err>& left, const V& right)
    {
        if (left.has_value())
        {
            return left.value(assume::has_value) == right;
        }
        return false;
    }

    template <typename T, typename Err, typename V>
        requires(!is_result_v<V>)
    constexpr auto operator<=>(const result<T, Err>& left, const V& right) //
        -> decltype(left.value(assume::has_value) <=> right)
    {
        if (left.has_value())
        {
            return left.value(assume::has_value) <=> right;
        }
        return std::strong_ordering::less;
    }

    // ### call_if

    namespace detail
    {
        template <typename T, typename Err>
        constexpr error_code call_if_error_code(const result<T, Err>& res) noexcept
        {
            return res.error_code();
        }

        template <typename T, typename Err, typename... Results>
        constexpr error_code call_if_error_code(const result<T, Err>& res,
                                                const Results&... results) noexcept
        {
            if (!res)
            {
                return res.error_code();
            }
            return call_if_error_code(results...);
        }
    }

    template <typename F, typename... Results>
    [[nodiscard]] constexpr auto call_if(F&& f, Results&&... results)
        -> result_wrap_t<decltype(std::forward<F>(f)(std::forward<Results>(results).value()...))>
    {
        static_assert((is_result_v<Results> && ...));
        if ((results && ...))
        {
            using R = decltype(std::forward<F>(f)(std::forward<Results>(results).value()...));
            if constexpr (std::is_same_v<R, void>)
            {
                std::forward<F>(f)(std::forward<Results>(results).value()...);
                return {};
            }
            else
            {
                return std::forward<F>(f)(std::forward<Results>(results).value()...);
            }
        }
        return detail::call_if_error_code(results...);
    }
}
