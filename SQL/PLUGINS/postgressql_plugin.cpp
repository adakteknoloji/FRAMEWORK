#include "adak_sql_struct.h"
#include "adak_sql_error.h"
#include "adak_defines.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "sql_ddl.h"

#include "postgressql_plugin.h"
#include "sql_plugins.h"

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::POSTGRESSQL_PLUGIN
***************************************************************************************/

POSTGRESSQL_PLUGIN::POSTGRESSQL_PLUGIN(QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name) : ADAK_SQL(adak_sql_struct , db_name)
{

}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::~POSTGRESSQL_PLUGIN
***************************************************************************************/

POSTGRESSQL_PLUGIN::~POSTGRESSQL_PLUGIN()
{

}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::SET_TABLE_AUTO_INCREMENT_ID_VALUE
***************************************************************************************/

void POSTGRESSQL_PLUGIN::SET_TABLE_AUTO_INCREMENT_ID_VALUE ( const QString p_table_name, const QString p_id_column_name, int p_last_auto_inc_value )
{
    SQL_DDL sql_query      ( this );

    sql_query.EXEC_SQL( QString ( "SELECT setval ( '%1', %2 )").arg ( p_table_name + "_" + p_id_column_name + "_seq" ).arg(p_last_auto_inc_value));
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::LOCK_ROW
***************************************************************************************/

int POSTGRESSQL_PLUGIN::LOCK_ROW (const QString id_column_name,const QString table_name, const QString where_info)
{
    SQL_DDL sql_query      ( this );

    if ( sql_query.EXEC_SQL( QString ("SELECT pg_try_advisory_lock("+id_column_name+") FROM " + table_name + " WHERE " + where_info)) EQ false) {
        return ADAK_FAIL;
    }

    if ( sql_query.NUM_OF_ROWS() <= 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    if ( sql_query.VALUE(0).toBool() EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::SET_CLIENT_CODEC
***************************************************************************************/

void POSTGRESSQL_PLUGIN::SET_CLIENT_CODEC()
{
    SQL_DDL sql_query      ( this );
    sql_query.EXEC_SQL     ( "SET CLIENT_ENCODING TO 'UTF8'");
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::UNLOCK_ROW
***************************************************************************************/

void POSTGRESSQL_PLUGIN::UNLOCK_ROW (const QString id_column_name,const QString table_name, const QString where_info)
{
    SQL_DDL sql_query      ( this );

    sql_query.EXEC_SQL( QString ("SELECT pg_advisory_unlock("+id_column_name+") FROM " + table_name + " WHERE " + where_info +""));
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::OPEN_CREATE_TABLE_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::OPEN_CREATE_TABLE_SQL( const QString &table_name )
{
    return "CREATE TABLE " + table_name + " (";
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::CLOSE_CREATE_TABLE_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CLOSE_CREATE_TABLE_SQL ( TABLE_STRUCT current_table )
{
    int index_count = current_table.index_count;
    if ( index_count EQ 1 ) {
         return ") WITH OIDS;";
    }

    QString index_sentence = ") WITH OIDS;";

    for ( int i = 1; i < index_count; i++ ) {
        m_database_index_count++;
        index_sentence    += " CREATE INDEX index" + QVariant(m_database_index_count).toString()  + " ON " + current_table.name
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
                   POSTGRESSQL_PLUGIN::CREATE_COLUMN_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CREATE_COLUMN_SQL( QString const& column_name,
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
        column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT, 1 ) + " ";
        return column_sentence;
    }

    switch (column_type) {
        case INTEGER:
            if ( is_auto_increment EQ true AND is_not_null EQ true ) {
                column_sentence += " serial unique ";
            }
            else {
                column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type );
            }
            break;

        case TEXT:
            column_sentence += " " + GET_COLUMN_TYPE_NAME (column_type ) + "(" + QVariant(length).toString() + ")";
            break;

        case REAL :
        case BLOB :
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type );
            break;

    }

    return column_sentence;
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::DROP_TABLE_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::DROP_TABLE_SQL ( const QString& table_name )
{
    return "DROP TABLE " + table_name + ";";
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::GET_TABLE_NAMES_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::GET_TABLE_NAMES_SQL()
{
    return "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'";
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::GET_COLUMN_NAMES_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::GET_COLUMN_NAMES_SQL( const QString &table_name, const QString& database_name)
{
    Q_UNUSED (database_name);
    return QString("SELECT column_name FROM information_schema.columns WHERE table_name = '%1'").arg(table_name);
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::ADD_ONE_COLUMN_SQL
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::ADD_ONE_COLUMN_SQL ( int column_number, TABLE_STRUCT current_table )
{
    QString table_name           = current_table.name;
    COLUMN_STRUCT current_column = current_table.columns[column_number];
    QString column_name          = current_column.name;
    QString add_column_sentence  = QString("ALTER TABLE %1  ADD COLUMN %2 ").arg(table_name).arg(column_name);

    if ( current_column.is_array EQ true ) {
        add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT, 1 ) + " ";
        return add_column_sentence;
    }

    switch ( current_column.type ) {
        case INTEGER :
        case REAL    :
        case BLOB    :
        default      :
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type );
            break;

        case TEXT:
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + "(" + QVariant(current_column.length).toString() + ") ";
            break;
    }

    if ( current_column.is_not_null EQ true ) {
        add_column_sentence += " NOT NULL";
    }
    return add_column_sentence;
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::GET_COLUMN_TYPE_NAME
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::GET_COLUMN_TYPE_NAME ( COLUMN_TYPE p_column_type, int is_array )
{
    switch ( p_column_type ) {
        case INTEGER :
        default      :
            return "INTEGER";
        case TEXT:
            if ( is_array EQ 1 ) {
                return "BYTEA";
            }
            return "VARCHAR";
        case REAL:
            return "DOUBLE PRECISION";
        case BLOB:
            return "BYTEA";
    }
    return "";
}


/**************************************************************************************
                   POSTGRESSQL_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME()
{
    return "postgres";
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::GET_SQL_DRIVER_ENUM
***************************************************************************************/

DB_DRIVER POSTGRESSQL_PLUGIN::GET_SQL_DRIVER_ENUM()
{
    return PSQL;
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::GET_SQL_DRIVER_NAME
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::GET_SQL_DRIVER_NAME()
{
    return "QPSQL";
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::SQL_PREPARE_QUERY
***************************************************************************************/

void POSTGRESSQL_PLUGIN::SQL_PREPARE_QUERY(QString *table_name, QString *id_column_name, QString *column_names, QString *where_condition, int sql_operation)
{
    Q_UNUSED(table_name);
    Q_UNUSED(id_column_name);
    Q_UNUSED(column_names);
    Q_UNUSED(sql_operation);

    where_condition->replace("like" , "ILIKE" , Qt::CaseSensitive );
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::CREATE_SELECT_QUERY
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CREATE_SELECT_QUERY(QString *table_name, QString *column_names, QString *where_condition, QString *group_by, QString *order_by, int baslanilacak_satir, int secilecek_satir_sayisi)
{
    QString query = "";
    query.append(QString("SELECT " + *column_names   ));
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

    if ( secilecek_satir_sayisi > 0 ) {
        query.append(QString(" LIMIT  "   + QVariant (secilecek_satir_sayisi).toString() + " OFFSET "   + QVariant (baslanilacak_satir).toString()));
    }

    return query;
}

/**************************************************************************************
                   POSTGRESSQL_PLUGIN::CREATE_INSERT_QUERY
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CREATE_INSERT_QUERY(QString *table_name, QString *id_column_name, QString *insert_column_names, QString *insert_column_values)
{
    Q_UNUSED(insert_column_values);

    QString query = "";
    query.append(QString("INSERT INTO " + *table_name          ));
    query.append(QString(" ( " + *insert_column_names + " ) "  ));

    QString values_str = "";

    QStringList columns_names = insert_column_names->split(",");

    for ( int i = 0 ; i < columns_names.size() ; i++ ) {
        if (values_str.isEmpty() EQ false ) {
            values_str.append(",");
        }

        QString column_name = CLEAR_UNWANTED_CHARACTERS(columns_names.at(i));

        values_str.append(QString(":" + column_name ));
    }

    query.append(QString(" VALUES( "+ values_str + " ) " ));

    query.append(" RETURNING " + *id_column_name );

    return query;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

bool POSTGRESSQL_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT()
{
    return true;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER(const QString &table_name,const QString &, const QString &insert_column_names)
{
    return QString ( "INSERT INTO "+ table_name +" ("+ insert_column_names +") VALUES" );
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CREATE_MULTIPLE_INSERT_BODY(const QString &,const QString &, const QString &, const QString &insert_column_values)
{
return QString ("(" + insert_column_values + ")");
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER(const QString &,const QString &, const QString &)
{
    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_MULTIPLE_INSERT_DIVIDER
***************************************************************************************/

QString POSTGRESSQL_PLUGIN::GET_MULTIPLE_INSERT_DIVIDER()
{
    return ",";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

int POSTGRESSQL_PLUGIN::GET_ID_USING_DRIVER ( QSqlQuery * query )
{
    int insert_id = -1;

    int position = query->at();
    if ( query->next() EQ true ) {
        if ( query->record().value(0).isValid() EQ true ) {
            insert_id = query->record().value(0).toInt();
        }
        query->seek (position);
    }

    return insert_id;
}
