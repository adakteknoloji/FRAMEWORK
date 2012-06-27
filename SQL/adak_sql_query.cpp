#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QStringList>
#include <QMapIterator>
// #include <iostream>

// using namespace std;

#include "adak_sql.h"
#include "sql_plugins.h"
#include "adak_sql_query.h"
#include "adak_sql_error.h"
#include <string.h>

/***********************************************************/
/*                    SQL_QUERY::SQL_QUERY                 */
/***********************************************************/

SQL_QUERY::SQL_QUERY (ADAK_SQL * db) : QSqlQuery (db->m_db_connection)
{
    m_db           = db;
    p_struct_id    = -1;
    p_table_id     = -1;  
}

/***********************************************************/
/*                    SQL_QUERY::~SQL_QUERY                */
/***********************************************************/

SQL_QUERY::~SQL_QUERY ()
{
}

/***********************************************************/
/*                 SQL_QUERY::PREPARE_INSERT               */
/***********************************************************/

void SQL_QUERY::PREPARE_INSERT (const QString &table_name, const QString &id_column_name, const QString &insert_column_names)
{
    m_bind_values_names.clear();
    m_bind_values.clear();

    m_table_name         = table_name;
    m_id_column_name     = id_column_name;
    m_data_column_names  = insert_column_names;
    m_data_column_values = insert_column_names;

    m_db->SQL_PREPARE_QUERY(&m_table_name,&m_id_column_name,&m_data_column_names,new QString(""),SQL_INSERT);
}

/***********************************************************/
/*                    SQL_QUERY::INSERT                    */
/***********************************************************/

int SQL_QUERY::INSERT ()
{
    QString query = m_db->CREATE_INSERT_QUERY(&m_table_name,&m_id_column_name,&m_data_column_names,&m_data_column_values);

    prepare(query);

    if (m_db->m_sql_transaction_counter <= 0) {
        ADAK_SQL_ERROR( m_db->m_db_connection, TRANSACTION_ERROR, ERROR_IN_INSERT, this);
    }

    BIND_ALL_VALUES();

    if (EXEC() EQ false ) {
        m_db->CANCEL_TRANSACTION();
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_INSERT , this);
    }

    int insert_id = -1;

    insert_id = m_db->GET_ID_USING_DRIVER( this );

    if ( insert_id EQ -1 ) {
        insert_id = TRY_TO_GET_ID_USING_QT();
    }

    // Dikkat !.. insert_id ' yi almadan finish() cagrilmamali
    finish();

    m_db->SQL_FINALIZE_QUERY(SQL_INSERT);

    return insert_id;
}

/***********************************************************/
/*                 SQL_QUERY::PREPARE_UPDATE               */
/***********************************************************/

void SQL_QUERY::PREPARE_UPDATE (const QString &table_name, const QString &id_column_name, const QString &update_column_names, const QString &where_condition)
{
    m_bind_values_names.clear();
    m_bind_values.clear();

    m_table_name         = table_name;
    m_id_column_name     = id_column_name;
    m_data_column_names  = update_column_names;
    m_data_column_values = update_column_names;
    m_where_condition    = where_condition;

    m_db->SQL_PREPARE_QUERY(&m_table_name,&m_id_column_name,&m_data_column_names,&m_where_condition,SQL_UPDATE);
}

/***********************************************************/
/*                    SQL_QUERY::UPDATE                    */
/***********************************************************/

void SQL_QUERY::UPDATE ()
{
    QString query = m_db->CREATE_UPDATE_QUERY(&m_table_name,&m_id_column_name,&m_data_column_names,&m_data_column_values,&m_where_condition);

    prepare(query);

    if (m_db->m_sql_transaction_counter <= 0) {
        ADAK_SQL_ERROR ( m_db->m_db_connection, TRANSACTION_ERROR, ERROR_IN_UPDATE, this);
    }

    BIND_ALL_VALUES();

    if (EXEC() EQ false ) {
        m_db->CANCEL_TRANSACTION();
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_UPDATE , this);
    }

    finish();

    m_db->SQL_FINALIZE_QUERY(SQL_UPDATE);
}

/***********************************************************/
/*                 SQL_QUERY::PREPARE_DELETE               */
/***********************************************************/

void SQL_QUERY::PREPARE_DELETE (const QString &table_name, const QString &where_condition)
{
    m_bind_values_names.clear();
    m_bind_values.clear();

    m_table_name         = table_name;
    m_where_condition    = where_condition;

    m_db->SQL_PREPARE_QUERY(&m_table_name,new QString(""),new QString(""),&m_where_condition,SQL_DELETE);
}

/***********************************************************/
/*                    SQL_QUERY::DELETE                    */
/***********************************************************/

