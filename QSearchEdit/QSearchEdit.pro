include (../INCLUDE/adak.pri)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET      = $$qtLibraryTarget(QSearchEdit)

CONFIG      += plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QSearchEdit.cpp \
              QSearchEdit_Plugin.cpp

HEADERS    += ../INCLUDE/QSearchEdit.h \
              QSearchEdit_Plugin.h

