# File system

## Overview

| Path                                  | Description                                                |                                |
| ------------------------------------- | ---------------------------------------------------------- | ------------------------------ |
| [dir/](dir)                           | Directories                                                | [Readme](dir/#readme)          |
| [io/](io)                             | I/O                                                        | [Readme](io/#readme)           |
| [path/](path)                         | Paths                                                      | [Readme](path/#readme)         |
| [permission/](permission)             | Permissions                                                | [Readme](permission/#readme)   |
| [standard/](standard)                 | Standard streams                                           | [Readme](standard/#readme)     |
| [symlink/](symlink)                   | Symbolic links                                             | [Readme](symlink/#readme)      |
| [tty/](tty)                           | TTYs (terminals)                                           | [Readme](tty/#readme)          |
| [append.hh](append.hh)                | Append to file                                             | [Tests](append.test.cc)        |
| [config.hh](config.hh)                | Config file parser                                         | [Tests](config.test.cc)        |
| [copy.hh](copy.hh)                    | Copy a file                                                | [Tests](copy.test.cc)          |
| [descriptor.hh](descriptor.hh)        | File descriptor wrapper                                    | [Tests](descriptor.test.cc)    |
| [error.hh](error.hh)                  | Error (enum etc)                                           |                                |
| [info.hh](info.hh)                    | File information                                           | [Tests](info.test.cc)          |
| [is\_directory.hh](is_directory.hh)   | Is directory                                               | [Tests](is_directory.test.cc)  |
| [is\_regular.hh](is_regular.hh)       | Is regular file                                            | [Tests](is_regular.test.cc)    |
| [is\_same.hh](is_same.hh)             | Check if two paths point to the same file                  | [Tests](is_same.test.cc)       |
| [is\_something.hh](is_something.hh)   | Is something                                               | [Tests](is_something.test.cc)  |
| [offset.hh](offset.hh)                | Get current file descriptor offset                         | [Tests](offset.test.cc)        |
| [option.hh](option.hh)                | Option flags (enum)                                        | [Tests](option.test.cc)        |
| [perm.hh](perm.hh)                    | Permission bits (enum)                                     | [Tests](perm.test.cc)          |
| [pointer.hh](pointer.hh)              | File pointer wrapper                                       | [Tests](pointer.test.cc)       |
| [read.hh](read.hh)                    | Read an entire regular file                                | [Tests](read.test.cc)          |
| [reader\_writer.hh](reader_writer.hh) | Open and read from or write to a file                      | [Tests](reader_writer.test.cc) |
| [remove.hh](remove.hh)                | Remove directory entry                                     | [Tests](remove.test.cc)        |
| [rename.hh](rename.hh)                | Rename directory entry                                     | [Tests](rename.test.cc)        |
| [seek.hh](seek.hh)                    | Seek in file (reposition read/write file offset)           | [Tests](seek.test.cc)          |
| [status.hh](status.hh)                | Get information about a file path or a file descriptor     | [Tests](status.test.cc)        |
| [touch.hh](touch.hh)                  | Touch a regular file                                       | [Tests](touch.test.cc)         |
| [truncate.hh](truncate.hh)            | Truncate/extend file to a specified size (zero by default) | [Tests](truncate.test.cc)      |
| [type.hh](type.hh)                    | Type (enum)                                                | [Tests](type.test.cc)          |
| [write.hh](write.hh)                  | Write to file                                              | [Tests](write.test.cc)         |
