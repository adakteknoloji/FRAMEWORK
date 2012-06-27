include (../INCLUDE/adak.pri)

QT += sql \
    network \
    webkit \
    xml
TARGET = LOGIN
TEMPLATE = lib
CONFIG += static
DEPENDPATH += .
INCLUDEPATH += ../INCLUDE .
SOURCES += \
    login_batch.cpp \
    sql_sunucu_secimi.cpp    \
    programi_guncellestirin_batch.cpp    \
    yardim_hatirlatma_dialog.cpp \
    first_login.cpp \
    login_utils.cpp \
    program_start.cpp   \
    adak_sql_error_dialog.cpp  \
    veritabani_secimi.cpp \
    yonetici_izni_batch.cpp \
    yeni_sunucu_tanimlama_batch.cpp

HEADERS += yonetim_db.h \
    ../INCLUDE/login.h \
    ../INCLUDE/login_enum.h \
    login_batch.h \
    sql_sunucu_secimi.h \
    programi_guncellestirin_batch.h \
    yardim_hatirlatma_dialog.h \
    first_login.h \
    login_utils.h \
    program_start.h \
    adak_sql_error_dialog.h \
    veritabani_secimi.h \
    yonetici_izni_batch.h \
    yeni_sunucu_tanimlama_batch.h

FORMS += \
    login_batch.ui \
    yardim_hatirlatma_dialog.ui \
    programi_guncellestirin_batch.ui \
    adak_sql_error_dialog.ui \
    yonetici_izni_batch.ui \
    yeni_sunucu_tanimlama_batch.ui

RESOURCES +=
