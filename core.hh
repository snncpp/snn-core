// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Framework core functionality

#pragma once

#include <cassert>          // assert
#include <compare>          // common_comparison_category, *_ordering, ...
#include <concepts>         // same_as
#include <cstddef>          // nullptr_t, ptrdiff_t, size_t
#include <cstdint>          // int*_t, uint*_t, *_MAX, ...
#include <initializer_list> // initializer_list
#include <iterator>         // begin, end, iterator_traits, *_iterator_tag
#include <limits>           // numeric_limits
#include <tuple>            // ignore, tuple, tuple_element, tuple_size
#include <type_traits>      // is_*, make_*signed, ...
#include <utility>          // as_const, declval, exchange, forward, get, move, pair, swap

// ## Requirements

// ### C++20

static_assert(__cplusplus >= 202002L, "C++20 support is required.");

// ### Standard 64-bit

static_assert(__CHAR_BIT__ == 8, "char must be 8-bit.");
static_assert(sizeof(void*) == 8, "Pointer size must be 8 bytes (64-bit).");
static_assert(sizeof(std::size_t) == 8, "std::size_t must be 8 bytes (64-bit).");
static_assert(std::is_same_v<std::uint8_t, unsigned char>, "std::uint8_t must be unsigned char.");

// ### Little-endian

#if !defined(__BYTE_ORDER__) || !defined(__ORDER_LITTLE_ENDIAN__)
#error "Both __BYTE_ORDER__ and __ORDER_LITTLE_ENDIAN__ must be defined."
#elif __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "Little-endian required."
#endif

// ## Macros

// ### SNN_ADDRESS_SANITIZER_BOOL

// https://clang.llvm.org/docs/AddressSanitizer.html
// #conditional-compilation-with-has-feature-address-sanitizer
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define SNN_ADDRESS_SANITIZER_BOOL true
#endif
// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
#elif defined(__SANITIZE_ADDRESS__)
#if __SANITIZE_ADDRESS__
#define SNN_ADDRESS_SANITIZER_BOOL true
#endif
#endif

#if !defined(SNN_ADDRESS_SANITIZER_BOOL)
#define SNN_ADDRESS_SANITIZER_BOOL false
#endif

// ### snn_assert & snn_should[_if_not_fuzzing]

// Including constants SNN_ASSERT_BOOL and SNN_SHOULD_BOOL.

#if defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)

#if defined(__OPTIMIZE__)
#define snn_assert(e) (__builtin_expect(!!(e), 1) ? (void)0 : __builtin_trap())
#else
#define snn_assert(e) assert(e)
#endif
#define SNN_ASSERT_BOOL true

#define snn_should(e)   snn_assert(e)
#define SNN_SHOULD_BOOL true

#define snn_should_if_not_fuzzing(e) ((void)0)

#else // !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)

#if defined(NDEBUG)
#define snn_assert(e)   ((void)0)
#define SNN_ASSERT_BOOL false
#elif defined(__OPTIMIZE__)
#define snn_assert(e)   (__builtin_expect(!!(e), 1) ? (void)0 : __builtin_trap())
#define SNN_ASSERT_BOOL true
#else
#define snn_assert(e)   assert(e)
#define SNN_ASSERT_BOOL true
#endif

#if defined(NDEBUG) || defined(__OPTIMIZE__)
#define snn_should(e)   ((void)0)
#define SNN_SHOULD_BOOL false
#else
#define snn_should(e)   assert(e)
#define SNN_SHOULD_BOOL true
#endif

#define snn_should_if_not_fuzzing(e) snn_should(e)

#endif

// ### SNN_INT128_BOOL

#if defined(__SIZEOF_INT128__) && defined(_LIBCPP_VERSION)
#define SNN_INT128_BOOL true
#else
// In GNU C++ Standard Library (libstdc++) `std::is_integral_v<__int128_t>` is `false`.
#define SNN_INT128_BOOL false
#endif

namespace snn
{
    // ## Types

    using usize = std::size_t;
    using isize = std::int64_t;

    using uptr = std::uintptr_t;
    using iptr = std::intptr_t;

    using iptrdiff = std::ptrdiff_t;

    using u64 = std::uint64_t;
    using u32 = std::uint32_t;
    using u16 = std::uint16_t;
    using u8  = std::uint8_t;

