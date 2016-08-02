#include <QDir>
#include <QDate>
#include <QWebSettings>
#include <QFont>
#include <QSettings>
#include <QByteArray>
#include <QCryptographicHash>
#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>
#include <QString>

#include "iostream"
#include "adak_std_utils.h"
#include "adak_sql.h"
#include "adak_sql_error.h"
#include "kernel_utils.h"
#include "adak_utils.h"
#include "adak_sql_struct.h"
#include "yonetim_db.h"
#include "login.h"
#include "yardim_hatirlatma_dialog.h"
#include "sql_sunucu_secimi.h"
#include "sql_plugins.h"
#include "program_start.h"
#include "login_utils.h"
#include "first_login.h"
#include "login_enum.h"

using std::cerr;
using std::endl;

typedef void (* P_KULLANICI_TAM_YETKI_FONKSIYONU)();

//Bunlar burda doldurup ise yaramadigi zaman direk temizlenmeli.
static QList<P_KULLANICI_TAM_YETKI_FONKSIYONU> G_TAM_YETKILENDIRME_FONKSIYONLARI;

extern ADAK_SQL *G_YONETIM_DB;
extern ADAK_SQL *DB;

bool        m_first_db_process = false;
int         m_veritabani_sayisi = 0;
static int  m_user_login_status = USER_ON_LOGIN_SCREEN;
QString     m_veritabani_connection_status = "";

/**************************************************************************************
                   INIT_PROGRAM  
***************************************************************************************/

void INIT_PROGRAM( ADAK_SQL_STRUCT *p_program_struct, void F_KULLANICI_TAM_YETKI_FONK(void))
{
    SET_PROGRAM_DB_STRUCTS( p_program_struct );
    SET_YONETIM_DB_STRUCTS( &yonetim_db );

    G_TAM_YETKILENDIRME_FONKSIYONLARI << F_KULLANICI_TAM_YETKI_FONK;
}

/**************************************************************************************
                   PROGRAMA_GIRIS
***************************************************************************************/

