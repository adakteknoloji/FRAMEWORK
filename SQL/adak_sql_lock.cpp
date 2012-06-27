#include "adak_sql.h"
#include "adak_sql_query.h"

/************************************************************/
/*               ADAK_SQL::LOCK_ROW                         */
/************************************************************/

int ADAK_SQL::LOCK_ROW (const QString id_column_name,const QString table_name, const QString where_info)
{
    switch (p_db_driver) {
        case SQLITE  :
            return ADAK_OK;

        case PSQL    :
            {
                SQL_QUERY sql_query      ( this );
                sql_query.PREPARE_SELECT ( QString ("SELECT pg_try_advisory_lock("+id_column_name+") FROM " + table_name + " WHERE " + where_info));
                if(sql_query.SELECT() EQ 0 ) {
                    return ADAK_FAIL;
                }
                sql_query.NEXT();
                if ( sql_query.VALUE(0).toBool() EQ false ) {

                    return ADAK_FAIL;
                }
                return ADAK_OK;
            }

        case MYSQL   :
            {
                SQL_QUERY sql_query      ( this );
                sql_query.PREPARE_SELECT ( QString ("SELECT GET_LOCK(\""+table_name + "." + where_info+"\",2)"));
                if(sql_query.SELECT() EQ 0) {
                    return ADAK_FAIL;
                }
                sql_query.NEXT();
                if ( sql_query.VALUE(0).toInt() EQ 0 ) {
                    return ADAK_FAIL;
                }
                return ADAK_OK;
            }
	case MSSQL   :
            {
	        // donen deger >= 0 sa locklanmistir aksi takdirde lock olmamistir.
                SQL_QUERY sql_query      ( this );
                sql_query.PREPARE_SELECT ( QString("sp_getapplock @Resource = '%1', @LockMode = 'Exclusive',@LockTimeout = 0,@LockOwner = 'Session'").arg(QString ("LOCK(\""+table_name + "." + where_info+"\",2)")) );
                if(sql_query.SELECT() EQ 0) {
                    return ADAK_FAIL;
                }
                sql_query.NEXT();
                QString sql_query_value = sql_query.VALUE(0).toString();
                if ( sql_query.VALUE(0).toInt() EQ 0 ) {
                    return ADAK_FAIL;
                }
                return ADAK_OK;
            }
        default      :
            return ADAK_OK;
            break;
    }
    return true;
}


/************************************************************/
/*               ADAK_SQL::UNLOCK_ROW                       */
/************************************************************/

void ADAK_SQL::UNLOCK_ROW (const QString id_column_name,const QString table_name, const QString where_info)
{
    switch (p_db_driver) {
        case SQLITE  :
            break;
        case PSQL    :
            EXEC_DDL( QString ("SELECT pg_advisory_unlock("+id_column_name+") FROM " + table_name + " WHERE " + where_info +"") );
            break;
        case MYSQL   :
            EXEC_DDL( QString ("SELECT RELEASE_LOCK(\""+table_name + "." + where_info+"\")") );
            break;
	case MSSQL   :
	    EXEC_DDL( QString("sp_releaseapplock @Resource = '%1', @LockOwner = 'Session'").arg(QString ("LOCK(\""+table_name + "." + where_info+"\",2)")));	  
        default      :
            break;
    }
}
