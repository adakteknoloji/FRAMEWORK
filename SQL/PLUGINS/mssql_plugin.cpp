#include "adak_sql_error.h"
#include "adak_sql_struct.h"
#include "adak_defines.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "sql_ddl.h"

#include "sql_plugins.h"
#include "mssql_plugin.h"
#include <stdlib.h>
#include <QCoreApplication>

/**************************************************************************************
                   MSSQL_PLUGIN::MSSQL_PLUGIN
***************************************************************************************/

MSSQL_PLUGIN::MSSQL_PLUGIN ( QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name ) : ADAK_SQL ( adak_sql_struct , db_name )
{
}

/**************************************************************************************
                   MSSQL_PLUGIN::~MSSQL_PLUGIN
***************************************************************************************/

MSSQL_PLUGIN::~MSSQL_PLUGIN()
{

}

/**************************************************************************************
                   MSSQL_PLUGIN::LOCK_ROW
***************************************************************************************/

int MSSQL_PLUGIN::LOCK_ROW (const QString ,const QString table_name, const QString where_info)
{
    //FIXME

    return ADAK_OK;

    // donen deger >= 0 sa locklanmistir aksi takdirde lock olmamistir.

    SQL_DDL sql_query      ( this );

    bool answer = sql_query.EXEC_SQL( QString("sp_getapplock @Resource = '%1', @LockMode = 'Exclusive',@LockTimeout = 2,@LockOwner = 'Session'").arg(QString ("LOCK("+table_name + "." + where_info+",2)")) );

    if(answer EQ false ) {
        return ADAK_FAIL;
    }

    if ( sql_query.NUM_OF_ROWS() <= 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    if ( sql_query.VALUE(0).toInt() EQ 0 ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   MSSQL_PLUGIN::UNLOCK_ROW
***************************************************************************************/

void MSSQL_PLUGIN::UNLOCK_ROW (const QString ,const QString table_name, const QString where_info)
{
    SQL_DDL sql_query      ( this );

    sql_query.EXEC_SQL( QString("sp_releaseapplock @Resource = '%1', @LockOwner = 'Session'").arg(QString ("LOCK("+table_name + "." + where_info+",2)")));
}

/**************************************************************************************
                   MSSQL_PLUGIN::OPEN_CREATE_TABLE_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::OPEN_CREATE_TABLE_SQL( const QString &table_name )
{
    return "CREATE TABLE " + table_name + " ( ";
}

/**************************************************************************************
                   MSSQL_PLUGIN::CLOSE_CREATE_TABLE_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::CLOSE_CREATE_TABLE_SQL ( TABLE_STRUCT current_table )
{
    int index_count = current_table.index_count;

    if ( index_count EQ 1 ) {
         return "\n );";
    }

    QString index_sentence = "\n );";

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
                   MSSQL_PLUGIN::CREATE_COLUMN_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::CREATE_COLUMN_SQL( QString const& column_name,
                                                COLUMN_TYPE    column_type,
                                                int            length,
                                                bool           is_auto_increment,
                                                bool           is_not_null,
                                                bool           is_array,
                                                INDEX_TYPE     index_type        )

{
    Q_UNUSED ( index_type );

    QString column_sentence = column_name;

    if ( is_array EQ true ) {
        column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT, 1 );
        if ( is_not_null EQ true ) {
            column_sentence += " NOT NULL";
        }
        else {
            column_sentence += " NULL";
        }
        return column_sentence;
    }

    switch (column_type) {
        case INTEGER :
        default      :

            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) +  " ";

            if (is_auto_increment EQ true AND is_not_null EQ true ) {
                column_sentence += " IDENTITY(1,1) ";
            }
            break;

        case TEXT:
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) + "(" + QVariant(length).toString() + ")";
            break;

        case REAL   :
        case BLOB   :
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) +  " ";
            break;
    }
    if ( is_not_null EQ true ) {
        column_sentence += " NOT NULL";
    }
    else {
        column_sentence += " NULL";
    }

    return column_sentence;
}

/**************************************************************************************
                   MSSQL_PLUGIN::DROP_TABLE_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::DROP_TABLE_SQL ( const QString& table_name )
{
    return "DROP TABLE " + table_name + ";";
}

/**************************************************************************************
                   MSSQL_PLUGIN::GET_TABLE_NAMES_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::GET_TABLE_NAMES_SQL()
{
    return "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES";
}

/**************************************************************************************
                   MSSQL_PLUGIN::GET_COLUMN_NAMES_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::GET_COLUMN_NAMES_SQL( const QString &table_name, const QString& database_name)
{
    return QString("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = "
                   "'%1' AND TABLE_SCHEMA = '%2'").arg(table_name).arg(database_name);
}

/**************************************************************************************
                   MSSQL_PLUGIN::ADD_ONE_COLUMN_SQL
***************************************************************************************/

QString MSSQL_PLUGIN::ADD_ONE_COLUMN_SQL ( int column_number, TABLE_STRUCT current_table )
{
    QString table_name           = current_table.name;
    COLUMN_STRUCT current_column = current_table.columns[column_number];
    QString column_name          = current_column.name;
    QString add_column_sentence  = QString("ALTER TABLE %1 ADD %2 ").arg(table_name).arg(column_name);

    if ( current_column.is_array EQ true ) {
        add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT, 1 );
        if ( current_column.is_not_null EQ true ) {
            add_column_sentence += " NOT NULL";
        }
        else {
            add_column_sentence += " NULL";
        }
        return add_column_sentence;
    }

    switch ( current_column.type ) {
        case INTEGER :
        case REAL    :
        case BLOB    :
        default      :
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) +  " ";
            break;
        case TEXT:
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) +  "(" + QVariant(current_column.length).toString() + ") ";
            break;
    }

    if ( current_column.is_not_null EQ true ) {
        add_column_sentence += " NOT NULL";
    }
    else {
        add_column_sentence += " NULL";
    }
    return add_column_sentence;
}

