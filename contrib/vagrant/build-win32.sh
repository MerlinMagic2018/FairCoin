#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/faircoin-win32"

CONFIG_SITE="$PWD/depends/i686-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

i686-w64-mingw32-strip "src/faircoind.exe"
i686-w64-mingw32-strip "src/faircoin-tx.exe"
i686-w64-mingw32-strip "src/qt/faircoin-qt.exe"
i686-w64-mingw32-strip "src/faircoin-cli.exe"
i686-w64-mingw32-strip "src/.libs/libfaircoinconsensus-0.dll"

make install DESTDIR="${destdir}"
