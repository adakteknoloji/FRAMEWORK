#include <iostream>
#include <QDate>
#include <QApplication>
#include "adak_utils.h"
#include "adak_sql_error_dialog.h"
#include "ui_adak_sql_error_dialog.h"
#include "email.h"
#include "login_utils.h"

using namespace std;

extern  QStringList G_FORM_NAME;

/**************************************************************************************
                   OPEN_ADAK_SQL_ERROR_DIALOG
***************************************************************************************/

void OPEN_ADAK_SQL_ERROR_DIALOG ( const QString& hostname,    const QString& db_name,
                                   const QString& driver_name, const QString& os, 
				   int   error_number,
                                   const QString& error_type,  const QString& error_text,
                                   const QString& aciklama,    const QString& hatali_query )
{
    QApplication::restoreOverrideCursor();

    ADAK_SQL_ERROR_DIALOG error_dialog;

    QString hatanin_olustugu_form;

    if ( G_FORM_NAME.size() > 0 ) {
         hatanin_olustugu_form = G_FORM_NAME.last();
    }
    else {
        // hatanin_olustugu_form = QT_TR_NOOP_UTF8 ("KERNEL dışı ekran");
        hatanin_olustugu_form = QObject::tr ("Screen is out of KERNEL");
    }
//    QString mesaj_icerigi = "Tarih                 :&nbsp;" + QDate::currentDate().toString ( "dd.MM.yyyy" ) + "<br />";
//    mesaj_icerigi        += "Saat                  :&nbsp;" + QTime::currentTime().toString ( "hh:mm" )      + "<br />";
//    mesaj_icerigi        += "Sunucu                :&nbsp;" + hostname                                       + "<br />";
//    mesaj_icerigi        += "İşletim Sistemi       :&nbsp;" + os                                             + "<br />";
//    mesaj_icerigi        += "Veritabanı            :&nbsp;" + db_name                                        + "<br />";
//    mesaj_icerigi        += "SQL MOTORU            :&nbsp;" + driver_name                                    + "<br />";
//    mesaj_icerigi        += "Hata Numarası         :&nbsp;" + QVariant ( error_number ).toString()           + "<br />";
//    mesaj_icerigi        += "Hata Türü             :&nbsp;" + error_type                                     + "<br />";
//    mesaj_icerigi        += "Hata Mesajı           :&nbsp;" + error_text                                     + "<br />";
//    mesaj_icerigi        += "Hatanın Alındığı Form :&nbsp;" + hatanin_olustugu_form                          + "<br />";
//    mesaj_icerigi        += "Açıklama              :&nbsp;" + aciklama                                       + "<br />";
//    mesaj_icerigi        += "Hatalı Sorgu          :&nbsp;" + hatali_query                                   + "<br />";


    QString mesaj_icerigi = QObject::tr ("Date               :&nbsp;") + QDate::currentDate().toString ( "dd.MM.yyyy" ) + "<br />";
    mesaj_icerigi        += QObject::tr ("Time               :&nbsp;") + QTime::currentTime().toString ( "hh:mm" )      + "<br />";
    mesaj_icerigi        += QObject::tr ("Server             :&nbsp;") + hostname                                       + "<br />";
    mesaj_icerigi        += QObject::tr ("Operating System   :&nbsp;") + os                                             + "<br />";
    mesaj_icerigi        += QObject::tr ("Database           :&nbsp;") + db_name                                        + "<br />";
    mesaj_icerigi        += QObject::tr ("SQL MOTOR          :&nbsp;") + driver_name                                    + "<br />";
    mesaj_icerigi        += QObject::tr ("Error Number       :&nbsp;") + QVariant ( error_number ).toString()           + "<br />";
    mesaj_icerigi        += QObject::tr ("ERror Type         :&nbsp;") + error_type                                     + "<br />";
    mesaj_icerigi        += QObject::tr ("Error Message      :&nbsp;") + error_text                                     + "<br />";
    mesaj_icerigi        += QObject::tr ("Screen name        :&nbsp;") + hatanin_olustugu_form                          + "<br />";
    mesaj_icerigi        += QObject::tr ("Description        :&nbsp;") + aciklama                                       + "<br />";
    mesaj_icerigi        += QObject::tr ("Failed query       :&nbsp;") + hatali_query                                   + "<br />";


    error_dialog.m_ui->textEdit_mail_text->setText        ( mesaj_icerigi );
    error_dialog.m_ui->label_mail_atilacak_adres->setText ( ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()) );

    error_dialog.exec();
}



