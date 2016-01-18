#include <QHeaderView>

#include "yonetim.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "ui_sms_sunucusu_formu.h"
#include "sms_sunucusu_formu.h"
#include "sms_servers.h"

extern ADAK_SQL *      G_YONETIM_DB;

/**************************************************************************************
                   SHOW_SMS_SUNUCUSU_FORMU
***************************************************************************************/

void OPEN_SMS_SUNUCUSU_FORMU ( QWidget *parent )
{
    SMS_SUNUCUSU_FORMU * F = new SMS_SUNUCUSU_FORMU ( parent );
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::SMS_SUNUCUSU_FORMU
***************************************************************************************/

SMS_SUNUCUSU_FORMU::SMS_SUNUCUSU_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ) , m_ui ( new Ui::SMS_SUNUCUSU_FORMU )
{
    m_ui->setupUi ( this );
    START_FORM_KERNEL (this, G_YONETIM_DB );
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::SETUP_FORM
***************************************************************************************/

void SMS_SUNUCUSU_FORMU::SETUP_FORM ()
{

    SET_SINGLE_RECORD_ID (1);

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget , true);

    SET_PAGE_TITLE ( tr ( "SMS SERVER" ) );
    SET_SETTING_NAME    ( "SMS_SUNUCUSU" );
    SET_HELP_PAGE       ( "yonetim-islemleri_sms-sunucusu-tanimlama" );

    SET_FIRST_FOCUS_WIDGET( m_ui->combo_box_sms_sunucusu );

    SQL_QUERY sql_query(G_YONETIM_DB);

    sql_query.PREPARE_SELECT("ynt_sms_sunucusu","sms_sunucu_id");

    if (sql_query.SELECT() EQ 0 ) {
        G_YONETIM_DB->START_TRANSACTION();
        sql_query.PREPARE_INSERT("ynt_sms_sunucusu","sms_sunucu_id","sms_sunucusu,sms_port, "
                                                                 "sms_kullanici_adi,sms_parola,sms_app_id");

        sql_query.SET_VALUE(":sms_sunucusu","");
        sql_query.SET_VALUE(":sms_port",80);
        sql_query.SET_VALUE(":sms_kullanici_adi","");
        sql_query.SET_VALUE(":sms_parola","");
        sql_query.SET_VALUE(":sms_app_id","");

        sql_query.INSERT();
        G_YONETIM_DB->COMMIT_TRANSACTION();
    }

    m_ui->combo_box_sms_sunucusu->addItems(GET_SMS_SERVER_LIST());

    QStringList servers;

    servers << "Türkiye - Avea";
    servers << "Türkiye - Turkcell";
    servers << "Türkiye - Vodafone";

    m_ui->combo_box_sms_sunucusu->addItems(servers);

    m_ui->line_edit_kullanici_adi->setMaxLength(50);
    m_ui->line_edit_sifre->setMaxLength(50);
    m_ui->line_edit_sunucu_id->setMaxLength(50);
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void SMS_SUNUCUSU_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::NEW_RECORD
***************************************************************************************/

void SMS_SUNUCUSU_FORMU::NEW_RECORD()
{
    return;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::GET_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::GET_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_sms_sunucusu","sms_sunucusu,sms_kullanici_adi, "
                                                  "sms_parola,sms_app_id","","");

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->line_edit_sifre->setText(sql_query.VALUE("sms_parola").toString());
    m_ui->line_edit_kullanici_adi->setText(sql_query.VALUE("sms_kullanici_adi").toString());
    m_ui->line_edit_sunucu_id->setText(sql_query.VALUE("sms_app_id").toString());

    QStringList sms_sunucusu = sql_query.VALUE("sms_sunucusu").toString().split("\t\t");

    if (sms_sunucusu.size() >= 1) {
        m_ui->combo_box_sms_sunucusu->setCurrentIndex(m_ui->combo_box_sms_sunucusu->findText(sms_sunucusu.at(0)));
        m_ui->combo_box_protokol->addItems(GET_SMS_PROTOCOL_LIST(m_ui->combo_box_sms_sunucusu->currentText()));
    }

    if (sms_sunucusu.size() >= 2) {
        m_ui->combo_box_protokol->setCurrentIndex(m_ui->combo_box_protokol->findText(sms_sunucusu.at(1)));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::CHECK_VAR
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->combo_box_sms_sunucusu ) {
        if ( GET_SMS_SERVER_LIST().contains(m_ui->combo_box_sms_sunucusu->currentText()) EQ true ) {
            m_ui->combo_box_protokol->addItems(GET_SMS_PROTOCOL_LIST(m_ui->combo_box_sms_sunucusu->currentText()));
        }
        else {
            MSG_ERROR("Bu sunucu programa tam olarak entegre edilmemiştir.İleri ki versiyonlarda entegre edilecektir.",NULL);
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::CHECK_EMPTY
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::CHECK_EMPTY()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::CHECK_ADD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::CHECK_ADD ()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::ADD_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::ADD_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::CHECK_UPDATE
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::UPDATE_RECORD
***************************************************************************************/

void SMS_SUNUCUSU_FORMU::UPDATE_RECORD ( int record_id )
{
    SQL_QUERY sql_query(G_YONETIM_DB);

    sql_query.PREPARE_UPDATE ( "ynt_sms_sunucusu","sms_sunucu_id","sms_sunucusu,sms_kullanici_adi,sms_parola,sms_app_id",
                               "sms_sunucu_id=:sms_sunucu_id");

    QString sunucu = m_ui->combo_box_sms_sunucusu->currentText() + "\t\t" + m_ui->combo_box_protokol->currentText();


    sql_query.SET_VALUE      ( ":sms_sunucusu"      ,sunucu );
    sql_query.SET_VALUE      ( ":sms_kullanici_adi" ,m_ui->line_edit_kullanici_adi->text() );
    sql_query.SET_VALUE      ( ":sms_parola"        ,m_ui->line_edit_sifre->text() );
    sql_query.SET_VALUE      ( ":sms_app_id"        ,m_ui->line_edit_sunucu_id->text() );

    sql_query.SET_VALUE      ( ":sms_sunucu_id",record_id );

    sql_query.UPDATE();
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::CHECK_DELETE
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::CHECK_DELETE(int record_id)
{
    Q_UNUSED(record_id);
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::DELETE_RECORD
***************************************************************************************/

void SMS_SUNUCUSU_FORMU::DELETE_RECORD(int record_id)
{
    Q_UNUSED(record_id);
    return;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::FIND_FIRST_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::FIND_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::SELECT_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::LOCK_RECORD
***************************************************************************************/

int SMS_SUNUCUSU_FORMU::LOCK_RECORD(int record_id)
{
    return G_YONETIM_DB->LOCK_ROW ( "sms_sunucu_id", "ynt_sms_sunucusu", QString ( "sms_sunucu_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                   SMS_SUNUCUSU_FORMU::UNLOCK_RECORD
***************************************************************************************/

void SMS_SUNUCUSU_FORMU::UNLOCK_RECORD( int record_id )
{
    Q_UNUSED(record_id);
    G_YONETIM_DB->UNLOCK_ROW ( "sms_sunucu_id", "ynt_sms_sunucusu", QString ( "sms_sunucu_id= %1" ).arg ( record_id ) );
}
