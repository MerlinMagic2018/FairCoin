#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/faircoin-win64"

CONFIG_SITE="$PWD/depends/x86_64-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

x86_64-w64-mingw32-strip "src/faircoind.exe"
x86_64-w64-mingw32-strip "src/faircoin-tx.exe"
x86_64-w64-mingw32-strip "src/qt/faircoin-qt.exe"
x86_64-w64-mingw32-strip "src/faircoin-cli.exe"
x86_64-w64-mingw32-strip "src/.libs/libfaircoinconsensus-0.dll"

make install DESTDIR="${destdir}"
