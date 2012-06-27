#include <QHeaderView>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QSplashScreen>

#include "yonetim.h"
#include "veritabanlari_formu.h"
#include "ui_veritabanlari_formu.h"
#include "kullanici_secimi.h"
#include "program_secimi.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "yonetim_utils.h"
#include "adak_utils.h"
#include "login.h"

#define KULLANICI_ID_COLUMN     0
#define KULLANICI_KODU_COLUMN   1
#define KULLANICI_ADI_COLUMN    2
#define PROGRAM_ID_COLUMN       0
#define PROGRAM_ADI_COLUMN      1

extern ADAK_SQL *G_YONETIM_DB;
extern ADAK_SQL *DB;

/**************************************************************************************
                   OPEN_VERITABANLARI_FORMU
***************************************************************************************/

void OPEN_VERITABANLARI_FORMU(QWidget *parent)
{
    VERITABANLARI_FORMU * F= new VERITABANLARI_FORMU (parent);
    F->EXEC (FULL_SCREEN);
}

/**************************************************************************************
                   VERITABANLARI_FORMU::VERITABANLARI_FORMU
***************************************************************************************/

VERITABANLARI_FORMU::VERITABANLARI_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ), m_ui ( new Ui::VERITABANLARI_FORMU )
{
    m_ui->setupUi    ( this );
    START_FORM_KERNEL( this, G_YONETIM_DB );
}

/**************************************************************************************
                   VERITABANLARI_FORMU::SETUP_FORM
***************************************************************************************/

