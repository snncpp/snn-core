# snncpp

A modern, safer alternative to the _C++ Standard Library_ (for Linux/BSD).

_The author of this library ([@msimonsson](https://github.com/msimonsson)) is available for remote work._


## Features

 * Immutable and mutable `array_view`, including `[c]strview`.
 * Safe and efficient string parsing with ranges ([example](json/is_number.hh)).
 * Strings with and without _Small String Optimization_ (`str/strbuf`).
 * Vector with optional small capacity and trivially relocatable optimization (`vec`).
 * Date and time library with IANA time zone database ([time/](time/)).
 * `result/optional` with reference support.
 * `.range()`/`.view(...)` member functions to discourage the use of iterators.
 * Validation member functions like `.all(...)` and `.any(...)` on iterable containers.
 * Safe integral class and a decimal class with a static/dynamic scale ([num/](num/)).
 * Command line options parsing ([env/](env/)).
 * Filesystem library ([file/](file/)).
 * Fast JSON parsing ([json/](json/)).
 * Go-like [build-tool][buildtool] with fuzzing support.
 * And much more...


## Goals

 * Safe by default or _even harder to shoot yourself in the foot_ (see [Safety](#safety)).
 * Readable and well tested code.
 * High performance without sacrificing on the points above.


## Non-goals

 * Replacing the entire _C++ Standard Library_. This library uses the _C++ Standard Library_
   throughout where appropriate, e.g. type traits, concepts, algorithms and utility functions
   like `std::move`.


## Requirements

 * C++20
 * 64-bit


## Officially supported platforms

This library currently targets [POSIX][posix] and is developed and tested on:

| Operating system     | Compiler             |
| -------------------- | -------------------- |
| FreeBSD 13.1         | Clang 13+            |
| Fedora Linux 36      | Clang 13+            |


## Limitations

 * No wide character support (by design).
 * No Windows support (most code outside of `file::`, `random::` and `process::` should work)[^contribwelcome].
 * No grapheme support[^contribwelcome].
 * No big-endian support[^contribwelcome].

[^contribwelcome]: Contributions welcome.


## Overview

| Path                                                                  | Description                                               |                                                       |
| --------------------------------------------------------------------- | --------------------------------------------------------- | ----------------------------------------------------- |
| [algo/](algo)                                                         | Range algorithms                                          | [Readme](algo/#readme)                                |
| [app/](app)                                                           | Reserved namespace for applications                       | [Readme](app/#readme)                                 |
| [ascii/](ascii)                                                       | ASCII string functions                                    | [Readme](ascii/#readme)                               |
| [base64/](base64)                                                     | Base64 encoding and decoding                              | [Readme](base64/#readme)                              |
| [chr/](chr)                                                           | Character (`char`) functions                              | [Readme](chr/#readme)                                 |
| [cmp/](cmp)                                                           | Comparison functions                                      | [Readme](cmp/#readme)                                 |
| [country/](country)                                                   | Country codes and names                                   | [Readme](country/#readme)                             |
| [crypto/](crypto)                                                     | Cryptographic hashes and derivation functions             | [Readme](crypto/#readme)                              |
| [encoding/](encoding)                                                 | Encoding schemes                                          | [Readme](encoding/#readme)                            |
| [env/](env)                                                           | Environment variables and command line options            | [Readme](env/#readme)                                 |
| [file/](file)                                                         | File system                                               | [Readme](file/#readme)                                |
| [fmt/](fmt)                                                           | Formatting                                                | [Readme](fmt/#readme)                                 |
| [fn/](fn)                                                             | Function objects                                          | [Readme](fn/#readme)                                  |
| [generic/](generic)                                                   | Generic errors                                            | [Readme](generic/#readme)                             |
| [hex/](hex)                                                           | Hex encoding and decoding                                 | [Readme](hex/#readme)                                 |
| [html/](html)                                                         | HTML encoding                                             | [Readme](html/#readme)                                |
| [io/](io)                                                             | I/O concepts                                              | [Readme](io/#readme)                                  |
| [json/](json)                                                         | JSON encoding and decoding                                | [Readme](json/#readme)                                |
| [map/](map)                                                           | Sorted and unsorted maps                                  | [Readme](map/#readme)                                 |
| [math/](math)                                                         | Math functions                                            | [Readme](math/#readme)                                |
| [mem/](mem)                                                           | Allocators and memory functions                           | [Readme](mem/#readme)                                 |
| [num/](num)                                                           | Numerical classes                                         | [Readme](num/#readme)                                 |
| [pair/](pair)                                                         | Pair functions and classes                                | [Readme](pair/#readme)                                |
| [pcre/](pcre)                                                         | Perl Compatible Regular Expressions                       | [Readme](pcre/#readme)                                |
| [pool/](pool)                                                         | Pool containers                                           | [Readme](pool/#readme)                                |
| [process/](process)                                                   | Shell commands and process spawning                       | [Readme](process/#readme)                             |
| [random/](random)                                                     | High-quality random data                                  | [Readme](random/#readme)                              |
| [range/](range)                                                       | Ranges and range views (including `[c]strrng` aliases)    | [Readme](range/#readme)                               |
| [regex/](regex)                                                       | Regular expressions                                       | [Readme](regex/#readme)                               |
| [set/](set)                                                           | Sorted and unsorted sets                                  | [Readme](set/#readme)                                 |
| [stream/](stream)                                                     | Stream classes and concepts                               | [Readme](stream/#readme)                              |
| [string/](string)                                                     | String functions and ranges                               | [Readme](string/#readme)                              |
| [system/](system)                                                     | System error category and system functions                | [Readme](system/#readme)                              |
| [thread/](thread)                                                     | Thread functions                                          | [Readme](thread/#readme)                              |
| [time/](time)                                                         | Date and time (including IANA Time Zone Database)         | [Readme](time/#readme)                                |
| [unicode/](unicode)                                                   | Unicode constants and functions                           | [Readme](unicode/#readme)                             |
| [url/](url)                                                           | URL encoding                                              | [Readme](url/#readme)                                 |
| [utf8/](utf8)                                                         | UTF-8 functions and ranges                                | [Readme](utf8/#readme)                                |
| [array.hh](array.hh)                                                  | Aggregate array (always initialized)                      | [Example/Tests](array.test.cc)                        |
| [array\_view.fwd.hh](array_view.fwd.hh)                               | Array view (forward declare) and `[c]strview` aliases     |                                                       |
| [array\_view.hh](array_view.hh)                                       | Array view with `[c]strview` specializations              | [Example/Tests](array_view.test.cc)                   |
| [contiguous\_interface.hh](contiguous_interface.hh)                   | Contiguous interface                                      | [Example/Tests](contiguous_interface.test.cc)         |
| [core.hh](core.hh)                                                    | Core functionality                                        | [Example/Tests](core.test.cc)                         |
| [debug.hh](debug.hh)                                                  | Debug functions and macros                                | [Example/Tests](debug.test.cc)                        |
| [defer.hh](defer.hh)                                                  | Call a function on destruction                            | [Example/Tests](defer.test.cc)                        |
| [error\_code.hh](error_code.hh)                                       | Error category and error code                             | [Example/Tests](error_code.test.cc)                   |
| [exception.hh](exception.hh)                                          | Exception and `throw_or_abort(...)` function              |                                                       |
| [formatter.hh](formatter.hh)                                          | Formatter primary template                                |                                                       |
| [fuzz.hh](fuzz.hh)                                                    | Fuzzer entry point                                        |                                                       |
| [main.hh](main.hh)                                                    | Application entry point                                   |                                                       |
| [make\_range.hh](make_range.hh)                                       | Make range function                                       | [Example/Tests](make_range.test.cc)                   |
| [null\_term.hh](null_term.hh)                                         | Null-terminated non-null pointer wrapper                  | [Example/Tests](null_term.test.cc)                    |
| [optional.fwd.hh](optional.fwd.hh)                                    | Optional (forward declare)                                |                                                       |
| [optional.hh](optional.hh)                                            | Optional (`result` without error code)                    | [Example/Tests](optional.test.cc)                     |
| [optional\_index.hh](optional_index.hh)                               | Optional index                                            | [Example/Tests](optional_index.test.cc)               |
| [result.hh](result.hh)                                                | Result with a value/reference or an error code            | [Example/Tests](result.test.cc)                       |
| [size\_prefixed\_string\_literal.hh](size_prefixed_string_literal.hh) | Size prefixed string literal                              | [Example/Tests](size_prefixed_string_literal.test.cc) |
| [strcore.fwd.hh](strcore.fwd.hh)                                      | String (forward declare), concepts and `str[buf]` aliases |                                                       |
| [strcore.hh](strcore.hh)                                              | String (`str[buf]`) and `concat(...)` function            | [Example/Tests](strcore.test.cc)                      |
| [unittest.hh](unittest.hh)                                            | Unit test entry point and `snn_require` macros            |                                                       |
| [val\_or\_ref.hh](val_or_ref.hh)                                      | Reassignable value or reference                           | [Example/Tests](val_or_ref.test.cc)                   |
| [vec.hh](vec.hh)                                                      | Vector with optional small-capacity                       | [Example/Tests](vec.test.cc)                          |


## Build tool

The snncpp Go-like [build-tool][buildtool] can build C++ projects that follow the same naming convention and
directory structure as [snn-core][snncore]. It understands simple preprocessing directives ([example](crypto/hash/sha256.hh))
and will link with libraries listed in `#include` comments ([example](crypto/hash/impl/sha256.openssl.hh)).

The [build-tool][buildtool] executable is `snn` (by default), run it without any arguments to see
what commands are available:

```console
$ snn
Usage: snn <command> [arguments]

Commands:
build   Build one or more applications
gen     Generate a makefile for one or more applications
run     Build and run a single application with optional arguments
runall  Build and run one or more applications

For more information run a command without arguments, e.g.:
snn build
```

For example, to run all unit tests in the [pair/](pair) subdirectory:

```console
$ snn runall --verbose pair/*.test.cc
clang++ --config ./.clang -iquote ../ -c -o pair/common.test.o pair/common.test.cc
clang++ --config ./.clang -o pair/common.test pair/common.test.o -L/usr/local/lib/
clang++ --config ./.clang -iquote ../ -c -o pair/core.test.o pair/core.test.cc
clang++ --config ./.clang -o pair/core.test pair/core.test.o -L/usr/local/lib/
./pair/common.test
./pair/core.test
```

## Getting started

The [Getting started](https://github.com/snncpp/build-tool#getting-started) guide for the [build-tool][buildtool]
shows how to use [snn-core][snncore].


## Safety

### Overview

 * Ranges and views are preferred over iterators.
 * Views will not bind to temporaries by default.
 * String ranges (`c[strrng]`) make string parsing/validation safe and very efficient ([example](json/is_number.hh)).
 * Hidden undefined behavior (UB) is minimized, e.g. `.front()` returns an optional and
   `.front(promise::not_empty)` explicitly shows that we know that the container is not empty.
 * The use of operators like `*expr` (indirection/dereference), `expr->` (member access via pointer)
   and `expr[...]` (subscript) in user code should be rare.
 * No uninitialized containers (unless explicitly asked for).
 * `not_null` and `byte_size` wrappers for low level memory operations.
 * Consistent naming, e.g. `.size()` for byte size and `.count()` for element count.
 * `.size()` is only available when `sizeof(value_type) == 1`, `.byte_size()` is always available
   (on contiguous containers).
 * No silent narrowing, e.g. `.value_or(...)`.
 * Consistent brace initialization.

### Promises

Promise tags are used to:
 * Prevent misuse and differentiate constructors, e.g. `promise::has_capacity`, `promise::is_sorted`
   and `promise::null_terminated`.
 * Select a different overload, e.g. `.value()` throws and `.value(promise::has_value)` asserts.
 * Select a more performant overload, e.g. `promise::no_overlap`.
 * Bypass expensive checks, e.g. `promise::is_utf8`.

Promises are _not_ used when there is an implicit promise that can be checked with the
`snn_should()` macro, for example:
 * Wrapping a pointer with `not_null`.
 * Wrapping an integral with `not_zero`.
 * Wrapping functions, e.g. `ascii::as_lower()` and `json::stream::as_*()`.

Promises are _not_ used when the intent is clear and contained in a single statement, even if the
arguments can't be validated, for example:
 * Constructing an object `T` from a data pointer and a size `T{data, size}`.
 * Copying memory with `mem::raw::copy(...)`.
 * Reading a fixed byte count from a pointer with `mem::raw::load<Int>(...)`.

A promise is recommended when a static count is part of the type, e.g. `array_view<..., Count>`.
Here `T{data}` is error prone if the count isn't included in the statement, whereas
`T{data, promise::has_capacity}` is less so.

### Assertions

Compiling with the `snn_assert()` macro disabled is not recommended, especially for public
production builds.

If `NDEBUG` is defined as a macro name then `snn_assert()` does nothing (optimized build or not).
In non-optimized builds `snn_assert()` is another name for `assert()`.
In optimized builds `snn_assert()` simply calls `__builtin_trap()` if the condition is false.

In non-optimized builds `snn_should()` is another name for `snn_assert()`.
In optimized builds `snn_should()` does nothing.

Checked with `snn_assert()`:
 * Promises where the check isn't expensive or is easily optimized away.
   Example: `.at(index, promise::within_bounds)` or `.front(promise::not_empty)`.

Checked in non-optimized builds with `snn_should()`:
 * Promises where the check is expensive or not easily optimized away.
 * `not_null` & `not_zero` wrappers.
 * `as_*()` functions, where there is an implicit promise that the value is valid.

Never checked:
 * Iterator invalidation.
 * `array_view<T, ...>` invalidation.

Using sanitizers in development help to catch bugs that this library can't protect against. The
[build-tool][buildtool] has a `--sanitize` option that enables sanitizers.

### Internal audits

#### Assumption: Sane types

It is assumed that all types are "sane":
 * If a type is copy constructible it must also be move constructible.
 * If a type is copy assignable it must also be move assignable.
 * If a type is move constructible it must also be nothrow move constructible.
 * If a type is move assignable it must also be nothrow move assignable.

This can be checked with the `sane` concept, but is not enforced. The worst thing that can happen if
a type is not "sane" is that a function with a `noexcept` specifier throws and `std::terminate()` is
called.

#### Assumption: 57-bit virtual address space

It is assumed that the largest addressable memory block is always less than 128 PiB (57-bit virtual
address space). 128 PiB is less than `constant::limit<usize>::max / 100`.

This means that code like the following could never overflow:

```cpp
auto decode(const cstrview s)
{
    const usize decoded_size = s.size() * 4;
    ...
}
```

Code like the following could theoretically overflow:

```cpp
auto decode(const cstrview s)
{
    const usize decoded_size = s.size() * 200;
    ...
}
```

A comment that includes the string "57-bit-virtual-address-space" should be added everywhere this
assumption is made.


##  Naming conventions

All identifiers should be in `snake_case` with the following exceptions:
 * Template parameters should be in upper CamelCase (capitalized first letter).
   Example: `I`, `Int`, `UInt` or `UnsignedInt`.
 * Type declarations and constexpr variables that directly depend on template parameters can also
   be in CamelCase.
   Example: `using UInt = std::make_unsigned_t<Int>`.
 * Private/protected member functions and variables should have an underscore suffix.
   Example: `value_`, `names_` or `error_count_`.
 * If a reserved keyword can't be avoided or if it's simply the best name for an identifier, it
   must have an underscore prefix.
   Example: `fn::_not`, `html::element::type::_template` or `http::method::_delete`.
 * Function-like macros should be lowercase with "snn_" prefix.
   Example: `snn_assert()` or `snn_should()`.
 * Macro constants should be uppercase with "SNN_" prefix.
   Example: `SNN_ASSERT_BOOL` or `SNN_INT128_BOOL`.


## Documentation

Generated API documentation is planned. Until then the code itself (which is pretty readable outside
of the `detail` namespace) and examples/unit tests should hopefully be enough to use this library.


## License

See [LICENSE.md](LICENSE.md). Copyright © 2022 [Mikael Simonsson](https://mikaelsimonsson.com).


[buildtool]: https://github.com/snncpp/build-tool
[posix]: https://en.wikipedia.org/wiki/POSIX  "Portable Operating System Interface"
[snncore]: https://github.com/snncpp/snn-core
