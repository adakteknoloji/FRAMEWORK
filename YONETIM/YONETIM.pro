include (../INCLUDE/adak.pri)

QT += sql \
    network \
    webkit \
    xml
TARGET = YONETIM
TEMPLATE = lib
CONFIG += static
DEPENDPATH += .
INCLUDEPATH += ../INCLUDE .
SOURCES += program_secimi.cpp \
    bilgisayarlar_formu.cpp \
    bilgisayar_secimi.cpp \
    veritabanlari_formu.cpp \
    kullanicilar_formu.cpp \
    kullanici_secimi.cpp \
    yonetim_007_arama.cpp \
    yonetim_utils.cpp \
    sifre_degistirme_batch.cpp \
    posta_sunucusu_formu.cpp \
    sms_sunucusu_formu.cpp \
    kullanici_degistir_batch.cpp \
    yonetim_hakkinda.cpp \
    hata_istek_batch.cpp 

HEADERS += program_secimi.h \
    bilgisayarlar_formu.h \
    bilgisayar_secimi.h \
    veritabanlari_formu.h \
    kullanicilar_formu.h \
    kullanici_secimi.h \
    ../INCLUDE/yonetim.h \
    ../INCLUDE/login.h \
    ../INCLUDE/login_enum.h \
    yonetim_007_arama.h \
    yonetim_utils.h \
    sifre_degistirme_batch.h \
    posta_sunucusu_formu.h \
    sms_sunucusu_formu.h \
    kullanici_degistir_batch.h \
    yonetim_hakkinda.h \
    hata_istek_batch.h

FORMS += bilgisayarlar_formu.ui \
    veritabanlari_formu.ui \
    kullanicilar_formu.ui \
    yonetim_007_arama.ui \
    sifre_degistirme_batch.ui \
    posta_sunucusu_formu.ui \
    sms_sunucusu_formu.ui \
    kullanici_degistir_batch.ui \
    yonetim_hakkinda.ui \
    hata_istek_batch.ui

RESOURCES += yonetim_icons.qrc



