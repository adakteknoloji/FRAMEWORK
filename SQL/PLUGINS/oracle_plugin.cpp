#include "adak_sql_struct.h"
#include "adak_utils.h"
#include "adak_sql_error.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "sql_ddl.h"

#include "oracle_plugin.h"
#include "sql_plugins.h"

/**************************************************************************************
                   ORACLE_PLUGIN::ORACLE_PLUGIN
***************************************************************************************/

ORACLE_PLUGIN::ORACLE_PLUGIN(QList<ADAK_SQL_STRUCT *> adak_sql_struct, QString db_name) : ADAK_SQL(adak_sql_struct, db_name)
{
    m_next_insert_id = -1;    
}

/**************************************************************************************
                   ORACLE_PLUGIN::~ORACLE_PLUGIN
***************************************************************************************/

ORACLE_PLUGIN::~ORACLE_PLUGIN()
{

}

/**************************************************************************************
                   ORACLE_PLUGIN::LOCK_ROW
***************************************************************************************/

int ORACLE_PLUGIN::LOCK_ROW ( const QString ,const QString table_name, const QString where_info )
{
    Q_UNUSED(table_name);
    Q_UNUSED(where_info);

    //if ( sql_query.size() <= 0 ) {
    //    return ADAK_FAIL;
    //}

    return ADAK_OK;
}

/**************************************************************************************
                   ORACLE_PLUGIN::UNLOCK_ROW
***************************************************************************************/

void ORACLE_PLUGIN::UNLOCK_ROW ( const QString ,const QString table_name, const QString where_info )
{
    Q_UNUSED(table_name);
    Q_UNUSED(where_info);
}

/**************************************************************************************
                   ORACLE_PLUGIN::CONNECT_TO_DATABASE
***************************************************************************************/

