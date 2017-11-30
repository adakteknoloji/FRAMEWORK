#include "adak_defines.h"
#include "update_db.h"

/*************************************/
/*          FIND_FIRST_INDEX         */
/*************************************/

int FIND_FIRST_INDEX ( QString current_string )
{
    int first_index = 0;
    while ( current_string[first_index].isSpace() EQ true ) {
        first_index++;
    }
    return first_index;
}

/*************************************/
/*          FIND_LAST_INDEX          */
/*************************************/

int FIND_LAST_INDEX ( int first_index, QString current_string )
{
    int last_index  = first_index;
    while ( current_string[last_index].isSpace() EQ false ) {
        if ( last_index EQ current_string.size() - 1 ) {
            break;
        }
        if ( current_string[last_index + 1].isSpace() EQ true ) {
            break;
        }
        last_index++;
    }
    return last_index;
}

/*************************************/
/*          GET_COLUMN_TYPE          */
/*************************************/

COLUMN_TYPE GET_COLUMN_TYPE ( QString column_type )
{
    if ( column_type EQ "ADAK_SQL_INTEGER") {
        return ADAK_SQL_INTEGER;
    }
    else if ( column_type EQ "ADAK_SQL_TEXT" ) {
        return ADAK_SQL_TEXT;
    }
    else if ( column_type EQ "ADAK_SQL_REAL" ) {
        return ADAK_SQL_REAL;
    }
    else if ( column_type EQ "ADAK_SQL_BLOB" ) {
        return ADAK_SQL_BLOB;
    }
    return ADAK_SQL_INTEGER;
}

/*************************************/
/*          GET_INDEX_TYPE           */
/*************************************/

INDEX_TYPE  GET_INDEX_TYPE  ( QString index_type )
{
    if ( index_type EQ "ADAK_SQL_NONE" ) {
        return ADAK_SQL_NONE;
    }
    else if ( index_type EQ "ADAK_SQL_PRIMARY" ) {
        return ADAK_SQL_PRIMARY;
    }
    else if ( index_type EQ "ADAK_SQL_UNIQUE" ) {
        return ADAK_SQL_UNIQUE;
    }
    else if ( index_type EQ "ADAK_SQL_INDEX" ) {
        return ADAK_SQL_INDEX;
    }
    return ADAK_SQL_NONE;
}
