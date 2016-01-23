#include <QDate>
#include <adak_utils.h>
#include <adak_sql.h>
#include <yonetim.h>
#include <email_sender.h>
#include <email.h>
#include "email_sender_gui.h"
#include "ui_email_sender_gui.h"

extern ADAK_SQL        *     G_YONETIM_DB;

void ADAK_EMAIL_SENDER ( const QString &from,const QString &gonderenin_gercek_adi,const QString &to, const QString &subject, const QString &body, QWidget * parent)
{
    EMAIL_SENDER_GUI * B = new EMAIL_SENDER_GUI ( from,gonderenin_gercek_adi,to, subject, body, parent );
    B->EXEC(NOT_FULL_SCREEN);
}

/**************************************************************************************
                   EMAIL_SENDER_GUI::EMAIL_SENDER_GUI
***************************************************************************************/

EMAIL_SENDER_GUI::EMAIL_SENDER_GUI ( QString from,QString gonderenin_gercek_adi, QString to, QString subject, QString body, QWidget * parent) :  BATCH_KERNEL (parent), m_ui(new Ui::EMAIL_SENDER_GUI)
{
    p_from           = from;
    p_to             = to;
    p_subject        = subject;
    p_body           = body;
    p_gonderenin_adi = gonderenin_gercek_adi;

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, NULL );
}

/**************************************************************************************
                   EMAIL_SENDER_GUI::SETUP_FORM
***************************************************************************************/

void EMAIL_SENDER_GUI::SETUP_FORM ()
{

    SET_AUTO_EXIT_BATCH ( true );
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Send") );
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );
    SET_PAGE_TITLE (tr("E-MAIL SENDING"));

    if ( p_from.isNull() EQ false ) {
        m_ui->line_edit_from->setText ( p_from );
    }
    if ( p_to.isNull() EQ false ) {
         m_ui->line_edit_to->setText(p_to);
    }
    if ( p_subject.isNull() EQ false ) {
         m_ui->line_edit_subject->setText ( p_subject );
    }

    if ( p_body.isEmpty() EQ false ) {
         m_ui->web_view_body->setHtml ( p_body );
    }
    else {
        m_ui->web_view_body->setHidden ( true );
        m_ui->text_edit_body_end->setHidden ( true );
    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_posta_sunucusu","email_sunucusu,email_port,email_baglanti_guvenligi","posta_sunucu_id=:posta_sunucu_id","");
    sql_query.SET_VALUE      ( ":posta_sunucu_id",1 );
    if( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        QString mail_server = sql_query.VALUE(0).toString();
        QString port_number = sql_query.VALUE(1).toString();
        int baglanti_guvenligi =  m_ui->combobox_baglanti_guvenligi->findText( sql_query.VALUE(2).toString() );
        m_ui->line_edit_mail_server->setText ( mail_server );
        m_ui->line_edit_port->setText ( port_number );
        m_ui->combobox_baglanti_guvenligi->setCurrentIndex( baglanti_guvenligi );
    }
    else {
        MSG_ERROR(tr ("Mail server did not define.It needs to be defined from 'System-Mail Server' menu."),NULL);
        //Posta Sunucusu Tanımlanmamış.'Sistem->Posta Sunucusu' menüsünden tanımlanması gerekli.
        close();
    }

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_adi,email_adresi,email_kullanici_adi","kullanici_id=:kullanici_id","");
    sql_query.SET_VALUE      ( ":kullanici_id",KULLANICI_ID() ) ;
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        m_ui->label_gonderenin_gercek_ismi->setText(sql_query.VALUE(0).toString() );
        m_ui->line_edit_from->setText(sql_query.VALUE(1).toString() );
        if ( sql_query.VALUE(2).toString().isEmpty() EQ false ) {
             m_ui->line_edit_kullanici_adi->setText ( sql_query.VALUE(2).toString() );
        }
        else {
           MSG_ERROR(tr ( "Your E-Mail address or E-Mail User name did not define. It needs to be defined from 'System-Users' menu."),NULL);
           //E-Posta adresiniz veya E-Posta Kullanıcı İsminiz tanımlanmamış.'Sistem->Kullanıcılar' menüsünden tanımlanması gerekli.
           close();
        }
    }
}

/**************************************************************************************
                   EMAIL_SENDER_GUI::CHECK_VAR
***************************************************************************************/

int EMAIL_SENDER_GUI::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->check_box_kullanici_parola ) {
        if ( m_ui->check_box_kullanici_parola->isChecked() EQ true ) {
            m_ui->frame_user_info->setEnabled ( true ) ;
        }
        else {
            m_ui->frame_user_info->setEnabled ( false );
        }
    }

    if ( m_ui->line_edit_from->text().isEmpty() EQ false ) {
        if ( m_ui->line_edit_from->text().contains("@") EQ false OR m_ui->line_edit_from->text().contains ( "." ) EQ false ) {
            MSG_ERROR(tr("Sender e-mail address entered as invalid address.... ")  ,NULL);//Gönderen olarak girilen e-mail adresi geçersiz
            return ADAK_FAIL;
        }

    }

    if ( m_ui->line_edit_to->text().isEmpty() EQ false ) {
        if ( m_ui->line_edit_to->text().contains("@") EQ false OR m_ui->line_edit_to->text().contains ( "." ) EQ false ) {
            MSG_ERROR(tr("Recipient e-mail address entered as invalid address.. ") ,NULL);//Alıcı olarak girilen e-mail adresi geçersiz
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   EMAIL_SENDER_GUI::CHECK_RUN
***************************************************************************************/

int EMAIL_SENDER_GUI::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   EMAIL_SENDER_GUI::RUN_BATCH
***************************************************************************************/

void EMAIL_SENDER_GUI::RUN_BATCH()
{
    QString all_body;

    p_from    = m_ui->line_edit_from->text();
    p_to      = m_ui->line_edit_to->text();
    p_subject = m_ui->line_edit_subject->text();

    all_body = ( m_ui->text_edit_body_begin->document()->toPlainText() ) + "<br>" + p_body +"<br>"+ (m_ui->text_edit_body_end->document()->toPlainText());

    bool    need_auth     = false;
    QString username      = m_ui->line_edit_kullanici_adi->text();
    QString password      = m_ui->line_edit_parola->text();

    if ( m_ui->check_box_kullanici_parola->isChecked() EQ true ) {
        need_auth = true;
    }

    int     port               = QVariant(m_ui->line_edit_port->text()).toInt();
    QString mail_server        = m_ui->line_edit_mail_server->text();
    QString protocol_name      = m_ui->combobox_baglanti_guvenligi->currentText();
    SSL_PROTOCOL protocol_id   = GET_SSL_PROTOCOL_ID ( protocol_name );

    EMAIL_SENDER::SET_EMAIL_SERVER ( mail_server,port,protocol_id );

    if ( need_auth EQ true ) {
        EMAIL_SENDER::SET_USER_EMAIL_INFO ( username,password,p_gonderenin_adi,true );
    }
    else {
        EMAIL_SENDER::SET_USER_EMAIL_INFO ( "","",p_gonderenin_adi,false );
    }

    SEND_MAIL ( p_from.toUtf8(),p_gonderenin_adi.toUtf8(), p_to.toUtf8(), p_subject.toUtf8(), all_body.toUtf8() , -1,-1 ,tr("Sended Your Message..."));

}


