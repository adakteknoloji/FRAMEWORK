@echo off

cd zlib-1.2.7

nmake -f win32\Makefile.msc 
cd contrib\minizip

cl /I..\..\ -c -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -nologo -MD -W3 -O2 -Oy- -Zi -Fd"zlib"  *.c

cd ..\..


copy adler32.obj ..
copy crc32.obj ..
copy deflate.obj ..
copy gzlib.obj ..
copy gzwrite.obj ..
copy inffast.obj ..
copy inflate.obj ..
copy inftrees.obj ..
copy trees.obj ..
copy zutil.obj ..

copy contrib\minizip\ioapi.obj ..
copy contrib\minizip\iowin32.obj ..
copy contrib\minizip\unzip.obj ..
copy contrib\minizip\zip.obj ..

cd ..
