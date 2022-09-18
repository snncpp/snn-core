// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/pool/append_only.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include <stdexcept> // invalid_argument

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            const usize min_elements_per_block = 3;
            pool::append_only<int> pool{min_elements_per_block};

            snn_require(!pool);
            snn_require(pool.is_empty());
            snn_require(pool.count() == 0);

            // The actual elements-per-block might be greater than requested (based on optimal
            // memory allocation size).
            snn_require(pool.elements_per_block() == 4);

            pool.append_inplace(112233);
            pool.append_inplace(43529);
            pool.append_inplace(23);
            pool.append_inplace(123);
            pool.append_inplace(73622);
            pool.append_inplace(829222);

            decltype(auto) i = pool.append_inplace(123456789);
            static_assert(std::is_same_v<decltype(i), int&>);
            snn_require(i == 123456789);

            snn_require(pool);
            snn_require(!pool.is_empty());
            snn_require(pool.count() == 7);

            snn_require(pool.back(promise::not_empty) == 123456789);

            return true;
        }

        class trivial final
        {
          public:
            constexpr trivial(const char (&s)[9])
                : s_{s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]}
            {
                if (s_.get<0>() == 'Z')
                {
                    throw std::invalid_argument{"Invalid."};
                }
            }

            template <usize N>
            constexpr bool operator==(const char (&s)[N]) const noexcept
            {
                return s_.view() == s;
            }

          private:
            array<char, 8> s_;
        };

        constexpr bool test_append_only()
        {
            pool::append_only<trivial> pool{3};
            snn_require(pool.is_empty());
            snn_require(!pool);
            snn_require(pool.count() == 0);
            snn_require(pool.elements_per_block() == 4); // Based on optimal memory block.

            snn_should(pool.blocks().is_empty());

            if (!std::is_constant_evaluated())
            {
                snn_require_throws_type(pool.append_inplace("ZZZZZZZZ"), std::invalid_argument);
                snn_should(pool.blocks().count() == 1); // Block allocated, but not used.
            }

            snn_require(pool.is_empty());
            snn_require(!pool);
            snn_require(pool.count() == 0);

            auto& t0 = pool.append_inplace("trivial0");
            snn_require(t0 == "trivial0");
            snn_require(!(t0 == "trivial1"));
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.count() == 1);
            snn_require(&pool.back(promise::not_empty) == &t0);
            snn_require(pool.back(promise::not_empty) == "trivial0");

            auto& t1 = pool.append_inplace("trivial1");
            snn_require(t1 == "trivial1");
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.count() == 2);
            snn_require(&pool.back(promise::not_empty) != &t0);
            snn_require(&pool.back(promise::not_empty) == &t1);

            auto& t2 = pool.append_inplace("trivial2");
            snn_require(t2 == "trivial2");
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.count() == 3);
            snn_require(&pool.back(promise::not_empty) == &t2);

            auto& t3 = pool.append_inplace("trivial3");
            snn_require(t3 == "trivial3");
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.count() == 4);
            snn_require(&pool.back(promise::not_empty) == &t3);

            snn_should(pool.blocks().count() == 1);

            if (!std::is_constant_evaluated())
            {
                snn_require_throws_type(pool.append_inplace("ZZZZZZZZ"), std::invalid_argument);
                snn_should(pool.blocks().count() == 2); // A second block allocated, but not used.
            }

            snn_require(&pool.back(promise::not_empty) == &t3);
            snn_require(pool.back(promise::not_empty) == "trivial3");

            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.count() == 4);

            auto& t4 = pool.append_inplace("trivial4");
            snn_require(t4 == "trivial4");
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.count() == 5);
            snn_require(&pool.back(promise::not_empty) == &t4);
            snn_require(pool.back(promise::not_empty) == "trivial4");

            snn_should(pool.blocks().count() == 2);

            pool::append_only<trivial> pool2{std::move(pool)};
            snn_require(pool.is_empty());
            snn_require(!pool);
            snn_require(!pool2.is_empty());
            snn_require(pool2);
            snn_require(pool2.back(promise::not_empty) == "trivial4");

            pool2.swap(pool);
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.back(promise::not_empty) == "trivial4");
            snn_require(pool2.is_empty());
            snn_require(!pool2);

            pool.swap(pool);
            snn_require(!pool.is_empty());
            snn_require(pool);
            snn_require(pool.back(promise::not_empty) == "trivial4");

            return true;
        }

        constexpr bool test_str(const usize min_elements_per_block, const cstrview string)
        {
            for (const int n : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9})
            {
                pool::append_only<str> pool{min_elements_per_block};
                snn_require(pool.is_empty());
                snn_require(!pool);
                snn_require(pool.count() == 0);
                snn_require(pool.elements_per_block() >= min_elements_per_block);
                for (int i = 0; i < n; ++i)
                {
                    auto& s = pool.append_inplace(string);
                    snn_require(s == string);
                    snn_require(!pool.is_empty());
                    snn_require(pool);
                    snn_require(&pool.back(promise::not_empty) == &s);
                    snn_require(pool.count() == static_cast<usize>(i + 1));
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
        snn_static_require(app::test_append_only());

        snn_static_require(app::test_str(1, "A short string."));
        snn_static_require(app::test_str(2, "A short string."));
        snn_static_require(app::test_str(3, "A short string."));
        snn_static_require(app::test_str(4, "A short string."));
        snn_static_require(app::test_str(5, "A short string."));

        snn_static_require(app::test_str(1, "A long string, which goes on the heap."));
        snn_static_require(app::test_str(2, "A long string, which goes on the heap."));
        snn_static_require(app::test_str(3, "A long string, which goes on the heap."));
        snn_static_require(app::test_str(4, "A long string, which goes on the heap."));
        snn_static_require(app::test_str(5, "A long string, which goes on the heap."));
    }
}
