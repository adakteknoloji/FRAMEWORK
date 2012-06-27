#include <QDate>

#include "adak_utils.h"
#include "sms.h"
#include "sms_sender_gui.h"
#include "sms_sender.h"
#include "yonetim.h"

extern ADAK_SQL * G_YONETIM_DB;

void SHOW_SMS_GUI(const QString &from, const QString &to, const QString &message, QWidget * parent)
{
    SMS_SENDER_GUI * F = new SMS_SENDER_GUI(from, to, message, parent);
    F->EXEC(NOT_FULL_SCREEN);
}

/**************************************************/
/*      SMS_SENDER_GUI::SMS_SENDER_GUI            */
/**************************************************/

SMS_SENDER_GUI::SMS_SENDER_GUI(QString from, QString to, QString message, QWidget * parent) :  BATCH_KERNEL (parent), m_ui(new Ui::SMS_SENDER_GUI)
{
    p_from_gsm_no    = from;
    p_to_gsm_no      = to;
    p_message        = message;

    m_ui->setupUi      (this);
    START_BATCH_KERNEL (this, NULL);

}
/**************************************************************************************
                   SMS_SENDER_GUI::SETUP_FORM
***************************************************************************************/

void SMS_SENDER_GUI::SETUP_FORM ()
{
    SET_PAGE_TITLE      ("SMS GÖNDER");
    SET_HELP_PAGE       ("sms_gonder");
    SET_SETTING_NAME    ("sms_gonder");

    SET_NAME_OF_RUN_BATCH_BUTTON(QString::fromUtf8("Gönder"));
    SET_AUTO_EXIT_BATCH ( true );
    REGISTER_BUTTON_WIDGET (m_ui->widget_batch_buttons);

    m_ui->line_edit_from->setText(KULLANICI_ADI());

    if (p_to_gsm_no.isEmpty() EQ false) {
        if ( p_to_gsm_no.contains(",",Qt::CaseInsensitive) EQ false ) {
            m_ui->line_edit_to->setText(p_to_gsm_no);
        }
        else {
            m_ui->label_to->hide();
            m_ui->label_gsm_zero->hide();
            m_ui->line_edit_ulke_kodu->hide();
            m_ui->line_edit_to->hide();
        }
    }

    if (p_message.isEmpty() EQ false) {
        m_ui->text_edit_message->document()->toPlainText();
    }
}

/*****************************************************/
/*         SMS_SENDER_GUI::CHECK_VAR                 */
/*****************************************************/

int SMS_SENDER_GUI::CHECK_VAR(QObject * object)
{
    Q_UNUSED(object);
    return ADAK_OK;
}

/*****************************************************/
/*          SMS_SENDER_GUI::CHECK_RUN              */
/*****************************************************/

int SMS_SENDER_GUI::CHECK_RUN()
{
    if ( m_ui->line_edit_to->text().isEmpty() EQ true ) {
        if ( m_ui->line_edit_to->isHidden() EQ false ) {
            MSG_ERROR("Lütfen gönderilecek telefon bilgisini giriniz.",m_ui->line_edit_to);
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/*****************************************************/
/*          SMS_SENDER_GUI::RUN_BATCH                */
/*****************************************************/

void SMS_SENDER_GUI::RUN_BATCH()
{
    QString message;

    p_from_gsm_no    = m_ui->line_edit_from->text();

    message    = m_ui->text_edit_message->document()->toPlainText();
    message   += " " + m_ui->line_edit_from->text();

    SQL_QUERY query (G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_sms_sunucusu","sms_app_id,sms_kullanici_adi,sms_parola,sms_sunucusu");
    if ( query.SELECT() EQ 0 ) {
        MSG_ERROR("SMS sunucu ayarları atanmamış.Önce yönetimden SMS sunucu ayarlarını atayınız.",NULL);
        return;
    }
    query.NEXT();

    SMS_SENDER * sms_sender = new SMS_SENDER(query.VALUE(3).toString(),query.VALUE(0).toString(),query.VALUE(1).toString(),query.VALUE(2).toString());

    if ( m_ui->line_edit_to->isHidden() EQ false ) {
        p_to_gsm_no      = m_ui->line_edit_ulke_kodu->text() + m_ui->line_edit_to->text();
        if ( sms_sender->SEND_SMS(p_from_gsm_no,p_to_gsm_no,message) EQ true ) {
            MSG_INFO("Mesajınız gönderildi.",NULL);
        }
    }
    else {
        QStringList telefon_numaralari = p_to_gsm_no.split(",",QString::SkipEmptyParts,Qt::CaseInsensitive);

        for ( int i = 0 ; i < telefon_numaralari.size() ; i++ ) {
            sms_sender->SEND_SMS(p_from_gsm_no,telefon_numaralari.at(i),message);
        }

        MSG_INFO("Mesajınız gönderildi.",NULL);
    }
}
