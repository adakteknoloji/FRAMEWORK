#!/bin/bash

#mysql-5.5.25-osx10.6-x86_64.tar dosyasi /ADAK dizinine acilmis olmali

qmake "INCLUDEPATH+=/ADAK/mysql-5.5.25-osx10.6-x86_64/include/" "LIBS+=-L/ADAK/mysql-5.5.25-osx10.6-x86_64/lib" "LIBS+=-lmysqlclient_r" mysql.pro
make clean all
make install
