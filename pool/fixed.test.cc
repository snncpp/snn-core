// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/pool/fixed.hh"

#include "snn-core/unittest.hh"
#include "snn-core/mem/raw/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            pool::fixed<int> pool{not_zero<usize>{6}};

            snn_require(!pool);
            snn_require(pool.is_empty());
            snn_require(!pool.is_full());
            snn_require(pool.count() == 0);
            snn_require(pool.capacity() == 6);
            snn_require(pool.remaining_capacity() == 6);

            pool.append(112233);
            pool.append(43529);
            pool.append_inplace(23);
            pool.append_inplace(123);

            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());
            snn_require(pool.count() == 4);
            snn_require(pool.capacity() == 6);
            snn_require(pool.remaining_capacity() == 2);

            pool.append(73622);
            pool.append_inplace(123456789);

            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(pool.is_full());
            snn_require(pool.count() == 6);
            snn_require(pool.capacity() == 6);
            snn_require(pool.remaining_capacity() == 0);

            // Pool is full, appending will throw.
            snn_require_throws_code(pool.append(7), generic::error::insufficient_capacity);
            snn_require_throws_code(pool.append_inplace(7), generic::error::insufficient_capacity);

            snn_require(pool.front(assume::not_empty) == 112233);
            snn_require(pool.at(2, assume::within_bounds) == 23);
            snn_require(pool.back(assume::not_empty) == 123456789);

            pool.drop_back(assume::not_empty);

            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());
            snn_require(pool.count() == 5);
            snn_require(pool.capacity() == 6);
            snn_require(pool.remaining_capacity() == 1);

            return true;
        }

        class trivial final
        {
          public:
            constexpr trivial(const char (&s)[5]) noexcept
                : s_{s[0], s[1], s[2], s[3]}
            {
            }

            constexpr char first() const noexcept
            {
                return s_[0];
            }

          private:
            char s_[4];
        };

        constexpr bool test_fixed()
        {
            static_assert(sizeof(trivial) == 4);

            pool::fixed<trivial> pool{not_zero<usize>{6}};

            snn_require(pool.count() == 0);
            snn_require(pool.capacity() == 6);
            snn_require(!pool);
            snn_require(pool.is_empty());
            snn_require(!pool.is_full());

            {
                usize c = 0;
                for (auto& t : pool)
                {
                    ++c;
                    snn_require(t.first() >= 'a' && t.first() <= 'u');
                }
                snn_require(c == 0);
            }

            auto& t1 = pool.append_inplace("abcd");
            snn_require(t1.first() == 'a');
            snn_require(pool.back(assume::not_empty).first() == 'a');
            snn_require(pool.front(assume::not_empty).first() == 'a');
            snn_require(pool.count() == 1);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());
            snn_require(&pool.at(0, assume::within_bounds) == &t1);

            auto& t2 = pool.append(trivial{"efgh"});
            snn_require(t2.first() == 'e');
            snn_require(&pool.back(assume::not_empty) == &t2);
            snn_require(&pool.front(assume::not_empty) == &t1);
            snn_require(pool.count() == 2);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());
            snn_require(&pool.at(1, assume::within_bounds) == &t2);

            auto& t3 = pool.append(trivial{"ijkl"});
            snn_require(t3.first() == 'i');
            snn_require(pool.count() == 3);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());

            auto& t4 = pool.append_inplace("mnop");
            snn_require(t4.first() == 'm');
            snn_require(pool.count() == 4);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());

            auto& t5 = pool.append_inplace("qrst");
            snn_require(t5.first() == 'q');
            snn_require(pool.count() == 5);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());

            auto& t6 = pool.append_inplace("uvwx");
            snn_require(t6.first() == 'u');
            snn_require(pool.count() == 6);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(pool.is_full());
            snn_require(&pool.at(5, assume::within_bounds) == &t6);

            {
                usize c = 0;
                for (auto& t : pool)
                {
                    ++c;
                    snn_require(t.first() >= 'a' && t.first() <= 'u');
                }
                snn_require(c == 6);
            }

            pool.drop_back(assume::not_empty);

            snn_require(&pool.back(assume::not_empty) == &t5);
            snn_require(pool.back(assume::not_empty).first() == 'q');
            snn_require(pool.count() == 5);
            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(!pool.is_full());

            return true;
        }

        constexpr bool test_fixed_extended()
        {
            {
                static_assert(sizeof(trivial) == 4);

                pool::fixed<trivial> pool{not_zero<usize>{6}};

                snn_require(pool.count() == 0);
                snn_require(pool.capacity() == 6);
                snn_require(!pool);
                snn_require(pool.is_empty());
                snn_require(!pool.is_full());

                {
                    usize c = 0;
                    for (auto& t : pool)
                    {
                        ++c;
                        snn_require(t.first() >= 'a' && t.first() <= 'u');
                    }
                    snn_require(c == 0);
                }

                auto& t1 = pool.append_inplace("abcd");
                snn_require(t1.first() == 'a');
                snn_require(pool.back(assume::not_empty).first() == 'a');
                snn_require(pool.front(assume::not_empty).first() == 'a');
                snn_require(pool.count() == 1);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());
                snn_require(&pool.at(0, assume::within_bounds) == &t1);

                auto& t2 = pool.append(trivial{"efgh"});
                snn_require(t2.first() == 'e');
                snn_require(&pool.back(assume::not_empty) == &t2);
                snn_require(&pool.front(assume::not_empty) == &t1);
                snn_require(pool.count() == 2);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());
                snn_require(&pool.at(1, assume::within_bounds) == &t2);

                auto& t3 = pool.append(trivial{"ijkl"});
                snn_require(t3.first() == 'i');
                snn_require(pool.count() == 3);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());

                auto& t4 = pool.append_inplace("mnop");
                snn_require(t4.first() == 'm');
                snn_require(pool.count() == 4);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());

                auto& t5 = pool.append_inplace("qrst");
                snn_require(t5.first() == 'q');
                snn_require(pool.count() == 5);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());

                auto& t6 = pool.append_inplace("uvwx");
                snn_require(t6.first() == 'u');
                snn_require(pool.count() == 6);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(pool.is_full());
                snn_require(&pool.at(5, assume::within_bounds) == &t6);

                snn_require_throws_code(pool.append(trivial{"uvwx"}),
                                        generic::error::insufficient_capacity);
                snn_require_throws_code(pool.append_inplace("uvwx"),
                                        generic::error::insufficient_capacity);

                snn_should(pool.cbegin() != nullptr);

                if (!std::is_constant_evaluated())
                {
                    snn_should(mem::raw::is_equal(not_null{pool.cbegin()},
                                                  not_null{"abcdefghijklmnopqrstuvwx"},
                                                  byte_size{24}));
                }

                {
                    usize c = 0;
                    for (auto& t : pool)
                    {
                        ++c;
                        snn_require(t.first() >= 'a' && t.first() <= 'u');
                    }
                    snn_require(c == 6);
                }

                pool.drop_back(assume::not_empty);

                snn_require(&pool.back(assume::not_empty) == &t5);
                snn_require(pool.back(assume::not_empty).first() == 'q');
                snn_require(pool.count() == 5);
                snn_require(pool);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());
            }

            {
                for (const int n : {0, 1, 2, 3, 4})
                {
                    pool::fixed<str> pool{not_zero<usize>{5}};
                    snn_require(pool.capacity() == 5);
                    snn_require(!pool);
                    snn_require(pool.is_empty());
                    snn_require(!pool.is_full());
                    for (int i = 0; i <= n; ++i)
                    {
                        auto& s = pool.append_inplace("A long string, which goes on the heap.");
                        snn_require(s.size() > str::default_capacity());
                        snn_require(pool.count() == to_usize(i + 1));
                        snn_require(pool);
                        snn_require(!pool.is_empty());
                    }
                }
            }

            {
                pool::fixed<str> pool{not_zero<usize>{2}};
                snn_require(pool.capacity() == 2);
                snn_require(pool.is_empty());

                pool.append_inplace("A long string, which goes on the heap.");
                snn_require(pool.count() == 1);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());

                pool.append_inplace("A long string, which goes on the heap.");
                snn_require(pool.count() == 2);
                snn_require(!pool.is_empty());
                snn_require(pool.is_full());

                pool.drop_back(assume::not_empty);
                snn_require(pool.count() == 1);
                snn_require(!pool.is_empty());
                snn_require(!pool.is_full());

                pool.drop_back(assume::not_empty);
                snn_require(pool.count() == 0);
                snn_require(pool.is_empty());
                snn_require(!pool.is_full());
            }

            // Move constructor.
            {
                pool::fixed<str> p1{not_zero<usize>{1}};
                snn_require(p1.capacity() == 1);
                snn_require(p1.count() == 0);
                snn_require(p1.is_empty());
                snn_require(!p1.is_full());
                p1.append_inplace("A long string, which goes on the heap.");
                snn_require(p1.count() == 1);
                snn_require(!p1.is_empty());
                snn_require(p1.is_full());

                pool::fixed<str> p2{std::move(p1)};

                snn_require(p1.capacity() == 0);
                snn_require(p1.count() == 0);
                snn_require(p1.is_empty());
                snn_require(p1.is_full());

                snn_require(p2.capacity() == 1);
                snn_require(p2.count() == 1);
                snn_require(!p2.is_empty());
                snn_require(p2.is_full());

                snn_should(p1.cbegin() == nullptr);

                usize c = 0;
                for (const auto& elem : p2)
                {
                    ++c;
                    snn_require(elem == "A long string, which goes on the heap.");
                }
                snn_require(c == 1);
            }

            // Move assignment operator.
            {
                pool::fixed<str> p1{not_zero<usize>{2}};
                p1.append_inplace("A long string, which goes on the heap.");
                p1.append(str{"A long string, which goes on the heap."});

                pool::fixed<str> p2{not_zero<usize>{1}};
                p2.append_inplace("Another long string, which goes on the heap.");

                snn_require(p1.count() == 2);
                snn_require(p2.count() == 1);
                snn_require(p1.capacity() == 2);
                snn_require(p2.capacity() == 1);
                snn_require(p1.is_full());
                snn_require(p2.is_full());

                p2 = std::move(p1);

                snn_require(p1.count() == 1);
                snn_require(p2.count() == 2);
                snn_require(p1.capacity() == 1);
                snn_require(p2.capacity() == 2);
                snn_require(p1.is_full());
                snn_require(p2.is_full());

                usize count = 0;

                count = 0;
                for (const auto& elem : p1)
                {
                    ++count;
                    snn_require(elem == "Another long string, which goes on the heap.");
                }
                snn_require(count == 1);

                count = 0;
                for (const auto& elem : p2)
                {
                    ++count;
                    snn_require(elem == "A long string, which goes on the heap.");
                }
                snn_require(count == 2);
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
        snn_static_require(app::test_fixed());
        snn_static_require(app::test_fixed_extended());
    }
}