void VERITABANLARI_FORMU::SETUP_FORM ()
{
    m_ui->table_widget_kullanicilar->hideColumn ( 0 );

    m_ui->table_widget_kullanicilar->setColumnWidth ( KULLANICI_KODU_COLUMN,100 );
    m_ui->table_widget_kullanicilar->setColumnWidth ( KULLANICI_ADI_COLUMN, 282 );

    REGISTER_BUTTONS_WIDGET ( m_ui->widget );

    REGISTER_CHANGER_BUTTON ( m_ui->button_kullanici_sec );

    SET_PAGE_TITLE    (tr("VERİTABANLARI ( ŞİRKETLER )"));
    SET_SETTING_NAME  ("VERITABANLARI_FORMU");
    SET_HELP_PAGE     ("yonetim-islemleri_veritabanlari");

    SET_FIRST_FOCUS_WIDGET( m_ui->lineEdit_veritabani_kodu );

    m_ui->lineEdit_veritabani_kodu->setMaxLength(10);
    m_ui->lineEdit_veritabani_adi->setMaxLength(50);
    m_ui->limitedTextEdit_not->SET_LINE_LIMIT(5);
    m_ui->limitedTextEdit_not->SET_MAX_STRING_LENGTH(100);
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void VERITABANLARI_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   VERITABANLARI_FORMU::NEW_RECORD
***************************************************************************************/

void VERITABANLARI_FORMU::NEW_RECORD()
{
    m_ui->lineEdit_veritabani_kodu->clear();
    m_ui->lineEdit_veritabani_adi->clear();
    m_ui->table_widget_kullanicilar->clearContents();
    m_ui->table_widget_kullanicilar->setRowCount(0);
    m_ui->limitedTextEdit_not->clear();

    m_ui->lineEdit_veritabani_kodu  ->setReadOnly(false);
    m_ui->lineEdit_veritabani_adi->setReadOnly(false);

    MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::GET_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::GET_RECORD ( int p_veritabani_id )
{
    m_ui->table_widget_kullanicilar->clearContents();
    m_ui->table_widget_kullanicilar->setRowCount(0);

    m_ui->lineEdit_veritabani_kodu->setReadOnly(true);

    SQL_QUERY sql_query       ( G_YONETIM_DB );
    SQL_QUERY kullanici_query ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT (  "ynt_veritabanlari","veritabani_ismi, veritabani_tanimi, aciklama","veritabani_id = :veritabani_id ","" );
    sql_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->lineEdit_veritabani_kodu->setText(sql_query.VALUE(0).toString());
    m_ui->lineEdit_veritabani_adi->setText (sql_query.VALUE(1).toString());
    m_ui->limitedTextEdit_not->setText  (sql_query.VALUE(2).toString());

    sql_query.PREPARE_SELECT("ynt_veritabani_kullanicilari","kullanici_id","veritabani_id = :veritabani_id","" );
    sql_query.SET_VALUE(":veritabani_id", p_veritabani_id );

    int current_table_row = 0;

    if ( sql_query.SELECT() > 0 ) {

        while ( sql_query.NEXT() EQ true ) {
            int kullanici_id = sql_query.VALUE(0).toInt();
            kullanici_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_kodu, kullanici_adi","kullanici_id = :kullanici_id "
                                             "AND silinmis_mi = 0" ,"");
            kullanici_query.SET_VALUE      ( ":kullanici_id", kullanici_id );
            if (kullanici_query.SELECT() EQ 0 ) {
                continue;
            }
            kullanici_query.NEXT();
            m_ui->table_widget_kullanicilar->insertRow ( current_table_row );
            m_ui->table_widget_kullanicilar->setItem   ( current_table_row, KULLANICI_ID_COLUMN  , new QTableWidgetItem( QVariant( kullanici_id ).toString() ) );
            m_ui->table_widget_kullanicilar->setItem   ( current_table_row, KULLANICI_KODU_COLUMN, new QTableWidgetItem( kullanici_query.VALUE(0).toString() ) );
            m_ui->table_widget_kullanicilar->setItem   ( current_table_row, KULLANICI_ADI_COLUMN , new QTableWidgetItem( kullanici_query.VALUE(1).toString() ) );
            current_table_row++;

        }
    }

    MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE(p_veritabani_id);

    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CHECK_VAR
***************************************************************************************/

int VERITABANLARI_FORMU::CHECK_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->lineEdit_veritabani_kodu ) {
        m_ui->lineEdit_veritabani_kodu->setText(m_ui->lineEdit_veritabani_kodu->text().toLower());
        if ( m_ui->lineEdit_veritabani_kodu->text().count() > 10 ) {
            MSG_ERROR(tr("Veritabanı ismi 10 karakterden fazla olamaz.") , NULL);
            return ADAK_FAIL;
        }
         QRegExp letter("(\\W+)");
         if ( m_ui->lineEdit_veritabani_kodu->text().contains(letter) EQ true ) {
            MSG_ERROR(tr("Veritabanı ismi rakam,harf veya \'_\' dışında karakter içeremez.") , NULL);
            return ADAK_FAIL;
         }
    }
    else if ( p_object EQ m_ui->lineEdit_veritabani_adi ) {
        if ( m_ui->lineEdit_veritabani_adi->text().count() > 50 ) {
            MSG_ERROR(tr("Veritabanı tanımı 50 karakterden fazla olamaz.") , NULL);
            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->limitedTextEdit_not ) {
        if ( m_ui->limitedTextEdit_not->toPlainText().count() > 100 ) {
            MSG_ERROR(tr("Açıklama 100 karakterden fazla olamaz.") , NULL);
            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ m_ui->table_widget_kullanicilar ) {

        if ( p_object->objectName() EQ "ITEM_DOUBLE_CLICKED" ) {

            int clicked_row    = m_ui->table_widget_kullanicilar->currentRow();
            int clicked_column = m_ui->table_widget_kullanicilar->currentColumn();

            return KULLANICILAR_LISTESINDEN_SATIR_SIL ( clicked_row, clicked_column );
        }

    }

    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CHECK_EMPTY
***************************************************************************************/

int VERITABANLARI_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_veritabani_kodu->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("Veritabanı ismi girilmemiş") , NULL);
        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_veritabani_adi->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("Veritabanı tanımı girilmemiş") , NULL);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CHECK_ADD
***************************************************************************************/

int VERITABANLARI_FORMU::CHECK_ADD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","veritabani_ismi = :veritabani_ismi" ,"");
    sql_query.SET_VALUE      ( ":veritabani_ismi", m_ui->lineEdit_veritabani_kodu->text() );
    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() > 0 ) {
            MSG_ERROR(tr("Bu kodla kayıtlı bir veritabani sistemde mevcut") , NULL);
            return ADAK_FAIL;
        }
    }

    QString veritabani_ismi = QString (SQL_DBNAME_ONEK() + "adak_" + m_ui->lineEdit_veritabani_kodu->text() );

    ADAK_SQL * db_gecici =  CONNECT_DATABASE( GET_PROGRAM_DB_STRUCTS(), veritabani_ismi, GET_SQL_DB_DRIVER_ENUM( SQL_MOTORU() ), false );

    if ( db_gecici EQ NULL ) {
        MSG_ERROR(tr("Bu kodla kayıtlı bir veritabani sistemde mevcut değildir.") , NULL);
        return ADAK_FAIL;
    }
    db_gecici->DISCONNECT_TO_DATABASE();

    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::ADD_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::ADD_RECORD ()
{
    QSplashScreen * splash = CREATE_SPLASH_SCREEN();

    splash->show();
    splash->showMessage ( QObject::tr ( "Veritabanı yaratılıyor. Lütfen Bekleyiniz..." ), Qt::AlignCenter,Qt::white );

    QString veritabani_ismi = QString ( SQL_DBNAME_ONEK() + "adak_" + m_ui->lineEdit_veritabani_kodu->text() );

    G_YONETIM_DB->COMMIT_TRANSACTION();

    G_YONETIM_DB->START_TRANSACTION();

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_INSERT ( "ynt_veritabanlari","veritabani_id","veritabani_ismi, veritabani_tanimi,aciklama" );

    sql_query.SET_VALUE      ( ":veritabani_ismi",      m_ui->lineEdit_veritabani_kodu->text() );
    sql_query.SET_VALUE      ( ":veritabani_tanimi",    m_ui->lineEdit_veritabani_adi->text()       );
    sql_query.SET_VALUE      ( ":aciklama",             m_ui->limitedTextEdit_not->toPlainText() );

    int veritabani_id         = sql_query.INSERT();

    QString log_detaylari = "Verıtabanı ID: " + QVariant ( veritabani_id ).toString() + " # Veritabanı İsmi: " + m_ui->lineEdit_veritabani_kodu->text();
                           + " # Veritabanı Tanımı: " + m_ui->lineEdit_veritabani_adi->text();

    YONETIM_007_KAYIT ( LOG_VERITABANLARI, LOG_VERITABANI_YARATMA, log_detaylari );

    int table_row_count = m_ui->table_widget_kullanicilar->rowCount();

    if ( table_row_count NE 0 ) {

        for ( int i = 0; i < table_row_count; i++ ) {

            int kullanici_id = m_ui->table_widget_kullanicilar->item(i,KULLANICI_ID_COLUMN)->text().toInt();
            // KULLANICI ICIN BOS BIR KAYIT ATIYORUZ SON YONETICI YETKI ATAYACAK
            sql_query.PREPARE_INSERT ( "ynt_veritabani_kullanicilari","id","veritabani_id,kullanici_id ");

            sql_query.SET_VALUE      ( ":veritabani_id", veritabani_id );
            sql_query.SET_VALUE      ( ":kullanici_id",  kullanici_id  );
            sql_query.INSERT();
        }
    }

    QList<int> program_ids = DB->GET_DB_PROGRAM_IDS();

    ADAK_SQL * YENI_DB =  CONNECT_DATABASE( GET_PROGRAM_DB_STRUCTS(), veritabani_ismi, GET_SQL_DB_DRIVER_ENUM( SQL_MOTORU() ), false );


    if ( program_ids.size() NE 0 ) {
        YENI_DB->START_TRANSACTION();
        if ( YENI_DB->SQL_CREATE_TABLES( QList<ADAK_SQL_STRUCT *>() , veritabani_ismi ) EQ false ) {
            MSG_ERROR(tr("Programın veritabanı yaratılamadı") , NULL);
            return -1;
        }

        YENI_DB->COMMIT_TRANSACTION();

        for ( int i = 0; i < program_ids.size() ; i++ ) {
            sql_query.PREPARE_INSERT ( "ynt_veritabani_programlari","id","veritabani_id, program_id" );

            sql_query.SET_VALUE      ( ":veritabani_id", veritabani_id     );
            sql_query.SET_VALUE      ( ":program_id"   , program_ids.at(i) );

            sql_query.INSERT();

            QString log_detaylari = "Veritabanı ID: " + QVariant ( veritabani_id ).toString() + " # Veritabanı İsmi: " + m_ui->lineEdit_veritabani_kodu->text()
                                   + " # Veritabanı Tanımı: " + m_ui->lineEdit_veritabani_adi->text() + " # Program Adı: " + ADAK_PROGRAM_LONGNAME (program_ids.at(i)).toLatin1();

            YONETIM_007_KAYIT ( LOG_VERITABANLARI, LOG_PROGRAM_TABLOSU_YARATMA, log_detaylari );
        }
    }

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();

    m_ui->lineEdit_veritabani_kodu->setReadOnly(true);

    splash->finish(this);

    MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE(veritabani_id);

    return veritabani_id;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CHECK_UPDATE
***************************************************************************************/

int VERITABANLARI_FORMU::CHECK_UPDATE (int p_veritabani_id)
{
    Q_UNUSED(p_veritabani_id);

    ADAK_MSG_ENUM answer = MSG_YES_NO_CANCEL(tr("Yapılan değişiklikleri onaylıyor musunuz?"),NULL);
    if ( answer EQ ADAK_NO OR answer EQ ADAK_CANCEL ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::UPDATE_RECORD
***************************************************************************************/

void VERITABANLARI_FORMU::UPDATE_RECORD ( int p_veritabani_id )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    SQL_QUERY insert_query   ( G_YONETIM_DB );

    sql_query.PREPARE_UPDATE ( "ynt_veritabanlari","veritabani_id", "veritabani_ismi,veritabani_tanimi,aciklama","veritabani_id = :veritabani_id ");
    sql_query.SET_VALUE      ( ":veritabani_ismi",   m_ui->lineEdit_veritabani_kodu->text() );
    sql_query.SET_VALUE      ( ":veritabani_tanimi", m_ui->lineEdit_veritabani_adi->text()  );
    sql_query.SET_VALUE      ( ":aciklama",          m_ui->limitedTextEdit_not->toPlainText()    );
    sql_query.SET_VALUE      ( ":veritabani_id"   ,  p_veritabani_id                           );
    sql_query.UPDATE();

    QString veritabani_ismi = QString (SQL_DBNAME_ONEK() + "adak_" + m_ui->lineEdit_veritabani_kodu->text() );

    QString log_detaylari = "Veritabanı ID: " + QVariant( p_veritabani_id ).toString() + " # Veritabanı: " + m_ui->lineEdit_veritabani_kodu->text()
                           + " # Veritabanı Tanımı: " + m_ui->lineEdit_veritabani_adi->text();

    YONETIM_007_KAYIT ( LOG_VERITABANLARI, LOG_KAYIT_GUNCELLEME, log_detaylari );

    sql_query.PREPARE_DELETE ("ynt_veritabani_kullanicilari","veritabani_id = :veritabani_id " );
    sql_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );
    sql_query.DELETE();

    int table_row_count = m_ui->table_widget_kullanicilar->rowCount();

    if ( table_row_count NE 0 ) {

        for ( int i = 0; i < table_row_count; i++ ) {

            int kullanici_id = m_ui->table_widget_kullanicilar->item( i,KULLANICI_ID_COLUMN )->text().toInt();

            sql_query.PREPARE_INSERT ( "ynt_veritabani_kullanicilari","id","veritabani_id,kullanici_id" );

            sql_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );
            sql_query.SET_VALUE      ( ":kullanici_id",  kullanici_id    );
            sql_query.INSERT();
        }
    }

    QList<int> program_ids = DB->GET_DB_PROGRAM_IDS();

    if ( program_ids.size() NE 0 ) {
        for ( int i = 0; i < program_ids.size() ; i++ ) {

            sql_query.PREPARE_SELECT ( "ynt_veritabani_programlari","id","veritabani_id = :veritabani_id AND program_id = :program_id " ,"");

            sql_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id    );
            sql_query.SET_VALUE      ( ":program_id",    program_ids.at(i)  );

            if ( sql_query.SELECT() EQ 0 ) {
                DB->SQL_CREATE_TABLES(DB->GET_SQL_STRUCT(program_ids.at(i)),veritabani_ismi);

                insert_query.PREPARE_INSERT ( "ynt_veritabani_programlari","id","program_id, veritabani_id");

                insert_query.SET_VALUE      ( ":program_id"    , program_ids.at(i));
                insert_query.SET_VALUE      ( ":veritabani_id" , p_veritabani_id  );

                insert_query.INSERT();

                QString log_detaylari = "Veritabanı ID: " + QVariant ( p_veritabani_id ).toString() + " # Veritabanı İsmi: " + m_ui->lineEdit_veritabani_kodu->text()
                                       + " # Veritabanı Tanımı: " + m_ui->lineEdit_veritabani_adi->text() + " # Program Adı: " + ADAK_PROGRAM_LONGNAME (program_ids.at(i)).toLatin1();

                YONETIM_007_KAYIT ( LOG_VERITABANLARI, LOG_PROGRAM_TABLOSU_YARATMA, log_detaylari );
            }
        }
    }

    MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE(p_veritabani_id);

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CHECK_DELETE
***************************************************************************************/

