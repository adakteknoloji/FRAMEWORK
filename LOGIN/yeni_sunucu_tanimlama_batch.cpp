#include <QSettings>
#include "yeni_sunucu_tanimlama_batch.h"
#include "ui_yeni_sunucu_tanimlama_batch.h"
#include "print.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "program_start.h"
#include "login_utils.h"
#include "login_enum.h"
#include "login.h"
#include "first_login.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_YENI_SUNUCU_TANIMLAMA_BATCH
***************************************************************************************/

bool OPEN_YENI_SUNUCU_TANIMLAMA_BATCH (bool yeni_profil , USER_LOGIN_INFO_STRUCT * user_informations, QWidget * parent )
{
    bool run_batch_clicked = false;

    YENI_SUNUCU_TANIMLAMA_BATCH * F = new YENI_SUNUCU_TANIMLAMA_BATCH ( yeni_profil , user_informations , &run_batch_clicked, parent );
    F->EXEC (NOT_FULL_SCREEN);

    return run_batch_clicked;
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::YENI_SUNUCU_TANIMLAMA_BATCH
***************************************************************************************/

YENI_SUNUCU_TANIMLAMA_BATCH::YENI_SUNUCU_TANIMLAMA_BATCH (bool yeni_profil , USER_LOGIN_INFO_STRUCT * user_informations, bool * run_batch_clicked, QWidget * parent )
                                        :  BATCH_KERNEL ( parent ), m_ui ( new Ui::YENI_SUNUCU_TANIMLAMA_BATCH )
{

    m_user_info         = user_informations;
    m_run_batch_click   = run_batch_clicked;
    m_yeni_profil       = yeni_profil;

    m_kayitli_profil_ismi.clear();

    m_ui->setupUi( this );
    START_BATCH_KERNEL(this, DB );
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::SETUP_FORM
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON( tr("Connect") );

    SET_FIRST_FOCUS_WIDGET( m_ui->line_edit_profil_ismi );

    REGISTER_BUTTON_WIDGET( m_ui->widget );

    SET_ENTER_KEY_FOR_RUN_BATCH( true );
    SET_AUTO_EXIT_BATCH ( true );

    m_kayitli_profil_ismi = m_user_info->secili_sql_profili;

    if( m_yeni_profil EQ true ) {
        CLEAR_SETTINGS( m_user_info );
        m_kayitli_profil_ismi.clear();
        m_ui->tool_button_kayitli_profili_sil->setVisible( false );
    }

    if( m_user_info->secili_sql_profili EQ "Yerel Disk" ) {
        m_ui->tool_button_kayitli_profili_sil->setVisible( false );
    }

    QString     mac_adresi      = "";
    QStringList mac_adressleri  = GET_MAC_ADDRESS();

    for ( int i = 0 ; i < mac_adressleri.size() ; i++ ) {
        mac_adresi.append( QString(mac_adressleri.at(0) ) );
        break;
    }

    m_argumanli_label_txt = m_ui->label_sql_uyarisi->text();

    m_ui->line_edit_mac_adresi->setText ( mac_adresi);
    m_ui->lineEdit_dongle_ip->setText   ( GET_DONGLE_IP() );

    m_ui->tool_button_kayitli_profili_sil->setIcon(QIcon(":/kernel_icons/kernel_sil_ikonu.png"));
    m_ui->tool_button_kayitli_profili_sil->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_ui->tool_button_kayitli_profili_sil->setIconSize(QSize(32,32));

    SET_PAGE_TITLE   (tr("NEW SERVER DEFINITION"));
    SET_SETTING_NAME ("YENİ_SUNUCU_TANIMLAMA");
    SET_HELP_PAGE    ("yeni-sunucu-tanimlama");

    m_ui->combobox_sql_motoru->addItems(GET_SQL_DRIVER_LIST());

    m_ui->lineEdit_sql_kul_adi->setMaxLength(50);
    m_ui->lineEdit_sql_kul_sifresi->setMaxLength(50);
    m_ui->lineEdit_sql_port->setMaxLength(10);
    m_ui->lineEdit_sunucu->setMaxLength(100);
    m_ui->lineEdit_veritabani_onek->setMaxLength(20);

    if ( m_user_info->sql_motoru.isEmpty() EQ true ) {
        m_user_info->sql_motoru = "NONE";
    }

    m_ui->lineEdit_veritabani_kodu->setText( "firma_veri_tabani" );

    if ( m_user_info->secili_sql_profili.isEmpty() EQ true OR m_yeni_profil EQ true ) {
        m_user_info->sql_motoru = "NONE";
        m_user_info->sql_sunucu = "localhost";
        FILL_SELECTED_SQL( m_user_info->sql_motoru );
    }

    if ( m_user_info->sql_port NE -1 ) {
        m_ui->lineEdit_sql_port->setText (QVariant( m_user_info->sql_port ).toString());
    }

    m_ui->line_edit_profil_ismi->setText    ( m_user_info->secili_sql_profili );
    m_ui->lineEdit_sunucu->setText          ( m_user_info->sql_sunucu      );
    m_ui->lineEdit_sql_kul_adi->setText     ( m_user_info->sql_username    );
    m_ui->lineEdit_sql_kul_sifresi->setText ( m_user_info->sql_password    );
    m_ui->lineEdit_veritabani_onek->setText ( m_user_info->sql_db_on_ek    );
    m_ui->lineEdit_veritabani_kodu->setText ( m_user_info->veritabani_kodu );

    m_ui->combobox_sql_motoru->setCurrentIndex (m_ui->combobox_sql_motoru->findText( m_user_info->sql_motoru ));

    SET_UYARI_LABEL();

    if ( m_user_info->is_auto_sifre EQ true ) {
        m_ui->lineEdit_sql_kul_sifresi->setEnabled( false );
        m_ui->checkBox_otomatik_sifre->setChecked( true );

        m_ui->lineEdit_sql_kul_sifresi->clear();
    }

    SQL_UYARISINI_GUNCELLE(GET_SQL_DB_DRIVER_ENUM(m_user_info->sql_motoru));

    ORGANIZE_SQL_WIDGETS();
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::CHECK_VAR
***************************************************************************************/

int YENI_SUNUCU_TANIMLAMA_BATCH::CHECK_VAR ( QObject * object )
{
    QRegExp letter("(\\W+)");

    if ( object EQ m_ui->lineEdit_sql_port ) {
        QRegExp non_digit("(\\D+)");
        if (m_ui->lineEdit_sql_port->text().contains(non_digit) EQ true ) {
            MSG_ERROR(tr("The port number can only consist of numeric characters.") , NULL);
            return ADAK_FAIL;//Port numarası sadece nümerik karakterlerden oluşabilir
        }
    }
    else if ( object EQ m_ui->combobox_sql_motoru ) {
        ORGANIZE_SQL_WIDGETS();
        m_ui->lineEdit_sql_port->setText(QVariant( GET_DEFAULT_SQL_PORT_NUMBER(GET_SQL_DB_DRIVER_ENUM(m_ui->combobox_sql_motoru->currentText()))).toString());
        SQL_UYARISINI_GUNCELLE( GET_SQL_DB_DRIVER_ENUM(m_ui->combobox_sql_motoru->currentText()));
    }
    else if ( object EQ m_ui->lineEdit_veritabani_onek ) {
        SET_UYARI_LABEL();
    }
    else if ( object EQ m_ui->tool_button_kayitli_profili_sil ) {
        KAYITLI_PROFILI_SIL(true);
    }
    else if( object EQ m_ui->lineEdit_veritabani_kodu ) {
        if ( m_ui->lineEdit_sql_kul_adi->text().contains(letter) EQ true ) {
            MSG_ERROR(QObject::tr("Database name numbers, letters or \'_\' can not contain characters outside.") , m_ui->lineEdit_veritabani_kodu );
            return ADAK_FAIL_UNDO;//Veritabanı ismi rakam,harf veya \'_\' dışında karakter içeremez.
        }
        SET_UYARI_LABEL();
    }
    else if ( object EQ m_ui->checkBox_otomatik_sifre ) {
        if ( m_ui->checkBox_otomatik_sifre->isChecked() EQ true ) {
            m_ui->lineEdit_sql_kul_sifresi->setEnabled( false );
            m_ui->lineEdit_sql_kul_sifresi->clear();
        }
        else {
            m_ui->lineEdit_sql_kul_sifresi->setEnabled( true );
        }
    }
    return ADAK_OK;

}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::CHECK_RUN
***************************************************************************************/

int YENI_SUNUCU_TANIMLAMA_BATCH::CHECK_RUN ()
{
    if ( m_ui->combobox_sql_motoru->currentText() NE "NONE" ) {
        if (m_ui->lineEdit_sql_port->text().isEmpty() EQ true ) {
            MSG_ERROR(tr("SQL engine port information can not be empty\n") , m_ui->lineEdit_sql_port);//Sql motoru port bilgisi boş bırakılamaz\n
            m_ui->lineEdit_sql_port->setText(QVariant(GET_DEFAULT_SQL_PORT_NUMBER(GET_SQL_DB_DRIVER_ENUM(m_ui->combobox_sql_motoru->currentText()))).toString());
            return ADAK_FAIL;
        }
    }
    if (m_ui->line_edit_profil_ismi->text().isEmpty() EQ true) {
        MSG_ERROR(tr("SQL profile name can not be empty\nYou must define a suitable profile name of your SQL settings.") , m_ui->line_edit_profil_ismi);
        return ADAK_FAIL;
    }//SQL profil ismi boş bırakılamaz.\nSql ayarlarınıza uygun bir profil ismi tanımlamanız gerekmektedir.

    OTOMATIK_SIFRE_KONTROLU();

    QString hata_mesaji="";
    *m_run_batch_click = CONNECT_TO_SQL_DATABASE( m_user_info->sql_sunucu ,
                                                  m_ui->lineEdit_sql_port->text().toInt(),
                                                  m_ui->combobox_sql_motoru->currentText(),
                                                  m_ui->lineEdit_veritabani_onek->text(),
                                                  m_user_info->sql_username,
                                                  m_user_info->sql_password,
                                                  m_ui->lineEdit_veritabani_kodu->text() );

    if ( *m_run_batch_click EQ false ) {
        MSG_ERROR( QString(tr("Connection Failed. Did you create databases ? \nHata:%1")).arg( hata_mesaji ) , NULL);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::RUN_BATCH
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::RUN_BATCH ()
{
    KAYITLI_PROFILI_SIL();

    QString sql_profili = m_ui->line_edit_profil_ismi->text();
    QString sid         = m_ui->line_edit_sid->text();

    m_user_info->secili_sql_profili= sql_profili;
    m_user_info->sql_port          = m_ui->lineEdit_sql_port->text().toInt();
    m_user_info->sql_motoru        = m_ui->combobox_sql_motoru->currentText();
    m_user_info->sql_db_on_ek      = m_ui->lineEdit_veritabani_onek->text();
    m_user_info->veritabani_kodu   = m_ui->lineEdit_veritabani_kodu->text();
    m_user_info->is_auto_sifre     = m_ui->checkBox_otomatik_sifre->isChecked();

    OTOMATIK_SIFRE_KONTROLU();

    //! kullanici bilgilerini sorsun diye
    m_user_info->is_first_login  = m_yeni_profil;

    DB = NULL;
    G_YONETIM_DB = NULL;

    if ( CONNECT_TO_DB( ADAK_YONETIM, m_user_info, true ) EQ ADAK_FAIL ) {
        qDebug( "I could not connect to database administration." );//Yonetim veri tabanina baglanamamadim.
        return;
    }
    if ( CONNECT_TO_DB( ADAK_PROGRAM, m_user_info, true ) EQ ADAK_FAIL ) {
        qDebug( "I could not connect to programme database." );//Program veri tabanina baglanamamadim
        return;
    }

    SQL_SET_DB_SID(sid);

    bool first_login_git = true;
    if ( CHECK_KULLANICI_BILGILERI( m_user_info ) EQ ADAK_FAIL ) {
        first_login_git = true;
    }

    if ( DB->SQL_TABLES_EXISTS("") EQ true ) {
        first_login_git = false;
        m_user_info->is_first_login = false;
    }

    WRITE_SETTINGS( m_user_info );

    if( first_login_git EQ true ) {
        FIRST_LOGIN( m_user_info, false );
    }
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::CHECK_EXIT
***************************************************************************************/

int YENI_SUNUCU_TANIMLAMA_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::ORGANIZE_SQL_WIDGETS
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::ORGANIZE_SQL_WIDGETS()
{

    if( m_user_info->is_auto_sifre EQ true ) {
        m_ui->lineEdit_sql_kul_sifresi->setEnabled( false );
    }
    else {
        m_ui->lineEdit_sql_kul_sifresi->setEnabled( true );
    }

    switch (GET_SQL_DB_DRIVER_ENUM(m_ui->combobox_sql_motoru->currentText())) {
        case SQLITE:
            m_ui->lineEdit_sql_port->setEnabled(false);
            m_ui->label_sql_port->setEnabled(false);
            m_ui->lineEdit_sunucu->setEnabled(false);
            m_ui->label_sunucu->setEnabled(false);
            m_ui->label_sid->setVisible(false);
            m_ui->line_edit_sid->setVisible(false);
            m_ui->line_edit_sid->clear();
            m_ui->checkBox_otomatik_sifre->setVisible( false );
            m_ui->lineEdit_sql_kul_adi->setEnabled( false );
            m_ui->lineEdit_sql_kul_sifresi->setEnabled( false );
            return;
        case ORACLE:
            m_ui->line_edit_sid->clear();
            m_ui->lineEdit_sql_port->setEnabled(true);
            m_ui->label_sql_port->setEnabled(true);
            m_ui->lineEdit_sunucu->setEnabled(true);
            m_ui->label_sunucu->setEnabled(true);
            m_ui->label_sid->setVisible(true);
            m_ui->line_edit_sid->setVisible(true);
            m_ui->line_edit_sid->setText("XE");
            m_ui->checkBox_otomatik_sifre->setVisible( true );
            m_ui->lineEdit_sql_kul_adi->setEnabled( true );
            return;
        default:
            m_ui->lineEdit_sql_port->setEnabled(true);
            m_ui->label_sql_port->setEnabled(true);
            m_ui->lineEdit_sunucu->setEnabled(true);
            m_ui->label_sunucu->setEnabled(true);
            m_ui->label_sid->setVisible(false);
            m_ui->line_edit_sid->setVisible(false);
            m_ui->line_edit_sid->clear();
            m_ui->checkBox_otomatik_sifre->setVisible( true );
            m_ui->lineEdit_sql_kul_adi->setEnabled( true );
            break;
    }
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::KAYITLI_PROFILI_SIL
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::KAYITLI_PROFILI_SIL(bool kaydi_sil_button_clicked)
{
    QSettings settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));

    settings.remove ( QString("%1_sql_motoru"   ).arg(m_kayitli_profil_ismi));
    settings.remove ( QString("%1_sql_sunucu"   ).arg(m_kayitli_profil_ismi));
    settings.remove ( QString("%1_sql_port"     ).arg(m_kayitli_profil_ismi));
    settings.remove ( QString("%1_sql_db_on_ek" ).arg(m_kayitli_profil_ismi));
    settings.remove ( QString("%1_sql_username" ).arg(m_kayitli_profil_ismi));
    settings.remove ( QString("%1_sql_password" ).arg(m_kayitli_profil_ismi));
    settings.remove ( QString("%1_sql_sid"      ).arg(m_kayitli_profil_ismi));

    QString profil_str    = settings.value("kayitli_sql_sunuculari","").toString();
    QStringList profiller = profil_str.split("-");

    profiller.removeOne( m_kayitli_profil_ismi );

    if (kaydi_sil_button_clicked EQ true) {
        m_user_info->secili_sql_profili     = "";
        *m_run_batch_click = true;
        close();
    }
    else {
        profiller << m_ui->line_edit_profil_ismi->text();
    }

    profil_str.clear();

    for ( int i = 0 ; i < profiller.size() ; i++ ) {
        if ( i NE 0 ) {
            profil_str.append("-");
        }

        profil_str.append(profiller.at(i));
    }

    settings.setValue("kayitli_sql_sunuculari",profil_str);
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::FILL_SELECTED_SQL
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::FILL_SELECTED_SQL ( QString sql_motoru )
{
    if ( sql_motoru EQ "NONE" ) {
        m_ui->lineEdit_sunucu->clear();
        m_ui->lineEdit_sql_port->clear();
    }
    else if ( sql_motoru EQ "PostgreSQL" ) {
        m_ui->lineEdit_sunucu->setText("localhost");
        m_ui->lineEdit_sql_port->setText("5432");
    }
    else if ( sql_motoru EQ "MySQL" ) {
        m_ui->lineEdit_sunucu->setText("localhost");        
    }
    else if ( sql_motoru EQ "Microsoft" ) {
        m_ui->lineEdit_sunucu->setText("localhost");        
    }
    else if ( sql_motoru EQ "ORACLE" ) {
        m_ui->lineEdit_sunucu->setText("localhost");
    }
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::SET_UYARI_LABEL
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::SET_UYARI_LABEL()
{
    m_ui->label_yonetim_str->setText( m_ui->lineEdit_veritabani_onek->text()+ "adak_yonetim" );
    m_ui->label_sirket_str->setText ( m_ui->lineEdit_veritabani_onek->text()+ "adak_" + m_ui->lineEdit_veritabani_kodu->text() );
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::OTOMATIK_SIFRE_KONTROLU
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::OTOMATIK_SIFRE_KONTROLU()
{
    m_user_info->sql_username = m_ui->lineEdit_sql_kul_adi->text();
    m_user_info->sql_sunucu   = m_ui->lineEdit_sunucu->text();

    //! OTOMATIK SIFRE BU IKI ADA GORE URETILIYOR.
    SET_SQL_USERNAME( m_user_info->sql_username );
    SET_SQL_HOSTNAME( m_user_info->sql_sunucu   );


    if ( m_ui->checkBox_otomatik_sifre->isChecked() EQ true ) {
        m_user_info->sql_password = SQL_DEFAULT_PASSWORD();
    }
    else {
        m_user_info->sql_password = m_ui->lineEdit_sql_kul_sifresi->text();
    }
}

/**************************************************************************************
                   YENI_SUNUCU_TANIMLAMA_BATCH::SQL_UYARISINI_GUNCELLE
***************************************************************************************/

void YENI_SUNUCU_TANIMLAMA_BATCH::SQL_UYARISINI_GUNCELLE( DB_DRIVER p_sql_motoru)
{
    m_ui->label_uyari_1->setVisible(true);
    m_ui->label_uyari_2->setVisible(true);
    m_ui->label_sql_uyarisi->setVisible(true);

    QString server_turu;
    QString db_olusturma_komutu;

    m_ui->label_sql_uyarisi->setVisible(true);
    switch( p_sql_motoru ) {
        case  SQLITE:
            m_ui->label_uyari_1->setVisible(false);
            m_ui->label_uyari_2->setVisible(false);
            m_ui->label_sql_uyarisi->setVisible(false);
            break;
        case PSQL:
            server_turu = tr(" In Postgres Server");//Postgres Server' da
            db_olusturma_komutu = "CREATE DATABASE  veritabani_adi  WITH ENCODING <br>'UTF8' TEMPLATE template0;";
            break;
        case MYSQL:
            server_turu = tr("In Mysql Server ");
            db_olusturma_komutu = "CREATE DATABASE IF NOT EXISTS  veritabani_adi  <br>DEFAULT CHARACTER SET utf8 COLLATE utf8_turkish_ci";
            break;
        case MSSQL:
            server_turu = tr("In MSsql Server");
            db_olusturma_komutu = "CREATE DATABASE veritabani_adi ;";
            break;
        case ORACLE:
            server_turu = tr("In Oracle Server");
            db_olusturma_komutu = "CREATE USER veritabani_adi IDENTIFIED BY veritabani_adi <br>DEFAULT TABLESPACE users TEMPORARY TABLESPACE temp "
                                  "<br > ve sonra  GRANT ALL PRIVILEGES TO veritabani_adi";
            break;
        case ODBC:
            server_turu = tr("In ODBC");
            db_olusturma_komutu = "CREATE DATABASE IF NOT EXISTS veritabani_adi <br>DEFAULT CHARACTER SET utf8 COLLATE utf8_turkish_ci;";
            break;
        default:
            break;
    }

    m_ui->label_sql_uyarisi->setText( m_argumanli_label_txt.arg(server_turu).arg(db_olusturma_komutu) );
}
