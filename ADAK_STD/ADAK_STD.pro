include (../INCLUDE/adak.pri)

QT += sql network webkit
TARGET = ADAK_STD
TEMPLATE = lib
CONFIG  += static

DEPENDPATH += . 
INCLUDEPATH += ../INCLUDE .
SOURCES += adak_std.cpp 

HEADERS += adak_std.h \
           ulkeler.h \
           iller.h \
           ilceler.h \
           cinsiyetler.h \
           ogrenim_durumlari.h \
           gsm_kodlari.h \
           dovizler.h \
           bankalar.h \
           banka_subeleri.h \
           vergi_daireleri.h \
           kan_gruplari.h \
           medeni_haller.h \
           sektorler.h \
           ../INCLUDE/adak_std_utils.h \
    meslekler.h \
    unvanlar.h
