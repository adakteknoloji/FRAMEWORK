#include <QSettings>
#include <QCryptographicHash>
#include "login_utils.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "login_enum.h"
#include "login.h"

static QList<ADAK_SQL_STRUCT * > G_PROGRAM_DB_STRUCTS;
static QList<ADAK_SQL_STRUCT * > G_YONETIM_DB_STRUCTS;

static int g_kullanicinin_calisabilecegi_veritabani_sayisi;

extern ADAK_SQL * G_YONETIM_DB;

/***********************************************************
                     GET_PROGRAM_DB_STRUCTS
************************************************************/

QList<ADAK_SQL_STRUCT * > GET_PROGRAM_DB_STRUCTS()
{
    return G_PROGRAM_DB_STRUCTS;
}

/***********************************************************
                     SET_PROGRAM_DB_STRUCTS
************************************************************/

void SET_PROGRAM_DB_STRUCTS( ADAK_SQL_STRUCT * P_SQL_STRUCT )
{
    G_PROGRAM_DB_STRUCTS << P_SQL_STRUCT;
}

/***********************************************************
                     GET_YONETIM_DB_STRUCTS
************************************************************/

QList<ADAK_SQL_STRUCT * > GET_YONETIM_DB_STRUCTS()
{
    return G_YONETIM_DB_STRUCTS;
}

/***********************************************************
                     SET_YONETIM_DB_STRUCTS
************************************************************/

void SET_YONETIM_DB_STRUCTS( ADAK_SQL_STRUCT * P_SQL_STRUCT)
{
    G_YONETIM_DB_STRUCTS << P_SQL_STRUCT;
}

/**************************************************************************************
                   CONNECT_TO_SQL_SERVER
***************************************************************************************/

bool CONNECT_TO_SQL_DATABASE(QString sunucu, int port, QString sql_motoru, QString veritabani_on_ek,
                             QString username, QString password, QString db_name, QString *return_error_text )
{

    if (sql_motoru.isEmpty() EQ true) {
        return false;
    }

    SET_SQL_MOTORU (sql_motoru);

    if (GET_SQL_DB_DRIVER_ENUM(sql_motoru) NE SQLITE) {
        if ( port <= 0 ) {
            port = GET_DEFAULT_SQL_PORT_NUMBER(GET_SQL_DB_DRIVER_ENUM(sql_motoru));
        }
        if (sunucu.isEmpty() EQ true) {
            sunucu = "localhost";
        }
    }

    SET_SQL_PORT    ( port   );
    SET_SQL_HOSTNAME( sunucu );
    SET_SQL_PASSWORD( QVariant( password ).toString() );
    SET_SQL_USERNAME( QVariant( username ).toString() );

    SET_SQL_DBNAME_ONEK (QVariant(veritabani_on_ek).toString());

    if( db_name.isEmpty() EQ true ) {
        db_name = "adak_yonetim";
    }
    else {
        db_name.prepend( "adak_");
    }

    ADAK_SQL * db_conn = CREATE_ADAK_SQL_CONNECTION( QList<ADAK_SQL_STRUCT *>(), QString(SQL_DBNAME_ONEK()+ db_name ), GET_SQL_DB_DRIVER_ENUM(SQL_MOTORU()) );

    if( db_conn->CONNECT_TO_DATABASE( false ) EQ ADAK_FAIL ) {
        if ( return_error_text NE NULL ) {
            *return_error_text = db_conn->GET_CONNECTION_LAST_ERROR();
        }
        return false;
    }

    return true;
}

/***********************************************************
                     WRITE_SETTINGS
************************************************************/

