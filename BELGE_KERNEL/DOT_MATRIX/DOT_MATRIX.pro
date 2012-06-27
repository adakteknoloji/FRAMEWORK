include (../adak.pri)

QT       += core gui

TARGET = DOT_MATRIX
TEMPLATE = app


SOURCES += main.cpp\
        printer_test.cpp    \
        dot_matrix.cpp

HEADERS  += printer_test.h  \
            dot_matrix.h \
            dot_matrix_sabitler.h   \

FORMS    += printer_test.ui


OBJECTS_DIR = moc
MOC_DIR     = moc
RCC_DIR     = moc
UI_DIR      = moc



