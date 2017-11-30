include (../INCLUDE/adak.pri)
qtHaveModule(webview)

QT        += xml sql network widgets printsupport uiplugin webenginewidgets

TEMPLATE = app
TARGET = ADAK_DESIGNER
INCLUDEPATH += ../INCLUDE .
DEPENDPATH += . 

LIBS += ../$$OBJDIR/$$libADAK
PRE_TARGETDEPS += ../$$OBJDIR/$$libADAK

SOURCES += designer_ana_menu.cpp \
    designer_main.cpp \
    designer_hakkinda_batch.cpp \
    designer_yetkilendirme_formu.cpp \
    sql_tablolar_fisi.cpp \
    sql_tablo_arama.cpp \
    sql_designer_utils.cpp \
    sql_index_batch.cpp \
    sql_struct_generator.cpp \
    belge_designer_fisi.cpp \
    belge_secimi.cpp \
    belge_struct_generator.cpp \
    sql_veritabani_ayarlari_batch.cpp

HEADERS += designer_ana_menu.h \
    designer.h \
    designer_hakkinda_batch.h \
    designer_yetkilendirme_formu.h \
    sql_tablolar_fisi.h \
    sql_designer.h \
    sql_tablo_arama.h \
    sql_index_batch.h \
    sql_struct_generator.h \
    belge.h \
    belge_designer.h \
    belge_designer_fisi.h \
    belge_secimi.h \
    belge_struct_generator.h \
    belge_struct.h \
    sql_veritabani_ayarlari_batch.h


FORMS += designer_ana_menu.ui \
    designer_hakkinda_batch.ui \
    designer_yetkilendirme_formu.ui \
    sql_tablolar_fisi.ui \
    sql_index_batch.ui \
    sql_tablo_arama.ui \
    belge_designer_fisi.ui \
    sql_veritabani_ayarlari_batch.ui

RESOURCES += designer_icons.qrc

win32:LIBS += -liphlpapi
