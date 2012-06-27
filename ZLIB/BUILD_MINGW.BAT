@echo off

cd zlib-1.2.7

mingw32-make -f win32\Makefile.gcc 
cd contrib\minizip

mingw32-gcc  -I..\..\ -O3 -Wall -o ioapi.o   -c  ioapi.c
mingw32-gcc  -I..\..\ -O3 -Wall -o iowin32.o -c  iowin32.c
mingw32-gcc  -I..\..\ -O3 -Wall -o unzip.o   -c  unzip.c
mingw32-gcc  -I..\..\ -O3 -Wall -o zip.o     -c  zip.c

cd ..\..


copy adler32.o ..
copy crc32.o ..
copy deflate.o ..
copy gzlib.o ..
copy gzwrite.o ..
copy inffast.o ..
copy inflate.o ..
copy inftrees.o ..
copy trees.o ..
copy zutil.o ..

copy contrib\minizip\ioapi.o ..
copy contrib\minizip\iowin32.o ..
copy contrib\minizip\unzip.o ..
copy contrib\minizip\zip.o ..

cd ..
