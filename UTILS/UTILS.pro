include (../INCLUDE/adak.pri)

QT += network

INCLUDEPATH += include \
    ../INCLUDE \
    ../HLUSB/$$OBJDIR
TARGET = UTILS
TEMPLATE = lib
CONFIG += static
DEPENDPATH += . \
    include \
    src
SOURCES += dongle.cpp \
    base_sql.cpp \
    virgul_ekle.cpp \
    round.cpp \
    adak_program.cpp \
    adak_cursor.cpp \
    adak_array.cpp \
    yaziyla_yaz.cpp \
    mac_address.cpp \
    file_io.cpp \
    memory.cpp \
    module_names.cpp \
    adak_image.cpp \
    temp_file.cpp \
    mali_yil.cpp \
    adak_message.cpp \
    adak_font.cpp

HEADERS += ../INCLUDE/adak_utils.h \
    ../INCLUDE/adak_defines.h
