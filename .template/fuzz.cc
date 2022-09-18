#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        // Do something with `input` here.

        return constant::exit::success;
    }
}