    using u32fast = std::uint_fast32_t;
    using u16fast = std::uint_fast16_t;
    using u8fast  = std::uint_fast8_t;

    using i64 = std::int64_t;
    using i32 = std::int32_t;
    using i16 = std::int16_t;
    using i8  = std::int8_t;

    using i32fast = std::int_fast32_t;
    using i16fast = std::int_fast16_t;
    using i8fast  = std::int_fast8_t;

    using byte = std::uint8_t;

#if SNN_INT128_BOOL
    using u128 = __uint128_t;
    using i128 = __int128_t;
#endif

    // ## Promises

    namespace promise
    {
        struct has_capacity_t final
        {
            explicit has_capacity_t() = default;
        };
        inline constexpr has_capacity_t has_capacity;

        struct has_value_t final
        {
            explicit has_value_t() = default;
        };
        inline constexpr has_value_t has_value;

        struct is_sorted_t final
        {
            explicit is_sorted_t() = default;
        };
        inline constexpr is_sorted_t is_sorted;

        struct is_utf8_t final
        {
            explicit is_utf8_t() = default;
        };
        inline constexpr is_utf8_t is_utf8;

        struct is_valid_t final
        {
            explicit is_valid_t() = default;
        };
        inline constexpr is_valid_t is_valid;

        struct no_overlap_t final
        {
            explicit no_overlap_t() = default;
        };
        inline constexpr no_overlap_t no_overlap;

        struct not_empty_t final
        {
            explicit not_empty_t() = default;
        };
        inline constexpr not_empty_t not_empty;

        struct null_terminated_t final
        {
            explicit null_terminated_t() = default;
        };
        inline constexpr null_terminated_t null_terminated;

        struct within_bounds_t final
        {
            explicit within_bounds_t() = default;
        };
        inline constexpr within_bounds_t within_bounds;
    }

    // ## Meta helpers & tags

    namespace meta
    {
        // ### Tags

        // #### inplace

        using inplace_t = std::in_place_t;
        inline constexpr inplace_t inplace;

        // #### internal

        struct internal_t final
        {
            explicit internal_t() = default;
        };
        inline constexpr internal_t internal;

        // #### iterators

        struct iterators_t final
        {
            explicit iterators_t() = default;
        };
        inline constexpr iterators_t iterators;

        // ### Validation

        // #### all

        template <template <typename> typename Is, typename... Ts>
        struct all : public std::bool_constant<(Is<Ts>::value && ...)>
        {
        };

        template <template <typename> typename Is, typename... Ts>
        inline constexpr bool all_v = all<Is, Ts...>::value;

        // #### any

        template <template <typename> typename Is, typename... Ts>
        struct any : public std::bool_constant<(Is<Ts>::value || ...)>
        {
        };

        template <template <typename> typename Is, typename... Ts>
        inline constexpr bool any_v = any<Is, Ts...>::value;

        // #### none

        template <template <typename> typename Is, typename... Ts>
        struct none : public std::bool_constant<(!Is<Ts>::value && ...)>
        {
        };

        template <template <typename> typename Is, typename... Ts>
        inline constexpr bool none_v = none<Is, Ts...>::value;

        // ### Miscellaneous

        // #### always_false

        template <typename>
        inline constexpr bool always_false = false;

        // #### index

        template <usize Index>
        using index_t = std::integral_constant<usize, Index>;

        template <usize Index>
        inline constexpr index_t<Index> index;

        // #### type

        template <typename T>
        using type_t = std::type_identity<T>;

        template <typename T>
        inline constexpr type_t<T> type;
    }

    // ## Bounds tags

    namespace bounds
    {
        // ### mask

        struct mask_t final
        {
            explicit mask_t() = default;
        };
        inline constexpr mask_t mask;
    }

    // ## Container tags

    namespace container
    {
        struct do_not_initialize_t final
        {
            explicit do_not_initialize_t() = default;
        };
        inline constexpr do_not_initialize_t do_not_initialize;

        struct fill_t final
        {
            explicit fill_t() = default;
        };
        inline constexpr fill_t fill;

        struct reserve_t final
        {
            explicit reserve_t() = default;
        };
        inline constexpr reserve_t reserve;
    }

    // ## Type traits

    // ### is_strict_integral

