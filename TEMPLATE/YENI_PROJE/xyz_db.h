#ifndef XYZ_DB_H
#define XYZ_DB_H

ADAK_SQL_STRUCT xyz_db = {
    3,
    1,
    {
        {
            "sabit_degerler",
            2,
            {
                {"degerler_id", INTEGER, 0, false, 1, true, true },
                {"program_versiyon", TEXT, 4, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"degerler_id"} }
            }
        }
    }
};
#endif // XYZ_DB_H 
