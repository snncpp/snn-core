// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Reassignable value or reference

#pragma once

#include "snn-core/core.hh"

namespace snn
{
    // ## Classes

    // ### val_or_ref<T>

    // Holds a value. `assign_or_rebind()` assigns a new value.

    template <typename T>
    class val_or_ref final
    {
      public:
        // #### Types

        using type = T;

        static_assert(std::is_same_v<std::remove_cvref_t<T>, T>, "Invalid type for val_or_ref<T>.");

        // #### Explicit constructors

        constexpr explicit val_or_ref(const not_deduced_t<T>& v) noexcept(
            std::is_nothrow_copy_constructible_v<T>)
            : value_{v}
        {
        }

        constexpr explicit val_or_ref(not_deduced_t<T>&& v) noexcept
            : value_{std::move(v)}
        {
        }

        // #### Assign or rebind

        constexpr void assign_or_rebind(const T& v) noexcept(std::is_nothrow_copy_assignable_v<T>)
        {
            value_ = v;
        }

        constexpr void assign_or_rebind(T&& v) noexcept
        {
            value_ = std::move(v);
        }

        // #### Get

        [[nodiscard]] constexpr T& get() & noexcept
        {
            return value_;
        }

        [[nodiscard]] constexpr const T& get() const& noexcept
        {
            return value_;
        }

        [[nodiscard]] constexpr T&& get() && noexcept
        {
            return std::move(value_);
        }

        [[nodiscard]] constexpr const T&& get() const&& noexcept
        {
            return std::move(value_);
        }

        // #### Callable

        template <typename... Args>
            requires callable<T&, Args...>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args) &
        {
            return value_(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires callable<const T&, Args...>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args) const&
        {
            return value_(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires callable<T&&, Args...>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args) &&
        {
            return std::move(value_)(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires callable<const T&&, Args...>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args) const&&
        {
            return std::move(value_)(std::forward<Args>(args)...);
        }

      private:
        T value_;
    };

    // ### val_or_ref<T&> specialization

    // Holds a reference. `assign_or_rebind()` rebinds to a new address.

    template <typename T>
    class val_or_ref<T&> final
    {
      public:
        // #### Types

        using type = T&;

        // #### Explicit constructors

        constexpr explicit val_or_ref(not_deduced_t<T>& ref) noexcept
            : ptr_{&ref}
        {
        }

        explicit val_or_ref(not_deduced_t<T>&&) = delete;

        // #### Converting constructors

        // Non-const to const converting constructor.

        template <same_as<std::remove_const_t<T>&> NonConstRef>
            requires std::is_const_v<T>
        constexpr val_or_ref(val_or_ref<NonConstRef> other) noexcept
            : ptr_{&other.get()}
        {
        }

        // #### Assign or rebind

        constexpr void assign_or_rebind(T& ref) noexcept
        {
            ptr_ = &ref;
        }

        void assign_or_rebind(T&& ref) = delete;

        // #### Get

        [[nodiscard]] constexpr T& get() noexcept
        {
            return *ptr_;
        }

        [[nodiscard]] constexpr const T& get() const noexcept
        {
            return *ptr_;
        }

        // #### Callable

        template <typename... Args>
            requires callable<T&, Args...>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args)
        {
            return get()(std::forward<Args>(args)...);
        }

        template <typename... Args>
            requires callable<const T&, Args...>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args) const
        {
            return get()(std::forward<Args>(args)...);
        }

      private:
        T* ptr_;
    };

    // ## Deduction

    // ### val_or_ref

    template <typename T>
    val_or_ref(T&&) -> val_or_ref<std::remove_const_t<T>>;

    // ## Functions

    // Helper functions.

    // ### as_ref

    template <typename T>
    [[nodiscard]] constexpr auto as_ref(T& v) noexcept
    {
        return val_or_ref<T&>{v};
    }

    template <typename T>
    void as_ref(const T&&) = delete;

    // ### as_cref

    template <typename T>
    [[nodiscard]] constexpr auto as_cref(const T& v) noexcept
    {
        return val_or_ref<const T&>{v};
    }

    template <typename T>
    void as_cref(const T&&) = delete;

    // ### as_val

    template <typename T>
    [[nodiscard]] constexpr auto as_val(T&& v)
    {
        return val_or_ref<std::remove_cvref_t<T>>{std::forward<T>(v)};
    }
}