    // "strict_integral" requirements:
    // * The type must be a power-of-two integral.
    // * The type must use all `sizeof(T) * __CHAR_BIT__` bits.
    // * The type must support `std::make_signed`.
    // * The type must support `std::make_unsigned`.
    // * The type must support `std::numeric_limits`.

    // Notable types that are not included are `bool` and `_BitInt(N)`.

    namespace detail
    {
        template <typename T>
        struct is_strict_integral_strict : public std::false_type
        {
        };

        template <>
        struct is_strict_integral_strict<char> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<signed char> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<unsigned char> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<wchar_t> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<char8_t> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<char16_t> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<char32_t> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<short> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<unsigned short> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<int> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<unsigned int> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<long> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<unsigned long> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<long long> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<unsigned long long> : public std::true_type
        {
        };

#if SNN_INT128_BOOL
        template <>
        struct is_strict_integral_strict<i128> : public std::true_type
        {
        };

        template <>
        struct is_strict_integral_strict<u128> : public std::true_type
        {
        };
#endif
    }

    template <typename T>
    struct is_strict_integral : public detail::is_strict_integral_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_strict_integral_v = is_strict_integral<T>::value;

    // clang-format off

    // ## Core concepts

    // ### arithmetic

    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    // ### convertible_to

    template <typename From, typename To>
    concept convertible_to = std::is_convertible_v<From, To>;

    // ### floating_point

    template <typename T>
    concept floating_point = std::is_floating_point_v<T>;

    // ### pointer

    template <typename T>
    concept pointer = std::is_pointer_v<T>;

    // ### same_as

    using std::same_as;

    // ### sane

    // This framework assumes that all types are "sane":
    // * If a type is copy constructible it must also be move constructible.
    // * If a type is copy assignable it must also be move assignable.
    // * If a type is move constructible it must also be nothrow move constructible.
    // * If a type is move assignable it must also be nothrow move assignable.

    template <typename T>
    concept sane = (!std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>) &&
                   (!std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>) &&
                   (!std::is_move_constructible_v<T> || std::is_nothrow_move_constructible_v<T>) &&
                   (!std::is_move_assignable_v<T> || std::is_nothrow_move_assignable_v<T>);

    // ### value_type_or

    // `value_type_or<T&>` will accept `T` or `T&`, but not `const T&`.
    // `value_type_or<const T&>` will accept `T` or `const T&`, but not `T&`.

    template <typename T, typename U>
    concept value_type_or = std::is_same_v<T, U> || std::is_same_v<T, std::remove_cvref_t<U>>;

    // ## Constructible concepts

    // ### brace_constructible_from

    template <typename T, typename... Args>
    concept brace_constructible_from = requires(Args&&... args) { T{std::forward<Args>(args)...}; };

    // ### constructible_from

    template <typename T, typename... Args>
    concept constructible_from = std::is_constructible_v<T, Args...>;

    // ### constructible_from_iterators

    template <typename T>
    concept constructible_from_iterators
        = requires(T& v) { T{meta::iterators, v.begin(), v.end()}; };

    // ### implicitly_default_constructible

    namespace detail
    {
        template <typename T>
        void takes_single_value_of(T);
    }

    template <typename T>
    concept implicitly_default_constructible = requires { detail::takes_single_value_of<T>({}); };

    // ### explicitly_default_constructible

    template <typename T>
    concept explicitly_default_constructible
        = std::is_default_constructible_v<T> && !implicitly_default_constructible<T>;

    // ## Callable concepts

    // ### callable

    // Callable without the overhead of `std::invoke(...)`.

    template <typename Fn, typename... Args>
    concept callable = requires(Fn&& fn, Args&&... args) {
        std::forward<Fn>(fn)(std::forward<Args>(args)...);
    };

    // ### predicate

    // Callable that doesn't modify the parameters, with a boolean-testable return value.

    template <typename P, typename... Args>
    concept predicate = requires(P& p, const Args&... args) { {!p(args...)} -> same_as<bool>; };

    // ## Integral concepts

    // ### integral

    template <typename T>
    concept integral = std::is_integral_v<T>;

    // ### strict_integral

    template <typename T>
    concept strict_integral = integral<T> && is_strict_integral_v<T>;

    // ### strict_integral_min

    template <typename T, usize BitCount>
    concept strict_integral_min = strict_integral<T> && (sizeof(T) * __CHAR_BIT__) >= BitCount;

