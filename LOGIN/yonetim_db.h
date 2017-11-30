#ifndef YONETIM_DB_H
#define YONETIM_DB_H

ADAK_SQL_STRUCT yonetim_db = {
    1,
    27,
    {
        {
            "adak_bankalar",
            6,
            {
                {"banka_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"banka_adi", ADAK_SQL_TEXT, 30, false, 1, false, false },
                {"banka_adresi", ADAK_SQL_TEXT, 150, false, 1, false, false },
                {"banka_telefonu", ADAK_SQL_TEXT, 15, false, 1, false, false },
                {"banka_web_adresi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"banka_email", ADAK_SQL_TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"banka_id"} }
            }
        },
        {
            "adak_banka_subeleri",
            10,
            {
                {"banka_sube_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"banka_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"il_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"ilce_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"sube_kodu", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"sube_adi", ADAK_SQL_TEXT, 40, false, 1, false, false },
                {"sube_telefonu", ADAK_SQL_TEXT, 15, false, 1, false, false },
                {"sube_adresi", ADAK_SQL_TEXT, 150, false, 1, false, false },
                {"sube_email", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"sube_web_adresi", ADAK_SQL_TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"banka_sube_id"} }
            }
        },
        {
            "adak_cinsiyetler",
            2,
            {
                {"cinsiyet_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"cinsiyet_adi", ADAK_SQL_TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"cinsiyet_id"} }
            }
        },
        {
            "adak_dovizler",
            3,
            {
                {"doviz_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"doviz_kodu", ADAK_SQL_TEXT, 5, false, 1, false, false },
                {"doviz_adi", ADAK_SQL_TEXT, 40, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"doviz_id"} }
            }
        },
        {
            "adak_email_log",
            8,
            {
                {"log_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"kullanici_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"gonderilen_email", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"tarih", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"record_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"program_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"modul_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"saat", ADAK_SQL_TEXT, 8, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"log_id"} }
            }
        },
        {
            "adak_gsm_kodlari",
            4,
            {
                {"gsm_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"ulke_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"gsm_kodu", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"gsm_adi", ADAK_SQL_TEXT, 20, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"gsm_id"} }
            }
        },
        {
            "adak_ilceler",
            3,
            {
                {"ilce_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"il_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"ilce_adi", ADAK_SQL_TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"ilce_id"} }
            }
        },
        {
            "adak_iller",
            6,
            {
                {"il_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"ulke_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"telefon_kodu1", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"telefon_kodu2", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"plaka_kodu", ADAK_SQL_TEXT, 5, false, 1, false, false },
                {"il_adi", ADAK_SQL_TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"il_id"} }
            }
        },
        {
            "adak_kan_gruplari",
            2,
            {
                {"kan_grubu_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"kan_grubu_adi", ADAK_SQL_TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"kan_grubu_id"} }
            }
        },
        {
            "adak_medeni_haller",
            2,
            {
                {"medeni_hal_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"medeni_hal_adi", ADAK_SQL_TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"medeni_hal_id"} }
            }
        },
        {
            "adak_meslekler",
            2,
            {
                {"meslek_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"meslek_adi", ADAK_SQL_TEXT, 128, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"meslek_id"} }
            }
        },
        {
            "adak_ogrenim_durumlari",
            2,
            {
                {"ogrenim_durumu_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"ogrenim_durumu_adi", ADAK_SQL_TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"ogrenim_durumu_id"} }
            }
        },
        {
            "adak_sektorler",
            2,
            {
                {"sektor_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"sektor_adi", ADAK_SQL_TEXT, 40, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"sektor_id"} }
            }
        },
        {
            "adak_ulkeler",
            5,
            {
                {"ulke_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"ulke_telefon_kodu", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"ulke_plaka_kodu", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"ulke_adi", ADAK_SQL_TEXT, 30, false, 1, false, false },
                {"doviz_id", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"ulke_id"} }
            }
        },
        {
            "adak_unvanlar",
            2,
            {
                {"unvan_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"unvan_adi", ADAK_SQL_TEXT, 128, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"unvan_id"} }
            }
        },
        {
            "adak_vergi_daireleri",
            3,
            {
                {"vergi_dairesi_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"il_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"vergi_dairesi_adi", ADAK_SQL_TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"vergi_dairesi_id"} }
            }
        },
        {
            "ynt_007",
            8,
            {
                {"id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"islem_tarihi", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"islem_saati", ADAK_SQL_TEXT, 5, false, 1, false, false },
                {"kullanici_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"bilgisayar_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"log_turu", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"log_islemi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"log_detaylari", ADAK_SQL_TEXT, 512, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_belge_degiskenleri",
            26,
            {
                {"belge_degisken_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"tasarim_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"degisken_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"font_size", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"align", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"pos_x", ADAK_SQL_REAL, 0, false, 1, false, false },
                {"pos_y", ADAK_SQL_REAL, 0, false, 1, false, false },
                {"size_vertical", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"size_horizontal", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"pixmap", ADAK_SQL_BLOB, 0, false, 1, false, false },
                {"text", ADAK_SQL_TEXT, 250, false, 1, false, false },
                {"text_size", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"is_visible_variable", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"grup", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"grup_enum", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"monospace_mi", ADAK_SQL_INTEGER, 1, false, 1, false, false },
                {"yazi_boyutu", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"font_family", ADAK_SQL_TEXT, 30, false, 1, false, false },
                {"is_bold", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"is_under_line", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"is_italic", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"is_strikeout", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"is_nlq", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"dot_matrix_mode", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"satir", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"soldan_bosluk", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"belge_degisken_id"} }
            }
        },
        {
            "ynt_belge_tasarimlari",
            17,
            {
                {"tasarim_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"belge_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"tasarim_adi", ADAK_SQL_TEXT, 30, false, 1, false, false },
                {"html_str", ADAK_SQL_TEXT, 1024, false, 1, false, false },
                {"belge_width", ADAK_SQL_REAL, 0, false, 1, false, false },
                {"belge_height", ADAK_SQL_REAL, 0, false, 1, false, false },
                {"row_count", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"row_space_mm", ADAK_SQL_REAL, 0, false, 1, false, false },
                {"row_space_satir", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"belge_satir_sayisi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"belge_toplam_karakter_sayisi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kopya_sayisi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kopya_konumu", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"printer_type", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"line_headers_visible", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"preview", ADAK_SQL_BLOB, 0, false, 1, false, false },
                {"readonly", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"tasarim_id"} }
            }
        },
        {
            "ynt_bilgisayar_kullanicilari",
            3,
            {
                {"id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"bilgisayar_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kullanici_id", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_bilgisayarlar",
            9,
            {
                {"bilgisayar_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"silinmis_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"bilgisayar_kodu", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"bilgisayar_adi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"dongle_ip", ADAK_SQL_TEXT, 20, false, 1, false, false },
                {"mac_adresi_kablolu", ADAK_SQL_TEXT, 17, false, 1, false, false },
                {"mac_adresi_kablosuz", ADAK_SQL_TEXT, 17, false, 1, false, false },
                {"mac_adresi_3g", ADAK_SQL_TEXT, 17, false, 1, false, false },
                {"mac_adresi", ADAK_SQL_TEXT, 17, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"bilgisayar_id"} }
            }
        },
        {
            "ynt_kullanicilar",
            8,
            {
                {"kullanici_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"silinmis_mi", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kullanici_kodu", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"parola", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"kullanici_adi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"email_adresi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"email_kullanici_adi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"aciklama", ADAK_SQL_TEXT, 100, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"kullanici_id"} }
            }
        },
        {
            "ynt_posta_sunucusu",
            4,
            {
                {"posta_sunucu_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"email_sunucusu", ADAK_SQL_TEXT, 100, false, 1, false, false },
                {"email_port", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"email_baglanti_guvenligi", ADAK_SQL_TEXT, 20, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"posta_sunucu_id"} }
            }
        },
        {
            "ynt_sms_sunucusu",
            7,
            {
                {"sms_sunucu_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"sms_sunucusu", ADAK_SQL_TEXT, 100, false, 1, false, false },
                {"sms_port", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"sms_baglanti_guvenligi", ADAK_SQL_TEXT, 20, false, 1, false, false },
                {"sms_kullanici_adi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"sms_parola", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"sms_app_id", ADAK_SQL_TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"sms_sunucu_id"} }
            }
        },
        {
            "ynt_veritabani_kullanicilari",
            3,
            {
                {"id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"veritabani_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"kullanici_id", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_veritabani_programlari",
            3,
            {
                {"id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"veritabani_id", ADAK_SQL_INTEGER, 0, false, 1, false, false },
                {"program_id", ADAK_SQL_INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_veritabanlari",
            4,
            {
                {"veritabani_id", ADAK_SQL_INTEGER, 0, false, 1, true, true },
                {"veritabani_ismi", ADAK_SQL_TEXT, 10, false, 1, false, false },
                {"veritabani_tanimi", ADAK_SQL_TEXT, 50, false, 1, false, false },
                {"aciklama", ADAK_SQL_TEXT, 100, false, 1, false, false }
            },
            1,
            {
                { ADAK_SQL_PRIMARY , 1, {"veritabani_id"} }
            }
        }
    }
};
#endif // YONETIM_DB_H 
