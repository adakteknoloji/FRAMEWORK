#include <QSettings>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTimer>
#include <QByteArray>
#include "adak_sql.h"
#include "login_batch.h"
#include "program_start.h"
#include "login.h"
#include "programi_guncellestirin_batch.h"
#include "ui_login_batch.h"
#include "login_utils.h"
#include "login_enum.h"
#include "login.h"

extern ADAK_SQL *G_YONETIM_DB;
extern ADAK_SQL *DB;

/**************************************************************************************
                       OPEN_LOGIN_BATCH
***************************************************************************************/

bool OPEN_LOGIN_BATCH ( USER_LOGIN_INFO_STRUCT * user_informations, QWidget * parent)
{
    bool all_informations_completed = false;

    LOGIN_BATCH * B = new LOGIN_BATCH (user_informations, &all_informations_completed ,parent);
    B->EXEC( NOT_FULL_SCREEN );

    return all_informations_completed;
}

/**************************************************************************************
                   LOGIN_BATCH::LOGIN_BATCH
***************************************************************************************/

LOGIN_BATCH::LOGIN_BATCH ( USER_LOGIN_INFO_STRUCT * user_informations,bool * all_informations_completed, QWidget * parent) : BATCH_KERNEL (parent), m_ui( new Ui::LOGIN_BATCH )
{
    m_user_info                     = user_informations;
    m_all_informations_completed    = all_informations_completed;

    m_ui->setupUi( this );

    START_BATCH_KERNEL( this, NULL );
}

/**************************************************************************************
                 LOGIN_BATCH::SETUP_FORM
***************************************************************************************/

void LOGIN_BATCH::SETUP_FORM ()
{
    SET_ENTER_KEY_FOR_RUN_BATCH  ( true );
    // translate SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Giriş" ) );
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr ( "Enter" ) );
    REGISTER_BUTTON_WIDGET       ( m_ui->widget_batch_buttons );

    // translate SET_PAGE_TITLE( ADAK_PROGRAM_SHORTNAME( ADAK_DISPLAY_ID() ) + tr(" Giriş Ekranı V") + ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()));
    SET_PAGE_TITLE( ADAK_PROGRAM_SHORTNAME( ADAK_DISPLAY_ID() ) + tr("Login Screen V") + ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()));
    SET_SETTING_NAME    ( "LOGIN_BATCH");
    SET_AUTO_EXIT_BATCH ( true );
    SET_HELP_PAGE       ( "E9_giris");

    SET_FIRST_FOCUS_WIDGET( m_ui->line_edit_kullanici_kodu );

    SET_WIDGETS_MAX_LENGTH();

    m_ui->text_edit_status->setReadOnly(true);

    if ( m_user_info->bilgisayar_id EQ -1 ) {
        m_user_info->kullanici_kodu.clear();
    }

    FILL_SQL_PROFILLERI  ();

    SECILI_PROFILI_YUKLE( m_user_info->secili_sql_profili );

    //////////////VERSIYON KONTROLU

    // HER PROGRAMIN KENDI LOGOSU SETLENIYOR

    switch ( ADAK_DISPLAY_ID() ) {

        case E9_PROGRAMI:
            m_ui->label_logo->setPixmap( QPixmap( QString::fromUtf8(":/e9_icons/E9_ICONS/e9_logo.png") ));
            break;
        default:
            m_ui->label_logo->setPixmap( QPixmap( QString::fromUtf8(":/yonetim_icons/adak_logo.png") ));
            break;
    }
    m_ui->label_program_adi->setText( ADAK_PROGRAM_LONGNAME (ADAK_DISPLAY_ID()));

    QSettings settings ( "ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID())) ;

    //QString check_date               = settings.value("last_program_update_check_date","").toString();
    QString not_check_update_version = settings.value("not_check_update_version","").toString();

    if ( ( not_check_update_version.isEmpty() EQ true OR not_check_update_version NE ADAK_PROGRAM_VERSION( ADAK_DISPLAY_ID() ) ) ) {

        const QString html = QVariant ("http://www.adak.com.tr/DOWNLOADS/E9/version.txt").toByteArray();

        QNetworkAccessManager * network_access_manager = new QNetworkAccessManager();
        QUrl url(html);
        QNetworkReply * network_reply = network_access_manager->get ( QNetworkRequest ( url ) );
        QEventLoop answer_loop;
        answer_loop.connect ( network_reply, SIGNAL ( finished() ), &answer_loop, SLOT ( quit() ) );
        QTimer bekleme_suresi;
        answer_loop.connect ( &bekleme_suresi, SIGNAL ( timeout() ), &answer_loop, SLOT ( quit() ) );
        bekleme_suresi.start ( 1000 );
        answer_loop.exec();
        QByteArray result = network_reply->readAll();

        if ( network_reply->error() != QNetworkReply::NoError ) {
            return;
        }

        QStringList program_list = QVariant(result).toString().split("\n",QString::SkipEmptyParts,Qt::CaseInsensitive);
        if (program_list.size() NE 0) {
            int index_of_program = -1;

            for ( int i = 0 ; i < program_list.size() ; i++ ) {
                if ( program_list.at(i).contains(QString("%1-").arg(ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID())),Qt::CaseInsensitive) EQ true ) {
                    index_of_program = i;
                    break;
                }
            }
            if ( index_of_program EQ - 1) {
                return;
            }

            QStringList program_info = program_list.at(index_of_program).split("-",QString::SkipEmptyParts,Qt::CaseInsensitive);
            if ( program_info.size() EQ 3 ) {
                if ( ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()) < program_info.at(1) ) {
                    OPEN_PROGRAMI_GUNCELLESTIRIN_BATCH(program_info.at(2),program_info.at(1),NULL);
                }
            }
        }

        settings.setValue("last_program_update_check_date",QDate::currentDate().toString("yyyy.MM.dd"));
    }

    //////////////////////////////////////////////

}

