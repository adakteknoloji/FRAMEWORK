include (../INCLUDE/adak.pri)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TARGET = ODS_IO
TEMPLATE = lib
CONFIG += static
INCLUDEPATH += . ../INCLUDE ../ZLIB/INCLUDE

SOURCES += \
    adak_importer.cpp \
    adak_exporter.cpp \
    export_kernel.cpp \
    import_kernel.cpp

HEADERS  += adak_importer.h \
            adak_exporter.h \
            ../INCLUDE/export_kernel.h \
            ../INCLUDE/import_kernel.h

FORMS    += import_kernel.ui


RESOURCES += \
    ods_io.qrc
