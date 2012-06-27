include (../INCLUDE/adak.pri)

QT      += sql
QT      -= gui

TARGET   = ADAK_SQL
TEMPLATE = lib
CONFIG += static 

DEPENDPATH += .
INCLUDEPATH += . ../INCLUDE PLUGINS/

SOURCES += \
           adak_sql.cpp \
           adak_sql_query.cpp \
           adak_sql_error.cpp \
           adak_sql_abstract.cpp \
    PLUGINS/mysql_plugin.cpp \
    PLUGINS/postgressql_plugin.cpp \
    PLUGINS/sqlite_plugin.cpp \
    PLUGINS/oracle_plugin.cpp \
    PLUGINS/mssql_plugin.cpp \
    PLUGINS/odbc_plugin.cpp \
    PLUGINS/sql_plugins.cpp \
    sql_ddl.cpp
HEADERS += ../INCLUDE/adak_sql_enums.h \
           ../INCLUDE/adak_sql.h \
           ../INCLUDE/adak_sql_struct.h \
           ../INCLUDE/adak_sql_error.h \
           ../INCLUDE/adak_sql_query.h \
            ../INCLUDE/adak_sql_abstract.h \
    PLUGINS/mysql_plugin.h \
    PLUGINS/postgressql_plugin.h \
    PLUGINS/sqlite_plugin.h \
    PLUGINS/oracle_plugin.h \
    PLUGINS/mssql_plugin.h \
    PLUGINS/odbc_plugin.h \
    ../INCLUDE/sql_plugins.h \
    ../INCLUDE/sql_ddl.h
