#include "adak_sql.h"
#include "ui_yonetim_007_arama.h"
#include "yonetim_007_arama.h"
#include "yonetim_utils.h"
#include "login.h"

#define     LOG_ID_COLUMN              0
#define     ISLEM_TARIHI_COLUMN        1
#define     ISLEM_SAATI_COLUMN         2
#define     KULLANICI_BILGISI_COLUMN   3
#define     BILGISAYAR_BILGISI_COLUMN  4
#define     LOG_TURU_COLUMN            5
#define     LOG_ISLEMI_COLUMN          6
#define     LOG_ACIKLAMA_COLUMN        7

extern ADAK_SQL * G_YONETIM_DB;

static int         g_secilen_id;

/**************************************************************************************
                   OPEN_YONETIM_007_ARAMA
***************************************************************************************/

void OPEN_YONETIM_007_ARAMA ( QWidget * parent )
{
    g_secilen_id = -1;
    YONETIM_007_ARAMA * F = new YONETIM_007_ARAMA(parent);
    F->EXEC (FULL_SCREEN);
}

/**************************************************************************************
                   YONETIM_007_ARAMA::YONETIM_007_ARAMA
***************************************************************************************/

YONETIM_007_ARAMA::YONETIM_007_ARAMA ( QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::YONETIM_007_ARAMA)
{
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, G_YONETIM_DB);
}


/**************************************************************************************
                   YONETIM_007_ARAMA::SETUP_FORM
***************************************************************************************/

void YONETIM_007_ARAMA::SETUP_FORM ()
{

    REGISTER_TABLE_WIDGET ( m_ui->tableWidget );
    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget_batch_buttons );

    SET_HEADERS(QStringList()<<tr ( "log_id")<<tr ( "Tarih" )<<tr ( "Saat" )<<tr ( "Kullanıcı Bilgisi")
                             <<tr ( "Bilgisayar Bilgisi" )<<tr ( "LOG Türü" )<<tr ( "LOG İşlemi" ) );

    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    SET_FIND_BUTTON_NAME(tr("Kayıtları Göster"));

    SET_SORTING ( false );

    m_ui->frame_tarih->setEnabled(false);
    m_ui->frame_log_turu->setEnabled(false);
    m_ui->frame_kullanici->setEnabled(false);
    m_ui->dateEdit_first_date->setDate(QDate::currentDate());
    m_ui->dateEdit_second_date->setDate(QDate::currentDate());

    SQL_QUERY sql_query(G_YONETIM_DB);
    sql_query.PREPARE_SELECT("ynt_kullanicilar","kullanici_kodu","silinmis_mi = 0");
    if ( sql_query.SELECT() > 0 ) {
        while (sql_query.NEXT() EQ true ) {
            m_ui->comboBox_kullanicilar->addItem(sql_query.VALUE(0).toString());
        }
    }

    m_ui->comboBox_log_turu->addItem("BILGISAYARLAR");
    m_ui->comboBox_log_turu->addItem("KULLANICILAR");
    m_ui->comboBox_log_turu->addItem("VERITABANLARI");
    m_ui->comboBox_log_turu->addItem("PROGRAM");

    SET_PAGE_TITLE    ( tr("YÖNETİM 007 / LOG BİLGİLERİ"));
    SET_SETTING_NAME  ("YONETIM_007");
    SET_HELP_PAGE     ("yonetim-islemleri_yonetim-log-bilgileri");

    m_ui->tableWidget->setColumnWidth ( ISLEM_TARIHI_COLUMN      , 130 );
    m_ui->tableWidget->setColumnWidth ( ISLEM_SAATI_COLUMN       ,  50 );
    m_ui->tableWidget->setColumnWidth ( KULLANICI_BILGISI_COLUMN , 120 );
    m_ui->tableWidget->setColumnWidth ( BILGISAYAR_BILGISI_COLUMN, 250 );

    m_ui->tableWidget->hideColumn ( LOG_ID_COLUMN);

    m_ui->limitedTextEdit_ekstra_bilgiler->SET_LINE_LIMIT(5);
    m_ui->limitedTextEdit_ekstra_bilgiler->SET_MAX_STRING_LENGTH(500);


    m_ui->checkbox_tarih->setChecked( true );
    m_ui->frame_tarih->setEnabled( true );

    SET_FIRST_FOCUS_WIDGET( m_ui->dateEdit_first_date );
}

/**************************************************************************************
                   YONETIM_007_ARAMA::CHECK_VAR
***************************************************************************************/

