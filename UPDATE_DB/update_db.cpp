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
    if ( column_type EQ "INTEGER") {
        return INTEGER;
    }
    else if ( column_type EQ "TEXT" ) {
        return TEXT;
    }
    else if ( column_type EQ "REAL" ) {
        return REAL;
    }
    else if ( column_type EQ "BLOB" ) {
        return BLOB;
    }
    return INTEGER;
}

/*************************************/
/*          GET_INDEX_TYPE           */
/*************************************/

INDEX_TYPE  GET_INDEX_TYPE  ( QString index_type )
{
    if ( index_type EQ "NONE" ) {
        return NONE;
    }
    else if ( index_type EQ "PRIMARY" ) {
        return PRIMARY;
    }
    else if ( index_type EQ "UNIQUE" ) {
        return UNIQUE;
    }
    else if ( index_type EQ "INDEX" ) {
        return INDEX;
    }
    return NONE;
}
