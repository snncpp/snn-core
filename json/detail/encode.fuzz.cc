// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/encode.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        try
        {
            str encoded = json::encode(input);
            ignore_if_unused(encoded);
        }
        catch (const exception& e)
        {
            snn_assert(e.error_code() == json::error::invalid_utf8);
        }
        catch (...)
        {
            snn_assert(false && "Unknown exception caught.");
        }

        return constant::exit::success;
    }
}