int VERITABANLARI_FORMU::CHECK_DELETE(int p_veritabani_id )
{

    SQL_QUERY query( G_YONETIM_DB );

    query.PREPARE_SELECT( "ynt_veritabani_kullanicilari", "kullanici_id", "veritabani_id = :veritabani_id" );
    query.SET_VALUE     ( ":veritabani_id", p_veritabani_id );

    if ( query.SELECT() NE 0 ) {
        while( query.NEXT() EQ true ) {
            if( query.VALUE(0).toInt() EQ KULLANICI_ID() ) {
                MSG_WARNING( "Kendi Şirketinizi Silemezsiniz!", NULL );
                return ADAK_FAIL;
            }
        }
    }

    ADAK_MSG_ENUM answer = MSG_YES_NO_CANCEL( tr ( "Bu işlem seçili veritabanının silinmesine ve ilgili verilerin tamamen kaybolmasına "
                                  "neden olacaktır. Devam etmek istiyor musunuz?"), NULL);
    if ( answer NE ADAK_YES ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::DELETE_RECORD
***************************************************************************************/

void VERITABANLARI_FORMU::DELETE_RECORD(int p_veritabani_id)
{
    QString veritabani_ismi = QString (SQL_DBNAME_ONEK() + "adak_" + m_ui->lineEdit_veritabani_kodu->text() );

    SQL_QUERY sql_query      ( G_YONETIM_DB ) ;
    SQL_QUERY delete_query   ( G_YONETIM_DB ) ;

    sql_query.PREPARE_SELECT ( "ynt_veritabani_programlari","program_id","veritabani_id = :veritabani_id" );

    sql_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );

    if ( sql_query.SELECT() > 0 ) {

        while ( sql_query.NEXT() EQ true ) {
            int program_id = sql_query.VALUE(0).toInt();
            delete_query.PREPARE_DELETE ( "ynt_veritabani_programlari","program_id = :program_id and veritabani_id = :veritabani_id" );

            delete_query.SET_VALUE      ( ":program_id",    program_id      );
            delete_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );

            delete_query.DELETE();
        }
    }

    delete_query.PREPARE_DELETE ( "ynt_veritabani_kullanicilari","veritabani_id = :veritabani_id" );
    delete_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );
    delete_query.DELETE();

    delete_query.PREPARE_DELETE ( " ynt_veritabanlari","veritabani_id = :veritabani_id " );
    delete_query.SET_VALUE      ( ":veritabani_id", p_veritabani_id );
    delete_query.DELETE();

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();

    ADAK_SQL * YENI_DB =  CONNECT_DATABASE( GET_PROGRAM_DB_STRUCTS(), veritabani_ismi, GET_SQL_DB_DRIVER_ENUM( SQL_MOTORU() ), false );

    if ( YENI_DB EQ NULL ) {
        return;
    }

    YENI_DB->START_TRANSACTION();

    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
    splash->show();
    splash->showMessage((QString("%1 Veritabanı siliniyor. Lütfen Bekleyiniz...").arg( veritabani_ismi )), Qt::AlignCenter, Qt::white );
    qApp->processEvents();
    YENI_DB->SQL_DROP_TABLES( GET_PROGRAM_DB_STRUCTS() );

    splash->finish(NULL);
    YENI_DB->COMMIT_TRANSACTION();

    QString log_detaylari = "Veritabanı ID: " + QVariant( p_veritabani_id).toString() + " # Veritabanı: " + veritabani_ismi
                           + " # Veritabanı Tanımı: " + m_ui->lineEdit_veritabani_adi->text();

    YONETIM_007_KAYIT ( LOG_VERITABANLARI, LOG_VERITABANI_SILME, log_detaylari );

}

