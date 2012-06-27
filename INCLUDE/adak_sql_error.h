#ifndef ADAK_SQL_ERROR_H
#define ADAK_SQL_ERROR_H

#include "adak_defines.h"

class QSqlDatabase;
class QSqlQuery;

    //  ADAK_SQL ERROR TYPE DEFINITIONS

enum SQL_ERROR_TYPE {
    CONNECTION_ERROR    =      0,
    CREATION_ERROR      =      1,
    DROP_ERROR          =      2,
    TRANSACTION_ERROR   =      3,
    SQL_QUERY_ERROR     =      4,
    EXEC_DDL_ERROR      =      5,
    DRIVER_TYPE_ERROR   =      6
};

//ADAK_SQL ERROR IN FUNCTION DEFINITIONS

enum SQL_ERROR_IN_FUNCTION {
    ERROR_IN_CONSTRUCTOR              = 0,
    ERROR_IN_START_TRANSACTION        = 1,
    ERROR_IN_COMMIT_TRANSACTION       = 2,
    ERROR_IN_CANCEL_TRANSACTION       = 3,
    ERROR_IN_PREPARE_SELECT           = 4,
    ERROR_IN_PREPARE_INSERT           = 5,
    ERROR_IN_PREPARE_UPDATE           = 6,
    ERROR_IN_PREPARE_DELETE           = 7,
    ERROR_IN_INSERT                   = 8,
    ERROR_IN_UPDATE                   = 9,
    ERROR_IN_DELETE                   = 10,
    ERROR_IN_SELECT                   = 11,
    ERROR_IN_CREATE_DATABASE          = 12,
    ERROR_IN_CREATE_TABLE             = 13,
    ERROR_IN_DROP_DATABASE            = 14,
    ERROR_IN_DROP_TABLES              = 15,
    ERROR_IN_USER_EXISTS              = 16,
    ERROR_IN_CREATE_USER              = 17,
    ERROR_IN_EXEC_DDL                 = 18,
    ERROR_IN_SET_TABLE_ID             = 19,
    ERROR_IN_GET_COLUMN_TYPE          = 20,
    ERROR_IN_FIND_TABLE_ID            = 21,
    ERROR_IN_VALUE                    = 22,
    ERROR_IN_SET_VALUE_MISSING        = 23,
    ERROR_IN_SET_VALUE_EXTRA          = 24,
    ERROR_IN_NEXT                     = 25
};

void SET_SQL_ERROR_FUNC( void ( * error_func_address ) ( const QString& hostname    , const QString& db_name  ,
                                                         const QString& driver_name , const QString& os ,
                                                         int   error_number      , const QString& error_type,
                                                         const QString& error_text  , const QString& aciklama , const QString& hatali_query ) );

void  ADAK_SQL_ERROR          (QSqlDatabase database,          int         error_type,
                                int          error_in_function, QSqlQuery * query,
                                QString extra_info_text="");

void ADAK_SQL_CONSOLE_ERROR   ( const QString& hostname    ,    const QString& db_name,
                                 const QString& driver_name ,    const QString& os ,
                                 int   error_number,        const QString& error_type,
                                 const QString& error_text ,     const QString& aciklama,   const QString& hatali_query );


void  ADAK_SQL_CONSOLE_ERROR  ( const QString& hostname    ,    const QString& db_name,
                                 const QString& driver_name ,    const QString& os ,
                                 int   error_number,        const QString& error_type,
                                 const QString& error_text ,     const QString& aciklama,   const QString& hatali_query );


#endif // ADAK_SQL_ERROR_H
