#include <QHeaderView>
#include <QCryptographicHash>
#include "yonetim.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "kullanicilar_formu.h"
#include "ui_kullanicilar_formu.h"
#include "kullanici_secimi.h"
#include "bilgisayar_secimi.h"
#include "yonetim_utils.h"
#include "login.h"



#define     BILGISAYAR_ID_COLUMN            0
#define     BILGISAYAR_KODU_COLUMN          1
#define     BILGISAYAR_ADI_COLUMN           2
#define     DONGLE_IP_COLUMN                3
#define     MAC_ADRESI_KABLOLU_COLUMN       4
#define     MAC_ADRESI_KABLOSUZ_COLUMN      5
#define     MAC_ADRESI_3G_COLUMN            6

#define     VERITABANI_ID_COLUMN            0
#define     VERITABANI_COLUMN               1
#define     VERITABANI_TANIMI_COLUMN        2

extern ADAK_SQL *  G_YONETIM_DB;
extern ADAK_SQL *  DB;


/**************************************************************************************
                   OPEN_KULLANICILAR_FORMU
***************************************************************************************/

bool OPEN_KULLANICILAR_FORMU ( void    (*F_PROGRAM_KULLANICI_YETKILERI_FONK) ( int,bool *, QWidget * ), QWidget *parent )
{
    bool yetkileri_tazele = false;
    KULLANICILAR_FORMU * F = new KULLANICILAR_FORMU ( F_PROGRAM_KULLANICI_YETKILERI_FONK, &yetkileri_tazele, parent );
    F->EXEC ( FULL_SCREEN );
    return yetkileri_tazele;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::KULLANICILAR_FORMU
**************************************************************************************/

KULLANICILAR_FORMU::KULLANICILAR_FORMU ( void    (*F_PROGRAM_KULLANICI_YETKILERI_FONK) ( int,bool *, QWidget * ),bool * yetkileri_tazele, QWidget * parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui::KULLANICILAR_FORMU )
{
    m_ui->setupUi( this );

    m_yetkileri_tazele = yetkileri_tazele;

    M_KULLANICI_YETKILERI_FONK = F_PROGRAM_KULLANICI_YETKILERI_FONK;

    START_FORM_KERNEL ( this, G_YONETIM_DB );
}

/**************************************************************************************
                 KULLANICILAR_FORMU::SETUP_FORM
***************************************************************************************/

void KULLANICILAR_FORMU::SETUP_FORM ()
{

    m_ui->table_widget_bilgisayarlar->hideColumn     ( 0 );
    m_ui->table_widget_veritabanlari->hideColumn     ( 0 );

    QFont bilgisayar_tableWidget_font = m_ui->table_widget_bilgisayarlar->font();
    int   point_size = bilgisayar_tableWidget_font.pointSize();
    point_size -= 1;
    bilgisayar_tableWidget_font.setPointSize  ( point_size );
    m_ui->table_widget_bilgisayarlar->setFont ( bilgisayar_tableWidget_font );


    m_ui->table_widget_bilgisayarlar->setColumnWidth ( BILGISAYAR_KODU_COLUMN,      110 );
    m_ui->table_widget_bilgisayarlar->setColumnWidth ( BILGISAYAR_ADI_COLUMN,       180 );
    m_ui->table_widget_bilgisayarlar->setColumnWidth ( DONGLE_IP_COLUMN,            100 );
    m_ui->table_widget_bilgisayarlar->setColumnWidth ( MAC_ADRESI_KABLOLU_COLUMN,   135 );
    m_ui->table_widget_bilgisayarlar->setColumnWidth ( MAC_ADRESI_KABLOSUZ_COLUMN,  140 );
    m_ui->table_widget_bilgisayarlar->setColumnWidth ( MAC_ADRESI_3G_COLUMN,        135 );


    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );

    REGISTER_CHANGER_BUTTON ( m_ui->button_bilgisayar_sec );
    REGISTER_CHANGER_BUTTON ( m_ui->button_veritabani_sec );

    REGISTER_SAVER_BUTTON   ( m_ui->pushButton_yetki_ata );

    SET_PAGE_TITLE    ( tr ( "USERS" ) );
    SET_SETTING_NAME  ( "KULLANICILAR_FORMU" );
    SET_HELP_PAGE     ( "yonetim-islemleri_kullanicilar" );

    m_ui->lineEdit_email->setMaxLength(50);
    m_ui->line_edit_email_kullanici_adi->setMaxLength(50);
    m_ui->lineEdit_kullanici_adi->setMaxLength(50);
    m_ui->lineEdit_kullanici_kodu->setMaxLength(10);
    m_ui->line_edit_yeni_sifre->setMaxLength(50);
    m_ui->line_edit_yeni_sifre_tekrar->setMaxLength(50);
    m_ui->limitedTextEdit_not->SET_LINE_LIMIT(5);
    m_ui->limitedTextEdit_not->SET_MAX_STRING_LENGTH(99);

    SET_FIRST_FOCUS_WIDGET( m_ui->lineEdit_kullanici_kodu );
}

