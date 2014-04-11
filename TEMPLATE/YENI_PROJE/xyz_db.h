#ifndef XYZ_DB_H
#define XYZ_DB_H

ADAK_SQL_STRUCT xyz_db = {
    3,
    1,
    {
        {
            "temp",
            2,
            {
                {"temp_id", INTEGER, 0, false, 1, true, true },
                {"temp_str", TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"temp_id"} }
            }
        }
    }
};
#endif // XYZ_DB_H 