int PROGRAMA_GIRIS (int argc, char *argv[] , int display_id, int program_id, QWidget * parent)
{
    // Windows'ta E9.dll herseyi icersin diye yapilan uckagit - argc -1 ile cagriliyor. Silmeyelim
    if (argc < 0) {
        return 0;
    }
    // End of uckagit

    SET_ADAK_DISPLAY_ID( display_id );
    SET_ADAK_VERITABANI_ID( program_id );
    SET_SQL_ERROR_FUNC ( &OPEN_ADAK_SQL_ERROR_DIALOG  );

    INIT_PROGRAM_DEFAULTS();

    USER_LOGIN_INFO_STRUCT * USER_INFO = new USER_LOGIN_INFO_STRUCT;

    READ_SETTINGS( USER_INFO );

    if( FIRST_LOGINE_GIRECEK_MI( USER_INFO) EQ true  ) {
        FIRST_LOGIN( USER_INFO, true );
        m_first_db_process = true;
    }

    if ( START_PROGRAM( argc, argv, USER_INFO, parent) EQ ADAK_FAIL ) {
        return ADAK_FAIL;
    }

    if ( DB EQ NULL ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/************************************************************************
                                START_PROGRAM
**************************************************************************/

int START_PROGRAM(int argc, char *argv[], USER_LOGIN_INFO_STRUCT *P_USER_INFO, QWidget * parent)
{
    SET_USER_LOGIN_STATUS( USER_ON_INIT );

    if( P_USER_INFO->is_auto_login EQ true ) {
        SET_USER_LOGIN_STATUS( USER_LOGINNED );
        m_first_db_process = true;
    }

    if ( G_YONETIM_DB EQ NULL ) {
        if ( CONNECT_TO_DB( ADAK_YONETIM, P_USER_INFO, false ) EQ ADAK_FAIL ) {
            ADAK_WARNING( QObject::tr( "<b>%1</b> Database could not be determined!").arg( SQL_DBNAME_ONEK() + "adak_yonetim" ), NULL, NULL );
        }
    }

    //! bilgisayar ve kullanici tespit ediliyor.
    CHECK_KULLANICI_BILGILERI( P_USER_INFO );

    if ( PROCESS_CONSOLE_ARGUMENTS (argc,argv, P_USER_INFO) EQ ADAK_FAIL ) {
        return ADAK_FAIL;
    }

    if( TRY_AUTO_LOGIN( P_USER_INFO ) EQ ADAK_OK ) {
        return ADAK_OK;
    }

    if ( m_first_db_process EQ false ) {
        SET_USER_LOGIN_STATUS( USER_ON_LOGIN_SCREEN );
        if ( OPEN_LOGIN_BATCH ( P_USER_INFO, parent ) EQ false ) {
            return ADAK_FAIL;
        }
    }

    if ( P_USER_INFO->is_first_login EQ true ) {
        FIRST_LOGIN( P_USER_INFO, false );
    }

    if ( LOGIN( P_USER_INFO ) EQ ADAK_OK ) {
        WRITE_SETTINGS( P_USER_INFO );
        SET_USER_LOGIN_STATUS( USER_LOGINNED );
    }

    return ADAK_OK;
}

/**************************************************************************************
                                RESET_PROGRAM_CORE
***************************************************************************************/

int  RESET_PROGRAM_CORE( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    int temp_login_status = GET_USER_LOGIN_STATUS();

    SET_USER_LOGIN_STATUS( USER_ON_INIT );

    if ( TRY_TO_CONNECT_DB( P_USER_INFO ) EQ ADAK_FAIL ) {
        SET_USER_LOGIN_STATUS( temp_login_status );
        return ADAK_FAIL;
    }
    if ( SET_BILGISAYAR_INFO( P_USER_INFO ) EQ ADAK_OK ) {

        if ( SET_KULLANICI_INFO  ( P_USER_INFO ) EQ ADAK_OK ) {
            SET_KULLANICI_PAROLA ( P_USER_INFO );
        }

        SET_VERITABANI_INFO( P_USER_INFO );
    }

    if ( TRY_AUTO_LOGIN ( P_USER_INFO ) EQ ADAK_OK ) {
        SET_USER_LOGIN_STATUS( temp_login_status );
        return ADAK_OK;
    }

    if ( P_USER_INFO->is_first_login EQ false ) {
        TRY_TO_SET_USER( P_USER_INFO );
        SET_USER_LOGIN_STATUS( temp_login_status );
        return ADAK_OK;
    }

    SET_USER_LOGIN_STATUS( temp_login_status );

    return ADAK_FAIL;
}

/**************************************************************************************
                                SET_BILGISAYAR_ADI
***************************************************************************************/

int SET_BILGISAYAR_INFO ( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL OR G_YONETIM_DB->SQL_TABLES_EXISTS("") EQ false ) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    QString     mac_adresi      = "Non-registered Mac Address(s):";
    QStringList mac_adressleri  = GET_MAC_ADDRESS();

    QString dongle_ip  = GET_DONGLE_IP();

    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar" , "bilgisayar_id, dongle_ip, bilgisayar_kodu, bilgisayar_adi, mac_adresi_kablolu, mac_adresi_kablosuz, mac_adresi_3g",
                               "silinmis_mi = 0 AND ( "
                               "   mac_adresi_kablolu    = :mac_1_kablolu "
                               "OR mac_adresi_kablosuz   = :mac_1_kablosuz "
                               "OR mac_adresi_3g         = :mac_1_3g "
                               "OR mac_adresi_kablolu    = :mac_2_kablolu "
                               "OR mac_adresi_kablosuz   = :mac_2_kablosuz "
                               "OR mac_adresi_3g         = :mac_2_3g "
                               "OR mac_adresi_kablolu    = :mac_3_kablolu "
                               "OR mac_adresi_kablosuz   = :mac_3_kablosuz "
                               "OR mac_adresi_3g         = :mac_3_3g )","");

    sql_query.SET_VALUE ( ":mac_1_kablolu",   mac_adressleri.at(0) );
    sql_query.SET_VALUE ( ":mac_1_kablosuz",  mac_adressleri.at(0) );
    sql_query.SET_VALUE ( ":mac_1_3g",        mac_adressleri.at(0) );

    mac_adresi.append(QString("\n" + mac_adressleri.at(0)));

    if ( mac_adressleri.size() > 1 ) {
        sql_query.SET_VALUE ( ":mac_2_kablolu",   mac_adressleri.at(1) );
        sql_query.SET_VALUE ( ":mac_2_kablosuz",  mac_adressleri.at(1) );
        sql_query.SET_VALUE ( ":mac_2_3g",        mac_adressleri.at(1) );
        mac_adresi.append(QString("\n" + mac_adressleri.at(1)));
    }
    else {
        sql_query.SET_VALUE ( ":mac_2_kablolu",   mac_adressleri.at(0) );
        sql_query.SET_VALUE ( ":mac_2_kablosuz",  mac_adressleri.at(0) );
        sql_query.SET_VALUE ( ":mac_2_3g",        mac_adressleri.at(0) );
    }

    if ( mac_adressleri.size() > 2 ) {
        sql_query.SET_VALUE ( ":mac_3_kablolu",   mac_adressleri.at(2) );
        sql_query.SET_VALUE ( ":mac_3_kablosuz",  mac_adressleri.at(2) );
        sql_query.SET_VALUE ( ":mac_3_3g",        mac_adressleri.at(2) );
        mac_adresi.append(QString("\n" + mac_adressleri.at(2)));
    }
    else {
        sql_query.SET_VALUE ( ":mac_3_kablolu",   mac_adressleri.at(0) );
        sql_query.SET_VALUE ( ":mac_3_kablosuz",  mac_adressleri.at(0) );
        sql_query.SET_VALUE ( ":mac_3_3g",        mac_adressleri.at(0) );
    }

    if ( sql_query.SELECT() EQ 0 ) { // Mac adres ile kayit bulunamadi
        if ( dongle_ip.isEmpty() EQ false ) { // Dongle varsa kontrol et

            sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar" , "bilgisayar_id, bilgisayar_kodu, bilgisayar_adi",
                                       "silinmis_mi = 0 AND dongle_ip = :dongle_ip","");

            sql_query.SET_VALUE      (":dongle_ip" , dongle_ip );

            if ( sql_query.SELECT() NE 0 ) { // Dongle ait kayit var.Setle
                sql_query.NEXT();
                P_USER_INFO->bilgisayar_id   =  sql_query.VALUE(0).toInt();
                P_USER_INFO->bilgisayar_kodu =  sql_query.VALUE(1).toString();
                P_USER_INFO->bilgisayar_adi  =  sql_query.VALUE(2).toString();
                return ADAK_OK;
            }
        }
    }
    else { //Mac adresinden kayit bulundu
        sql_query.NEXT();

        if ( sql_query.VALUE("dongle_ip").toString().trimmed().isEmpty() EQ true OR
             sql_query.VALUE("dongle_ip").toString().trimmed() EQ dongle_ip) {  // Dongle ipsini kontrol et.Ayni ise setle
            P_USER_INFO->bilgisayar_id   =  sql_query.VALUE(0).toInt();
            P_USER_INFO->bilgisayar_kodu =  sql_query.VALUE(2).toString();
            P_USER_INFO->bilgisayar_adi  =  sql_query.VALUE(3).toString();
            return ADAK_OK;
        }
    }

    if ( GET_USER_LOGIN_STATUS() EQ USER_ON_LOGIN_SCREEN ) {
        ADAK_ERROR( QString(QObject::tr("This computer is not registered in the system.\n%1")).arg(mac_adresi)   , NULL,NULL);
    }

    P_USER_INFO->bilgisayar_id   =  -1;
    P_USER_INFO->bilgisayar_kodu =  "Non-registered";

    return ADAK_FAIL;
}

/**************************************************************************************
                                SET_KULLANICI_KODU
***************************************************************************************/

int SET_KULLANICI_INFO( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id, kullanici_adi, kullanici_kodu","silinmis_mi = 0 ","");

    if( P_USER_INFO->kullanici_kodu.isEmpty() EQ false ) {
        sql_query.AND_EKLE ( "kullanici_kodu = :kullanici_kodu" );
        sql_query.SET_VALUE( ":kullanici_kodu", P_USER_INFO->kullanici_kodu );
    }
    else {
        sql_query.AND_EKLE ( "kullanici_id != :kullanici_id");
        sql_query.SET_VALUE( ":kullanici_id", 1); // yonetici haric
    }

    if ( sql_query.SELECT() EQ 0) {
        if ( GET_USER_LOGIN_STATUS() EQ USER_ON_LOGIN_SCREEN ) {
            ADAK_ERROR( QObject::tr("This coded user not found") , NULL,NULL);
        }
        return ADAK_FAIL;
    }

    sql_query.NEXT ();

    P_USER_INFO->kullanici_id   = sql_query.VALUE( 0 ).toInt();
    P_USER_INFO->kullanici_adi  = sql_query.VALUE( 1 ).toString();
    P_USER_INFO->kullanici_kodu = sql_query.VALUE( 2 ).toString();

    if ( P_USER_INFO->kullanici_id NE 1) {
         SQL_QUERY query (G_YONETIM_DB);

         query.PREPARE_SELECT("ynt_bilgisayar_kullanicilari","id","bilgisayar_id = :bilgisayar_id AND kullanici_id = :kullanici_id");
         query.SET_VALUE     (":bilgisayar_id",P_USER_INFO->bilgisayar_id);
         query.SET_VALUE     (":kullanici_id" ,P_USER_INFO->kullanici_id );

         if ( query.SELECT() EQ 0 ) {
            if ( GET_USER_LOGIN_STATUS() EQ USER_ON_LOGIN_SCREEN ) {
                ADAK_ERROR( QObject::tr("This user is not registered on the computer.") , NULL,NULL);
            }
            return ADAK_FAIL;
        }
    }

    TRY_TO_SET_DB( P_USER_INFO );

    return ADAK_OK;
}

