@echo off

echo %1 derleniyor

cd %1

qmake
mingw32-make

echo %1 Derlendi

cd ..
