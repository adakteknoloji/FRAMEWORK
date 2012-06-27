#include <QCryptographicHash>

#include <adak_utils.h>
#include <adak_sql.h>
#include <yonetim.h>
#include "sifre_degistirme_batch.h"
#include "ui_sifre_degistirme_batch.h"

extern ADAK_SQL *  G_YONETIM_DB;

/**************************************************************************************
                   OPEN_SIFRE_DEGISTIRME_BATCH
***************************************************************************************/

void OPEN_SIFRE_DEGISTIRME_BATCH ( QWidget * parent )
{
    SIFRE_DEGISTIRME_BATCH * F = new SIFRE_DEGISTIRME_BATCH ( -1, parent );
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   SIFRE_DEGISTIRME_BATCH::SIFRE_DEGISTIRME_BATCH
***************************************************************************************/

SIFRE_DEGISTIRME_BATCH::SIFRE_DEGISTIRME_BATCH ( int kullanici_id , QWidget * parent ) : BATCH_KERNEL ( parent ),   m_ui ( new Ui::SIFRE_DEGISTIRME_BATCH )
{
    if (kullanici_id NE -1) {
        m_kullanici_id = kullanici_id;
    }
    else {
        m_kullanici_id = KULLANICI_ID();
    }

    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, G_YONETIM_DB );
}

/**************************************************************************************
                   SIFRE_DEGISTIRME_BATCH::SETUP_FORM
***************************************************************************************/

void SIFRE_DEGISTIRME_BATCH::SETUP_FORM ()
{
    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIRST_FOCUS_WIDGET(m_ui->line_edit_old_pass);

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_PAGE_TITLE ( tr ( "ŞİFRE DEĞİŞTİRME EKRANI" ) );
    SET_AUTO_EXIT_BATCH ( true );

    SET_SETTING_NAME  ("SIFRE_DEGISTIRME");

    SET_HELP_PAGE ("yonetim-islemleri_sifre-degistir");

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_kodu","kullanici_id = :kullanici_id","");

    sql_query.SET_VALUE      ( ":kullanici_id", m_kullanici_id );

    if ( sql_query.SELECT() EQ 1 ) {
        sql_query.NEXT();
        m_ui->lineEdit_kullanici_kodu->setText ( sql_query.VALUE(0).toString() );
    }

    m_ui->lineEdit_kullanici_kodu->setMaxLength(50);
    m_ui->line_edit_old_pass->setMaxLength(50);
    m_ui->lineEdit_yeni_sifre->setMaxLength(50);
    m_ui->lineEdit_yeni_sifre_tekrar->setMaxLength(50);
}

/**************************************************************************************
                   SIFRE_DEGISTIRME_BATCH::CHECK_VAR
***************************************************************************************/

int SIFRE_DEGISTIRME_BATCH::CHECK_VAR (QObject * )
{
    return ADAK_OK;
}

/**************************************************************************************
                   SIFRE_DEGISTIRME_BATCH::CHECK_RUN
***************************************************************************************/

int SIFRE_DEGISTIRME_BATCH::CHECK_RUN()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_id=:kullanici_id AND parola = :parola","" );

    sql_query.SET_VALUE      ( ":kullanici_id", m_kullanici_id );

    QByteArray hash = QCryptographicHash::hash ( m_ui->line_edit_old_pass->text().toUtf8(),QCryptographicHash::Md5 );

    sql_query.SET_VALUE ( ":parola", QString( hash.toHex() ) );

    if ( sql_query.SELECT() EQ 0 ) {
        m_ui->line_edit_old_pass->clear();
        MSG_INFO(QObject::tr ( "Kullandığınız şifreyi yanlış girdiniz.Şifre değişikliği için kullanmakta olduğunuz şifreyi yazmanız gerekmektedir." ) , m_ui->line_edit_old_pass );
        return ADAK_FAIL;
    }

    if  ( m_ui->lineEdit_yeni_sifre->text() NE m_ui->lineEdit_yeni_sifre_tekrar->text() ) {
        MSG_ERROR(tr ( "Girdiğiniz yeni şifre ile tekrarı uyuşmuyor, lütfen kontrol ediniz"), m_ui->lineEdit_yeni_sifre );
        m_ui->lineEdit_yeni_sifre->clear();
        m_ui->lineEdit_yeni_sifre_tekrar->clear();
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SIFRE_DEGISTIRME_BATCH::RUN_BATCH
***************************************************************************************/

void SIFRE_DEGISTIRME_BATCH::RUN_BATCH()
{
    G_YONETIM_DB->START_TRANSACTION();

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_UPDATE ( "ynt_kullanicilar","kullanici_id","parola","kullanici_id = :kullanici_id");

    QByteArray hash = QCryptographicHash::hash( m_ui->lineEdit_yeni_sifre->text().toUtf8(), QCryptographicHash::Md5);

    sql_query.SET_VALUE ( ":parola"        , QString( hash.toHex()));
    sql_query.SET_VALUE ( ":kullanici_id"  , m_kullanici_id );
    sql_query.UPDATE();

    G_YONETIM_DB->COMMIT_TRANSACTION();
}