/**************************************************************************************
                                TRY_TO_SET_DB
***************************************************************************************/

int TRY_TO_SET_DB ( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabani_kullanicilari","veritabani_id","kullanici_id = :kullanici_id","");

    sql_query.SET_VALUE      ( ":kullanici_id" , P_USER_INFO->kullanici_id );

    sql_query.SELECT();

    switch (sql_query.NUM_OF_ROWS()) {
        case 1 :
            sql_query.NEXT();

            if ( P_USER_INFO->veritabani_id EQ -1 ) {
                P_USER_INFO->veritabani_id = sql_query.VALUE(0).toInt();
            }

            sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_ismi, veritabani_tanimi","veritabani_id = :veritabani_id ","");

            sql_query.SET_VALUE      ( ":veritabani_id" , P_USER_INFO->veritabani_id );

            if ( sql_query.SELECT() EQ 0 ) {
                break;
            }

            sql_query.NEXT();

            P_USER_INFO->veritabani_kodu    = sql_query.VALUE(0).toString();
            P_USER_INFO->veritabani_tanimi  = sql_query.VALUE(1).toString();

            return ADAK_OK;
        default :
            break;
    }

    return ADAK_FAIL;
}

/**************************************************************************************
                            SET_KULLANICI_PAROLA
***************************************************************************************/

int SET_KULLANICI_PAROLA ( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if ( P_USER_INFO->is_first_login EQ true ) {
        return ADAK_OK;
    }

    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_id = :kullanici_id AND parola = :parola AND silinmis_mi = 0","" );

    sql_query.SET_VALUE      ( ":kullanici_id", P_USER_INFO->kullanici_id );

    QByteArray hash = QCryptographicHash::hash ( P_USER_INFO->kullanici_sifre.toUtf8(), QCryptographicHash::Md5 );
    sql_query.SET_VALUE ( ":parola", QString( hash.toHex() ) );

    if ( sql_query.SELECT() EQ 0 ) {
        if ( GET_USER_LOGIN_STATUS() EQ USER_ON_LOGIN_SCREEN ) {
            ADAK_ERROR(QObject::tr( "Invalid password" ) , NULL,NULL);
        }
        return ADAK_FAIL;
    }

    return ADAK_OK;
}


/**************************************************************************************
                            SET_VERITABANI_KODU
***************************************************************************************/

int SET_VERITABANI_INFO ( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        return ADAK_FAIL;
    }

    SQL_QUERY query ( G_YONETIM_DB );

    query.PREPARE_SELECT ("ynt_veritabanlari","veritabani_id, veritabani_tanimi","","");

    query.AND_EKLE ( "veritabani_ismi = :veritabani_ismi" );
    query.SET_VALUE( ":veritabani_ismi", P_USER_INFO->veritabani_kodu );

    if ( query.SELECT("veritabani_id", 0, 1) EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    P_USER_INFO->veritabani_id      = query.VALUE(0).toInt();
    P_USER_INFO->veritabani_tanimi  = query.VALUE(1).toInt();

    query.PREPARE_SELECT("ynt_veritabani_kullanicilari","id","veritabani_id = :veritabani_id AND kullanici_id = :kullanici_id");

    query.SET_VALUE(":veritabani_id",P_USER_INFO->veritabani_id );
    query.SET_VALUE(":kullanici_id" ,P_USER_INFO->kullanici_id  );

    if ( query.SELECT() EQ 0 ) {
        P_USER_INFO->veritabani_id      = -1;
        P_USER_INFO->veritabani_kodu    = QObject::tr("Not Defined");
        P_USER_INFO->veritabani_tanimi  = QObject::tr("Not Defined");
        return ADAK_FAIL;
    }


    return ADAK_OK;
}

/**************************************************************************************
                            GET_PROGRAM_DB_COUNT
***************************************************************************************/

int GET_PROGRAM_DB_COUNT( int program_id, int p_ynt_kullanici_id )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        //Ilk giris sadece 1 db olacak
        return 1;
    }

    if ( p_ynt_kullanici_id EQ 1 ) {
        SQL_QUERY sql_query ( G_YONETIM_DB );

        sql_query.PREPARE_SELECT ( "ynt_veritabanlari" , "veritabani_id","","");

        return (sql_query.SELECT());
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabani_programlari","veritabani_id","program_id = :program_id","");
    sql_query.SET_VALUE(":program_id" , program_id);

    return (sql_query.SELECT());
}


/**************************************************************************************
                            GET_USER_DB_COUNT
***************************************************************************************/

int GET_USER_DB_COUNT( int p_ynt_kullanici_id )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        //Ilk giris sadece -1 olacak
        return -1;
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    if ( p_ynt_kullanici_id EQ 1) {
        sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","","");

        return (sql_query.SELECT("veritabani_ismi"));
    }

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari,ynt_veritabani_kullanicilari,ynt_veritabani_programlari","ynt_veritabanlari.veritabani_id",
                               "ynt_veritabanlari.veritabani_id = "
                               "ynt_veritabani_kullanicilari.veritabani_id AND kullanici_id =:kullanici_id AND "
                               "ynt_veritabanlari.veritabani_id = ynt_veritabani_programlari.veritabani_id AND "
                               "ynt_veritabani_programlari.program_id = :program_id ","");

    sql_query.SET_VALUE( ":kullanici_id", p_ynt_kullanici_id   );
    sql_query.SET_VALUE( ":program_id"  , ADAK_VERITABANI_ID());

    return (sql_query.SELECT("veritabani_ismi"));
}

/**************************************************************************************
                            INIT_CONSOLE_ARGUMENTS
***************************************************************************************/

