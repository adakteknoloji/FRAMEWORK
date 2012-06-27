#ifndef LOGIN_UTILS_H
#define LOGIN_UTILS_H

class QString;
class QWidget;

struct USER_LOGIN_INFO_STRUCT {
    //SQL BILGILERI
    int      sql_port;
    bool     sql_user_created;
    bool     sql_informations_approved;
    bool     sql_tablolari_guncellestir;
    QString  sql_motoru;
    QString  sql_sunucu;
    QString  sql_db_on_ek;
    QString  sql_username;
    QString  sql_password;

    // USER BILGILERI
    int      bilgisayar_id;
    int      kullanici_id;
    int      veritabani_id;
    bool     is_yonetici;
    bool     informations_approved;
    bool     is_yonetim_db_connectted;
    bool     kullanici_veritabaninda_calisabilir;
    bool     is_auto_login;
    bool     is_first_login;
    bool     is_auto_sifre;
    QString  bilgisayar_adi;
    QString  bilgisayar_kodu;
    QString  mac_adresi;
    QString  dongle_ip;
    QString  kullanici_kodu;
    QString  kullanici_adi;
    QString  kullanici_sifre;
    QString  veritabani_kodu;
    QString  veritabani_tanimi;
    QString  secili_sql_profili;

};


bool OPEN_YONETICI_IZNI_BATCH           ( QWidget * parent );
bool OPEN_YENI_SUNUCU_TANIMLAMA_BATCH   ( bool yeni_profil , USER_LOGIN_INFO_STRUCT* user_informations , QWidget * parent );
bool OPEN_LOGIN_BATCH                   ( USER_LOGIN_INFO_STRUCT * user_informations , QWidget * parent);

void WRITE_SETTINGS                     ( USER_LOGIN_INFO_STRUCT * P_USER_LOGIN_INFO );
void READ_SETTINGS                      ( USER_LOGIN_INFO_STRUCT * P_USER_LOGIN_INFO, QString p_profil_ismi = "" );
void CLEAR_SETTINGS                     ( USER_LOGIN_INFO_STRUCT * P_USER_LOGIN_INFO );

void ADAK_CLEAR_SETTINGS               ();
void YONETIM_007_GIRIS_KAYDI            ();

void OPEN_ADAK_SQL_ERROR_DIALOG        ( const QString& hostname,
                                          const QString& db_name,
                                          const QString& driver_name,
                                          const QString& os,
                                          int   error_number,
                                          const QString& error_type,
                                          const QString& error_text,
                                          const QString& aciklama,
                                          const QString& hatali_query );


#endif // LOGIN_UTILS_H
