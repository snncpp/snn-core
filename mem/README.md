# Allocators and memory functions

## Overview

| Path                                          | Description                                                           |                                    |
| --------------------------------------------- | --------------------------------------------------------------------- | ---------------------------------- |
| [raw/](raw)                                   | Raw memory functions                                                  | [Readme](raw/#readme)              |
| [allocator.hh](allocator.hh)                  | Allocator without state                                               | [Tests](allocator.test.cc)         |
| [construct.hh](construct.hh)                  | Construct a single object at a given address                          |                                    |
| [copy\_construct.hh](copy_construct.hh)       | Copy objects to an uninitialized address                              |                                    |
| [destruct.hh](destruct.hh)                    | Destruct object(s) at a given address                                 |                                    |
| [destruct\_n.hh](destruct_n.hh)               | Destruct N objects at a given address                                 |                                    |
| [move\_construct.hh](move_construct.hh)       | Move objects to an uninitialized address                              |                                    |
| [relocate.hh](relocate.hh)                    | Relocate objects to an uninitialized address                          |                                    |
| [relocate\_backward.hh](relocate_backward.hh) | Relocate objects backward to an uninitialized address                 |                                    |
| [relocate\_left.hh](relocate_left.hh)         | Relocate objects to an uninitialized address N positions to the left  |                                    |
| [relocate\_right.hh](relocate_right.hh)       | Relocate objects to an uninitialized address N positions to the right |                                    |
| [trivial\_allocator.hh](trivial_allocator.hh) | Allocator without state for trivial types                             | [Tests](trivial_allocator.test.cc) |
