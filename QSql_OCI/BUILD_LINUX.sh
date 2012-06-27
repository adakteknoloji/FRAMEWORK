#!/bin/bash

set -e

qmake "INCLUDEPATH+=/opt/instantclient_11_2/sdk/include" "LIBS+= /opt/instantclient_11_2/libclntsh.so.11.1 -L/opt/instantclient_11_2 -lnnz11" oci.pro

make clean all
make install
