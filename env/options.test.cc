// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/env/options.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/vec.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            const array argv{
                env::argument{"./app"},
                "-t",
                "something",
            };
            const auto arguments = argv.view();

            env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

            snn_require(opts); // Parsed successfully.

            snn_require(opts.program_name() == "./app");

            snn_require(!opts.option('v').is_set());
            snn_require(opts.option('t').is_set());

            const auto args = opts.arguments();
            snn_require(args.count() == 1);
            snn_require(args.front().value().to<cstrview>() == "something");

            return true;
        }

        constexpr bool test_options()
        {
            static_assert(sizeof(env::option) == 48);

            static_assert(env::option::is_valid_short_flag_name('a'));
            static_assert(env::option::is_valid_short_flag_name('z'));
            static_assert(env::option::is_valid_short_flag_name('A'));
            static_assert(env::option::is_valid_short_flag_name('Z'));
            static_assert(env::option::is_valid_short_flag_name('0'));
            static_assert(env::option::is_valid_short_flag_name('9'));
            static_assert(!env::option::is_valid_short_flag_name(' '));
            static_assert(!env::option::is_valid_short_flag_name('-'));
            static_assert(!env::option::is_valid_short_flag_name('.'));
            static_assert(!env::option::is_valid_short_flag_name('?'));
            static_assert(!env::option::is_valid_short_flag_name('\0'));

            static_assert(env::option::is_valid_long_flag_name("do"));
            static_assert(env::option::is_valid_long_flag_name("new"));
            static_assert(env::option::is_valid_long_flag_name("NEW"));
            static_assert(env::option::is_valid_long_flag_name("format"));
            static_assert(env::option::is_valid_long_flag_name("ignore-zeros"));
            static_assert(env::option::is_valid_long_flag_name("do-this-and-that"));
            static_assert(env::option::is_valid_long_flag_name("a1"));
            static_assert(env::option::is_valid_long_flag_name("1a"));
            static_assert(env::option::is_valid_long_flag_name("a-b"));
            static_assert(env::option::is_valid_long_flag_name("1-2"));

            static_assert(!env::option::is_valid_long_flag_name(""));
            static_assert(!env::option::is_valid_long_flag_name("a"));
            static_assert(!env::option::is_valid_long_flag_name("abc?"));
            static_assert(!env::option::is_valid_long_flag_name("a--b"));
            static_assert(!env::option::is_valid_long_flag_name("do-"));
            static_assert(!env::option::is_valid_long_flag_name("åäö"));
            static_assert(!env::option::is_valid_long_flag_name("ignore-zeros-"));
            static_assert(!env::option::is_valid_long_flag_name("-do"));
            static_assert(!env::option::is_valid_long_flag_name("-"));
            static_assert(!env::option::is_valid_long_flag_name("--"));
            static_assert(!env::option::is_valid_long_flag_name("---"));
            static_assert(!env::option::is_valid_long_flag_name("."));
            static_assert(!env::option::is_valid_long_flag_name(".."));

            snn_require_nothrow((env::option{"abc", 'a'}));
            snn_require_throws_code((env::option{"abc", '.'}), env::error::invalid_flag_name);
            snn_require_throws_code((env::option{"-", 'a'}), env::error::invalid_flag_name);

            {
                env::option o{"verbose"};
                snn_require(o.long_flag() == "verbose");
                snn_require(!o.short_flag().has_value());
                snn_require(o.is_boolean());
                snn_require(!o.is_set());
                snn_require(o.count() == 0);
            }

            {
                env::option o{"verbose", 'v'};
                snn_require(o.long_flag() == "verbose");
                snn_require(o.short_flag().has_value());
                snn_require(o.short_flag().value() == 'v');
                snn_require(o.is_boolean());
                snn_require(!o.is_set());
                snn_require(o.count() == 0);
            }

            {
                env::option o{"skip-empty", nullopt, env::option::takes_values};
                snn_require(o.long_flag() == "skip-empty");
                snn_require(!o.short_flag().has_value());
                snn_require(!o.is_boolean());
                snn_require(!o.is_set());
                snn_require(o.count() == 0);
                snn_require(o.values().is_empty());
            }

            {
                snn_require(env::option{"skip-empty"} < env::option{"verbose"});
                snn_require(!(env::option{"verbose"} < env::option{"skip-empty"}));
                snn_require(!(env::option{"verbose"} < env::option{"verbose"}));

                snn_require(env::option{"skip-empty"} < cstrview{"verbose"});
                snn_require(!(env::option{"verbose"} < cstrview{"skip-empty"}));
                snn_require(!(env::option{"verbose"} < cstrview{"verbose"}));
            }

            {
                const array argv{
                    env::argument{"./app"},
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

                snn_require(opts); // Parsed successfully.

                snn_require(!opts.get('v').value().is_set());
                snn_require(!opts.option('t').is_set());

                snn_require(opts.program_name() == "./app");

                const auto& args = opts.arguments();
                snn_require(args.is_empty());
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "hello",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

                snn_require(opts); // Parsed successfully.

                const auto ec = opts.error_code();
                snn_require(!ec);

                snn_require(opts.program_name() == "./app");

                snn_require(!opts.get('v').value().is_set());
                snn_require(!opts.get("verbose").value().is_set());

                snn_require(!opts.option('t').is_set());
                snn_require(!opts.option("time").is_set());

                snn_require_throws_code(opts.option('u'), env::error::unknown_option);
                snn_require_throws_code(opts.option("unknown"), env::error::unknown_option);

                const auto& args = opts.arguments();
                snn_require(args.count() == 1);
                snn_require(args.at(0).value().to<cstrview>() == "hello");
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "-",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

                snn_require(opts); // Parsed successfully.

                snn_require(opts.program_name() == "./app");

                snn_require(!opts.get('v').value().is_set());
                snn_require(!opts.option('t').is_set());

                const auto& args = opts.arguments();
                snn_require(args.count() == 1);
                snn_require(args.at(0).value().to<cstrview>() == "-");
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "-t",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

                snn_require(opts); // Parsed successfully.

                snn_require(opts.program_name() == "./app");

                snn_require(!opts.get('v').value().is_set());
                snn_require(opts.get('t').value().is_set());

                snn_require(!opts.option('v').is_set());
                snn_require(opts.option('t').is_set());

                const auto& args = opts.arguments();
                snn_require(args.count() == 0);
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "-vv",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

                snn_require(opts); // Parsed successfully.

                snn_require(opts.program_name() == "./app");

                snn_require(opts.get('v').value().is_set());
                snn_require(!opts.get('t').value().is_set());

                snn_require(opts.option('v').is_set());
                snn_require(!opts.option('t').is_set());

                snn_require(opts.option('v').count() == 2);

                const auto& args = opts.arguments();
                snn_require(args.count() == 0);
            }

            {
                // These are all equal (if the last value is used).
                array variations{
                    vec<env::argument>{{"./app", "-t", "-c", "23", "--", "-v", "abcdef"}},
                    vec<env::argument>{{"./app", "-c", "23", "-t", "--", "-v", "abcdef"}},
                    vec<env::argument>{{"./app", "-t", "--count", "23", "--", "-v", "abcdef"}},
                    vec<env::argument>{{"./app", "--count", "23", "-t", "--", "-v", "abcdef"}},
                    vec<env::argument>{{"./app", "-tc", "23", "--", "-v", "abcdef"}},
                    vec<env::argument>{{"./app", "-tc23", "--", "-v", "abcdef"}},
                    vec<env::argument>{
                        {"./app", "-t", "--count", "42", "-c23", "--", "-v", "abcdef"}},
                };

                for (const auto& variation : variations)
                {
                    const auto arguments = variation.view();

                    env::options opts{arguments,
                                      {{"count", 'c', env::option::takes_values},
                                       {"time", 't'},
                                       {"verbose", 'v'}}};

                    snn_require(opts); // Parsed successfully.

                    snn_require(opts.program_name() == "./app");

                    snn_require(opts.get('v').value().count() == 0);
                    snn_require(!opts.get('v').value().is_set());
                    snn_require(!opts.get("verbose").value().is_set());

                    snn_require(!opts.option("verbose").is_set());

                    snn_require(opts.get('t').value().count() == 1);
                    snn_require(opts.get('t').value().is_set());

                    snn_require(opts.option('t').count() == 1);
                    snn_require(opts.option('t').is_set());

                    const auto& opt = opts.get('c').value();
                    snn_require(opt.count() >= 1);
                    const auto& values = opt.values();
                    snn_require(values.count() >= 1);
                    snn_require(values.back().value() == "23");

                    snn_require(opts.get("count").value().is_set());

                    snn_require(!opts.get("other"));

                    const auto& args = opts.arguments();
                    snn_require(args.count() == 2);
                    snn_require(args.at(0).value().to<cstrview>() == "-v");
                    snn_require(args.at(1).value().to<cstrview>() == "abcdef");
                }
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "-t",
                    "-x",
                    "something",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {{"verbose", 'v'}, {"time", 't'}}};

                snn_require(!opts); // Parse failed.
                snn_require(opts.error_code() == env::error::unknown_option);
                snn_require(opts.error_message() == "Unknown option \"-x\"");

                // Options and arguments are cleared if parsing fails.
                snn_require(!opts.get('v'));
                snn_require(!opts.get("verbose"));
                snn_require(!opts.get('t'));
                snn_require(!opts.get("time"));
                snn_require(opts.arguments().is_empty());
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "--unknown-option",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {}};

                snn_require(!opts); // Parse failed.
                snn_require(opts.error_code() == env::error::unknown_option);
                snn_require(opts.error_message() == "Unknown option \"--unknown-option\"");
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "---invalid-option",
                };
                const auto arguments = argv.view();

                env::options opts{arguments, {}};

                snn_require(!opts); // Parse failed.
                snn_require(opts.error_code() == env::error::unknown_option);
                snn_require(opts.error_message() == "Unknown option \"---invalid-option\"");
            }

            {
                const array argv{
                    env::argument{"./app"},
                    "--option-req-value",
                };
                const auto arguments = argv.view();

                env::options opts{arguments,
                                  {{"option-req-value", 'o', env::option::takes_values}}};

                snn_require(!opts); // Parse failed.
                snn_require(opts.error_code() == env::error::option_requires_value);
                snn_require(opts.error_message() ==
                            "Option \"--option-req-value\" requires a value");
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
        snn_static_require(app::test_options());
    }
}