/**************************************************************************************
                   LOGIN_BATCH::CHECK_VAR
***************************************************************************************/

int LOGIN_BATCH::CHECK_VAR(QObject * object)
{
    if ( object EQ m_ui->push_button_sql_ayarlari OR object EQ m_ui->combo_box_sql_profilleri  ) {

        bool yeni_profil_mi = false;

        if( m_ui->combo_box_sql_profilleri->currentText() EQ tr("New Server") ) {
            yeni_profil_mi = true;
        }
        else if( object EQ m_ui->combo_box_sql_profilleri ) {
           SECILI_PROFILI_YUKLE( m_ui->combo_box_sql_profilleri->currentText() );
           return ADAK_OK;
        }
        QString onceki_profil = m_user_info->secili_sql_profili;
        if ( OPEN_YENI_SUNUCU_TANIMLAMA_BATCH( yeni_profil_mi, m_user_info, this) EQ false ) {
            CLOSE_ALL_EVENTS();
            SECILI_PROFILI_YUKLE( onceki_profil );
            OPEN_ALL_EVENTS();
            return ADAK_FAIL;
        }

        CLOSE_ALL_EVENTS();
        FILL_SQL_PROFILLERI();
        SECILI_PROFILI_YUKLE( m_user_info->secili_sql_profili);
        OPEN_ALL_EVENTS();

    }
    else if ( object EQ m_ui->line_edit_kullanici_kodu) {

        if ( m_ui->line_edit_kullanici_kodu->text().toLower() EQ QObject::tr( "Administrator") ) {
            m_ui->line_edit_kullanici_kodu->setText( tr("administrator") );
        }

        if ( m_user_info->is_first_login EQ false ) {

            m_user_info->kullanici_kodu = m_ui->line_edit_kullanici_kodu->text();

            if ( SET_KULLANICI_INFO( m_user_info ) EQ ADAK_FAIL ) {
                return ADAK_FAIL;
            }

            SET_VERITABANI_INFO( m_user_info );
        }
        else {
            m_user_info->kullanici_kodu = m_ui->line_edit_kullanici_kodu->text();
        }
        REORGANIZE_SCREEN();
    }
    else if ( object EQ m_ui->check_box_sql_tablolarini_guncelle ) {
        m_user_info->sql_tablolari_guncellestir= m_ui->check_box_sql_tablolarini_guncelle->isChecked();
    }
    else if ( object EQ m_ui->check_box_otomatik_login ) {
        QSettings settings ( "ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));
        if ( m_ui->check_box_otomatik_login->isChecked() EQ true ) {
            settings.setValue( "is_auto_login", true );
        }
        else {
            settings.setValue( "is_auto_login", true );
        }
    }
    else if ( object EQ m_ui->line_edit_kullanici_sifre ) {
         m_user_info->kullanici_sifre = m_ui->line_edit_kullanici_sifre->text();
    }

    return ADAK_OK;
}

/**************************************************************************************
                   LOGIN_BATCH::CHECK_RUN
***************************************************************************************/

int LOGIN_BATCH::CHECK_RUN()
{
    if ( IS_DB_CONNECTED(ADAK_YONETIM) EQ ADAK_FAIL ) {
        if ( m_ui->line_edit_kullanici_kodu->text().isEmpty() EQ true ) {
           MSG_ERROR(tr("User code has not been entered") , m_ui->line_edit_kullanici_kodu );
           return ADAK_FAIL;
        }
        if ( m_ui->line_edit_kullanici_kodu->text() EQ tr("administrator") ) {
            MSG_ERROR(tr("'administrator' is a specific user code for the system. Please enter a different user code."), m_ui->line_edit_kullanici_kodu );
            return ADAK_FAIL;
        }
    }
    if ( m_ui->line_edit_kullanici_kodu->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("User code has not been entered") , m_ui->line_edit_kullanici_kodu );
        return ADAK_FAIL;
    }

    if ( m_ui->line_edit_kullanici_sifre->text().isEmpty() EQ false ) {
        if ( SET_KULLANICI_PAROLA( m_user_info ) EQ ADAK_FAIL ) {
            SET_FOCUS( m_ui->line_edit_kullanici_sifre );
            return ADAK_FAIL;
        }
        m_user_info->kullanici_sifre = m_ui->line_edit_kullanici_sifre->text();
    }

    if ( CHECK_KULLANICI_BILGISAYARA_KAYITLI_MI( m_user_info->bilgisayar_id, m_user_info->kullanici_id ) EQ ADAK_FAIL ) {
        return ADAK_FAIL;
    }

    SET_USER_LOGIN_STATUS( USER_ON_LOGIN_SCREEN );
    if ( CHECK_USER_ACCESS( m_user_info ) EQ ADAK_FAIL ) {
        SET_FOCUS( m_ui->line_edit_kullanici_sifre );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   LOGIN_BATCH::RUN_BATCH
***************************************************************************************/

void LOGIN_BATCH::RUN_BATCH()
{
    if( G_YONETIM_DB->SQL_TABLES_EXISTS("") EQ true  ) {
        int user_db_count = GET_USER_DB_COUNT( m_user_info->kullanici_id );

        if ( user_db_count > 1 ) {
            int veritabani_secimi = VERITABANI_SEC( m_user_info->kullanici_id , &(m_user_info->veritabani_kodu)
                                                                      , &(m_user_info->veritabani_tanimi),this );
            if ( veritabani_secimi EQ -1 ) {
                return;
            }
        }
        else if (user_db_count EQ 1 ) {
            m_user_info->veritabani_kodu = GET_VERITABANI_ISMI( m_user_info );
        }
    }
    else {
        int secim = MSG_YES_NO( tr("No tables in the database. Are tables created?"), NULL );
        if ( secim EQ ADAK_YES ) {
            m_user_info->is_first_login = true;
        }
    }

    if ( m_user_info->veritabani_kodu.isEmpty() EQ false ) {

        if ( m_user_info->sql_motoru EQ "NONE" ) {
            if ( PROGRAM_DB_VAR_MI( m_user_info->veritabani_kodu ) EQ false ) {
                MSG_WARNING( tr("No Database!"), NULL );
                return;
            }
            else {
                CONNECT_TO_DB( ADAK_PROGRAM, m_user_info, false );
            }
        }
        else {
            DB = CREATE_ADAK_SQL_CONNECTION( GET_PROGRAM_DB_STRUCTS(), QString(SQL_DBNAME_ONEK()+ "adak_" + m_user_info->veritabani_kodu ),
                                              GET_SQL_DB_DRIVER_ENUM( m_user_info->sql_motoru ) );

            if( DB->CONNECT_TO_DATABASE( false ) EQ ADAK_FAIL ) {
                 MSG_WARNING( tr("No Database!"), NULL );
                return;
            }
        }

        if( DB->SQL_TABLES_EXISTS("") EQ false ) {
            int secim = MSG_YES_NO( tr("No tables in the database. Are tables created?"), NULL );
            if ( secim EQ ADAK_YES ) {
               // m_user_info->is_first_login = true;
            }
        }

    }

    m_user_info->kullanici_kodu    = m_ui->line_edit_kullanici_kodu->text();
    m_user_info->kullanici_sifre  = m_ui->line_edit_kullanici_sifre->text();

    *m_all_informations_completed = true;

    m_user_info->is_auto_login = m_ui->check_box_otomatik_login->isChecked();



}

/**************************************************************************************
                   LOGIN_BATCH::KULLANICI_SIFRESI_DEGISTIR
***************************************************************************************/

void LOGIN_BATCH::KULLANICI_SIFRESI_DEGISTIR ()
{
    if ( IS_DB_CONNECTED(ADAK_YONETIM) EQ false ) {
        return;
    }

    if ( SET_KULLANICI_INFO( m_user_info ) EQ ADAK_FAIL ) {
        return;
    }

    if( SET_KULLANICI_PAROLA( m_user_info ) EQ ADAK_FAIL ) {
        return;
    }
}

/**************************************************************************************
                 LOGIN_BATCH::SET_WIDGETS_MAX_LENGTH
***************************************************************************************/

void LOGIN_BATCH::SET_WIDGETS_MAX_LENGTH()
{
    m_ui->line_edit_kullanici_kodu  ->setMaxLength          (10);
    m_ui->line_edit_kullanici_sifre->setMaxLength          (50);

}

/**************************************************************************************
                   LOGIN_BATCH::REORGANIZE_ALL_VALUES
***************************************************************************************/

void LOGIN_BATCH::REORGANIZE_SCREEN ()
{
    m_ui->frame_bilgisayar_bilgileri->show();
    m_ui->lineEdit_bilgisayar_adi->clear();

    m_ui->frame_bilgisayar_bilgileri->setVisible( true );

    m_ui->line_edit_kullanici_adi->clear();
    m_ui->line_edit_kullanici_kodu->clear();
    m_ui->line_edit_kullanici_sifre->clear();

    if ( m_user_info->kullanici_veritabaninda_calisabilir EQ false ) {
        m_ui->check_box_sql_tablolarini_guncelle ->hide();
    }

    if ( m_user_info->is_first_login EQ true ) {
        m_ui->line_edit_kullanici_adi->setReadOnly(false);
    }
    else {
        m_ui->line_edit_kullanici_adi->setReadOnly(true);
    }

    m_ui->text_edit_status->setHtml( GET_DB_CONNECTION_STATUS() );

    QString bilgisayar_adi  = m_user_info->bilgisayar_adi;
    QString kullanici_kodu  = m_user_info->kullanici_kodu;

    FILL_COMPUTER_WIDGETS(bilgisayar_adi );
    FILL_USER_WIDGETS    (kullanici_kodu );
}

/**************************************************************************************
                   LOGIN_BATCH::FILL_SQL_PROFILLERI
***************************************************************************************/

void LOGIN_BATCH::FILL_SQL_PROFILLERI()
{
    QSettings settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));
    QString profil_str    = settings.value("kayitli_sql_sunuculari","").toString();
    QStringList profiller = profil_str.split("-");

    profiller << tr("New Server");

    m_ui->combo_box_sql_profilleri->clear();
    m_ui->combo_box_sql_profilleri->addItems(profiller);
}

