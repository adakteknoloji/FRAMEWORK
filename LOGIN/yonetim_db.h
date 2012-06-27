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
                {"banka_id", INTEGER, 0, false, 1, true, true },
                {"banka_adi", TEXT, 30, false, 1, false, false },
                {"banka_adresi", TEXT, 150, false, 1, false, false },
                {"banka_telefonu", TEXT, 15, false, 1, false, false },
                {"banka_web_adresi", TEXT, 50, false, 1, false, false },
                {"banka_email", TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"banka_id"} }
            }
        },
        {
            "adak_banka_subeleri",
            10,
            {
                {"banka_sube_id", INTEGER, 0, false, 1, true, true },
                {"banka_id", INTEGER, 0, false, 1, false, false },
                {"il_id", INTEGER, 0, false, 1, false, false },
                {"ilce_id", INTEGER, 0, false, 1, false, false },
                {"sube_kodu", TEXT, 10, false, 1, false, false },
                {"sube_adi", TEXT, 40, false, 1, false, false },
                {"sube_telefonu", TEXT, 15, false, 1, false, false },
                {"sube_adresi", TEXT, 150, false, 1, false, false },
                {"sube_email", TEXT, 50, false, 1, false, false },
                {"sube_web_adresi", TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"banka_sube_id"} }
            }
        },
        {
            "adak_cinsiyetler",
            2,
            {
                {"cinsiyet_id", INTEGER, 0, false, 1, true, true },
                {"cinsiyet_adi", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"cinsiyet_id"} }
            }
        },
        {
            "adak_dovizler",
            3,
            {
                {"doviz_id", INTEGER, 0, false, 1, true, true },
                {"doviz_kodu", TEXT, 5, false, 1, false, false },
                {"doviz_adi", TEXT, 40, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"doviz_id"} }
            }
        },
        {
            "adak_email_log",
            8,
            {
                {"log_id", INTEGER, 0, false, 1, true, true },
                {"kullanici_id", INTEGER, 0, false, 1, false, false },
                {"gonderilen_email", TEXT, 50, false, 1, false, false },
                {"tarih", TEXT, 10, false, 1, false, false },
                {"record_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false },
                {"modul_id", INTEGER, 0, false, 1, false, false },
                {"saat", TEXT, 8, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"log_id"} }
            }
        },
        {
            "adak_gsm_kodlari",
            4,
            {
                {"gsm_id", INTEGER, 0, false, 1, true, true },
                {"ulke_id", INTEGER, 0, false, 1, false, false },
                {"gsm_kodu", TEXT, 10, false, 1, false, false },
                {"gsm_adi", TEXT, 20, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"gsm_id"} }
            }
        },
        {
            "adak_ilceler",
            3,
            {
                {"ilce_id", INTEGER, 0, false, 1, true, true },
                {"il_id", INTEGER, 0, false, 1, false, false },
                {"ilce_adi", TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"ilce_id"} }
            }
        },
        {
            "adak_iller",
            6,
            {
                {"il_id", INTEGER, 0, false, 1, true, true },
                {"ulke_id", INTEGER, 0, false, 1, false, false },
                {"telefon_kodu1", TEXT, 10, false, 1, false, false },
                {"telefon_kodu2", TEXT, 10, false, 1, false, false },
                {"plaka_kodu", TEXT, 5, false, 1, false, false },
                {"il_adi", TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"il_id"} }
            }
        },
        {
            "adak_kan_gruplari",
            2,
            {
                {"kan_grubu_id", INTEGER, 0, false, 1, true, true },
                {"kan_grubu_adi", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kan_grubu_id"} }
            }
        },
        {
            "adak_medeni_haller",
            2,
            {
                {"medeni_hal_id", INTEGER, 0, false, 1, true, true },
                {"medeni_hal_adi", TEXT, 10, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"medeni_hal_id"} }
            }
        },
        {
            "adak_meslekler",
            2,
            {
                {"meslek_id", INTEGER, 0, false, 1, true, true },
                {"meslek_adi", TEXT, 128, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"meslek_id"} }
            }
        },
        {
            "adak_ogrenim_durumlari",
            2,
            {
                {"ogrenim_durumu_id", INTEGER, 0, false, 1, true, true },
                {"ogrenim_durumu_adi", TEXT, 30, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"ogrenim_durumu_id"} }
            }
        },
        {
            "adak_sektorler",
            2,
            {
                {"sektor_id", INTEGER, 0, false, 1, true, true },
                {"sektor_adi", TEXT, 40, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"sektor_id"} }
            }
        },
        {
            "adak_ulkeler",
            5,
            {
                {"ulke_id", INTEGER, 0, false, 1, true, true },
                {"ulke_telefon_kodu", TEXT, 10, false, 1, false, false },
                {"ulke_plaka_kodu", TEXT, 10, false, 1, false, false },
                {"ulke_adi", TEXT, 30, false, 1, false, false },
                {"doviz_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"ulke_id"} }
            }
        },
        {
            "adak_unvanlar",
            2,
            {
                {"unvan_id", INTEGER, 0, false, 1, true, true },
                {"unvan_adi", TEXT, 128, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"unvan_id"} }
            }
        },
        {
            "adak_vergi_daireleri",
            3,
            {
                {"vergi_dairesi_id", INTEGER, 0, false, 1, true, true },
                {"il_id", INTEGER, 0, false, 1, false, false },
                {"vergi_dairesi_adi", TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"vergi_dairesi_id"} }
            }
        },
        {
            "ynt_007",
            8,
            {
                {"id", INTEGER, 0, false, 1, true, true },
                {"islem_tarihi", TEXT, 10, false, 1, false, false },
                {"islem_saati", TEXT, 5, false, 1, false, false },
                {"kullanici_id", INTEGER, 0, false, 1, false, false },
                {"bilgisayar_id", INTEGER, 0, false, 1, false, false },
                {"log_turu", INTEGER, 0, false, 1, false, false },
                {"log_islemi", INTEGER, 0, false, 1, false, false },
                {"log_detaylari", TEXT, 512, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_belge_degiskenleri",
            26,
            {
                {"belge_degisken_id", INTEGER, 0, false, 1, true, true },
                {"tasarim_id", INTEGER, 0, false, 1, false, false },
                {"degisken_id", INTEGER, 0, false, 1, false, false },
                {"font_size", INTEGER, 0, false, 1, false, false },
                {"align", INTEGER, 0, false, 1, false, false },
                {"pos_x", REAL, 0, false, 1, false, false },
                {"pos_y", REAL, 0, false, 1, false, false },
                {"size_vertical", INTEGER, 0, false, 1, false, false },
                {"size_horizontal", INTEGER, 0, false, 1, false, false },
                {"pixmap", BLOB, 0, false, 1, false, false },
                {"text", TEXT, 250, false, 1, false, false },
                {"text_size", INTEGER, 0, false, 1, false, false },
                {"is_visible_variable", INTEGER, 0, false, 1, false, false },
                {"grup", TEXT, 50, false, 1, false, false },
                {"grup_enum", INTEGER, 0, false, 1, false, false },
                {"monospace_mi", INTEGER, 1, false, 1, false, false },
                {"yazi_boyutu", INTEGER, 0, false, 1, false, false },
                {"font_family", TEXT, 30, false, 1, false, false },
                {"is_bold", INTEGER, 0, false, 1, false, false },
                {"is_under_line", INTEGER, 0, false, 1, false, false },
                {"is_italic", INTEGER, 0, false, 1, false, false },
                {"is_strikeout", INTEGER, 0, false, 1, false, false },
                {"is_nlq", INTEGER, 0, false, 1, false, false },
                {"dot_matrix_mode", INTEGER, 0, false, 1, false, false },
                {"satir", INTEGER, 0, false, 1, false, false },
                {"soldan_bosluk", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"belge_degisken_id"} }
            }
        },
        {
            "ynt_belge_tasarimlari",
            17,
            {
                {"tasarim_id", INTEGER, 0, false, 1, true, true },
                {"belge_id", INTEGER, 0, false, 1, false, false },
                {"tasarim_adi", TEXT, 30, false, 1, false, false },
                {"html_str", TEXT, 1024, false, 1, false, false },
                {"belge_width", REAL, 0, false, 1, false, false },
                {"belge_height", REAL, 0, false, 1, false, false },
                {"row_count", INTEGER, 0, false, 1, false, false },
                {"row_space_mm", REAL, 0, false, 1, false, false },
                {"row_space_satir", INTEGER, 0, false, 1, false, false },
                {"belge_satir_sayisi", INTEGER, 0, false, 1, false, false },
                {"belge_toplam_karakter_sayisi", INTEGER, 0, false, 1, false, false },
                {"kopya_sayisi", INTEGER, 0, false, 1, false, false },
                {"kopya_konumu", INTEGER, 0, false, 1, false, false },
                {"printer_type", INTEGER, 0, false, 1, false, false },
                {"line_headers_visible", INTEGER, 0, false, 1, false, false },
                {"preview", BLOB, 0, false, 1, false, false },
                {"readonly", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"tasarim_id"} }
            }
        },
        {
            "ynt_bilgisayar_kullanicilari",
            3,
            {
                {"id", INTEGER, 0, false, 1, true, true },
                {"bilgisayar_id", INTEGER, 0, false, 1, false, false },
                {"kullanici_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_bilgisayarlar",
            9,
            {
                {"bilgisayar_id", INTEGER, 0, false, 1, true, true },
                {"silinmis_mi", INTEGER, 0, false, 1, false, false },
                {"bilgisayar_kodu", TEXT, 10, false, 1, false, false },
                {"bilgisayar_adi", TEXT, 50, false, 1, false, false },
                {"dongle_ip", TEXT, 20, false, 1, false, false },
                {"mac_adresi_kablolu", TEXT, 17, false, 1, false, false },
                {"mac_adresi_kablosuz", TEXT, 17, false, 1, false, false },
                {"mac_adresi_3g", TEXT, 17, false, 1, false, false },
                {"mac_adresi", TEXT, 17, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"bilgisayar_id"} }
            }
        },
        {
            "ynt_kullanicilar",
            8,
            {
                {"kullanici_id", INTEGER, 0, false, 1, true, true },
                {"silinmis_mi", INTEGER, 0, false, 1, false, false },
                {"kullanici_kodu", TEXT, 10, false, 1, false, false },
                {"parola", TEXT, 50, false, 1, false, false },
                {"kullanici_adi", TEXT, 50, false, 1, false, false },
                {"email_adresi", TEXT, 50, false, 1, false, false },
                {"email_kullanici_adi", TEXT, 50, false, 1, false, false },
                {"aciklama", TEXT, 100, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"kullanici_id"} }
            }
        },
        {
            "ynt_posta_sunucusu",
            4,
            {
                {"posta_sunucu_id", INTEGER, 0, false, 1, true, true },
                {"email_sunucusu", TEXT, 100, false, 1, false, false },
                {"email_port", TEXT, 10, false, 1, false, false },
                {"email_baglanti_guvenligi", TEXT, 20, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"posta_sunucu_id"} }
            }
        },
        {
            "ynt_sms_sunucusu",
            7,
            {
                {"sms_sunucu_id", INTEGER, 0, false, 1, true, true },
                {"sms_sunucusu", TEXT, 100, false, 1, false, false },
                {"sms_port", TEXT, 10, false, 1, false, false },
                {"sms_baglanti_guvenligi", TEXT, 20, false, 1, false, false },
                {"sms_kullanici_adi", TEXT, 50, false, 1, false, false },
                {"sms_parola", TEXT, 50, false, 1, false, false },
                {"sms_app_id", TEXT, 50, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"sms_sunucu_id"} }
            }
        },
        {
            "ynt_veritabani_kullanicilari",
            3,
            {
                {"id", INTEGER, 0, false, 1, true, true },
                {"veritabani_id", INTEGER, 0, false, 1, false, false },
                {"kullanici_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_veritabani_programlari",
            3,
            {
                {"id", INTEGER, 0, false, 1, true, true },
                {"veritabani_id", INTEGER, 0, false, 1, false, false },
                {"program_id", INTEGER, 0, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"id"} }
            }
        },
        {
            "ynt_veritabanlari",
            4,
            {
                {"veritabani_id", INTEGER, 0, false, 1, true, true },
                {"veritabani_ismi", TEXT, 10, false, 1, false, false },
                {"veritabani_tanimi", TEXT, 50, false, 1, false, false },
                {"aciklama", TEXT, 100, false, 1, false, false }
            },
            1,
            {
                { PRIMARY , 1, {"veritabani_id"} }
            }
        }
    }
};
#endif // YONETIM_DB_H 
