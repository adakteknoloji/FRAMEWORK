include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET = TASARIMCI
TEMPLATE = lib
CONFIG += static
DEPENDPATH += . 
INCLUDEPATH += . ../INCLUDE

SOURCES += tasarim_design_widget.cpp \
    tasarim_secimi.cpp \
    tasarim_utils.cpp \
    tasarim_batch_base.cpp \
    tasarim_menu_batch.cpp  \
    tasarim_belge_batch.cpp \
    tasarim_yeni_tasarim_batch.cpp  \
    tasarim_belge_degiskenleri_secimi.cpp \
    tasarim_export.cpp \
    tasarim_import.cpp \
    tasarim_batch.cpp


HEADERS += \
    tasarim_design_widget.h \
    tasarim_secimi.h \
    tasarim_belge_secimi.h \
    tasarim.h \
    ../INCLUDE/tasarim_defines.h \
    ../INCLUDE/belge_kernel_utils.h \
    ../INCLUDE/tasarim_utils.h \
    ../INCLUDE/belge_kernel_open.h \
    ../INCLUDE/tasarim_degiskeni.h \
    tasarim_batch_base.h \
    tasarim_menu_batch.h    \
    tasarim_belge_batch.h \
    tasarim_yeni_tasarim_batch.h    \
    tasarim_belge_degiskenleri_secimi.h \
    tasarim_export.h \
    tasarim_import.h \
    tasarim_batch.h


RESOURCES += tasarimci_icons.qrc

FORMS += \
    tasarim_menu_batch.ui   \
    tasarim_belge_batch.ui \
    tasarim_yeni_tasarim_batch.ui \
    tasarim_batch.ui


















