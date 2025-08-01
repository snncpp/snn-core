// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Contiguous range

// * `contiguous<[const ]char*>` are specializations with extended functionality.
// * `contiguous<[const ]char*>` specializations never hold null pointers.
// * `strrng` is an alias for `range::contiguous<char*>`.
// * `cstrrng` is an alias for `range::contiguous<const char*>`.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/contiguous_interface.hh"
#include "snn-core/ascii/lower_wrapper.hh"
#include "snn-core/ascii/to_integral_prefix.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/encoding/scheme.hh"
#include "snn-core/mem/raw/is_equal.hh"
#include "snn-core/mem/raw/load.hh"
#include "snn-core/mem/raw/load_swap.hh"
#include "snn-core/mem/raw/move.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/contiguous.fwd.hh"
#include "snn-core/utf8/core.hh"

namespace snn::range
{
    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    namespace detail::contiguous
    {
        // For `contiguous<[const ]char*>` specializations that never hold null pointers.

        inline char single_char = '\xFF'; // Not null-terminated.

        // Helper functions for `contiguous<[const ]char*>::drop_front_read(...)`.

        template <character Char, typename T>
        constexpr void drop_front_read(Char*& first, Char* const last, T& obj) noexcept
        {
            snn_should(first != nullptr && last != nullptr);
            snn_should(first < last && to_usize(last - first) >= sizeof(T));

            if (std::is_constant_evaluated())
            {
                array<char, sizeof(T)> a;
                a.fill_front(cstrview{init::from, first, last});
                obj = std::bit_cast<T>(a);
            }
            else
            {
                mem::raw::move(not_null<const char*>{first}, not_null{&obj},
                               snn::byte_size{sizeof(T)});
            }

            first += sizeof(T);
        }

        template <character Char, typename T, typename... Ts>
        constexpr void drop_front_read(Char*& first, Char* const last, T& obj, Ts&... objs) noexcept
        {
            detail::contiguous::drop_front_read(first, last, obj);
            detail::contiguous::drop_front_read(first, last, objs...);
        }
    }

    // ## Classes

    // ### contiguous (nullable iterator) {#contiguous}

    template <pointer ContiguousIt>
    class contiguous final : public contiguous_interface<contiguous<ContiguousIt>>
    {
      private:
        ContiguousIt first_;
        ContiguousIt last_;

      public:
        // #### Types

        using iterator       = ContiguousIt;
        using const_iterator = deep_const_t<iterator>;

        using pointer       = iterator;
        using const_pointer = const_iterator;

        using element_type = std::remove_pointer_t<pointer>;
        using value_type   = std::remove_cv_t<element_type>;

        using reference       = element_type&;
        using const_reference = const element_type&;

        // #### Default constructor

        constexpr contiguous() noexcept
            : first_{nullptr},
              last_{nullptr}
        {
        }

        // #### Explicit constructors

        template <typename T, usize C>
            requires(std::is_convertible_v<T*, iterator>)
        constexpr explicit contiguous(array_view<T, C> v) noexcept
            : first_{v.begin()},
              last_{v.end()}
        {
        }

        constexpr explicit contiguous(init::from_t, const iterator first,
                                      const iterator last) noexcept
            : first_{first},
              last_{last}
        {
            snn_should(first_ <= last_);
        }

        // #### Converting constructors

        template <typename It>
            requires(std::is_convertible_v<It, iterator>)
        constexpr contiguous(contiguous<It> rng) noexcept
            : first_{rng.begin()},
              last_{rng.end()}
        {
        }

        // #### Status

        constexpr explicit operator bool() const noexcept
        {
            return first_ != last_;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return first_ == last_;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return last_;
        }

        // #### Forward

        constexpr void drop_front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            ++first_;
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front()
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        // #### Bidirectional

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        constexpr void drop_back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            --last_;
        }

