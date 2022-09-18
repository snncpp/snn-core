# IANA Time Zone Database

## Build "fat" zoneinfo

Don't use a path with spaces or special characters in it.

    % cd /tmp
    % fetch 'https://data.iana.org/time-zones/tzdb-latest.tar.lz'
    % tar -xvf tzdb-latest.tar.lz

    x tzdb-2022a/
    x tzdb-2022a/...
    x tzdb-2022a/...

    % cd tzdb-2022a
    % vi Makefile

Replace:

    BACKWARD= backward
    CFLAGS=

With:

    BACKWARD=
    CFLAGS= -DZIC_BLOAT_DEFAULT=\"fat\"

Use GNU Awk (FreeBSD only, gawk must be installed), replace:

    AWK= awk

With:

    AWK= gawk

Make:

    % mkdir build
    % make TOPDIR=$(realpath build) install

## Generate header files

    % cd ~/project/cpp/snn-core/time/zone/db/detail
    % snn build db.gen.cc
    % ./db.gen 2022a /tmp/tzdb-2022a/build/usr/share/zoneinfo