void WRITE_SETTINGS( USER_LOGIN_INFO_STRUCT * P_USER_LOGIN_INFO )
{
    if ( P_USER_LOGIN_INFO->kullanici_id EQ 1 ) {
        return;
    }

    QSettings settings ( "ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID())) ;

    QString sql_profili = P_USER_LOGIN_INFO->secili_sql_profili;

    //! OTOMATIK SIFRE DEVREDE ISE SQL KULLANICI SIFRESINI SISTEMDEN
    //! BASKA KIMSE BILEMEZ, BUNDANDOLAYI DISARI KAYDETMIYORUZ.

    if ( P_USER_LOGIN_INFO->is_auto_sifre EQ true ) {
        P_USER_LOGIN_INFO->sql_password.clear();
    }

    settings.setValue ( QString("%1_sql_motoru"      ).arg(sql_profili), P_USER_LOGIN_INFO->sql_motoru   );
    settings.setValue ( QString("%1_sql_sunucu"      ).arg(sql_profili), P_USER_LOGIN_INFO->sql_sunucu   );
    settings.setValue ( QString("%1_sql_port"        ).arg(sql_profili), P_USER_LOGIN_INFO->sql_port     );
    settings.setValue ( QString("%1_sql_db_on_ek"    ).arg(sql_profili), P_USER_LOGIN_INFO->sql_db_on_ek );
    settings.setValue ( QString("%1_sql_username"    ).arg(sql_profili), P_USER_LOGIN_INFO->sql_username );
    settings.setValue ( QString("%1_sql_password"    ).arg(sql_profili), P_USER_LOGIN_INFO->sql_password );
    settings.setValue ( QString("%1_sql_sid"         ).arg(sql_profili), SQL_GET_DB_SID()                );
    settings.setValue ( QString("%1_veritabani_kodu" ).arg(sql_profili), P_USER_LOGIN_INFO->veritabani_kodu );
    settings.setValue ( QString("%1_kullanici_kodu"  ).arg(sql_profili), P_USER_LOGIN_INFO->kullanici_kodu );
    settings.setValue ( QString("%1_is_auto_sifre"   ).arg(sql_profili), P_USER_LOGIN_INFO->is_auto_sifre );
    settings.setValue ( QString("is_auto_login"      ), P_USER_LOGIN_INFO->is_auto_login  );

    if ( P_USER_LOGIN_INFO->is_auto_login EQ true ) {
        QByteArray hash = QCryptographicHash::hash ( QVariant ( P_USER_LOGIN_INFO->kullanici_sifre ).toByteArray(), QCryptographicHash::Md5);
        settings.setValue( "kullanici_parolasi", QString( hash.toHex() ) );
    }

    settings.setValue ( "secili_kullanici_sunucusu" , sql_profili );

    QString profil_str = settings.value("kayitli_sql_sunuculari" , "" ).toString();
    QStringList sql_profilleri = profil_str.split("-");

    sql_profilleri.removeDuplicates();

    for ( int i = 0 ; i < sql_profilleri.size() ; i++ ) {
        if ( sql_profilleri.at( i ).isEmpty() EQ true ) {
            sql_profilleri.removeAt( i );
        }
    }

    if( sql_profilleri.contains( sql_profili ) EQ false ) {
        sql_profilleri.append( sql_profili );
    }

    profil_str = sql_profilleri.join("-");

    settings.setValue("kayitli_sql_sunuculari", profil_str );
}

/***********************************************************
                     READ_SETTINGS
************************************************************/

void READ_SETTINGS( USER_LOGIN_INFO_STRUCT * P_USER_LOGIN_INFO, QString p_profil_ismi  )
{
    CLEAR_SETTINGS( P_USER_LOGIN_INFO );

    QSettings settings ( "ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME( ADAK_DISPLAY_ID() ) );

    P_USER_LOGIN_INFO->secili_sql_profili = p_profil_ismi;
    if( p_profil_ismi.isEmpty() EQ true  ) {
        P_USER_LOGIN_INFO->secili_sql_profili = settings.value("secili_kullanici_sunucusu", "" ).toString();
    }

    if ( P_USER_LOGIN_INFO->secili_sql_profili.isEmpty() EQ true ) {
        QStringList sql_profilleri = settings.value("kayitli_sql_sunuculari", "" ).toString().split("-");
        if ( sql_profilleri.size() NE 0 ) {
            P_USER_LOGIN_INFO->secili_sql_profili = sql_profilleri.at(0);
        }
    }

    QString sql_profili = P_USER_LOGIN_INFO->secili_sql_profili;

    P_USER_LOGIN_INFO->sql_motoru      = settings.value ( QString("%1_sql_motoru"     ).arg(sql_profili), "" ).toString();
    P_USER_LOGIN_INFO->sql_sunucu      = settings.value ( QString("%1_sql_sunucu"     ).arg(sql_profili), "" ).toString();
    P_USER_LOGIN_INFO->sql_port        = settings.value ( QString("%1_sql_port"       ).arg(sql_profili), -1 ).toInt();
    P_USER_LOGIN_INFO->sql_db_on_ek    = settings.value ( QString("%1_sql_db_on_ek"   ).arg(sql_profili), P_USER_LOGIN_INFO->sql_db_on_ek ).toString();
    P_USER_LOGIN_INFO->sql_username    = settings.value ( QString("%1_sql_username"   ).arg(sql_profili), P_USER_LOGIN_INFO->sql_username ).toString();
    P_USER_LOGIN_INFO->sql_password    = settings.value ( QString("%1_sql_password"   ).arg(sql_profili), "" ).toString();
    P_USER_LOGIN_INFO->veritabani_kodu = settings.value ( QString("%1_veritabani_kodu").arg(sql_profili), "" ).toString();
    P_USER_LOGIN_INFO->kullanici_kodu  = settings.value ( QString("%1_kullanici_kodu" ).arg(sql_profili), "" ).toString();
    P_USER_LOGIN_INFO->is_auto_sifre   = settings.value ( QString("%1_is_auto_sifre"  ).arg(sql_profili), "" ).toBool();

    SQL_SET_DB_SID ( settings.value ( QString("%1_sql_sid").arg(sql_profili) , "" ).toString() );

    P_USER_LOGIN_INFO->is_auto_login = settings.value ( "is_auto_login", false ).toBool();

    //! DEFAUT SIFREYI DUZGUN ALMAM ICIN SETLEMEK ZORUNDAYIM
    SET_SQL_USERNAME( P_USER_LOGIN_INFO->sql_username );
    SET_SQL_HOSTNAME( P_USER_LOGIN_INFO->sql_sunucu   );

    //! EGER OTOMATIK SIFRE AKTIF ISE SISTEMDEN OTOMATIK ALACAK
    if ( P_USER_LOGIN_INFO->is_auto_sifre EQ true ) {
        P_USER_LOGIN_INFO->sql_password = SQL_DEFAULT_PASSWORD();
    }
}

/***********************************************************
                     CLEAR_SETTINGS
************************************************************/

void CLEAR_SETTINGS(USER_LOGIN_INFO_STRUCT *P_USER_LOGIN_INFO)
{
    //SQL BILGILERI
    P_USER_LOGIN_INFO->sql_port                   = -1;
    P_USER_LOGIN_INFO->sql_user_created           = false;
    P_USER_LOGIN_INFO->sql_informations_approved  = false;
    P_USER_LOGIN_INFO->sql_tablolari_guncellestir = false;
    P_USER_LOGIN_INFO->sql_motoru                 = "YOK";
    P_USER_LOGIN_INFO->sql_sunucu                 = "localhost";
    P_USER_LOGIN_INFO->sql_db_on_ek               = "";
    P_USER_LOGIN_INFO->sql_username               = "";
    P_USER_LOGIN_INFO->sql_password               = "";
    P_USER_LOGIN_INFO->secili_sql_profili.clear();

    // YONETIM BILGILERI
    P_USER_LOGIN_INFO->is_first_login             = false;
    P_USER_LOGIN_INFO->informations_approved      = false;
    P_USER_LOGIN_INFO->is_yonetici                = false;
    P_USER_LOGIN_INFO->is_yonetim_db_connectted   = false;
    P_USER_LOGIN_INFO->is_auto_login              = false;
    P_USER_LOGIN_INFO->is_auto_sifre              = false;
    P_USER_LOGIN_INFO->bilgisayar_id              = -1;
    P_USER_LOGIN_INFO->bilgisayar_kodu            = "";
    P_USER_LOGIN_INFO->bilgisayar_adi             = "";
    P_USER_LOGIN_INFO->mac_adresi                 = "";
    P_USER_LOGIN_INFO->dongle_ip                  = "";
    P_USER_LOGIN_INFO->kullanici_id               = -1;
    P_USER_LOGIN_INFO->kullanici_kodu             = "";
    P_USER_LOGIN_INFO->kullanici_adi              = "";
    P_USER_LOGIN_INFO->kullanici_sifre           = "";
    P_USER_LOGIN_INFO->veritabani_id              = -1;
    P_USER_LOGIN_INFO->veritabani_kodu            = "";
    P_USER_LOGIN_INFO->veritabani_tanimi          = "";

}

/**************************************************************************************
                   ADAK_CLEAR_SETTINGS
***************************************************************************************/

void ADAK_CLEAR_SETTINGS()
{
    QSettings ynt_settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(YONETIM_libADAK));
    ynt_settings.clear();
    QSettings prg_settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));
    prg_settings.clear();
}

