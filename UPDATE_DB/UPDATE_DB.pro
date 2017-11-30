include (../INCLUDE/adak.pri)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets
QT        -= gui

TARGET      = UPDATE_DB
CONFIG     += console
TEMPLATE    = app

SOURCES    += update_db_main.cpp update_db.cpp

INCLUDEPATH+= ../INCLUDE .
HEADERS    += update_db.h    adak_sql_struct.h

LIBS           += ../$$OBJDIR/$$libADAK
PRE_TARGETDEPS += ../$$OBJDIR/$$libADAK

win32:LIBS+= -liphlpapi
