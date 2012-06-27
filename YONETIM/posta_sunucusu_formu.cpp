#include <QHeaderView>

#include "yonetim.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "ui_posta_sunucusu_formu.h"
#include "posta_sunucusu_formu.h"

extern ADAK_SQL *      G_YONETIM_DB;

/**************************************************************************************
                   OPEN_POSTA_SUNUCUSU_FORMU
***************************************************************************************/

void OPEN_POSTA_SUNUCUSU_FORMU ( QWidget *parent )
{
    POSTA_SUNUCUSU_FORMU * F = new POSTA_SUNUCUSU_FORMU (parent);
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::POSTA_SUNUCUSU_FORMU
***************************************************************************************/

POSTA_SUNUCUSU_FORMU::POSTA_SUNUCUSU_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui::POSTA_SUNUCUSU_FORMU )
{
    m_ui->setupUi ( this );
    START_FORM_KERNEL (this, G_YONETIM_DB );
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::SETUP_FORM
***************************************************************************************/

void POSTA_SUNUCUSU_FORMU::SETUP_FORM ()
{

    SET_SINGLE_RECORD_ID (1);

    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget , true);

    SET_FIRST_FOCUS_WIDGET( m_ui->line_edit_mail_server );

    SET_PAGE_TITLE ( tr ( "POSTA SUNUCUSU" ) );
    SET_SETTING_NAME   ( "POSTA_SUNUCUSU" );
    SET_HELP_PAGE ( "yonetim-islemleri_posta-sunucusu-tanimlama" );


    SQL_QUERY sql_query ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_posta_sunucusu" ,"posta_sunucu_id","","");
    if ( sql_query.SELECT() EQ 0 ) {
        G_YONETIM_DB->START_TRANSACTION();
        sql_query.PREPARE_INSERT ( "ynt_posta_sunucusu","posta_sunucu_id","email_sunucusu,email_port,email_baglanti_guvenligi");
        sql_query.SET_VALUE(":email_sunucusu","Tanımlı Değil");
        sql_query.SET_VALUE(":email_port",25);
        sql_query.SET_VALUE(":email_baglanti_guvenligi","YOK");
        sql_query.INSERT();
        G_YONETIM_DB->COMMIT_TRANSACTION();
    }

    m_ui->line_edit_mail_server->setMaxLength(100);
    m_ui->line_edit_port->setMaxLength(10);
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void POSTA_SUNUCUSU_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::NEW_RECORD
***************************************************************************************/

void POSTA_SUNUCUSU_FORMU::NEW_RECORD()
{
    return;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::GET_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::GET_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT("ynt_posta_sunucusu","email_sunucusu,email_port,email_baglanti_guvenligi","","");
    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->line_edit_mail_server->setText ( sql_query.VALUE(0).toString() );
    m_ui->line_edit_port->setText ( sql_query.VALUE(1).toString() );
    m_ui->combo_box_baglanti_guvenligi->setCurrentIndex ( m_ui->combo_box_baglanti_guvenligi->findText(sql_query.VALUE(2).toString()));

    return ADAK_OK;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::CHECK_VAR
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::CHECK_VAR ( QObject * object )
{
    Q_UNUSED ( object );
    return ADAK_OK;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::CHECK_EMPTY
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::CHECK_EMPTY()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::CHECK_ADD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::CHECK_ADD ()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::ADD_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::ADD_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::CHECK_UPDATE
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::CHECK_UPDATE ( int record_id )
{
    Q_UNUSED ( record_id );
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::UPDATE_RECORD
***************************************************************************************/

void POSTA_SUNUCUSU_FORMU::UPDATE_RECORD ( int record_id )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_UPDATE ( "ynt_posta_sunucusu" , "posta_sunucu_id" , "email_sunucusu, email_port, email_baglanti_guvenligi" , "posta_sunucu_id = :posta_sunucu_id");

    sql_query.SET_VALUE      ( ":email_sunucusu",m_ui->line_edit_mail_server->text() );
    sql_query.SET_VALUE      ( ":email_port",m_ui->line_edit_port->text() );
    sql_query.SET_VALUE      ( ":email_baglanti_guvenligi",m_ui->combo_box_baglanti_guvenligi->currentText() );
    sql_query.SET_VALUE      ( ":posta_sunucu_id",record_id  );

    sql_query.UPDATE();
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::CHECK_DELETE
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::CHECK_DELETE ( int record_id )
{
    Q_UNUSED ( record_id );
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::DELETE_RECORD
***************************************************************************************/

void POSTA_SUNUCUSU_FORMU::DELETE_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
    return;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::FIND_FIRST_RECORD()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::FIND_LAST_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::FIND_NEXT_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::FIND_PREV_RECORD ()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::FIND_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::FIND_RECORD()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::SELECT_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::SELECT_RECORD()
{
    return 0;
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::LOCK_RECORD
***************************************************************************************/

int POSTA_SUNUCUSU_FORMU::LOCK_RECORD ( int record_id )
{
    return G_YONETIM_DB->LOCK_ROW ( "posta_sunucu_id", "ynt_posta_sunucusu", QString ( "posta_sunucu_id = %1" ).arg ( record_id ) );
}

/**************************************************************************************
                   POSTA_SUNUCUSU_FORMU::UNLOCK_RECORD
***************************************************************************************/

void POSTA_SUNUCUSU_FORMU::UNLOCK_RECORD ( int record_id )
{
    Q_UNUSED ( record_id );
    G_YONETIM_DB->UNLOCK_ROW ( "posta_sunucu_id", "ynt_posta_sunucusu", QString ( "posta_sunucu_id= %1" ).arg ( record_id ) );
}

