include (../INCLUDE/adak.pri)

QT += sql \
    webkit
TARGET = BELGE_KERNEL
TEMPLATE = lib
CONFIG += static
DEPENDPATH += . 
INCLUDEPATH += . ../INCLUDE

SOURCES += belge_kernel.cpp \
           tasarim_degiskeni.cpp \
           belge_kernel_utils.cpp \
           dot_matrix.cpp \
           dot_matrix_sabitler.cpp


HEADERS += \
    dot_matrix.h \
    dot_matrix_sabitler.h \
    ../INCLUDE/belge_kernel_class.h \
    ../INCLUDE/tasarim_degiskeni.h  \
    ../INCLUDE/belge_kernel_open.h \
    ../INCLUDE/belge_kernel_utils.h \
    ../INCLUDE/belge_kernel_defines.h










