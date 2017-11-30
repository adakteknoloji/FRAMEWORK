#ifndef DESIGNER_DB_H
#define DESIGNER_DB_H

ADAK_SQL_STRUCT designer_db = {
    0,
    7,
    {
        {
            "blg_belge_degiskenleri",
            10,
            {
                {"degisken_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"belge_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"order_num", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"grubu", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"adi", ADAK_SQL_TEXT, 30, false, 1, false, false },
                {"kodu", ADAK_SQL_TEXT, 15, false, 1, false, false },
                {"font_size", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"align", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"monospace_mi", ADAK_SQL_INTEGER, 1, false, 1, false, false },
                {"text_boyutu", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"degisken_id"} }
            }
        },
        {
            "blg_belgeler",
            2,
            {
                {"belge_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"belge_adi", ADAK_SQL_TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"belge_id"} }
            }
        },
        {
            "sql_alanlar",
            12,
            {
                {"alan_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"tablo_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"alan_order_num", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"alan_adi", ADAK_SQL_TEXT, 32, false, 1, false, false },
                {"veritabanindaki_adi", ADAK_SQL_TEXT, 32, false, 1, false, false },
                {"alan_tipi", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"uzunluk", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"auto_increment_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"not_null_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"dizi_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"dizi_boyutu", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"aciklama", ADAK_SQL_TEXT, 128, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"alan_id"} }
            }
        },
        {
            "sql_db_info",
            2,
            {
                {"db_info_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"program_id", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"db_info_id"} }
            }
        },
        {
            "sql_kullanici_yetkileri",
            13,
            {
                {"yetki_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"yetki_order_num", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kullanici_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"yetkilendirme_yapabilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"bilgisayar_frm_girebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kullanici_frm_girebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"firma_frm_girebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kayit_ekleyebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kayit_guncelleyebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kayit_silebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"ynt_007_gorebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"vrtabani_guncelleyebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"adak_std_guncelleyebilir_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"yetki_id"} }
            }
        },
        {
            "sql_kullanici_yetkileri_fisi",
            1,
            {
                {"kullanici_yetkileri_fisi_id", ADAK_SQL_INTEGER, 0, false, 1, true, true }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"kullanici_yetkileri_fisi_id"} }
            }
        },
        {
            "sql_tablolar",
            5,
            {
                {"tablo_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"veritabani_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"tablo_adi", ADAK_SQL_TEXT, 29, false, 1, false, false },
                {"aciklama", ADAK_SQL_TEXT, 512, false, 1, false, false },
                {"indexler_array", ADAK_SQL_INTEGER, 0, true, 40, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"tablo_id"} }
            }
        }
    }
};
#endif // DESIGNER_DB_H 