    // ### signed_integral

    template <typename T>
    concept signed_integral = integral<T> && std::is_signed_v<T>;

    // ### strict_signed_integral

    template <typename T>
    concept strict_signed_integral = signed_integral<T> && strict_integral<T>;

    // ### unsigned_integral

    template <typename T>
    concept unsigned_integral = integral<T> && !signed_integral<T>;

    // ### strict_unsigned_integral

    template <typename T>
    concept strict_unsigned_integral = unsigned_integral<T> && strict_integral<T>;

    // ### character

#if defined(__CHAR_UNSIGNED__)
    template <typename T>
    concept character = strict_unsigned_integral<T> && same_as<const T, const char>;
#else
    template <typename T>
    concept character = strict_signed_integral<T> && same_as<const T, const char>;
#endif

    // ### same_signedness_as

    template <typename T, typename U>
    concept same_signedness_as
        = integral<T> && integral<U> && (std::is_signed_v<T> == std::is_signed_v<U>);

    // ## Iterable concepts

    // ### legacy_iterable

    template <typename T>
    concept legacy_iterable = requires(T& v) { {v.begin()} -> same_as<decltype(v.end())>; };

    // ## Non-type concepts

    // ### power_of_two

    template <usize N>
    concept power_of_two = N > 0 && (N & (N - 1)) == 0;

    // ## Has concepts

    // ### has_append_inplace

    template <typename T>
    concept has_append_inplace = requires(T& v) { v.append_inplace(); };

    // ### has_at

    template <typename T>
    concept has_at = requires(T& v) { v.at(usize{}, promise::within_bounds); };

    // ### has_contiguous_iterator

    template <typename T>
    concept has_contiguous_iterator = requires(T& v) { {v.begin()} -> pointer; };

    // ### has_count

    template <typename T>
    concept has_count = requires(T& v) { {v.count()} -> unsigned_integral; };

    // ### has_drop_back

    template <typename T>
    concept has_drop_back = requires(T& v) { v.drop_back(promise::not_empty); };

    // ### has_drop_front

    template <typename T>
    concept has_drop_front = requires(T& v) { v.drop_front(promise::not_empty); };

    // ### has_to

    template <typename From, typename To>
    concept has_to = requires(const From& v) { v.template to<To>(); };

    // clang-format on

    // ## Type traits

    // ### deep_const

    namespace detail
    {
        template <typename T>
        struct deep_const_strict
        {
            using type = T;
        };

        template <typename T>
        struct deep_const_strict<T&>
        {
            using type = const T&;
        };

        template <typename T>
        struct deep_const_strict<T&&>
        {
            using type = const T&&;
        };

        template <typename T>
        struct deep_const_strict<T*>
        {
            using type = const T*;
        };
    }

    template <typename T>
    struct deep_const : public detail::deep_const_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    using deep_const_t = typename deep_const<T>::type;

    // ### front_value

    template <typename T>
    struct front_value
    {
        using type = std::remove_cvref_t<decltype(std::declval<T>().front(promise::not_empty))>;
    };

    template <typename T>
    using front_value_t = typename front_value<T>::type;

    // ### is_trivially_relocatable

    // A type `T` is trivially relocatable if `std::is_trivially_copyable_v<T>` is true or if it has
    // a member type `trivially_relocatable_type` which is `T`.

    namespace detail
    {
        template <typename, typename = void>
        struct has_trivially_relocatable_type_member : public std::false_type
        {
        };

        template <typename T>
        struct has_trivially_relocatable_type_member<
            T, std::void_t<typename T::trivially_relocatable_type>>
            : public std::is_same<typename T::trivially_relocatable_type, T>
        {
        };

        template <typename T>
        struct is_trivially_relocatable_strict
            : public std::bool_constant<std::is_trivially_copyable_v<T> ||
                                        has_trivially_relocatable_type_member<T>::value>
        {
        };
    }

    template <typename T>
    struct is_trivially_relocatable
        : public detail::is_trivially_relocatable_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_trivially_relocatable_v = is_trivially_relocatable<T>::value;

    // ### not_deduced

    template <typename T>
    struct not_deduced
    {
        using type = T;
    };

    template <typename T>
    using not_deduced_t = typename not_deduced<T>::type;

