@echo off

echo %1 derleniyor

cd %1

qmake
nmake

echo %1 Derlendi

cd ..
