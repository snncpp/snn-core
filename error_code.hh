// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error category and error code

#pragma once

#include "snn-core/array.hh"
#include "snn-core/array_view.fwd.hh"
#include "snn-core/null_term.hh"
#include "snn-core/strcore.fwd.hh"
#include "snn-core/mem/raw/compare.hh"

namespace snn
{
    // ## Type traits

    // ### is_error_code_enum

    // `is_error_code_enum_strict<...>` must be specialized for all error code enums.

    template <typename>
    struct is_error_code_enum_strict : public std::false_type
    {
    };

    template <typename T>
    struct is_error_code_enum : public is_error_code_enum_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_error_code_enum_v = is_error_code_enum<T>::value;

    // ## Concepts

    // ### error_code_enum

    template <typename T>
    concept error_code_enum = is_error_code_enum_v<T>;

    // ## Classes

    // ### error_category

    // Id must be unique.

    // Supports three-way comparison but the order is unspecified in a non-constexpr context. In a
    // constexpr context categories are sorted by id.

    class error_category final
    {
      public:
        using message_type                 = null_term<const char*>;
        using message_lookup_function_type = message_type (*)(i32, const message_type*, usize);

        template <same_as<const char> ConstChar, u32 IdN, u32 NameN>
        constexpr explicit error_category(ConstChar (&id)[IdN], ConstChar (&name)[NameN]) noexcept
            : id_data_{not_null{id}},
              name_data_{not_null{name}},
              id_size_{IdN - 1},
              name_size_{NameN - 1}
        {
        }

        template <same_as<const char> ConstChar, u32 IdN, u32 NameN, usize MessageCount>
        constexpr explicit error_category(
            ConstChar (&id)[IdN], ConstChar (&name)[NameN],
            const array<message_type, MessageCount>& message_lookup) noexcept
            : id_data_{not_null{id}},
              name_data_{not_null{name}},
              message_lookup_{message_lookup.begin()},
              message_count_{MessageCount},
              id_size_{IdN - 1},
              name_size_{NameN - 1}
        {
        }

        template <same_as<const char> ConstChar, u32 IdN, u32 NameN, usize MessageCount>
        explicit error_category(ConstChar (&)[IdN], ConstChar (&)[NameN],
                                const array<message_type, MessageCount>&&) = delete; // Temporary

        template <same_as<const char> ConstChar, u32 IdN, u32 NameN>
        constexpr explicit error_category(ConstChar (&id)[IdN], ConstChar (&name)[NameN],
                                          message_lookup_function_type lookup_function) noexcept
            : id_data_{not_null{id}},
              name_data_{not_null{name}},
              lookup_function_{lookup_function},
              id_size_{IdN - 1},
              name_size_{NameN - 1}
        {
            snn_assert(lookup_function != nullptr);
        }

        // Non-copyable
        error_category(const error_category&)            = delete;
        error_category& operator=(const error_category&) = delete;

        // Non-movable
        error_category(error_category&&)            = delete;
        error_category& operator=(error_category&&) = delete;

        ~error_category() = default; // "Rule of five".

        template <typename String>
        [[nodiscard]] constexpr String id() const
        {
            return String{id_data_, usize{id_size_}};
        }

        template <typename String>
        [[nodiscard]] constexpr String message(const i32 value) const
        {
            return lookup_function_(value, message_lookup_, message_count_).to<String>();
        }

        template <typename String>
        [[nodiscard]] constexpr String name() const
        {
            return String{name_data_, usize{name_size_}};
        }

        constexpr bool operator==(const error_category& other) const noexcept
        {
            return std::addressof(*this) == std::addressof(other);
        }

        constexpr std::strong_ordering operator<=>(const error_category& other) const noexcept
        {
            if (std::is_constant_evaluated())
            {
                if (std::addressof(*this) == std::addressof(other))
                {
                    return std::strong_ordering::equal;
                }
                else
                {
                    // Id must be unique so we can use it for ordering in a constexpr context.
                    const byte_size min_size{id_size_ < other.id_size_ ? id_size_ : other.id_size_};
                    const int c = mem::raw::compare(id_data_, other.id_data_, min_size);
                    if (const auto cmp = c <=> 0; cmp != 0)
                    {
                        return cmp;
                    }
                    return id_size_ <=> other.id_size_;
                }
            }
            else
            {
                return std::addressof(*this) <=> std::addressof(other);
            }
        }

