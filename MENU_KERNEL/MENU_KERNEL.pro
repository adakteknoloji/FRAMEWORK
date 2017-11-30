include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET       = MENU_KERNEL
TEMPLATE     = lib
CONFIG      += static

DEPENDPATH  += .

INCLUDEPATH += ../INCLUDE

HEADERS     += ../INCLUDE/menu_kernel.h \
               ../INCLUDE/abstract_kernel.h

SOURCES     += menu_kernel.cpp
