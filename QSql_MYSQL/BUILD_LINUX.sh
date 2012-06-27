#!/bin/bash

#libmysqlclient-dev kurulu olmali

qmake "INCLUDEPATH+=/usr/include/mysql/" "LIBS+=-lmysqlclient_r" mysql.pro
make clean all
make install
