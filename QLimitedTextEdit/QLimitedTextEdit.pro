include (../INCLUDE/adak.pri)

TARGET      = $$qtLibraryTarget(QLimitedTextEdit)

CONFIG      += designer plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QLimitedTextEdit.cpp \
              QLimitedTextEdit_Plugin.cpp

HEADERS    += ../INCLUDE/QLimitedTextEdit.h \
              QLimitedTextEdit_Plugin.h

