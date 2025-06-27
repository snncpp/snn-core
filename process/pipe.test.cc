// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/process/pipe.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            {
                // Will throw if pipe creation failed.
                process::pipe pipe;
                snn_require(pipe.reading_end().value_or(-1) > 0);
                snn_require(pipe.writing_end().value_or(-1) > 0);
            }
            {
                // Non-throwing alternative.
                auto res = process::pipe::make();
                snn_require(res);
                process::pipe& pipe = res.value(assume::has_value);
                snn_require(pipe.reading_end().value_or(-1) > 0);
                snn_require(pipe.writing_end().value_or(-1) > 0);
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

        static_assert(is_trivially_relocatable_v<process::pipe>);
    }
}
