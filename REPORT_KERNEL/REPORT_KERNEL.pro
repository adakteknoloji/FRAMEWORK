include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET = REPORT_KERNEL
TEMPLATE = lib
CONFIG += static
DEPENDPATH += . 
INCLUDEPATH += ../INCLUDE .
SOURCES += report_shower.cpp \
           print_utils.cpp \
           etiket_kernel.cpp \
           report_kernel.cpp \
           html_tag.cpp
HEADERS += ../INCLUDE/print.h \
           ../INCLUDE/etiket_kernel.h \
           ../INCLUDE/etiket_sablonlari.h \
           ../INCLUDE/report_kernel.h \
           ../INCLUDE/html_tag.h \
           report_shower.h
