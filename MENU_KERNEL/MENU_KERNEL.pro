include (../INCLUDE/adak.pri)

QT          += sql network webkit xml

TARGET       = MENU_KERNEL
TEMPLATE     = lib
CONFIG      += static

DEPENDPATH  += .

INCLUDEPATH += ../INCLUDE

HEADERS     += ../INCLUDE/menu_kernel.h \
               ../INCLUDE/abstract_kernel.h

SOURCES     += menu_kernel.cpp
