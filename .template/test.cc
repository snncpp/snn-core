#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_something()
        {
            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_something());
    }
}
