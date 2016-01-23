#include <QSplashScreen>
#include <QCryptographicHash>
#include "first_login.h"
#include "adak_sql.h"
#include "login.h"
#include "login_utils.h"
#include "program_start.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "yardim_hatirlatma_dialog.h"
#include "login_enum.h"

extern ADAK_SQL *G_YONETIM_DB;
extern ADAK_SQL *DB;

/***********************************************************
                     FIRST_LOGIN
************************************************************/

int FIRST_LOGIN( USER_LOGIN_INFO_STRUCT * P_USER_LOGIN_INFO, bool p_ilk_kurulum )
{
    //! PROGRAM ILK CALISTIGINDA BURAYA DUSER VE GEREKLI
    //! BUTUN VERI TABANLARINI VE YETKI ATAMASINI YAPAR.

    //! NOTE : VERITABANI ILK YARATILDIGINDA DEFAULT OLARAK
    //! OLUSACAK KULLANICI VE BILGISAYAR BILGILERI

    P_USER_LOGIN_INFO->bilgisayar_kodu    = QObject::tr ("C01");
    P_USER_LOGIN_INFO->bilgisayar_adi     = QObject::tr ("C01 Computer");
    P_USER_LOGIN_INFO->kullanici_kodu     = QObject::tr ("U01");
    P_USER_LOGIN_INFO->kullanici_adi      = QObject::tr ("U01 User");
    P_USER_LOGIN_INFO->kullanici_sifre   = "";

    if( p_ilk_kurulum EQ true ) {
        P_USER_LOGIN_INFO->sql_motoru         = "NONE";
        P_USER_LOGIN_INFO->secili_sql_profili = QObject::tr ("Local Disk");
        P_USER_LOGIN_INFO->veritabani_kodu    = "000";
    }

    P_USER_LOGIN_INFO->veritabani_tanimi  = QString(QObject::tr ("%1 Database")).arg( P_USER_LOGIN_INFO->veritabani_kodu );

    if ( P_USER_LOGIN_INFO->is_first_login EQ true ) {
        QSplashScreen * splash = CREATE_SPLASH_SCREEN();
        splash->show();
        splash->showMessage((QString("Configuring program for first usage. Please wait ...")), Qt::AlignCenter, Qt::white );
        splash->finish(NULL);
    }

    //! SQL_LITE VERITABANINA BAGLANILMAYA CALISILDIGINDA OTOMATIK OLARAK
    //! BAGLANILAN ISIMDE BIR VERITABANI OLUSTURUR

    QString program_db_name = SQL_DBNAME_ONEK() + "adak_" + P_USER_LOGIN_INFO->veritabani_kodu;
    QString yonetim_db_name = SQL_DBNAME_ONEK() + "adak_yonetim";

    G_YONETIM_DB = CONNECT_TO_DATABASE( ADAK_YONETIM, yonetim_db_name, P_USER_LOGIN_INFO->sql_motoru );
    DB           = CONNECT_TO_DATABASE( ADAK_PROGRAM, program_db_name, P_USER_LOGIN_INFO->sql_motoru );

    G_YONETIM_DB->START_TRANSACTION();

    //! [1] YENI BIR VERI TABANI OLSTURULUYOR

    if( p_ilk_kurulum EQ false ) {
        QString uyari_string = QString (QObject::tr("Tables needed do not exist in database!"
            "\nWill create tables and a new user according to the information you gave.\n\n Will do;\n"
            "Create database : %1 - %2 .\n"
            "Create %3 : user and give it all the permissions."))
            .arg( P_USER_LOGIN_INFO->veritabani_kodu )
            .arg( P_USER_LOGIN_INFO->veritabani_tanimi  )
            .arg( P_USER_LOGIN_INFO->kullanici_kodu  );

        if ( P_USER_LOGIN_INFO->is_first_login EQ true ) {
            ADAK_MSG_ENUM answer = ADAK_YES_NO_CANCEL(uyari_string , NULL, NULL);

            if (answer NE ADAK_YES) {
                return ADAK_OK;
            }
        }
    }

    if ( P_USER_LOGIN_INFO->kullanici_id EQ 1 ) {
        return ADAK_OK;
    }

    if ( CREATE_TABLES_OF_YONETIM( yonetim_db_name ) EQ ADAK_FAIL ) {
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT("ynt_kullanicilar", "kullanici_kodu","kullanici_kodu = :kullanici_kodu","");
    sql_query.SET_VALUE ( ":kullanici_kodu" , P_USER_LOGIN_INFO->kullanici_kodu );

    if ( sql_query.SELECT() EQ 0 ) {

        ADD_YONETICI_USER( P_USER_LOGIN_INFO->kullanici_sifre );

        sql_query.PREPARE_INSERT ( "ynt_kullanicilar","kullanici_id","kullanici_kodu, kullanici_adi,silinmis_mi, parola");

        sql_query.SET_VALUE ( ":kullanici_kodu" , P_USER_LOGIN_INFO->kullanici_kodu );
        sql_query.SET_VALUE ( ":kullanici_adi"  , P_USER_LOGIN_INFO->kullanici_adi  );
        sql_query.SET_VALUE ( ":silinmis_mi"    , 0 );

        QByteArray hash = QCryptographicHash::hash(QString(P_USER_LOGIN_INFO->kullanici_sifre).toUtf8(), QCryptographicHash::Md5);

        sql_query.SET_VALUE ( ":parola", QString( hash.toHex()));

        P_USER_LOGIN_INFO->kullanici_id = sql_query.INSERT();

        QStringList mac_adresleri = GET_MAC_ADDRESS();

        sql_query.PREPARE_INSERT ( "ynt_bilgisayarlar","bilgisayar_id","bilgisayar_kodu, bilgisayar_adi, "
                                   "mac_adresi_kablolu,mac_adresi_kablosuz,mac_adresi_3g, silinmis_mi ");

        sql_query.SET_VALUE( ":bilgisayar_kodu"     , P_USER_LOGIN_INFO->bilgisayar_kodu );
        sql_query.SET_VALUE( ":bilgisayar_adi"      , P_USER_LOGIN_INFO->bilgisayar_adi  );

        if ( mac_adresleri.size() >= 1 ) {
            sql_query.SET_VALUE( ":mac_adresi_kablolu", GET_MAC_ADDRESS().at(0)  );
        }
        else {
            sql_query.SET_VALUE( ":mac_adresi_kablolu", "" );
        }

        if ( mac_adresleri.size() >= 2 ) {
            sql_query.SET_VALUE( ":mac_adresi_kablosuz", GET_MAC_ADDRESS().at(1) );
        }
        else {
            sql_query.SET_VALUE( ":mac_adresi_kablosuz", "" );
        }

        if ( mac_adresleri.size() >= 3 ) {
            sql_query.SET_VALUE( ":mac_adresi_3g", GET_MAC_ADDRESS().at(2) );
        }
        else {
            sql_query.SET_VALUE( ":mac_adresi_3g", "" );
        }

        sql_query.SET_VALUE( ":silinmis_mi", 0 );

        P_USER_LOGIN_INFO->bilgisayar_id = sql_query.INSERT();

        //! VERITABANI ID OLUSUYOR.
        if ( CREATE_TABLES_OF_PROGRAM( P_USER_LOGIN_INFO, false ) EQ ADAK_FAIL ) {
            return ADAK_FAIL;
        }
        //! END

        sql_query.PREPARE_INSERT( "ynt_bilgisayar_kullanicilari","id","bilgisayar_id, kullanici_id");
        sql_query.SET_VALUE( ":bilgisayar_id", P_USER_LOGIN_INFO->bilgisayar_id );
        sql_query.SET_VALUE( ":kullanici_id" , P_USER_LOGIN_INFO->kullanici_id  );
        sql_query.INSERT();

        sql_query.PREPARE_INSERT( "ynt_veritabani_kullanicilari","id","veritabani_id, kullanici_id");
        sql_query.SET_VALUE( ":veritabani_id", P_USER_LOGIN_INFO->veritabani_id );
        sql_query.SET_VALUE( ":kullanici_id",  P_USER_LOGIN_INFO->kullanici_id  );
        sql_query.INSERT();
    }

    SET_STATIC_VALUES( P_USER_LOGIN_INFO );

    SET_KULLANICININ_CALISABILECEGI_VERITABANI_SAYISI ( 1 );
    SET_BILGISAYARDA_KAYITLI_KULLANICI_SAYISI         ( 1 );

    KULLANICI_TAM_YETKILENDIR( P_USER_LOGIN_INFO->kullanici_id );

    SHOW_YARDIM_HATIRLATMA_DIALOG();

    G_YONETIM_DB->COMMIT_TRANSACTION();

    //! [1] END

    return ADAK_OK;
}