/**************************************************************************************
                   KULLANICILAR_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void KULLANICILAR_FORMU::CLEAR_FORM_MEMBERS()
{
    QByteArray hash      =  QCryptographicHash::hash ( QString("").toUtf8(), QCryptographicHash::Md5 );
    m_kullanici_parolasi =  QString( hash.toHex());
}

/**************************************************************************************
                   KULLANICILAR_FORMU::NEW_RECORD
***************************************************************************************/

void KULLANICILAR_FORMU::NEW_RECORD()
{
    m_ui->lineEdit_kullanici_kodu->clear();
    m_ui->lineEdit_kullanici_adi->clear();
    m_ui->line_edit_email_kullanici_adi->clear();
    m_ui->line_edit_yeni_sifre->clear();
    m_ui->line_edit_yeni_sifre_tekrar->clear();
    m_ui->lineEdit_email->clear();
    m_ui->limitedTextEdit_not->clear();
    m_ui->table_widget_bilgisayarlar->clearContents();
    m_ui->table_widget_bilgisayarlar->setRowCount(0);
    m_ui->table_widget_veritabanlari->clearContents();
    m_ui->table_widget_veritabanlari->setRowCount(0);

    m_ui->button_bilgisayar_sec->setEnabled(true);
    m_ui->button_veritabani_sec->setEnabled(true);
    m_ui->frame->setEnabled(true);
    m_ui->frame_2->setEnabled(true);
    m_ui->lineEdit_kullanici_kodu->setEnabled(true);
    m_ui->lineEdit_kullanici_adi->setEnabled (true);

}