void SQL_QUERY::DELETE ()
{
    QString query = m_db->CREATE_DELETE_QUERY(&m_table_name,&m_where_condition);

    prepare(query);

    if (m_db->m_sql_transaction_counter <= 0) {
        ADAK_SQL_ERROR ( m_db->m_db_connection, TRANSACTION_ERROR, ERROR_IN_DELETE, this);
    }

    BIND_ALL_VALUES();

    if (EXEC() EQ false ) {
        m_db->CANCEL_TRANSACTION();
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_DELETE , this);
    }

    finish();

    m_db->SQL_FINALIZE_QUERY(SQL_DELETE);
}

/***********************************************************/
/*                 SQL_QUERY::PREPARE_SELECT               */
/***********************************************************/

void SQL_QUERY::PREPARE_SELECT (const QString &table_name, const QString &column_names)
{
    PREPARE_SELECT(table_name,column_names,"","");
}

/***********************************************************/
/*                 SQL_QUERY::PREPARE_SELECT               */
/***********************************************************/

void SQL_QUERY::PREPARE_SELECT (const QString &table_name, const QString &column_names, const QString &where_condition)
{
    PREPARE_SELECT(table_name,column_names,where_condition,"");
}

/***********************************************************/
/*                 SQL_QUERY::PREPARE_SELECT               */
/***********************************************************/

void SQL_QUERY::PREPARE_SELECT (const QString &table_name, const QString &column_names, const QString &where_condition, const QString &group_by)
{
    m_bind_values_names.clear();
    m_bind_values.clear();

    m_table_name         = table_name;
    m_data_column_names  = column_names;
    m_where_condition    = where_condition;
    m_group_by           = group_by;

    m_db->SQL_PREPARE_QUERY(&m_table_name,new QString(""),&m_data_column_names,&m_where_condition,SQL_SELECT);
}

/***********************************************************/
/*                    SQL_QUERY::SELECT                    */
/***********************************************************/

int SQL_QUERY::SELECT ()
{
    QString query = m_db->CREATE_SELECT_QUERY(&m_table_name,&m_data_column_names,&m_where_condition,&m_group_by,new QString(""),-1,-1);

    m_db->m_sql_active_query_counter++;

    prepare(query);

    BIND_ALL_VALUES();

    if ( EXEC() EQ false ) {
        ADAK_SQL_ERROR ( m_db->m_db_connection,  SQL_QUERY_ERROR, ERROR_IN_SELECT, this);
    }

    return NUM_OF_ROWS();
}

/***********************************************************/
/*                    SQL_QUERY::SELECT                    */
/***********************************************************/

int SQL_QUERY::SELECT (const QString &order_by)
{
    QString order_by_column = order_by;

    QString query = m_db->CREATE_SELECT_QUERY(&m_table_name,&m_data_column_names,&m_where_condition,&m_group_by,&order_by_column,-1,-1);

    m_db->m_sql_active_query_counter++;

    prepare(query);

    BIND_ALL_VALUES();    

    if ( EXEC() EQ false ) {
        ADAK_SQL_ERROR ( m_db->m_db_connection,  SQL_QUERY_ERROR, ERROR_IN_SELECT, this);
    }

    return NUM_OF_ROWS();
}

/***********************************************************/
/*                    SQL_QUERY::SELECT                    */
/***********************************************************/

int SQL_QUERY::SELECT (const QString &order_by, int baslanilacak_satir, int secilecek_satir_sayisi)
{
    QString order_by_column = order_by;

    QString query = m_db->CREATE_SELECT_QUERY(&m_table_name,&m_data_column_names,&m_where_condition,&m_group_by,&order_by_column,baslanilacak_satir,secilecek_satir_sayisi);

    m_db->m_sql_active_query_counter++;

    prepare(query);

    BIND_ALL_VALUES();

    if ( EXEC() EQ false ) {
        ADAK_SQL_ERROR ( m_db->m_db_connection,  SQL_QUERY_ERROR, ERROR_IN_SELECT, this);
    }

    return NUM_OF_ROWS();
}

/***********************************************************/
/*                    SQL_QUERY::BIND_ALL_VALUES           */
/***********************************************************/

void SQL_QUERY::BIND_ALL_VALUES ()
{
    QString query = lastQuery();

    for ( int i = 0 ; i < m_bind_values_names.size() ; i++ ) {
        if ( query.contains(m_bind_values_names.at(i),Qt::CaseInsensitive) EQ false ) {
            ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_SET_VALUE_EXTRA, this, "***** Gereksiz SET_VALUE --> " +  m_bind_values_names.at(i) + " ********");
        }
    }

    for ( int i = 0 ; i < m_bind_values_names.size() ; i++ ) {
        bindValue (QString( m_bind_values_names.at(i)), m_bind_values.at(i) );
    }
}

