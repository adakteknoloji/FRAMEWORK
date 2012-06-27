#ifndef SQL_DLL_H
#define SQL_DLL_H

#include "adak_sql_query.h"
#include "adak_sql.h"

class SQL_DDL : public SQL_QUERY
{
public:
                    SQL_DDL                     (ADAK_SQL * db);

    bool            EXEC_SQL                    ( const QString& query );


    void            START_MULTIPLE_INSERT       (const QString& table_name,const QString &id_column_name,const QString& insert_column_names);
    void            ADD_VALUES_TO_QUEUE         (const QString& column_values                                );
    void            RESET_MULTIPLE_INSERT       ();
    void            COMMIT_MULTIPLE_INSERT      ();

private:
    int             m_multiple_insert_count;
    QString         m_multiple_insert_query;
    QString         m_multiple_insert_table_name;
    QString         m_multiple_insert_id_column_name;
    QString         m_multiple_insert_column_names;

};

#endif // SQL_DLL_H
