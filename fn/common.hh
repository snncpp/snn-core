// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common function objects

// Some function objects have an `is_transparent` member type, its use is explained here:
// https://en.cppreference.com/w/cpp/utility/functional/less_void#Notes (CC-BY-SA)
// "The member type is_transparent indicates to the caller that this function object is a
// transparent function object: it accepts arguments of arbitrary types and uses perfect forwarding,
// which avoids unnecessary copying and conversion when the function object is used in heterogeneous
// context, or with rvalue arguments."

#pragma once

#include "snn-core/array_view.fwd.hh"
#include <functional> // invoke, less<void>

namespace snn::fn
{
    // ## Arithmetic

    // These could be aliased to `std::plus<void>` and so on, but both Clang and GCC will suppress
    // warnings when using function objects defined in system headers. With these function objects
    // we get warnings _and_ `[[nodiscard]]`.

    // ### add

    struct add final
    {
        explicit add() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) + std::forward<U>(u)))
        {
            return std::forward<T>(t) + std::forward<U>(u);
        }

        using is_transparent = void;
    };

    // ### divide

    struct divide final
    {
        explicit divide() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) / std::forward<U>(u)))
        {
            return std::forward<T>(t) / std::forward<U>(u);
        }

        using is_transparent = void;
    };

    // ### modulo

    struct modulo final
    {
        explicit modulo() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) % std::forward<U>(u)))
        {
            return std::forward<T>(t) % std::forward<U>(u);
        }

        using is_transparent = void;
    };

    // ### multiply

    struct multiply final
    {
        explicit multiply() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) * std::forward<U>(u)))
        {
            return std::forward<T>(t) * std::forward<U>(u);
        }

        using is_transparent = void;
    };

    // ### negate

    struct negate final
    {
        explicit negate() = default;

        template <typename T>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& v) const
            noexcept(noexcept(-std::forward<T>(v)))
        {
            return -std::forward<T>(v);
        }

        using is_transparent = void;
    };

    // ### subtract

    struct subtract final
    {
        explicit subtract() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) - std::forward<U>(u)))
        {
            return std::forward<T>(t) - std::forward<U>(u);
        }

        using is_transparent = void;
    };

    // ## Comparison

    // These could be aliased to `std::equal_to<void>` and so on, but both Clang and GCC will
    // suppress warnings when using function objects defined in system headers. With these function
    // objects we get warnings _and_ `[[nodiscard]]`.

    // `less_than` will use `std::less<void>` internally for pointer types.

    // ### equal_to

    struct equal_to final
    {
        explicit equal_to() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) == std::forward<U>(u)))
        {
            return std::forward<T>(t) == std::forward<U>(u);
        }

        using is_transparent = void;
    };

    using eq = equal_to; // Idiomatic shorthand version.

    // ### greater_than

    struct greater_than final
    {
        explicit greater_than() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) > std::forward<U>(u)))
        {
            return std::forward<T>(t) > std::forward<U>(u);
        }

        using is_transparent = void;
    };

    using gt = greater_than; // Idiomatic shorthand version.

    // ### greater_than_or_equal_to

    struct greater_than_or_equal_to final
    {
        explicit greater_than_or_equal_to() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) >= std::forward<U>(u)))
        {
            return std::forward<T>(t) >= std::forward<U>(u);
        }

        using is_transparent = void;
    };

    using gte = greater_than_or_equal_to; // Idiomatic shorthand version.

    // ### less_than

    struct less_than final
    {
        explicit less_than() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) < std::forward<U>(u)))
            requires(!pointer<T> || !pointer<U>)
        {
            return std::forward<T>(t) < std::forward<U>(u);
        }

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::less<void>{}(std::forward<T>(t), std::forward<U>(u))))
            requires(pointer<T> && pointer<U>)
        {
            // `std::less<void>` is well defined for pointers.
            return std::less<void>{}(std::forward<T>(t), std::forward<U>(u));
        }

        using is_transparent = void;
    };

    using lt = less_than; // Idiomatic shorthand version.

    // ### less_than_or_equal_to

    struct less_than_or_equal_to final
    {
        explicit less_than_or_equal_to() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) <= std::forward<U>(u)))
        {
            return std::forward<T>(t) <= std::forward<U>(u);
        }

        using is_transparent = void;
    };

    using lte = less_than_or_equal_to; // Idiomatic shorthand version.

    // ### not_equal_to

    struct not_equal_to final
    {
        explicit not_equal_to() = default;

        template <typename T, typename U>
        [[nodiscard]] constexpr decltype(auto) operator()(T&& t, U&& u) const
            noexcept(noexcept(std::forward<T>(t) != std::forward<U>(u)))
        {
            return std::forward<T>(t) != std::forward<U>(u);
        }

        using is_transparent = void;
    };

    using ne = not_equal_to; // Idiomatic shorthand version.

    // ## Negators

    // ### _not

    // Wrap and negate the return value of another callable object.

    template <typename F>
    class _not final
    {
      public:
        constexpr explicit _not(F f) noexcept
            : f_{std::move(f)}
        {
        }

        template <typename... Args>
        [[nodiscard]] constexpr bool operator()(Args&&... args) const
            noexcept(noexcept(f_(std::forward<Args>(args)...)))
        {
            static_assert(std::is_same_v<decltype(!f_(std::forward<Args>(args)...)), bool>);
            return !f_(std::forward<Args>(args)...);
        }

      private:
        F f_;
    };

    // ## Incrementing/Decrementing

    // ### incrementing

    template <typename T>
    class incrementing final
    {
      public:
        constexpr explicit incrementing(T initial) noexcept
            : v_{std::move(initial)}
        {
        }

        [[nodiscard]] constexpr decltype(auto) operator()() noexcept(noexcept(v_++))
        {
            return v_++;
        }

      private:
        T v_;
    };

    // ### decrementing

    template <typename T>
    class decrementing final
    {
      public:
        constexpr explicit decrementing(T initial) noexcept
            : v_{std::move(initial)}
        {
        }

        [[nodiscard]] constexpr decltype(auto) operator()() noexcept(noexcept(v_--))
        {
            return v_--;
        }

      private:
        T v_;
    };

    // ## Min/Max

    // ### max

    template <typename TwoArgPred = fn::less_than>
    class max final
    {
      public:
        constexpr explicit max(TwoArgPred is_less = TwoArgPred{}) noexcept
            : is_less_{std::move(is_less)}
        {
        }

        template <typename T>
        [[nodiscard]] constexpr T& operator()(T& a, T& b) const noexcept(noexcept(is_less_(a, b)))
        {
            return is_less_(a, b) ? b : a;
        }

        template <typename T>
        [[nodiscard]] constexpr const T& operator()(const T& a, const T& b) const
            noexcept(noexcept(is_less_(a, b)))
        {
            return is_less_(a, b) ? b : a;
        }

        // Don't allow rvalues (will return a dangling reference).

        template <typename T>
        void operator()(const T&, const T&&) const = delete;

        template <typename T>
        void operator()(const T&&, const T&) const = delete;

        template <typename T>
        void operator()(const T&&, const T&&) const = delete;

      private:
        TwoArgPred is_less_;
    };

    // ### min

    template <typename TwoArgPred = fn::less_than>
    class min final
    {
      public:
        constexpr explicit min(TwoArgPred is_less = TwoArgPred{}) noexcept
            : is_less_{std::move(is_less)}
        {
        }

        template <typename T>
        [[nodiscard]] constexpr T& operator()(T& a, T& b) const noexcept(noexcept(is_less_(b, a)))
        {
            return is_less_(b, a) ? b : a;
        }

        template <typename T>
        [[nodiscard]] constexpr const T& operator()(const T& a, const T& b) const
            noexcept(noexcept(is_less_(b, a)))
        {
            return is_less_(b, a) ? b : a;
        }

        // Don't allow rvalues (will return a dangling reference).

        template <typename T>
        void operator()(const T&, const T&&) const = delete;

        template <typename T>
        void operator()(const T&&, const T&) const = delete;

        template <typename T>
        void operator()(const T&&, const T&&) const = delete;

      private:
        TwoArgPred is_less_;
    };

    // ## Filter / Validation

    // ### all

    template <typename OneArgPred>
    class all final
    {
      public:
        constexpr explicit all(OneArgPred p) noexcept
            : p_{std::move(p)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return p_(v) && (p_(vals) && ...);
        }

      private:
        OneArgPred p_;
    };

    // ### any

    template <typename OneArgPred>
    class any final
    {
      public:
        constexpr explicit any(OneArgPred p) noexcept
            : p_{std::move(p)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return p_(v) || (p_(vals) || ...);
        }

      private:
        OneArgPred p_;
    };

    // ### none

    template <typename OneArgPred>
    class none final
    {
      public:
        constexpr explicit none(OneArgPred p) noexcept
            : p_{std::move(p)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return !p_(v) && (!p_(vals) && ...);
        }

      private:
        OneArgPred p_;
    };

    // ### contains

    template <typename T>
    class contains final
    {
      public:
        constexpr explicit contains(not_deduced_t<T> value)
            : value_{std::forward<T>(value)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return v.contains(value_) && (vals.contains(value_) && ...);
        }

      private:
        T value_;
    };

    template <typename T>
    contains(T&&) -> contains<T>;

    // ### has_back

    template <typename T>
    class has_back final
    {
      public:
        constexpr explicit has_back(not_deduced_t<T> value)
            : value_{std::forward<T>(value)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return v.has_back(value_) && (vals.has_back(value_) && ...);
        }

      private:
        T value_;
    };

    template <typename T>
    has_back(T&&) -> has_back<T>;

    // ### has_front

    template <typename T>
    class has_front final
    {
      public:
        constexpr explicit has_front(not_deduced_t<T> value)
            : value_{std::forward<T>(value)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return v.has_front(value_) && (vals.has_front(value_) && ...);
        }

      private:
        T value_;
    };

    template <typename T>
    has_front(T&&) -> has_front<T>;

    // ### in_array

    template <typename T, usize Count>
    class in_array final
    {
      public:
        static_assert(Count > 0);

        template <typename... Args>
        constexpr explicit in_array(not_deduced_t<T> first, Args&&... args)
            : array_{std::move(first), std::forward<Args>(args)...}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            constexpr auto index_seq = std::make_index_sequence<Count>{};
            return contains_(v, index_seq) && (contains_(vals, index_seq) && ...);
        }

      private:
        T array_[Count];

        template <typename V, usize... Indexes>
        constexpr bool contains_(const V& v, std::index_sequence<Indexes...>) const
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            return ((v == array_[Indexes]) || ...);

            SNN_DIAGNOSTIC_POP
        }
    };

    template <typename T, typename... Tn>
    in_array(T, Tn...) -> in_array<T, 1 + sizeof...(Tn)>;
    template <usize N, typename... Tn>
    in_array(const char (&)[N], Tn...) -> in_array<cstrview, 1 + sizeof...(Tn)>;

    // ### in_interval

    // Interval with exclusive end, i.e. a half-open interval "[Lower, Upper)".

    template <typename LowerIncl, typename UpperExcl = LowerIncl,
              typename TwoArgPred = fn::less_than>
    class in_interval final
    {
      public:
        constexpr explicit in_interval(not_deduced_t<LowerIncl> lower,
                                       not_deduced_t<UpperExcl> upper,
                                       not_deduced_t<TwoArgPred> is_less = TwoArgPred{})
            : lower_{std::forward<LowerIncl>(lower)},
              upper_{std::forward<UpperExcl>(upper)},
              is_less_{std::move(is_less)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return in_interval_(v) && (in_interval_(vals) && ...);
        }

      private:
        LowerIncl lower_;
        UpperExcl upper_;
        TwoArgPred is_less_;

        template <typename V>
        constexpr bool in_interval_(const V& v) const
        {
            // `v >= lower_ && v < upper_` but with `is_less_` predicate.
            return !is_less_(v, lower_) && is_less_(v, upper_);
        }
    };

    template <typename LowerIncl, typename UpperExcl>
    in_interval(LowerIncl&&, UpperExcl&&) -> in_interval<LowerIncl, UpperExcl>;
    template <typename LowerIncl, typename UpperExcl, typename TwoArgPred>
    in_interval(LowerIncl&&, UpperExcl&&, TwoArgPred) //
        ->in_interval<LowerIncl, UpperExcl, TwoArgPred>;

    // ### in_interval_incl

    // Interval with inclusive end, i.e. a closed interval "[Lower, Upper]".

    template <typename LowerIncl, typename UpperIncl = LowerIncl,
              typename TwoArgPred = fn::less_than>
    class in_interval_incl final
    {
      public:
        constexpr explicit in_interval_incl(not_deduced_t<LowerIncl> lower,
                                            not_deduced_t<UpperIncl> upper,
                                            not_deduced_t<TwoArgPred> is_less = TwoArgPred{})
            : lower_{std::forward<LowerIncl>(lower)},
              upper_{std::forward<UpperIncl>(upper)},
              is_less_{std::move(is_less)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return in_interval_incl_(v) && (in_interval_incl_(vals) && ...);
        }

      private:
        LowerIncl lower_;
        UpperIncl upper_;
        TwoArgPred is_less_;

        template <typename V>
        constexpr bool in_interval_incl_(const V& v) const
        {
            // `v >= lower_ && v <= upper_` but with `is_less_` predicate.
            return !is_less_(v, lower_) && !is_less_(upper_, v);
        }
    };

    template <typename LowerIncl, typename UpperIncl>
    in_interval_incl(LowerIncl&&, UpperIncl&&) -> in_interval_incl<LowerIncl, UpperIncl>;
    template <typename LowerIncl, typename UpperIncl, typename TwoArgPred>
    in_interval_incl(LowerIncl&&, UpperIncl&&, TwoArgPred) //
        ->in_interval_incl<LowerIncl, UpperIncl, TwoArgPred>;

    // ### in_tuple

    template <typename... Types>
    class in_tuple final
    {
      public:
        constexpr explicit in_tuple(not_deduced_t<Types>... args)
            : tuple_{std::forward<Types>(args)...}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            constexpr auto index_seq = std::make_index_sequence<sizeof...(Types)>{};
            return contains_(v, index_seq) && (contains_(vals, index_seq) && ...);
        }

      private:
        std::tuple<Types...> tuple_;

        template <typename V, usize... Indexes>
        constexpr bool contains_(const V& v, std::index_sequence<Indexes...>) const
        {
            return ((v == std::get<Indexes>(tuple_)) || ...);
        }
    };

    template <typename... Types>
    in_tuple(Types&&...) -> in_tuple<Types...>;

    // ### is

    template <typename TwoArgPred, typename T>
    class is final
    {
      public:
        constexpr explicit is(not_deduced_t<TwoArgPred> pred, not_deduced_t<T> value)
            : pred_{std::move(pred)},
              value_{std::forward<T>(value)}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return pred_(v, value_) && (pred_(vals, value_) && ...);
        }

      private:
        TwoArgPred pred_;
        T value_;
    };

    template <typename TwoArgPred, typename T>
    is(TwoArgPred, T&&) -> is<TwoArgPred, T>;

    // ### is_all_of

    template <typename... OneArgPreds>
    class is_all_of final
    {
      public:
        constexpr explicit is_all_of(OneArgPreds... args) noexcept
            : preds_{std::move(args)...}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            constexpr auto index_seq = std::make_index_sequence<sizeof...(OneArgPreds)>{};
            return all_(v, index_seq) && (all_(vals, index_seq) && ...);
        }

      private:
        std::tuple<OneArgPreds...> preds_;

        template <typename V, usize... Indexes>
        constexpr bool all_(const V& v, std::index_sequence<Indexes...>) const
        {
            return (std::get<Indexes>(preds_)(v) && ...);
        }
    };

    // ### is_any_of

    template <typename... OneArgPreds>
    class is_any_of final
    {
      public:
        constexpr explicit is_any_of(OneArgPreds... args) noexcept
            : preds_{std::move(args)...}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            constexpr auto index_seq = std::make_index_sequence<sizeof...(OneArgPreds)>{};
            return any_(v, index_seq) && (any_(vals, index_seq) && ...);
        }

      private:
        std::tuple<OneArgPreds...> preds_;

        template <typename V, usize... Indexes>
        constexpr bool any_(const V& v, std::index_sequence<Indexes...>) const
        {
            return (std::get<Indexes>(preds_)(v) || ...);
        }
    };

    // ### is_none_of

    template <typename... OneArgPreds>
    class is_none_of final
    {
      public:
        constexpr explicit is_none_of(OneArgPreds... args) noexcept
            : preds_{std::move(args)...}
        {
        }

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            constexpr auto index_seq = std::make_index_sequence<sizeof...(OneArgPreds)>{};
            return none_(v, index_seq) && (none_(vals, index_seq) && ...);
        }

      private:
        std::tuple<OneArgPreds...> preds_;

        template <typename V, usize... Indexes>
        constexpr bool none_(const V& v, std::index_sequence<Indexes...>) const
        {
            return (!std::get<Indexes>(preds_)(v) && ...);
        }
    };

    // ### is_empty

    struct is_empty final
    {
        explicit is_empty() = default;

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return v.is_empty() && (vals.is_empty() && ...);
        }
    };

    // ### is_zero

    struct is_zero final
    {
        explicit is_zero() = default;

        template <typename V, typename... Vals>
        [[nodiscard]] constexpr bool operator()(const V& v, const Vals&... vals) const
        {
            return v == 0 && ((vals == 0) && ...);
        }
    };

    // ## Miscellaneous

    // ### as_const

    template <typename F>
    class as_const final
    {
      public:
        constexpr explicit as_const(F f) noexcept
            : f_{std::move(f)}
        {
        }

        template <typename... Args>
        [[nodiscard]] constexpr decltype(auto) operator()(const Args&... args) const
            noexcept(noexcept(f_(args...)))
        {
            return f_(args...);
        }

      private:
        F f_;
    };

    // ### bind

    template <typename F, typename... FrontArgs>
    class bind final
    {
      public:
        constexpr explicit bind(not_deduced_t<F> f, not_deduced_t<FrontArgs>... front_args)
            : f_{std::move(f)},
              front_args_{std::forward<FrontArgs>(front_args)...}
        {
        }

        template <typename... Args>
        [[nodiscard]] constexpr decltype(auto) operator()(Args&&... args) const
        {
            return apply_(std::make_index_sequence<sizeof...(FrontArgs)>{},
                          std::forward<Args>(args)...);
        }

      private:
        F f_;
        std::tuple<FrontArgs...> front_args_;

        template <usize... Indexes, typename... Args>
        constexpr decltype(auto) apply_(std::index_sequence<Indexes...>, Args&&... args) const
        {
            return f_(get<Indexes>(front_args_)..., std::forward<Args>(args)...);
        }
    };

    template <typename F, typename... FrontArgs>
    bind(F, FrontArgs&&...) -> bind<F, FrontArgs...>;

    // ### blackhole

    struct blackhole final
    {
        explicit blackhole() = default;

        template <typename... Args>
        constexpr void operator()(Args&&...) const noexcept
        {
        }
    };

    // ### element

    template <usize Index, typename F>
    class element final
    {
      public:
        constexpr explicit element(meta::index_t<Index>, F f) noexcept
            : f_{std::move(f)}
        {
        }

        template <typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) const
        {
            return f_(get<Index>(std::forward<Args>(args))...);
        }

      private:
        F f_;
    };

    // ### ret(urn)

    template <typename T>
    class ret final
    {
      public:
        constexpr explicit ret(not_deduced_t<T> value)
            : value_{std::forward<T>(value)}
        {
        }

        template <typename... Args>
        [[nodiscard]] constexpr const T& operator()(Args&&...) const noexcept
        {
            return value_;
        }

      private:
        T value_;
    };

    template <typename T>
    ret(T&&) -> ret<T>;

    // ### stream_to

    // Stream to an object with `<<` (stream insertion) operator.

    template <typename T>
    class stream_to final
    {
      public:
        constexpr explicit stream_to(T& dest) noexcept
            : dest_{dest}
        {
        }

        stream_to(const T&) = delete;

        template <typename V, typename... Vals>
        constexpr decltype(auto) operator()(const V& v, const Vals&... vals) const
        {
            return ((dest_ << v) << ... << vals);
        }

      private:
        T& dest_;
    };

    // ### to

    template <typename To, typename F>
    class to final
    {
      public:
        constexpr explicit to(meta::type_t<To>, F f) noexcept
            : f_{std::move(f)}
        {
        }

        template <typename V, typename... Vals>
        constexpr decltype(auto) operator()(const V& v, const Vals&... vals) const
        {
            return f_(conv_(v), conv_(vals)...);
        }

      private:
        F f_;

        template <typename V>
        constexpr auto conv_(const V& v) const
        {
            if constexpr (has_to<V, To>)
                return v.template to<To>();
            else
                return To{v};
        }
    };

    // ### transparent

    struct transparent final
    {
        explicit transparent() = default;

        using is_transparent = void;

        template <typename T>
        [[nodiscard]] constexpr T&& operator()(T&& v) const noexcept
        {
            return std::forward<T>(v);
        }
    };

    // ### invoke

    template <typename F, typename Projection = fn::transparent, typename ProjectionN = Projection>
    class invoke final
    {
      public:
        constexpr explicit invoke(F f) noexcept
            : f_{std::move(f)}
        {
        }

        constexpr explicit invoke(F f, Projection p)
            : f_{std::move(f)},
              p_{p},
              pn_{p}
        {
        }

        constexpr explicit invoke(F f, Projection p, ProjectionN pn) noexcept
            : f_{std::move(f)},
              p_{std::move(p)},
              pn_{std::move(pn)}
        {
        }

        template <typename Arg, typename... ArgN>
        constexpr decltype(auto) operator()(Arg&& arg, ArgN&&... argn) const
        {
            return std::invoke(f_, std::invoke(p_, std::forward<Arg>(arg)),
                               std::invoke(pn_, std::forward<ArgN>(argn))...);
        }

      private:
        F f_;
        Projection p_{};
        ProjectionN pn_{};
    };

    // ### unpack

    template <typename F>
    class unpack final
    {
      public:
        constexpr explicit unpack(F f) noexcept
            : f_{std::move(f)}
        {
        }

        template <typename Tuple>
        constexpr decltype(auto) operator()(Tuple&& t) const
        {
            // std::apply uses std::get<> (and not get<>) so it can't be used here.
            return apply_(
                std::forward<Tuple>(t),
                std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
        }

      private:
        F f_;

        template <typename Tuple, usize... Indexes>
        constexpr decltype(auto) apply_(Tuple&& t, std::index_sequence<Indexes...>) const
        {
            return f_(get<Indexes>(std::forward<Tuple>(t))...);
        }
    };
}