/**************************************************************************************
                   ADAK_SQL_ERROR_DIALOG::ADAK_SQL_ERROR_DIALOG
***************************************************************************************/

ADAK_SQL_ERROR_DIALOG::ADAK_SQL_ERROR_DIALOG ( QWidget * parent ) : QDialog ( parent ), m_ui(new Ui::ADAK_SQL_ERROR_DIALOG)
{
    m_ui->setupUi ( this );

    connect ( m_ui->button_mail_at, SIGNAL ( clicked() ),   this,   SLOT(SLOT_HATA_MAILI_AT() ) );
}

/**************************************************************************************
                   ADAK_SQL_ERROR_DIALOG::~ADAK_SQL_ERROR_DIALOG
**************************************************************************************/

ADAK_SQL_ERROR_DIALOG::~ADAK_SQL_ERROR_DIALOG()
{
    delete m_ui;
}

/**************************************************************************************
                   ADAK_SQL_ERROR_DIALOG::SLOT_HATA_MAILI_AT
***************************************************************************************/

void ADAK_SQL_ERROR_DIALOG::SLOT_HATA_MAILI_AT()
{
    if ( m_ui->text_edit_detayli_bilgi->toPlainText().isEmpty() EQ true ) {
        //translate ADAK_ERROR ("Lütfen \"Hatayı <b>NEREDE ve NE YAPARKEN</b> aldınız\" kısmını doldurunuz.\n Hatanın <b>Çözümü için Çok Önemlidir</b>.",m_ui->text_edit_detayli_bilgi,this);
        ADAK_ERROR ("Please fill the \"<b>WHERE and WHAT WAS DOING</b> section\".\n To fix the bug it's <b>Very Important</b>.",m_ui->text_edit_detayli_bilgi,this);
        return;
    }

    if ( m_ui->line_edit_email->text().isEmpty() EQ true ) {
        //translate ADAK_ERROR("Lütfen iletişim için e-posta adresinizi yazınız.",m_ui->line_edit_email,this);
        ADAK_ERROR ("Please write your e-mail account to contact you if we need.",m_ui->line_edit_email,this);
        return;
    }

    if ( m_ui->line_edit_email->text().contains("@",Qt::CaseInsensitive ) EQ false ) {
        //translate ADAK_ERROR("Lütfen geçerli bir e-posta adresi yazınız.",m_ui->line_edit_email,this);
        ADAK_ERROR("Please write a valid e-mail account.",m_ui->line_edit_email,this);
        return;
    }

    m_ui->button_mail_at->setEnabled( false );

    QString from = m_ui->line_edit_email->text();

    QString subject         = ADAK_PROGRAM_LONGNAME(ADAK_DISPLAY_ID()) + " " + ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()) +
                            " SQL Hatası";

    //translate " SQL Hatası";
    //translate QString gosterilecek_msg= QObject::tr( "Zaman ayırıp hatayı bildirdiğiniz için teşekkür ederiz.");
    QString gosterilecek_msg= QObject::tr( "Thank you for sending bug report.");

    EMAIL_SET_EMAIL_SERVER ("smtp.gmail.com", 587, STARTTLS );
    EMAIL_SET_USER_EMAIL_INFO ( "adakerror", "qaz123XSW", "Adak Error Sender", true  );
    SEND_MAIL ( "adakerror@gmail.com", from, ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()), subject, QString(m_ui->textEdit_mail_text->toPlainText() + "\n" + m_ui->text_edit_detayli_bilgi->toPlainText()), -1 , -1 , gosterilecek_msg, false);

    close();
}
