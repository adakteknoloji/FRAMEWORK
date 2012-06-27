#!/bin/bash

qmake "INCLUDEPATH+=/ADAK/FRAMEWORK/FreeTDS/MACOSX/include" "LIBS=-L/ADAK/FRAMEWORK/FreeTDS/MACOSX/lib -lsybdb" tds.pro

make clean all
make install
