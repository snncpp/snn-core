// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/pcre/pattern.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            {
                pcre::pattern p{"[a-z]+"};
                snn_require(p);
                snn_require(p.is_valid());
                snn_require(p.match_first("a"));
                snn_require(p.match_first("onetwo"));
                snn_require(p.match_first("123abc"));
                snn_require(p.match_first("_a"));
                snn_require(p.match_first("a_"));
                snn_require(!p.match_first(""));
                snn_require(!p.match_first("_"));
                snn_require(!p.match_first("ä"));
                snn_require(!p.match_first("A"));
                snn_require(!p.match_first("123"));
            }
            {
                pcre::pattern p{"[a-z]+", pcre::icase};
                snn_require(p);
                snn_require(p.is_valid());
                snn_require(p.match_first("a"));
                snn_require(p.match_first("A"));
                snn_require(p.match_first("_a"));
                snn_require(p.match_first("_A"));
                snn_require(!p.match_first(""));
                snn_require(!p.match_first("_"));
            }
            {
                pcre::pattern p{"([a-z]+)([0-9]+)"};
                snn_require(p);
                snn_require(p.is_valid());

                const auto matches = p.match_first("one11two22");
                snn_require(matches);
                snn_require(!matches.is_empty());
                snn_require(matches.count() == 3);

                {
                    const auto match = matches.at(0).value();
                    snn_require(match.position() == 0);
                    snn_require(match.count() == 5);
                    snn_require(match.size() == 5);
                    snn_require(match.view() == "one11");
                }

                {
                    const auto match = matches.at(1).value();
                    snn_require(match.position() == 0);
                    snn_require(match.count() == 3);
                    snn_require(match.size() == 3);
                    snn_require(match.view() == "one");
                }

                {
                    const auto match = matches.at(2).value();
                    snn_require(match.position() == 3);
                    snn_require(match.count() == 2);
                    snn_require(match.size() == 2);
                    snn_require(match.view() == "11");
                }

                snn_require(!matches.at(3).has_value());
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        static_assert(is_trivially_relocatable_v<pcre::pattern>);
        static_assert(sizeof(pcre::pattern) == 16);
        static_assert(sizeof(pcre::matches) == 32);

        {
            pcre::pattern p{"[a-z]+", "i"};
            snn_require(p);
            snn_require(p.is_valid());
            snn_require(p.match_first("a"));
            snn_require(p.match_first("A"));
            snn_require(p.match_first("_a"));
            snn_require(p.match_first("_A"));
            snn_require(!p.match_first(""));
            snn_require(!p.match_first("_"));

            snn_require_throws_code((pcre::pattern{"[a-z]+", "@"}),
                                    pcre::error::invalid_pattern_modifier);
        }

        {
            pcre::pattern p{strbuf{"[a-z]+"}, strbuf{"i"}}; // From temporaries.
            snn_require(p);
            snn_require(p.is_valid());
            snn_require(p.match_first("a"));
            snn_require(p.match_first("A"));
            snn_require(p.match_first("_a"));
            snn_require(p.match_first("_A"));
            snn_require(!p.match_first(""));
            snn_require(!p.match_first("_"));
        }

        {
            pcre::pattern p{"abc\\"};
            snn_require(!p);
            snn_require(!p.is_valid());
            snn_require(p.error_number() == PCRE2_ERROR_END_BACKSLASH);
            snn_require(p.error_position() == 4);
        }

        {
            // Remember that $ (dollar) allows for a newline at the end by default.
            // This can be disabled with options or modifiers.

            pcre::pattern a_to_z_only_with_nl{"^[a-z]+$"};
            pcre::pattern a_to_z_only_without_nl{"^[a-z]+$", pcre::dollar_endonly};

            snn_require(a_to_z_only_with_nl.match_first("a"));
            snn_require(a_to_z_only_with_nl.match_first("a\n"));

            snn_require(a_to_z_only_without_nl.match_first("a"));
            snn_require(!a_to_z_only_without_nl.match_first("a\n"));

            snn_require(!a_to_z_only_with_nl.match_first("a\n\n"));
            snn_require(!a_to_z_only_without_nl.match_first("a\n\n"));

            snn_require(!a_to_z_only_with_nl.match_first("a\nb\n"));
            snn_require(!a_to_z_only_without_nl.match_first("a\nb\n"));
        }

        {
            // UTF mode with UCP (unicode properties).
            pcre::pattern utf_word{"^\\w+$", "up"};

            snn_require(utf_word.match_first("åäö"));
            snn_require(!utf_word.match_first("\xE5")); // Invalid UTF-8.

            // Non-UTF mode.
            pcre::pattern non_utf_word{"^\\w+$"};

            snn_require(!non_utf_word.match_first("åäö"));
        }

        {
            const auto matches = pcre::pattern{"^([^0-9]+)([0-9]+)$", "D"}.match_first("åäö123");
            snn_require(matches);
            snn_require(!matches.is_empty());
            snn_require(matches.count() == 3);

            {
                const auto match = matches.at(0).value();
                snn_require(match.position() == 0);
                snn_require(match.count() == 9);
                snn_require(match.size() == 9);
                snn_require(match.view() == "åäö123");
            }

            {
                const auto match = matches.at(1).value();
                snn_require(match.position() == 0);
                snn_require(match.count() == 6);
                snn_require(match.size() == 6);
                snn_require(match.view() == "åäö");
            }

            {
                const auto match = matches.at(2).value();
                snn_require(match.position() == 6);
                snn_require(match.count() == 3);
                snn_require(match.size() == 3);
                snn_require(match.view() == "123");
            }

            snn_require(!matches.at(3).has_value());
        }

        {
            const auto matches = pcre::pattern{"[0-9]+"}.match_first("onetwo");
            snn_require(!matches);
            snn_require(matches.is_empty());
            snn_require(matches.count() == 0);
        }
    }
}
