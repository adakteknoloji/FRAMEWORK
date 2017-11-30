include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

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
