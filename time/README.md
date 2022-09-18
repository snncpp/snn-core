# Date and time (including IANA Time Zone Database)

## Overview

| Path                                           | Description                                  |                                             |
| ---------------------------------------------- | -------------------------------------------- | ------------------------------------------- |
| [steady/](steady)                              | Steady (monotonic) time                      | [Readme](steady#readme)                     |
| [wall/](wall)                                  | Wall time                                    | [Readme](wall#readme)                       |
| [zone/](zone)                                  | Time zone classes and IANA database          | [Readme](zone#readme)                       |
| [core.hh](core.hh)                             | Core constants, functions and structures     | [Example/Tests](core.test.cc)               |
| [duration.formatter.hh](duration.formatter.hh) | Duration formatter                           | [Example/Tests](duration.formatter.test.cc) |
| [duration.hh](duration.hh)                     | Duration                                     | [Example/Tests](duration.test.cc)           |
| [error.hh](error.hh)                           | Error (enum etc)                             |                                             |
| [limit.hh](limit.hh)                           | Limit                                        | [Example/Tests](limit.test.cc)              |
| [parse.hh](parse.hh)                           | Parse functions                              | [Example/Tests](parse.test.cc)              |
| [point.hh](point.hh)                           | Time point and `now()` function              | [Example/Tests](point.test.cc)              |
| [stopwatch.hh](stopwatch.hh)                   | Stopwatch                                    | [Example/Tests](stopwatch.test.cc)          |
| [unit.fwd.hh](unit.fwd.hh)                     | Unit (forward declare), aliases and concepts |                                             |
| [unit.hh](unit.hh)                             | Unit                                         | [Example/Tests](unit.test.cc)               |


## Time point

### Format syntax

    Character(s)    Description

    aa              Ante meridiem (am) or post meridiem (pm), abbreviated, lower case.
                    "am" or "pm".
    AA              Ante meridiem (AM) or post meridiem (PM), abbreviated, upper case.
                    "AM" or "PM".
    b
    c               ISO 8601/RFC 3339 (without fraction).
                    "2001-02-03T04:05:06+07:00"
    d               Day of the month.
                    "1" to "31".
    dd              Day of the month, zero padded.
                    "01" to "31".
    DD              Day of the month, space padded.
                    " 1" to "31".
    e               Day of the week (single digit)
                    "0" (Sunday) to "6" (Saturday).
    eee             Day of the week (abbreviated name).
                    "Sun" (Sunday) to "Sat" (Saturday).
    f               Full date.
                    "2001-02-03"
    g               Hour (12-hour clock).
                    "1" to "12".
    gg              Hour (12-hour clock), zero padded.
                    "01" to "12".
    h               Hour (24-hour clock).
                    "0" to "23".
    hh              Hour (24-hour clock), zero padded.
                    "00" to "23".
    i               Minute
                    "0" to "59".
    ii              Minute, zero padded.
                    "00" to "59".
    j
    k
    l               RFC 1123 (time zone abbreviation is always "GMT").
                    "Sat, 03 Feb 2001 04:05:06 GMT"
    m               Month
                    "1" to "12".
    mm              Month, zero padded.
                    "01" to "12".
    mmm             Month (abbreviated name).
                    "Jan" to "Dec".
    n               Nanosecond fraction, 1-digit, fixed. (Tenths of a second.)
    nn              Nanosecond fraction, 2-digit, fixed. (Hundredths of a second.)
    nnn...
    nnnnnnnnn       Nanosecond fraction, 9-digit, fixed.
    N               Nanosecond fraction, 1-digit, trimmed. (Tenths of a second.)
    NN              Nanosecond fraction, 2-digit, trimmed. (Hundredths of a second.)
    NNN...
    NNNNNNNNN       Nanosecond fraction, 9-digit, trimmed.
    o               Time offset in seconds.
                    "0", "3600", "-28800" etc.
    oooo            Time offset in zero padded +/-hours-minutes.
                    "+0700"
    ooooo           Time offset in zero padded +/-hours-colon-minutes.
                    "+07:00"
    OOO             Time zone abbreviation.
                    "GMT", "CEST" etc.
    p
    q               Quick/readable with optional fraction.
                    "2001-02-03 04:05:06 +0700" or "2001-02-03 04:05:06.809 +0700".
    r               RFC 2822
                    "Sat, 03 Feb 2001 04:05:06 +0700"
    s               Second
                    "0" to "59".
    ss              Second, zero padded.
                    "00" to "59".
    t               Full 24-hour time.
                    "15:30:40"
    u
    v               Is daylight saving time.
                    "0" or "1".
    w
    x
    yyyy            Year, 4-digit (minimum), zero padded.
    z               ISO 8601/RFC 3339 with 'Z' timezone (without fraction).
    .               Is only special if followed by an optional fraction N, in which case it will be
                    removed if the fraction is zero.

### Reserved format syntax

 * Uppercase versions of gg, hh, ii, mm, ss etc. are reserved for space padding.
 * w is reserved for week number.
