#include <QCryptographicHash>

#include "yonetici_izni_batch.h"
#include "ui_yonetici_izni_batch.h"

extern ADAK_SQL *           G_YONETIM_DB;

/**************************************************************************************
                   OPEN_YONETICI_IZNI_BATCH
***************************************************************************************/

bool OPEN_YONETICI_IZNI_BATCH ( QWidget * parent )
{
    bool onay_alindi = false;

    YONETICI_IZNI_BATCH * F = new YONETICI_IZNI_BATCH ( &onay_alindi, parent );
    F->EXEC (NOT_FULL_SCREEN);

    return onay_alindi;
}

/**************************************************************************************
                   YONETICI_IZNI_BATCH::YONETICI_IZNI_BATCH
***************************************************************************************/

YONETICI_IZNI_BATCH::YONETICI_IZNI_BATCH ( bool * onay_alindi,QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::YONETICI_IZNI_BATCH )
{
    m_onay_alindi      = onay_alindi;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, G_YONETIM_DB );
}

/**************************************************************************************
                   YONETICI_IZNI_BATCH::SETUP_FORM
***************************************************************************************/

void YONETICI_IZNI_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Confirm") );

    SET_ENTER_KEY_FOR_RUN_BATCH  (true);

    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_yonetici_sifre );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_run );

    SET_PAGE_TITLE    (tr("ADMINISTRATOR APPROVAL IS REQUIRED"));
    SET_AUTO_EXIT_BATCH ( true );
    SET_SETTING_NAME  ("YONETICI_IZNI");
    SET_HELP_PAGE     ("yönetici_onayı_batch.html");

    m_ui->line_edit_yonetici_sifre->setMaxLength(50);
}

/**************************************************************************************
                   YONETICI_IZNI_BATCH::CHECK_VAR
***************************************************************************************/

int YONETICI_IZNI_BATCH::CHECK_VAR ( QObject *  )
{
    return ADAK_OK;
}

/**************************************************************************************
                   YONETICI_IZNI_BATCH::CHECK_RUN
***************************************************************************************/

int YONETICI_IZNI_BATCH::CHECK_RUN ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_id=:kullanici_id AND parola = :parola","" );

    sql_query.SET_VALUE      ( ":kullanici_id", 1 );

    QByteArray hash = QCryptographicHash::hash ( m_ui->line_edit_yonetici_sifre->text().toUtf8(),QCryptographicHash::Md5 );

    sql_query.SET_VALUE ( ":parola", QString( hash.toHex() ) );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_INFO(QObject::tr ( "You entered the wrong Admin password.Please check.") , m_ui->line_edit_yonetici_sifre );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   YONETICI_IZNI_BATCH::RUN_BATCH
***************************************************************************************/

void YONETICI_IZNI_BATCH::RUN_BATCH ()
{
    *m_onay_alindi = true;
}