/**************************************************************************************
                   LOGIN_BATCH::SECILI_PROFILI_YUKLE
***************************************************************************************/

void LOGIN_BATCH::SECILI_PROFILI_YUKLE (QString profil)
{

    QStringList profiller;

    for ( int i = 0 ; i < m_ui->combo_box_sql_profilleri->count() ; i++ ) {
         profiller << m_ui->combo_box_sql_profilleri->itemText(i);
    }

    if ( profil.isEmpty() EQ true ) {
        if ( m_user_info->secili_sql_profili.isEmpty() EQ true ) {
            if (profiller.size() >= 2) {
                m_ui->combo_box_sql_profilleri->setCurrentIndex(profiller.size() - 2);
            }
        }
        else {
            m_ui->combo_box_sql_profilleri->setCurrentIndex(m_ui->combo_box_sql_profilleri->findText(m_user_info->secili_sql_profili));
        }
    }
    else {
        m_ui->combo_box_sql_profilleri->setCurrentIndex(m_ui->combo_box_sql_profilleri->findText(profil));
    }

    QString kayitli_profil_ismi = m_ui->combo_box_sql_profilleri->currentText();

    READ_SETTINGS( m_user_info, kayitli_profil_ismi );

    QSettings settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));
    settings.setValue( "secili_kullanici_sunucusu", kayitli_profil_ismi );

    RESET_PROGRAM_CORE( m_user_info );

    m_ui->line_edit_kullanici_kodu->setText(m_user_info->kullanici_kodu);
    m_ui->line_edit_kullanici_adi->setText(m_user_info->kullanici_adi);

    if ( m_user_info->secili_sql_profili EQ tr("New Server") ) {
        if ( OPEN_YENI_SUNUCU_TANIMLAMA_BATCH( true, m_user_info, this ) EQ true ) {

            FILL_SQL_PROFILLERI();

            m_ui->text_edit_status->clear();
            //! baglanti status try_connect_db'de de var
            QString baglanti_status;

            baglanti_status.append(QObject::tr("Connected Server"));

            QString status_str = QString(tr("IP/Adress        : <b>%1</b><br>Port: <b>%2</b><br>SQL Engine: <b>%3</b><br>"));

            if (GET_SQL_DB_DRIVER_ENUM( m_user_info->sql_motoru) EQ SQLITE) {
                status_str = QString(tr("SQL Engine      : <b>SQLite</b><br>"));
            }
            else {
                status_str = QString(status_str)
                             .arg(m_user_info->sql_sunucu)
                             .arg(m_user_info->sql_port)
                             .arg(m_user_info->sql_motoru);
            }

            status_str.append( baglanti_status );
            SET_DB_CONNECTION_STATUS( status_str );

            //! End baglanti
        }
    }

    if ( m_user_info->secili_sql_profili NE kayitli_profil_ismi ) {
         m_ui->combo_box_sql_profilleri->setCurrentIndex(m_ui->combo_box_sql_profilleri->findText( m_user_info->secili_sql_profili ));
    }

    WRITE_SETTINGS( m_user_info );

    REORGANIZE_SCREEN();
}

/**************************************************************************************
                   LOGIN_BATCH::REORGANIZE_ALL_VALUES
***************************************************************************************/

void LOGIN_BATCH::FILL_COMPUTER_WIDGETS(QString bilgisayar_adi)
{
    m_ui->lineEdit_bilgisayar_adi ->setText( m_user_info->bilgisayar_kodu + " " + bilgisayar_adi);

    m_ui->text_edit_status->setText( GET_DB_CONNECTION_STATUS());
}

/**************************************************************************************
                   LOGIN_BATCH::FILL_USER_WIDGETS
***************************************************************************************/

void LOGIN_BATCH::FILL_USER_WIDGETS(QString kullanici_kodu)
{
    if ( kullanici_kodu.isEmpty() EQ false ) {
        m_ui->line_edit_kullanici_kodu->setText(kullanici_kodu);
        m_ui->line_edit_kullanici_adi ->setText(m_user_info->kullanici_adi);
        m_ui->line_edit_kullanici_sifre->setText(m_user_info->kullanici_sifre);
    }
}