/**************************************************************************************
                   KULLANICILAR_FORMU::GET_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::GET_RECORD ( int p_kullanici_id )
{
    m_ui->table_widget_bilgisayarlar->clearContents();
    m_ui->table_widget_bilgisayarlar->setRowCount(0);
    m_ui->table_widget_veritabanlari->clearContents();
    m_ui->table_widget_veritabanlari->setRowCount(0);

    SQL_QUERY sql_query             ( G_YONETIM_DB );
    SQL_QUERY bilgisayar_query      ( G_YONETIM_DB );
    SQL_QUERY veritabani_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_kodu, kullanici_adi, parola, email_adresi, aciklama, "
                               "email_kullanici_adi","kullanici_id = :kullanici_id" ,"");
    sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    sql_query.NEXT();
    m_ui->lineEdit_kullanici_kodu->setText ( sql_query.VALUE(0).toString() );

    m_ui->frame_sifre->setDisabled(true);
    m_ui->check_box_sifre->setEnabled(true);
    m_ui->check_box_sifre->setChecked(false);

    if ( p_kullanici_id EQ 1 ) {
        m_ui->button_bilgisayar_sec->setDisabled(true);
        m_ui->button_veritabani_sec->setDisabled(true);
        m_ui->frame->setDisabled(true);
        m_ui->frame_2->setDisabled(true);
        m_ui->lineEdit_kullanici_kodu->setDisabled(true);
        m_ui->lineEdit_kullanici_adi->setDisabled (true);
    }
    else {
        m_ui->button_bilgisayar_sec->setEnabled(true);
        m_ui->button_veritabani_sec->setEnabled(true);
        m_ui->frame->setEnabled(true);
        m_ui->frame_2->setEnabled(true);
        m_ui->lineEdit_kullanici_kodu->setEnabled(true);
        m_ui->lineEdit_kullanici_adi->setEnabled (true);
    }

    m_ui->lineEdit_kullanici_adi->setText  ( sql_query.VALUE(1).toString() );
    m_ui->lineEdit_email->setText          ( sql_query.VALUE(3).toString() );
    m_ui->limitedTextEdit_not->setText     ( sql_query.VALUE(4).toString() );
    m_ui->line_edit_email_kullanici_adi->setText  ( sql_query.VALUE(5).toString() );
    m_ui->line_edit_yeni_sifre->clear();
    m_ui->line_edit_yeni_sifre_tekrar->clear();

    m_kullanici_parolasi = sql_query.VALUE(2).toString();

    QByteArray hash      = QCryptographicHash::hash ( QString("").toUtf8(), QCryptographicHash::Md5);

    if ( m_kullanici_parolasi NE hash.toHex() ) {
        m_ui->line_edit_yeni_sifre->setText( "*****");
        m_ui->line_edit_yeni_sifre_tekrar->setText( "*****");
    }

    int current_table_row = 0;

    sql_query.PREPARE_SELECT ( "ynt_bilgisayar_kullanicilari","bilgisayar_id","kullanici_id = :kullanici_id","");
    sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

    if ( sql_query.SELECT() > 0 ) {

        while ( sql_query.NEXT() EQ true ) {

            int bilgisayar_id = sql_query.VALUE(0).toInt();
            bilgisayar_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_kodu, bilgisayar_adi, dongle_ip, "
                                              "mac_adresi_kablolu, mac_adresi_kablosuz, mac_adresi_3g","bilgisayar_id = :bilgisayar_id AND silinmis_mi = 0","");
            bilgisayar_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id );
            if (bilgisayar_query.SELECT() EQ 0 ) {
                continue;
            }
            bilgisayar_query.NEXT();

            QString mac_kablolu;
            QString mac_kablosuz;
            QString mac_3g;

            if ( bilgisayar_query.VALUE(3).toString().replace ( ":", "" ).isEmpty() EQ false ) {
                mac_kablolu = bilgisayar_query.VALUE(3).toString();
            }
            if ( bilgisayar_query.VALUE(4).toString().replace ( ":", "" ).isEmpty() EQ false ) {
                mac_kablosuz = bilgisayar_query.VALUE(4).toString();
            }
            if ( bilgisayar_query.VALUE(5).toString().replace ( ":", "" ).isEmpty() EQ false ) {
                mac_3g = bilgisayar_query.VALUE(5).toString();
            }

            m_ui->table_widget_bilgisayarlar->insertRow ( current_table_row );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, BILGISAYAR_ID_COLUMN       , new QTableWidgetItem ( QVariant( bilgisayar_id ).toString() ) );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, BILGISAYAR_KODU_COLUMN     , new QTableWidgetItem ( bilgisayar_query.VALUE(0).toString() ) );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, BILGISAYAR_ADI_COLUMN      , new QTableWidgetItem ( bilgisayar_query.VALUE(1).toString() ) );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, DONGLE_IP_COLUMN           , new QTableWidgetItem ( bilgisayar_query.VALUE(2).toString() ) );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, MAC_ADRESI_KABLOLU_COLUMN  , new QTableWidgetItem ( mac_kablolu  ) );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, MAC_ADRESI_KABLOSUZ_COLUMN , new QTableWidgetItem ( mac_kablosuz ) );
            m_ui->table_widget_bilgisayarlar->setItem   ( current_table_row, MAC_ADRESI_3G_COLUMN       , new QTableWidgetItem ( mac_3g       ) );

            current_table_row++;

        }
    }

    sql_query.PREPARE_SELECT ( "ynt_veritabani_kullanicilari","veritabani_id","kullanici_id = :kullanici_id" ,"");
    sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

    if ( sql_query.SELECT() > 0 ) {

        current_table_row = 0;

        while ( sql_query.NEXT() EQ true ) {
            int veritabani_id = sql_query.VALUE(0).toInt();
            veritabani_query.PREPARE_SELECT( "ynt_veritabanlari","veritabani_ismi, veritabani_tanimi","veritabani_id = :veritabani_id","" ) ;
            veritabani_query.SET_VALUE     ( ":veritabani_id", veritabani_id );

            if ( veritabani_query.SELECT() EQ 0 ) {
                continue;
            }
            veritabani_query.NEXT();
            m_ui->table_widget_veritabanlari->insertRow ( current_table_row );
            m_ui->table_widget_veritabanlari->setItem   ( current_table_row, VERITABANI_ID_COLUMN      , new QTableWidgetItem( QVariant( veritabani_id ).toString() ) );
            m_ui->table_widget_veritabanlari->setItem   ( current_table_row, VERITABANI_COLUMN         , new QTableWidgetItem( veritabani_query.VALUE(0).toString() ) );
            m_ui->table_widget_veritabanlari->setItem   ( current_table_row, VERITABANI_TANIMI_COLUMN  , new QTableWidgetItem( veritabani_query.VALUE(1).toString() ) );
            current_table_row++;
        }
    }

     return ADAK_OK;
}



/**************************************************************************************
                   KULLANICILAR_FORMU::CHECK_VAR
***************************************************************************************/

