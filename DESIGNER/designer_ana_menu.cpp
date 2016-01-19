#include "designer.h"
#include "sql_designer.h"
#include "belge_designer.h"
#include "yonetim.h"
#include "sql_struct_generator.h"
#include "sql_veritabani_ayarlari_batch.h"
#include "sql_designer.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "QSplashScreen"

#include "designer_ana_menu.h"
#include "ui_designer_ana_menu.h"

extern ADAK_SQL_STRUCT    designer_db;
extern ADAK_SQL        *  DB;

enum {

    MENU_1                           = 10,
        MENU_SQL_DESIGNER            = 11,
        MENU_BELGE_DESIGNER          = 12,

    MENU_2                           = 30,
        MENU_HEADER_OLUSTUR          = 31,
        MENU_CIKIS                   = 32,

    MENU_SISTEM                      = 50,
        MENU_KULLANICILAR            = 51,
        MENU_BILGISAYARLAR           = 52,
        MENU_VERITABANLARI           = 53,
        MENU_VERITABANI_BILGILERI    = 54,
        MENU_007_YONETIM             = 55,

    MENU_YONETIM                     = 70,
        MENU_007_BILGILERI           = 72,
    MENU_YARDIM                      = 90,
        MENU_HAKKINDA                = 91,

    BOS_MENU_1                       = 110,
    BOS_MENU_2                       = 130
};

void STRUCTURDAN_VERITABANINI_YARAT ();

/**************************************************************************************
                   OPEN_DESIGNER_ANA_MENU
***************************************************************************************/