    // ### promote_integral

    // Promote integral type to minimum N-bit integral type with the same sign.

    namespace detail
    {
        template <usize BitCount, bool IsSigned>
        struct promote_integral_lookup
        {
        };

        template <>
        struct promote_integral_lookup<8, true>
        {
            using type = i8;
        };

        template <>
        struct promote_integral_lookup<8, false>
        {
            using type = u8;
        };

        template <>
        struct promote_integral_lookup<16, true>
        {
            using type = i16;
        };

        template <>
        struct promote_integral_lookup<16, false>
        {
            using type = u16;
        };

        template <>
        struct promote_integral_lookup<32, true>
        {
            using type = i32;
        };

        template <>
        struct promote_integral_lookup<32, false>
        {
            using type = u32;
        };

        template <>
        struct promote_integral_lookup<64, true>
        {
            using type = i64;
        };

        template <>
        struct promote_integral_lookup<64, false>
        {
            using type = u64;
        };

#if SNN_INT128_BOOL
        template <>
        struct promote_integral_lookup<128, true>
        {
            using type = i128;
        };

        template <>
        struct promote_integral_lookup<128, false>
        {
            using type = u128;
        };
#endif

        template <usize BitCount, bool IsSigned>
        using promote_integral_lookup_t =
            typename promote_integral_lookup<BitCount, IsSigned>::type;
    }

    template <integral Int, usize BitCount>
    struct promote_integral
    {
      private:
        static constexpr usize int_bit_count_ = sizeof(Int) * __CHAR_BIT__;
        static constexpr usize bit_count_ = int_bit_count_ < BitCount ? BitCount : int_bit_count_;

      public:
        using type = detail::promote_integral_lookup_t<bit_count_, std::is_signed_v<Int>>;
    };

    template <integral Int, usize BitCount>
    using promote_integral_t = typename promote_integral<Int, BitCount>::type;

    // ### remove_cv_rvalue_ref

    template <typename T>
    struct remove_cv_rvalue_ref
    {
        using type = std::remove_cvref_t<T>;
    };

    template <typename T>
    struct remove_cv_rvalue_ref<T&>
    {
        using type = T&;
    };

    template <typename T>
    using remove_cv_rvalue_ref_t = typename remove_cv_rvalue_ref<T>::type;

    // ### trivially_relocatable_if

    // Provides the first type if the types following it are all trivially relocatable,
    // or `void` if not.

    template <typename T, typename... Ts>
    struct trivially_relocatable_if
        : public std::conditional<meta::all_v<is_trivially_relocatable, Ts...>, T, void>
    {
        static_assert(sizeof...(Ts) > 0, "No types to check if trivially relocatable.");
    };

    template <typename T, typename... Ts>
    using trivially_relocatable_if_t = typename trivially_relocatable_if<T, Ts...>::type;

    // ## Range concepts

    // ### input_range

    template <typename T>
    concept input_range = has_drop_front<T>;

    // ### forward_range

    template <typename T>
    concept forward_range = input_range<T> && std::is_copy_constructible_v<T>;

    // ### bidirectional_range

    template <typename T>
    concept bidirectional_range = forward_range<T> && has_drop_back<T>;

    // ### random_access_range

    template <typename T>
    concept random_access_range = bidirectional_range<T> && has_at<T>;

    // ### contiguous_range

    template <typename T>
    concept contiguous_range = random_access_range<T> && has_contiguous_iterator<T>;

    // ## Initializer list

    // Make accessible for unqualified lookup.
    using std::initializer_list;

    // Alias (remember that alias templates are currently not deduced by Clang).
    template <typename T>
    using init_list = initializer_list<T>;

    // ## Constants

    namespace constant
    {
        // ### General

        inline constexpr usize bits_per_byte = __CHAR_BIT__;
        inline constexpr usize dynamic_count = SIZE_MAX;
        inline constexpr usize npos          = SIZE_MAX;

        template <typename T>
        inline constexpr T value_initialized{};

        // ### Exit status

        namespace exit
        {
            inline constexpr int failure = 1;
            inline constexpr int success = 0;
        }

        // ### Integral limit

        template <integral Int>
        struct limit final
        {
            static constexpr Int min = std::numeric_limits<Int>::min();
            static constexpr Int max = std::numeric_limits<Int>::max();
        };

