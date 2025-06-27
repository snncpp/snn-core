// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/regex/pattern.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            snn_require(regex::pattern{"[a-z]+"}.match_first("123abc"));
            snn_require(!regex::pattern{"[a-z]+"}.match_first("123"));

            snn_require(regex::pattern{"[a-z]+"}.match_full("abc"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("aBc"));

            snn_require(regex::pattern{"[a-z]+"}.match_all("abc", fn::blackhole{}) == 1);
            snn_require(regex::pattern{"[a-z]+"}.match_all("a123b", fn::blackhole{}) == 2);
            snn_require(regex::pattern{"[a-z]+"}.match_all("123", fn::blackhole{}) == 0);

            snn_require(regex::pattern{"[0-9]+"}.replace("one11two22", "($&)") == "one(11)two(22)");
            snn_require(regex::pattern{"[a-z]+"}.replace("one11two22", "$&$&") ==
                        "oneone11twotwo22");
            snn_require(regex::pattern{"[A-Z]+"}.replace("one11two22", "$&$&") == "one11two22");

            regex::pattern p{"([a-z]+)([0-9]+)"};
            const auto matches = p.match_first("one11two22");
            snn_require(matches);
            snn_require(!matches.is_empty());
            snn_require(matches.count() == 3);
            snn_require(matches.at(0).value().view() == "one11");
            snn_require(matches.at(1).value().view() == "one");
            snn_require(matches.at(2).value().view() == "11");

            return true;
        }

        bool test_match_all()
        {
            {
                snn_require(regex::pattern{"[a-z]+"}.match_all("onetwo", fn::blackhole{}) == 1);
                snn_require(regex::pattern{"[a-z]+"}.match_all("o", fn::blackhole{}) == 1);
                snn_require(regex::pattern{"[a-zA-Z]+"}.match_all("OneTwo", fn::blackhole{}) == 1);
                snn_require(regex::pattern{"[a-z]+"}.match_all("a_", fn::blackhole{}) == 1);
                snn_require(regex::pattern{"[a-z]+"}.match_all("_b", fn::blackhole{}) == 1);
                snn_require(regex::pattern{"[a-z]+"}.match_all("OneTwo", fn::blackhole{}) == 2);
                snn_require(regex::pattern{"[a-z]+"}.match_all("a_b", fn::blackhole{}) == 2);

                snn_require(regex::pattern{"[a-z]+"}.match_all("", fn::blackhole{}) == 0);
                snn_require(regex::pattern{"[a-z]+"}.match_all("123", fn::blackhole{}) == 0);
                snn_require(regex::pattern{"[a-z]+"}.match_all("T", fn::blackhole{}) == 0);
                snn_require(regex::pattern{"[a-z]+"}.match_all("ä", fn::blackhole{}) == 0);
            }
            {
                usize count = 0;
                snn_require(regex::pattern{"([a-z]+)([0-9]+)"}.match_all(
                                "one1two22", [&count](const auto matches) {
                                    snn_require(matches);
                                    snn_require(matches.count() == 3);
                                    const auto match0 = matches.at(0).value();
                                    const auto match1 = matches.at(1).value();
                                    const auto match2 = matches.at(2).value();
                                    snn_require(!matches.at(3).has_value());

                                    if (count == 0)
                                    {
                                        snn_require(match0.position() == 0);
                                        snn_require(match0.count() == 4);
                                        snn_require(match0.size() == 4);
                                        snn_require(match0.view() == "one1");

                                        snn_require(match1.position() == 0);
                                        snn_require(match1.count() == 3);
                                        snn_require(match1.size() == 3);
                                        snn_require(match1.view() == "one");

                                        snn_require(match2.position() == 3);
                                        snn_require(match2.count() == 1);
                                        snn_require(match2.size() == 1);
                                        snn_require(match2.view() == "1");
                                    }
                                    else if (count == 1)
                                    {
                                        snn_require(match0.position() == 4);
                                        snn_require(match0.count() == 5);
                                        snn_require(match0.size() == 5);
                                        snn_require(match0.view() == "two22");

                                        snn_require(match1.position() == 4);
                                        snn_require(match1.count() == 3);
                                        snn_require(match1.size() == 3);
                                        snn_require(match1.view() == "two");

                                        snn_require(match2.position() == 7);
                                        snn_require(match2.count() == 2);
                                        snn_require(match2.size() == 2);
                                        snn_require(match2.view() == "22");
                                    }
                                    ++count;
                                }) == 2);
                snn_require(count == 2);
            }

            return true;
        }

        bool test_match_first()
        {
            {
                snn_require(regex::pattern{"[a-z]+"}.match_first("onetwo"));
                snn_require(regex::pattern{"[a-z]+"}.match_first("o"));
                snn_require(regex::pattern{"[a-zA-Z]+"}.match_first("OneTwo"));
                snn_require(regex::pattern{"[a-z]+"}.match_first("OneTwo"));
                snn_require(regex::pattern{"[a-z]+"}.match_first("a_b"));
                snn_require(regex::pattern{"[a-z]+"}.match_first("a_"));
                snn_require(regex::pattern{"[a-z]+"}.match_first("_b"));

                snn_require(!regex::pattern{"[a-z]+"}.match_first(""));
                snn_require(!regex::pattern{"[a-z]+"}.match_first("123"));
                snn_require(!regex::pattern{"[a-z]+"}.match_first("T"));
                snn_require(!regex::pattern{"[a-z]+"}.match_first("ä"));
            }
            {
                const auto matches = regex::pattern{"([a-z]+)([0-9]+)"}.match_first("one11two22");
                snn_require(matches);
                snn_require(!matches.is_empty());
                snn_require(matches.count() == 3);

                usize count = 0;
                for (const auto match : matches)
                {
                    if (count == 0)
                    {
                        snn_require(match.position() == 0);
                        snn_require(match.count() == 5);
                        snn_require(match.size() == 5);
                        snn_require(match.view() == "one11");
                    }
                    else if (count == 1)
                    {
                        snn_require(match.position() == 0);
                        snn_require(match.count() == 3);
                        snn_require(match.size() == 3);
                        snn_require(match.view() == "one");
                    }
                    else if (count == 2)
                    {
                        snn_require(match.position() == 3);
                        snn_require(match.count() == 2);
                        snn_require(match.size() == 2);
                        snn_require(match.view() == "11");
                    }
                    ++count;
                }
                snn_require(count == 3);

                snn_require(matches.at(0).has_value());

                const auto match = matches.at(1).value();
                snn_require(match.position() == 0);
                snn_require(match.count() == 3);
                snn_require(match.size() == 3);
                snn_require(match.view() == "one");

                snn_require(matches.at(2).has_value());
                snn_require(!matches.at(3).has_value());
            }
            {
                const auto matches = regex::pattern{"[a-z]+"}.match_first("1two");
                snn_require(matches);
                snn_require(!matches.is_empty());
                snn_require(matches.count() == 1);

                auto rng = matches.range();
                snn_require(rng);
                snn_require(!rng.is_empty());

                auto match0 = rng.front().value();
                snn_require(match0.position() == 1);
                snn_require(match0.count() == 3);
                snn_require(match0.size() == 3);
                snn_require(match0.view() == "two");

                rng.drop_front(assume::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());

                snn_require(!rng.front().has_value());
            }
            {
                const auto matches = regex::pattern{"[0-9]+"}.match_first("onetwo");
                snn_require(!matches);
                snn_require(matches.is_empty());
                snn_require(matches.count() == 0);
                snn_require(matches.begin() == matches.end());
            }
            {
                const regex::pattern p{strbuf{"[a-z]+"}, regex::icase};
                const str s{"123ONETWO"};
                snn_require(p.match_first(s));
            }
            {
                const regex::pattern p{std::regex{"[a-z]+", std::regex::icase}};
                const str s{"123ONETWO"};
                snn_require(p.match_first(s));
            }

            return true;
        }

        bool test_match_full()
        {
            snn_require(regex::pattern{"[a-z]+"}.match_full("onetwo"));
            snn_require(regex::pattern{"[a-z]+"}.match_full("o"));
            snn_require(regex::pattern{"[a-zA-Z]+"}.match_full("OneTwo"));

            snn_require(!regex::pattern{"[a-z]+"}.match_full(""));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("OneTwo"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("123"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("a_b"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("a_"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("_b"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("T"));
            snn_require(!regex::pattern{"[a-z]+"}.match_full("ä"));

            {
                const auto matches = regex::pattern{"([a-z]{3})([a-z]{3})"}.match_full("onetwo");
                snn_require(matches);
                snn_require(!matches.is_empty());
                snn_require(matches.count() == 3);

                usize count = 0;
                for (const auto match : matches)
                {
                    if (count == 0)
                    {
                        snn_require(match.position() == 0);
                        snn_require(match.count() == 6);
                        snn_require(match.size() == 6);
                        snn_require(match.view() == "onetwo");
                    }
                    else if (count == 1)
                    {
                        snn_require(match.position() == 0);
                        snn_require(match.count() == 3);
                        snn_require(match.size() == 3);
                        snn_require(match.view() == "one");
                    }
                    else if (count == 2)
                    {
                        snn_require(match.position() == 3);
                        snn_require(match.count() == 3);
                        snn_require(match.size() == 3);
                        snn_require(match.view() == "two");
                    }
                    ++count;
                }
                snn_require(count == 3);

                snn_require(matches.at(0).has_value());

                const auto match = matches.at(1).value();
                snn_require(match.position() == 0);
                snn_require(match.count() == 3);
                snn_require(match.size() == 3);
                snn_require(match.view() == "one");

                snn_require(matches.at(2).has_value());
                snn_require(!matches.at(3).has_value());
            }

            {
                const auto matches = regex::pattern{"[0-9][a-z]+"}.match_full("1two");
                snn_require(matches);
                snn_require(!matches.is_empty());
                snn_require(matches.count() == 1);

                auto rng = matches.range();
                snn_require(rng);
                snn_require(!rng.is_empty());

                auto match0 = rng.front().value();
                snn_require(match0.position() == 0);
                snn_require(match0.count() == 4);
                snn_require(match0.size() == 4);
                snn_require(match0.view() == "1two");

                rng.drop_front(assume::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());

                snn_require(!rng.front().has_value());
            }

            {
                const auto matches = regex::pattern{"[0-9]+"}.match_full("onetwo");
                snn_require(!matches);
                snn_require(matches.is_empty());
                snn_require(matches.count() == 0);
                snn_require(matches.begin() == matches.end());
            }

            {
                const auto matches = regex::pattern{".*"}.match_full("");
                snn_require(matches);
                snn_require(matches.count() == 1);

                const auto match = matches.at(0).value();
                snn_require(match.position() == 0);
                snn_require(match.count() == 0);
                snn_require(match.size() == 0);
                snn_require(match.view() == "");
            }

            {
                const str s{"OneTwo"};
                const regex::pattern p{"[a-z]+", regex::icase};
                snn_require(p.match_full(s));
            }

            {
                const cstrview s{"One\0"};
                snn_require(s.size() == 4);
                snn_require(regex::pattern{"([A-Z][a-z]+\\0)+"}.match_full(s));
                snn_require(!regex::pattern{"([A-Z][a-z]+)+"}.match_full(s));
            }

            return true;
        }

        bool test_replace()
        {
            snn_require(regex::pattern{"[0-9]+"}.replace("one11two22", "($&)") == "one(11)two(22)");
            snn_require(regex::pattern{"[a-z]+"}.replace("one11two22", "$&$&") ==
                        "oneone11twotwo22");
            snn_require(regex::pattern{"[A-Z]+"}.replace("one11two22", "$&$&") == "one11two22");
            snn_require(regex::pattern{"[a-z]+"}.replace("one11two22", "$$1$$&") == "$1$&11$1$&22");
            snn_require(regex::pattern{"([a-z]+)\\0"}.replace("one\0two\0", "$1$&") ==
                        "oneone\0twotwo\0");

            // Replacement format string can not contain null characters.
            snn_require_throws_code(regex::pattern{"[a-z]+"}.replace("a", "a\0"),
                                    generic::error::unexpected_null_character);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
        snn_require(app::test_match_all());
        snn_require(app::test_match_first());
        snn_require(app::test_match_full());
        snn_require(app::test_replace());
    }
}