/**************************************************************************************
                   SET_KULLANICI_ID
***************************************************************************************/

static int g_kullanici_id       = -1;
void SET_KULLANICI_ID ( int kullanici_id )
{
    g_kullanici_id = kullanici_id;
}

/**************************************************************************************
                   KULLANICI_ID
***************************************************************************************/

int KULLANICI_ID()
{
    return g_kullanici_id;
}

/**************************************************************************************
                   SET_KULLANICI_KODU
***************************************************************************************/

static QString g_kullanici_kodu="";
void SET_KULLANICI_KODU ( const QString kullanici_kodu )
{
    g_kullanici_kodu = kullanici_kodu;
}

/**************************************************************************************
                   KULLANICI_KODU
***************************************************************************************/

QString KULLANICI_KODU()
{
    return g_kullanici_kodu;
}


/**************************************************************************************
                   SET_KULLANICI_ADI
***************************************************************************************/

static QString  g_kullanici_adi="";
void SET_KULLANICI_ADI ( const QString kullanici_adi )
{
    g_kullanici_adi = kullanici_adi;
}

/**************************************************************************************
                   KULLANICI_ADI
***************************************************************************************/

QString KULLANICI_ADI()
{
    return g_kullanici_adi;
}

/**************************************************************************************
                   SET_BILGISAYAR_ID
***************************************************************************************/