        // ### Floating point limit

        namespace fp
        {
            template <floating_point Fp>
            struct limit final
            {
                static constexpr Fp min_negative = std::numeric_limits<Fp>::lowest();
                static constexpr Fp min_positive = std::numeric_limits<Fp>::min();
                static constexpr Fp max          = std::numeric_limits<Fp>::max();
                static constexpr Fp infinity     = std::numeric_limits<Fp>::infinity();
                static constexpr Fp nan          = std::numeric_limits<Fp>::quiet_NaN();
            };
        }

        // ### Byte sizes

        namespace size
        {
            // #### Metric

            template <strict_integral_min<16> Int>
            inline constexpr Int kilobyte = 1'000; // kB

            template <strict_integral_min<32> Int>
            inline constexpr Int megabyte = 1'000'000; // MB

            template <strict_integral_min<32> Int>
            inline constexpr Int gigabyte = 1'000'000'000; // GB

            template <strict_integral_min<64> Int>
            inline constexpr Int terabyte = 1'000'000'000'000; // TB

            template <strict_integral_min<64> Int>
            inline constexpr Int petabyte = 1'000'000'000'000'000; // PB

            template <strict_integral_min<64> Int>
            inline constexpr Int exabyte = 1'000'000'000'000'000'000; // EB

            // #### IEC

            template <strict_integral_min<16> Int>
            inline constexpr Int kibibyte = 1'024; // KiB

            template <strict_integral_min<32> Int>
            inline constexpr Int mebibyte = 1'048'576; // MiB

            template <strict_integral_min<32> Int>
            inline constexpr Int gibibyte = 1'073'741'824; // GiB

            template <strict_integral_min<64> Int>
            inline constexpr Int tebibyte = 1'099'511'627'776; // TiB

            template <strict_integral_min<64> Int>
            inline constexpr Int pebibyte = 1'125'899'906'842'624; // PiB

            template <strict_integral_min<64> Int>
            inline constexpr Int exbibyte = 1'152'921'504'606'846'976; // EiB
        }
    }

    // ## Integral conversion functions (non-destructive bit casts)

    // Note: C++20 requires that signed integers are two's complement.

    template <strict_integral Int>
    [[nodiscard]] constexpr auto force_signed(const Int i) noexcept
    {
        return static_cast<std::make_signed_t<Int>>(i);
    }

    template <strict_integral Int>
    [[nodiscard]] constexpr auto force_unsigned(const Int i) noexcept
    {
        return static_cast<std::make_unsigned_t<Int>>(i);
    }

    [[nodiscard]] constexpr char to_char(const byte b) noexcept
    {
        return static_cast<char>(b);
    }

    [[nodiscard]] constexpr byte to_byte(const char b) noexcept
    {
        return static_cast<byte>(b);
    }

    [[nodiscard]] constexpr i8 to_i8(const u8 i) noexcept
    {
        return static_cast<i8>(i);
    }

    [[nodiscard]] constexpr u8 to_u8(const i8 i) noexcept
    {
        return static_cast<u8>(i);
    }

    [[nodiscard]] constexpr i16 to_i16(const u16 i) noexcept
    {
        return static_cast<i16>(i);
    }

    [[nodiscard]] constexpr u16 to_u16(const i16 i) noexcept
    {
        return static_cast<u16>(i);
    }

    [[nodiscard]] constexpr i32 to_i32(const u32 i) noexcept
    {
        return static_cast<i32>(i);
    }

    [[nodiscard]] constexpr u32 to_u32(const i32 i) noexcept
    {
        return static_cast<u32>(i);
    }

    [[nodiscard]] constexpr i64 to_i64(const u64 i) noexcept
    {
        return static_cast<i64>(i);
    }

    [[nodiscard]] constexpr u64 to_u64(const i64 i) noexcept
    {
        return static_cast<u64>(i);
    }

    [[nodiscard]] constexpr isize to_isize(const usize i) noexcept
    {
        return static_cast<isize>(i);
    }

    [[nodiscard]] constexpr usize to_usize(const isize i) noexcept
    {
        return static_cast<usize>(i);
    }

    // ## Utility functions

    // Compile-time array count.
    template <typename T, usize N>
    [[nodiscard]] consteval usize array_count(T (&)[N]) noexcept
    {
        return N;
    }

