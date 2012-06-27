#!/bin/bash

set -e

qmake "INCLUDEPATH+=/ADAK/instantclient_10_2/sdk/include" "LIBS+=-L/ADAK/instantclient_10_2" oci.pro

make clean all
make install
