// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/descriptor.hh"

#include "snn-core/unittest.hh"
#include <sys/types.h>  // socket
#include <sys/socket.h> // socket, PF_INET, SOCK_DGRAM

namespace snn
{
    void unittest()
    {
        static_assert(is_trivially_relocatable_v<file::descriptor>);

        {
            file::descriptor d;
            snn_require(!d);
            snn_require(!d.has_value());
            snn_require(d.value_or(-1) == -1);
            snn_require_throws_code(d.value(), generic::error::no_value);
        }

        {
            file::descriptor d{-1};
            snn_require(!d);
            snn_require(!d.has_value());
            snn_require(d.value_or(-1) == -1);
            snn_require_throws_code(d.value(), generic::error::no_value);
        }

        {
            file::descriptor d{-2};
            snn_require(!d);
            snn_require(!d.has_value());
            snn_require(d.value_or(-1) == -1);
            snn_require_throws_code(d.value(), generic::error::no_value);
        }

        {
            file::descriptor sock{::socket(PF_INET, SOCK_DGRAM, 0)};
            snn_require(sock);
            snn_require(sock.has_value());
            snn_require(sock.value() > 0);
            snn_require(sock.value(assume::has_value) > 0);
            snn_require(sock.value_or(-1) > 0);
        }

        {
            int desc = -1;
            {
                file::descriptor sock{::socket(PF_INET, SOCK_DGRAM, 0)};
                snn_require(sock);
                snn_require(sock.has_value());
                desc = sock.value(assume::has_value);
                // ::close() will be called on the descriptor here.
            }
            // The following code is only for this single-threaded unit test, never close a
            // descriptor more than once in production.
            snn_require(desc > 0);
            snn_require(::close(desc) == -1);
            snn_require(errno == EBADF);
        }

        {
            file::descriptor sock{::socket(PF_INET, SOCK_DGRAM, 0)};
            snn_require(sock);
            snn_require(sock.has_value());
            const int desc = sock.value(assume::has_value);
            snn_require(desc > 0);

            file::descriptor s2{std::move(sock)};
            snn_require(s2);
            snn_require(s2.has_value());
            snn_require(!sock);
            snn_require(!sock.has_value());
            snn_require(s2.value(assume::has_value) == desc);

            file::descriptor s3;
            snn_require(!s3);
            snn_require(!s3.has_value());
            s3 = std::move(s2);
            snn_require(s3);
            snn_require(s3.has_value());
            snn_require(!s2);
            snn_require(!s2.has_value());
            snn_require(s3.value(assume::has_value) == desc);

            sock.swap(s3);
            snn_require(!s3);
            snn_require(!s3.has_value());
            snn_require(sock);
            snn_require(sock.has_value());

            snn_require(sock.close());
            snn_require(!sock);
            snn_require(!sock.has_value());

            snn_require(!sock.close()); // Already closed.
            snn_require(!sock);
            snn_require(!sock.has_value());
        }
    }
}