/**************************************************************************************
                   VERITABANLARI_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","","" );

    if ( sql_query.SELECT("veritabani_ismi",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","","" );

    if ( sql_query.SELECT("veritabani_ismi DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::FIND_NEXT_RECORD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","veritabani_ismi > :veritabani_ismi","" );
    sql_query.SET_VALUE      ( ":veritabani_ismi", m_ui->lineEdit_veritabani_kodu->text() );

    if ( sql_query.SELECT("veritabani_ismi",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::FIND_PREV_RECORD ( )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","veritabani_ismi < :veritabani_ismi","" );
    sql_query.SET_VALUE      ( ":veritabani_ismi", m_ui->lineEdit_veritabani_kodu->text() );

    if ( sql_query.SELECT("veritabani_ismi DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::FIND_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::FIND_RECORD()
{
    if (m_ui->lineEdit_veritabani_kodu->text().isEmpty() EQ true ) {
        return 0;
    }
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id","veritabani_ismi = :veritabani_ismi","" );
    sql_query.SET_VALUE      ( ":veritabani_ismi", m_ui->lineEdit_veritabani_kodu->text() );

    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   VERITABANLARI_FORMU::SELECT_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::SELECT_RECORD()
{
    return VERITABANI_SEC ( -1, NULL, NULL, this );
}

/**************************************************************************************
                   VERITABANLARI_FORMU::LOCK_RECORD
***************************************************************************************/

