# Reserved namespace for applications

The `snn::app` namespace is reserved for user applications (yours), unit tests, fuzzers, generators etc.

It is recommended to qualify function calls in this namespace, e.g. `app::my_function(...)`. This is to
avoid surprises with [ADL](https://en.cppreference.com/w/cpp/language/adl "Argument-dependent lookup").
