#include "ui_hata_istek_batch.h"
#include "hata_istek_batch.h"
#include "print.h"
#include "email.h"
#include "yonetim.h"

extern ADAK_SQL    * G_YONETIM_DB;
extern QStringList    G_FORM_NAME;

/**************************************************************************************
                   OPEN_HATA_ISTEK_BATCH
***************************************************************************************/

void OPEN_HATA_ISTEK_BATCH ( QWidget * parent )
{
    HATA_ISTEK_BATCH * F = new HATA_ISTEK_BATCH ( parent );
    F->EXEC(NOT_FULL_SCREEN);
}

/**************************************************************************************
                   HATA_ISTEK_BATCH::HATA_ISTEK_BATCH
***************************************************************************************/

HATA_ISTEK_BATCH::HATA_ISTEK_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::HATA_ISTEK_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, G_YONETIM_DB );
}

/**************************************************************************************
                   HATA_ISTEK_BATCH::SETUP_FORM
***************************************************************************************/

void HATA_ISTEK_BATCH::SETUP_FORM ()
{

    SET_PAGE_TITLE    (tr("HATA / İSTEK BİLDİRİMİ"));
    SET_SETTING_NAME  ("HATA_ISTEK_EKRANI");
    SET_HELP_PAGE     ("hata_istek");
    SET_AUTO_EXIT_BATCH ( true );
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("GÖNDER")      );
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_button );

    if ( KULLANICI_ID() NE -1 ) {
         SQL_QUERY sql_query(G_YONETIM_DB);
         sql_query.PREPARE_SELECT ( "ynt_kullanicilar","email_adresi","kullanici_id = :kullanici_id AND silinmis_mi = 0","");
         sql_query.SET_VALUE       ( ":kullanici_id", KULLANICI_ID() );
         if ( sql_query.SELECT() NE 0 ) {
             sql_query.NEXT();
             m_ui->line_edit_email->setText(sql_query.VALUE(0).toString());
         }
    }

    SET_FIRST_FOCUS_WIDGET ( m_ui->text_edit_hata_istek );

    m_ui->line_edit_email->setMaxLength(50);
    m_ui->label_mail_atilacak_adres->setText(ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()));
}

/**************************************************************************************
                   HATA_ISTEK_BATCH::CHECK_VAR
***************************************************************************************/

int HATA_ISTEK_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED (object);
    return ADAK_OK;
}

/**************************************************************************************
                   HATA_ISTEK_BATCH::CHECK_RUN
***************************************************************************************/

int HATA_ISTEK_BATCH::CHECK_RUN ()
{
    if ( m_ui->text_edit_hata_istek->toPlainText().isEmpty() EQ true ) {
        MSG_WARNING("Lütfen Hata/İstek kısmını doldurunuz.",m_ui->text_edit_hata_istek);
        return ADAK_FAIL;
    }

    if ( m_ui->line_edit_email->text().isEmpty() EQ true ) {
        MSG_WARNING("Lütfen iletişim için e-posta adresinizi yazınız.",m_ui->line_edit_email);
        return ADAK_FAIL;
    }

    if ( m_ui->line_edit_email->text().contains("@",Qt::CaseInsensitive ) EQ false ) {
        MSG_WARNING("Lütfen geçerli bir e-posta adresi yazınız.",m_ui->line_edit_email);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   HATA_ISTEK_BATCH::RUN_BATCH
***************************************************************************************/

void HATA_ISTEK_BATCH::RUN_BATCH ()
{
    QString from             = m_ui->line_edit_email->text();
    QString subject          = "Hata / Istek Bildirimi ( " + ADAK_PROGRAM_LONGNAME(ADAK_DISPLAY_ID()) + " " + ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()) + " )";

    EMAIL_SET_EMAIL_SERVER ("smtp.gmail.com", 587, STARTTLS );
    EMAIL_SET_USER_EMAIL_INFO ( "adakerror", "qaz123XSW", "Adak Error Sender", true  );
    SEND_MAIL ( "adakerror@gmail.com", from, ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()), subject, QString(m_ui->text_edit_hata_istek->toPlainText()), -1 , -1 , "", false);

    MSG_INFO("Hata / İstek bilgisi gönderildi.", NULL);
}