        // #### Random access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos)
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < count())
            {
                return *(first_ + pos);
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < count())
            {
                return *(first_ + pos);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference at(const usize pos, assume::within_bounds_t) noexcept
        {
            snn_assert(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   assume::within_bounds_t) const noexcept
        {
            snn_assert(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            snn_should(first_ <= last_);
            return static_cast<usize>(last_ - first_);
        }

        // #### Contiguous

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> pop_back()
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (first_ != last_)
            {
                return *(--last_);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference pop_back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(--last_);
        }

        constexpr contiguous pop_back_n(const usize count) noexcept
        {
            const auto last = last_;
            last_ -= math::min(count, this->count());
            return contiguous{init::from, last_, last};
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> pop_front()
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (first_ != last_)
            {
                return *(first_++);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference pop_front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(first_++);
        }

        constexpr contiguous pop_front_n(const usize count) noexcept
        {
            const auto first = first_;
            first_ += math::min(count, this->count());
            return contiguous{init::from, first, first_};
        }

        // #### Extended

        [[nodiscard]] constexpr const_pointer data() const noexcept
        {
            return first_;
        }

        template <typename OneArgPred>
        constexpr contiguous pop_back_while(OneArgPred p)
            noexcept(noexcept(p(std::as_const(*first_))))
        {
            const auto last = last_;
            while (first_ != last_ && p(std::as_const(*(last_ - 1))))
            {
                --last_;
            }
            return contiguous{init::from, last_, last};
        }

        template <typename OneArgPred>
        constexpr contiguous pop_front_while(OneArgPred p)
            noexcept(noexcept(p(std::as_const(*first_))))
        {
            const auto first = first_;
            while (first_ != last_ && p(std::as_const(*first_)))
            {
                ++first_;
            }
            return contiguous{init::from, first, first_};
        }

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{sizeof(value_type) * count()};
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            static_assert(sizeof(value_type) == 1, "Use count() or byte_size() instead.");
            return count();
        }

        constexpr void truncate(const usize count) noexcept
        {
            if (this->count() > count)
            {
                last_ = first_ + count;
            }
        }

        [[nodiscard]] constexpr auto range() noexcept
        {
            return *this;
        }

        [[nodiscard]] constexpr auto range() const noexcept
        {
            return contiguous<const_pointer>{init::from, first_, last_};
        }

        [[nodiscard]] constexpr auto view() noexcept
        {
            return array_view<element_type>{first_, count()};
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<const element_type>{first_, count()};
        }

        template <contiguous_range Rng>
            requires(std::is_same_v<typename Rng::iterator, iterator>)
        [[nodiscard]] constexpr contiguous without_suffix(Rng suffix,
                                                          assume::is_valid_t) const noexcept
        {
            snn_should(suffix.end() == last_ && suffix.begin() >= first_);
            return contiguous{init::from, first_, suffix.begin()};
        }

        [[nodiscard]] constexpr pointer writable() noexcept
            requires(!std::is_const_v<element_type>)
        {
            return first_;
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.
    };

    // ### contiguous<const char*> specialization {#contiguous-const-char-ptr}

    template <>
    class contiguous<const char*> final : public contiguous_interface<contiguous<const char*>>
    {
      private:
        const char* first_;
        const char* last_;

        friend class contiguous<char*>;

        constexpr explicit contiguous(init::internal_t, const char* const first,
                                      const char* const last) noexcept
            : first_{first},
              last_{last}
        {
        }

      public:
        // #### Types

        using value_type   = char;
        using element_type = const char;

        using iterator       = const char*;
        using const_iterator = const char*;

        using pointer       = const char*;
        using const_pointer = const char*;

        using reference       = const char&;
        using const_reference = const char&;

        // #### Default constructor

        constexpr contiguous() noexcept
            : first_{&detail::contiguous::single_char},
              last_{first_}
        {
        }

        // #### Explicit constructors

        template <same_as<const char> ConstChar, usize N>
        constexpr explicit contiguous(ConstChar (&s)[N]) noexcept
            : first_{s},
              last_{first_ + (N - 1)}
        {
        }

        template <character Char, usize C>
        constexpr explicit contiguous(array_view<Char, C> v) noexcept
            : first_{v.begin()},
              last_{v.end()}
        {
        }

        constexpr explicit contiguous(init::from_t, const iterator first,
                                      const iterator last) noexcept
            : contiguous{}
        {
            if (first != last)
            {
                snn_should(first != nullptr && last != nullptr && first < last);
                first_ = first;
                last_  = last;
            }
        }

        // #### Converting constructors

        template <character Char>
        constexpr contiguous(contiguous<Char*> rng) noexcept
            : first_{rng.begin()},
              last_{rng.end()}
        {
        }

        // #### Status

        constexpr explicit operator bool() const noexcept
        {
            return first_ != last_;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return first_ == last_;
        }

        // #### Iterators

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return last_;
        }

        // #### Forward

        constexpr void drop_front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            ++first_;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const noexcept
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        // #### Bidirectional

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const noexcept
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        constexpr void drop_back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            --last_;
        }

        // #### Random access

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const noexcept
        {
            if (pos < count())
            {
                return *(first_ + pos);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   assume::within_bounds_t) const noexcept
        {
            snn_assert(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            snn_should(first_ <= last_);
            return static_cast<usize>(last_ - first_);
        }

        // #### Contiguous

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> pop_back() noexcept
        {
            if (first_ != last_)
            {
                return *(--last_);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference pop_back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(--last_);
        }

        constexpr contiguous pop_back_n(const usize count) noexcept
        {
            const auto last = last_;
            last_ -= math::min(count, this->count());
            return contiguous{init::internal, last_, last};
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> pop_front() noexcept
        {
            if (first_ != last_)
            {
                return *(first_++);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference pop_front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(first_++);
        }

        constexpr contiguous pop_front_n(const usize count) noexcept
        {
            const auto first = first_;
            first_ += math::min(count, this->count());
            return contiguous{init::internal, first, first_};
        }

        // #### Extended

        [[nodiscard]] constexpr auto data() const noexcept
        {
            return not_null<const_pointer>{first_};
        }

        constexpr bool drop_back(const same_as<char> auto c) noexcept
        {
            if (has_back(c))
            {
                --last_;
                return true;
            }
            return false;
        }

        constexpr bool drop_back(const transient<cstrview> s) noexcept
        {
            if (has_back(s))
            {
                last_ -= s.get().size();
                return true;
            }
            return false;
        }

        constexpr bool drop_front(const same_as<char> auto c) noexcept
        {
            if (first_ != last_ && *first_ == c)
            {
                ++first_;
                return true;
            }
            return false;
        }

        template <u16 Offset>
        constexpr bool drop_front(const same_as<char> auto c) noexcept
        {
            if (has_front<Offset>(c))
            {
                first_ += (Offset + 1u);
                return true;
            }
            return false;
        }

        template <u16 Offset = 0>
        constexpr bool drop_front(const transient<cstrview> s) noexcept
        {
            if (has_front<Offset>(s))
            {
                first_ += (Offset + s.get().size());
                return true;
            }
            return false;
        }

        // Drop exactly N elements from the front.

        constexpr bool drop_front_exactly_n(const usize count) noexcept
        {
            if (this->count() >= count)
            {
                first_ += count;
                return true;
            }
            return false;
        }

        template <predicate<char> OneArgPred>
        constexpr bool drop_front_if(OneArgPred p)
        {
            if (first_ != last_ && p(std::as_const(*first_)))
            {
                ++first_;
                return true;
            }
            return false;
        }

        constexpr bool drop_front_icase(const ascii::lower_wrapper<cstrview> ls) noexcept
        {
            const auto s = ls.get();
            if (count() >= s.size())
            {
                for (usize i = 0; i < s.size(); ++i)
                {
                    const char c = s.data().get()[i];
                    if (chr::to_alpha_lower(*(first_ + i)) != c)
                    {
                        return false;
                    }
                }
                first_ += s.size();
                return true;
            }
            return false;
        }

        template <strict_integral Int>
        [[nodiscard]] constexpr bool drop_front_load(Int& i) noexcept
        {
            if (count() >= sizeof(i))
            {
                i = static_cast<Int>(mem::raw::load<std::make_unsigned_t<Int>>(first_));
                first_ += sizeof(i);
                return true;
            }
            return false;
        }

        template <strict_integral Int>
        [[nodiscard]] constexpr bool drop_front_load_swap(Int& i) noexcept
        {
            if (count() >= sizeof(i))
            {
                i = static_cast<Int>(mem::raw::load_swap<std::make_unsigned_t<Int>>(first_));
                first_ += sizeof(i);
                return true;
            }
            return false;
        }

        template <typename T, typename... Ts>
            requires(meta::none_v<std::is_const, T, Ts...> &&
                     meta::all_v<std::is_trivially_copyable, T, Ts...>)
        [[nodiscard]] constexpr bool drop_front_read(T& obj, Ts&... objs) noexcept
        {
            constexpr usize TotalSize = (sizeof(T) + ... + sizeof(Ts));
            if (count() >= TotalSize)
            {
                detail::contiguous::drop_front_read(first_, last_, obj, objs...);
                return true;
            }
            return false;
        }

        [[nodiscard]] constexpr bool has_front(const same_as<char> auto c) const noexcept
        {
            return first_ != last_ && *first_ == c;
        }

        template <u16 Offset>
        [[nodiscard]] constexpr bool has_front(const same_as<char> auto c) const noexcept
        {
            return count() >= (Offset + 1u) && *(first_ + Offset) == c;
        }

        template <u16 Offset = 0>
        [[nodiscard]] constexpr bool has_front(const transient<cstrview> prefix) const noexcept
        {
            const cstrview s = prefix.get();
            return count() >= (Offset + s.size()) &&
                   mem::raw::is_equal(not_null{first_ + Offset}, s.data(), s.byte_size());
        }

        template <predicate<char> OneArgPred>
        [[nodiscard]] constexpr bool has_front_if(OneArgPred p) const
        {
            return first_ != last_ && p(std::as_const(*first_));
        }

        template <u16 Offset, predicate<char> OneArgPred>
        [[nodiscard]] constexpr bool has_front_if(OneArgPred p) const
        {
            return count() >= (Offset + 1u) && p(std::as_const(*(first_ + Offset)));
        }

        [[nodiscard]] constexpr bool has_back(const same_as<char> auto c) const noexcept
        {
            return first_ != last_ && *(last_ - 1) == c;
        }

        [[nodiscard]] constexpr bool has_back(const transient<cstrview> suffix) const noexcept
        {
            const cstrview s = suffix.get();
            if (s.size() <= count())
            {
                const usize pos = count() - s.size();
                return mem::raw::is_equal(not_null{first_ + pos}, s.data(), s.byte_size());
            }
            return false;
        }

        template <predicate<char> OneArgPred>
        [[nodiscard]] constexpr bool has_back_if(OneArgPred p) const
        {
            return first_ != last_ && p(std::as_const(*(last_ - 1)));
        }

        template <typename OneArgPred>
        constexpr contiguous pop_back_while(OneArgPred p)
            noexcept(noexcept(p(std::as_const(*first_))))
        {
            const auto last = last_;
            while (first_ != last_ && p(std::as_const(*(last_ - 1))))
            {
                --last_;
            }
            return contiguous{init::internal, last_, last};
        }

        template <strict_integral Int, math::base Base = math::base::decimal, usize MaxDigits = 0>
        constexpr pair::value_count<Int, usize> pop_front_integral(
            ascii::leading_zeros policy = ascii::leading_zeros::disallow) noexcept
        {
            const auto p = ascii::to_integral_prefix<Int, Base, MaxDigits>(*this, policy);
            first_ += p.count;
            return p;
        }

        template <encoding::scheme Encoding = encoding::scheme::utf8>
        constexpr pair::value_count<u32> pop_front_codepoint() noexcept
        {
            static_assert(Encoding == encoding::scheme::utf8, "Only UTF-8 is supported.");
            if (first_ != last_)
            {
                const char c = *first_;
                if (utf8::is_1_byte_sequence(c))
                {
                    ++first_;
                    return {to_byte(c), 1};
                }
                else
                {
                    const auto p = utf8::decode_multibyte_sequence(*this);
                    first_ += p.count;
                    return p;
                }
            }
            return {utf8::invalid, 0};
        }

        template <typename OneArgPred>
        constexpr contiguous pop_front_while(OneArgPred p)
            noexcept(noexcept(p(std::as_const(*first_))))
        {
            const auto first = first_;
            while (first_ != last_ && p(std::as_const(*first_)))
            {
                ++first_;
            }
            return contiguous{init::internal, first, first_};
        }

        template <typename OneArgPred, typename OneArgFn>
        constexpr contiguous pop_front_while(OneArgPred p, OneArgFn f)
        {
            const auto first = first_;
            while (first_ != last_ && p(std::as_const(*first_)))
            {
                f(*first_);
                ++first_;
            }
            return contiguous{init::internal, first, first_};
        }

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{count()};
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return count();
        }

        constexpr void truncate(const usize count) noexcept
        {
            if (this->count() > count)
            {
                last_ = first_ + count;
            }
        }

        [[nodiscard]] constexpr auto range() const noexcept
        {
            return *this;
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<element_type>{not_null<const_pointer>{first_}, count()};
        }

        template <contiguous_range Rng>
            requires(std::is_same_v<typename Rng::iterator, iterator>)
        [[nodiscard]] constexpr contiguous without_suffix(Rng suffix,
                                                          assume::is_valid_t) const noexcept
        {
            snn_should(suffix.end() == last_ && suffix.begin() >= first_);
            return contiguous{init::internal, first_, suffix.begin()};
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.
    };

    // ### contiguous<char*> specialization {#contiguous-char-ptr}

    template <>
    class contiguous<char*> final : public contiguous_interface<contiguous<char*>>
    {
      private:
        char* first_;
        char* last_;

        constexpr explicit contiguous(init::internal_t, char* const first,
                                      char* const last) noexcept
            : first_{first},
              last_{last}
        {
        }

      public:
        // #### Types

        using value_type   = char;
        using element_type = char;

        using iterator       = char*;
        using const_iterator = const char*;

        using pointer       = char*;
        using const_pointer = const char*;

        using reference       = char&;
        using const_reference = const char&;

        // #### Default constructor

        constexpr contiguous() noexcept
            : first_{&detail::contiguous::single_char},
              last_{first_}
        {
        }

        // #### Explicit constructors

        template <usize C>
        constexpr explicit contiguous(array_view<char, C> v) noexcept
            : first_{v.begin()},
              last_{v.end()}
        {
        }

        constexpr explicit contiguous(init::from_t, const iterator first,
                                      const iterator last) noexcept
            : contiguous{}
        {
            if (first != last)
            {
                snn_should(first != nullptr && last != nullptr && first < last);
                first_ = first;
                last_  = last;
            }
        }

        // #### Status

        constexpr explicit operator bool() const noexcept
        {
            return first_ != last_;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return first_ == last_;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return last_;
        }

        // #### Forward

        constexpr void drop_front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            ++first_;
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front() noexcept
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const noexcept
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        [[nodiscard]] constexpr const_reference front(assume::not_empty_t) const noexcept
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        // #### Bidirectional

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const noexcept
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        [[nodiscard]] constexpr const_reference back(assume::not_empty_t) const noexcept
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        constexpr void drop_back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            --last_;
        }

        // #### Random access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) noexcept
        {
            if (pos < count())
            {
                return *(first_ + pos);
            }
            return nullopt;
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const noexcept
        {
            if (pos < count())
            {
                return *(first_ + pos);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference at(const usize pos, assume::within_bounds_t) noexcept
        {
            snn_assert(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   assume::within_bounds_t) const noexcept
        {
            snn_assert(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            snn_should(first_ <= last_);
            return static_cast<usize>(last_ - first_);
        }

        // #### Contiguous

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> pop_back() noexcept
        {
            if (first_ != last_)
            {
                return *(--last_);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference pop_back(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(--last_);
        }

        constexpr contiguous pop_back_n(const usize count) noexcept
        {
            const auto last = last_;
            last_ -= math::min(count, this->count());
            return contiguous{init::internal, last_, last};
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> pop_front() noexcept
        {
            if (first_ != last_)
            {
                return *(first_++);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr reference pop_front(assume::not_empty_t) noexcept
        {
            snn_assert(first_ != last_);
            return *(first_++);
        }

        constexpr contiguous pop_front_n(const usize count) noexcept
        {
            const auto first = first_;
            first_ += math::min(count, this->count());
            return contiguous{init::internal, first, first_};
        }

        // #### Extended

        [[nodiscard]] constexpr auto data() const noexcept
        {
            return not_null<const_pointer>{first_};
        }

        constexpr bool drop_back(const same_as<char> auto c) noexcept
        {
            if (has_back(c))
            {
                --last_;
                return true;
            }
            return false;
        }

        constexpr bool drop_back(const transient<cstrview> s) noexcept
        {
            if (has_back(s))
            {
                last_ -= s.get().size();
                return true;
            }
            return false;
        }

        constexpr bool drop_front(const same_as<char> auto c) noexcept
        {
            if (first_ != last_ && *first_ == c)
            {
                ++first_;
                return true;
            }
            return false;
        }

        template <u16 Offset>
        constexpr bool drop_front(const same_as<char> auto c) noexcept
        {
            if (has_front<Offset>(c))
            {
                first_ += (Offset + 1u);
                return true;
            }
            return false;
        }

        template <u16 Offset = 0>
        constexpr bool drop_front(const transient<cstrview> s) noexcept
        {
            if (has_front<Offset>(s))
            {
                first_ += (Offset + s.get().size());
                return true;
            }
            return false;
        }

        // Drop exactly N elements from the front.

        constexpr bool drop_front_exactly_n(const usize count) noexcept
        {
            if (this->count() >= count)
            {
                first_ += count;
                return true;
            }
            return false;
        }

        template <predicate<char> OneArgPred>
        constexpr bool drop_front_if(OneArgPred p)
        {
            if (first_ != last_ && p(std::as_const(*first_)))
            {
                ++first_;
                return true;
            }
            return false;
        }

        constexpr bool drop_front_icase(const ascii::lower_wrapper<cstrview> ls) noexcept
        {
            const auto s = ls.get();
            if (count() >= s.size())
            {
                for (usize i = 0; i < s.size(); ++i)
                {
                    const char c = s.data().get()[i];
                    if (chr::to_alpha_lower(*(first_ + i)) != c)
                    {
                        return false;
                    }
                }
                first_ += s.size();
                return true;
            }
            return false;
        }

        template <strict_integral Int>
        [[nodiscard]] constexpr bool drop_front_load(Int& i) noexcept
        {
            if (count() >= sizeof(i))
            {
                i = static_cast<Int>(mem::raw::load<std::make_unsigned_t<Int>>(first_));
                first_ += sizeof(i);
                return true;
            }
            return false;
        }

        template <strict_integral Int>
        [[nodiscard]] constexpr bool drop_front_load_swap(Int& i) noexcept
        {
            if (count() >= sizeof(i))
            {
                i = static_cast<Int>(mem::raw::load_swap<std::make_unsigned_t<Int>>(first_));
                first_ += sizeof(i);
                return true;
            }
            return false;
        }

        template <typename T, typename... Ts>
            requires(meta::none_v<std::is_const, T, Ts...> &&
                     meta::all_v<std::is_trivially_copyable, T, Ts...>)
        [[nodiscard]] constexpr bool drop_front_read(T& obj, Ts&... objs) noexcept
        {
            constexpr usize TotalSize = (sizeof(T) + ... + sizeof(Ts));
            if (count() >= TotalSize)
            {
                detail::contiguous::drop_front_read(first_, last_, obj, objs...);
                return true;
            }
            return false;
        }

        [[nodiscard]] constexpr bool has_front(const same_as<char> auto c) const noexcept
        {
            return first_ != last_ && *first_ == c;
        }

        template <u16 Offset>
        [[nodiscard]] constexpr bool has_front(const same_as<char> auto c) const noexcept
        {
            return count() >= (Offset + 1u) && *(first_ + Offset) == c;
        }

        template <u16 Offset = 0>
        [[nodiscard]] constexpr bool has_front(const transient<cstrview> prefix) const noexcept
        {
            const cstrview s = prefix.get();
            return count() >= (Offset + s.size()) &&
                   mem::raw::is_equal(not_null{first_ + Offset}, s.data(), s.byte_size());
        }

        template <predicate<char> OneArgPred>
        [[nodiscard]] constexpr bool has_front_if(OneArgPred p) const
        {
            return first_ != last_ && p(std::as_const(*first_));
        }

        template <u16 Offset, predicate<char> OneArgPred>
        [[nodiscard]] constexpr bool has_front_if(OneArgPred p) const
        {
            return count() >= (Offset + 1u) && p(std::as_const(*(first_ + Offset)));
        }

        [[nodiscard]] constexpr bool has_back(const same_as<char> auto c) const noexcept
        {
            return first_ != last_ && *(last_ - 1) == c;
        }

        [[nodiscard]] constexpr bool has_back(const transient<cstrview> suffix) const noexcept
        {
            const cstrview s = suffix.get();
            if (s.size() <= count())
            {
                const usize pos = count() - s.size();
                return mem::raw::is_equal(not_null{first_ + pos}, s.data(), s.byte_size());
            }
            return false;
        }

        template <predicate<char> OneArgPred>
        [[nodiscard]] constexpr bool has_back_if(OneArgPred p) const
        {
            return first_ != last_ && p(std::as_const(*(last_ - 1)));
        }

        template <typename OneArgPred>
        constexpr contiguous pop_back_while(OneArgPred p)
            noexcept(noexcept(p(std::as_const(*first_))))
        {
            const auto last = last_;
            while (first_ != last_ && p(std::as_const(*(last_ - 1))))
            {
                --last_;
            }
            return contiguous{init::internal, last_, last};
        }

        template <strict_integral Int, math::base Base = math::base::decimal, usize MaxDigits = 0>
        constexpr pair::value_count<Int, usize> pop_front_integral(
            ascii::leading_zeros policy = ascii::leading_zeros::disallow) noexcept
        {
            const auto p = ascii::to_integral_prefix<Int, Base, MaxDigits>(*this, policy);
            first_ += p.count;
            return p;
        }

        template <encoding::scheme Encoding = encoding::scheme::utf8>
        constexpr pair::value_count<u32> pop_front_codepoint() noexcept
        {
            static_assert(Encoding == encoding::scheme::utf8, "Only UTF-8 is supported.");
            if (first_ != last_)
            {
                const char c = *first_;
                if (utf8::is_1_byte_sequence(c))
                {
                    ++first_;
                    return {to_byte(c), 1};
                }
                else
                {
                    const auto p = utf8::decode_multibyte_sequence(*this);
                    first_ += p.count;
                    return p;
                }
            }
            return {utf8::invalid, 0};
        }

        template <typename OneArgPred>
        constexpr contiguous pop_front_while(OneArgPred p)
            noexcept(noexcept(p(std::as_const(*first_))))
        {
            const auto first = first_;
            while (first_ != last_ && p(std::as_const(*first_)))
            {
                ++first_;
            }
            return contiguous{init::internal, first, first_};
        }

        template <typename OneArgPred, typename OneArgFn>
        constexpr contiguous pop_front_while(OneArgPred p, OneArgFn f)
        {
            const auto first = first_;
            while (first_ != last_ && p(std::as_const(*first_)))
            {
                f(*first_);
                ++first_;
            }
            return contiguous{init::internal, first, first_};
        }

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{count()};
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return count();
        }

        constexpr void truncate(const usize count) noexcept
        {
            if (this->count() > count)
            {
                last_ = first_ + count;
            }
        }

        [[nodiscard]] constexpr auto range() noexcept
        {
            return *this;
        }

        [[nodiscard]] constexpr auto range() const noexcept
        {
            return contiguous<const char*>{init::internal, first_, last_};
        }

        [[nodiscard]] constexpr auto view() noexcept
        {
            return array_view<element_type>{not_null<pointer>{first_}, count()};
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<const element_type>{not_null<const_pointer>{first_}, count()};
        }

        template <contiguous_range Rng>
            requires(std::is_same_v<typename Rng::iterator, iterator>)
        [[nodiscard]] constexpr contiguous without_suffix(Rng suffix,
                                                          assume::is_valid_t) const noexcept
        {
            snn_should(suffix.end() == last_ && suffix.begin() >= first_);
            return contiguous{init::internal, first_, suffix.begin()};
        }

        [[nodiscard]] constexpr auto writable() noexcept
        {
            return not_null<pointer>{first_};
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.
    };

    SNN_DIAGNOSTIC_POP

    // ## Deduction guides

    // ### contiguous

    template <usize N>
    contiguous(const char (&)[N]) -> contiguous<const char*>;
}
