#include "adak_sql.h"
#include "designer_yetkilendirme_formu.h"
#include "ui_designer_yetkilendirme_formu.h"
#include "yonetim.h"

extern ADAK_SQL * G_YONETIM_DB;
extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_DESIGNER_YETKILENDIRME_FORMU
***************************************************************************************/

void OPEN_DESIGNER_YETKILENDIRME_FORMU ( int p_yetki_id, bool * yetkileri_tazele, QWidget * parent )
{
    DESIGNER_YETKILENDIRME_FORMU * F = new DESIGNER_YETKILENDIRME_FORMU ( p_yetki_id, yetkileri_tazele, parent );
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::DESIGNER_YETKILENDIRME_FORMU
***************************************************************************************/

DESIGNER_YETKILENDIRME_FORMU::DESIGNER_YETKILENDIRME_FORMU ( int p_kullanici_id, bool * yetkileri_tazele, QWidget * parent ) : FORM_KERNEL ( parent ),
m_ui ( new Ui::DESIGNER_YETKILENDIRME_FORMU )
{
    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "sql_kullanici_yetkileri", "yetki_id", " kullanici_id = :kullanici_id" );
    query.SET_VALUE     ( ":kullanici_id", p_kullanici_id );

    if ( query.SELECT() EQ 0 ) {
        //  EGER KULLANICI YOK ISE OLUSTURULUYOR
        SQL_QUERY s_query( DB );
        DB->START_TRANSACTION();
        s_query.PREPARE_INSERT( "sql_kullanici_yetkileri", "yetki_id", "kullanici_id" );
        s_query.SET_VALUE     ( ":kullanici_id", p_kullanici_id );
        s_query.INSERT();
        DB->COMMIT_TRANSACTION();
    }

    query.PREPARE_SELECT( "sql_kullanici_yetkileri", "yetki_id", " kullanici_id = :kullanici_id" );
    query.SET_VALUE     ( ":kullanici_id", p_kullanici_id );
    query.SELECT();
    query.NEXT();

    m_yetkileri_tazele = yetkileri_tazele;
    m_start_record_id  = query.VALUE( 0 ).toInt();

    m_ui->setupUi           ( this );
    START_FORM_KERNEL       ( this, DB );
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::SETUP_FORM
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::SETUP_FORM()
{
    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );
    SET_SINGLE_RECORD_ID    ( m_start_record_id );
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::NEW_RECORD
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::NEW_RECORD()
{
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::GET_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::GET_RECORD ( int p_yetki_id )
{
    Q_UNUSED ( p_yetki_id );

    SQL_QUERY sql_query     ( DB );
    SQL_QUERY yonetim_query ( G_YONETIM_DB );


    sql_query.PREPARE_SELECT ( "sql_kullanici_yetkileri","yetkilendirme_yapabilir_mi, bilgisayar_frm_girebilir_mi, "
                                "kullanici_frm_girebilir_mi, firma_frm_girebilir_mi, "
                                "ynt_007_gorebilir_mi, vrtabani_guncelleyebilir_mi, "
                                "adak_std_guncelleyebilir_mi, "
                                "kayit_ekleyebilir_mi, kayit_guncelleyebilir_mi, kayit_silebilir_mi, kullanici_id ",
                                "yetki_id = :yetki_id" );

    sql_query.SET_VALUE      (  ":yetki_id" , p_yetki_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    yonetim_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_kodu, kullanici_adi","kullanici_id = :kullanici_id AND silinmis_mi = 0");

    yonetim_query.SET_VALUE      (  ":kullanici_id", sql_query.VALUE("kullanici_id").toInt() );

    if ( yonetim_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    yonetim_query.NEXT();

    m_ui->label_kullanici_kodu->setText ( yonetim_query.VALUE(0).toString());
    m_ui->label_kullanici_adi->setText  ( yonetim_query.VALUE(1).toString());

    m_ui->comboBox_yetkilendirme_yapabilir_mi->setCurrentIndex ( sql_query.VALUE(0).toInt() );
    m_ui->comboBox_bilgisayar_formuna_girebilir_mi->setCurrentIndex ( sql_query.VALUE(1).toInt() );
    m_ui->comboBox_kullanicilar_formuna_girebilir_mi->setCurrentIndex ( sql_query.VALUE(2).toInt() );
    m_ui->comboBox_firmalar_formuna_girebilir_mi->setCurrentIndex ( sql_query.VALUE(3).toInt() );
    m_ui->comboBox_yonetim_007_bilgilerini_gorebilir_mi->setCurrentIndex ( sql_query.VALUE(4).toInt() );
    m_ui->comboBox_veritabanini_guncelleyebilir_mi->setCurrentIndex ( sql_query.VALUE(5).toInt() );
    m_ui->comboBox_adak_std_veritabanini_guncelleyebilir_mi->setCurrentIndex ( sql_query.VALUE(6).toInt() );
    m_ui->comboBox_kayit_ekleyebilir_mi->setCurrentIndex ( sql_query.VALUE(7).toInt() );
    m_ui->comboBox_kayit_guncelleyebilir_mi->setCurrentIndex ( sql_query.VALUE(8).toInt() );
    m_ui->comboBox_kayit_silebilir_mi->setCurrentIndex ( sql_query.VALUE(9).toInt() );

    return ADAK_OK;

}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::SELECT_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CHECK_EMPTY
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::CHECK_EMPTY ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CHECK_VAR
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::CHECK_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}


/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CHECK_ADD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::ADD_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::ADD_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CHECK_UPDATE
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::CHECK_UPDATE ( int p_yetki_id )
{
    Q_UNUSED ( p_yetki_id );
    return ADAK_OK;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::UPDATE_RECORD
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::UPDATE_RECORD ( int p_yetki_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE ( "sql_kullanici_yetkileri","yetki_id","yetkilendirme_yapabilir_mi,bilgisayar_frm_girebilir_mi,"
                               "kullanici_frm_girebilir_mi,firma_frm_girebilir_mi,ynt_007_gorebilir_mi, "
                               "vrtabani_guncelleyebilir_mi,adak_std_guncelleyebilir_mi,kayit_ekleyebilir_mi, "
                               "kayit_guncelleyebilir_mi,kayit_silebilir_mi","yetki_id = :yetki_id" );

    sql_query.SET_VALUE ( ":yetkilendirme_yapabilir_mi"                 , m_ui->comboBox_yetkilendirme_yapabilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":bilgisayar_frm_girebilir_mi"         , m_ui->comboBox_bilgisayar_formuna_girebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":kullanici_frm_girebilir_mi"          , m_ui->comboBox_kullanicilar_formuna_girebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":firma_frm_girebilir_mi"              , m_ui->comboBox_firmalar_formuna_girebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":ynt_007_gorebilir_mi"       , m_ui->comboBox_yonetim_007_bilgilerini_gorebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":vrtabani_guncelleyebilir_mi"            , m_ui->comboBox_veritabanini_guncelleyebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":adak_std_guncelleyebilir_mi"  , m_ui->comboBox_adak_std_veritabanini_guncelleyebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":kayit_ekleyebilir_mi"                       , m_ui->comboBox_kayit_ekleyebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":kayit_guncelleyebilir_mi"                   , m_ui->comboBox_kayit_guncelleyebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":kayit_silebilir_mi"                         , m_ui->comboBox_kayit_silebilir_mi->currentIndex());
    sql_query.SET_VALUE ( ":yetki_id"                                   , p_yetki_id );
    sql_query.UPDATE();
    
    int kullanici_id = 0;

    sql_query.PREPARE_SELECT ( "sql_kullanici_yetkileri", "kullanici_id","yetki_id = :yetki_id");

    sql_query.SET_VALUE(":yetki_id", p_yetki_id );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        kullanici_id = sql_query.VALUE(0).toInt();
    }

    if ( kullanici_id EQ KULLANICI_ID() ) {
        *m_yetkileri_tazele = true;
    }
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CHECK_DELETE
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::CHECK_DELETE ( int p_yetki_id )
{
    Q_UNUSED ( p_yetki_id );
    return ADAK_OK;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::DELETE_RECORD
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::DELETE_RECORD ( int p_yetki_id )
{
    Q_UNUSED ( p_yetki_id );
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::FIND_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::FIND_FIRST_RECORD()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::LOCK_RECORD
***************************************************************************************/

int DESIGNER_YETKILENDIRME_FORMU::LOCK_RECORD ( int p_yetki_id )
{
    return DB->LOCK_ROW ( "yetki_id", "sql_kullanici_yetkileri", QString (  "yetki_id = %1" ).arg ( p_yetki_id ));
}

/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::UNLOCK_RECORD
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::UNLOCK_RECORD ( int p_yetki_id  )
{
    DB->UNLOCK_ROW ( "yetki_id", "sql_kullanici_yetkileri", QString( "yetki_id = %1" ).arg ( p_yetki_id ) );
}
/**************************************************************************************
                   DESIGNER_YETKILENDIRME_FORMU::CHANGE_ALL_COMBOBOX_STATUS
***************************************************************************************/

void DESIGNER_YETKILENDIRME_FORMU::CHANGE_ALL_COMBOBOX_STATUS ( int status )
{
    m_ui->comboBox_yetkilendirme_yapabilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_bilgisayar_formuna_girebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_kullanicilar_formuna_girebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_firmalar_formuna_girebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_yonetim_007_bilgilerini_gorebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_veritabanini_guncelleyebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_adak_std_veritabanini_guncelleyebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_kayit_ekleyebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_kayit_guncelleyebilir_mi->setCurrentIndex ( status );
    m_ui->comboBox_kayit_silebilir_mi->setCurrentIndex ( status );
}