static int  g_bilgisayar_id = -1;
void SET_BILGISAYAR_ID ( int bilgisayar_id )
{
    g_bilgisayar_id = bilgisayar_id;
}

/**************************************************************************************
                   BILGISAYAR_ID
***************************************************************************************/

int BILGISAYAR_ID()
{
    return g_bilgisayar_id;
}

/**************************************************************************************
                   SET_BILGISAYAR_KODU
***************************************************************************************/

static QString g_bilgisayar_kodu="";
void SET_BILGISAYAR_KODU ( const QString bilgisayar_kodu )
{
    g_bilgisayar_kodu = bilgisayar_kodu;
}

/**************************************************************************************
                   BILGISAYAR_KODU
***************************************************************************************/

QString BILGISAYAR_KODU()
{
    return g_bilgisayar_kodu;
}


/**************************************************************************************
                   SET_BILGISAYAR_ADI
***************************************************************************************/

static QString           g_bilgisayar_adi="";
void SET_BILGISAYAR_ADI ( const QString bilgisayar_adi )
{
    g_bilgisayar_adi = bilgisayar_adi;
}

/**************************************************************************************
                   BILGISAYAR_ADI
***************************************************************************************/

QString BILGISAYAR_ADI()
{
    return g_bilgisayar_adi;
}

/**************************************************************************************
                   SET_VERITABANI_ID
***************************************************************************************/

static int             g_veritabani_id           = -1;
void SET_VERITABANI_ID ( int veritabani_id )
{
    g_veritabani_id = veritabani_id;
}

/**************************************************************************************
                   VERITABANI_ID
***************************************************************************************/

int VERITABANI_ID()
{
    return g_veritabani_id;
}

/**************************************************************************************
                   SET_VERITABANI_ISMI
***************************************************************************************/

static QString         g_veritabani_ismi        = "";
void SET_VERITABANI_ISMI ( const QString veritabani_ismi )
{
    g_veritabani_ismi = veritabani_ismi;
}

/**************************************************************************************
                   VERITABANI_ISMI
***************************************************************************************/

QString VERITABANI_ISMI()
{
    return g_veritabani_ismi;
}


/**************************************************************************************
                   SET_VERITABANI_TANIMI
***************************************************************************************/

static QString         g_veritabani_tanimi          = "";
void SET_VERITABANI_TANIMI ( const QString veritabani_tanimi )
{
    g_veritabani_tanimi = veritabani_tanimi;
}

/**************************************************************************************
                   VERITABANI_TANIMI
***************************************************************************************/

QString VERITABANI_TANIMI ()
{
    return g_veritabani_tanimi;
}


/**************************************************************************************
                   VERITABANI_TANIMI
***************************************************************************************/

QString VERITABANI_TANIMI (QString p_veritabani_ismi)
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_veritabanlari" , "veritabani_tanimi",
                         "veritabani_ismi = :veritabani_ismi");
    query.SET_VALUE(":veritabani_ismi" , p_veritabani_ismi );

    if ( query.SELECT() EQ 0 ) {
        return "";
    }
    query.NEXT();

    return query.VALUE(0).toString();
}