int PROCESS_CONSOLE_ARGUMENTS(int argc, char *argv[], USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    for (int i = 1; i < argc; i++) {
        if (QString(argv[i]).toUpper() EQ "--CLEAR" OR QString(argv[i]).toUpper() EQ "-CLEAR" ) {
            ADAK_CLEAR_SETTINGS();
        }
    }

    P_USER_INFO->kullanici_veritabaninda_calisabilir = false;
    for (int i = 1; i < argc; i++) {
        if (QString(argv[i]).toUpper() EQ "--SQL" OR QString(argv[i]).toUpper() EQ "-SQL") {
            P_USER_INFO->kullanici_veritabaninda_calisabilir = true;
        }
    }

    if (QString(argv[1]).toUpper() EQ "--SQL_UPDATE" OR QString(argv[1]).toUpper() EQ "-SQL_UPDATE") {
        if ( argc < 4 ) {
            cerr << QObject::tr("The following format should be used to update\n\t\tE9 -sql_update ipveyahost port sql_motoru db_user db_passwd ").toStdString() << endl;
            exit(0);
        }

        SET_SQL_HOSTNAME    (QVariant(argv[2]).toString());
        SET_SQL_PORT        (QVariant(argv[3]).toInt()   );
        SET_SQL_MOTORU      (QVariant(argv[4]).toString());

        if ( argc >= 5 ) {
            SET_SQL_USERNAME    (QVariant(argv[5]).toString());
        }
        else {
            SET_SQL_USERNAME    (SQL_DEFAULT_USERNAME());
        }

        if ( argc EQ 6 ) {
            SET_SQL_PASSWORD    (QVariant(argv[6]).toString());
        }
        else {
            SET_SQL_PASSWORD    (SQL_DEFAULT_PASSWORD());
        }

        G_YONETIM_DB->UPDATE_TABLES( GET_YONETIM_DB_STRUCTS() );
        cerr << QObject::tr("Table update process is completed.").toStdString() << endl;
        exit(0);
    }

    if (QString(argv[1]).toUpper() EQ "--UPDATE_ADAKSTD" OR QString(argv[1]).toUpper() EQ "-UPDATE_ADAKSTD") {
        if ( argc < 4 ) {
            cerr << QObject::tr("The following format should be used for update\n\t\tE9 -update_adakstd ipveyahost port sql_motoru db_user db_passwd ").toStdString() << endl;
            exit(0);
        }

        SET_SQL_HOSTNAME    (QVariant(argv[2]).toString());
        SET_SQL_PORT        (QVariant(argv[3]).toInt()   );
        SET_SQL_MOTORU      (QVariant(argv[4]).toString());

        if ( argc >= 5 ) {
            SET_SQL_USERNAME    (QVariant(argv[5]).toString());
        }
        else {
            SET_SQL_USERNAME    (SQL_DEFAULT_USERNAME());
        }

        if ( argc EQ 6 ) {
            SET_SQL_PASSWORD    (QVariant(argv[6]).toString());
        }
        else {
            SET_SQL_PASSWORD    (SQL_DEFAULT_PASSWORD());
        }
        G_YONETIM_DB->UPDATE_TABLES( GET_YONETIM_DB_STRUCTS() );

        cerr << QObject::tr("ADAK_STD update process is completed.").toStdString() << endl;
        exit(0);
    }

    return ADAK_OK;
}

/**************************************************************************************
                                CHECK_USER_ACCESS
***************************************************************************************/

