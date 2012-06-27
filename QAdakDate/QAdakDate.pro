include (../INCLUDE/adak.pri)

TARGET      = $$qtLibraryTarget(QAdakDate)

CONFIG      += designer plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QAdakDate.cpp \
              QAdakDate_Plugin.cpp

HEADERS    += ../INCLUDE/QAdakDate.h \
              QAdakDate_Plugin.h 