/***********************************************************/
/*                    SQL_QUERY::SET_LIKE                  */
/***********************************************************/

void SQL_QUERY::SET_LIKE (const QString& definition, const QVariant& value)
{
    QString set_value = value.toString();

    if (set_value EQ "*" OR set_value EQ "%") {
        set_value.clear();
    }

    SET_VALUE(definition,QString( "%" + set_value + "%" ));
}

/***********************************************************/
/*                    SQL_QUERY::SET_VALUE                 */
/***********************************************************/

void SQL_QUERY::SET_VALUE (const QString& definition, const QVariant& value)
{
    if ( definition.contains(":") EQ false ) {
        QString text = QString("<br>Tablo Name: %1 <br>").arg(m_table_name);

        if ( m_data_column_names.isEmpty() EQ false ) {
            text.append(QString("Column Name: %1 <br>").arg(m_data_column_names));
        }

        if ( m_where_condition.isEmpty() EQ false ) {
            text.append(QString("Where Condition:%1<br>").arg(m_where_condition));
        }

        text.append(QString("olan sorgunun SET_VALUE methodlarından birinde '%1' atanırken ':' unutulmuş.").arg(definition.trimmed()));

        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_SET_VALUE_EXTRA, NULL, text);
    }

    m_bind_values_names << definition.trimmed ();
    m_bind_values       << value;
}

/***********************************************************/
/*                    SQL_QUERY::SET_VALUE                 */
/***********************************************************/

void SQL_QUERY::SET_VALUE (const int insert_sira, const QVariant &value )
{
    bindValue(insert_sira,value);
}

/***********************************************************/
/*                    SQL_QUERY::EXEC                      */
/***********************************************************/

bool SQL_QUERY::EXEC (const QString& query)
{
    bool return_value = exec(query);

    //if ( return_value EQ false ) {
         //ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_EXEC_DDL, this);
    //}   // Debug icin gerekebilir diye tutuluyor

    return return_value;
}

/***********************************************************/
/*                    SQL_QUERY::EXEC                      */
/***********************************************************/

bool SQL_QUERY::EXEC  ()
{
    QMapIterator<QString, QVariant> BV(boundValues());
    while (BV.hasNext()) {
        BV.next();
        if (BV.value().isValid() EQ false) {
            if ( BV.key().isEmpty() EQ true AND BV.value().toString().isEmpty() EQ true ) {
                continue;
            }
            QString key_name = BV.key ();
            if (key_name.isEmpty () EQ false) {
                ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_SET_VALUE_MISSING, this, "***** UNUTULAN Deger --> " + BV.key() + " ********");
            }
            ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_SET_VALUE_EXTRA, this, "***** Gereksiz SET_VALUE --> " +  QVariant(BV.value()).toString () + " ********");
        }
    }

    m_bind_values_names.clear();
    m_bind_values.clear();

    return exec();
}

/***********************************************************/
/*                    SQL_QUERY::FINISH                    */
/***********************************************************/

void SQL_QUERY::FINISH ()
{
    if (isSelect() EQ true) {
        m_db->m_sql_active_query_counter--;
        finish();
    }
}

/***********************************************************/
/*                 SQL_QUERY::RESTART_NEXT                 */
/***********************************************************/

void SQL_QUERY::RESTART_NEXT ()
{
    if (isSelect() EQ true) {
        seek (-1);
    }
}

/***********************************************************/
/*                    SQL_QUERY::NUM_OF_ROWS               */
/***********************************************************/

int SQL_QUERY::NUM_OF_ROWS ()
{
    int       num_of_rows = 0;

    switch (m_db->GET_SQL_DRIVER_ENUM()) {
        case MYSQL :
        case PSQL  :
            num_of_rows = size();
	    break;
        default     :
        {
            int       position = at();
            seek (-1);
            while (next()) {
                num_of_rows++;
            }
            seek (position);
            return num_of_rows;
        }
        break;
    }

    if (num_of_rows < 0) {
        num_of_rows = 0; // -1 dondurdugunde de 0 yap
    }
    return num_of_rows;
}

/***********************************************************/
/*                    SQL_QUERY::NEXT                      */
/***********************************************************/

bool SQL_QUERY::NEXT ()
{
    if (isSelect() EQ false) {
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_NEXT, this);
    }
    bool return_value = next();
    p_record = record();

    return return_value;
}

/***********************************************************/
/*                    SQL_QUERY::VALUE                     */
/***********************************************************/

QVariant SQL_QUERY::VALUE (int sutun_no)
{
    if (at() < 0) {
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_VALUE, this);
    }

    QVariant ret_value = p_record.value(sutun_no);
    if (ret_value.isValid() EQ false) {
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_VALUE, this);
    }
    return ret_value;
}

