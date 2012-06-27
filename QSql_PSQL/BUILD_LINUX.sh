#!/bin/bash

# postgres-client ve libpq-dev kurulu olmali


qmake "INCLUDEPATH += /usr/include/postgresql/" "LIBS+=-L/usr/lib -lpq" psql.pro
make clean all
make install

