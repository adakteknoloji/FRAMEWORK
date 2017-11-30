#include "adak_sql_error.h"
#include "adak_sql_struct.h"
#include "adak_defines.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "adak_sql_query.h"

#include "odbc_plugin.h"
#include "sql_plugins.h"

/**************************************************************************************
                   ODBC_PLUGIN::ODBC_PLUGIN
***************************************************************************************/

ODBC_PLUGIN::ODBC_PLUGIN (QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name) : ADAK_SQL ( adak_sql_struct , db_name )
{

}

/**************************************************************************************
                   ODBC_PLUGIN::~ODBC_PLUGIN
***************************************************************************************/

ODBC_PLUGIN::~ODBC_PLUGIN()
{

}

/**************************************************************************************
                   ODBC_PLUGIN::LOCK_ROW
***************************************************************************************/

int ODBC_PLUGIN::LOCK_ROW (const QString ,const QString , const QString )
{
    return ADAK_OK;
}

/**************************************************************************************
                   ODBC_PLUGIN::UNLOCK_ROW
***************************************************************************************/

void ODBC_PLUGIN::UNLOCK_ROW (const QString ,const QString , const QString )
{

}

/**************************************************************************************
                   ODBC_PLUGIN::OPEN_CREATE_TABLE_SQL
***************************************************************************************/

QString ODBC_PLUGIN::OPEN_CREATE_TABLE_SQL( QString const& table_name )
{
    return "CREATE TABLE IF NOT EXISTS " + table_name + " (";
}

/**************************************************************************************
                   ODBC_PLUGIN::CLOSE_CREATE_TABLE_SQL
***************************************************************************************/

QString ODBC_PLUGIN::CLOSE_CREATE_TABLE_SQL ( TABLE_STRUCT current_table )
{
    int index_count = current_table.index_count;
    if ( index_count EQ 1 ) {
        return "\n ) ENGINE = INNODB;";
    }

    QString index_sentence = ",";
    bool index_comma_needed = false;
    for ( int i = 1; i < index_count; i++ ) {
        if ( index_comma_needed EQ true ) {
            index_sentence += ", ";
        }
        index_comma_needed = true;
        index_sentence    += "\n\t INDEX ( ";
        INDEX_STRUCT current_index      = current_table.indexes[i];
        int   index_column_count = current_index.column_count;
        bool column_comma_needed = false;
        for ( int j = 0; j < index_column_count; j++ ) {
            if ( column_comma_needed EQ true ) {
                index_sentence += ", ";
            }
            column_comma_needed = true;
            index_sentence += current_index.columns[j];
        }
        index_sentence += " )";
    }
    index_sentence += "\n) ENGINE = INNODB;";
    return index_sentence;
}

/**************************************************************************************
                   ODBC_PLUGIN::CREATE_COLUMN_SQL
***************************************************************************************/

QString ODBC_PLUGIN::CREATE_COLUMN_SQL (       QString const& column_name,
                                                COLUMN_TYPE    column_type,
                                                int            length,
                                                bool           is_auto_increment,
                                                bool           is_not_null,
                                                bool           is_array,
                                                INDEX_TYPE     index_type        )

{

    QString column_sentence = column_name;

    if ( is_array EQ true ) {
        column_sentence += " " + GET_COLUMN_TYPE_NAME ( ADAK_SQL_TEXT, 1 ) + " ";
        return column_sentence;
    }

    switch ( column_type ) {
        case ADAK_SQL_INTEGER    :
        case ADAK_SQL_REAL       :
        case ADAK_SQL_BLOB       :
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type );
            break;

        case ADAK_SQL_TEXT:
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) + "(" + QVariant(length).toString() + ")";
            break;
    }

    if ( is_not_null EQ true ) {
        column_sentence += " NOT NULL";
    }

    if (column_type EQ ADAK_SQL_TEXT ) {
        return column_sentence;
    }

    if ( is_auto_increment EQ true ) {
        column_sentence += " AUTO_INCREMENT";
    }

    switch ( index_type ) {
        case ADAK_SQL_PRIMARY:
            column_sentence += " PRIMARY KEY";
            break;
        case ADAK_SQL_UNIQUE:
        case ADAK_SQL_INDEX:
        case ADAK_SQL_NONE:
        default:
            break;
    }
    return column_sentence;
}

