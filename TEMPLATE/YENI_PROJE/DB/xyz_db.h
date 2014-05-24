#ifndef XYZ_DB_H
#define XYZ_DB_H

ADAK_SQL_STRUCT xyz_db = {
    98,
    1,
    {
        {
            "xyz_ayarlar",
            4,
            {
                {"ayarlar_id", INTEGER, 0, false, 1, true, true },
                {"program_version", TEXT, 10, false, 1, false, false },
                {"mali_yil_baslangic_tarihi", INTEGER, 8, false, 1, false, false },
                {"mali_yil_bitis_tarihi", INTEGER, 8, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"ayarlar_id"} }
            }
        }
    }
};
#endif // XYZ_DB_H 
