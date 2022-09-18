// This requires Google Benchmark to be installed.
// https://github.com/google/benchmark (Apache License 2.0)

#include "snn-core/array_view.hh"
#include <benchmark/benchmark.h> // [#lib:benchmark]

#pragma GCC diagnostic ignored "-Wglobal-constructors"

static void BM_SomeFunction(benchmark::State& state)
{
    using namespace snn;

    // Setup

    array strings{
        cstrview{"123"},
        "-832732",
        "236741479",
    };

    for (auto _ : state)
    {
        // Code to benchmark.

        benchmark::DoNotOptimize(strings);

        for (auto s : strings)
        {
            auto p = s.to_prefix<i32>();
            benchmark::DoNotOptimize(p);
        }
    }
}

// Register benchmark.
BENCHMARK(BM_SomeFunction);

BENCHMARK_MAIN();