int KULLANICILAR_FORMU::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->table_widget_bilgisayarlar ) {
        if ( p_object->objectName() EQ "ITEM_DOUBLE_CLICKED" ) {
            int clicked_row    = m_ui->table_widget_bilgisayarlar->currentRow();
            int clicked_column = m_ui->table_widget_bilgisayarlar->currentColumn();

            return BILGISAYAR_LISTESINDEN_SATIR_SIL ( clicked_row, clicked_column );
        }
    }
    else if ( p_object EQ m_ui->table_widget_veritabanlari ) {
        if ( p_object->objectName() EQ "ITEM_DOUBLE_CLICKED" ) {
            int clicked_row    = m_ui->table_widget_veritabanlari->currentRow();
            int clicked_column = m_ui->table_widget_veritabanlari->currentColumn();

            return VERITABANI_LISTESINDEN_SATIR_SIL ( clicked_row, clicked_column );
        }
    }
    else if ( p_object EQ m_ui->check_box_sifre ) {
        if ( m_ui->check_box_sifre->isChecked() EQ true ) {
            m_ui->frame_sifre->setEnabled(true);

            m_ui->line_edit_yeni_sifre->clear();
            m_ui->line_edit_yeni_sifre_tekrar->clear();
        }
        else {
            m_ui->frame_sifre->setDisabled(true);
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::CHECK_EMPTY
***************************************************************************************/

int KULLANICILAR_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_kullanici_kodu->text().isEmpty() EQ true ) {
        MSG_ERROR(tr ( "User code can not be empty." ) , m_ui->lineEdit_kullanici_kodu );//Kullanıcı kodu boş bırakılamaz
        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_kullanici_adi->text().isEmpty() EQ true ) {
        MSG_ERROR(tr ( "User name can not be empty." ) , m_ui->lineEdit_kullanici_adi);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::CHECK_ADD
***************************************************************************************/

int KULLANICILAR_FORMU::CHECK_ADD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_kodu = :kullanici_kodu "
                               "AND silinmis_mi = :silinmis_mi","" );
    sql_query.SET_VALUE      ( ":kullanici_kodu",  m_ui->lineEdit_kullanici_kodu->text() );
    sql_query.SET_VALUE      ( ":silinmis_mi " , 0 );

    if ( sql_query.SELECT() NE 0 ) {
        MSG_ERROR(tr ( "There are registered users in the system with this code." )  , NULL);//Bu kodla kayıtlı kullanıcı sistemde mevcut
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::ADD_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::ADD_RECORD ()
{
    SQL_QUERY sql_query     ( G_YONETIM_DB );
    sql_query.PREPARE_INSERT ( "ynt_kullanicilar","kullanici_id","kullanici_kodu, kullanici_adi, "
                               "parola, email_adresi, email_kullanici_adi, aciklama, silinmis_mi ");

    sql_query.SET_VALUE      (  ":kullanici_kodu", m_ui->lineEdit_kullanici_kodu->text()        );
    sql_query.SET_VALUE      (  ":kullanici_adi" , m_ui->lineEdit_kullanici_adi->text()         );

    QByteArray hash      = QCryptographicHash::hash ( m_ui->line_edit_yeni_sifre->text().toUtf8(), QCryptographicHash::Md5);
    m_kullanici_parolasi = QString ( hash.toHex() );

    sql_query.SET_VALUE      (  ":parola"        , m_kullanici_parolasi                         );
    sql_query.SET_VALUE      (  ":email_adresi"  , m_ui->lineEdit_email->text()                 );
    sql_query.SET_VALUE      (  ":email_kullanici_adi"  , m_ui->line_edit_email_kullanici_adi->text() );
    sql_query.SET_VALUE      (  ":aciklama"      , m_ui->limitedTextEdit_not->toPlainText()       );
    sql_query.SET_VALUE      (  ":silinmis_mi"   , 0                                            );

    int kullanici_id        =   sql_query.INSERT();

    int table_row_count = m_ui->table_widget_bilgisayarlar->rowCount();

    if ( table_row_count NE 0 ) {

        for ( int i = 0; i < table_row_count; i++ ) {
            int bilgisayar_id = m_ui->table_widget_bilgisayarlar->item ( i,BILGISAYAR_ID_COLUMN )->text().toInt();
            sql_query.PREPARE_INSERT ( "ynt_bilgisayar_kullanicilari","id","bilgisayar_id, kullanici_id" );
            sql_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id );
            sql_query.SET_VALUE      ( ":kullanici_id" , kullanici_id  );
            sql_query.INSERT();
        }
    }
    table_row_count = m_ui->table_widget_veritabanlari->rowCount();
    if ( table_row_count NE 0 ) {
        for ( int i = 0; i < table_row_count; i++ ) {
            int veritabani_id  = m_ui->table_widget_veritabanlari->item ( i, VERITABANI_ID_COLUMN )->text().toInt();
            sql_query.PREPARE_INSERT (  "ynt_veritabani_kullanicilari","id","veritabani_id, kullanici_id");
            sql_query.SET_VALUE      (  ":veritabani_id", veritabani_id );
            sql_query.SET_VALUE      (  ":kullanici_id",  kullanici_id  );
            sql_query.INSERT();
        }
    }

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();

    QString log_detaylari = tr("User ID: ") + QVariant (  kullanici_id ).toString() + tr(" # User Code: ") + m_ui->lineEdit_kullanici_kodu->text()
                           + tr(" # User Name: ") + m_ui->lineEdit_kullanici_adi->text();

    YONETIM_007_KAYIT ( LOG_KULLANICILAR, LOG_KAYIT_EKLEME, log_detaylari );
    return kullanici_id;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::CHECK_UPDATE
***************************************************************************************/

int KULLANICILAR_FORMU::CHECK_UPDATE (int p_kullanici_id )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_kodu = :kullanici_kodu "
                               "AND silinmis_mi = :silinmis_mi ","" );

    sql_query.SET_VALUE      ( ":kullanici_kodu" , m_ui->lineEdit_kullanici_kodu->text() );
    sql_query.SET_VALUE      ( ":silinmis_mi"   ,0 );

    if ( sql_query.SELECT() > 0 ) {

        sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() NE p_kullanici_id ) {
            MSG_ERROR(tr ( "There are registered users in the system with this code." )  , NULL);//bu kodla kullanıcı sistemde mevcut
            return ADAK_FAIL;
        }
    }

    if ( m_ui->check_box_sifre->isChecked() EQ true ) {
        if ( m_ui->line_edit_yeni_sifre->text() NE m_ui->line_edit_yeni_sifre_tekrar->text() ) {
            MSG_ERROR("New and repeat passwords do not match,Please check.",m_ui->line_edit_yeni_sifre);//Girdiğiniz yeni şifre ve tekrarı uyuşmuyor Lütfen kontrol ediniz.
            return ADAK_FAIL;
        }
    }

    ADAK_MSG_ENUM answer = MSG_YES_NO_CANCEL( tr ( "Do you approve of the changes you made?" ) , NULL);//Yapılan değişiklikleri onaylıyor musunuz?
    if ( answer EQ ADAK_CANCEL OR answer EQ ADAK_NO  ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;

}

