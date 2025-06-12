// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pattern (`std::regex` wrapper)

// Uses ECMAScript grammar by default:
// https://en.cppreference.com/w/cpp/regex/ecmascript

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/regex/matches.hh"
#include "snn-core/regex/matches_view.hh"
#include "snn-core/string/append_iterator.hh"
#include <regex>

namespace snn::regex
{
    // ## Constants

    // ### Pattern options

    inline constexpr auto ecmascript = std::regex::ECMAScript;
    inline constexpr auto icase      = std::regex::icase;
    inline constexpr auto optimize   = std::regex::optimize;
    inline constexpr auto multiline  = std::regex::multiline;

    // ## Classes

    // ### pattern

    class pattern final
    {
      public:
        // #### Types

        using option_type = std::regex_constants::syntax_option_type;

        // #### Constructors

        explicit pattern(std::regex pat) noexcept
            : pat_{std::move(pat)}
        {
        }

        explicit pattern(const transient<cstrview> pat, const option_type options = ecmascript)
            : pat_{pat.get().cbegin(), pat.get().size(), options}
        {
        }

        // #### Match

        template <typename Callback>
        usize match_all(const cstrview subject, Callback cb) const
        {
            std::cregex_iterator cur{subject.begin(), subject.end(), pat_};
            const std::cregex_iterator last;
            usize count = 0;
            while (cur != last)
            {
                cb(matches_view{*cur});
                ++cur;
                ++count;
            }
            return count;
        }

        [[nodiscard]] matches match_first(const cstrview subject) const
        {
            matches ma;
            const bool b = std::regex_search(subject.begin(), subject.end(), ma.cmatch(), pat_);
            snn_should(ma.cmatch().ready());
            snn_should((b && ma.count() > 0) || (!b && ma.count() == 0));
            ignore_if_unused(b);
            return ma;
        }

        [[nodiscard]] matches match_full(const cstrview subject) const
        {
            matches ma;
            const bool b = std::regex_match(subject.begin(), subject.end(), ma.cmatch(), pat_);
            snn_should(ma.cmatch().ready());
            snn_should((b && ma.count() > 0) || (!b && ma.count() == 0));
            ignore_if_unused(b);
            return ma;
        }

        // #### Replace

        // Replace all matches with replacement format string.
        // Replacement format string can not contain null bytes.

        // Replacement format string syntax:
        // $& Full match.
        // $0 Full match.
        // $1 Sub match 1.
        // ...
        // $9 Sub match 9.
        // $$ Literal '$' character.

        template <any_strcore Str = str>
        [[nodiscard]] Str replace(const transient<cstrview> subject,
                                  const str& replacement_format) const
        {
            const cstrview s = subject.get();
            // std::regex_replace will pass the "regex replacement format string" as a
            // null-terminated c-string. Even if passed a std::string it will call c_str() on it.
            Str result{init::reserve, s.size()};
            std::regex_replace(string::append_iterator{result}, s.begin(), s.end(), pat_,
                               replacement_format.null_terminated().get());
            return result;
        }

        // #### Internal

        [[nodiscard]] std::regex& internal() noexcept
        {
            return pat_;
        }

        [[nodiscard]] const std::regex& internal() const noexcept
        {
            return pat_;
        }

      private:
        std::regex pat_;
    };
}
