// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/create_temporary.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/dir/remove.hh"

namespace snn
{
    void unittest()
    {
        static_assert(file::dir::is_valid_temporary_prefix("a"));
        static_assert(file::dir::is_valid_temporary_prefix("abc"));
        static_assert(file::dir::is_valid_temporary_prefix("abc-"));
        static_assert(file::dir::is_valid_temporary_prefix("c123"));
        static_assert(file::dir::is_valid_temporary_prefix("c123_"));
        static_assert(file::dir::is_valid_temporary_prefix("abcdefghijklmnopqrstuvwxyz"));
        static_assert(file::dir::is_valid_temporary_prefix("a-b_c"));
        static_assert(file::dir::is_valid_temporary_prefix("b-_"));

        static_assert(!file::dir::is_valid_temporary_prefix(""));
        static_assert(!file::dir::is_valid_temporary_prefix(" "));
        static_assert(!file::dir::is_valid_temporary_prefix("0abc"));
        static_assert(!file::dir::is_valid_temporary_prefix("/abc"));
        static_assert(!file::dir::is_valid_temporary_prefix("abc."));
        static_assert(!file::dir::is_valid_temporary_prefix("ABC"));
        static_assert(!file::dir::is_valid_temporary_prefix("a b"));
        static_assert(!file::dir::is_valid_temporary_prefix("a.b"));
        static_assert(!file::dir::is_valid_temporary_prefix("abcdefghijklmnopqrstuvwxyz"
                                                            "abcdefghijklmnopqrstuvwxyz"
                                                            "abcdefghijklmnopqrstuvwxyz"));

        // result<str> create_temporary(const cstrview prefix, str path)
        {
            const str s = file::dir::create_temporary("snn-unittest-", "/tmp").value();
            snn_require(s.has_front("/tmp/snn-unittest-"));
            snn_require(!s.has_back('/'));
            snn_require(s.size() == string_size("/tmp/snn-unittest-aabbccdd"));

            snn_require(file::dir::remove(s));
        }
        {
            // Invalid prefix.
            auto res = file::dir::create_temporary("", "/tmp");
            snn_require(!res);
            snn_require(res.error_code() == file::error::invalid_temporary_directory_prefix);
        }
        {
            // Invalid prefix.
            auto res = file::dir::create_temporary("invalid prefix", "/tmp");
            snn_require(!res);
            snn_require(res.error_code() == file::error::invalid_temporary_directory_prefix);
        }
        {
            // Empty path.
            auto res = file::dir::create_temporary("abc", "");
            snn_require(!res);
            snn_require(res.error_code() == file::error::invalid_temporary_directory_path);
        }

        // result<str> create_temporary(const cstrview prefix)
        {
            const str s = file::dir::create_temporary("snn-unittest-").value();
            snn_require(s.contains("/snn-unittest-"));
            snn_require(!s.has_back('/'));
            snn_require(s.size() >= string_size("/snn-unittest-aabbccdd"));

            snn_require(file::dir::remove(s));
        }
        {
            // Invalid prefix.
            auto res = file::dir::create_temporary("");
            snn_require(!res);
            snn_require(res.error_code() == file::error::invalid_temporary_directory_prefix);
        }
        {
            // Invalid prefix.
            auto res = file::dir::create_temporary("invalid prefix");
            snn_require(!res);
            snn_require(res.error_code() == file::error::invalid_temporary_directory_prefix);
        }
    }
}