/**************************************************************************************
                   KULLANICILAR_FORMU::UPDATE_RECORD
***************************************************************************************/

void KULLANICILAR_FORMU::UPDATE_RECORD  ( int p_kullanici_id )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );
    sql_query.PREPARE_UPDATE ( "ynt_kullanicilar","kullanici_id","kullanici_kodu,kullanici_adi,parola,email_adresi,email_kullanici_adi,aciklama","kullanici_id = :kullanici_id");

    sql_query.SET_VALUE      ( ":kullanici_kodu",          m_ui->lineEdit_kullanici_kodu->text()       );
    sql_query.SET_VALUE      ( ":kullanici_adi",           m_ui->lineEdit_kullanici_adi->text()        );

    if ( m_ui->check_box_sifre->isChecked() EQ true ) {
        QByteArray hash      = QCryptographicHash::hash ( m_ui->line_edit_yeni_sifre->text().toUtf8(), QCryptographicHash::Md5);
        m_kullanici_parolasi = QString ( hash.toHex() );
    }

    sql_query.SET_VALUE      ( ":parola",                  m_kullanici_parolasi                        );
    sql_query.SET_VALUE      ( ":email_adresi",            m_ui->lineEdit_email->text()                );
    sql_query.SET_VALUE      ( ":email_kullanici_adi",     m_ui->line_edit_email_kullanici_adi->text() );
    sql_query.SET_VALUE      ( ":aciklama",                m_ui->limitedTextEdit_not->toPlainText()    );
    sql_query.SET_VALUE      ( ":kullanici_id",            p_kullanici_id                              );

    sql_query.UPDATE();

    QString log_detaylari = "Kullanıcı ID: " + QVariant ( p_kullanici_id).toString() + " # Kullanıcı Kodu: " + m_ui->lineEdit_kullanici_kodu->text()
                           + " # Kullanıcı Adı: " + m_ui->lineEdit_kullanici_adi->text();

    YONETIM_007_KAYIT ( LOG_KULLANICILAR, LOG_KAYIT_GUNCELLEME, log_detaylari );

    sql_query.PREPARE_DELETE ( "ynt_bilgisayar_kullanicilari","kullanici_id = :kullanici_id");
    sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

    sql_query.DELETE();

    int table_row_count = m_ui->table_widget_bilgisayarlar->rowCount();

    if ( table_row_count NE 0 ) {

        for ( int i = 0; i < table_row_count; i++ ) {
            int bilgisayar_id = m_ui->table_widget_bilgisayarlar->item( i,BILGISAYAR_ID_COLUMN )->text().toInt();
            sql_query.PREPARE_INSERT ( "ynt_bilgisayar_kullanicilari","id","bilgisayar_id,kullanici_id");

            sql_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id );
            sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

            sql_query.INSERT();
        }
    }
    sql_query.PREPARE_DELETE ( "ynt_veritabani_kullanicilari","kullanici_id = :kullanici_id" );
    sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

    sql_query.DELETE();

    table_row_count = m_ui->table_widget_veritabanlari->rowCount();

    if ( table_row_count NE 0) {

        for ( int i = 0; i < table_row_count; i++ ) {

            int veritabani_id = m_ui->table_widget_veritabanlari->item(i,VERITABANI_ID_COLUMN)->text().toInt();

            sql_query.PREPARE_INSERT ( "ynt_veritabani_kullanicilari","id","veritabani_id, kullanici_id");

            sql_query.SET_VALUE      ( ":veritabani_id"    , veritabani_id       );
            sql_query.SET_VALUE      ( ":kullanici_id", p_kullanici_id );

            sql_query.INSERT();
        }
    }

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();
}