void OPEN_DESIGNER_ANA_MENU ()
{

//    Dikkat Bu islem cok tehlikelidi.
//    STRUCTURDAN_VERITABANINI_YARAT ();


    if ( KULLANICI_ID() NE 1 ) {

        SQL_QUERY sql_query ( DB );



        sql_query.PREPARE_SELECT ( "sql_kullanici_yetkileri","yetki_id","kullanici_id = :kullanici_id" );
        sql_query.SET_VALUE      ( ":kullanici_id", KULLANICI_ID() );

        if ( sql_query.SELECT() EQ 0 ) {
            ADAK_ERROR(QObject::tr ("No authorization for this user."),NULL,NULL);//Bu kullanıcı için yetkilendirme yapılmamış.
            exit(0);
        }
    }

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("sql_db_info","program_id");

    if ( query.SELECT() EQ 0 ) {
        OPEN_SQL_VERITABANI_AYARLARI_BATCH(NULL);
    }
    else {
        query.NEXT();
        if ( query.VALUE(0).toInt() EQ -1 ) {
            OPEN_SQL_VERITABANI_AYARLARI_BATCH(NULL);
        }
    }

    DESIGNER_ANA_MENU * ADAK_DESIGNER = new DESIGNER_ANA_MENU ( NULL );
    ADAK_DESIGNER->setWindowState ( Qt::WindowMaximized );
    ADAK_DESIGNER->show();
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::DESIGNER_ANA_MENU
***************************************************************************************/

DESIGNER_ANA_MENU::DESIGNER_ANA_MENU ( QWidget * p_parent ) : MENU_KERNEL ( p_parent ), ui ( new Ui::DESIGNER_ANA_MENU )
{
    ui->setupUi ( this );

    START_KERNEL();
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SETUP_FORM
***************************************************************************************/

void DESIGNER_ANA_MENU::SETUP_FORM()
{
    struct MENU_STRUCT MENULER[] = {

            { BOS_MENU_1,                "",0,"", "",0,"",0, MENU_SAYFADA },
            { MENU_1,                    "",0,"", "",0,"",0, MENU_SAYFADA },
            { MENU_SQL_DESIGNER,         tr ( "SQL DESIGNER" )  , MENU_1,":/designer_icons/designer_database.png", "",0,"",0 ,MENU_SAYFADA},
            { MENU_BELGE_DESIGNER,       tr ( "DOCUMENT DESIGNER" ), MENU_1,":/designer_icons/designer_reports.png", "",0,"",0 ,MENU_SAYFADA},
            { MENU_2,                    "",0,"", "",0,"",0, MENU_SAYFADA },
            { MENU_HEADER_OLUSTUR,       tr ("CREATE HEADER"),MENU_2,":/designer_icons/designer_header_olustur_icon.png", "",0,"",0 ,MENU_SAYFADA},
            { MENU_CIKIS,                tr ("EXIT"),         MENU_2,":/designer_icons/designer_exit_button.png", "",0,"",0 ,MENU_SAYFADA},
            { BOS_MENU_2,                "",0,"", "",0,"",0, MENU_SAYFADA },

            { MENU_YONETIM,              tr ("Administration"), 0, "" , "", 0, "", 0, MENU_TOPMENUDE }  ,
            { MENU_007_BILGILERI,        tr ( "007 Informations" )           ,     MENU_YONETIM,"", "" , 0, "", 0, MENU_TOPMENUDE},

            {MENU_SISTEM,                 tr ("System"), 0, "" , "", 0, "", 0, MENU_TOPMENUDE },
            { MENU_KULLANICILAR,          tr ( "Users" ),            MENU_SISTEM,"", "" , 0, "", 0, MENU_TOPMENUDE},
            { MENU_BILGISAYARLAR,         tr ( "Computers" ),           MENU_SISTEM,"", "" , 0, "", 0, MENU_TOPMENUDE},
            { MENU_VERITABANLARI,         tr ( "Databases/Companies" ), MENU_SISTEM,"", "" , 0, "", 0, MENU_TOPMENUDE},
            { MENU_VERITABANI_BILGILERI,  tr ( "Database Settings" )    , MENU_SISTEM,"", "" , 0, "", 0, MENU_TOPMENUDE},
            { MENU_007_YONETIM,           tr ( "007 Administration" ),             MENU_SISTEM,"", "" , 0, "", 0, MENU_TOPMENUDE},

            { MENU_YARDIM,                tr ("Help"), 0, "" , "", 0, "", 0, MENU_TOPMENUDE },
            {MENU_HAKKINDA,               tr ( "About" ),     MENU_YARDIM,"", "" , 0, "", 0, MENU_TOPMENUDE },


        { 0       , ""   , 0, "","",0,"",0,MENU_SAYFADA }
    };

    SET_KERNEL_STRUCT                       ( MENULER );
    REGISTER_MENU_WIDGET                    ( ui->main_widget );
    SET_ICON_SIZE                           ( 80, 80 );
    SET_BUTTON_TEXT_POS                     ( BUTTON_DISINDA_ALT_TARAFTA );
    HIDE_BUTTON_COLUMN_HEADER               ();

    SHOW_MENU_ITEM                          ( MENU_SQL_DESIGNER   );
    SHOW_MENU_ITEM                          ( MENU_BELGE_DESIGNER );
    SHOW_MENU_ITEM                          ( MENU_HEADER_OLUSTUR );
    SHOW_MENU_ITEM                          ( MENU_CIKIS          );
    SHOW_MENU_ITEM                          ( MENU_HAKKINDA       );
    SHOW_MENU_ITEM                          ( BOS_MENU_1          );
    SHOW_MENU_ITEM                          ( BOS_MENU_2          );

    KULLANICI_YETKILERINI_AYARLA            ();
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::DESIGNER_ANA_MENU
***************************************************************************************/

DESIGNER_ANA_MENU::~DESIGNER_ANA_MENU()
{
    delete ui;
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_SQL_DESIGNER
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_SQL_DESIGNER()
{
    OPEN_SQL_DESIGNER ( this );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_BELGE_DESIGNER
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_BELGE_DESIGNER ()
{
    OPEN_BELGE_DESIGNER ( -1, NULL );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_HAKKINDA
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_HAKKINDA ()
{
    OPEN_DESIGNER_HAKKINDA ( this );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_HEADER_OLUSTUR
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_HEADER_OLUSTUR()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT ( "sql_tablolar","tablo_id" );
    if ( sql_query.SELECT() EQ 0 ) {
        MSG_WARNING(tr ( "Table is not registered in the system, header filehas not been created."),NULL);//Kayıtlı tablo olmadığından herhangi bir header dosyası yaratılmadı.
        return;
    }
    if ( HEADER_DOSYASI_YARAT ( this ) EQ true ) {
        OPEN_HEADER_DOSYASI_OLUSTUR ( -1 );
        MSG_WARNING(tr  ( "Header file has been successfull created. " ) , NULL);//Header dosyası başarıyla yaratıldı
        return;
    }

    MSG_WARNING(tr ( "File has not been created." ) , NULL);//Dosya yaratılamadı.
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_KULLANICILAR
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_KULLANICILAR()
{
    OPEN_KULLANICILAR_FORMU ( &OPEN_DESIGNER_YETKILENDIRME_FORMU, this );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_BILGISAYARLAR
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_BILGISAYARLAR()
{
    OPEN_BILGISAYARLAR_FORMU( this );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_VERITABANLARI
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_VERITABANLARI()
{
    OPEN_VERITABANLARI_FORMU( this );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::SLOT_YONETIM_007
***************************************************************************************/

void DESIGNER_ANA_MENU::SLOT_YONETIM_007()
{
    OPEN_YONETIM_007_ARAMA ( this );
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::KULLANICI_YETKILERINI_AYARLA
***************************************************************************************/

void DESIGNER_ANA_MENU::KULLANICI_YETKILERINI_AYARLA ()
{
    if ( KULLANICI_ID() EQ 1) {
        SHOW_MENU_ITEM ( MENU_KULLANICILAR          );
        SHOW_MENU_ITEM ( MENU_VERITABANLARI         );
        SHOW_MENU_ITEM ( MENU_VERITABANI_BILGILERI  );
        SHOW_MENU_ITEM ( MENU_BILGISAYARLAR         );
        SHOW_MENU_ITEM ( MENU_007_YONETIM           );
        HIDE_MENU_ITEM ( MENU_SQL_DESIGNER          );
        HIDE_MENU_ITEM ( MENU_BELGE_DESIGNER        );
        HIDE_MENU_ITEM ( MENU_HEADER_OLUSTUR        );
        return;
    }

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT ( "sql_kullanici_yetkileri","kullanici_frm_girebilir_mi, "
                               "bilgisayar_frm_girebilir_mi, firma_frm_girebilir_mi, "
                               "ynt_007_gorebilir_mi, "
                               "vrtabani_guncelleyebilir_mi","kullanici_id = :kullanici_id" );

    sql_query.SET_VALUE ( ":kullanici_id", KULLANICI_ID() );
    if ( sql_query.SELECT() EQ 0 ) {

        MSG_WARNING(tr ( "No authorization for this user." ) , NULL );//Bu kullanıcı için yetkilendirme yapılmamış.
        return;
    }

    sql_query.NEXT();

    if ( sql_query.VALUE(0).toInt() EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_KULLANICILAR );
    }
    if ( sql_query.VALUE(1).toInt() EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_BILGISAYARLAR );
    }
    if ( sql_query.VALUE(2).toInt() EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_VERITABANLARI );
    }
    if ( sql_query.VALUE(3).toInt() EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_007_YONETIM );
    }
}

/**************************************************************************************
                   DESIGNER_ANA_MENU::MENU_ITEM_CLICKED
***************************************************************************************/

void DESIGNER_ANA_MENU::MENU_ITEM_CLICKED ( int p_enum_id )
{
    switch ( p_enum_id ) {

        case MENU_BILGISAYARLAR :
            SLOT_BILGISAYARLAR();
            break;
        case MENU_KULLANICILAR :
            SLOT_KULLANICILAR();
            break;
        case MENU_VERITABANLARI :
            SLOT_VERITABANLARI();
            break;
        case MENU_007_YONETIM :
            SLOT_YONETIM_007();
            break;
        case MENU_SQL_DESIGNER :
            SLOT_SQL_DESIGNER();
            break;
        case MENU_BELGE_DESIGNER :
            SLOT_BELGE_DESIGNER();
            break;
        case MENU_HEADER_OLUSTUR :
            SLOT_HEADER_OLUSTUR();
            break;
        case MENU_HAKKINDA :
            SLOT_HAKKINDA();
            break;
        case MENU_VERITABANI_BILGILERI:
            OPEN_SQL_VERITABANI_AYARLARI_BATCH(this);
            break;
        case MENU_CIKIS :
            close ();
        default :
            break;

    }
}


/**************************************************************************************
                   PRIMARY_KEY_EKLE
***************************************************************************************/

void PRIMARY_KEY_EKLE ( int row_id , int tablo_id )
{
    int indexler_array [INDEXLER_ARRAY_SIZE] = {0};
    indexler_array[0]                        = row_id;

    QString array_content = PACK_INT_ARRAY(indexler_array,INDEXLER_ARRAY_SIZE);

    SQL_QUERY query (DB);

    query.PREPARE_UPDATE("sql_tablolar","tablo_id","indexler_array","tablo_id=:tablo_id");

    query.SET_VALUE     (":indexler_array",array_content);
    query.SET_VALUE     (":tablo_id"      ,tablo_id);

    query.UPDATE();
}

/**************************************************************************************
                   STRUCTURDAN_VERITABANINI_YARAT
***************************************************************************************/

void STRUCTURDAN_VERITABANINI_YARAT ()
{

    DB->START_TRANSACTION();

    ADAK_SQL_STRUCT * struct_ptr = NULL; // &e9_db;

    SQL_QUERY sql_delete ( DB );

    sql_delete.PREPARE_DELETE("sql_tablolar","");
    sql_delete.DELETE();

    sql_delete.PREPARE_DELETE("sql_alanlar","");
    sql_delete.DELETE();

    for ( int i = 0 ; i < struct_ptr->table_count ; i++ ) {
        SQL_QUERY sql_onar ( DB );

        int tablo_id = -1;

        sql_onar.PREPARE_SELECT("sql_tablolar","tablo_id","tablo_adi=:tablo_adi" );
        sql_onar.SET_VALUE     (":tablo_adi" , struct_ptr->tables[i].name        );

        if ( sql_onar.SELECT() EQ 0 ) {
            sql_onar.PREPARE_INSERT("sql_tablolar","tablo_id","tablo_adi");
            sql_onar.SET_VALUE(":tablo_adi",struct_ptr->tables[i].name);
            tablo_id = sql_onar.INSERT();
        }
        else {
            sql_onar.NEXT();
            tablo_id = sql_onar.VALUE(0).toInt();
        }

        int order_num = 0;

        for ( int j = 0 ; j < struct_ptr->tables[i].column_count ; j++ ) {

            sql_onar.PREPARE_SELECT("sql_alanlar","alan_id","tablo_id=:tablo_id AND alan_adi=:alan_adi" );
            sql_onar.SET_VALUE     (":tablo_id"  , tablo_id                                );
            sql_onar.SET_VALUE     (":alan_adi"  , struct_ptr->tables[i].columns[j].name   );

            if ( sql_onar.SELECT() NE 0 ) {
                if ( j EQ 0 ) {
                    PRIMARY_KEY_EKLE(sql_onar.VALUE(0).toInt(),tablo_id);
                }
                continue;
            }

            sql_onar.PREPARE_INSERT("sql_alanlar","alan_id","tablo_id,alan_order_num,alan_adi,veritabanindaki_adi,alan_tipi,"
                                    "uzunluk,auto_increment_mi,not_null_mi,dizi_mi,dizi_boyutu,aciklama");


            sql_onar.SET_VALUE(":tablo_id", tablo_id );
            sql_onar.SET_VALUE(":alan_order_num",order_num);
            sql_onar.SET_VALUE(":alan_adi", struct_ptr->tables[i].columns[j].name );
            sql_onar.SET_VALUE(":veritabanindaki_adi", struct_ptr->tables[i].columns[j].name);

            if ( struct_ptr->tables[i].columns[j].type EQ INTEGER ) {
                sql_onar.SET_VALUE(":alan_tipi","INTEGER");
            }
            else if ( struct_ptr->tables[i].columns[j].type EQ TEXT ) {
                sql_onar.SET_VALUE(":alan_tipi","TEXT");
            }
            else if ( struct_ptr->tables[i].columns[j].type EQ REAL ) {
                sql_onar.SET_VALUE(":alan_tipi","REAL");
            }
            else if ( struct_ptr->tables[i].columns[j].type EQ BLOB ) {
                sql_onar.SET_VALUE(":alan_tipi","BLOB");
            }

            sql_onar.SET_VALUE(":uzunluk", struct_ptr->tables[i].columns[j].length);

            if ( struct_ptr->tables[i].columns[j].is_auto_increment EQ false ) {
                sql_onar.SET_VALUE(":auto_increment_mi", 0);
            }
            else {
                sql_onar.SET_VALUE(":auto_increment_mi", 1);
            }

            if ( struct_ptr->tables[i].columns[j].is_not_null EQ false ) {
                sql_onar.SET_VALUE(":not_null_mi", 0);
            }
            else {
                sql_onar.SET_VALUE(":not_null_mi", 1);
            }

            if ( struct_ptr->tables[i].columns[j].is_array EQ false ) {
                sql_onar.SET_VALUE(":dizi_mi", 0);
            }
            else {
                sql_onar.SET_VALUE(":dizi_mi", 1);
            }

            sql_onar.SET_VALUE(":dizi_boyutu", struct_ptr->tables[i].columns[j].array_length );
            sql_onar.SET_VALUE(":aciklama", "");

            int row_id = sql_onar.INSERT();

            if ( j EQ 0 ) {
                PRIMARY_KEY_EKLE(row_id,tablo_id);
            }

            order_num += 128;
        }
    }

    DB->COMMIT_TRANSACTION();

    exit(99);
}

/**************************************************************************************
                        BELGE_VERITABANINI_ONAR
***************************************************************************************/
/*
void BELGE_VERITABANINI_ONAR()
{

    ADAK_SQL                * DESIGNER_DB = NULL;

    QString db_name = SQL_DBNAME_ONEK() + "adak_e9";

    DESIGNER_DB = CREATE_ADAK_SQL_CONNECTION( &e9_db , db_name , GET_SQL_DB_DRIVER_ENUM(SQL_MOTORU()));

    if ( DESIGNER_DB->CONNECT_TO_SERVER(true) EQ ADAK_FAIL ) {

        DB->CANCEL_TRANSACTION();
        MSG_WARNING(  tr("Veritabi Tespit edilemedi"), NULL );
        return;
    }

    DESIGNER_DB->CONNECT_TO_DATABASE(db_name);


    DESIGNER_DB->START_TRANSACTION();

    SQL_QUERY query( DESIGNER_DB );

    query.PREPARE_DELETE("blg_belgeler", "");
    query.DELETE();

    query.PREPARE_DELETE("blg_belge_degiskenleri", "");
    query.DELETE();

    //Tahsilat makbuzu header onar
    for (int i = 0; i < ADAK_BELGE_COUNT; ++i) {

    //belge insert

    query.PREPARE_INSERT( "blg_belgeler","belge_id", "belge_id, belge_adi" );
    query.SET_VALUE     (":belge_id", e9_belge_struct[i].belge_id );
    query.SET_VALUE     (":belge_adi", e9_belge_struct[i].belge_adi );
    query.INSERT();


        // header
        int order_header_number = 0;
        for (int j = 0; j < e9_belge_struct[i].header_struct_size; ++j) {

            query.PREPARE_INSERT( "blg_belge_degiskenleri","degisken_id", "degisken_id, belge_id, order_num, grubu, adi, kodu, font_size, align" );

            query.SET_VALUE     (":degisken_id" ,  e9_belge_struct[i].header_struct[j].degisken_id      );
            query.SET_VALUE     (":belge_id"    ,  e9_belge_struct[i].belge_id                          );
            query.SET_VALUE     (":order_num"   ,  order_header_number                                  );
            query.SET_VALUE     (":grubu"       ,  HEADER                                               );
            query.SET_VALUE     (":adi"         ,  e9_belge_struct[i].header_struct[j].degisken_adi     );
            query.SET_VALUE     (":kodu"        ,  e9_belge_struct[i].header_struct[j].degisken_kodu    );
            query.SET_VALUE     (":font_size"   ,  e9_belge_struct[i].header_struct[j].font_size        );
            query.SET_VALUE     (":align"       ,  e9_belge_struct[i].header_struct[j].align            );
            query.INSERT();

            order_header_number += 1024;

        }


        // line

        int order_line_number = 0;
        for (int j = 0; j < e9_belge_struct[i].line_struct_size; ++j) {
            query.PREPARE_INSERT( "blg_belge_degiskenleri","degisken_id", "degisken_id, belge_id, order_num, grubu, adi, kodu, font_size, align" );

            query.SET_VALUE     (":degisken_id" ,  e9_belge_struct[i].line_struct[j].degisken_id    );
            query.SET_VALUE     (":belge_id"    ,  e9_belge_struct[i].belge_id                      );
            query.SET_VALUE     (":order_num"   ,  order_line_number                                );
            query.SET_VALUE     (":grubu"       ,  LINE                                             );
            query.SET_VALUE     (":adi"         ,  e9_belge_struct[i].line_struct[j].degisken_adi   );
            query.SET_VALUE     (":kodu"        ,  e9_belge_struct[i].line_struct[j].degisken_kodu  );
            query.SET_VALUE     (":font_size"   ,  e9_belge_struct[i].line_struct[j].font_size      );
            query.SET_VALUE     (":align"       ,  e9_belge_struct[i].line_struct[j].align          );
            query.INSERT();

            order_line_number += 1024;
        }

        // footer
        int order_footer_number = 0;
        for (int j = 0; j < e9_belge_struct[i].footer_struct_size; ++j) {

            query.PREPARE_INSERT( "blg_belge_degiskenleri","degisken_id", "degisken_id, belge_id, order_num, grubu, adi, kodu, font_size, align" );

            query.SET_VALUE     (":degisken_id" ,  e9_belge_struct[i].footer_struct[j].degisken_id      );
            query.SET_VALUE     (":belge_id"    ,  e9_belge_struct[i].belge_id                          );
            query.SET_VALUE     (":order_num"   ,  order_footer_number                                  );
            query.SET_VALUE     (":grubu"       ,  FOOTER                                               );
            query.SET_VALUE     (":adi"         ,  e9_belge_struct[i].footer_struct[j].degisken_adi     );
            query.SET_VALUE     (":kodu"        ,  e9_belge_struct[i].footer_struct[j].degisken_kodu    );
            query.SET_VALUE     (":font_size"   ,  e9_belge_struct[i].footer_struct[j].font_size        );
            query.SET_VALUE     (":align"       ,  e9_belge_struct[i].footer_struct[j].align            );
            query.INSERT();

            order_footer_number += 1024;
        }

    }

    DESIGNER_DB->COMMIT_TRANSACTION();

    MSG_WARNING( tr("Designer Onarildi"), NULL );
}

*/