int YONETIM_007_ARAMA::CHECK_VAR ( QObject * object )
{
    if (object EQ m_ui->dateEdit_first_date ) {
        if ( m_ui->dateEdit_first_date->date() > m_ui->dateEdit_second_date->date() ) {
            MSG_ERROR(tr("İlk tarih son tarihten büyük olamaz" ) , NULL);
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->dateEdit_second_date ) {
        if ( m_ui->dateEdit_second_date->date() < m_ui->dateEdit_first_date->date() ) {
            MSG_ERROR(tr ( "Son tarih ilk tarihten küçük olamaz" ) , NULL);
            return ADAK_FAIL;
        }
    }
    else if ( object EQ m_ui->checkbox_tarih ) {
        if ( m_ui->checkbox_tarih->isChecked() EQ true ) {
            m_ui->frame_tarih->setEnabled(true);
        }
        else {
            m_ui->frame_tarih->setEnabled(false);
        }
    }
    else if ( object EQ m_ui->checkbox_log_turu ) {
        if ( m_ui->checkbox_log_turu->isChecked() EQ true ) {
            m_ui->frame_log_turu->setEnabled(true);
        }
        else {
            m_ui->frame_log_turu->setEnabled(false);
        }
    }
    else if ( object EQ m_ui->checkbox_kullanici ) {
        if ( m_ui->checkbox_kullanici->isChecked() EQ true ) {
            m_ui->frame_kullanici->setEnabled(true);
        }
        else {
            m_ui->frame_kullanici->setEnabled(false);
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   YONETIM_007_ARAMA::CHECK_RUN
***************************************************************************************/

int YONETIM_007_ARAMA::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   YONETIM_007_ARAMA::SELECTED_LINE
***************************************************************************************/

void YONETIM_007_ARAMA::SELECTED_LINE (  int selected_line_id )
{
    SQL_QUERY sql_query ( G_YONETIM_DB);
    sql_query.PREPARE_SELECT("ynt_007","log_detaylari","id = :id");
    sql_query.SET_VALUE(":id", selected_line_id );
    if (sql_query.SELECT() EQ 0 ) {
        m_ui->limitedTextEdit_ekstra_bilgiler->clear();
        return;
    }
    sql_query.NEXT();
    m_ui->limitedTextEdit_ekstra_bilgiler->setText( sql_query.VALUE(0).toString());
}

/**************************************************************************************
                   YONETIM_007_ARAMA::SEARCH
***************************************************************************************/

void YONETIM_007_ARAMA::SEARCH()
{
    QString first_date   = m_ui->dateEdit_first_date->date().toString("yyyy.MM.dd");
    QString last_date    = m_ui->dateEdit_second_date->date().toString("yyyy.MM.dd");

    SQL_QUERY sql_query (G_YONETIM_DB);

    int kullanici_id = 0;

    sql_query.PREPARE_SELECT ( "ynt_007","id, islem_tarihi, islem_saati, kullanici_id, bilgisayar_id,log_turu, log_islemi"  );

    if ( m_ui->checkbox_tarih->isChecked() EQ true ) {

        sql_query.AND_EKLE  ( "islem_tarihi BETWEEN :ilk_tarih AND :son_tarih " );
        sql_query.SET_VALUE ( ":ilk_tarih", first_date );
        sql_query.SET_VALUE ( ":son_tarih", last_date );
    }

    if ( m_ui->checkbox_log_turu->isChecked() EQ true ) {

        sql_query.AND_EKLE( " log_turu = :log_turu " );
        sql_query.SET_VALUE( ":log_turu", GET_LOG_TURU(m_ui->comboBox_log_turu->currentText()) );
    }

    if ( m_ui->checkbox_kullanici->isChecked() EQ true ) {

        SQL_QUERY query( G_YONETIM_DB );

        query.PREPARE_SELECT("ynt_kullanicilar","kullanici_id","kullanici_kodu = :kullanici_kodu AND silinmis_mi = :silinmis_mi");

        query.SET_VALUE(":silinmis_mi", 0 );
        query.SET_VALUE(":kullanici_kodu", m_ui->comboBox_kullanicilar->currentText());
        if ( query.SELECT() EQ 0 ) {
            return;
        }

        query.NEXT();
        kullanici_id = query.VALUE(0).toInt();

        sql_query.AND_EKLE  ( "kullanici_id = :kullanici_id" );
        sql_query.SET_VALUE ( ":kullanici_id", kullanici_id );
    }


    if ( sql_query.SELECT("islem_tarihi DESC, islem_saati DESC ") EQ 0 ) {
        return;
    }
    while ( sql_query.NEXT() EQ true ) {
        int current_row = ADD_NEW_ROW();
        m_ui->tableWidget->item ( current_row, LOG_ID_COLUMN )->setText ( sql_query.VALUE(0).toString() );

        QString islem_tarihi = sql_query.VALUE(1).toDate().toString("dd.MM.yyyy.dddd ");
        m_ui->tableWidget->item ( current_row, ISLEM_TARIHI_COLUMN )->setText ( islem_tarihi );
        m_ui->tableWidget->item ( current_row, ISLEM_SAATI_COLUMN )->setText ( sql_query.VALUE(2).toString());

        m_ui->tableWidget->item ( current_row, KULLANICI_BILGISI_COLUMN )->setText
                                    ( KULLANICI_BILGISI_OLUSTUR ( sql_query.VALUE(3).toInt() ) );
        m_ui->tableWidget->item ( current_row, BILGISAYAR_BILGISI_COLUMN )->setText
                ( BILGISAYAR_BILGISI_OLUSTUR ( sql_query.VALUE(4).toInt() ) );

        m_ui->tableWidget->item ( current_row, LOG_TURU_COLUMN )->setText( GET_LOG_TURU(sql_query.VALUE(5).toInt()));
        m_ui->tableWidget->item ( current_row, LOG_ISLEMI_COLUMN )->setText(GET_LOG_ISLEMI(sql_query.VALUE(6).toInt()));
    }

}

/**************************************************************************************
                   YONETIM_007_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int YONETIM_007_ARAMA::SINGLE_LINE_SELECTED ( int current_row )
{
    Q_UNUSED (current_row);
    return ADAK_CONTINUE;
}