/**************************************************************************************
                   KULLANICILAR_FORMU::CHECK_DELETE
***************************************************************************************/

int KULLANICILAR_FORMU::CHECK_DELETE ( int kullanici_id )
{
    if ( kullanici_id EQ 1 ) {
        MSG_INFO(tr("Admin user can not be deleted!"),m_ui->limitedTextEdit_not);//Yönetici kullanıcısı silinemez!
        return ADAK_FAIL;
    }

    if ( m_ui->lineEdit_kullanici_kodu->text() EQ KULLANICI_KODU() ) {
        MSG_WARNING( tr("You can not delete that login is made!"), NULL );//Giriş yapmış olduğunuz kullanıcıyı silemezsiniz.
        return ADAK_FAIL;
    }

    SQL_QUERY query( G_YONETIM_DB );

    query.PREPARE_SELECT( "ynt_kullanicilar", "kullanici_id", "");
    int kullanici_sayisi = query.SELECT();

    if ( kullanici_sayisi EQ 2 ) {
        MSG_WARNING( tr("The remaining single-user the system, you can not delete!"), NULL );//Sistemde Kalan Tek Kullanıcı, Silemezsiniz.
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::DELETE_RECORD
***************************************************************************************/

void KULLANICILAR_FORMU::DELETE_RECORD ( int p_kullanici_id )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_DELETE ( "ynt_bilgisayar_kullanicilari","kullanici_id = :kullanici_id" );
    sql_query.SET_VALUE      ( ":kullanici_id" , p_kullanici_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "ynt_veritabani_kullanicilari","kullanici_id = :kullanici_id" );
    sql_query.SET_VALUE      ( ":kullanici_id" , p_kullanici_id );
    sql_query.DELETE();


    sql_query.PREPARE_UPDATE ( "ynt_kullanicilar","kullanici_id","silinmis_mi","kullanici_id = :kullanici_id ") ;
    sql_query.SET_VALUE      ( ":silinmis_mi"  , 1 );
    sql_query.SET_VALUE      ( ":kullanici_id" , p_kullanici_id );
    sql_query.UPDATE();

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();

    QString log_detaylari = tr("User ID: ") + QVariant( p_kullanici_id).toString() + tr(" # User Code: ") + m_ui->lineEdit_kullanici_kodu->text()
                           + tr(" # User Name: ") + m_ui->lineEdit_kullanici_adi->text();

    YONETIM_007_KAYIT ( LOG_KULLANICILAR, LOG_KAYIT_SILME, log_detaylari );

}

/**************************************************************************************
                   KULLANICILAR_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    if ( KULLANICI_ID() EQ 1 ) {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id","silinmis_mi = 0","");
    }
    else {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id","silinmis_mi = 0 AND kullanici_id != 1","");
    }

    if ( sql_query.SELECT("kullanici_kodu", 0 , 1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   KULLANICILAR_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    if ( KULLANICI_ID() EQ 1 ) {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id","silinmis_mi = 0","");
    }
    else {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id","silinmis_mi = 0 AND kullanici_id != 1","");
    }

    if ( sql_query.SELECT("kullanici_kodu DESC", 0 , 1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   KULLANICILAR_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::FIND_NEXT_RECORD ( )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    if ( KULLANICI_ID() EQ 1 ) {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id"," kullanici_kodu > :kullanici_kodu AND silinmis_mi = 0","");
    }
    else {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id"," kullanici_kodu > :kullanici_kodu AND silinmis_mi = 0 AND kullanici_id != 1","");
    }

    sql_query.SET_VALUE      ( ":kullanici_kodu", m_ui->lineEdit_kullanici_kodu->text() );

    if ( sql_query.SELECT("kullanici_kodu", 0 , 1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   KULLANICILAR_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::FIND_PREV_RECORD ( )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    if ( KULLANICI_ID() EQ 1 ) {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id"," kullanici_kodu < :kullanici_kodu AND silinmis_mi = 0","");
    }
    else {
        sql_query.PREPARE_SELECT( "ynt_kullanicilar","kullanici_id"," kullanici_kodu < :kullanici_kodu AND silinmis_mi = 0 AND kullanici_id != 1","");
    }

    sql_query.SET_VALUE      ( ":kullanici_kodu", m_ui->lineEdit_kullanici_kodu->text() );

    if ( sql_query.SELECT("kullanici_kodu DESC", 0 , 1) EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}


/**************************************************************************************
                      KULLANICILAR_FORMU::FIND_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::FIND_RECORD()
{
    if ( m_ui->lineEdit_kullanici_kodu->text().isEmpty() EQ true ) {
        return 0;
    }
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_kodu = :kullanici_kodu AND silinmis_mi = 0" ,"");
    sql_query.SET_VALUE      ( ":kullanici_kodu", m_ui->lineEdit_kullanici_kodu->text() );
    if ( sql_query.SELECT() EQ 0 ) {
         return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   KULLANICILAR_FORMU::SELECT_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::SELECT_RECORD()
{
    if (KULLANICI_ID() EQ 1) {
        return KULLANICI_SEC ( true , this );
    }

    return KULLANICI_SEC ( false, this );
}

/**************************************************************************************
                   KULLANICILAR_FORMU::LOCK_RECORD
***************************************************************************************/

int KULLANICILAR_FORMU::LOCK_RECORD ( int p_kullanici_id )
{
    return G_YONETIM_DB->LOCK_ROW ( "kullanici_id", "ynt_kullanicilar", QString ( "kullanici_id = %1" ).arg ( p_kullanici_id ) );
}

/**************************************************************************************
                   KULLANICILAR_FORMU::UNLOCK_RECORD
***************************************************************************************/

void KULLANICILAR_FORMU::UNLOCK_RECORD ( int p_kullanici_id )
{
    G_YONETIM_DB->UNLOCK_ROW ( "kullanici_id", "ynt_kullanicilar", QString ( "kullanici_id = %1" ).arg ( p_kullanici_id ) );
}


/**************************************************************************************
                   KULLANICILAR_FORMU::LISTEYE_BILGISAYAR_EKLE
***************************************************************************************/

int KULLANICILAR_FORMU::LISTEYE_BILGISAYAR_EKLE ()
{
    m_secilen_bilgisayar_id = BILGISAYAR_SEC ( this );

    if ( m_secilen_bilgisayar_id  < 1 ) {
        return ADAK_RECORD_UNCHANGED;
    }

    for ( int i = 0; i < m_ui->table_widget_bilgisayarlar->rowCount(); i++ ) {

        int current_bilgisayar_id = m_ui->table_widget_bilgisayarlar->item (  i, BILGISAYAR_ID_COLUMN )->text().toInt();

        if ( current_bilgisayar_id NE m_secilen_bilgisayar_id ) {
           continue;
        }

        MSG_INFO(tr ( "The selected computer is already available on line no %n.","",i+1 ) ,NULL );//Seçilen bilgisayar %n nolu satırda zaten mevcut.
        return ADAK_RECORD_UNCHANGED;
    }

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_kodu,bilgisayar_adi,dongle_ip, "
                               "mac_adresi_kablolu, mac_adresi_kablosuz, mac_adresi_3g","bilgisayar_id = :bilgisayar_id","");

    sql_query.SET_VALUE      ( ":bilgisayar_id", m_secilen_bilgisayar_id );

    if ( sql_query.SELECT() EQ 0 ) {

        return ADAK_RECORD_UNCHANGED;
    }

    sql_query.NEXT();

    int new_row_num         = m_ui->table_widget_bilgisayarlar->rowCount();

    QString mac_kablolu;
    QString mac_kablosuz;
    QString mac_3g;

    if ( sql_query.VALUE(3).toString().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_kablolu = sql_query.VALUE(3).toString();
    }
    if ( sql_query.VALUE(4).toString().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_kablosuz = sql_query.VALUE(4).toString();
    }
    if ( sql_query.VALUE(5).toString().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_3g = sql_query.VALUE(5).toString();
    }


    m_ui->table_widget_bilgisayarlar->insertRow ( new_row_num );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, BILGISAYAR_ID_COLUMN,        new QTableWidgetItem ( QVariant(m_secilen_bilgisayar_id).toString() ) );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, BILGISAYAR_KODU_COLUMN,      new QTableWidgetItem ( sql_query.VALUE(0).toString() ) );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, BILGISAYAR_ADI_COLUMN,       new QTableWidgetItem ( sql_query.VALUE(1).toString() ) );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, DONGLE_IP_COLUMN,            new QTableWidgetItem ( sql_query.VALUE(2).toString() ) );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, MAC_ADRESI_KABLOLU_COLUMN,   new QTableWidgetItem ( mac_kablolu  ) );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, MAC_ADRESI_KABLOSUZ_COLUMN,  new QTableWidgetItem ( mac_kablosuz ) );
    m_ui->table_widget_bilgisayarlar->setItem   ( new_row_num, MAC_ADRESI_3G_COLUMN,        new QTableWidgetItem ( mac_3g       ) );

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::LISTEYE_VERITABANI_EKLE
***************************************************************************************/

