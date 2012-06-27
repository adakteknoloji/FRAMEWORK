include (../INCLUDE/adak.pri)

TARGET      = $$qtLibraryTarget(QCommaEdit)

CONFIG      += designer plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QCommaEdit.cpp \
              QCommaEdit_Plugin.cpp

HEADERS    += ../INCLUDE/QCommaEdit.h \
              QCommaEdit_Plugin.h

