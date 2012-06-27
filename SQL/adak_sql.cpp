#include <iostream>
#include <QStringList>
#include <adak_sql_struct.h>
#include "sql_ddl.h"
#include "adak_sql_error.h"
#include "sql_plugins.h"

using namespace std;

ADAK_SQL   *         G_YONETIM_DB         = NULL;
ADAK_SQL   *         DB                   = NULL;

/*********************************************/
/*             ADAK_SQL::ADAK_SQL          */
/*********************************************/

ADAK_SQL::ADAK_SQL( QList<ADAK_SQL_STRUCT *> adak_sql_struct, const QString& db_name ) : ADAK_SQL_ABSTRACT ( adak_sql_struct , db_name )
{

}

/*********************************************/
/*            ADAK_SQL::~ADAK_SQL          */
/*********************************************/

ADAK_SQL::~ADAK_SQL()
{

}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_FIND_NUM_OF_TABLES_IN_DB
***************************************************************************************/

int ADAK_SQL::SQL_FIND_NUM_OF_TABLES_IN_DB ( )
{
    SQL_DDL sql_query  ( this );

    QString query = GET_TABLE_NAMES_SQL();

    sql_query.EXEC_SQL( query );

    int count = sql_query.NUM_OF_ROWS();

    return count;
}

/**************************************************************************************
                   ADAK_SQL::UPDATE_TABLES
***************************************************************************************/

int ADAK_SQL::UPDATE_TABLES(QList<ADAK_SQL_STRUCT *> adak_sql_struct,QString db_name)
{
    if (adak_sql_struct.size() EQ 0) {
        adak_sql_struct = m_adak_sql_struct;
    }

    for ( int i = 0 ; i < adak_sql_struct.size() ; i++ ) {
        ADAK_SQL_STRUCT * modul_struct = adak_sql_struct.at(i);

        QStringList  TABLE_NAME_LIST;
        QStringList  COLUMN_NAME_LIST;
        bool         table_exists;

        QString database_name           = "";

        if ( db_name.isEmpty() EQ true ) {
            database_name = m_db_name;
        }
        else {
            database_name = db_name;
        }

        SQL_DDL sql_query  ( this );

        sql_query.EXEC_SQL( GET_TABLE_NAMES_SQL() );

        while ( sql_query.NEXT() EQ true ) {
            TABLE_NAME_LIST.append(sql_query.VALUE(0).toString());
        }
        for ( int i = 0; i < modul_struct->table_count; i++ ) {
            QString current_table = modul_struct->tables[i].name;
            table_exists     = false;
            for ( int j = 0; j < TABLE_NAME_LIST.size(); j++ ) {
                if ( TABLE_NAME_LIST.at(j).contains ( current_table,Qt::CaseInsensitive ) EQ true) {
                    table_exists = true;
                    break;
                }
            }
            if ( table_exists EQ false ) {
                QString create_table_sql = CREATE_ONE_TABLE_SQL(i, modul_struct );
                if ( sql_query.EXEC_SQL(create_table_sql) EQ false ) {
                    ADAK_SQL_ERROR(this->m_db_connection,CREATION_ERROR,ERROR_IN_EXEC_DDL,NULL);
                }
                continue;
            }

            if ( sql_query.EXEC_SQL(GET_COLUMN_NAMES_SQL(current_table, database_name )) EQ false ) {
                return ADAK_FAIL;
            }

            COLUMN_NAME_LIST.clear();

            while ( sql_query.NEXT() EQ true ) {
                switch ( GET_SQL_DRIVER_ENUM() ) {
                    case SQLITE :
                        COLUMN_NAME_LIST.append(sql_query.VALUE("name").toString());
                        break;
                    case MYSQL :
                        COLUMN_NAME_LIST.append(sql_query.VALUE(0).toString());
                        break;
                    case PSQL  :
                        COLUMN_NAME_LIST.append(sql_query.VALUE(0).toString());
                        break;
                    case MSSQL  :
                        COLUMN_NAME_LIST.append(sql_query.VALUE(0).toString());
                        break;
                    case ORACLE  :
                        COLUMN_NAME_LIST.append(sql_query.VALUE(0).toString());
                        break;
                    case UNKNOWN :
                    default      : {
                        return ADAK_FAIL;
                    }
                }
            }
            for ( int j = 0; j < modul_struct->tables[i].column_count; j++ ) {
                QString current_column = modul_struct->tables[i].columns[j].name;
                bool column_exists = false;
                for ( int k = 0; k < COLUMN_NAME_LIST.size(); k++ ) {
                    if ( COLUMN_NAME_LIST.at(k) EQ current_column ) {
                        column_exists = true;
                        break;
                    }
                }
                if ( column_exists EQ false ) {
                    QString create_one_column_sql = ADD_ONE_COLUMN_SQL(j,modul_struct->tables[i]);
                    SQL_DDL query ( this );
                    query.EXEC_SQL(create_one_column_sql);
                    continue;
                }
            }
        }
    }

    return ADAK_OK;
}
