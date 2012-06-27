#!/bin/bash

qmake "INCLUDEPATH+=/ADAK/FRAMEWORK/FreeTDS/LNX64/include" "LIBS=-L/ADAK/FRAMEWORK/FreeTDS/LNX64/lib -lsybdb" tds.pro

make clean all
make install
