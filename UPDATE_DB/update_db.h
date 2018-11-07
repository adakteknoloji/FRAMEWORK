#ifndef UPDATE_DB_H
#define UPDATE_DB_H

#include "adak_sql_enums.h"
#include <QString>

int             FIND_FIRST_INDEX    ( QString current_string                    );
int             FIND_LAST_INDEX     ( int first_index,  QString current_string  );
COLUMN_TYPE     GET_COLUMN_TYPE     ( QString column_type );
INDEX_TYPE      GET_INDEX_TYPE      ( QString index_type  );

#endif // UPDATE_DB_H
