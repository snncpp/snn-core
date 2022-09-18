# Range views

## Overview

| Path                          | Description                     |                                    |
| ----------------------------- | ------------------------------- | ---------------------------------- |
| [by\_value.hh](by_value.hh)   | Return by-value                 | [Example/Tests](by_value.test.cc)  |
| [cache.hh](cache.hh)          | Cache front element             | [Example/Tests](cache.test.cc)     |
| [chunk.hh](chunk.hh)          | Split with N elements per chunk | [Example/Tests](chunk.test.cc)     |
| [drop.hh](drop.hh)            | Drop N elements from the front  | [Example/Tests](drop.test.cc)      |
| [drop\_last.hh](drop_last.hh) | Drop N elements from the back   | [Example/Tests](drop_last.test.cc) |
| [element.hh](element.hh)      | Return `get<Index>(...)`        | [Example/Tests](element.test.cc)   |
| [enumerate.hh](enumerate.hh)  | Pair element with its index     | [Example/Tests](enumerate.test.cc) |
| [filter.hh](filter.hh)        | Filter with predicate           | [Example/Tests](filter.test.cc)    |
| [invoke.hh](invoke.hh)        | Return `std::invoke(op, ...)`   | [Example/Tests](invoke.test.cc)    |
| [move.hh](move.hh)            | Return `std::move(...)`         | [Example/Tests](move.test.cc)      |
| [pair.hh](pair.hh)            | Pair two ranges                 | [Example/Tests](pair.test.cc)      |
| [reverse.hh](reverse.hh)      | Reverse a range                 | [Example/Tests](reverse.test.cc)   |
| [take.hh](take.hh)            | Take N elements from the front  | [Example/Tests](take.test.cc)      |
| [to.hh](to.hh)                | Return `.to<To>()` or `To{...}` | [Example/Tests](to.test.cc)        |
| [to\_prefix.hh](to_prefix.hh) | Return `.to_prefix<To>()`       | [Example/Tests](to_prefix.test.cc) |
| [transform.hh](transform.hh)  | Return `op(...)`                | [Example/Tests](transform.test.cc) |
| [value.hh](value.hh)          | Return `.value()`               | [Example/Tests](value.test.cc)     |