int KULLANICILAR_FORMU::LISTEYE_VERITABANI_EKLE()
{
    m_secilen_veritabani_id = VERITABANI_SEC ( -1, NULL, NULL, this );

    if ( m_secilen_veritabani_id  < 1 ) {
        return ADAK_RECORD_UNCHANGED;
    }

    for ( int i = 0; i < m_ui->table_widget_veritabanlari->rowCount(); i++ ) {

        int current_veritabani_id = m_ui->table_widget_veritabanlari->item( i, VERITABANI_ID_COLUMN )->text().toInt();

        if ( current_veritabani_id NE m_secilen_veritabani_id ) {
           continue;
        }

        MSG_INFO(tr ( "The selected database are already available on line no %n.", "", i + 1 ) , NULL);//Seçilen veritabanı %n nolu satırda zaten mevcut.
        return ADAK_RECORD_UNCHANGED;
    }

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id, veritabani_ismi, veritabani_tanimi","veritabani_id = :veritabani_id ","");
    sql_query.SET_VALUE      ( ":veritabani_id", m_secilen_veritabani_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_RECORD_UNCHANGED;
    }

    sql_query.NEXT();

    int new_row_num          = m_ui->table_widget_veritabanlari->rowCount();

    m_ui->table_widget_veritabanlari->insertRow ( new_row_num );
    m_ui->table_widget_veritabanlari->setItem   ( new_row_num, VERITABANI_ID_COLUMN   , new QTableWidgetItem ( QVariant(m_secilen_veritabani_id).toString() ) );
    m_ui->table_widget_veritabanlari->setItem   ( new_row_num, VERITABANI_COLUMN , new QTableWidgetItem ( sql_query.VALUE(1).toString() ) );
    m_ui->table_widget_veritabanlari->setItem   ( new_row_num, VERITABANI_TANIMI_COLUMN  , new QTableWidgetItem ( sql_query.VALUE(2).toString() )  );

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::BILGISAYAR_LISTESINDEN_SATIR_SIL
***************************************************************************************/

int KULLANICILAR_FORMU::BILGISAYAR_LISTESINDEN_SATIR_SIL ( int p_clicked_row, int p_clicked_column )
{

    if ( p_clicked_row < 0 OR p_clicked_column < 0 ) {
        return ADAK_FAIL;
    }

    m_ui->table_widget_bilgisayarlar->removeRow ( p_clicked_row );

    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::VERITABANI_LISTESINDEN_SATIR_SIL
***************************************************************************************/

int KULLANICILAR_FORMU::VERITABANI_LISTESINDEN_SATIR_SIL ( int p_clicked_row, int p_clicked_column )
{
    if ( p_clicked_row < 0 OR p_clicked_column < 0 ) {
        return ADAK_FAIL;
    }

    m_ui->table_widget_veritabanlari->removeRow ( p_clicked_row );

    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int KULLANICILAR_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( p_button EQ m_ui->button_bilgisayar_sec ) {
        return LISTEYE_BILGISAYAR_EKLE();
    }
    else if ( p_button EQ m_ui->button_veritabani_sec ) {
        return LISTEYE_VERITABANI_EKLE();
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   KULLANICILAR_FORMU::SAVER_BUTTON_CLICKED
***************************************************************************************/

void    KULLANICILAR_FORMU::SAVER_BUTTON_CLICKED( QAbstractButton * p_button, int p_record_id )
{
    if ( p_button EQ m_ui->pushButton_yetki_ata ) {

        SQL_QUERY query( G_YONETIM_DB );

        QList < int > yetkisi_bulunan_db_ler;

        query.PREPARE_SELECT( "ynt_veritabani_kullanicilari", "veritabani_id", "kullanici_id = :kullanici_id");
        query.SET_VALUE     ( ":kullanici_id", GET_RECORD_ID() );

        if ( query.SELECT() NE 0 ) {
            while ( query.NEXT() EQ true ) {
                yetkisi_bulunan_db_ler << query.VALUE( 0 ).toInt();
            }
        }
        if ( yetkisi_bulunan_db_ler.contains( VERITABANI_ID() ) EQ false ) {
            MSG_WARNING( tr("No work on the user's database <b>%1</b> authority.").arg( VERITABANI_ISMI() ), NULL );//Kullanıcının  <b> %1 </b>  veritabanında çalışma yetkisi yok.
        }
        else {
            M_KULLANICI_YETKILERI_FONK ( p_record_id  ,m_yetkileri_tazele, nativeParentWidget() );
        }
    }
}