/**************************************************************************************
                   SET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI
***************************************************************************************/

void SET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI ( int p_veritabani_sayisi )
{
    g_kullanicinin_calisabilecegi_veritabani_sayisi = p_veritabani_sayisi;
}

/**************************************************************************************
                   GET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI
***************************************************************************************/

int GET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI ()
{
    return g_kullanicinin_calisabilecegi_veritabani_sayisi;
}

/**************************************************************************************
                   BILGISAYARDA_KAYITLI_KULLANICI_SAYISINI_BUL
***************************************************************************************/

int BILGISAYARDA_KAYITLI_KULLANICI_SAYISINI_BUL ()
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT  (  "ynt_bilgisayar_kullanicilari","kullanici_id","kullanici_id > 1 AND bilgisayar_id = :bilgisayar_id");
    sql_query.SET_VALUE       (  ":bilgisayar_id", BILGISAYAR_ID() );
    return sql_query.SELECT();
}

/**************************************************************************************
                   KULLANICININ_CALISABILECEGI_VERITABANI_SAYISINI_BUL
***************************************************************************************/

int KULLANICININ_CALISABILECEGI_VERITABANI_SAYISINI_BUL ()
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabani_kullanicilari","id","kullanici_id = :kullanici_id");
    sql_query.SET_VALUE      ( ":kullanici_id", KULLANICI_ID() );

    return sql_query.SELECT();
}

/**************************************************************************************
                   YONETIM_007_GIRIS_KAYDI
***************************************************************************************/

void YONETIM_007_GIRIS_KAYDI ()
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_ismi, veritabani_tanimi","veritabani_id = :veritabani_id");

    sql_query.SET_VALUE      (":veritabani_id",VERITABANI_ID() );

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }
    sql_query.NEXT();

    QString aciklama = "Giriş yapılan veritabanı: " + sql_query.VALUE(0).toString() + " # Giriş yapılan veritabanı tanımı: "+ sql_query.VALUE(1).toString();

    YONETIM_007_KAYIT ( LOG_PROGRAM, LOG_PROGRAMA_GIRIS, aciklama );
}

/**************************************************************************************
                   GET_LOG_TURU
***************************************************************************************/

QString GET_LOG_TURU ( int log_turu )
{
    if ( log_turu EQ LOG_BILGISAYARLAR ) {
        return "BILGISAYARLAR";
    }
    else if ( log_turu EQ LOG_KULLANICILAR ) {
        return "KULLANICILAR";
    }
    else if ( log_turu EQ LOG_VERITABANLARI ) {
        return "VERITABANLARI";
    }
    else if (log_turu EQ LOG_PROGRAM ) {
        return "PROGRAM";
    }

    return NULL;
}

/**************************************************************************************
                   GET_LOG_TURU
***************************************************************************************/

int GET_LOG_TURU ( QString log_turu )
{
    if ( log_turu EQ "BILGISAYARLAR" ) {
        return LOG_BILGISAYARLAR;
    }
    else if ( log_turu EQ "KULLANICILAR" ) {
        return LOG_KULLANICILAR;
    }
    else if ( log_turu EQ "VERITABANLARI" ) {
        return LOG_VERITABANLARI;
    }
    else if (log_turu EQ "PROGRAM" ) {
        return LOG_PROGRAM;
    }
    else {
        return -1;
    }
}

/**************************************************************************************
                   GET_LOG_ISLEMI
***************************************************************************************/

QString GET_LOG_ISLEMI ( int log_islemi )
{
    switch ( log_islemi ) {
        case LOG_KAYIT_EKLEME :
            return "KAYIT EKLEME";
        case LOG_KAYIT_GUNCELLEME :
            return "KAYIT GUNCELLEME";
        case LOG_KAYIT_SILME :
            return "KAYIT_SILME";
        case LOG_PROGRAMA_GIRIS :
            return "PROGRAMA GIRIS";
        case LOG_PROGRAMDAN_CIKIS :
            return "PROGRAMDAN CIKIS";
        case LOG_VERITABANI_YARATMA :
            return "VERITABANI YARATMA";
        case LOG_VERITABANI_SILME :
            return "VERITABANI SILME";
        case LOG_PROGRAM_TABLOSU_YARATMA :
            return "PROGRAM TABLOSU YARATMA";
        case LOG_PROGRAM_TABLOSU_SILME :
            return "PROGRAM TABLOSU SILME";
        default :
            return NULL;
    }
}