int VERITABANLARI_FORMU::LOCK_RECORD ( int p_veritabani_id )
{
    return G_YONETIM_DB->LOCK_ROW ( "veritabani_id", "ynt_veritabanlari", QString ( "veritabani_id = %1" ).arg ( p_veritabani_id ) );
}

/**************************************************************************************
                   VERITABANLARI_FORMU::UNLOCK_RECORD
***************************************************************************************/

void VERITABANLARI_FORMU::UNLOCK_RECORD( int p_veritabani_id  )
{
    G_YONETIM_DB->UNLOCK_ROW ( "veritabani_id", "ynt_veritabanlari", QString ( "veritabani_id = %1" ).arg ( p_veritabani_id ) );
}

/**************************************************************************************
                   VERITABANLARI_FORMU::LISTEYE_KULLANICI_EKLE
***************************************************************************************/

int VERITABANLARI_FORMU::LISTEYE_KULLANICI_EKLE()
{
    p_secilen_kullanici_id = KULLANICI_SEC ( false, this );

    if ( p_secilen_kullanici_id  < 1 ) {
        return ADAK_RECORD_UNCHANGED;
    }

    for ( int i = 0; i < m_ui->table_widget_kullanicilar->rowCount(); i++ ) {

        int current_kullanici_id = m_ui->table_widget_kullanicilar->item(i,KULLANICI_ID_COLUMN)->text().toInt();

        if ( current_kullanici_id NE p_secilen_kullanici_id ) {
           continue;
        }

        MSG_INFO(tr("Seçilen kullanıcı %n nolu satırda zaten mevcut.","",i+1) , NULL);
        return ADAK_RECORD_UNCHANGED;

    }

    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_kodu, kullanici_adi","kullanici_id = :kullanici_id " ,"");
    sql_query.SET_VALUE      ( ":kullanici_id" , p_secilen_kullanici_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_RECORD_UNCHANGED;
    }
    sql_query.NEXT();

    int new_row_num                     = m_ui->table_widget_kullanicilar->rowCount();

    m_ui->table_widget_kullanicilar->insertRow ( new_row_num );
    m_ui->table_widget_kullanicilar->setItem   ( new_row_num, KULLANICI_ID_COLUMN  , new QTableWidgetItem ( QVariant(p_secilen_kullanici_id).toString() ) );
    m_ui->table_widget_kullanicilar->setItem   ( new_row_num, KULLANICI_KODU_COLUMN , new QTableWidgetItem (sql_query.VALUE(0).toString()) );
    m_ui->table_widget_kullanicilar->setItem   ( new_row_num, KULLANICI_ADI_COLUMN , new QTableWidgetItem (sql_query.VALUE(1).toString()) );

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::KULLANICILAR_LISTESINDEN_SATIR_SIL
***************************************************************************************/

int VERITABANLARI_FORMU::KULLANICILAR_LISTESINDEN_SATIR_SIL( int p_clicked_row, int p_clicked_column )
{
    if ( p_clicked_row < 0 OR p_clicked_column < 0 ) {
        return ADAK_FAIL;
    }

    m_ui->table_widget_kullanicilar->removeRow(p_clicked_row);
    return ADAK_OK;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int VERITABANLARI_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( p_button EQ m_ui->button_kullanici_sec ) {
        return LISTEYE_KULLANICI_EKLE();
    }

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   VERITABANLARI_FORMU::MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE
***************************************************************************************/

void VERITABANLARI_FORMU::MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE(int veritabani_id)
{
    QList<int> mevcut_veritabanlari_ids;
    QList<int> program_ids = DB->GET_DB_PROGRAM_IDS();

    if ( veritabani_id NE -1 ) {
        SQL_QUERY sql_query (G_YONETIM_DB);

        sql_query.PREPARE_SELECT ( "ynt_veritabani_programlari","program_id","veritabani_id = :veritabani_id");

        sql_query.SET_VALUE      ( ":veritabani_id", veritabani_id );

        if ( sql_query.SELECT() > 0 ) {
            while ( sql_query.NEXT() EQ true ) {
                mevcut_veritabanlari_ids << sql_query.VALUE(0).toInt();
            }
        }
    }

    m_ui->text_edit_mevcut_veritabanlari->clear();

    if ( mevcut_veritabanlari_ids.size() EQ 0 ) {
        m_ui->label_mevcut_veritabanlari->hide();
        m_ui->text_edit_mevcut_veritabanlari->hide();
    }
    else {
        m_ui->label_mevcut_veritabanlari->setVisible(true);
        m_ui->text_edit_mevcut_veritabanlari->setVisible(true);

        QString text = "";

        for ( int i = 0 ; i < mevcut_veritabanlari_ids.size() ; i++ ) {
            program_ids.removeOne(mevcut_veritabanlari_ids.at(i));

            text.append(ADAK_PROGRAM_LONGNAME(mevcut_veritabanlari_ids.at(i)) + "<br>");

        }

        m_ui->text_edit_mevcut_veritabanlari->setHtml(text);
    }

    m_ui->text_edit_yaratilacak_veritabanlari->clear();

    if ( program_ids.size() EQ 0 ) {
        m_ui->label_yaratilacak_veritabanlari->hide();
        m_ui->text_edit_yaratilacak_veritabanlari->hide();
        //m_ui->check_box_veritabanlarini_guncelle->hide();
    }
    else {
        m_ui->label_yaratilacak_veritabanlari->setVisible(true);
        m_ui->text_edit_yaratilacak_veritabanlari->setVisible(true);
        //m_ui->check_box_veritabanlarini_guncelle->setVisible(true);

        QString text = "";

        for ( int i = 0 ; i < program_ids.size() ; i++ ) {
            text.append(ADAK_PROGRAM_LONGNAME(program_ids.at(i)) + "<br>");
        }

        m_ui->text_edit_yaratilacak_veritabanlari->setHtml(text);
    }
}
