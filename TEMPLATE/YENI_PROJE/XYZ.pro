include (/ADAK/FRAMEWORK/INCLUDE/adak.pri)

QT += sql network webkit
TEMPLATE = app
TARGET   = XYZ
DEPENDPATH += .
INCLUDEPATH += . /ADAK/FRAMEWORK/INCLUDE
HEADERS   += xyz.h \
             xyz_ana_menu_class.h \
             xyz_ana_menu_open.h
FORMS     += xyz_ana_menu.ui
SOURCES   += xyz_main.cpp \
             xyz_ana_menu.cpp
RESOURCES += xyz_icons.qrc

LIBS +=  /ADAK/FRAMEWORK/$$OBJDIR/libADAK.a
PRE_TARGETDEPS += /ADAK/FRAMEWORK/$$OBJDIR/libADAK.a

win32:LIBS += -L"C:/Program Files/MySQL/MySQL Server 5.5/lib" \
    -L"C:/Program Files/PostgreSQL/9.0/lib" \
    -L"C:/QtSDK/Desktop/Qt/4.7.2/mingw/plugins/sqldrivers" \
    -lqsqlite4 \
    -lpq \
    -lmysql \
    -liphlpapi
