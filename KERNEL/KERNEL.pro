include (../INCLUDE/adak.pri)

QT          += sql network webkit xml

TARGET       = KERNEL
TEMPLATE     = lib
CONFIG      += static

DEPENDPATH  += .

RESOURCES += kernel_icons.qrc

INCLUDEPATH += include ../INCLUDE

HEADERS     += ../INCLUDE/fis_kernel.h \
               ../INCLUDE/form_kernel.h \
               ../INCLUDE/secim_kerneli.h \
               ../INCLUDE/arama_kernel.h \
               ../INCLUDE/batch_kernel.h \
               ../INCLUDE/kernel_utils.h \
               ../INCLUDE/base_kernel.h \
               ../INCLUDE/base_event_kernel.h \
               ../INCLUDE/kernel_private_utils.h \
               ../INCLUDE/abstract_kernel.h \
               ../INCLUDE/sekme_menu_kernel.h \
               ../INCLUDE/QAdakTabWidget.h

SOURCES     += fis_kernel.cpp \
               form_kernel.cpp \
               secim_kerneli.cpp \
               arama_kernel.cpp \
               batch_kernel.cpp \
               kernel_utils.cpp \
               base_kernel.cpp \
               base_event_kernel.cpp \
               kernel_private_utils.cpp \
               abstract_kernel.cpp \
               sekme_menu_kernel.cpp \
               QAdakTabWidget.cpp
