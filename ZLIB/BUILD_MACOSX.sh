#!/bin/bash

set -e

rm -r -f zlib*
tar -jxvf ../zlib*.tar.bz2

cd zlib*

export CFLAGS="-O3 -s -fPIC"
export CXFLAGS="-O3 -s -fPIC"

./configure --static

make clean all


cd contrib/minizip

gcc -O3 -s -fPIC  -DHAVE_HIDDEN   -I../.. -c -o ioapi.o ioapi.c
gcc -O3 -s -fPIC  -DHAVE_HIDDEN   -I../.. -c -o zip.o   zip.c
gcc -O3 -s -fPIC  -DHAVE_HIDDEN   -I../.. -c -o unzip.o unzip.c

cd ../..


cp -f adler32.o ..
cp -f crc32.o ..
cp -f deflate.o ..
cp -f gzlib.o ..
cp -f gzwrite.o ..
cp -f inffast.o ..
cp -f inflate.o ..
cp -f inftrees.o ..
cp -f trees.o ..
cp -f zutil.o ..

cp -f contrib/minizip/ioapi.o ..
cp -f contrib/minizip/unzip.o ..
cp -f contrib/minizip/zip.o ..

cd ..
