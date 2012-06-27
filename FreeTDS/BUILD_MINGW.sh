#!/bin/bash

#  Cygwin/MSys shell icinden calistirilmali

set -e

tar -zxvf ../freetds-patched.tgz
cd freetds-0.91.dev.20120312

./configure --prefix=/c/FRAMEWORK/FreeTDS/MINGW/ --enable-msdblib --with-tdsver=7.1 
make
make install

cd ..
rm -r -f freetds-0.91.dev.20120312
rm -r -f share
