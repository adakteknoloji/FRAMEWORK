include (../INCLUDE/adak.pri)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET      = $$qtLibraryTarget(QAdakDate)

CONFIG      += plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QAdakDate.cpp \
              QAdakDate_Plugin.cpp

HEADERS    += ../INCLUDE/QAdakDate.h \
              QAdakDate_Plugin.h 
