include (../INCLUDE/adak.pri)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET      = $$qtLibraryTarget(QCommaEdit)

CONFIG      += plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QCommaEdit.cpp \
              QCommaEdit_Plugin.cpp

HEADERS    += ../INCLUDE/QCommaEdit.h \
              QCommaEdit_Plugin.h

