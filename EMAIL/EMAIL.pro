include (../INCLUDE/adak.pri)

QT          += sql network webkit

TARGET       = EMAIL
TEMPLATE     = lib
CONFIG      += static

DEPENDPATH  += .

INCLUDEPATH += ../INCLUDE .

SOURCES     += email_sender.cpp \
               email_sender_gui.cpp

HEADERS     += email_sender.h \
               email_sender_gui.h

FORMS       += email_sender_gui.ui
RESOURCES   += email_icons.qrc
