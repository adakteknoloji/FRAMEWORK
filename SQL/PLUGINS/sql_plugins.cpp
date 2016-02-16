#include <QStringList>

#include "adak_sql_query.h"
#include "adak_sql_struct.h"

#include "mssql_plugin.h"
#include "mysql_plugin.h"
#include "oracle_plugin.h"
#include "sqlite_plugin.h"
#include "postgressql_plugin.h"
#include "odbc_plugin.h"

#include "sql_plugins.h"
#include "adak_utils.h"

static QString g_database_sid = "XE";

/**************************************************************************************
                   CREATE_ADAK_SQL_CONNECTION
***************************************************************************************/

ADAK_SQL * CREATE_ADAK_SQL_CONNECTION ( QList<ADAK_SQL_STRUCT *> adak_sql_struct , QString db_name , DB_DRIVER db_driver )
{
    switch ( db_driver ) {
        case PSQL  :
            return (new POSTGRESSQL_PLUGIN(adak_sql_struct , db_name ));
        case MYSQL :
            return (new MYSQL_PLUGIN      (adak_sql_struct , db_name ));
        case MSSQL :
            return (new MSSQL_PLUGIN      (adak_sql_struct , db_name ));
        case ORACLE:
            return (new ORACLE_PLUGIN     (adak_sql_struct , db_name ));
        case ODBC  :
            return (new ODBC_PLUGIN       (adak_sql_struct , db_name ));
        case SQLITE:
        default    :
            return (new SQLITE_PLUGIN     (adak_sql_struct , db_name ));
    }
}

/**************************************************************************************
                   GET_DEFAULT_SQL_PORT_NUMBER
***************************************************************************************/

int GET_DEFAULT_SQL_PORT_NUMBER ( DB_DRIVER db_driver )
{
    switch ( db_driver ) {
        case MYSQL :
            return 3306;
        case PSQL  :
            return 5432;
        case MSSQL :
            return 1433;
        case ORACLE:
            return 1521;
        case ODBC  :
            return -1;
        case SQLITE:
        default    :
            return 0;
    }
}

/**************************************************************************************
                   GET_SQL_DB_DRIVER_ENUM
***************************************************************************************/

DB_DRIVER GET_SQL_DB_DRIVER_ENUM ( QString driver_name )
{
    if ( driver_name.toUpper() EQ "NONE" ) {
        return SQLITE;
    }
    else if ( driver_name.toUpper() EQ "MYSQL" ) {
        return MYSQL;
    }
    else if ( driver_name.toUpper() EQ "POSTGRESQL" ) {
        return PSQL;
    }
    else if ( driver_name.toUpper() EQ "MICROSOFTSQL" ) {
        return MSSQL;
    }
    else if ( driver_name.toUpper() EQ "ORACLE" ) {
        return ORACLE;
    }
    else if ( driver_name.toUpper() EQ "ODBC" ) {
        return ODBC;
    }

    return UNKNOWN;
}

/**************************************************************************************
                   GET_SQL_DRIVER_LIST
***************************************************************************************/

QStringList GET_SQL_DRIVER_LIST()
{
    return (QStringList() << "NONE"  << "PostgreSQL" << "MySQL" << "MicrosoftSQL"<< "ORACLE" << "ODBC" );
}

/**************************************************************************************
                                SQL_SET_DB_SID
***************************************************************************************/

void SQL_SET_DB_SID (QString sid)
{
    if ( sid.isEmpty() EQ true ) {
        return;
    }

    g_database_sid = sid;
}

/**************************************************************************************
                                SQL_GET_DB_SID
***************************************************************************************/

QString SQL_GET_DB_SID ()
{
    return g_database_sid;
}

/**************************************************************************************
                                CONNECT_DATABASE
***************************************************************************************/

ADAK_SQL * CONNECT_DATABASE(QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name, DB_DRIVER driver, bool show_error_screen)
{
    ADAK_SQL * DB_CONN = CREATE_ADAK_SQL_CONNECTION( adak_sql_struct, db_name, driver );

    if ( DB_CONN->CONNECT_TO_DATABASE( show_error_screen ) EQ ADAK_FAIL ) {
        delete DB_CONN;
        return NULL;
    }
    return DB_CONN;
}
