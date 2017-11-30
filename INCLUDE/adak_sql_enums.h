#ifndef ENUM_TYPES_H
#define ENUM_TYPES_H


enum COLUMN_TYPE {
    ADAK_SQL_INTEGER,
    ADAK_SQL_TEXT,
    ADAK_SQL_REAL,
    ADAK_SQL_BLOB
};


enum INDEX_TYPE {
     ADAK_SQL_NONE,
     ADAK_SQL_PRIMARY,
     ADAK_SQL_UNIQUE,
     ADAK_SQL_INDEX
};

enum SQL_QUERY_TYPE {
    INSERT_QUERY,
    UPDATE_QUERY,
    DELETE_QUERY,
    SELECT_QUERY
};


#endif // ENUM_TYPES_H