    // Ignore unused variables, to prevent warnings in non-debug builds.
    template <typename... Args>
    constexpr void ignore_if_unused(const Args&...) noexcept
    {
    }

    // Promote integral to minimum N-bit (32 by default) integral type with the same sign.
    template <usize BitCount = 32, integral Int>
    [[nodiscard]] constexpr auto promote(const Int i) noexcept
    {
        return promote_integral_t<Int, BitCount>{i};
    }

    // String literal size in bytes, excluding trailing null character.
    template <same_as<const char> ConstChar, usize N>
    [[nodiscard]] consteval usize string_size(ConstChar (&)[N]) noexcept
    {
        return N - 1;
    }

    // Convert enum to underlying type.
    template <typename E>
        requires(std::is_enum_v<E>)
    [[nodiscard]] constexpr auto to_underlying(const E e) noexcept
    {
        return static_cast<std::underlying_type_t<E>>(e);
    }

    // ## Helpers

    // ### loop

    // Loop count helper.

    namespace loop
    {
        struct count final
        {
            usize value{};

            [[nodiscard]] constexpr bool operator--(int) noexcept
            {
                if (value > 0)
                {
                    --value;
                    return true;
                }
                return false;
            }
        };
    }

    // ## Wrappers

    // ### byte_size

    class byte_size final
    {
      public:
        constexpr explicit byte_size(const usize size) noexcept
            : size_{size}
        {
        }

        [[nodiscard]] constexpr usize get() const noexcept
        {
            return size_;
        }

        constexpr auto operator<=>(const byte_size&) const noexcept = default;

      private:
        usize size_;
    };

    // ### not_null

    template <pointer Ptr>
    class not_null final
    {
      public:
        constexpr explicit not_null(const Ptr ptr) noexcept
            : ptr_{ptr}
        {
            snn_should(ptr_ != nullptr);
        }

        explicit not_null(std::nullptr_t) = delete;

        template <convertible_to<Ptr> P>
        constexpr not_null(const not_null<P> other) noexcept
            : ptr_{other.get()}
        {
        }

        [[nodiscard]] constexpr Ptr get() const noexcept
        {
            return ptr_;
        }

      private:
        Ptr ptr_;
    };

    // ### not_zero

    template <integral Int>
    class not_zero final
    {
      public:
        constexpr explicit not_zero(const Int value) noexcept
            : value_{value}
        {
            snn_should(value_ != 0);
        }

        [[nodiscard]] constexpr Int get() const noexcept
        {
            return value_;
        }

        constexpr auto operator<=>(const not_zero&) const noexcept = default;

      private:
        Int value_;
    };

    // ### numeric

    template <arithmetic Num>
    class numeric final
    {
      public:
        constexpr explicit numeric(const Num value) noexcept
            : value_{value}
        {
        }

        [[nodiscard]] constexpr Num get() const noexcept
        {
            return value_;
        }

      private:
        Num value_;
    };

    template <arithmetic Num>
    [[nodiscard]] constexpr numeric<Num> as_num(const Num v) noexcept
    {
        return numeric<Num>{v};
    }

    // ### semi_const

    // Can be moved but not assigned to.

    template <typename T>
        requires(!std::is_reference_v<T> && !std::is_const_v<T>)
    class semi_const final
    {
      public:
        // #### Constructors & assignment operators

        // Converting constructor.

        constexpr semi_const(T value) noexcept
            : value_{std::move(value)}
        {
        }

        // Copy-constructible but not copy-assignable.

        semi_const(const semi_const&)            = default;
        semi_const& operator=(const semi_const&) = delete;

        // Move-constructible but not move-assignable.

        semi_const(semi_const&&)            = default;
        semi_const& operator=(semi_const&&) = delete;

        // #### Get

        [[nodiscard]] constexpr const T& get() const noexcept
        {
            return value_;
        }

        // #### Comparison

        constexpr auto operator<=>(const semi_const&) const = default;

      private:
        T value_;
    };

    // ### transient

    // Make type T bind to temporaries.

    template <typename T>
    class transient final
    {
      public:
        // #### Converting constructor

        template <typename U>
            requires(std::is_convertible_v<const U&, T>)
        constexpr transient(const U& u) noexcept(noexcept(T{u}))
            : value_{u}
        {
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

      private:
        T value_;
    };
}
