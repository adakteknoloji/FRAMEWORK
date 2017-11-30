@echo off

echo %1 derleniyor

cd %1

qmake
mingw32-make -j4

echo %1 Derlendi

cd ..
