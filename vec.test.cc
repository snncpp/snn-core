// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/vec.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/algo/is_equal.hh"
#include <string> // string

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            vec<str> v;

            snn_require(!v); // Empty.
            snn_require(v.is_empty());
            snn_require(v.capacity() == 0);
            snn_require(v.count() == 0);
            // snn_require(v.size() == 0); // Will not compile, `sizeof(str) != 1`.

            v.append("Hello");
            v.append_inplace(container::fill, 8u, 'a');

            snn_require(v); // Not empty.
            snn_require(!v.is_empty());
            snn_require(v.capacity() >= 2);
            snn_require(v.count() == 2);
            snn_require(v.byte_size().get() == 48);

            snn_require(v.at(0).value() == "Hello");
            snn_require(v.at(1).value() == "aaaaaaaa");

            snn_require(algo::is_equal(v.range(), init_list<cstrview>{"Hello", "aaaaaaaa"}));

            v.reserve_append(2); // Optimization (not required).
            v << "this" << "and-that";

            snn_require(v.count() == 4);
            snn_require(algo::is_equal(v.range(), init_list<cstrview>{"Hello", "aaaaaaaa", "this",
                                                                      "and-that"}));

            snn_require(algo::is_equal(v.view(2).range(), init_list<cstrview>{"this", "and-that"}));

            snn_require(v.none(fn::is_empty{}));

            v.append("");
            snn_require(!v.none(fn::is_empty{}));

            return true;
        }

        class trivial final
        {
          public:
            constexpr trivial() noexcept
                : s_{'m', 'n', 'o', 'p'}
            {
            }

            constexpr trivial(char a, char b, char c, char d) noexcept
                : s_{a, b, c, d}
            {
            }

            constexpr trivial(const char (&s)[5])
                : trivial{s[0], s[1], s[2], s[3]}
            {
                if (s_.front(promise::not_empty) == 'Z')
                {
                    throw_or_abort(generic::error::invalid_value);
                }
            }

            constexpr const char* data() const noexcept
            {
                return s_.begin();
            }

            constexpr bool operator==(const char (&s)[5]) const noexcept
            {
                return s_.view() == s;
            }

            constexpr bool operator==(const trivial& other) const noexcept = default;

          private:
            array<char, 4> s_;
        };

        template <typename T, usize SmallCapacity>
        [[nodiscard]] constexpr bool count_eq(const vec<T, SmallCapacity>& v, const usize count)
        {
            if (v.count() == count && v.capacity() >= count &&
                v.byte_size().get() == (count * sizeof(T)))
            {
                return (count == 0 && v.is_empty() && !v) || (count > 0 && !v.is_empty() && v);
            }
            return false;
        }

        template <typename Vec, typename... Args>
        constexpr bool test_vec1(Args&&... args)
        {
            using T = typename Vec::value_type;

            // Default constructor.
            {
                Vec v;
                snn_require(count_eq(v, 0));
                snn_require(!count_eq(v, 1));
            }

            // Reserve constructor.
            {
                Vec v{container::reserve, 0};
                snn_require(count_eq(v, 0));
                snn_require(v.capacity() == v.default_capacity());
            }
            {
                Vec v{container::reserve, 20};
                snn_require(count_eq(v, 0));
                snn_require(v.capacity() >= 20);
            }

            // Initializer list constructor.
            {
                const T x{std::forward<Args>(args)...};

                {
                    Vec v{}; // Default constructor.
                    snn_require(count_eq(v, 0));
                    snn_require(v.default_capacity() > 0 || v.capacity() == 0);
                }
                {
                    Vec v{x};
                    snn_require(!count_eq(v, 0));
                    snn_require(count_eq(v, 1));
                    snn_require(v.at(0).value() == x);
                }
                {
                    Vec v{x, x};
                    snn_require(count_eq(v, 2));
                    snn_require(v.at(0).value() == x);
                    snn_require(v.at(1).value() == x);
                }
                {
                    Vec v{x, x, x};
                    snn_require(count_eq(v, 3));
                    snn_require(v.at(0).value() == x);
                    snn_require(v.at(1).value() == x);
                    snn_require(v.at(2).value() == x);
                }
            }

            // Iterator constructor.
            {
                Vec v1;
                Vec v2{meta::iterators, v1.begin(), v1.end()};
                snn_require(count_eq(v2, 0));
            }

            // Iterator constructor.
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                Vec v1;
                v1.append(y);
                v1.append(x);
                v1.append(x);
                v1.append(y);
                snn_require(count_eq(v1, 4));

                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                Vec v2{meta::iterators, v1.begin() + 1, v1.end() - 1};

                SNN_DIAGNOSTIC_POP

                snn_require(count_eq(v2, 2));
                snn_require(v2.at(0).value() == x);
                snn_require(v2.at(1).value() == x);
            }

            // Copy constructor.
            {
                Vec v1;
                snn_require(count_eq(v1, 0));
                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v1, 1));
                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v1, 2));

                Vec v2{v1};
                snn_require(count_eq(v2, 2));
                snn_require(v1 == v2);
            }

            // Copy assignment operator.
            {
                Vec v1;
                v1.append_inplace(std::forward<Args>(args)...);
                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v1, 2));

                Vec v2;
                v2.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v2, 1));
                snn_require(v1 != v2);

                v2 = v1;
                snn_require(count_eq(v2, 2));
                snn_require(v1 == v2);
            }

            // Move constructor.
            {
                Vec v1;
                v1.append_inplace(std::forward<Args>(args)...);
                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v1, 2));

                Vec v2{std::move(v1)};
                snn_require(count_eq(v2, 2));
                snn_require(count_eq(v1, 0));

                // Reuse v1.
                v1.append_inplace(std::forward<Args>(args)...);
                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v1, 2));
            }

            // Move assignment operator.
            if constexpr (Vec::default_capacity() == 0)
            {
                Vec v1;
                v1.append_inplace(std::forward<Args>(args)...);
                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v1, 2));

                Vec v2;
                v2.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v2, 1));
                snn_require(v1 != v2);

                v2 = std::move(v1);
                snn_require(count_eq(v2, 2));
                snn_require(count_eq(v1, 1));
                snn_require(v1 != v2);
            }

            // begin/end
            {
                Vec v;
                snn_require(v.begin() == v.end());
            }
            {
                Vec v;
                v.append_inplace(std::forward<Args>(args)...);
                v.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v, 2));

                auto cur  = v.begin();
                auto last = v.end();

                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                usize count = 0;
                while (cur < last)
                {
                    ++count;
                    ++cur;
                }

                SNN_DIAGNOSTIC_POP

                snn_require(count == 2);
                snn_require(cur == last);
            }

            // append(T)
            {
                T x{std::forward<Args>(args)...};

                Vec v;
                v.append(x);
                v.append(x);
                v.append(x);
                v.append(x);
                snn_require(count_eq(v, 4));

                usize count = 0;
                for (const auto& y : v)
                {
                    snn_require(y == x);
                    ++count;
                }
                snn_require(count == 4);

                while (v.count() < v.capacity())
                {
                    v.append(x);
                }
                // Append from self at capacity.
                v.append(v.back(promise::not_empty));
            }

            // append(vec)
            {
                T x{std::forward<Args>(args)...};

                Vec v1;
                v1.append(x);
                v1.append(x);
                v1.append(x);
                v1.append(x);
                snn_require(count_eq(v1, 4));

                Vec v2;
                v2.append(v1);
                v2.append(x);
                v2.append(v1);

                usize count = 0;
                for (const auto& y : v2)
                {
                    snn_require(y == x);
                    ++count;
                }
                snn_require(count == 9);

                // Append self.
                v2.append(v2);
                v2.append(v2);
                v2.append(v2);

                count = 0;
                for (const auto& y : v2)
                {
                    snn_require(y == x);
                    ++count;
                }
                snn_require(count == 72);
            }

            // append_inplace
            {
                Vec v;

                v.append_inplace(std::forward<Args>(args)...);
                v.append_inplace(std::forward<Args>(args)...);
                v.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v, 3));

                T x{std::forward<Args>(args)...};
                T y{};
                snn_require(x != y);

                usize count = 0;
                for (const auto& e : v)
                {
                    snn_require(e == x);
                    ++count;
                }
                snn_require(count == 3);

                v.append_inplace();
                snn_require(count_eq(v, 4));
                snn_require(v.at(0).value() == x);
                snn_require(v.at(1).value() == x);
                snn_require(v.at(2).value() == x);
                snn_require(v.at(3).value() == y);
            }

            // at
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                snn_require(x != y);

                Vec v;

                snn_require_throws_code(v.at(0).value(), generic::error::no_value);
                snn_require_throws_code(v.at(1).value(), generic::error::no_value);

                const Vec v_const_copy1{v};

                snn_require_throws_code(v_const_copy1.at(0).value(), generic::error::no_value);
                snn_require_throws_code(v_const_copy1.at(1).value(), generic::error::no_value);

                v.append(x);
                v.append(x);
                snn_require(count_eq(v, 2));

                snn_require_nothrow(v.at(0).value());
                snn_require_nothrow(v.at(1).value());
                snn_require_throws_code(v.at(2).value(), generic::error::no_value);

                const Vec v_const_copy2{v};

                snn_require_nothrow(v_const_copy2.at(0).value());
                snn_require_nothrow(v_const_copy2.at(1).value());
                snn_require_throws_code(v_const_copy2.at(2).value(), generic::error::no_value);

                usize count = 0;
                for (const auto& xy : v)
                {
                    snn_require(xy == x);
                    ++count;
                }
                snn_require(count == 2);

                for (usize i = 0; i < v.count(); ++i)
                {
                    snn_require(v.at(i).value() == x);
                    v.at(i).value() = y;
                    snn_require(v.at(i).value() == y);
                }

                count = 0;
                for (const auto& xy : v)
                {
                    snn_require(xy == y);
                    ++count;
                }
                snn_require(count == 2);
            }

            // at(pos, promise::within_bounds_t)
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                snn_require(x != y);

                Vec v;

                v.append(x);
                v.append(x);
                snn_require(count_eq(v, 2));

                usize count = 0;
                for (const auto& xy : v)
                {
                    snn_require(xy == x);
                    ++count;
                }
                snn_require(count == 2);

                for (usize i = 0; i < v.count(); ++i)
                {
                    snn_require(v.at(i, promise::within_bounds) == x);
                    v.at(i, promise::within_bounds) = y;
                }

                const Vec v2 = v;
                snn_require(count_eq(v2, 2));

                for (usize i = 0; i < v2.count(); ++i)
                {
                    snn_require(v2.at(i, promise::within_bounds) == y);
                }
            }

            // back/front
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                snn_require(x != y);

                Vec v;

                snn_require(!v.front().has_value());
                snn_require(!v.back().has_value());

                v.append(x);
                snn_require(count_eq(v, 1));
                snn_require(v.front(promise::not_empty) == x);
                snn_require(v.back(promise::not_empty) == x);
                snn_require(v.front().value() == x);
                snn_require(v.back().value() == x);

                v.append(y);
                snn_require(count_eq(v, 2));
                snn_require(v.front(promise::not_empty) == x);
                snn_require(v.back(promise::not_empty) == y);
                snn_require(v.front().value() == x);
                snn_require(v.back().value() == y);

                const Vec v2 = v;
                snn_require(v == v2);

                v.back(promise::not_empty) = x;
                snn_require(count_eq(v, 2));
                snn_require(v.back(promise::not_empty) == x);

                snn_require(v != v2);

                snn_require(count_eq(v2, 2));
                snn_require(v2.front(promise::not_empty) == x);
                snn_require(v2.back(promise::not_empty) == y);
            }

            // capacity
            // See reserve.

            // clear/is_empty
            {
                Vec v;
                snn_require(count_eq(v, 0));

                v.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v, 1));
                v.append_inplace(std::forward<Args>(args)...);
                snn_require(count_eq(v, 2));

                v.clear();
                snn_require(count_eq(v, 0));
            }

            // contains
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;
                snn_require(!v.contains(x));
                snn_require(!v.contains(y));

                v.append(x);
                snn_require(v.contains(x));
                snn_require(!v.contains(y));

                v.append(y);
                snn_require(v.contains(x));
                snn_require(v.contains(y));

                v.drop_back(promise::not_empty);
                snn_require(v.contains(x));
                snn_require(!v.contains(y));

                v.drop_back(promise::not_empty);
                snn_require(!v.contains(x));
                snn_require(!v.contains(y));
            }

            // data/writable
            {
                const T y{};

                Vec v;
                if constexpr (Vec::default_capacity() == 0)
                {
                    snn_require(v.data() == nullptr);
                    snn_require(v.writable() == nullptr);
                }
                else
                {
                    snn_require(v.data() != nullptr);
                    snn_require(v.writable() != nullptr);
                }

                v.append(y);
                snn_require(count_eq(v, 1));
                snn_require(v.data() != nullptr);
                snn_require(v.writable() != nullptr);
                snn_require(v.data() == &v.at(0).value());
                snn_require(v.writable() == &v.at(0).value());

                auto data_ptr     = v.data();
                auto writable_ptr = v.writable();
                snn_require(data_ptr == writable_ptr);
                static_assert(std::is_same_v<decltype(data_ptr), const T*>);
                static_assert(std::is_same_v<decltype(writable_ptr), T*>);
            }

            // insert_at
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;
                snn_require(count_eq(v, 0));
                snn_require_throws_code(v.insert_at(1, x),
                                        generic::error::invalid_position_for_replace_or_insert);
                v.insert_at(0, x);
                snn_require(count_eq(v, 1));
                snn_require(v.at(0).value() == x);
                v.insert_at(0, y);
                snn_require(count_eq(v, 2));
                snn_require(v.at(0).value() == y);
                snn_require(v.at(1).value() == x);
                while (v.count() < v.capacity())
                {
                    v.insert_at(1, x);
                }
                snn_require(v.count() == v.capacity());
                snn_require(v.back(promise::not_empty) == x);
                v.insert_at(v.count(), y);
                snn_require(v.back(promise::not_empty) == y);
            }

            // find
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;
                snn_require(v.find(x).value_or_npos() == constant::npos);
                snn_require(v.find(y).value_or_npos() == constant::npos);

                v.append(x);
                snn_require(v.find(x).value_or_npos() == 0);
                snn_require(v.find(x, 1).value_or_npos() == constant::npos);
                snn_require(v.find(x, 99).value_or_npos() == constant::npos);
                snn_require(v.find(y).value_or_npos() == constant::npos);

                v.append(y);
                snn_require(v.find(x).value_or_npos() == 0);
                snn_require(v.find(x, 1).value_or_npos() == constant::npos);
                snn_require(v.find(y).value_or_npos() == 1);

                v.append(x);
                snn_require(v.find(x).value_or_npos() == 0);
                snn_require(v.find(x, 1).value_or_npos() == 2);
                snn_require(v.find(x, 2).value_or_npos() == 2);
                snn_require(v.find(x, 3).value_or_npos() == constant::npos);
                snn_require(v.find(y).value_or_npos() == 1);
            }

            // find_in_reverse
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;
                snn_require(v.find_in_reverse(x).value_or_npos() == constant::npos);
                snn_require(v.find_in_reverse(y).value_or_npos() == constant::npos);

                v.append(x);
                snn_require(v.find_in_reverse(x).value_or_npos() == 0);
                snn_require(v.find_in_reverse(x, 1).value_or_npos() == 0);
                snn_require(v.find_in_reverse(x, 99).value_or_npos() == 0);
                snn_require(v.find_in_reverse(y).value_or_npos() == constant::npos);

                v.append(y);
                snn_require(v.find_in_reverse(x).value_or_npos() == 0);
                snn_require(v.find_in_reverse(y).value_or_npos() == 1);
                snn_require(v.find_in_reverse(y, 1).value_or_npos() == 1);
                snn_require(v.find_in_reverse(y, 0).value_or_npos() == constant::npos);

                v.append(x);
                snn_require(v.find_in_reverse(x).value_or_npos() == 2);
                snn_require(v.find_in_reverse(x, 2).value_or_npos() == 2);
                snn_require(v.find_in_reverse(x, 1).value_or_npos() == 0);
                snn_require(v.find_in_reverse(y).value_or_npos() == 1);
            }

            // count(needle)
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;
                snn_require(v.count(x) == 0);
                snn_require(v.count(y) == 0);

                v.append(x);
                snn_require(v.count(x) == 1);
                snn_require(v.count(y) == 0);

                v.append(y);
                snn_require(v.count(x) == 1);
                snn_require(v.count(y) == 1);

                v.append(x);
                snn_require(v.count(x) == 2);
                snn_require(v.count(y) == 1);
            }

            // has_front, has_back
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;
                snn_require(!v.has_front(x));
                snn_require(!v.has_front(y));
                snn_require(!v.has_back(x));
                snn_require(!v.has_back(y));

                v.append(x);
                snn_require(v.has_front(x));
                snn_require(!v.has_front(y));
                snn_require(v.has_back(x));
                snn_require(!v.has_back(y));

                v.append(y);
                snn_require(v.has_front(x));
                snn_require(!v.has_front(y));
                snn_require(!v.has_back(x));
                snn_require(v.has_back(y));
            }

            // reserve/reserve_append/capacity
            {
                T x{std::forward<Args>(args)...};

                Vec v;
                snn_require(count_eq(v, 0));
                snn_require(v.capacity() == v.default_capacity());

                v.reserve(0);
                snn_require(v.capacity() == v.default_capacity());

                v.reserve_append(0);
                snn_require(v.capacity() == v.default_capacity());

                v.append(x);
                v.append(x);
                snn_require(count_eq(v, 2));
                snn_require(v.capacity() >= 2);

                v.reserve_append(10);
                snn_require(v.capacity() >= 12);
                snn_require(count_eq(v, 2));

                snn_require(v.capacity() < 100);
                v.reserve(100);
                snn_require(v.capacity() >= 100);
                snn_require(count_eq(v, 2));

                usize count = 0;
                for (const auto& y : v)
                {
                    snn_require(y == x);
                    ++count;
                }
                snn_require(count == 2);

                const usize cap = v.capacity();
                v.reserve(0);
                snn_require(v.capacity() == cap);
                v.reserve(50);
                snn_require(v.capacity() == cap);
                v.reserve(cap);
                snn_require(v.capacity() == cap);
                v.reserve_append(0);
                snn_require(v.capacity() == cap);
                v.reserve_append(50);
                snn_require(v.capacity() == cap);
                v.reserve_append(cap - 2);
                snn_require(v.capacity() == cap);

                snn_require_throws_code(v.reserve(constant::limit<usize>::max),
                                        generic::error::capacity_would_exceed_max_capacity);

                snn_require_throws_code(v.reserve_append(constant::limit<usize>::max),
                                        generic::error::capacity_would_exceed_max_capacity);
            }

            // count
            // Checked throughout.

            // swap
            if constexpr (Vec::default_capacity() == 0)
            {
                Vec v1;
                v1.append_inplace(std::forward<Args>(args)...);
                v1.append_inplace(std::forward<Args>(args)...);

                Vec v2;

                snn_require(count_eq(v1, 2));
                snn_require(count_eq(v2, 0));

                swap(v1, v2);

                snn_require(count_eq(v2, 2));
                snn_require(count_eq(v1, 0));

                v1.swap(v2);

                snn_require(count_eq(v1, 2));
                snn_require(count_eq(v2, 0));

                v1.swap(v1); // Swap with self.
                snn_require(count_eq(v1, 2));

                std::swap(v1, v1); // Swap with self.
                snn_require(count_eq(v1, 2));
            }

            // truncate
            {
                const T x{std::forward<Args>(args)...};
                const T y{};
                snn_require(x != y);

                Vec v;

                snn_require(count_eq(v, 0));
                v.truncate(99);
                snn_require(count_eq(v, 0));
                v.truncate(0);
                snn_require(count_eq(v, 0));

                v.append(x);
                v.append(y);
                snn_require(count_eq(v, 2));

                v.truncate(99);
                snn_require(count_eq(v, 2));
                v.truncate(2);
                snn_require(count_eq(v, 2));
                v.truncate(1);
                snn_require(count_eq(v, 1));
                v.truncate(0);
                snn_require(count_eq(v, 0));
                v.truncate(99);
                snn_require(count_eq(v, 0));
            }

            // operator==/operator!=
            {
                Vec v1;
                Vec v2;
                snn_require(v1 == v1);
                snn_require(v1 == v2);

                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(v1 != v2);

                v2.append_inplace(std::forward<Args>(args)...);
                snn_require(v1 == v1);
                snn_require(v1 == v2);

                v2.append_inplace(std::forward<Args>(args)...);
                snn_require(v1 != v2);

                v1.append_inplace(std::forward<Args>(args)...);
                snn_require(v1 == v1);
                snn_require(v1 == v2);

                v1.clear();
                snn_require(v1 != v2);

                v2.clear();
                snn_require(v1 == v2);

                v1.append_inplace(std::forward<Args>(args)...);
                v1.append_inplace(std::forward<Args>(args)...);

                T x{};
                v2.append(x);
                v2.append(x);

                snn_require(count_eq(v1, 2));
                snn_require(v1.count() == v2.count());
                snn_require(v1 != v2);
            }

            // operator<<(T)
            {
                T x{std::forward<Args>(args)...};

                Vec v;
                v << x;
                v << x << x << x;
                snn_require(count_eq(v, 4));

                usize count = 0;
                for (const auto& y : v)
                {
                    snn_require(y == x);
                    ++count;
                }
                snn_require(count == 4);
            }

            // operator<<(vec)
            {
                T x{std::forward<Args>(args)...};

                Vec v1;
                v1.append(x);
                v1.append(x);
                v1.append(x);
                v1.append(x);
                snn_require(count_eq(v1, 4));

                Vec v2;
                v2 << v1;
                v2 << v1 << v1;

                usize count = 0;
                for (const auto& y : v2)
                {
                    snn_require(y == x);
                    ++count;
                }
                snn_require(count == 12);
            }

            // drop_back
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                Vec v;

                v.append(x);
                v.append(y);
                snn_require(count_eq(v, 2));

                v.drop_back(promise::not_empty);
                v.drop_back(promise::not_empty);
                snn_require(count_eq(v, 0));

                v.append(y);
                v.append(x);
                snn_require(count_eq(v, 2));

                v.drop_back(promise::not_empty);
                v.drop_back(promise::not_empty);
                snn_require(count_eq(v, 0));
            }

            // drop_back_n
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                Vec v;

                v.drop_back_n(0);
                snn_require(count_eq(v, 0));

                v.drop_back_n(99);
                snn_require(count_eq(v, 0));

                v.append(y);
                v.append(x);
                snn_require(count_eq(v, 2));

                v.drop_back_n(2);
                snn_require(count_eq(v, 0));

                v.append(x);
                v.append(y);
                snn_require(count_eq(v, 2));

                v.drop_back_n(1);
                snn_require(count_eq(v, 1));

                snn_require(v.at(0).value() == x);

                v.drop_back_n(99);
                snn_require(count_eq(v, 0));
            }

            // drop_at
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                Vec v;

                v.drop_at(0, 1);
                v.drop_at(0, 10);
                v.drop_at(1, 1);
                v.drop_at(1, 10);

                snn_require(count_eq(v, 0));

                v.append(y);
                v.append(y);
                v.append(y);
                v.append(y);
                v.append(x);
                v.append(x);
                v.append(x);
                v.append(x);
                snn_require(count_eq(v, 8));

                v.drop_at(1, 1);
                snn_require(count_eq(v, 7));

                snn_require(v.at(0).value() == y);
                snn_require(v.at(1).value() == y);
                snn_require(v.at(2).value() == y);
                snn_require(v.at(3).value() == x);
                snn_require(v.at(4).value() == x);
                snn_require(v.at(5).value() == x);
                snn_require(v.at(6).value() == x);

                v.drop_at(2, 2);
                snn_require(count_eq(v, 5));

                snn_require(v.at(0).value() == y);
                snn_require(v.at(1).value() == y);
                snn_require(v.at(2).value() == x);
                snn_require(v.at(3).value() == x);
                snn_require(v.at(4).value() == x);

                v.drop_at(0, 1);
                snn_require(count_eq(v, 4));

                snn_require(v.at(0).value() == y);
                snn_require(v.at(1).value() == x);
                snn_require(v.at(2).value() == x);
                snn_require(v.at(3).value() == x);

                v.drop_at(4, 1);
                snn_require(count_eq(v, 4));

                snn_require(v.at(0).value() == y);
                snn_require(v.at(1).value() == x);
                snn_require(v.at(2).value() == x);
                snn_require(v.at(3).value() == x);

                v.drop_at(3, 99);
                snn_require(count_eq(v, 3));

                snn_require(v.at(0).value() == y);
                snn_require(v.at(1).value() == x);
                snn_require(v.at(2).value() == x);

                while (v.count() > 0)
                {
                    v.drop_at(0, 1);
                }
                snn_require(count_eq(v, 0));
            }
            {
                const T x{std::forward<Args>(args)...};
                const T y{};

                Vec v;

                v.append(x);
                v.append(x);
                v.append(x);
                v.append(x);
                v.append(y);

                // Remove elements from the middle, if the wrong elements are removed here
                // AddressSanitizer should catch it on destruction of `v`.
                v.drop_at(1, 2);
                snn_require(count_eq(v, 3));
            }

            // Reallocations
            {
                Vec v;

                usize reallocations = 0;
                usize cap           = v.capacity();
                for (loop::count lc{1000}; lc--;)
                {
                    v.append_inplace(std::forward<Args>(args)...);
                    if (cap != v.capacity())
                    {
                        cap = v.capacity();
                        ++reallocations;
                    }
                }
                snn_require(count_eq(v, 1000));
                snn_require(reallocations <= 15);
            }

            return true;
        }

        template <usize SmallCapacity>
        constexpr bool test_vec2()
        {
            // size
            {
                vec<char, SmallCapacity> v;
                snn_require(v.size() == 0);
                v.append_inplace('a');
                snn_require(v.size() == 1);
                v.append_inplace('b');
                snn_require(v.size() == 2);
            }

            // Initializer list constructor.
            {
                vec<str, SmallCapacity> v{"One"};
                snn_require(count_eq(v, 1));
                snn_require(v.at(0).value() == "One");
            }
            {
                vec<str, SmallCapacity> v{"One", "Two"};
                snn_require(count_eq(v, 2));
                snn_require(v.at(0).value() == "One");
                snn_require(v.at(1).value() == "Two");
            }
            {
                vec<str, SmallCapacity> v{"One", "Two", "Three"};
                snn_require(count_eq(v, 3));
                snn_require(v.at(0).value() == "One");
                snn_require(v.at(1).value() == "Two");
                snn_require(v.at(2).value() == "Three");
            }

            // Append self.
            {
                vec<str, SmallCapacity> v{"One", "Two", "A long string, which goes on the heap."};
                snn_require(count_eq(v, 3));
                snn_require(v.at(0).value() == "One");
                snn_require(v.at(1).value() == "Two");
                snn_require(v.at(2).value() == "A long string, which goes on the heap.");
                v.append(v);
                snn_require(count_eq(v, 6));
                snn_require(v.at(0).value() == "One");
                snn_require(v.at(1).value() == "Two");
                snn_require(v.at(2).value() == "A long string, which goes on the heap.");
                snn_require(v.at(3).value() == "One");
                snn_require(v.at(4).value() == "Two");
                snn_require(v.at(5).value() == "A long string, which goes on the heap.");
            }

            // Check raw memory.
            {
                vec<trivial, SmallCapacity> v;
                str s;

                static_assert(sizeof(trivial) == 4);

                snn_require(v.capacity() == v.default_capacity());
                for (loop::count lc{10}; lc--;)
                {
                    v.append_inplace('a', 'b', 'c', 'd');
                    s << "abcd";
                }
                snn_require(count_eq(v, 10));
                snn_require(s.size() == 40);

                if (!std::is_constant_evaluated())
                {
                    snn_require(mem::raw::is_equal(not_null{v.begin()[0].data()}, s.data(),
                                                   byte_size{40}));
                }
            }

            // append_inplace() with exceptions.
            {
                vec<trivial, SmallCapacity> v;
                snn_require(count_eq(v, 0));
                snn_require(v.capacity() == v.default_capacity());

                snn_require_throws_code(v.append_inplace("ZZZZ"), generic::error::invalid_value);
                snn_require(count_eq(v, 0));
                snn_require(v.capacity() == v.default_capacity());

                v.append_inplace("abcd");
                snn_require(count_eq(v, 1));
                snn_require(v.count() <= v.capacity());

                while (v.count() < v.capacity())
                {
                    v.append_inplace("abcd");
                }
                snn_require(v.count() == v.capacity());

                const auto count = v.count();

                snn_require_throws_code(v.append_inplace("ZZZZ"), generic::error::invalid_value);
                snn_require(v.count() == count);
                snn_require(v.count() == v.capacity());

                usize counter = 0;
                for (const auto& t : v)
                {
                    snn_require(t == "abcd");
                    ++counter;
                }
                snn_require(counter == count);

                v.append_inplace("abcd");

                counter = 0;
                for (const auto& t : v)
                {
                    snn_require(t == "abcd");
                    ++counter;
                }
                snn_require(counter == (count + 1));
            }

            // insert_at
            {
                vec<str, SmallCapacity> v{"abcdefghijklmnopqrstuvwxyz",
                                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

                v.insert_at(0, "Another long string, which also goes on the heap.");
                snn_require(count_eq(v, 3));
                snn_require(v.at(0).value() == "Another long string, which also goes on the heap.");
                snn_require(v.at(1).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(2).value() == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

                v.insert_at(1, "123");
                snn_require(count_eq(v, 4));
                snn_require(v.at(0).value() == "Another long string, which also goes on the heap.");
                snn_require(v.at(1).value() == "123");
                snn_require(v.at(2).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(3).value() == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

                v.insert_at(3, "456");
                snn_require(count_eq(v, 5));
                snn_require(v.at(0).value() == "Another long string, which also goes on the heap.");
                snn_require(v.at(1).value() == "123");
                snn_require(v.at(2).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(3).value() == "456");
                snn_require(v.at(4).value() == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

                v.insert_at(5, "789");
                snn_require(count_eq(v, 6));
                snn_require(v.at(0).value() == "Another long string, which also goes on the heap.");
                snn_require(v.at(1).value() == "123");
                snn_require(v.at(2).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(3).value() == "456");
                snn_require(v.at(4).value() == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                snn_require(v.at(5).value() == "789");
            }

            // range
            {
                vec<int, SmallCapacity> v{9, 3, 7, 2};
                auto rng = v.range();
                snn_require(rng.count() == 4);
                snn_require(rng.begin() == v.begin());
                snn_require(rng.end() == v.end());
            }
            {
                const vec<int, SmallCapacity> v{9, 3, 7, 2};
                const auto rng = v.range();
                snn_require(rng.count() == 4);
                snn_require(rng.begin() == v.begin());
                snn_require(rng.end() == v.end());
            }

            // view
            {
                vec<str, SmallCapacity> v{"abc", "ABC", "abc", "123"};

                v.replace("abc", "abcdefghijklmnopqrstuvwxyz");

                snn_require(count_eq(v, 4));
                snn_require(v.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(1).value() == "ABC");
                snn_require(v.at(2).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(3).value() == "123");

                {
                    auto s = v.view(1, 2);
                    snn_require(s.count() == 2);
                    snn_require(s.at(0).value() == "ABC");
                    snn_require(s.at(1).value() == "abcdefghijklmnopqrstuvwxyz");
                }

                {
                    auto s = v.view(3);
                    snn_require(s.count() == 1);
                    snn_require(s.at(0).value() == "123");
                }
            }
            {
                const vec<int, SmallCapacity> v{9, 3, 7, 2};
                snn_require(count_eq(v, 4));

                {
                    auto s = v.view(1, 2);
                    snn_require(s.count() == 2);
                    snn_require(s.at(0).value() == 3);
                    snn_require(s.at(1).value() == 7);
                }

                {
                    auto s = v.view(3);
                    snn_require(s.count() == 1);
                    snn_require(s.at(0).value() == 2);
                }
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

        static_assert(!is_trivially_relocatable_v<std::string>);

        static_assert(is_trivially_relocatable_v<vec<int>>);
        static_assert(is_trivially_relocatable_v<vec<str>>);
        static_assert(is_trivially_relocatable_v<vec<std::string>>);

        static_assert(!is_trivially_relocatable_v<vec<int, 1>>);
        static_assert(!is_trivially_relocatable_v<vec<int, 2>>);
        static_assert(!is_trivially_relocatable_v<vec<str, 1>>);
        static_assert(!is_trivially_relocatable_v<vec<str, 2>>);
        static_assert(!is_trivially_relocatable_v<vec<std::string, 1>>);
        static_assert(!is_trivially_relocatable_v<vec<std::string, 2>>);

        static_assert(sizeof(vec<int>) == 24);
        static_assert(sizeof(vec<char>) == 24);
        static_assert(sizeof(vec<str>) == 24);
        static_assert(sizeof(vec<vec<str>>) == 24);

        static_assert(sizeof(vec<int, 1>) == 32);
        static_assert(sizeof(vec<int, 2>) == 32);
        static_assert(sizeof(vec<int, 3>) == 40);
        static_assert(sizeof(vec<char, 1>) == 32);
        static_assert(sizeof(vec<char, 8>) == 32);
        static_assert(sizeof(vec<char, 9>) == 40);
        static_assert(sizeof(vec<str, 1>) == 48);
        static_assert(sizeof(vec<str, 2>) == 72);

        snn_static_require(app::test_vec1<vec<u16>>(constant::limit<u16>::max));
        snn_static_require(app::test_vec1<vec<u8>>(constant::limit<u8>::max));
        snn_static_require(app::test_vec1<vec<app::trivial>>('a', 'b', 'c', 'd'));
        snn_static_require(app::test_vec1<vec<str>>("A long string, which goes on the heap."));
        snn_static_require(app::test_vec1<vec<str>>("A short string."));
        snn_static_require(app::test_vec2<0>());

        // `std::string` (not always constexpr).

        snn_require(app::test_vec1<vec<std::string>>("A long string, which goes on the heap."));
        snn_require(app::test_vec1<vec<std::string>>("A short string."));

        // Small capacity (not constexpr).

        snn_require(app::test_vec1<vec<str, 1>>("A long string, which goes on the heap."));
        snn_require(app::test_vec1<vec<str, 1>>("A short string."));

        snn_require(app::test_vec1<vec<str, 2>>("A long string, which goes on the heap."));
        snn_require(app::test_vec1<vec<str, 2>>("A short string."));

        snn_require(app::test_vec2<1>());
        snn_require(app::test_vec2<2>());
    }
}