int ORACLE_PLUGIN::CONNECT_TO_DATABASE ( const QString db_name )
{
    QString temp_db_name = "";

    if ( db_name.isEmpty() EQ true ) {
        temp_db_name = m_db_name;
    }
    else {
        temp_db_name = db_name;
    }

    QSqlQuery sql_query ( m_db_connection );

    if ( sql_query.exec ( QString ("ALTER SESSION SET CURRENT_SCHEMA = %1").arg ( temp_db_name ) ) EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   ORACLE_PLUGIN::OPEN_CREATE_TABLE_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::OPEN_CREATE_TABLE_SQL( const QString &table_name )
{
    m_auto_increment_id_column_name.clear();
    return "CREATE TABLE IF NOT EXISTS " + table_name + " ( ";
}

/**************************************************************************************
                   ORACLE_PLUGIN::CLOSE_CREATE_TABLE_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::CLOSE_CREATE_TABLE_SQL ( TABLE_STRUCT current_table )
{
    int index_count = current_table.index_count;

    QString index_sentence = "\n )";

    if ( index_count NE 1 ) {
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
            index_sentence += " )";
        }
    }

    if ( m_auto_increment_id_column_name.isEmpty() EQ false ) {
        index_sentence += "\n";
        CREATE_AUTO_INC_ID_COLUMN_FOR_ORACLE ( current_table.name );
    }

    return index_sentence;
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_COLUMN_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_COLUMN_SQL( QString const& column_name,
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
        return column_sentence;
    }

    switch (column_type) {
        case INTEGER :
        default      :

            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) + " ";

            if ( is_auto_increment EQ true AND is_not_null EQ true ) {
                m_auto_increment_id_column_name = column_name;
                //column_sentence += " NOT NULL";
            }
            break;

        case TEXT:
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) + "(" + QVariant(length).toString() + ")";
            break;

        case REAL:
        case BLOB:
            column_sentence += " " + GET_COLUMN_TYPE_NAME ( column_type ) + " ";
            break;
    }

    return column_sentence;
}

/**************************************************************************************
                   ORACLE_PLUGIN::DROP_TABLE_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::DROP_TABLE_SQL ( const QString& table_name )
{
    return "DROP TABLE " + table_name;
}

/**************************************************************************************
                   ORACLE_PLUGIN::GET_TABLE_NAMES_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::GET_TABLE_NAMES_SQL()
{
    return "SELECT table_name FROM all_tables";
}

/**************************************************************************************
                   ORACLE_PLUGIN::GET_COLUMN_NAMES_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::GET_COLUMN_NAMES_SQL( const QString &table_name, const QString& )
{
    return QString("SELECT COLUMN_NAME FROM user_tab_columns WHERE table_name='%1'").arg(table_name);
}

/**************************************************************************************
                   ORACLE_PLUGIN::ADD_ONE_COLUMN_SQL
***************************************************************************************/

QString ORACLE_PLUGIN::ADD_ONE_COLUMN_SQL ( int column_number, TABLE_STRUCT current_table )
{
    QString table_name           = current_table.name;
    COLUMN_STRUCT current_column = current_table.columns[column_number];
    QString column_name          = current_column.name;
    QString add_column_sentence  = QString("ALTER TABLE %1 ADD %2 ").arg(table_name).arg(column_name);

    if ( current_column.is_array EQ true ) {
        add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( TEXT , 1 ) + " ";
        return add_column_sentence;
    }

    switch ( current_column.type ) {
        case INTEGER :
        case REAL    :
        case BLOB    :
        default      :
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + " ";
            break;

        case TEXT:
            add_column_sentence += " " + GET_COLUMN_TYPE_NAME ( current_column.type ) + "(" + QVariant ( current_column.length ).toString() + ") ";
            break;
    }

    if ( current_column.is_not_null EQ true ) {
        add_column_sentence += " NOT NULL";
    }
    return add_column_sentence;
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_AUTO_INC_ID_COLUMN_FOR_ORACLE
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_AUTO_INC_ID_COLUMN_FOR_ORACLE ( const QString &table_name )
{

    // Oracle da auto increment column yaratmak icin asagidaki kodun tablo yaratma syntax indan sonra yerlestirilmesi
    // gerekiyor. CLOSE_CREATE_TABLE_SQL stringine bu fonksiyonun geri dondurdugu stringi ekleyerek bu 'slem' gerceklestiriyoruz.

    m_sql_trigger_list << "CREATE SEQUENCE s" + table_name + " START WITH 1 INCREMENT BY 1 NOMAXVALUE";

    return "";
}

/**************************************************************************************
                   ORACLE_PLUGIN::GET_COLUMN_TYPE_NAME
***************************************************************************************/

QString ORACLE_PLUGIN::GET_COLUMN_TYPE_NAME ( COLUMN_TYPE p_column_type, int is_array )
{
    switch ( p_column_type ) {
        case INTEGER :
        default      :
            return "NUMBER(10,0)";
        case TEXT:
            if ( is_array EQ 1 ) {
                 return "BLOB";
	    }
            return "NVARCHAR2";
        case REAL:
            return "NUMBER(10,2)";
        case BLOB:
            return "BLOB";
    }
    return "";
}

/**************************************************************************************
                   ORACLE_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME
***************************************************************************************/

QString ORACLE_PLUGIN::GET_SQL_DRIVER_DEFAULT_DB_NAME()
{
    return SQL_GET_DB_SID();
}

/**************************************************************************************
                   ORACLE_PLUGIN::GET_SQL_DRIVER_ENUM
***************************************************************************************/

DB_DRIVER ORACLE_PLUGIN::GET_SQL_DRIVER_ENUM()
{
    return ORACLE;
}


/**************************************************************************************
                   ORACLE_PLUGIN::GET_SQL_DRIVER_NAME
***************************************************************************************/

QString ORACLE_PLUGIN::GET_SQL_DRIVER_NAME()
{
    return "QOCI";
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_SELECT_QUERY
***************************************************************************************/

void ORACLE_PLUGIN::SQL_PREPARE_QUERY(QString *table_name, QString *id_column_name, QString *column_names, QString *where_condition, int sql_operation)
{
    Q_UNUSED(table_name);
    Q_UNUSED(id_column_name);
    Q_UNUSED(column_names);
    Q_UNUSED(where_condition);
    Q_UNUSED(sql_operation);
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_SELECT_QUERY
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_SELECT_QUERY(QString *table_name, QString *column_names, QString *where_condition, QString *group_by, QString *order_by, int baslanilacak_satir, int secilecek_satir_sayisi)
{
    QString query            = "";
    QString limit_condition  = "";

    query.append(QString("SELECT " + *column_names   ));

    query.append(QString(" FROM " + *table_name      ));

    if ( where_condition->isEmpty() EQ false ) {
        query.append(QString(" WHERE " + *where_condition));
    }

    if ( (*order_by).contains(" DESC",Qt::CaseInsensitive) EQ false ) {
        if ( secilecek_satir_sayisi > 0 ) {
            limit_condition.append(QString( "( ROWNUM > "     + QVariant (baslanilacak_satir).toString() +
                                            " AND ROWNUM < "  + QVariant (baslanilacak_satir + secilecek_satir_sayisi + 1).toString() + " )"));
            if ( query.contains("WHERE",Qt::CaseInsensitive) EQ true ) {
                query.append(" AND ");
            }
            else {
                query.append(" WHERE ");
            }

            query.append(limit_condition);
        }
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
                   ORACLE_PLUGIN::CREATE_INSERT_QUERY
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_INSERT_QUERY(QString *table_name, QString *id_column_name, QString *insert_column_names, QString *insert_column_values)
{
    Q_UNUSED(insert_column_values);

    SQL_DDL sql_query ( this );

    if ( sql_query.EXEC_SQL("SELECT s" + CLEAR_UNWANTED_CHARACTERS(*table_name) + ".nextval FROM dual" ) EQ false ) {
        return "";
    }

    sql_query.NEXT();

    m_next_insert_id = sql_query.VALUE(0).toInt();

    QString query = "";
    query.append(QString("INSERT INTO " + *table_name          ));
    query.append(QString(" ( " + *insert_column_names + "," + *id_column_name + " ) "  ));

    QString values_str = "";

    QStringList columns_names = insert_column_names->split(",");

    for ( int i = 0 ; i < columns_names.size() ; i++ ) {
        if (values_str.isEmpty() EQ false ) {
            values_str.append(",");
        }

        QString column_name = CLEAR_UNWANTED_CHARACTERS(columns_names.at(i));

        values_str.append(QString(":" + column_name));
    }

    query.append(QString(" VALUES( "+ values_str + "," + QVariant(m_next_insert_id).toString() +" ) " ));

    return query;
}

/**************************************************************************************
                   ORACLE_PLUGIN::GET_ID_USING_DRIVER
***************************************************************************************/

int ORACLE_PLUGIN::GET_ID_USING_DRIVER( QSqlQuery *  )
{
    return m_next_insert_id;
}

/**************************************************************************************
                   ORACLE_PLUGIN::SQL_FINALIZE_QUERY
***************************************************************************************/

void ORACLE_PLUGIN::SQL_FINALIZE_QUERY(int sql_operation)
{
    Q_UNUSED(sql_operation);

    m_next_insert_id = -1;
}


/**************************************************************************************
                   ORACLE_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT
***************************************************************************************/

bool ORACLE_PLUGIN::IS_DB_HAS_MULTIPLE_INSERT()
{
    return false;
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_MULTIPLE_INSERT_HEADER(const QString &,const QString &, const QString &)
{
    return QString("");
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_MULTIPLE_INSERT_BODY
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_MULTIPLE_INSERT_BODY(const QString &table_name,const QString &id_column_name, const QString &insert_column_names, const QString &insert_column_values)
{
    SQL_DDL sql_query ( this );

    if ( sql_query.EXEC_SQL("SELECT s" + table_name + ".nextval FROM dual" ) EQ false ) {
        return "";
    }

    sql_query.NEXT();

    int insert_id = sql_query.VALUE(0).toInt();

    QString query = "";

    query.append(QString("INSERT INTO " + table_name          ));
    query.append(QString(" ( " + insert_column_names + "," + id_column_name + " ) "  ));
    query.append(QString(" VALUES( "+ insert_column_values + "," + QVariant(insert_id).toString() +" ) " ));

    return query;
}

/**************************************************************************************
                   ORACLE_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER
***************************************************************************************/

QString ORACLE_PLUGIN::CREATE_MULTIPLE_INSERT_FOOTER(const QString &,const QString &, const QString &)
{
    return QString("");
}
