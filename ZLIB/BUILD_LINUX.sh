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
sed -i "2,2s/CFLAGS.*/CFLAGS=-O3 -s -fPIC -I..\/..\//" Makefile

make  clean all

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