/***********************************************************/
/*                    SQL_QUERY::VALUE                     */
/***********************************************************/

QVariant SQL_QUERY::VALUE (const QString& column_name)
{
    if (at() < 0) {
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_VALUE, this);
    }
    QVariant ret_value = p_record.value(column_name);
    if (ret_value.isValid() EQ false) {
        ADAK_SQL_ERROR( m_db->m_db_connection, SQL_QUERY_ERROR, ERROR_IN_VALUE, this);
    }
    return ret_value;
}

/***********************************************************/
/*                    SQL_QUERY::VALUE                     */
/***********************************************************/

int SQL_QUERY::TRY_TO_GET_ID_USING_QT()
{
    return (lastInsertId().toInt());
}

/**************************************************************************************
                        SQL_QUERY::PARANTEZ_AC
***************************************************************************************/

void SQL_QUERY::PARANTEZ_AC()
{
    m_where_condition.append(" ( ");
}

/**************************************************************************************
                        SQL_QUERY::PARANTEZ_KAPA
***************************************************************************************/

void SQL_QUERY::PARANTEZ_KAPA()
{
    m_where_condition.append(" ) ");
}

/**************************************************************************************
                        SQL_WHERE::AND_EKLE
***************************************************************************************/

void SQL_QUERY::AND_EKLE(QString where_string)
{
    if ( m_where_condition.isEmpty() EQ false ) {
        m_where_condition.append(" AND ");
    }
    m_where_condition.append(where_string);
}

/**************************************************************************************
                        SQL_WHERE::OR_EKLE
***************************************************************************************/

void SQL_QUERY::OR_EKLE(QString where_string)
{
    if ( m_where_condition.isEmpty() EQ false ) {
        m_where_condition.append(" OR ");
    }
    m_where_condition.append(where_string);
}

/**************************************************************************************
                        SQL_WHERE::ADD_MULTI
***************************************************************************************/

void SQL_QUERY::ADD_MULTI (QString column_id_name, QList<QVariant> values, ADAK_QUERY_SPLITTER_TYPES splitter)
{
    QString where_str = "(";

    QString value_name = column_id_name;
    if ( column_id_name.contains(".") EQ true ) {
        int dot_position = column_id_name.indexOf( "." ) + 1;
        value_name = column_id_name.mid( dot_position, column_id_name.size() );
    }

    for ( int i = 0 ; i < values.size() ; i ++ ) {
        if ( i NE 0 ) {
            if ( splitter EQ ADAK_OR ) {
                where_str.append(" OR ");
            }
            else if ( splitter EQ ADAK_AND ) {
                where_str.append(" AND ");
            }
        }
        // eger gelen degerde . varsa onu almiyoruz sql kabul etmiyor
        QString set_value_name = QString(":%1_value_%2").arg( value_name ).arg(i);

        SET_VALUE( set_value_name , values.at(i) );

        where_str.append(QString("%1=%2").arg(column_id_name).arg(set_value_name));
    }

    where_str.append(")");

    m_where_condition.append(where_str);
}

/**************************************************************************************
                        SQL_WHERE::AND_MULTI_EKLE
***************************************************************************************/

void SQL_QUERY::AND_MULTI_EKLE(QString column_id_name, QList<QVariant> values, ADAK_QUERY_SPLITTER_TYPES splitter)
{
    if ( values.size() EQ 0 ) {
        return;
    }

    if ( m_where_condition.isEmpty() EQ false ) {
        m_where_condition.append(" AND ");
    }

    ADD_MULTI (column_id_name,values,splitter);
}

/**************************************************************************************
                        SQL_WHERE::OR_MULTI_EKLE
***************************************************************************************/

void SQL_QUERY::OR_MULTI_EKLE(QString column_id_name, QList<QVariant> values, ADAK_QUERY_SPLITTER_TYPES splitter)
{
    if ( values.size() EQ 0 ) {
        return;
    }

    if ( m_where_condition.isEmpty() EQ false ) {
        m_where_condition.append(" OR ");
    }

    ADD_MULTI (column_id_name,values,splitter);
}

/**************************************************************************************
                        SQL_WHERE::TO_QVARIANT
***************************************************************************************/

QList<QVariant> SQL_QUERY::TO_QVARIANT(QList<int> values)
{
    QList<QVariant> degerler;

    for ( int i = 0 ; i < values.size() ; i++ ) {
        degerler << values.at(i);
    }

    return degerler;
}

/**************************************************************************************
                        SQL_WHERE::TO_QVARIANT
***************************************************************************************/

QList<QVariant> SQL_QUERY::TO_QVARIANT(QStringList values)
{
    QList<QVariant> degerler;

    for ( int i = 0 ; i < values.size() ; i++ ) {
        degerler << values.at(i);
    }

    return degerler;
}
