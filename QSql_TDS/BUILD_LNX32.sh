#!/bin/bash

qmake "INCLUDEPATH+=/ADAK/FRAMEWORK/FreeTDS/LNX32/include" "LIBS=-L/ADAK/FRAMEWORK/FreeTDS/LNX32/lib -lsybdb" tds.pro

make clean all
make install
