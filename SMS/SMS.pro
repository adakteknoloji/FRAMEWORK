include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TEMPLATE     = lib
CONFIG      += static
TARGET       = SMS

INCLUDEPATH += ../INCLUDE SERVERS .

SOURCES     += sms_sender.cpp sms_sender_gui.cpp sms_server_abstract.cpp SERVERS/clickatell.cpp \
    SERVERS/sms_servers.cpp

HEADERS     += ../INCLUDE/sms.h sms_sender_gui.h sms_sender.h ../INCLUDE/sms_server_abstract.h SERVERS/clickatell.h \
    ../INCLUDE/sms_servers.h

FORMS       += sms_sender_gui.ui
