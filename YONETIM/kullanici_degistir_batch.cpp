#include <QByteArray>
#include <QCryptographicHash>
#include "kullanici_degistir_batch.h"
#include "ui_kullanici_degistir_batch.h"
#include "yonetim.h"
#include "yonetim_utils.h"
#include "login.h"

extern ADAK_SQL * G_YONETIM_DB;
extern ADAK_SQL * DB;

/**************************************************************************************
                   KULLANICI_DEGISTIR
***************************************************************************************/

int KULLANICI_DEGISTIR ( QWidget * p_parent )
{
    int kullanici_id = -1;

    KULLANICI_DEGISTIR_BATCH * F = new KULLANICI_DEGISTIR_BATCH ( &kullanici_id ,  p_parent );
    F->EXEC(NOT_FULL_SCREEN);

    return kullanici_id;
}

/**************************************************************************************
                   KULLANICI_DEGISTIR_BATCH::KULLANICI_DEGISTIR_BATCH
***************************************************************************************/

KULLANICI_DEGISTIR_BATCH::KULLANICI_DEGISTIR_BATCH (int *kullanici_id, QWidget *p_parent) : BATCH_KERNEL ( p_parent ), m_ui ( new Ui::KULLANICI_DEGISTIR_BATCH )
{
    m_kullanici_id_ptr = kullanici_id;

    m_ui->setupUi     ( this );
    START_BATCH_KERNEL( this, G_YONETIM_DB);
}


/**************************************************************************************
                 KULLANICI_DEGISTIR_BATCH::SETUP_FORM
***************************************************************************************/

void KULLANICI_DEGISTIR_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Changing User")      );
    SET_FIRST_FOCUS_WIDGET       ( m_ui->lineEdit_kullanici_kodu );
    REGISTER_BUTTON_WIDGET       ( m_ui->widget_batch_buttons    );
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true  );
    SET_AUTO_EXIT_BATCH ( true );

    SET_PAGE_TITLE    ( tr ( "CHANGING USER SCREEN" ) ) ;
    SET_SETTING_NAME  ( "KULLANICI_DEGISTIRME" );
    SET_HELP_PAGE     ( "yonetim-islemleri_kullanici-degistir" );

    m_kullanici_id = -1;

    m_ui->lineEdit_kullanici_adi->setMaxLength(50);
    m_ui->lineEdit_kullanici_kodu->setMaxLength(10);
    m_ui->lineEdit_kullanici_parola->setMaxLength(50);
}

/**************************************************************************************
                   KULLANICI_DEGISTIR_BATCH::CHECK_VAR
***************************************************************************************/

int KULLANICI_DEGISTIR_BATCH::CHECK_VAR ( QObject * p_object )
{
    SQL_QUERY sql_query ( G_YONETIM_DB );

    if ( p_object EQ m_ui->lineEdit_kullanici_kodu ) {

        if ( m_ui->lineEdit_kullanici_kodu->text().isEmpty() EQ true ) {
            m_ui->lineEdit_kullanici_adi->clear();
            m_ui->lineEdit_kullanici_parola->clear();
            m_kullanici_id = -1;
            return ADAK_OK;
        }

        sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id, kullanici_adi","kullanici_kodu = :kullanici_kodu AND silinmis_mi = 0","");
        sql_query.SET_VALUE      ( ":kullanici_kodu", m_ui->lineEdit_kullanici_kodu->text() );

        if ( sql_query.SELECT() EQ 0 ) {
            MSG_ERROR(tr ( "User not found." ) , m_ui->lineEdit_kullanici_kodu );
            return ADAK_FAIL_UNDO;
        }
        sql_query.NEXT();

        m_kullanici_id = sql_query.VALUE(0).toInt();

        if ( m_kullanici_id NE 1 )  {
            SQL_QUERY sql_query ( G_YONETIM_DB );

            sql_query.PREPARE_SELECT ( "ynt_bilgisayar_kullanicilari","id","bilgisayar_id = :bilgisayar_id AND kullanici_id =:kullanici_id");

            sql_query.SET_VALUE      ( ":bilgisayar_id", BILGISAYAR_ID() );
            sql_query.SET_VALUE      ( ":kullanici_id" , m_kullanici_id  );

            if ( sql_query.SELECT() EQ 0 ) {
                MSG_ERROR( QObject::tr("The user is not registered on the computer.") , m_ui->lineEdit_kullanici_kodu );//Kullanıcı bilgisayarda kayıtlı değil.
                return ADAK_FAIL_UNDO;
            }
        }

        m_ui->lineEdit_kullanici_adi->setText ( sql_query.VALUE(1).toString() );

    }
    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICI_DEGISTIR_BATCH::CHECK_RUN
***************************************************************************************/

int KULLANICI_DEGISTIR_BATCH::CHECK_RUN()
{
    if ( m_ui->lineEdit_kullanici_kodu->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("User name can not be empty." ) , m_ui->lineEdit_kullanici_kodu);//Kullanıcı kodu boş bırakılamaz.
        return ADAK_FAIL;
    }

    if ( m_kullanici_id < 1 ) {
        MSG_ERROR(tr ( "Invalid user" ) , m_ui->lineEdit_kullanici_kodu );//Geçersiz kullanıcı
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id","kullanici_id = :kullanici_id AND parola = :parola AND silinmis_mi = 0","" );

    sql_query.SET_VALUE      ( ":kullanici_id", m_kullanici_id );

    QByteArray hash = QCryptographicHash::hash ( m_ui->lineEdit_kullanici_parola->text().toUtf8(), QCryptographicHash::Md5 );

    sql_query.SET_VALUE ( ":parola", QString( hash.toHex() ) );

    if ( sql_query.SELECT() EQ 0 ) {
        m_ui->lineEdit_kullanici_parola->clear();
        MSG_ERROR(tr ( "Invalid password" ) , m_ui->lineEdit_kullanici_parola );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   KULLANICI_DEGISTIR_BATCH::RUN_BATCH
***************************************************************************************/

void KULLANICI_DEGISTIR_BATCH::RUN_BATCH()
{
    if ( m_kullanici_id EQ KULLANICI_ID() ) {
        return;
    }
    SET_KULLANICI_ID   ( m_kullanici_id );
    SET_KULLANICI_KODU ( m_ui->lineEdit_kullanici_kodu->text() );
    SET_KULLANICI_ADI  ( m_ui->lineEdit_kullanici_adi->text()  );

    *m_kullanici_id_ptr = m_kullanici_id;

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari,ynt_veritabani_kullanicilari","ynt_veritabanlari.veritabani_id","ynt_veritabanlari.veritabani_id = "
                               "ynt_veritabani_kullanicilari.veritabani_id AND veritabani_ismi=:veritabani_ismi AND kullanici_id =:kullanici_id","");

    sql_query.SET_VALUE( ":kullanici_id"   , m_kullanici_id    );
    sql_query.SET_VALUE( ":veritabani_ismi", VERITABANI_ISMI() );

    if (sql_query.SELECT() EQ 0 AND m_kullanici_id NE 1) {
        MSG_ERROR(tr ( "The user selected no authorized to work in this database.\nSelect a new database." ) , NULL);//Seçili kullanıcının bu veritabanında çalışma izni yok. Yeni veritabanı seçiniz.
        if ( VERITABANI_DEGISTIR(&DB,this) EQ -1 ) {
            exit(88);
        }
        MSG_INFO(tr("Company change successful. \nSelected Company: "+ VERITABANI_ISMI().toUtf8() + " " + VERITABANI_TANIMI().toUtf8() ),NULL);//Firma değişikliği başarılı.\n Seçilen Firma:
    }
}