/**************************************************************************************
                   ODBC_PLUGIN::DROP_TABLE_SQL
***************************************************************************************/

QString ODBC_PLUGIN::DROP_TABLE_SQL ( const QString& table_name )
{
    return "DROP TABLE IF EXISTS" + table_name + ";";
}

/**************************************************************************************
                   ODBC_PLUGIN::GET_TABLE_NAMES_SQL
***************************************************************************************/

QString ODBC_PLUGIN::GET_TABLE_NAMES_SQL()
{
    return "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'";
}

/**************************************************************************************
                   ODBC_PLUGIN::GET_COLUMN_NAMES_SQL
***************************************************************************************/

QString ODBC_PLUGIN::GET_COLUMN_NAMES_SQL( const QString& table_name, const QString& database_name  )
{
    return QString("SELECT column_name FROM information_schema.columns WHERE table_name = "
                   "'%1' AND TABLE_SCHEMA = '%2'").arg(table_name).arg(database_name);
}

/**************************************************************************************
                   ODBC_PLUGIN::ADD_ONE_COLUMN_SQL
***************************************************************************************/

QString ODBC_PLUGIN::ADD_ONE_COLUMN_SQL ( int column_number, TABLE_STRUCT current_table )
{
    QString table_name           = current_table.name;
    COLUMN_STRUCT current_column = current_table.columns[column_number];
    QString column_name          = current_column.name;
    QString add_column_sentence  = QString("ALTER TABLE %1  ADD COLUMN %2 ").arg(table_name).arg(column_name);

    if ( current_column.is_array EQ true ) {
        add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( ADAK_SQL_TEXT, 1 ) + " ";
        return add_column_sentence;
    }

    switch ( current_column.type ) {
        case ADAK_SQL_INTEGER    :
        case ADAK_SQL_REAL       :
        case ADAK_SQL_BLOB       :
        default         :
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + " ";
            break;
        case ADAK_SQL_TEXT:
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + "(" + QVariant(current_column.length).toString() + ") ";
            break;
    }

    if ( current_column.is_not_null EQ true ) {
        add_column_sentence += " NOT NULL";
    }
    return add_column_sentence;
}

/**************************************************************************************
                   ODBC_PLUGIN::GET_COLUMN_TYPE_NAME
***************************************************************************************/

QString ODBC_PLUGIN::GET_COLUMN_TYPE_NAME ( COLUMN_TYPE p_column_type, int is_array )
{
    switch ( p_column_type ) {
        case ADAK_SQL_INTEGER :
        default      :
            return "INTEGER";
        case ADAK_SQL_TEXT:
            if ( is_array EQ 1 ) {
                return "BLOB";
            }
            return "VARCHAR";
        case ADAK_SQL_REAL:
            return "REAL";
        case ADAK_SQL_BLOB:
            return "BLOB";
    }
    return "";
}

/**************************************************************************************
                   ODBC_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME
***************************************************************************************/

QString ODBC_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME()
{
    return "";
}

/**************************************************************************************
                   ODBC_PLUGIN::GET_SQL_DRIVER_ENUM
***************************************************************************************/

DB_DRIVER ODBC_PLUGIN::GET_SQL_DRIVER_ENUM()
{
    return ODBC;
}


/**************************************************************************************
                   ODBC_PLUGIN::GET_SQL_DRIVER_NAME
***************************************************************************************/

QString ODBC_PLUGIN::GET_SQL_DRIVER_NAME()
{
    return "QODBC";
}


/**************************************************************************************
                   ODBC_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT
***************************************************************************************/

bool ODBC_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT()
{
    return false;
}

/**************************************************************************************
                   ODBC_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER
***************************************************************************************/

QString ODBC_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER(const QString &,const QString &, const QString &)
{
    return "";
}

/**************************************************************************************
                   ODBC_PLUGIN::CREATE_MULTIPLE_INSERT_BODY
***************************************************************************************/

QString ODBC_PLUGIN::CREATE_MULTIPLE_INSERT_BODY(const QString &table_name,const QString &, const QString &insert_column_names, const QString &insert_column_values)
{
    return QString ( "INSERT INTO "+ table_name +" ("+ insert_column_names +") VALUES ("+ insert_column_values +")");
}

/**************************************************************************************
                   ODBC_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER
***************************************************************************************/

QString ODBC_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER(const QString &,const QString &, const QString &)
{
    return "";
}
