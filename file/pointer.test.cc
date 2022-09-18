// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/pointer.hh"

#include "snn-core/unittest.hh"
#include <cstdio>   // fopen
#include <unistd.h> // close

namespace snn
{
    void unittest()
    {
        static_assert(is_trivially_relocatable_v<file::pointer>);

        {
            file::pointer fp;
            snn_require(!fp);
            snn_require(!fp.has_value());
            snn_require(fp.value_or(nullptr) == nullptr);
            snn_require_throws_code(fp.value(), generic::error::no_value);

            snn_require(!fp.integer_descriptor().has_value());
            snn_require_throws_code(fp.integer_descriptor().value(), generic::error::no_value);
            snn_require(fp.integer_descriptor().value_or(-1) == -1);
        }

        {
            file::pointer fp{nullptr};
            snn_require(!fp);
            snn_require(!fp.has_value());
            snn_require_throws_code(fp.value(), generic::error::no_value);

            snn_require(!fp.integer_descriptor().has_value());
            snn_require_throws_code(fp.integer_descriptor().value(), generic::error::no_value);
            snn_require(fp.integer_descriptor().value_or(-1) == -1);
        }

        {
            file::pointer fp{std::fopen("/dev/null", "wb")};
            snn_require(fp);
            snn_require(fp.has_value());
            snn_require(fp.value() != nullptr);
            snn_require(fp.value(promise::has_value) != nullptr);
            snn_require(fp.value_or(nullptr) != nullptr);

            snn_require(fp.integer_descriptor().has_value());
            snn_require(fp.integer_descriptor().value() != -1);
            snn_require(fp.integer_descriptor().value_or(-1) != -1);
        }

        {
            int fd = -1;
            {
                file::pointer fp{std::fopen("/dev/null", "wb")};
                snn_require(fp);
                snn_require(fp.has_value());
                fd = fp.integer_descriptor().value();
                // std::fclose() will be called on the pointer here.
            }
            snn_require(fd > 0);
            snn_require(::close(fd) == -1);
            snn_require(errno == EBADF);
        }

        {
            file::pointer fp{std::fopen("/dev/null", "wb")};
            snn_require(fp);
            snn_require(fp.has_value());

            std::FILE* const ptr = fp.value();
            snn_require(ptr != nullptr);

            file::pointer fp2{std::move(fp)};
            snn_require(fp2);
            snn_require(fp2.has_value());
            snn_require(fp2.value(promise::has_value) == ptr);
            snn_require(!fp);
            snn_require(!fp.has_value());

            file::pointer fp3;
            snn_require(!fp3);
            snn_require(!fp3.has_value());
            fp3 = std::move(fp2);
            snn_require(fp3);
            snn_require(fp3.has_value());
            snn_require(fp3.value(promise::has_value) == ptr);
            snn_require(!fp2);
            snn_require(!fp2.has_value());

            swap(fp, fp3);
            snn_require(!fp3);
            snn_require(!fp3.has_value());
            snn_require(fp);
            snn_require(fp.has_value());
            snn_require(fp.value(promise::has_value) == ptr);

            snn_require(fp.close());
            snn_require(!fp);
            snn_require(!fp.has_value());

            snn_require(!fp.close()); // Already closed.
            snn_require(!fp);
            snn_require(!fp.has_value());
        }
    }
}
