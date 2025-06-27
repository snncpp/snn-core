// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/zero.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include "snn-core/vec.hh"
#include "snn-core/fn/common.hh"

namespace snn
{
    void unittest()
    {
        {
            strbuf buf{init::fill, 64, 'a'};
            snn_require(buf.capacity() == 64);
            snn_require(buf.size() == 64);
            snn_require(buf.all(fn::is{fn::equal_to{}, 'a'}));

            mem::raw::zero(buf.writable(), byte_size{0});
            snn_require(buf.size() == 64);
            snn_require(buf.all(fn::is{fn::equal_to{}, 'a'}));

            mem::raw::zero(buf.writable(), byte_size{1});
            snn_require(buf.size() == 64);
            snn_require(buf == "\0aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
            snn_require(buf.front(assume::not_empty) == '\0');
            snn_require(buf.back(assume::not_empty) == 'a');

            buf.at(0).value() = 'a';
            snn_require(buf.size() == 64);
            snn_require(buf.all(fn::is{fn::equal_to{}, 'a'}));

            mem::raw::zero(buf);
            snn_require(buf.capacity() == 64);
            snn_require(buf.size() == 64);
            snn_require(buf.all(fn::is{fn::equal_to{}, '\0'}));
        }

        {
            vec<int> v;
            snn_require(v.byte_size().get() == 0);
            snn_require(v.writable() == nullptr);

            mem::raw::zero(v); // Does nothing.
            snn_require(v.byte_size().get() == 0);
            snn_require(v.writable() == nullptr);

            v.append(123);
            v.append(456);
            snn_require(v.front(assume::not_empty) == 123);
            snn_require(v.back(assume::not_empty) == 456);

            mem::raw::zero(v);
            snn_require(v.front(assume::not_empty) == 0);
            snn_require(v.back(assume::not_empty) == 0);
        }
    }
}
