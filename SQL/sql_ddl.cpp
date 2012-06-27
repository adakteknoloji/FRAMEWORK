#include "adak_defines.h"
#include "sql_ddl.h"
#include "stdlib.h"

#define SQL_MAX_MULTIPLE_INSERT_COUNT 2000

/**************************************************************************************
                   SQL_DDL::SQL_DDL
***************************************************************************************/

SQL_DDL::SQL_DDL(ADAK_SQL * db ) : SQL_QUERY ( db )
{
    RESET_MULTIPLE_INSERT();
}

/**************************************************************************************
                   SQL_DDL::EXEC_SQL
***************************************************************************************/

bool SQL_DDL::EXEC_SQL(const QString &query)
{
    return EXEC(query);
}

/**************************************************************************************
                   SQL_DDL::START_MULTIPLE_INSERT
***************************************************************************************/

void SQL_DDL::RESET_MULTIPLE_INSERT()
{
    m_multiple_insert_count          = -1;
    m_multiple_insert_query          = "";
    m_multiple_insert_table_name     = "";
    m_multiple_insert_column_names   = "";
    m_multiple_insert_id_column_name = "";
}

/**************************************************************************************
                   SQL_DDL::START_MULTIPLE_INSERT
***************************************************************************************/

void SQL_DDL::START_MULTIPLE_INSERT(const QString &table_name,const QString &id_column_name, const QString &insert_column_names )
{
    m_multiple_insert_table_name     = table_name;
    m_multiple_insert_id_column_name = id_column_name;
    m_multiple_insert_column_names   = insert_column_names;
    m_multiple_insert_count          = 0;

    m_multiple_insert_query = m_db->CREATE_MULTIPLE_INSERT_HEADER(table_name,id_column_name,insert_column_names);
}

/**************************************************************************************
                   SQL_DDL::START_MULTIPLE_INSERT
***************************************************************************************/

void SQL_DDL::ADD_VALUES_TO_QUEUE(const QString &column_values)
{
    if ( m_multiple_insert_count NE 0 ) {
        m_multiple_insert_query.append(m_db->GET_MULTIPLE_INSERT_DIVIDER());
    }

    m_multiple_insert_query.append(m_db->CREATE_MULTIPLE_INSERT_BODY(m_multiple_insert_table_name,m_multiple_insert_id_column_name,m_multiple_insert_column_names,column_values));
    m_multiple_insert_count++;

    if ( m_db->IS_DB_HAS_MULTIPLE_INSERT() EQ false OR m_multiple_insert_count >= SQL_MAX_MULTIPLE_INSERT_COUNT) {

        m_multiple_insert_query.append(m_db->CREATE_MULTIPLE_INSERT_FOOTER(m_multiple_insert_table_name,m_multiple_insert_id_column_name,
                                                                           m_multiple_insert_column_names));

        if ( m_multiple_insert_query.isEmpty() EQ false ) {
            if ( EXEC(m_multiple_insert_query) EQ false ) {
                qDebug(QString("SQL Hatası:" + m_multiple_insert_query).toUtf8().data());
                exit(88);
            }
        }

        START_MULTIPLE_INSERT(m_multiple_insert_table_name,m_multiple_insert_id_column_name,m_multiple_insert_column_names);

        m_multiple_insert_count = 0;
    }
}

/**************************************************************************************
                   SQL_DDL::COMMIT_MULTIPLE_INSERT
***************************************************************************************/

void SQL_DDL::COMMIT_MULTIPLE_INSERT()
{
    if ( m_multiple_insert_count NE 0 ) {

        m_multiple_insert_query.append(m_db->CREATE_MULTIPLE_INSERT_FOOTER(m_multiple_insert_table_name,m_multiple_insert_id_column_name,
                                                                           m_multiple_insert_column_names));

        if ( m_multiple_insert_query.isEmpty() EQ false ) {
            if ( EXEC(m_multiple_insert_query) EQ false ) {
                qDebug(QString("SQL Hatası:" + m_multiple_insert_query).toUtf8().data());
                exit(88);
            }
        }
    }

    RESET_MULTIPLE_INSERT();
}
