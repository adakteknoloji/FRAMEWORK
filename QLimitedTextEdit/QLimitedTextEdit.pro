include (../INCLUDE/adak.pri)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET      = $$qtLibraryTarget(QLimitedTextEdit)

CONFIG      += plugin

TEMPLATE    = lib

INCLUDEPATH += ../INCLUDE .


SOURCES    += QLimitedTextEdit.cpp \
              QLimitedTextEdit_Plugin.cpp

HEADERS    += ../INCLUDE/QLimitedTextEdit.h \
              QLimitedTextEdit_Plugin.h

