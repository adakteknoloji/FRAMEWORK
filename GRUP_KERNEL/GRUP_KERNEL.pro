include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET       = GRUP_KERNEL
TEMPLATE     = lib
CONFIG      += static

DEPENDPATH  += .

INCLUDEPATH += include ../INCLUDE ../KERNEL/

HEADERS     += ../INCLUDE/tree_kernel.h \
               ../INCLUDE/base_tree_kernel.h \
               ../INCLUDE/tree_secim_kernel.h \
               ../INCLUDE/base_grup_kernel.h \
               ../INCLUDE/grup_kernel.h \
               ../INCLUDE/grup_secim_kernel.h \
               ../INCLUDE/tree_arama_kernel.h \
    grup_kernel_grup_fisi.h

SOURCES     += tree_kernel.cpp \
               base_tree_kernel.cpp \
               tree_secim_kernel.cpp \
               base_grup_kernel.cpp \
               grup_kernel.cpp \
               grup_secim_kernel.cpp \
               tree_arama_kernel.cpp \
               adak_gruplar.cpp \
    grup_kernel_grup_fisi.cpp

FORMS +=       \
               grup_kernel.ui \
    grup_kernel_grup_fisi.ui
