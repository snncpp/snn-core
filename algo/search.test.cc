// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/search.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/step.hh"
#include "snn-core/range/view/take.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            auto rng = algo::search(cstrrng{"abcdef"}, range::step{'a', 'd'});
            snn_require(algo::is_equal(rng, {'a', 'b', 'c', 'd', 'e', 'f'}));

            rng = algo::search(cstrrng{"abcdef"}, range::step{'c', 'e'});
            snn_require(algo::is_equal(rng, {'c', 'd', 'e', 'f'}));

            rng = algo::search(cstrrng{"abcdef"}, range::step{'c', 'z'});
            snn_require(rng.is_empty());

            static_assert(std::is_same_v<decltype(rng), cstrrng>);

            return true;
        }

        constexpr bool test_search()
        {
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{""}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"a"}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"ab"}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"abc"}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"abcd"}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"abcde"}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"abcdef"}).view() == "abcdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"abcdefg"}).view() == "");

            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"c"}).view() == "cdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"cd"}).view() == "cdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"cde"}).view() == "cdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"cdef"}).view() == "cdef");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"cdefg"}).view() == "");

            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"f"}).view() == "f");
            snn_require(algo::search(cstrrng{"abcdef"}, cstrrng{"fg"}).view() == "");

            snn_require(algo::search(cstrrng{"aaabc"}, cstrrng{"a"}).view() == "aaabc");
            snn_require(algo::search(cstrrng{"aaabc"}, cstrrng{"ab"}).view() == "abc");

            snn_require(algo::search(cstrrng{"aaabc"}, cstrrng{"AB"}).view() == "");
            snn_require(
                algo::search(cstrrng{"aaabc"}, cstrrng{"AB"}, ascii::fn::equal_to_icase{}).view() ==
                "abc");

            snn_require(algo::is_equal(algo::search(range::step{'a', 'g'}, cstrrng{""}),
                                       {'a', 'b', 'c', 'd', 'e', 'f'}));
            snn_require(algo::is_equal(algo::search(range::step{'a', 'g'}, cstrrng{"c"}),
                                       {'c', 'd', 'e', 'f'}));
            snn_require(algo::is_equal(algo::search(range::step{'a', 'g'}, cstrrng{"cd"}),
                                       {'c', 'd', 'e', 'f'}));
            snn_require(algo::is_equal(algo::search(range::step{'a', 'g'}, cstrrng{"cdef"}),
                                       {'c', 'd', 'e', 'f'}));
            snn_require(algo::search(range::step{'a', 'g'}, cstrrng{"cdefg"}).is_empty());

            snn_require(algo::is_equal(algo::search(cstrrng{"aaabcdef"} | range::v::take{5},
                                                    cstrrng{"a"}),
                                       {'a', 'a', 'a', 'b', 'c'}));
            snn_require(algo::is_equal(algo::search(cstrrng{"aaabcdef"} | range::v::take{5},
                                                    cstrrng{"ab"}),
                                       {'a', 'b', 'c'}));

            snn_require(
                algo::search(cstrrng{"aaabcdef"} | range::v::take{5}, cstrrng{"AB"}).is_empty());
            snn_require(algo::is_equal(algo::search(cstrrng{"aaabcdef"} | range::v::take{5},
                                                    cstrrng{"AB"}, ascii::fn::equal_to_icase{}),
                                       {'a', 'b', 'c'}));

            {
                cstrview subject{"abcdefghijklmnopqrstuvwxyz"};
                array<int, 3> needle{'r', 's', 't'};
                static_assert(std::is_same_v<decltype(needle.range()), range::contiguous<int*>>);

                auto rng = algo::search(subject.range(), needle.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<const char*>>);

                snn_require(rng.view() == "rstuvwxyz");
                snn_require(rng.begin() == subject.view(17).begin());
                snn_require(rng.end() == subject.end());
            }

            {
                auto rng = algo::search(range::step{'a', 'g'}, cstrrng{"cd"});
                static_assert(std::is_same_v<decltype(rng), range::step<char, char>>);
                snn_require(rng);
                snn_require(algo::is_equal(rng, {'c', 'd', 'e', 'f'}));
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
        snn_static_require(app::test_search());
    }
}
