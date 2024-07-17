#!/bin/bash

prefix="/usr/local"
version="1.0.1"
destdir="/bitcoin/dist/faircoin-${version}-linux"

CONFIG_SITE="$PWD/depends/x86_64-pc-linux-gnu/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make clean && make -j4

strip "src/faircoind"
strip "src/faircoin-tx"
strip "src/qt/faircoin-qt"
strip "src/faircoin-cli"
strip "src/.libs/libfaircoinconsensus.so"

make install DESTDIR="${destdir}"