int CHECK_USER_ACCESS( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if (PROGRAMA_UYGUN_DONGLE_VARMI() EQ false) {
        ADAK_ERROR(QObject::tr("Unknown DONGLE/ it is not suitable DONGLE ! \n\n Dongle IP : ") + GET_DONGLE_IP(), NULL , NULL);
        exit (1);
    }

    if ( GET_USER_LOGIN_STATUS() EQ USER_ON_LOGIN_SCREEN ) {
        if (P_USER_INFO->bilgisayar_id EQ -1 AND P_USER_INFO->kullanici_id NE 1 AND P_USER_INFO->is_first_login EQ false) {
            QStringList mac_adresleri = GET_MAC_ADDRESS();
            QString     mac_adresi      = QObject::tr("Non-registered Mac Address (s):");

            for ( int i = 0 ; i < mac_adresleri.size() ; i++ ) {
                mac_adresi.append(QString("\n" + mac_adresleri.at(i)));
            }

            ADAK_ERROR(QString(QObject::tr("This computer is not registered in the system.\n%1")).arg(mac_adresi), NULL , NULL);
            return ADAK_FAIL;
        }

        if ( SET_KULLANICI_PAROLA( P_USER_INFO ) EQ ADAK_FAIL ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                    CHECK_KULLANICI_BILGISAYARA_KAYITLI_MI
***************************************************************************************/

int CHECK_KULLANICI_BILGISAYARA_KAYITLI_MI( int p_ynt_bilgisayar_id, int p_ynt_kullanici_id )
{
    if (IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL) {
        return ADAK_OK;
    }
    //! SQLITE OZEL
    if ( SQL_MOTORU() EQ "NONE" ) {
        if (G_YONETIM_DB->SQL_TABLES_EXISTS("") EQ false ){
            ADAK_ERROR(QObject::tr("No Tables in the database"), NULL, NULL);
            return ADAK_FAIL;
        }
    }

    if ( p_ynt_kullanici_id EQ 1 ) {
        return ADAK_OK;
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayar_kullanicilari","id",
                               "bilgisayar_id = :bilgisayar_id "
                               "AND kullanici_id    = :kullanici_id","");

    sql_query.SET_VALUE      ( ":bilgisayar_id", p_ynt_bilgisayar_id );
    sql_query.SET_VALUE      ( ":kullanici_id" , p_ynt_kullanici_id  );

    if ( sql_query.SELECT() EQ 0 ) {
        ADAK_ERROR(QObject::tr("This user is not registered on the computer.") , NULL,NULL);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                            TRY_TO_CONNECT_SERVER
***************************************************************************************/

int TRY_TO_CONNECT_DB( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    QString baglanti_status         = "";
    int     m_connection_status     = ADAK_OK;

    G_YONETIM_DB = NULL;

    while ( 1 ) {

        m_connection_status = ADAK_OK;
        if ( CONNECT_TO_SQL_DATABASE( P_USER_INFO->sql_sunucu, P_USER_INFO->sql_port , P_USER_INFO->sql_motoru,
                                      P_USER_INFO->sql_db_on_ek, P_USER_INFO->sql_username, P_USER_INFO->sql_password, "" ) EQ false ) {

            QString secilen_profil = SQL_SUNUCU_SEC(NULL);

            if ( secilen_profil EQ QObject::tr("New Server") ) {
                P_USER_INFO->secili_sql_profili = secilen_profil;
                return ADAK_FAIL;
            }

            READ_SETTINGS( P_USER_INFO, secilen_profil );
            WRITE_SETTINGS( P_USER_INFO );

            //! OTOMATIK SIFRE BU IKI ADA GORE URETILIYOR.
            SET_SQL_USERNAME( P_USER_INFO->sql_username );
            SET_SQL_HOSTNAME( P_USER_INFO->sql_sunucu   );

            if ( P_USER_INFO->is_auto_sifre EQ true ) {
                P_USER_INFO->sql_password = SQL_DEFAULT_PASSWORD();
            }

            m_connection_status = ADAK_FAIL;

            if ( secilen_profil.isEmpty() EQ true ) {
                return ADAK_FAIL;
            }
        }
        else {
            break;
        }
    }

    if ( m_connection_status EQ ADAK_OK ) {
        if ( CONNECT_TO_DB(ADAK_YONETIM, P_USER_INFO, false ) EQ ADAK_FAIL ) {
            return ADAK_FAIL;
        }
    }

    baglanti_status.append(QObject::tr("<br>Connection Established"));

    QString status_str = QString(QObject::tr("IP/Address        : <b>%1</b><br>Port: <b>%2</b><br>SQL Engine: <b>%3</b><br>"));

    if (GET_SQL_DB_DRIVER_ENUM( P_USER_INFO->sql_motoru) EQ SQLITE) {
        status_str = QString(QObject::tr("SQL Engine      : <b>SQLite</b><br>"));
    }
    else {
        status_str = QString(status_str)
                     .arg(P_USER_INFO->sql_sunucu)
                     .arg(P_USER_INFO->sql_port)
                     .arg(P_USER_INFO->sql_motoru);
    }

    m_veritabani_connection_status.clear();
    m_veritabani_connection_status.append(status_str);
    m_veritabani_connection_status.append(baglanti_status);


    return m_connection_status;
}

/**************************************************************************************
                   SET_DB_CONNECTION_STATUS
***************************************************************************************/

void SET_DB_CONNECTION_STATUS(QString p_conn_status)
{
    m_veritabani_connection_status = p_conn_status;
}

/**************************************************************************************
                   GET_DB_CONNECTION_STATUS
***************************************************************************************/

QString GET_DB_CONNECTION_STATUS()
{
    return m_veritabani_connection_status;
}

/**************************************************************************************
                   LOGIN
***************************************************************************************/

int LOGIN( USER_LOGIN_INFO_STRUCT *P_USER_INFO)
{
    SET_STATIC_VALUES( P_USER_INFO );

    G_YONETIM_DB->START_TRANSACTION();

    P_USER_INFO->veritabani_id = GET_VERITABANI_ID( P_USER_INFO->veritabani_kodu );

    if ( P_USER_INFO->kullanici_id EQ 1 AND P_USER_INFO->veritabani_id EQ -1 ) {
        if ( P_USER_INFO->veritabani_kodu.isEmpty() EQ true) {
            SQL_QUERY sql_query ( G_YONETIM_DB );

            sql_query.PREPARE_SELECT("ynt_veritabanlari","veritabani_ismi","","");

            if ( sql_query.SELECT() NE 0 ) {
                sql_query.NEXT();
                P_USER_INFO->veritabani_kodu = sql_query.VALUE(0).toString();
                SET_VERITABANI_INFO( P_USER_INFO );
            }
        }
    }

    bool yonetici_izni_alindi = false;

    for ( int i = 0 ; i < GET_PROGRAM_DB_STRUCTS().size() ; i++ ) {

        if ( GET_PROGRAM_DB_COUNT(GET_PROGRAM_DB_STRUCTS().at(i)->adak_program_id, P_USER_INFO->kullanici_id ) EQ 0 ) {

            yonetici_izni_alindi = YONETICI_IZNI_AL();
            break;
        }
        else {
            if ( DB->SQL_TABLES_EXISTS("") EQ false ) {

                yonetici_izni_alindi = YONETICI_IZNI_AL();
                break;
            }
        }
    }

    if ( P_USER_INFO->kullanici_id EQ 1 OR yonetici_izni_alindi EQ true ) {
        for ( int i = 0 ; i < GET_PROGRAM_DB_STRUCTS().size() ; i++ ) {
            SQL_QUERY sql_query ( G_YONETIM_DB );

            sql_query.PREPARE_SELECT("ynt_veritabani_programlari","id","veritabani_id =:veritabani_id AND program_id = :program_id","");

            sql_query.SET_VALUE      ( ":veritabani_id",   P_USER_INFO->veritabani_id   );
            sql_query.SET_VALUE      ( ":program_id",      GET_PROGRAM_DB_STRUCTS().at(i)->adak_program_id );

            //! veritabani var fakat tabloyok ise olusturmasi gerekiyor.
            if ( sql_query.SELECT() EQ 0 ) {
                QSplashScreen * splash = CREATE_SPLASH_SCREEN();
                splash->show();
                splash->showMessage((QString(QObject::tr("Preparing database. Please wait..."))), Qt::AlignCenter, Qt::white );
                DB->SQL_CREATE_TABLES(GET_PROGRAM_DB_STRUCTS().at(i));
                splash->finish(NULL);

                sql_query.PREPARE_INSERT ( "ynt_veritabani_programlari","id","veritabani_id, program_id");
                sql_query.SET_VALUE      ( ":veritabani_id",   P_USER_INFO->veritabani_id   );
                sql_query.SET_VALUE      ( ":program_id",      GET_PROGRAM_DB_STRUCTS().at(i)->adak_program_id );
                sql_query.INSERT();

                if (yonetici_izni_alindi EQ true) {
                    KULLANICI_TAM_YETKILENDIR (P_USER_INFO->kullanici_id);
                }
            }
            if( DB->SQL_TABLES_EXISTS("") EQ false ) {
                QSplashScreen * splash = CREATE_SPLASH_SCREEN();
                splash->show();
                splash->showMessage((QString(QObject::tr("Preparing database. Please wait..."))), Qt::AlignCenter, Qt::white );
                DB->SQL_CREATE_TABLES(GET_PROGRAM_DB_STRUCTS().at(i));
                splash->finish(NULL);

                if (yonetici_izni_alindi EQ true) {
                    KULLANICI_TAM_YETKILENDIR (P_USER_INFO->kullanici_id);
                }
            }
        }
    }

    SET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI ( KULLANICININ_CALISABILECEGI_VERITABANI_SAYISINI_BUL() );

    SET_STATIC_VALUES( P_USER_INFO );

    YONETIM_007_GIRIS_KAYDI();

    if ( P_USER_INFO->sql_tablolari_guncellestir EQ true ) {
        VERITABANLARINI_GUNCELLE();
    }

    if ( G_YONETIM_DB NE NULL ) {
        if ( QDate::currentDate().dayOfWeek() EQ 2 OR QDate::currentDate().dayOfWeek() EQ 5 ) {
            SQL_QUERY sql_query      ( G_YONETIM_DB );

            sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_id = :kullanici_id AND parola = :parola","" );

            sql_query.SET_VALUE      ( ":kullanici_id", 1 );

            QByteArray hash = QCryptographicHash::hash ( QString("").toUtf8(),QCryptographicHash::Md5 );
            sql_query.SET_VALUE ( ":parola", QString( hash.toHex() ) );

            if ( sql_query.SELECT() NE 0 ) {
                if (G_YONETIM_DB->GET_SQL_DRIVER_ENUM () NE SQLITE) {
                    ADAK_INFO(QObject::tr ("Attention!. \"Admin\" password is not assigned. You should assign \"Admin\" password for avoid security problems."), NULL,NULL);
                }
            }
        }
    }


    G_YONETIM_DB->COMMIT_TRANSACTION();

    return ADAK_OK;
}

/**************************************************************************************
                            YONETICI_IZNI_AL
***************************************************************************************/

bool YONETICI_IZNI_AL()
{
    SQL_QUERY query (G_YONETIM_DB);

    query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_id=:kullanici_id AND parola = :parola");
    query.SET_VALUE  ( ":kullanici_id", 1 );

    QByteArray hash = QCryptographicHash::hash ( QString("").toUtf8(),QCryptographicHash::Md5 );
    query.SET_VALUE  ( ":parola", QString( hash.toHex() ) );

    if ( query.SELECT() EQ 0 ) {
        if ( OPEN_YONETICI_IZNI_BATCH(NULL) EQ false ) {
            exit(88);
        }
    }

    return true;
}

/**************************************************************************************
                            GET_VERITABANI_ID
***************************************************************************************/

int GET_VERITABANI_ID( QString p_veritabani_ismi )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT("ynt_veritabanlari","veritabani_id","veritabani_ismi = :veritabani_ismi","");
    sql_query.SET_VALUE     (":veritabani_ismi" , p_veritabani_ismi );

    if (sql_query.SELECT() EQ 0 ) {
        return -1;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                            GET_VERITABANI_ISMI
***************************************************************************************/

QString GET_VERITABANI_ISMI( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    if ( P_USER_INFO->kullanici_id EQ 1) {
        sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id,veritabani_ismi,veritabani_tanimi");

        if ( sql_query.SELECT("veritabani_ismi") NE 0 ) {
            sql_query.NEXT();
            P_USER_INFO->veritabani_tanimi = sql_query.VALUE(2).toString();
            return sql_query.VALUE(1).toString();
        }
    }
    else {
        sql_query.PREPARE_SELECT ( "ynt_veritabanlari,ynt_veritabani_kullanicilari,ynt_veritabani_programlari","ynt_veritabanlari.veritabani_id,ynt_veritabanlari.veritabani_ismi,ynt_veritabanlari.veritabani_tanimi",
                                   "ynt_veritabanlari.veritabani_id = "
                                   "ynt_veritabani_kullanicilari.veritabani_id AND kullanici_id =:kullanici_id AND "
                                   "ynt_veritabanlari.veritabani_id = ynt_veritabani_programlari.veritabani_id AND "
                                   "ynt_veritabani_programlari.program_id = :program_id ","");

        sql_query.SET_VALUE( ":kullanici_id", P_USER_INFO->kullanici_id );
        sql_query.SET_VALUE( ":program_id"  , ADAK_VERITABANI_ID());

        if (sql_query.SELECT("veritabani_ismi") NE 0) {
            sql_query.NEXT();
            P_USER_INFO->veritabani_tanimi = sql_query.VALUE(2).toString();
            return sql_query.VALUE(1).toString();
        }
    }

    return "";
}


/**************************************************************************************
                            CONNECT_TO_DB
***************************************************************************************/

int CONNECT_TO_DB ( int db_type, USER_LOGIN_INFO_STRUCT *P_USER_INFO, bool p_sql_error )
{
    SET_STATIC_VALUES( P_USER_INFO);

    switch ( db_type ) {
        case ADAK_YONETIM: {
            QString db_name = QString(SQL_DBNAME_ONEK() + "adak_yonetim" );

            if ( G_YONETIM_DB EQ NULL ) {
                G_YONETIM_DB = CONNECT_TO_DATABASE( ADAK_YONETIM, db_name, P_USER_INFO->sql_motoru, p_sql_error );
            }
            if ( G_YONETIM_DB NE NULL ) {
                return ADAK_OK;
            }
            else {
                P_USER_INFO->is_first_login = true;
            }
            break;
        }
        case ADAK_PROGRAM: {
            QString db_name = QString(SQL_DBNAME_ONEK() + "adak_" + P_USER_INFO->veritabani_kodu );

            if ( DB EQ NULL ) {
                DB = CONNECT_TO_DATABASE( ADAK_PROGRAM, db_name, P_USER_INFO->sql_motoru , p_sql_error );
            }
            if ( DB NE NULL ) {
                return ADAK_OK;
            }
            break;
        }
        default:
        break;
    }

    return ADAK_FAIL;
}

/**************************************************************************************
                            TRY_TO_SET_USER
***************************************************************************************/

int TRY_TO_SET_USER ( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if ( IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL OR G_YONETIM_DB->SQL_TABLES_EXISTS("adak_yonetim") EQ false ) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayar_kullanicilari","kullanici_id","kullanici_id > 1 AND bilgisayar_id = :bilgisayar_id");

    sql_query.SET_VALUE      ( ":bilgisayar_id", P_USER_INFO->bilgisayar_id );

    int kullanici_sayisi = sql_query.SELECT();

    SET_BILGISAYARDA_KAYITLI_KULLANICI_SAYISI ( kullanici_sayisi );

    switch ( kullanici_sayisi ) {
        case 0    : // Sadece yonetici var
            //RESET_USER_VARIABLES();
            break;
        case 1    :
            sql_query.NEXT();

            P_USER_INFO->kullanici_id = sql_query.VALUE(0).toInt();

            sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_kodu, kullanici_adi","kullanici_id = :kullanici_id","" );

            sql_query.SET_VALUE      ( ":kullanici_id", P_USER_INFO->kullanici_id  );

            if ( sql_query.SELECT() EQ 0 ) {
                break;
            }

            sql_query.NEXT();

            P_USER_INFO->kullanici_kodu = sql_query.VALUE(0).toString();
            P_USER_INFO->kullanici_adi  = sql_query.VALUE(1).toString();

            TRY_TO_SET_DB( P_USER_INFO );

            break;
        default  :  // birden fazla var
            break;
    }

    return ADAK_OK;
}

/**************************************************************************************
                            TRY_AUTO_LOGIN
***************************************************************************************/

int TRY_AUTO_LOGIN( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    if ( P_USER_INFO->is_auto_login EQ false ) {
        return ADAK_FAIL;
    }

    if ( P_USER_INFO->veritabani_kodu EQ QObject::tr("Not Defined")) {
        return ADAK_FAIL;
    }

    if ( CHECK_USER_ACCESS( P_USER_INFO ) EQ ADAK_FAIL ) {
        return ADAK_FAIL;
    }

    if ( CHECK_KULLANICI_BILGISAYARA_KAYITLI_MI( P_USER_INFO->bilgisayar_id, P_USER_INFO->kullanici_id ) EQ ADAK_FAIL ) {
        return ADAK_FAIL;
    }

    if ( CONNECT_TO_DB( ADAK_PROGRAM, P_USER_INFO, false ) EQ ADAK_FAIL ) {
        m_first_db_process = false;
        return ADAK_FAIL;
    }

    return LOGIN( P_USER_INFO );
}

/**************************************************************************************
                        VERITABANLARINI_GUNCELLE
***************************************************************************************/

int VERITABANLARINI_GUNCELLE()
{
    if ( DB EQ NULL OR G_YONETIM_DB EQ NULL) {
        return ADAK_FAIL;
    }

    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
    splash->show();
    splash->showMessage((QString(QObject::tr("SQL tables is organizing in database. Please wait..."))), Qt::AlignCenter, Qt::white );

    G_YONETIM_DB->UPDATE_TABLES( GET_YONETIM_DB_STRUCTS() );
    DB->UPDATE_TABLES          ( GET_PROGRAM_DB_STRUCTS() );

    splash->finish(NULL);

    return ADAK_OK;
}

/**************************************************************************************
                                IS_DB_CONNECTED
***************************************************************************************/

int IS_DB_CONNECTED ( int db_type )
{
    switch ( db_type ) {
        case ADAK_YONETIM:
            if ( G_YONETIM_DB EQ NULL ) {
                return ADAK_FAIL;
            }
        break;
        case ADAK_PROGRAM:
            if ( DB EQ NULL ) {
                return ADAK_FAIL;
            }
        break;
        default:
             return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                                SET_STATIC_VALUES
***************************************************************************************/

void SET_STATIC_VALUES ( USER_LOGIN_INFO_STRUCT *P_USER_INFO )
{
    SET_SQL_PORT         ( P_USER_INFO->sql_port             );
    SET_SQL_MOTORU       ( P_USER_INFO->sql_motoru           );
    SET_SQL_HOSTNAME     ( P_USER_INFO->sql_sunucu           );
    SET_SQL_DBNAME_ONEK  ( P_USER_INFO->sql_db_on_ek         );
    SET_SQL_USERNAME     ( P_USER_INFO->sql_username         );
    SET_SQL_PASSWORD     ( P_USER_INFO->sql_password         );
    if ( P_USER_INFO->is_auto_sifre EQ true ) {
        SET_SQL_PASSWORD     ( SQL_DEFAULT_PASSWORD() );
    }

    SET_BILGISAYAR_ID    ( P_USER_INFO->bilgisayar_id        );
    SET_KULLANICI_ID     ( P_USER_INFO->kullanici_id         );
    SET_VERITABANI_ID    ( P_USER_INFO->veritabani_id        );
    SET_BILGISAYAR_ADI   ( P_USER_INFO->bilgisayar_adi       );
    SET_BILGISAYAR_KODU  ( P_USER_INFO->bilgisayar_kodu      );
    SET_KULLANICI_KODU   ( P_USER_INFO->kullanici_kodu       );
    SET_KULLANICI_ADI    ( P_USER_INFO->kullanici_adi        );
    SET_VERITABANI_ISMI  ( P_USER_INFO->veritabani_kodu      );
    SET_VERITABANI_TANIMI( P_USER_INFO->veritabani_tanimi    );
}

/**************************************************************************************
                                CLEAR_STATIC_VALUES
***************************************************************************************/

void CLEAR_STATIC_VALUES()
{
    SET_BILGISAYAR_ID    ( -1 );
    SET_BILGISAYAR_KODU  ( "" );
    SET_BILGISAYAR_ADI   ( "" );

    SET_KULLANICI_ID     ( -1 );
    SET_KULLANICI_KODU   ( "" );
    SET_KULLANICI_ADI    ( "" );

    SET_VERITABANI_ID    ( -1 );
    SET_VERITABANI_ISMI  ( "" );
    SET_VERITABANI_TANIMI( "" );
}


/**************************************************************************************
                                CONNECT_TO_DATABASE
***************************************************************************************/

ADAK_SQL * CONNECT_TO_DATABASE( int p_db_type, QString p_db_name, QString p_sql_motoru, bool p_sql_error )
{
    QList<ADAK_SQL_STRUCT *> DB_STRUCT_LIST = GET_YONETIM_DB_STRUCTS();

    if( p_db_type EQ ADAK_PROGRAM ) {
        DB_STRUCT_LIST = GET_PROGRAM_DB_STRUCTS();
    }

    ADAK_SQL * db_conn = CREATE_ADAK_SQL_CONNECTION( DB_STRUCT_LIST, p_db_name, GET_SQL_DB_DRIVER_ENUM( p_sql_motoru ) );

    if ( db_conn->CONNECT_TO_DATABASE( p_sql_error ) EQ ADAK_FAIL ) {
        db_conn = NULL;
    }

    return db_conn;
}

/**************************************************************************************
                                CREATE_TABLES_OF_YONETIM
***************************************************************************************/

int CREATE_TABLES_OF_YONETIM( QString p_db_name )
{
    if ( CREATE_TABLES_OF_DATABASE( GET_YONETIM_DB_STRUCTS() , ADAK_YONETIM, p_db_name, NULL) EQ false ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                                CREATE_TABLES_OF_PROGRAM
***************************************************************************************/

int CREATE_TABLES_OF_PROGRAM( USER_LOGIN_INFO_STRUCT *P_USER_INFO, bool start_yonetim_transaction)
{
    QString db_name = SQL_DBNAME_ONEK() + "adak_" + P_USER_INFO->veritabani_kodu;

    if ( CREATE_TABLES_OF_DATABASE( GET_PROGRAM_DB_STRUCTS(), ADAK_PROGRAM, db_name, NULL) EQ false ) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query(G_YONETIM_DB);

    if ( start_yonetim_transaction EQ true ) {
        G_YONETIM_DB->START_TRANSACTION();
    }

    sql_query.PREPARE_INSERT ( "ynt_veritabanlari","veritabani_id","veritabani_ismi, veritabani_tanimi ");

    sql_query.SET_VALUE      ( ":veritabani_ismi",   P_USER_INFO->veritabani_kodu );
    sql_query.SET_VALUE      ( ":veritabani_tanimi", P_USER_INFO->veritabani_tanimi  );

    P_USER_INFO->veritabani_id = sql_query.INSERT();

    for ( int i = 0 ; i < GET_PROGRAM_DB_STRUCTS().size() ; i++ ) {
        sql_query.PREPARE_INSERT ( "ynt_veritabani_programlari","id","veritabani_id, program_id");

        sql_query.SET_VALUE      ( ":veritabani_id", P_USER_INFO->veritabani_id   );
        sql_query.SET_VALUE      ( ":program_id"   , GET_PROGRAM_DB_STRUCTS().at(i)->adak_program_id );

        sql_query.INSERT();
    }

    if ( start_yonetim_transaction EQ true ) {
        G_YONETIM_DB->COMMIT_TRANSACTION();
    }

    return ADAK_OK;
}

/**************************************************************************************
                                CREATE_TABLES_OF_DATABASE
***************************************************************************************/

int CREATE_TABLES_OF_DATABASE(QList<ADAK_SQL_STRUCT *> db_struct, int db_type, QString p_db_name, QWidget *parent)
{
    QSplashScreen * splash = CREATE_SPLASH_SCREEN();

    splash->show();
    splash->showMessage ( QObject::tr( "The program is preparing for first usage. Please wait..." ), Qt::AlignCenter, Qt::white );

    if( db_type EQ ADAK_YONETIM ) {
        G_YONETIM_DB->SQL_CREATE_TABLES  ( db_struct, p_db_name );
    }
    else { //ADAK_PROGRAM
        DB->SQL_CREATE_TABLES( db_struct, p_db_name );
    }

    splash->finish(parent);

    return ADAK_OK;
}

/**************************************************************************************
                            KULLANICI_TAM_YETKILENDIR
***************************************************************************************/

void KULLANICI_TAM_YETKILENDIR(int p_kullanici_id)
{
    if ( p_kullanici_id > 1 ) {
        if ( DB NE NULL AND G_TAM_YETKILENDIRME_FONKSIYONLARI.size() NE 0 ) {
            SET_KULLANICI_ID( p_kullanici_id );

            for ( int i = 0 ; i < G_TAM_YETKILENDIRME_FONKSIYONLARI.size() ; i++ ) {
                if ( G_TAM_YETKILENDIRME_FONKSIYONLARI.at(i) NE NULL ) {
                    G_TAM_YETKILENDIRME_FONKSIYONLARI.at(i)();
                }
            }
        }
    }
}

/**************************************************************************************
                                ADD_YONETICI_USER
***************************************************************************************/

void ADD_YONETICI_USER( QString p_yonetici_parolasi )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_INSERT( "ynt_kullanicilar", "kullanici_id", "kullanici_kodu, kullanici_adi,silinmis_mi, parola");

    sql_query.SET_VALUE ( ":kullanici_kodu" , "yonetici" );
    sql_query.SET_VALUE ( ":kullanici_adi"  , "Yönetim Kullanıcısı");
    sql_query.SET_VALUE ( ":silinmis_mi"    , 0           );

    QByteArray hash     = QCryptographicHash::hash(QString( p_yonetici_parolasi ).toUtf8(), QCryptographicHash::Md5);

    sql_query.SET_VALUE ( ":parola", QString( hash.toHex()));

    sql_query.INSERT();
}

/**************************************************************************************
                                FIRS_LOGINE_GIRECEK_MI
***************************************************************************************/

bool FIRST_LOGINE_GIRECEK_MI(USER_LOGIN_INFO_STRUCT *P_USER_INFO)
{
    //! db var settings yoksa //sql lite ile giris yapilmaya calisiliyor.
    if( P_USER_INFO->sql_motoru.isEmpty() EQ true OR P_USER_INFO->veritabani_kodu.isEmpty() EQ true  ) {

        QString yonetim_db_file_path = QDir::currentPath() + "/" + SQL_DBNAME_ONEK() + "adak_yonetim.db";
        if ( FILE_EXISTS( yonetim_db_file_path ) EQ false ) {
            return true;
        }

        //! YONETIM DB VAR

        SET_SQL_MOTORU( "NONE" );

        P_USER_INFO->sql_motoru         = "NONE";
        P_USER_INFO->secili_sql_profili = QObject::tr("Locak Disk");
        P_USER_INFO->veritabani_kodu    = "";

        QString yonetim_db_name = SQL_DBNAME_ONEK() + "adak_yonetim";

        G_YONETIM_DB = CONNECT_TO_DATABASE( ADAK_YONETIM, yonetim_db_name, P_USER_INFO->sql_motoru );

        if ( G_YONETIM_DB->SQL_TABLES_EXISTS("") EQ true ) {
            //! Tablolar Mevcut

            //! BILGISAYAR VE KULLANICI OKUNMAYA CALISILIYOR.
            CHECK_KULLANICI_BILGILERI( P_USER_INFO );

            WRITE_SETTINGS( P_USER_INFO );
            return false;
        }

        return true;
    }

    //! SETTINGS VAR YONETIM_DB YOKSA
    if ( P_USER_INFO->sql_motoru EQ "NONE" ) {

        QString yonetim_db_file_path = QDir::currentPath() + "/" + SQL_DBNAME_ONEK() + "adak_yonetim.db";
        if ( FILE_EXISTS( yonetim_db_file_path ) EQ false ) {
            return true;
        }
    }

    return false;
}

/**************************************************************************************
                                PROGRAM_DB_VAR_MI
***************************************************************************************/

bool PROGRAM_DB_VAR_MI( QString p_veritabani_kodu )
{
    QString program_db_file_path = QDir::currentPath() + "/" + SQL_DBNAME_ONEK() + "adak_" + p_veritabani_kodu + ".db";
    if ( FILE_EXISTS( program_db_file_path ) EQ false ) {
        return false;
    }
    return true;
}

/**************************************************************************************
                        INIT_PROGRAM_DEFAULTS
***************************************************************************************/

void INIT_PROGRAM_DEFAULTS( )
{
    Q_INIT_RESOURCE ( kernel_icons );
    Q_INIT_RESOURCE ( yonetim_icons );
    Q_INIT_RESOURCE ( ods_io );

    QCoreApplication::addLibraryPath( QDir::currentPath() );
    QCoreApplication::addLibraryPath( QDir::currentPath()                   + "/lib" );
    QCoreApplication::addLibraryPath( QDir::currentPath()                   + "/plugins" );
    QCoreApplication::addLibraryPath( QCoreApplication::applicationDirPath() );
    QCoreApplication::addLibraryPath( QCoreApplication::applicationDirPath() + "/lib" );
    QCoreApplication::addLibraryPath( QCoreApplication::applicationDirPath() + "/plugins" );

    #ifdef WIN32
        QFont serifFont ( "Sans Serif", 10, QFont::Normal );
        QApplication::setFont ( serifFont );
    #endif

    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QCoreApplication::setOrganizationName(QObject::tr("Adak Technology"));
    QCoreApplication::setOrganizationDomain(ADAK_PROGRAM_WEB(ADAK_DISPLAY_ID()));
    QCoreApplication::setApplicationName(ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));
}

/**************************************************************************************
                        CHECK_KULLANICI_BILGILERI
***************************************************************************************/

int CHECK_KULLANICI_BILGILERI(USER_LOGIN_INFO_STRUCT *P_USER_INFO)
{
    if ( SET_BILGISAYAR_INFO( P_USER_INFO ) EQ ADAK_FAIL ) {
        P_USER_INFO->kullanici_kodu = "";
        P_USER_INFO->kullanici_adi  = "";
        P_USER_INFO->kullanici_id   = -1;
        // login batch gidinde tekrar setting okudugu icin settingsi degistirmem gerekli
        WRITE_SETTINGS( P_USER_INFO );
        return ADAK_FAIL;
    }

    if ( P_USER_INFO->bilgisayar_id NE -1 ) {
        if ( SET_KULLANICI_INFO( P_USER_INFO ) EQ ADAK_FAIL ) {
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                        SET_USER_LOGIN_STATUS
***************************************************************************************/

void SET_USER_LOGIN_STATUS(int p_login_status)
{
    m_user_login_status = p_login_status;
}


/**************************************************************************************
                        GET_USER_LOGIN_STATUS
***************************************************************************************/

int GET_USER_LOGIN_STATUS()
{
    return m_user_login_status;
}
