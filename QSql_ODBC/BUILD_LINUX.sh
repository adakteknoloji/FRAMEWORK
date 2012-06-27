#!/bin/bash

# unixodbc-dev kurulu olmali
qmake
make clean all
make install