/**************************************************************************************
                   MSSQL_PLUGIN::GET_COLUMN_TYPE_NAME
***************************************************************************************/

QString MSSQL_PLUGIN::GET_COLUMN_TYPE_NAME ( COLUMN_TYPE p_column_type, int is_array )
{
    switch ( p_column_type ) {
        case INTEGER :
        default      :
            return "INT";
        case TEXT:
            if ( is_array EQ 1 ) {
                return "BINARY";
            }
            return "NVARCHAR";
        case REAL:
            return "REAL";
        case BLOB:
            return "BINARY";
    }
    return "";
}

/**************************************************************************************
                   MSSQL_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME
***************************************************************************************/

QString MSSQL_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME()
{
    return "master";
}

/**************************************************************************************
                   MSSQL_PLUGIN::GET_SQL_DRIVER_ENUM
***************************************************************************************/

DB_DRIVER MSSQL_PLUGIN::GET_SQL_DRIVER_ENUM()
{
    return MSSQL;
}


/**************************************************************************************
                   MSSQL_PLUGIN::GET_SQL_DRIVER_NAME
***************************************************************************************/

QString MSSQL_PLUGIN::GET_SQL_DRIVER_NAME()
{
    return "QTDS";
}

/**************************************************************************************
                   MSSQL_PLUGIN::CREATE_SELECT_QUERY
***************************************************************************************/

QString MSSQL_PLUGIN::CREATE_SELECT_QUERY(QString *table_name, QString *column_names, QString *where_condition, QString *group_by, QString *order_by, int baslanilacak_satir, int secilecek_satir_sayisi)
{
    Q_UNUSED(baslanilacak_satir);

    QString query = "";
    query.append(QString("SELECT "));

    if ( secilecek_satir_sayisi > 0 ) {
        query.append(QString(" TOP  "   + QVariant (secilecek_satir_sayisi).toString() + " " ));
    }

    query.append(column_names);

    query.append(QString(" FROM " + *table_name      ));

    if ( where_condition->isEmpty() EQ false ) {
        query.append(QString(" WHERE " + *where_condition));
    }

    if ( group_by->isEmpty() EQ false ) {
        query.append(QString(" GROUP BY " + *group_by    ));
    }

    if ( order_by->isEmpty() EQ false ) {
        query.append(QString(" ORDER BY " + *order_by    ));
    }

    return query;
}

/**************************************************************************************
                   MSSQL_PLUGIN::GET_ID_USING_DRIVER
***************************************************************************************/

int MSSQL_PLUGIN::GET_ID_USING_DRIVER( QSqlQuery * query )
{
    if ( query->exec("SELECT @@IDENTITY" ) EQ true ) {
        query->next();
        return query->value(0).toInt();
    }

    //PREPARE_INSERT ( m_last_insert_query_id_column_name, m_last_insert_query );

    return -1;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::IS_DB_HAS_MULTIPLE_INSERT
***************************************************************************************/

bool MSSQL_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT()
{
    return false;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_MULTIPLE_INSERT_HEADER
***************************************************************************************/

QString MSSQL_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER(const QString &,const QString &, const QString &)
{
    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_MULTIPLE_INSERT_BODY
***************************************************************************************/

QString MSSQL_PLUGIN::CREATE_MULTIPLE_INSERT_BODY(const QString &table_name,const QString &, const QString &insert_column_names, const QString &insert_column_values)
{
    return QString ( "INSERT INTO "+ table_name +" ("+ insert_column_names +") VALUES ("+ insert_column_values +")");
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_MULTIPLE_INSERT_FOOTER
***************************************************************************************/

QString MSSQL_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER(const QString &,const QString &, const QString &)
{
    return "";
}
