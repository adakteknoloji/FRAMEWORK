#include <QStringList>
#include <QSqlError>
#include "adak_sql.h"
#include "adak_sql_query.h"
#include "adak_sql_error.h"
#include "adak_sql_struct.h"
#include "adak_sql_abstract.h"
#include "adak_utils.h"

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::ADAK_SQL_ABSTRACT
***************************************************************************************/

ADAK_SQL_ABSTRACT::ADAK_SQL_ABSTRACT( QList<ADAK_SQL_STRUCT *> adak_sql_struct ,QString db_name )
{
    //DBler ve program_ids ler setlenir.
    m_adak_sql_struct           = adak_sql_struct;

    for ( int i = 0 ; i < m_adak_sql_struct.size() ; i++ ) {
        m_db_program_ids << m_adak_sql_struct.at(i)->adak_program_id;
    }

    m_db_name                    = db_name;
    m_sql_transaction_counter    = 0;
    m_sql_active_query_counter   = 0;
    m_database_index_count       = 0;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::~ADAK_SQL_ABSTRACT
***************************************************************************************/

ADAK_SQL_ABSTRACT::~ADAK_SQL_ABSTRACT()
{
    REMOVE_CONNECT(m_db_name);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_CONNECTION_LAST_ERROR
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_CONNECTION_LAST_ERROR()
{
    return m_db_connection.lastError().text();
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_FIND_NUM_OF_TABLES_IN_DB
***************************************************************************************/

int ADAK_SQL_ABSTRACT::SQL_FIND_NUM_OF_TABLES_IN_DB ( )
{
    return 0;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CONNECT_TO_DATABASE
***************************************************************************************/

int ADAK_SQL_ABSTRACT::CONNECT_TO_DATABASE ( bool show_error_screen )
{

    if( m_db_connection.isOpen() EQ true ) {
        m_db_connection.close();
    }

    if ( QSqlDatabase::connectionNames().contains(m_db_name) EQ true ) {
        REMOVE_CONNECT(m_db_name);
    }

    m_db_connection = QSqlDatabase::addDatabase( GET_SQL_DRIVER_NAME(), m_db_name );

    m_db_connection.setDatabaseName( m_db_name     );
    m_db_connection.setPort        ( SQL_PORT()    );
    m_db_connection.setHostName    ( SQL_HOSTNAME());
    m_db_connection.setUserName    ( SQL_USERNAME());
    m_db_connection.setPassword    ( SQL_PASSWORD());

    if ( m_db_connection.open() EQ false ) {
        if ( show_error_screen EQ true ) {
            ADAK_SQL_ERROR( m_db_connection, CONNECTION_ERROR, ERROR_IN_CONSTRUCTOR, NULL);
        }
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::DISCONNECT_TO_DATABASE
***************************************************************************************/

int ADAK_SQL_ABSTRACT::DISCONNECT_TO_DATABASE ( )
{
    m_db_connection.close();

    return ADAK_OK;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_SQL_STRUCT
***************************************************************************************/

ADAK_SQL_STRUCT * ADAK_SQL_ABSTRACT::GET_SQL_STRUCT(int program_id)
{
    for ( int i = 0 ; i < m_adak_sql_struct.size() ; i++ ) {
        if ( program_id EQ m_adak_sql_struct.at(i)->adak_program_id ) {
            return m_adak_sql_struct.at(i);
        }
    }

    return NULL;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_DB_PROGRAM_IDS
***************************************************************************************/

QList<int> ADAK_SQL_ABSTRACT::GET_DB_PROGRAM_IDS()
{
    return m_db_program_ids;
}


/**************************************************************************************
                   ADAK_SQL_ABSTRACT::START_TRANSACTION
***************************************************************************************/

void ADAK_SQL_ABSTRACT::START_TRANSACTION ()
{
    if (m_sql_transaction_counter > 0) {
        ADAK_SQL_ERROR( m_db_connection, TRANSACTION_ERROR, ERROR_IN_START_TRANSACTION, NULL);
    }

    m_db_connection.transaction();

    m_sql_transaction_counter++;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::COMMIT_TRANSACTION
***************************************************************************************/

void ADAK_SQL_ABSTRACT::COMMIT_TRANSACTION ()
{
    if (m_sql_transaction_counter <= 0) {
        ADAK_SQL_ERROR( m_db_connection, TRANSACTION_ERROR, ERROR_IN_COMMIT_TRANSACTION, NULL);
    }
    m_db_connection.commit();

    m_sql_transaction_counter--;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CANCEL_TRANSACTION
***************************************************************************************/

void ADAK_SQL_ABSTRACT::CANCEL_TRANSACTION ()
{
    if (m_sql_transaction_counter <= 0) {
        ADAK_SQL_ERROR( m_db_connection, TRANSACTION_ERROR, ERROR_IN_CANCEL_TRANSACTION, NULL);
    }
    m_db_connection.rollback();

    m_sql_transaction_counter--;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::TRANSACTION_IS_ON
***************************************************************************************/

bool ADAK_SQL_ABSTRACT::TRANSACTION_IS_ON ()
{
    return (m_sql_transaction_counter > 0);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SET_TABLE_AUTO_INCREMENT_ID_VALUE
***************************************************************************************/

void ADAK_SQL_ABSTRACT::SET_TABLE_AUTO_INCREMENT_ID_VALUE ( const QString , const QString , int )
{

}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::LOCK_ROW
***************************************************************************************/

int ADAK_SQL_ABSTRACT::LOCK_ROW (const QString ,const QString , const QString )
{
    return ADAK_OK;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::UNLOCK_ROW
***************************************************************************************/

void ADAK_SQL_ABSTRACT::UNLOCK_ROW (const QString ,const QString , const QString )
{

}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_SQL_DRIVER_NAME
***************************************************************************************/

DB_DRIVER ADAK_SQL_ABSTRACT::GET_SQL_DRIVER_ENUM()
{
    return UNKNOWN;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_SQL_DRIVER_NAME
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_SQL_DRIVER_NAME()
{
    return "";
}


/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_SQL_DRIVER_DEFAULT_DB_NAME
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_SQL_DRIVER_DEFAULT_DB_NAME ()
{
    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_CREATE_TABLES
***************************************************************************************/

bool ADAK_SQL_ABSTRACT::SQL_CREATE_TABLES (ADAK_SQL_STRUCT *adak_sql_struct, QString db_name)
{
    QList<ADAK_SQL_STRUCT *> db_list;

    db_list << adak_sql_struct;

    return SQL_CREATE_TABLES(db_list,db_name);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_CREATE_TABLES
***************************************************************************************/

bool ADAK_SQL_ABSTRACT::SQL_CREATE_TABLES (QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name)
{

    if( db_name.isEmpty() EQ true ) {
        db_name = m_db_name;
    }

    if ( QSqlDatabase::connectionNames().contains( db_name ) EQ false ) {
        CONNECT_TO_DATABASE( false );
    }

    if ( adak_sql_struct.size() EQ 0 ) {
        adak_sql_struct = m_adak_sql_struct;
    }

    for ( int i = 0 ; i < adak_sql_struct.size() ; i++ ) {
        QStringList create_table_sql;

        create_table_sql = CREATE_ALL_TABLES_SQL( adak_sql_struct.at(i) );

        QSqlQuery create_table_query ( m_db_connection );

        for ( int i = 0; i < create_table_sql.count(); i++ ) {
            if ( create_table_query.exec ( create_table_sql[i] )  EQ false ) {
                ADAK_SQL_ERROR( m_db_connection, CREATION_ERROR, ERROR_IN_CREATE_TABLE, &create_table_query);
            }
        }

        for ( int i = 0 ; i < m_sql_trigger_list.size() ; i++ ) {
            if ( create_table_query.exec ( m_sql_trigger_list.at(i) )  EQ false ) {
                ADAK_SQL_ERROR( m_db_connection, CREATION_ERROR, ERROR_IN_CREATE_TABLE, &create_table_query);
            }
        }
    }

    return true;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_DROP_TABLES
***************************************************************************************/

bool ADAK_SQL_ABSTRACT::SQL_DROP_TABLES (ADAK_SQL_STRUCT *adak_sql_struct, QString db_name)
{
    QList<ADAK_SQL_STRUCT *> db_list;

    db_list << adak_sql_struct;

    return SQL_DROP_TABLES(db_list,db_name);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_DROP_TABLES
***************************************************************************************/

bool ADAK_SQL_ABSTRACT::SQL_DROP_TABLES (QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name)
{
    if ( db_name.isEmpty() EQ false ) {
        CONNECT_TO_DATABASE( false );
    }

    if ( adak_sql_struct.size() EQ 0 ) {
        adak_sql_struct = m_adak_sql_struct;
    }

    for ( int i = 0 ; i < adak_sql_struct.size() ; i++ ) {
        QStringList drop_table_sql_list;

        drop_table_sql_list      = DROP_ALL_TABLES_SQL( adak_sql_struct.at(i) );

        QSqlQuery drop_table_query ( m_db_connection );
        for ( int j = 0; j < adak_sql_struct.at(i)->table_count; j++ ) {
            if ( drop_table_query.exec ( drop_table_sql_list[j] )  EQ false ) {
                ADAK_SQL_ERROR( m_db_connection , DROP_ERROR, ERROR_IN_DROP_TABLES, &drop_table_query);
            }
        }
    }

    return true;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_DB_EXISTS
***************************************************************************************/

bool ADAK_SQL_ABSTRACT::SQL_TABLES_EXISTS ( const QString  )
{
    if ( SQL_FIND_NUM_OF_TABLES_IN_DB() > 0 ) {
        return true;
    }

    return false;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::OPEN_CREATE_TABLE_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::OPEN_CREATE_TABLE_SQL( const QString &table_name )
{
    Q_UNUSED(table_name);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CLOSE_CREATE_TABLE_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CLOSE_CREATE_TABLE_SQL ( TABLE_STRUCT current_table )
{
    Q_UNUSED(current_table);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_COLUMN_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CREATE_COLUMN_SQL( QString const& column_name, COLUMN_TYPE column_type, int length, bool is_auto_increment, bool is_not_null,bool is_array,INDEX_TYPE index_type )
{
    Q_UNUSED(column_name);
    Q_UNUSED(column_type);
    Q_UNUSED(length);
    Q_UNUSED(is_auto_increment);
    Q_UNUSED(is_not_null);
    Q_UNUSED(is_array);
    Q_UNUSED(index_type);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::DROP_TABLE_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::DROP_TABLE_SQL ( const QString& table_name )
{
    Q_UNUSED(table_name);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_TABLE_NAMES_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_TABLE_NAMES_SQL()
{
    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_COLUMN_NAMES_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_COLUMN_NAMES_SQL( const QString &table_name, const QString& database_name)
{
    Q_UNUSED(table_name);
    Q_UNUSED(database_name);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::ADD_ONE_COLUMN_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::ADD_ONE_COLUMN_SQL ( int column_number, TABLE_STRUCT current_table )
{
    Q_UNUSED(column_number);
    Q_UNUSED(current_table);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_COLUMN_TYPE_NAME
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_COLUMN_TYPE_NAME ( COLUMN_TYPE p_column_type, int is_array )
{
    Q_UNUSED(p_column_type);
    Q_UNUSED(is_array);

    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_COLUMN_TYPE_NAME
***************************************************************************************/

void ADAK_SQL_ABSTRACT::REMOVE_CONNECT(QString connection_name)
{
    QSqlDatabase db = QSqlDatabase::database(connection_name);

    if ( db.isValid() EQ false ) {
        return;
    }

    if ( QSqlDatabase::database(connection_name).isOpen() EQ true ) {
        QSqlDatabase::database(connection_name).close();
    }

    QSqlDatabase::removeDatabase(connection_name);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SET_CLIENT_CODEC
***************************************************************************************/

void ADAK_SQL_ABSTRACT::SET_CLIENT_CODEC()
{

}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_ALL_TABLES_SQL
***************************************************************************************/

QStringList ADAK_SQL_ABSTRACT::CREATE_ALL_TABLES_SQL ( ADAK_SQL_STRUCT * database_struct )
{
    QStringList tables_sql_list;
    int table_count = database_struct->table_count;
    for ( int i = 0; i < table_count; i++ ) {
        tables_sql_list.append( CREATE_ONE_TABLE_SQL( i, database_struct ));
    }
    return tables_sql_list;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_ONE_TABLE_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CREATE_ONE_TABLE_SQL ( int table_number, ADAK_SQL_STRUCT * database_struct )
{
    TABLE_STRUCT   current_table    = database_struct->tables[table_number];
    int     column_count            = current_table.column_count;
    QString create_table_sql        = OPEN_CREATE_TABLE_SQL( current_table.name);

    for ( int i = 0; i < column_count; i++ ) {
        COLUMN_STRUCT    current_column       = current_table.columns[i];
        INDEX_TYPE column_index_type          = NONE;
        if ( QString::compare( current_table.indexes[0].columns[0], current_column.name ) EQ 0 ) {
            column_index_type = PRIMARY;
        }
        QString   create_column_sql    = CREATE_COLUMN_SQL ( current_column.name,
                                                                     current_column.type,
                                                                     current_column.length,
                                                                     current_column.is_auto_increment,
                                                                     current_column.is_not_null,
                                                                     current_column.is_array,
                                                                     column_index_type );

        if ( i + 1 NE column_count ) {
            create_column_sql += ",";
        }

        create_table_sql += " " + create_column_sql;
    }

    create_table_sql += CLOSE_CREATE_TABLE_SQL( current_table );
    return create_table_sql;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::DROP_ALL_TABLES_SQL
***************************************************************************************/

QStringList ADAK_SQL_ABSTRACT::DROP_ALL_TABLES_SQL ( ADAK_SQL_STRUCT * database_struct )
{
    QStringList drop_database_list;

    for (int i = 0; i < database_struct->table_count; i ++ ) {
        drop_database_list.append( DROP_TABLE_SQL( database_struct->tables[i].name));
    }

    return drop_database_list;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_PREPARE_QUERY
***************************************************************************************/

void ADAK_SQL_ABSTRACT::SQL_PREPARE_QUERY(QString *table_name, QString *id_column_name, QString *column_names, QString *where_condition, int sql_operation)
{
    Q_UNUSED(table_name);
    Q_UNUSED(id_column_name);
    Q_UNUSED(column_names);
    Q_UNUSED(where_condition);
    Q_UNUSED(sql_operation);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::SQL_FINALIZE_QUERY
***************************************************************************************/

void ADAK_SQL_ABSTRACT::SQL_FINALIZE_QUERY(int sql_operation)
{
    Q_UNUSED(sql_operation);
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::DROP_ALL_TABLES_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CREATE_SELECT_QUERY (QString *table_name, QString *column_names, QString *where_condition, QString *group_by, QString *order_by, int baslanilacak_satir, int secilecek_satir_sayisi)
{
    QString query = "";
    query.append(QString("SELECT " + *column_names   ));
    query.append(QString(" FROM "  + *table_name      ));

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
        query.append(QString(" LIMIT "   + QVariant (secilecek_satir_sayisi).toString()));

    }

    if ( baslanilacak_satir >= 0 ) {
        query.append(QString(" OFFSET "   + QVariant (baslanilacak_satir).toString()));
    }


    return query;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_INSERT_QUERY
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CREATE_INSERT_QUERY (QString *table_name, QString *id_column_name, QString *insert_column_names, QString *insert_column_values)
{
    Q_UNUSED(id_column_name);
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

        values_str.append(QString(":" + column_name));
    }

    query.append(QString(" VALUES( "+ values_str + " ) " ));

    return query;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::DROP_ALL_TABLES_SQL
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CREATE_UPDATE_QUERY (QString *table_name, QString *id_column_name, QString *update_column_names, QString *update_column_values, QString *where_condition)
{
    Q_UNUSED(id_column_name);
    Q_UNUSED(update_column_values);

    QString query = "";
    query.append(QString("UPDATE " + *table_name          ));
    query.append(QString(" SET "                         ));

    QString values_str = "";

    QStringList columns_names = update_column_names->split(",");

    for ( int i = 0 ; i < columns_names.size() ; i++ ) {
        if (values_str.isEmpty() EQ false ) {
            values_str.append(",");
        }

        QString column_name = CLEAR_UNWANTED_CHARACTERS(columns_names.at(i));

        values_str.append(QString(column_name + "=:"+column_name));
    }

    query.append(" " + values_str);

    if ( where_condition->isEmpty() EQ false ) {
        query.append(QString(" WHERE " + *where_condition));
    }

    return query;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::CREATE_DELETE_QUERY
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CREATE_DELETE_QUERY (QString * table_name, QString * where_condition)
{
    QString query = "";
    query.append(QString("DELETE FROM " + *table_name     ));

    if ( where_condition->isEmpty() EQ false ) {
        query.append(QString(" WHERE " + *where_condition ));
    }

    return query;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_MULTIPLE_INSERT_DIVIDER
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::GET_MULTIPLE_INSERT_DIVIDER()
{
    return "";
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

int ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER ( QSqlQuery *  )
{
    return -1;
}

/**************************************************************************************
                   ADAK_SQL_ABSTRACT::GET_ID_USING_DRIVER
***************************************************************************************/

QString ADAK_SQL_ABSTRACT::CLEAR_UNWANTED_CHARACTERS(QString string_name)
{
    QString column_name = string_name;

    column_name.replace(" ","",Qt::CaseInsensitive);

    return column_name;
}