      private:
        not_null<const char*> id_data_;
        not_null<const char*> name_data_;
        message_lookup_function_type lookup_function_ = error_category::lookup_;
        const message_type* message_lookup_           = nullptr;
        usize message_count_                          = 0;
        u32 id_size_;
        u32 name_size_;

        static constexpr message_type lookup_(const i32 value,
                                              const message_type* const message_lookup,
                                              const usize message_count) noexcept
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            message_type msg{""};
            if (value >= 0)
            {
                const auto index = to_usize(value);
                if (index < message_count)
                {
                    msg = message_lookup[index];
                }
            }
            return msg;

            SNN_DIAGNOSTIC_POP
        }
    };

    // ### error_code

    // An error code holds an integer value and a pointer to a category.

    // Forward declare.
    class error_code;

    // Forward declare.
    template <error_code_enum Enum>
    constexpr error_code make_error_code(Enum) noexcept;

    namespace detail
    {
        // Forward declare (for friend declaration).
        template <typename>
        class result_void_storage;
    }

    class error_code final
    {
      public:
        // #### Default constructor

        constexpr error_code() noexcept
            : category_{nullptr},
              value_{0}
        {
        }

        // #### Converting constructors

        constexpr error_code(const i32 value, const error_category& category) noexcept
            : category_{&category},
              value_{value}
        {
        }

        template <error_code_enum Enum>
        constexpr error_code(const Enum e) noexcept
            : error_code{make_error_code(e)}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return value_ != 0;
        }

        // #### Observers

        [[nodiscard]] constexpr const error_category& category() const noexcept
        {
            if (category_ != nullptr)
            {
                return *category_;
            }
            return empty_category_;
        }

        template <typename Buf>
        constexpr void format(strcore<Buf>& append_to) const
        {
            using array_view_type = array_view<const typename strcore<Buf>::value_type>;
            static_assert(std::is_same_v<array_view_type, cstrview>);
            append_to << category().name<array_view_type>() << ": " << message<array_view_type>()
                      << " (" << as_num(value_) << ')';
        }

        template <any_strcore Str = str>
        [[nodiscard]] constexpr Str format() const
        {
            Str append_to;
            format(append_to);
            return append_to;
        }

        [[nodiscard]] static constexpr bool has_boolean_state() noexcept
        {
            return false;
        }

        template <typename String>
        [[nodiscard]] constexpr String message() const
        {
            return category().message<String>(value_);
        }

        [[nodiscard]] constexpr i32 value() const noexcept
        {
            return value_;
        }

        [[nodiscard]] constexpr error_code non_zero_or(
            const not_zero<i32> value, const error_category& category) const noexcept
        {
            if (value_ != 0)
            {
                return *this;
            }
            return error_code{value.get(), category};
        }

        // #### Comparison

        constexpr bool operator==(const error_code other) const noexcept
        {
            return category_ == other.category_ && value_ == other.value_;
        }

        constexpr std::strong_ordering operator<=>(const error_code other) const noexcept
        {
            if (std::is_constant_evaluated())
            {
                if (const auto cmp = category() <=> other.category(); cmp != 0)
                {
                    return cmp;
                }
                return value_ <=> other.value_;
            }
            else
            {
                if (const auto cmp = category_ <=> other.category_; cmp != 0)
                {
                    return cmp;
                }
                return value_ <=> other.value_;
            }
        }

      private:
        friend detail::result_void_storage<error_code>;

        const error_category* category_;
        i32 value_;

        static constexpr error_category empty_category_{"", ""};

        constexpr explicit error_code(meta::internal_t, const i32 value,
                                      const error_category* category) noexcept
            : category_{category},
              value_{value}
        {
        }
    };

    // ## Functions

    // ### make_error_code

    template <error_code_enum Enum>
    [[nodiscard]] constexpr error_code make_error_code(const Enum) noexcept
    {
        static_assert(meta::always_false<Enum>,
                      "make_error_code() has not been overloaded for this enum.");
        return error_code{};
    }

    [[nodiscard]] constexpr error_code make_error_code(const i32 value,
                                                       const error_category& category) noexcept
    {
        return error_code{value, category};
    }
}
