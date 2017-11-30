@echo off
set MSVC=1

echo %1 derleniyor

cd %1

qmake
nmake

echo %1 Derlendi

cd ..
