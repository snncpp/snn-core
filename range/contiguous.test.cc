// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/contiguous.hh"

#include "snn-core/array.hh"
#include "snn-core/make_range.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/math/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            cstrrng rng{"abc123def"};
            static_assert(std::is_same_v<decltype(rng), range::contiguous<const char*>>);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.count() == 9);

            snn_require(rng.all(chr::is_alphanumeric));
            snn_require(!rng.all(chr::is_alpha));

            snn_require(rng.any(chr::is_digit));
            snn_require(!rng.any(chr::is_alpha_upper));

            snn_require(rng.has_front('a'));
            snn_require(rng.has_back('f'));

            snn_require(rng.drop_front("abc"));
            snn_require(!rng.drop_front("xyz"));

            snn_require(rng.view() == "123def");

            snn_require(rng.pop_front_while(chr::is_digit).view() == "123");

            snn_require(rng.view() == "def");

            snn_require(rng.pop_front_while(fn::in_array{'a', 'b', 'c', 'd', 'e'}).view() == "de");

            snn_require(rng.view() == "f");

            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty) == 'f');
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            return true;
        }

        constexpr bool has_codepoints(cstrrng rng, const init_list<u32> codepoints)
        {
            usize index       = 0;
            const auto cp_rng = make_range(codepoints);
            while (rng)
            {
                const u32 cp = rng.pop_front_codepoint().value;
                if (cp_rng.at(index) != cp)
                {
                    return false;
                }
                ++index;
            }
            return index == cp_rng.count();
        }

        template <typename Rng = strrng>
        constexpr bool test_drop_front_exactly_n()
        {
            array src{'a', 'b', 'c', 'd', 'e', 'f'};
            Rng rng = src.range();
            snn_require(rng.count() == 6);
            snn_require(rng.view() == "abcdef");

            snn_require(!rng.drop_front_exactly_n(99));
            snn_require(!rng.drop_front_exactly_n(7));

            snn_require(rng.count() == 6);
            snn_require(rng.view() == "abcdef");

            snn_require(rng.drop_front_exactly_n(0));
            snn_require(rng.count() == 6);
            snn_require(rng.view() == "abcdef");

            snn_require(rng.drop_front_exactly_n(2));
            snn_require(rng.count() == 4);
            snn_require(rng.view() == "cdef");

            snn_require(rng.drop_front_exactly_n(4));
            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            snn_require(rng.drop_front_exactly_n(0));
            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            snn_require(!rng.drop_front_exactly_n(1));
            snn_require(!rng.drop_front_exactly_n(99));

            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            return true;
        }

        template <typename Rng>
        constexpr bool drop_front_load()
        {
            array src{'a', 'b', 'c', 'd', 'e', 'f'};
            Rng rng = src.range();
            snn_require(rng.count() == 6);
            snn_require(rng.view() == "abcdef");

            u32 a{};

            snn_require(rng.drop_front_load(a));
            snn_require(a == 0x64636261);
            snn_require(rng.count() == 2);
            snn_require(rng.view() == "ef");

            snn_require(!rng.drop_front_load(a));
            snn_require(a == 0x64636261); // Previous value.
            snn_require(rng.count() == 2);
            snn_require(rng.view() == "ef");

            i16 b{};

            snn_require(rng.drop_front_load(b));
            snn_require(b == 0x6665);
            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            snn_require(!rng.drop_front_load(b));
            snn_require(b == 0x6665); // Previous value.
            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            return true;
        }

        template <typename Rng>
        constexpr bool drop_front_load_swap()
        {
            array src{'a', 'b', 'c', 'd', 'e', 'f'};
            Rng rng = src.range();
            snn_require(rng.count() == 6);
            snn_require(rng.view() == "abcdef");

            u32 a{};

            snn_require(rng.drop_front_load_swap(a));
            snn_require(a == 0x61626364);
            snn_require(rng.count() == 2);
            snn_require(rng.view() == "ef");

            snn_require(!rng.drop_front_load_swap(a));
            snn_require(a == 0x61626364); // Previous value.
            snn_require(rng.count() == 2);
            snn_require(rng.view() == "ef");

            i16 b{};

            snn_require(rng.drop_front_load_swap(b));
            snn_require(b == 0x6566);
            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            snn_require(!rng.drop_front_load_swap(b));
            snn_require(b == 0x6566); // Previous value.
            snn_require(rng.count() == 0);
            snn_require(rng.view() == "");

            return true;
        }

        template <typename strrng>
        constexpr bool test_drop_front_read()
        {
            {
                array src{'a', 'b', 'c', 'd', 'e', 'f'};
                strrng rng = src.range();
                snn_require(rng.count() == 6);

                u64 a{};
                u32 b{};

                snn_require(!rng.drop_front_read(a));
                snn_require(rng.count() == 6);

                snn_require(rng.drop_front_read(b));
                snn_require(rng.count() == 2);
                snn_require(rng.view() == "ef");

                snn_require(b == 0x64636261);

                snn_require(src.view() == "abcdef");
            }
            {
                array src{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
                strrng rng = src.range();
                snn_require(rng.count() == 8);

                array<char, 4> a;
                array<char, 2> b;
                array<char, 1> c;

                snn_require(a.all(fn::is_zero{}));
                snn_require(b.all(fn::is_zero{}));
                snn_require(c.all(fn::is_zero{}));

                snn_require(rng.drop_front_read(a, b, c));
                snn_require(rng.count() == 1);
                snn_require(rng.front().value() == 'h');

                snn_require(a.view() == "abcd");
                snn_require(b.view() == "ef");
                snn_require(c.view() == "g");

                snn_require(!rng.drop_front_read(a, b, c));
                snn_require(rng.count() == 1);
                snn_require(rng.front().value() == 'h');

                snn_require(a.view() == "abcd");
                snn_require(b.view() == "ef");
                snn_require(c.view() == "g");

                snn_require(!rng.drop_front_read(a));
                snn_require(rng.count() == 1);
                snn_require(rng.front().value() == 'h');

                snn_require(a.view() == "abcd");

                snn_require(!rng.drop_front_read(b));
                snn_require(rng.count() == 1);
                snn_require(rng.front().value() == 'h');

                snn_require(b.view() == "ef");

                snn_require(rng.drop_front_read(c));
                snn_require(rng.count() == 0);

                snn_require(c.view() == "h");

                snn_require(src.view() == "abcdefgh");
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        snn_static_require(app::drop_front_load<strrng>());
        snn_static_require(app::drop_front_load<cstrrng>());

        snn_static_require(app::drop_front_load_swap<strrng>());
        snn_static_require(app::drop_front_load_swap<cstrrng>());

        snn_static_require(app::test_drop_front_read<strrng>());
        snn_static_require(app::test_drop_front_read<cstrrng>());

        snn_static_require(app::test_drop_front_exactly_n<strrng>());
        snn_static_require(app::test_drop_front_exactly_n<cstrrng>());

        // constexpr contiguous()
        {
            constexpr range::contiguous<int*> rng;
            static_assert(rng.begin() == nullptr);
            static_assert(rng.end() == nullptr);
            static_assert(rng.count() == 0);
            static_assert(rng.is_empty());
        }

        // constexpr explicit contiguous(ContiguousIt first, ContiguousIt last)
        // constexpr ContiguousIt begin() const
        // constexpr ContiguousIt end() const
        {
            array<int, 0> a;

            range::contiguous rng{init::from, a.begin(), a.end()};
            snn_require(a.begin() == rng.begin());
            snn_require(a.end() == rng.end());

            using range_type = decltype(rng);

            // Members types.
            static_assert(std::is_same_v<range_type::value_type, int>);
            static_assert(std::is_same_v<range_type::element_type, int>);
            static_assert(std::is_same_v<range_type::iterator, int*>);
            static_assert(std::is_same_v<range_type::const_iterator, const int*>);
            static_assert(std::is_same_v<range_type::pointer, int*>);
            static_assert(std::is_same_v<range_type::const_pointer, const int*>);
            static_assert(std::is_same_v<range_type::reference, int&>);
            static_assert(std::is_same_v<range_type::const_reference, const int&>);

            // Concepts
            static_assert(forward_range<range_type>);
            static_assert(bidirectional_range<range_type>);
            static_assert(random_access_range<range_type>);
            static_assert(contiguous_range<range_type>);
        }

        // constexpr explicit operator bool() const
        // constexpr bool is_empty() const
        // constexpr void drop_front(assume::not_empty_t)
        // constexpr optional<dereference_type> front()
        // constexpr optional<const_dereference_type> front() const
        // constexpr dereference_type front(assume::not_empty_t)
        // constexpr const_dereference_type front(assume::not_empty_t) const
        {
            array a{123, 456};

            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.count() == 2);
            snn_require(rng.byte_size().get() == 8);

            snn_require(rng.front(assume::not_empty) == 123);
            snn_require(rng.front().value() == 123);
            snn_require(std::as_const(rng).front(assume::not_empty) == 123);
            snn_require(std::as_const(rng).front().value() == 123);

            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 456);

            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.front().has_value());
            snn_require(!std::as_const(rng).front().has_value());
        }

        // constexpr optional<dereference_type> back()
        // constexpr optional<const_dereference_type> back() const
        // constexpr dereference_type back(assume::not_empty_t)
        // constexpr const_dereference_type back(assume::not_empty_t) const
        // constexpr void drop_back(assume::not_empty_t)
        {
            array a{123, 456};

            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.back(assume::not_empty) == 456);
            snn_require(rng.back().value() == 456);
            snn_require(std::as_const(rng).back(assume::not_empty) == 456);
            snn_require(std::as_const(rng).back().value() == 456);

            rng.drop_back(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.back(assume::not_empty) == 123);

            rng.drop_back(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.back().has_value());
            snn_require(!std::as_const(rng).back().has_value());
        }

        // constexpr optional<dereference_type> at(const usize pos)
        // constexpr optional<const_dereference_type> at(const usize pos) const
        // constexpr dereference_type at(const usize pos, assume::within_bounds_t)
        // constexpr const_dereference_type at(const usize pos, assume::within_bounds_t) const
        // constexpr usize count() const
        {
            array a{123, 456};

            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng.count() == 2);

            snn_require(rng.at(0, assume::within_bounds) == 123);
            snn_require(rng.at(0).value() == 123);
            snn_require(std::as_const(rng).at(0, assume::within_bounds) == 123);
            snn_require(std::as_const(rng).at(0).value() == 123);

            snn_require(rng.at(1, assume::within_bounds) == 456);
            snn_require(rng.at(1).value() == 456);
            snn_require(std::as_const(rng).at(1, assume::within_bounds) == 456);
            snn_require(std::as_const(rng).at(1).value() == 456);

            rng.drop_front(assume::not_empty);

            snn_require(rng.count() == 1);

            rng.drop_front(assume::not_empty);

            snn_require(rng.count() == 0);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.at(0).has_value());
            snn_require(!std::as_const(rng).at(0).has_value());
        }

        // constexpr optional<dereference_type> pop_back() noexcept
        // constexpr dereference_type pop_back(assume::not_empty_t) noexcept
        {
            array a{123, 456};

            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng.count() == 2);

            snn_require(rng.pop_back(assume::not_empty) == 456);

            snn_require(rng.count() == 1);

            snn_require(rng.pop_back().value() == 123);

            snn_require(rng.count() == 0);
            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.pop_back().has_value());
        }

        // constexpr optional<dereference_type> pop_front() noexcept
        // constexpr dereference_type pop_front(assume::not_empty_t) noexcept
        {
            array a{123, 456};

            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng.count() == 2);

            snn_require(rng.pop_front(assume::not_empty) == 123);

            snn_require(rng.count() == 1);

            snn_require(rng.pop_front().value() == 456);

            snn_require(rng.count() == 0);
            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.pop_front().has_value());
        }

        // constexpr contiguous pop_back_n(const usize count) noexcept
        {
            array a{123, 456, 789};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_back_n(2);

            snn_require(algo::is_equal(r, {456, 789}));
            snn_require(algo::is_equal(rng, {123}));
        }
        {
            array a{123, 456, 789};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_back_n(99);

            snn_require(algo::is_equal(r, {123, 456, 789}));
            snn_require(rng.is_empty());
        }
        {
            array a{123, 456, 789};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_back_n(0);

            snn_require(r.is_empty());
            snn_require(algo::is_equal(rng, {123, 456, 789}));
        }

        // constexpr contiguous pop_front_n(const usize count) noexcept
        {
            array a{123, 456, 789};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_front_n(2);

            snn_require(algo::is_equal(r, {123, 456}));
            snn_require(algo::is_equal(rng, {789}));
        }
        {
            array a{123, 456, 789};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_front_n(99);

            snn_require(algo::is_equal(r, {123, 456, 789}));
            snn_require(rng.is_empty());
        }
        {
            array a{123, 456, 789};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_front_n(0);

            snn_require(r.is_empty());
            snn_require(algo::is_equal(rng, {123, 456, 789}));
        }

        // contiguous without_suffix(Rng suffix, assume::is_valid_t) const
        {
            array a{1, 3, 8, 10, 12};

            const range::contiguous rng{init::from, a.begin(), a.end()};
            snn_require(algo::is_equal(rng, {1, 3, 8, 10, 12}));

            auto suffix = rng;

            snn_require(algo::is_equal(rng, suffix));

            suffix.pop_front_n(3);

            snn_require(algo::is_equal(suffix, {10, 12}));

            auto r = rng.without_suffix(suffix, assume::is_valid);

            snn_require(algo::is_equal(r, {1, 3, 8}));
            snn_require(algo::is_equal(suffix, {10, 12}));
        }

        // constexpr void truncate(const usize count)
        {
            array a{1, 3, 8, 10, 12};

            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng.count() == 5);
            snn_require(algo::is_equal(rng, {1, 3, 8, 10, 12}));

            rng.truncate(6);
            snn_require(rng.count() == 5);
            snn_require(algo::is_equal(rng, {1, 3, 8, 10, 12}));

            rng.truncate(5);
            snn_require(rng.count() == 5);
            snn_require(algo::is_equal(rng, {1, 3, 8, 10, 12}));

            rng.truncate(4);
            snn_require(rng.count() == 4);
            snn_require(algo::is_equal(rng, {1, 3, 8, 10}));

            rng.truncate(2);
            snn_require(rng.count() == 2);
            snn_require(algo::is_equal(rng, {1, 3}));

            rng.truncate(1);
            snn_require(rng.count() == 1);
            snn_require(algo::is_equal(rng, {1}));

            rng.truncate(0);
            snn_require(rng.count() == 0);

            snn_require(rng.begin() == a.begin());
        }

        // constexpr bool has_front(const V& value) const
        // constexpr bool has_back(const V& value) const
        {
            array a{3, 8};
            range::contiguous rng{init::from, a.begin(), a.end()};

            snn_require(rng.has_front(3));
            snn_require(!rng.has_front(8));
            snn_require(rng.has_back(8));
            snn_require(!rng.has_back(3));

            rng.drop_front(assume::not_empty);
            snn_require(rng.count() == 1);

            snn_require(rng.has_front(8));
            snn_require(!rng.has_front(3));
            snn_require(rng.has_back(8));
            snn_require(!rng.has_back(3));

            rng.drop_front(assume::not_empty);
            snn_require(rng.is_empty());

            snn_require(!rng.has_front(8));
            snn_require(!rng.has_back(8));
        }

        // constexpr contiguous pop_back_while(OneArgPred p)
        {
            array a{1, 3, 8, 10, 12};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_back_while(math::fn::is_even{});

            snn_require(algo::is_equal(r, {8, 10, 12}));
            snn_require(algo::is_equal(rng, {1, 3}));
        }

        // constexpr contiguous pop_front_while(OneArgPred p)
        {
            array a{1, 3, 8, 10, 12};

            range::contiguous rng{init::from, a.begin(), a.end()};

            auto r = rng.pop_front_while(math::fn::is_odd{});

            snn_require(algo::is_equal(r, {1, 3}));
            snn_require(algo::is_equal(rng, {8, 10, 12}));
        }

        // constexpr bool contains(const V& value) const
        {
            array a{1, 3};
            range::contiguous rng{init::from, a.begin(), a.end()};
            snn_require(rng.contains(1));
            snn_require(rng.contains(3));
            snn_require(!rng.contains(2));
        }

        // constexpr void each(OneArgFn f)
        {
            array a{1, 3};
            range::contiguous rng{init::from, a.begin(), a.end()};
            rng.each([](int& i) { ++i; });
            snn_require(algo::is_equal(a.range(), {2, 4}));
        }

        // constexpr void each(OneArgFn f) const
        {
            array a{1, 3};
            const range::contiguous rng{init::from, a.begin(), a.end()};
            int sum = 0;
            rng.each([&](int i) { sum += i; });
            snn_require(sum == 4);
        }

        // find
        // find_in_reverse
        {
            static constexpr array a{8, 8, 10, 12};
            constexpr range::contiguous<const int*> rng = a.range();

            static_assert(rng.find(8).value_or_npos() == 0);
            static_assert(rng.find(8, 0).value_or_npos() == 0);
            static_assert(rng.find(8, 1).value_or_npos() == 1);
            static_assert(rng.find(8, 2).value_or_npos() == constant::npos);
            static_assert(rng.find(12).value_or_npos() == 3);
            static_assert(rng.find(12, 0).value_or_npos() == 3);
            static_assert(rng.find(12, 1).value_or_npos() == 3);
            static_assert(rng.find(12, 2).value_or_npos() == 3);
            static_assert(rng.find(12, 3).value_or_npos() == 3);
            static_assert(rng.find(12, 4).value_or_npos() == constant::npos);
            static_assert(rng.find(3).value_or_npos() == constant::npos);
            static_assert(rng.find(3, 99).value_or_npos() == constant::npos);

            static_assert(rng.find_in_reverse(8).value_or_npos() == 1);
            static_assert(rng.find_in_reverse(8, 99).value_or_npos() == 1);
            static_assert(rng.find_in_reverse(8, 1).value_or_npos() == 1);
            static_assert(rng.find_in_reverse(8, 0).value_or_npos() == 0);
            static_assert(rng.find_in_reverse(12).value_or_npos() == 3);
            static_assert(rng.find_in_reverse(12, 99).value_or_npos() == 3);
            static_assert(rng.find_in_reverse(12, 3).value_or_npos() == 3);
            static_assert(rng.find_in_reverse(12, 2).value_or_npos() == constant::npos);
            static_assert(rng.find_in_reverse(12, 1).value_or_npos() == constant::npos);
            static_assert(rng.find_in_reverse(12, 0).value_or_npos() == constant::npos);
            static_assert(rng.find_in_reverse(3).value_or_npos() == constant::npos);
            static_assert(rng.find_in_reverse(3, 99).value_or_npos() == constant::npos);
        }

        // constexpr void transform(OneArgOp op)
        {
            array a{1, 3};
            range::contiguous rng{init::from, a.begin(), a.end()};
            rng.transform([](int i) { return i * i; });
            snn_require(algo::is_equal(a.range(), {1, 9}));
        }

        // constexpr bool all(OneArgPred p) const
        // constexpr bool any(OneArgPred p) const
        // constexpr bool none(OneArgPred p) const
        {
            static constexpr array a{8, 10, 12};
            constexpr auto rng = a.range();
            static_assert(std::is_same_v<decltype(rng), const range::contiguous<const int*>>);
            static_assert(rng.all(math::fn::is_even{}));
            static_assert(!rng.all(math::fn::is_odd{}));
            static_assert(rng.any(math::fn::is_even{}));
            static_assert(!rng.any(math::fn::is_odd{}));
            static_assert(!rng.none(math::fn::is_even{}));
            static_assert(rng.none(math::fn::is_odd{}));
        }
        {
            static constexpr array a{1, 3, 5};
            constexpr auto rng = a.range();
            static_assert(std::is_same_v<decltype(rng), const range::contiguous<const int*>>);
            static_assert(!rng.all(math::fn::is_even{}));
            static_assert(rng.all(math::fn::is_odd{}));
            static_assert(!rng.any(math::fn::is_even{}));
            static_assert(rng.any(math::fn::is_odd{}));
            static_assert(rng.none(math::fn::is_even{}));
            static_assert(!rng.none(math::fn::is_odd{}));
        }
        {
            static constexpr array a{1, 4, 7};
            constexpr auto rng = a.range();
            static_assert(std::is_same_v<decltype(rng), const range::contiguous<const int*>>);
            static_assert(!rng.all(math::fn::is_even{}));
            static_assert(!rng.all(math::fn::is_odd{}));
            static_assert(rng.any(math::fn::is_even{}));
            static_assert(rng.any(math::fn::is_odd{}));
            static_assert(!rng.none(math::fn::is_even{}));
            static_assert(!rng.none(math::fn::is_odd{}));
        }

        // void drop_front(assume::not_empty_t)
        {
            range::contiguous rng{"abcdef"};

            snn_require(rng.count() == 6);
            snn_require(rng.front(assume::not_empty) == 'a');
            snn_require(rng.back(assume::not_empty) == 'f');

            rng.drop_front(assume::not_empty);

            snn_require(rng.count() == 5);
            snn_require(rng.front(assume::not_empty) == 'b');
            snn_require(rng.back(assume::not_empty) == 'f');
        }

        // void drop_back(assume::not_empty_t)
        {
            range::contiguous rng{"abcdef"};

            snn_require(rng.count() == 6);
            snn_require(rng.front(assume::not_empty) == 'a');
            snn_require(rng.back(assume::not_empty) == 'f');

            rng.drop_back(assume::not_empty);

            snn_require(rng.count() == 5);
            snn_require(rng.front(assume::not_empty) == 'a');
            snn_require(rng.back(assume::not_empty) == 'e');
        }

        // contiguous pop_back_n(const usize count)
        {
            range::contiguous rng{"abcdef"};

            auto r = rng.pop_back_n(0);
            snn_require(r.is_empty());

            snn_require(rng.count() == 6);
            snn_require(rng.size() == 6);
            snn_require(rng.byte_size().get() == 6);

            r = rng.pop_back_n(2);
            snn_require(r.count() == 2);
            snn_require(r.front(assume::not_empty) == 'e');
            snn_require(r.back(assume::not_empty) == 'f');

            snn_require(rng.count() == 4);
            snn_require(rng.front(assume::not_empty) == 'a');
            snn_require(rng.back(assume::not_empty) == 'd');

            r = rng.pop_back_n(5);
            snn_require(r.count() == 4);
            snn_require(r.front(assume::not_empty) == 'a');
            snn_require(r.back(assume::not_empty) == 'd');

            snn_require(rng.is_empty());
            snn_require(rng.begin() == rng.end());

            r = rng.pop_back_n(5);
            snn_require(r.is_empty());

            snn_require(rng.is_empty());
            snn_require(rng.begin() == rng.end());
        }
        {
            range::contiguous rng{"abcdef"};

            auto r = rng.pop_back_n(6);
            snn_require(r.count() == 6);
            snn_require(r.front(assume::not_empty) == 'a');
            snn_require(r.back(assume::not_empty) == 'f');

            snn_require(rng.is_empty());
            snn_require(rng.begin() == rng.end());
        }

        {
            range::contiguous rng{"abcDEFghijklmnopqrstuvwxyz"};
            static_assert(sizeof(rng) == (sizeof(char*) * 2));

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.count() == 26);

            snn_require(rng.at(0, assume::within_bounds) == 'a');
            snn_require(rng.at(25, assume::within_bounds) == 'z');
            snn_require(!rng.at(26).has_value());

            snn_require(rng.back(assume::not_empty) == 'z');
            snn_require(rng.pop_back(assume::not_empty) == 'z');
            snn_require(rng.count() == 25);
            snn_require(rng.back(assume::not_empty) == 'y');
            snn_require(rng.back().value() == 'y');

            snn_require(rng.has_front('a'));
            snn_require(!rng.has_front('b'));

            snn_require(rng.has_front<0>('a'));
            snn_require(rng.has_front<1>('b'));
            snn_require(rng.has_front<2>('c'));
            snn_require(rng.has_front<24>('y'));
            snn_require(!rng.has_front<0>('b'));
            snn_require(!rng.has_front<1>('c'));
            snn_require(!rng.has_front<2>('d'));
            snn_require(!rng.has_front<25>('z'));

            snn_require(rng.has_front(""));
            snn_require(rng.has_front("a"));
            snn_require(rng.has_front("ab"));
            snn_require(rng.has_front("abc"));
            snn_require(!rng.has_front("bc"));
            snn_require(!rng.has_front("aBc"));

            snn_require(rng.has_front<1>("bc"));
            snn_require(rng.has_front<2>("cDE"));
            snn_require(rng.has_front<23>("xy"));
            snn_require(!rng.has_front<1>("BC"));
            snn_require(!rng.has_front<2>("efg"));
            snn_require(!rng.has_front<24>("yz"));

            snn_require(rng.has_front_if(chr::is_alpha_lower));
            snn_require(rng.has_front_if(fn::_not{chr::is_alpha_upper}));
            snn_require(!rng.has_front_if(chr::is_alpha_upper));

            snn_require(rng.has_front_if<1>(chr::is_alpha_lower));
            snn_require(!rng.has_front_if<1>(chr::is_alpha_upper));
            snn_require(rng.has_front_if<24>(chr::is_alpha_lower));
            snn_require(!rng.has_front_if<25>(chr::is_alpha_lower));

            snn_require(rng.drop_front('a'));
            snn_require(rng.count() == 24);
            snn_require(rng.has_front('b'));

            snn_require(!rng.drop_front('a'));
            snn_require(rng.count() == 24);
            snn_require(rng.has_front('b'));

            snn_require(rng.drop_front("bc"));
            snn_require(rng.count() == 22);
            snn_require(rng.has_front('D'));

            snn_require(!rng.drop_front("bc"));
            snn_require(rng.count() == 22);
            snn_require(rng.has_front('D'));

            snn_require(!rng.drop_front("def"));
            snn_require(rng.count() == 22);
            snn_require(rng.has_front('D'));

            snn_require(rng.drop_front_icase(ascii::as_lower("def")));
            snn_require(rng.count() == 19);
            snn_require(rng.has_front('g'));

            snn_require(!rng.drop_front_icase(ascii::as_lower("def")));
            snn_require(rng.count() == 19);
            snn_require(rng.has_front('g'));
            snn_require(!rng.drop_front_icase(ascii::as_lower("ghj")));
            snn_require(rng.count() == 19);
            snn_require(rng.has_front('g'));
            snn_require(!rng.drop_front_icase(ascii::as_lower("fhi")));
            snn_require(rng.count() == 19);
            snn_require(rng.has_front('g'));

            snn_require(rng.drop_front_if([](const char c) { return c >= 'g' && c <= 'i'; }));
            snn_require(rng.count() == 18);
            snn_require(rng.has_front('h'));
            snn_require(rng.drop_front_if([](const char c) { return c >= 'g' && c <= 'i'; }));
            snn_require(rng.count() == 17);
            snn_require(rng.has_front('i'));
            snn_require(rng.drop_front_if([](const char c) { return c >= 'g' && c <= 'i'; }));
            snn_require(rng.count() == 16);
            snn_require(rng.has_front('j'));
            snn_require(!rng.drop_front_if([](const char c) { return c >= 'g' && c <= 'i'; }));
            snn_require(rng.count() == 16);
            snn_require(rng.has_front('j'));

            const auto p = rng.pop_front_integral<u32>();
            snn_require(p.value == 0);
            snn_require(p.count == 0);
            snn_require(rng.count() == 16);
            snn_require(rng.has_front('j'));
        }

        {
            constexpr auto rng = cstrview{"abcdefghijklmnopqrstuvwxyz"}.range();

            using rng_type = decltype(rng);
            static_assert(std::is_same_v<rng_type, const range::contiguous<const char*>>);
            static_assert(std::is_same_v<front_value_t<rng_type&>, char>);

            static_assert(rng);
            static_assert(!rng.is_empty());
            static_assert(rng.count() == 26);
            static_assert(rng.has_front('a'));
            static_assert(rng.has_front<1>('b'));
            static_assert(rng.has_front("ab"));
            static_assert(!rng.has_front("bc"));
            static_assert(rng.has_front("abcdef"));
            static_assert(rng.has_front<4>("efg"));
            static_assert(!rng.has_front<4>("eeg"));
            static_assert(rng.front(assume::not_empty) == 'a');
            static_assert(rng.front().value() == 'a');
            static_assert(rng.back(assume::not_empty) == 'z');
            static_assert(rng.back().value() == 'z');
            static_assert(rng.at(1, assume::within_bounds) == 'b');
            static_assert(rng.at(1).value() == 'b');
            static_assert(!rng.at(99).has_value());
        }

        {
            constexpr cstrrng rng{"ABc"};

            static_assert(rng);
            static_assert(!rng.is_empty());
            static_assert(rng.count() == 3);
            static_assert(rng.has_back('c'));
            static_assert(rng.has_back(""));
            static_assert(rng.has_back("c"));
            static_assert(rng.has_back("Bc"));
            static_assert(rng.has_back("ABc"));
            static_assert(rng.has_back_if(chr::is_alpha_lower));

            static_assert(!rng.has_back('C'));
            static_assert(!rng.has_back('\0'));
            static_assert(!rng.has_back("BC"));
            static_assert(!rng.has_back("cB"));
            static_assert(!rng.has_back("ABc\0"));
            static_assert(!rng.has_back("ABcc"));
            static_assert(!rng.has_back("AABc"));
            static_assert(!rng.has_back_if(chr::is_alpha_upper));
        }

        {
            constexpr cstrrng rng;

            static_assert(!rng);
            static_assert(rng.is_empty());
            static_assert(rng.count() == 0);
            static_assert(!rng.has_back('\0'));
            static_assert(!rng.has_back('c'));
            static_assert(!rng.has_back("c"));
            static_assert(!rng.has_back("Bc"));
            static_assert(!rng.has_back("ABc"));
            static_assert(!rng.has_back_if(chr::is_alpha_lower));
        }

        {
            range::contiguous rng{"abcdefghijklmnopqrstuvwxyz"};

            snn_require(rng.pop_front_n(99).count() == 26);

            snn_require(rng.count() == 0);
            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.has_front('a'));
            snn_require(!rng.has_front('b'));

            snn_require(!rng.has_front<0>('a'));
            snn_require(!rng.has_front<1>('b'));
            snn_require(!rng.has_front<2>('c'));
            snn_require(!rng.has_front<25>('z'));
            snn_require(!rng.has_front<0>('b'));
            snn_require(!rng.has_front<1>('c'));
            snn_require(!rng.has_front<2>('d'));
            snn_require(!rng.has_front<26>('\0'));

            snn_require(!rng.has_front("ab"));
            snn_require(!rng.has_front("abc"));
            snn_require(!rng.has_front("bc"));
            snn_require(!rng.has_front("aBc"));

            snn_require(!rng.has_front<1>("bc"));
            snn_require(!rng.has_front<2>("cde"));
            snn_require(!rng.has_front<24>("yz"));
            snn_require(!rng.has_front<1>("BC"));
            snn_require(!rng.has_front<2>("efg"));
            snn_require(!rng.has_front<25>("z\0"));

            snn_require(!rng.has_front_if(chr::is_alpha_lower));
            snn_require(!rng.has_front_if(fn::_not{chr::is_alpha_upper}));
            snn_require(!rng.has_front_if(chr::is_alpha_upper));

            snn_require(!rng.has_front_if<1>(chr::is_alpha_lower));
            snn_require(!rng.has_front_if<1>(chr::is_alpha_upper));
            snn_require(!rng.has_front_if<25>(chr::is_alpha_lower));
            snn_require(!rng.has_front_if<26>(chr::is_alpha_lower));

            snn_require(!rng.drop_front('a'));
            snn_require(!rng.drop_front("bc"));
            snn_require(!rng.drop_front_icase(ascii::as_lower("def")));
            snn_require(!rng.drop_front_icase(ascii::as_lower("ghj")));
            snn_require(!rng.drop_front_icase(ascii::as_lower("fhi")));

            const auto p = rng.pop_front_integral<u32>();
            snn_require(p.value == 0);
            snn_require(p.count == 0);

            snn_require(rng.count() == 0);
            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        {
            range::contiguous rng{".,abc123ghi000456"};
            snn_require(rng.count() == 17);

            snn_require(rng.front(assume::not_empty) == '.');
            snn_require(rng.count() == 17);
            snn_require(rng.pop_front(assume::not_empty) == '.');
            snn_require(rng.count() == 16);

            snn_require(rng.front().value() == ',');
            snn_require(rng.count() == 16);
            snn_require(rng.pop_front().value() == ',');
            snn_require(rng.count() == 15);

            auto p = rng.pop_front_integral<u32>();
            snn_require(p.value == 0);
            snn_require(p.count == 0);
            snn_require(rng.count() == 15);

            snn_require(!rng.drop_front("ghi"));
            snn_require(rng.count() == 15);

            snn_require(rng.drop_front("abc"));
            snn_require(rng.count() == 12);

            p = rng.pop_front_integral<u32>();
            snn_require(p.value == 123);
            snn_require(p.count == 3);
            snn_require(rng.count() == 9);

            snn_require(!rng.drop_front("abc"));
            snn_require(rng.count() == 9);

            snn_require(rng.drop_front("ghi"));
            snn_require(rng.count() == 6);

            p = rng.pop_front_integral<u32>();
            snn_require(p.value == 0);
            snn_require(p.count == 1);
            snn_require(rng.count() == 5);

            p = rng.pop_front_integral<u32, math::base::decimal, 3>(ascii::leading_zeros::allow);
            snn_require(p.value == 4);
            snn_require(p.count == 3);
            snn_require(rng.count() == 2);

            p = rng.pop_front_integral<u32>(ascii::leading_zeros::allow);
            snn_require(p.value == 56);
            snn_require(p.count == 2);
            snn_require(rng.count() == 0);

            snn_require(!rng);
            snn_require(rng.is_empty());
            snn_require(!rng.front().has_value());
            snn_require(!rng.back().has_value());
            snn_require(!rng.pop_front().has_value());
            snn_require(!rng.pop_back().has_value());
        }

        {
            range::contiguous rng{"abc1234"};
            snn_require(rng.count() == 7);

            snn_require(!rng.drop_front('b'));
            snn_require(rng.count() == 7);

            snn_require(!rng.drop_front<1>('a'));
            snn_require(rng.count() == 7);

            snn_require(rng.drop_front<1>('b'));
            snn_require(rng.count() == 5);
            snn_require(rng.has_front("c1234"));

            snn_require(rng.drop_front<2>('2'));
            snn_require(rng.count() == 2);
            snn_require(rng.has_front("34"));

            snn_require(!rng.drop_front<0>('4'));
            snn_require(!rng.drop_front<1>('3'));
            snn_require(!rng.drop_front<2>('4'));
            snn_require(rng.count() == 2);
            snn_require(rng.has_front("34"));

            snn_require(rng.drop_front<1>('4'));
            snn_require(rng.is_empty());
        }

        {
            range::contiguous rng{"abc1234"};
            snn_require(rng.count() == 7);

            snn_require(!rng.drop_front("bc1"));
            snn_require(!rng.drop_front<2>("bc1"));
            snn_require(rng.count() == 7);

            snn_require(rng.drop_front<1>("bc1"));
            snn_require(rng.count() == 3);
            snn_require(rng.has_front("234"));

            snn_require(!rng.drop_front<1>("234"));
            snn_require(!rng.drop_front<1>("34\0"));
            snn_require(!rng.drop_front<2>("4\0"));
            snn_require(rng.count() == 3);
            snn_require(rng.has_front("234"));

            snn_require(rng.drop_front<1>("34"));
            snn_require(rng.is_empty());
        }

        // drop_back(' ')
        // drop_back("...")
        {
            cstrrng rng{"abc"};

            snn_require(!rng.drop_back('\0'));
            snn_require(!rng.drop_back('C'));
            snn_require(rng.has_back('c'));
            snn_require(rng.size() == 3);

            snn_require(rng.drop_back('c'));
            snn_require(rng.has_back('b'));
            snn_require(rng.size() == 2);

            snn_require(rng.drop_back(""));
            snn_require(rng.has_back('b'));
            snn_require(rng.size() == 2);

            snn_require(!rng.drop_back("abc"));
            snn_require(!rng.drop_back("ab\0"));
            snn_require(!rng.drop_back("AB"));
            snn_require(!rng.drop_back("B"));
            snn_require(rng.has_back("ab"));
            snn_require(rng.size() == 2);

            snn_require(rng.drop_back("ab"));
            snn_require(rng.is_empty());

            snn_require(!rng.drop_back("ab"));
            snn_require(!rng.drop_back('a'));
        }
        {
            auto a   = to_array("abc");
            auto rng = a.range();
            static_assert(std::is_same_v<decltype(rng), strrng>);

            snn_require(!rng.drop_back('\0'));
            snn_require(!rng.drop_back('C'));
            snn_require(rng.has_back('c'));
            snn_require(rng.size() == 3);

            snn_require(rng.drop_back('c'));
            snn_require(rng.has_back('b'));
            snn_require(rng.size() == 2);

            snn_require(rng.drop_back(""));
            snn_require(rng.has_back('b'));
            snn_require(rng.size() == 2);

            snn_require(!rng.drop_back("abc"));
            snn_require(!rng.drop_back("ab\0"));
            snn_require(!rng.drop_back("AB"));
            snn_require(!rng.drop_back("B"));
            snn_require(rng.has_back("ab"));
            snn_require(rng.size() == 2);

            snn_require(rng.drop_back("ab"));
            snn_require(rng.is_empty());

            snn_require(!rng.drop_back("ab"));
            snn_require(!rng.drop_back('a'));
        }

        {
            range::contiguous rng{"abc123ghij"};
            snn_require(rng.count() == 10);
            snn_require(rng.has_front('a'));
            {
                auto r = rng.pop_front_n(0);
                snn_require(r.count() == 0);
                snn_require(!r);
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 10);
            snn_require(rng.has_front('a'));
            {
                auto r = rng.pop_front_n(1);
                snn_require(r.count() == 1);
                snn_require(r.has_front('a'));
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 9);
            snn_require(rng.has_front('b'));
            {
                auto r = rng.pop_front_n(5);
                snn_require(r.count() == 5);
                snn_require(r.has_front('b'));
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 4);
            snn_require(rng.has_front('g'));
            {
                auto r = rng.pop_front_n(4);
                snn_require(r.count() == 4);
                snn_require(r.has_front('g'));
                snn_require(r.end() == rng.begin());
            }
            snn_require(!rng);
            snn_require(rng.count() == 0);
            {
                auto r = rng.pop_front_n(4);
                snn_require(r.count() == 0);
                snn_require(!r);
                snn_require(r.end() == rng.begin());
            }
            snn_require(!rng);
            snn_require(rng.count() == 0);
        }
        {
            range::contiguous rng{"abc123ghij"};
            snn_require(rng.count() == 10);
            snn_require(rng.has_front('a'));

            auto r = rng.pop_front_n(20);
            snn_require(r.count() == 10);
            snn_require(r.has_front('a'));
            snn_require(r.end() == rng.begin());

            snn_require(!rng);
            snn_require(rng.count() == 0);
        }

        {
            range::contiguous rng{"Å\0\xF0\x9F\x8D\x83"
                                  "a"};
            snn_require(rng.count() == 8);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{0xC5, 2});
            snn_require(rng.count() == 6);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{0x0, 1});
            snn_require(rng.count() == 5);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{0x1F343, 4});
            snn_require(rng.count() == 1);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{0x61, 1});
            snn_require(rng.count() == 0);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{utf8::invalid, 0});
            snn_require(rng.count() == 0);
        }
        {
            range::contiguous rng{"\xF0\x82\x82\xAC"}; // Overlong - "EURO SIGN" (U+20AC)
            snn_require(rng.count() == 4);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{utf8::invalid, 1});
            snn_require(rng.count() == 3);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{utf8::invalid, 1});
            snn_require(rng.count() == 2);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{utf8::invalid, 1});
            snn_require(rng.count() == 1);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{utf8::invalid, 1});
            snn_require(rng.count() == 0);
            snn_require(rng.pop_front_codepoint() == pair::value_count<u32>{utf8::invalid, 0});
            snn_require(rng.count() == 0);
        }

        {
            range::contiguous rng{"abc1234"};
            snn_require(rng.count() == 7);
            snn_require(rng.has_front('a'));
            {
                auto r = rng.pop_front_while(chr::is_alpha_upper);
                snn_require(r.count() == 0);
                snn_require(!r);
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 7);
            snn_require(rng.has_front('a'));
            {
                auto r = rng.pop_front_while(chr::is_alpha_lower);
                snn_require(r.count() == 3);
                snn_require(r);
                snn_require(r.has_front('a'));
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 4);
            snn_require(rng.has_front('1'));
            {
                auto r = rng.pop_front_while(chr::is_ascii);
                snn_require(r.count() == 4);
                snn_require(r);
                snn_require(r.has_front('1'));
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 0);
            snn_require(!rng);
            {
                auto r = rng.pop_front_while(chr::is_ascii);
                snn_require(r.count() == 0);
                snn_require(!r);
                snn_require(r.end() == rng.begin());
            }
            snn_require(rng.count() == 0);
            snn_require(!rng);
        }

        {
            range::contiguous rng{"a123"};
            snn_require(rng.count() == 4);
            snn_require(rng.back(assume::not_empty) == '3');
            {
                auto r = rng.pop_back_while(chr::is_alpha_lower);
                snn_require(r.count() == 0);
                snn_require(!r);
                snn_require(r.begin() == rng.end());
            }
            snn_require(rng.count() == 4);
            snn_require(rng.back(assume::not_empty) == '3');
            {
                auto r = rng.pop_back_while(chr::is_digit);
                snn_require(r.count() == 3);
                snn_require(r);
                snn_require(cstrview{r} == "123");
                snn_require(r.begin() == rng.end());
            }
            snn_require(rng.count() == 1);
            snn_require(rng.back(assume::not_empty) == 'a');
            {
                auto r = rng.pop_back_while(chr::is_alpha_lower);
                snn_require(r.count() == 1);
                snn_require(r);
                snn_require(cstrview{r} == "a");
                snn_require(r.begin() == rng.end());
            }
            snn_require(rng.count() == 0);
            snn_require(!rng);
            {
                auto r = rng.pop_back_while(chr::is_alpha_lower);
                snn_require(r.count() == 0);
                snn_require(!r);
                snn_require(r.begin() == rng.end());
            }
            snn_require(rng.count() == 0);
            snn_require(!rng);
        }

        {
            str s{"abcDEF"};
            auto rng = s.range();

            snn_require(rng.count() == 6);
            snn_require(rng.size() == 6);
            snn_require(rng.byte_size().get() == 6);

            snn_require(rng.has_front('a'));
            {
                auto r = rng.pop_front_while(chr::is_alpha_lower, chr::to_alpha_upper_inplace);
                snn_require(r);
                snn_require(r.count() == 3);
                snn_require(r.has_front('A'));
                snn_require(cstrview{r} == "ABC");
            }
            snn_require(rng.count() == 3);
            snn_require(rng.has_front('D'));
            {
                auto r = rng.pop_front_while(chr::is_alpha_upper, chr::to_alpha_lower_inplace);
                snn_require(r);
                snn_require(r.count() == 3);
                snn_require(r.has_front('d'));
                snn_require(cstrview{r} == "def");
            }
            snn_require(!rng);
            snn_require(rng.count() == 0);
            snn_require(s == "ABCdef");
        }

        {
            using utf8::invalid;

            static_assert(app::has_codepoints(cstrrng{""}, {}));
            static_assert(app::has_codepoints(cstrrng{"\xf0"}, {invalid}));
            static_assert(app::has_codepoints(cstrrng{"ab"}, {0x61, 0x62}));
            static_assert(app::has_codepoints(cstrrng{"Å Ö"}, {0xc5, 0x20, 0xd6}));
            static_assert(app::has_codepoints(cstrrng{"\xf0\x9f\x90\x99"}, {0x1f419}));
            static_assert(app::has_codepoints(cstrrng{"\xf0\x9f\x8d\x82\x61"}, {0x1f342, 0x61}));
            static_assert(app::has_codepoints(cstrrng{"\xf0\x9f\x8d\x83\x61\xc7\xe0"},
                                              {0x1f343, 0x61, invalid, invalid}));
            static_assert(
                app::has_codepoints(cstrrng{"\xf0\x9f\x8d\x82\xf0\x9f\x8d\x83\xf0\x9f\x8d\x8a"},
                                    {0x1f342, 0x1f343, 0x1f34a}));
            static_assert(
                app::has_codepoints(cstrrng{"\xf0\xf0\x9f\x8d\x82\xf0\x9f\x8d\x83\xf0\x9f\x8d\x8a"},
                                    {invalid, 0x1f342, 0x1f343, 0x1f34a}));
            static_assert(app::has_codepoints(cstrrng{"\xf0\x9f\x8d\x82\xc5\xf0\x9f\x8d\x83"},
                                              {0x1f342, invalid, 0x1f343}));

            // Overlong
            static_assert(app::has_codepoints(cstrrng{"\xF0\x82\x82\xAC"},
                                              {invalid, invalid, invalid, invalid}));

            static_assert(!app::has_codepoints(cstrrng{""}, {0x10}));
            static_assert(!app::has_codepoints(cstrrng{"ab"}, {0x62, 0x63}));
            static_assert(!app::has_codepoints(cstrrng{"\xf0\x9f\x8d\x83\x61"}, {0x1a343, 0x61}));
        }

        // without_suffix
        {
            range::contiguous rng{"abc123"};
            snn_require(rng.count() == 6);
            snn_require(cstrview{rng} == "abc123");

            auto sub = rng;
            snn_require(sub.count() == 6);
            snn_require(cstrview{sub} == "abc123");

            auto r1 = rng.without_suffix(sub, assume::is_valid);
            snn_require(r1.is_empty());
            snn_require(cstrview{r1} == "");

            snn_require(rng.count() == 6);
            snn_require(cstrview{rng} == "abc123");
            snn_require(sub.count() == 6);
            snn_require(cstrview{sub} == "abc123");

            sub.pop_front_n(3);

            auto r2 = rng.without_suffix(sub, assume::is_valid);
            snn_require(r2.count() == 3);
            snn_require(cstrview{r2} == "abc");

            snn_require(rng.count() == 6);
            snn_require(cstrview{rng} == "abc123");
            snn_require(sub.count() == 3);
            snn_require(cstrview{sub} == "123");
        }
    }
}
