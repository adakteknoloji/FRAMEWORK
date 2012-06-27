#ifndef SQLITE_PLUGIN_H
#define SQLITE_PLUGIN_H

class QString;

#include "adak_sql.h"

struct ADAK_SQL_STRUCT;

class SQLITE_PLUGIN : public ADAK_SQL
{
public:
    SQLITE_PLUGIN                                           ( QList<ADAK_SQL_STRUCT *> adak_sql_struct,QString db_name );
    ~SQLITE_PLUGIN                                          ( );

    DB_DRIVER           GET_SQL_DRIVER_ENUM                 ( );
    QString             GET_SQL_DRIVER_NAME                 ( );
    QString             GET_SQL_DRIVER_DEFAULT_DB_NAME      ( );

    int                 CONNECT_TO_DATABASE                 ( bool show_error_screen );
    bool                SQL_DROP_TABLES                     ( QList<ADAK_SQL_STRUCT *> adak_sql_struct );

    QString             OPEN_CREATE_TABLE_SQL               ( QString const &table_name );
    QString             CLOSE_CREATE_TABLE_SQL              ( TABLE_STRUCT current_table );
    QString             CREATE_COLUMN_SQL                   ( QString const& column_name,
                                                                COLUMN_TYPE    column_type,
                                                                int            length,
                                                                bool           is_auto_increment,
                                                                bool           is_not_null,
                                                                bool           is_array,
                                                                INDEX_TYPE     index_type        );

    bool                IS_DB_HAS_MULTIPLE_INSERT           ();
    QString             CREATE_MULTIPLE_INSERT_HEADER       (const QString &table_name,const QString &id_column_name, const QString &insert_column_names);
    QString             CREATE_MULTIPLE_INSERT_BODY         (const QString &table_name,const QString &id_column_name, const QString &insert_column_names,const QString &insert_column_values);
    QString             CREATE_MULTIPLE_INSERT_FOOTER       (const QString &table_name,const QString &id_column_name, const QString &insert_column_names);

    QString             DROP_TABLE_SQL                      ( const QString& table_name );
    QString             GET_TABLE_NAMES_SQL                 ( );
    QString             GET_COLUMN_NAMES_SQL                ( const QString& table_name, const QString& database_name );
    QString             ADD_ONE_COLUMN_SQL                  ( int column_number, TABLE_STRUCT current_table );
    QString             GET_COLUMN_TYPE_NAME                ( COLUMN_TYPE p_column_type, int is_array = 0 );

};

#endif // SQLITE_PLUGIN_H







