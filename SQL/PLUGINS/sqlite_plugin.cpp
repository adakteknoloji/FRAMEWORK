#include <QCoreApplication>
#include <QStringList>
#include <QDir>
#include "adak_sql_struct.h"
#include "adak_defines.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "adak_sql_query.h"
#include "adak_sql_error.h"
#include "sqlite_plugin.h"
#include "sql_plugins.h"

/**************************************************************************************
                   SQLITE_PLUGIN::SQLITE_PLUGIN
***************************************************************************************/

SQLITE_PLUGIN::SQLITE_PLUGIN(QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name) : ADAK_SQL ( adak_sql_struct , db_name)
{
}

/**************************************************************************************
                   SQLITE_PLUGIN::~SQLITE_PLUGIN
***************************************************************************************/

SQLITE_PLUGIN::~SQLITE_PLUGIN()
{

}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CONNECT_TO_DATABASE
***************************************************************************************/

int SQLITE_PLUGIN::CONNECT_TO_DATABASE ( bool show_error_screen )
{
    Q_UNUSED( show_error_screen );

    QString db_name = QString ( m_db_name + ".db" );

    if ( QSqlDatabase::connectionNames().contains(db_name) EQ true ) {
        REMOVE_CONNECT(db_name);
    }

    m_db_connection = QSqlDatabase::addDatabase( "QSQLITE" , db_name );

    if( m_db_connection.isOpen() EQ true ) {
        m_db_connection.close();
    }

    QString database_file_path = QDir::currentPath() + "/" + QString ( m_db_name + ".db" );

    m_db_connection.setDatabaseName  ( database_file_path );

    if( m_db_connection.open() EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQLITE_PLUGIN::SQL_DROP_TABLES
***************************************************************************************/

bool SQLITE_PLUGIN::SQL_DROP_TABLES (QList<ADAK_SQL_STRUCT *> adak_sql_struct )
{
    CONNECT_TO_DATABASE( false );

    for ( int i = 0 ; i < adak_sql_struct.size() ; i++ ) {
        QStringList drop_table_sql_list      = DROP_ALL_TABLES_SQL( adak_sql_struct.at(i) );

        for ( int i = 0; i < adak_sql_struct.at(i)->table_count; i++ ) {
            QSqlQuery drop_table_query ( m_db_connection );
            if ( drop_table_query.exec ( drop_table_sql_list[i] )  EQ false ) {
                ADAK_SQL_ERROR( m_db_connection, DROP_ERROR, ERROR_IN_DROP_TABLES, &drop_table_query);
            }
            QSqlQuery vacuum_query ( m_db_connection);
            if (vacuum_query.exec("VACUUM") EQ false ) {
                ADAK_SQL_ERROR( m_db_connection, EXEC_DDL_ERROR, ERROR_IN_DROP_TABLES, &vacuum_query);
            }
        }
    }

    CONNECT_TO_DATABASE( false );

    return true;
}

/**************************************************************************************
                   SQLITE_PLUGIN::OPEN_CREATE_TABLE_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::OPEN_CREATE_TABLE_SQL ( QString const &table_name )
{
    return "CREATE TABLE IF NOT EXISTS " + table_name + " (";
}

/**************************************************************************************
                   SQLITE_PLUGIN::CLOSE_CREATE_TABLE_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::CLOSE_CREATE_TABLE_SQL ( TABLE_STRUCT current_table )
{
    int index_count = current_table.index_count;
    if ( index_count EQ 1 ) {
        return "\n);";
    }

    QString index_sentence = "\n);";

    for ( int i = 1; i < index_count; i++ ) {
        m_database_index_count++;
        index_sentence    += "\n\t CREATE INDEX index" + QVariant(m_database_index_count).toString()  + " ON " + current_table.name
                             +" ( ";
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
        index_sentence += " );";
    }
    return index_sentence;
}

/**************************************************************************************
                   SQLITE_PLUGIN::CREATE_COLUMN_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::CREATE_COLUMN_SQL(       QString const& column_name,
                                                COLUMN_TYPE    column_type,
                                                int            length,
                                                bool           is_auto_increment,
                                                bool           is_not_null,
                                                bool           is_array,
                                                INDEX_TYPE     index_type        )

{
    Q_UNUSED (is_auto_increment);
    QString column_sentence = column_name;

    if ( is_array EQ true ) {
        column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT, 1 ) + " ";
        return column_sentence;
    }

    switch ( column_type ) {
        case INTEGER :
        case REAL    :
        case BLOB    :

            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type );
            break;

        case TEXT:
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) + "(" + QVariant(length).toString() + ")";
            break;
    }

    if ( is_not_null EQ true ) {
        if (index_type EQ PRIMARY ) {
            column_sentence += " NOT NULL";
        }
        else {
            column_sentence += " DEFAULT 0 NOT NULL";
        }
    }

    if (  index_type EQ PRIMARY ) {
        column_sentence += " PRIMARY KEY AUTOINCREMENT";
    }
    return column_sentence;
}

/**************************************************************************************
                   SQLITE_PLUGIN::DROP_TABLE_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::DROP_TABLE_SQL ( const QString& table_name )
{
    return "DROP TABLE IF EXISTS " + table_name + ";";
}

/**************************************************************************************
                   SQLITE_PLUGIN::GET_TABLE_NAMES_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::GET_TABLE_NAMES_SQL()
{
    return "SELECT name FROM sqlite_master";
}

/**************************************************************************************
                   SQLITE_PLUGIN::GET_COLUMN_NAMES_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::GET_COLUMN_NAMES_SQL( const QString& table_name, const QString& database_name )
{
    Q_UNUSED (database_name);
    return "PRAGMA table_info (" + table_name + ")";
}

/**************************************************************************************
                   SQLITE_PLUGIN::ADD_ONE_COLUMN_SQL
***************************************************************************************/

QString SQLITE_PLUGIN::ADD_ONE_COLUMN_SQL ( int column_number, TABLE_STRUCT current_table )
{
    QString table_name           = current_table.name;
    COLUMN_STRUCT current_column = current_table.columns[column_number];
    QString column_name          = current_column.name;
    QString add_column_sentence  = QString("ALTER TABLE '%1'  ADD COLUMN '%2' ").arg(table_name).arg(column_name);

    if ( current_column.is_array EQ true ) {
        add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT, 1 ) + " ";
        return add_column_sentence;
    }

    switch ( current_column.type ) {
        case INTEGER    :
        case REAL       :
        case BLOB       :
        default         :
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + " ";
            break;
        case TEXT       :
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + "(" + QVariant(current_column.length).toString() + ") ";
            break;

    }

    if ( current_column.is_not_null EQ true ) {
        add_column_sentence += " DEFAULT 0 NOT NULL";
    }

    return add_column_sentence;
}

/**************************************************************************************
                   SQLITE_PLUGIN::GET_COLUMN_TYPE_NAME
***************************************************************************************/

QString SQLITE_PLUGIN::GET_COLUMN_TYPE_NAME ( COLUMN_TYPE p_column_type, int is_array )
{
    switch ( p_column_type ) {
        case INTEGER :
        default      :
            return "INTEGER";
        case TEXT    :
            if ( is_array EQ 1 ) {
                return "BLOB";
            }
            return "VARCHAR";
        case REAL   :
            return "REAL";
        case BLOB   :
            return "BLOB";
    }
    return "";
}
/**************************************************************************************
                   SQLITE_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME
***************************************************************************************/

QString SQLITE_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME()
{
    return "sqlite";
}

/**************************************************************************************
                   SQLITE_PLUGIN::GET_SQL_DRIVER_ENUM
***************************************************************************************/

DB_DRIVER SQLITE_PLUGIN::GET_SQL_DRIVER_ENUM()
{
    return SQLITE;
}


/**************************************************************************************
                   SQLITE_PLUGIN::GET_SQL_DRIVER_NAME
***************************************************************************************/

QString SQLITE_PLUGIN::GET_SQL_DRIVER_NAME()
{
    return "QSQLITE";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::IS_DB_HAS_MULTIPLE_INSERT
***************************************************************************************/

bool SQLITE_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT()
{
    return false;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_MULTIPLE_INSERT_HEADER
***************************************************************************************/

QString SQLITE_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER(const QString &,const QString &, const QString &)
{
    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_MULTIPLE_INSERT_BODY
***************************************************************************************/

QString SQLITE_PLUGIN::CREATE_MULTIPLE_INSERT_BODY(const QString &table_name,const QString &, const QString &insert_column_names, const QString &insert_column_values)
{
    return QString ( "INSERT INTO "+ table_name +" ("+ insert_column_names +") VALUES ("+ insert_column_values +")");
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_MULTIPLE_INSERT_FOOTER
***************************************************************************************/

QString SQLITE_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER(const QString &,const QString &, const QString &)
{
    return "";
}
