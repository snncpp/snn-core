#include "snn-core/main.hh"

namespace snn
{
    int main(array_view<const env::argument>)
    {
        fmt::print_line("Hello!");

        return constant::exit::success;
    }
}
