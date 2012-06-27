include (../INCLUDE/adak.pri)

TARGET      = $$qtLibraryTarget(QSearchEdit)

CONFIG      += designer plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QSearchEdit.cpp \
              QSearchEdit_Plugin.cpp

HEADERS    += ../INCLUDE/QSearchEdit.h \
              QSearchEdit_Plugin.h

