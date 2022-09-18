// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/regex/range/match.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const regex::pattern pat{"([a-z]+)([0-9]+)"};
            regex::range::match rng{"This one1. And another three33.", pat};

            // Concepts
            static_assert(input_range<decltype(rng)>);
            static_assert(!forward_range<decltype(rng)>);

            snn_require(rng);
            snn_require(!rng.is_empty());

            usize count = 0;
            for (const auto matches : rng)
            {
                if (count == 0)
                {
                    snn_require(matches);
                    snn_require(matches.count() == 3);

                    const auto match0 = matches.at(0).value();
                    snn_require(match0.position() == 5);
                    snn_require(match0.count() == 4);
                    snn_require(match0.size() == 4);
                    snn_require(match0.view() == "one1");

                    const auto match1 = matches.at(1).value();
                    snn_require(match1.position() == 5);
                    snn_require(match1.count() == 3);
                    snn_require(match1.size() == 3);
                    snn_require(match1.view() == "one");

                    const auto match2 = matches.at(2).value();
                    snn_require(match2.position() == 8);
                    snn_require(match2.count() == 1);
                    snn_require(match2.size() == 1);
                    snn_require(match2.view() == "1");

                    snn_require(!matches.at(3).has_value());
                }
                else if (count == 1)
                {
                    snn_require(matches);
                    snn_require(matches.count() == 3);

                    usize inner_count = 0;
                    for (const auto match : matches)
                    {
                        if (inner_count == 0)
                        {
                            snn_require(match.position() == 23);
                            snn_require(match.count() == 7);
                            snn_require(match.size() == 7);
                            snn_require(match.view() == "three33");
                        }
                        else if (inner_count == 1)
                        {
                            snn_require(match.position() == 23);
                            snn_require(match.count() == 5);
                            snn_require(match.size() == 5);
                            snn_require(match.view() == "three");
                        }
                        else if (inner_count == 2)
                        {
                            snn_require(match.position() == 28);
                            snn_require(match.count() == 2);
                            snn_require(match.size() == 2);
                            snn_require(match.view() == "33");
                        }
                        ++inner_count;
                    }
                    snn_require(inner_count == 3);
                }
                ++count;
            }
            snn_require(count == 2);

            return true;
        }

        bool test_match()
        {
            const regex::pattern pat{"([A-Z]+)([0-9]+)"};
            regex::range::match rng{"This one1. And another three33.", pat};

            snn_require(!rng);
            snn_require(rng.is_empty());

            usize count = 0;
            for (const auto matches : rng)
            {
                ignore_if_unused(matches);
                ++count;
            }
            snn_require(count == 0);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
        snn_require(app::test_match());
    }
}
