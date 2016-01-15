#include <QHeaderView>
#include "yonetim.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "bilgisayarlar_formu.h"
#include "ui_bilgisayarlar_formu.h"
#include "bilgisayar_secimi.h"
#include "kullanici_secimi.h"
#include "yonetim_utils.h"
#include "login.h"

#define KULLANICI_ID_COLUMN      0
#define KULLANICI_KODU_COLUMN    1
#define KULLANICI_ADI_COLUMN     2

extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_BILGISAYARLAR_FORMU
***************************************************************************************/


void OPEN_BILGISAYARLAR_FORMU(QWidget *parent)
{
    BILGISAYARLAR_FORMU * F = new BILGISAYARLAR_FORMU ( parent );
    F->EXEC(FULL_SCREEN);
    // Windows'ta E9.dll herseyi icersin diye yapilan uckagit
    PROGRAMA_GIRIS (-1, NULL, -1, -1 , NULL);
    // End of uckagit
}

/**************************************************************************************
                 BILGISAYARLAR_FORMU::BILGISAYARLAR_FORMU
***************************************************************************************/

BILGISAYARLAR_FORMU::BILGISAYARLAR_FORMU ( QWidget * parent ) : FORM_KERNEL( parent ), m_ui ( new Ui::BILGISAYARLAR_FORMU )
{
    m_ui->setupUi ( this );
    START_FORM_KERNEL         ( this, G_YONETIM_DB);
}

/**************************************************************************************
                 BILGISAYARLAR_FORMU::SETUP_FORM
***************************************************************************************/

void BILGISAYARLAR_FORMU::SETUP_FORM ()
{
    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_buttons_widget );

    REGISTER_CHANGER_BUTTON ( m_ui->button_current_dongle_kopyala     );
    REGISTER_CHANGER_BUTTON ( m_ui->button_mac_kopyala                );
    REGISTER_CHANGER_BUTTON ( m_ui->button_kullanici_sec              );

    SET_PAGE_TITLE      ( tr ( "COMPUTERS" ) );
    SET_HELP_PAGE       ( "yonetim-islemleri_bilgisayarlar" );

    m_ui->table_widget_kullanicilar->hideColumn(0);
    m_ui->table_widget_kullanicilar->setColumnWidth ( KULLANICI_KODU_COLUMN,100 );
    m_ui->table_widget_kullanicilar->setColumnWidth ( KULLANICI_ADI_COLUMN ,400 );

    SET_SETTING_NAME    ( "BILGISAYARLAR" );

    SET_FIRST_FOCUS_WIDGET( m_ui->lineEdit_bilgisayar_kodu );

    m_ui->line_edit_mac_1->setMaxLength(17);
    m_ui->line_edit_mac_2->setMaxLength(17);
    m_ui->line_edit_mac_3->setMaxLength(17);
    m_ui->line_edit_mac_giris_1->setMaxLength(17);
    m_ui->line_edit_mac_giris_2->setMaxLength(17);
    m_ui->line_edit_mac_giris_3->setMaxLength(17);

    m_ui->lineEdit_bilgisayar_adi->setMaxLength(50);
    m_ui->lineEdit_bilgisayar_kodu->setMaxLength(10);
    m_ui->lineEdit_current_dongle->setMaxLength(20);
    m_ui->lineEdit_dongle_IP->setMaxLength(20);

    DISABLE_CLEAR_ON_WIDGET(m_ui->lineEdit_current_dongle);
    DISABLE_CLEAR_ON_WIDGET(m_ui->line_edit_mac_giris_1);
    DISABLE_CLEAR_ON_WIDGET(m_ui->line_edit_mac_giris_2);
    DISABLE_CLEAR_ON_WIDGET(m_ui->line_edit_mac_giris_3);
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void BILGISAYARLAR_FORMU::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::NEW_RECORD
***************************************************************************************/

void BILGISAYARLAR_FORMU::NEW_RECORD()
{
    m_ui->lineEdit_bilgisayar_kodu->clear();
    m_ui->lineEdit_bilgisayar_adi->clear();
    m_ui->lineEdit_dongle_IP->clear();
    m_ui->line_edit_mac_1->clear();
    m_ui->line_edit_mac_2->clear();
    m_ui->line_edit_mac_3->clear();

    m_ui->lineEdit_current_dongle->setText ( GET_DONGLE_IP () );

    QList<QLineEdit * > mac_giris_line_editleri;

    mac_giris_line_editleri<<m_ui->line_edit_mac_giris_1<<m_ui->line_edit_mac_giris_2<<m_ui->line_edit_mac_giris_3;

    QStringList mac_adresleri = GET_MAC_ADDRESS();

    for ( int i = 0 ; i < mac_adresleri.size() ; i++ ) {
        if ( i EQ mac_giris_line_editleri.size() ) {
            break;
        }
        mac_giris_line_editleri.at(i)->setText(mac_adresleri.at(i));

    }

    m_ui->table_widget_kullanicilar->clearContents  ();
    m_ui->table_widget_kullanicilar->setRowCount    (0);
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::GET_RECORD
***************************************************************************************/

int  BILGISAYARLAR_FORMU::GET_RECORD ( int p_bilgisayar_id )
{
    m_ui->table_widget_kullanicilar->clearContents  ();
    m_ui->table_widget_kullanicilar->setRowCount    (0);

    SQL_QUERY sql_query       ( G_YONETIM_DB );
    SQL_QUERY kullanici_query ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_kodu, bilgisayar_adi, dongle_ip, mac_adresi_kablolu, "
                               "mac_adresi_kablosuz, mac_adresi_3g","bilgisayar_id = :bilgisayar_id " ,"");

    sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();
    m_ui->lineEdit_bilgisayar_kodu->setText         ( sql_query.VALUE(0).toString() );
    m_ui->lineEdit_bilgisayar_adi->setText          ( sql_query.VALUE(1).toString() );
    m_ui->lineEdit_dongle_IP->setText               ( sql_query.VALUE(2).toString() );
    m_ui->line_edit_mac_1->setText                  ( sql_query.VALUE(3).toString() );
    m_ui->line_edit_mac_2->setText                  ( sql_query.VALUE(4).toString() );
    m_ui->line_edit_mac_3->setText                  ( sql_query.VALUE(5).toString() );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayar_kullanicilari","kullanici_id","bilgisayar_id = :bilgisayar_id");
    sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );


    int current_table_row = 0;

    if ( sql_query.SELECT() > 0 ) {

        while ( sql_query.NEXT() EQ true ) {
            int kullanici_id = sql_query.VALUE(0).toInt();
            kullanici_query.PREPARE_SELECT ( " ynt_kullanicilar","kullanici_kodu,kullanici_adi","kullanici_id = :kullanici_id AND silinmis_mi = :silinmis_mi ","" ) ;
            kullanici_query.SET_VALUE      ( ":kullanici_id", kullanici_id );
            kullanici_query.SET_VALUE      ( ":silinmis_mi", 0             );
            if ( kullanici_query.SELECT() EQ 0 ) {
                continue;
            }
            kullanici_query.NEXT();
            m_ui->table_widget_kullanicilar->insertRow ( current_table_row );
            m_ui->table_widget_kullanicilar->setItem   ( current_table_row, KULLANICI_ID_COLUMN , new QTableWidgetItem( QVariant( kullanici_id ).toString() ) );
            m_ui->table_widget_kullanicilar->setItem   ( current_table_row, KULLANICI_KODU_COLUMN, new QTableWidgetItem( kullanici_query.VALUE(0).toString()) );
            m_ui->table_widget_kullanicilar->setItem   ( current_table_row, KULLANICI_ADI_COLUMN, new QTableWidgetItem( kullanici_query.VALUE(1).toString() ) );
            current_table_row++;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::CHECK_VAR
***************************************************************************************/

int BILGISAYARLAR_FORMU::CHECK_VAR ( QObject * p_object )
{

    if ( p_object EQ m_ui->lineEdit_bilgisayar_kodu ) {
        if ( m_ui->lineEdit_bilgisayar_kodu->text().count() > 10 ) {
            MSG_ERROR(tr ( "Computer code canm not be more than 10 characters.") , NULL);
            return ADAK_FAIL;//Bilgisayar kodu 10 karakterden fazla olamaz.
        }
    }
    else if ( p_object EQ m_ui->lineEdit_bilgisayar_adi ) {
        if ( m_ui->lineEdit_bilgisayar_adi->text().count() > 50 ) {
            MSG_ERROR(tr ( "Computer name can not be more than 50 characters.") , NULL);
            return ADAK_FAIL;//Bilgisayar adi 50 karakterden fazla olamaz.
        }
    }
    else if ( p_object EQ m_ui->line_edit_mac_1 ) {
        m_ui->line_edit_mac_1->setText ( m_ui->line_edit_mac_1->text().toUpper());
    }
    else if ( p_object EQ m_ui->line_edit_mac_2 ) {
        m_ui->line_edit_mac_2->setText ( m_ui->line_edit_mac_2->text().toUpper());
    }
    else if ( p_object EQ m_ui->line_edit_mac_3 ) {
        m_ui->line_edit_mac_3->setText ( m_ui->line_edit_mac_3->text().toUpper());
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
                   BILGISAYARLAR_FORMU::CHECK_EMPTY
***************************************************************************************/

int BILGISAYARLAR_FORMU::CHECK_EMPTY ()
{
    if ( m_ui->lineEdit_bilgisayar_kodu->text().isEmpty() EQ true ) {
        MSG_ERROR(tr ( "Computer code has not been entered." ) , m_ui->lineEdit_bilgisayar_kodu);
        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_bilgisayar_adi->text().isEmpty() EQ true ) {
        MSG_ERROR(tr ( "Computer code has not been entered." )  , m_ui->lineEdit_bilgisayar_adi);
        return ADAK_FAIL;
    }
    QString mac_1 = m_ui->line_edit_mac_1->text().replace ( ":", "" );;
    QString mac_2 = m_ui->line_edit_mac_2->text().replace ( ":", "" );;
    QString mac_3 = m_ui->line_edit_mac_3->text().replace ( ":", "" );;

    if ( mac_1.isEmpty()              EQ true AND
         mac_2.isEmpty()              EQ true AND
         mac_3.isEmpty()              EQ true AND
         m_ui->lineEdit_dongle_IP->text().isEmpty() EQ true ) {

        MSG_ERROR(tr ( "You must enter at least one of the computer recording the MAC addresses or DONGLE IP." ) , m_ui->line_edit_mac_1);
        return ADAK_FAIL;//Bilgisayar kaydederken MAC adreslerinden en az birini veya DONGLE IP yi girmelisiniz.
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::CHECK_ADD
***************************************************************************************/

int BILGISAYARLAR_FORMU::CHECK_ADD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","bilgisayar_kodu = :bilgisayar_kodu "
                                "AND silinmis_mi = 0 ","");
    sql_query.SET_VALUE       ( ":bilgisayar_kodu", m_ui->lineEdit_bilgisayar_kodu->text() );
    if ( sql_query.SELECT () > 0 ) {
        MSG_ERROR( tr ( "These codes registered in a computer present in the system" ) , NULL );
        return ADAK_FAIL;//Bu kodla kayıtlı bir bilgisayar sistemde mevcut
    }

    if ( m_ui->line_edit_mac_1->text().replace ( ":", "" ).isEmpty() EQ false ) {
        sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","(mac_adresi_kablolu = :mac_adresi_kablolu OR mac_adresi_kablosuz = :mac_adresi_kablosuz OR mac_adresi_3g = :mac_adresi_3g) "
                                    "AND silinmis_mi = 0 " );
        sql_query.SET_VALUE       ( ":mac_adresi_kablolu" , m_ui->line_edit_mac_1->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_kablosuz", m_ui->line_edit_mac_1->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_3g"      , m_ui->line_edit_mac_1->text() );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(QString( m_ui->line_edit_mac_1->text() + " " +
                                  tr ( "This MAC Address registered in a computer present in the system" ) ),NULL);
            return ADAK_FAIL;//Bu mac adresi ile kayıtlı bir bilgisayar sistemde mevcut
        }
    }

    if ( m_ui->line_edit_mac_2->text().replace ( ":", "" ).isEmpty() EQ false ) {

        sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","(mac_adresi_kablolu = :mac_adresi_kablolu OR mac_adresi_kablosuz = :mac_adresi_kablosuz OR mac_adresi_3g = :mac_adresi_3g) "
                                    "AND silinmis_mi = 0 " );
        sql_query.SET_VALUE       ( ":mac_adresi_kablolu" , m_ui->line_edit_mac_2->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_kablosuz", m_ui->line_edit_mac_2->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_3g"      , m_ui->line_edit_mac_2->text() );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(QString( m_ui->line_edit_mac_2->text() + " " +
                                  tr ( "This MAC Address registered in a computer present in the system" ) ),NULL);
            return ADAK_FAIL;
        }

    }

    if ( m_ui->line_edit_mac_3->text().replace ( ":", "" ).isEmpty() EQ false ) {

        sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id", "(mac_adresi_kablolu = :mac_adresi_kablolu OR mac_adresi_kablosuz = :mac_adresi_kablosuz OR mac_adresi_3g = :mac_adresi_3g) "
                                    "AND silinmis_mi = 0 ");
        sql_query.SET_VALUE       ( ":mac_adresi_kablolu" , m_ui->line_edit_mac_3->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_kablosuz", m_ui->line_edit_mac_3->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_3g"      , m_ui->line_edit_mac_3->text() );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(QString( m_ui->line_edit_mac_3->text() + " " +
                                  tr ( "This MAC Address registered in a computer present in the system" )) ,NULL);
            return ADAK_FAIL;
        }
    }

    if  ( m_ui->lineEdit_dongle_IP->text() NE "" ) {
         sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","dongle_ip = :dongle_ip "
                                     "AND silinmis_mi = 0 ");
        sql_query.SET_VALUE        (  ":dongle_ip", m_ui->lineEdit_dongle_IP->text() );
        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR( tr ( "This dongle is registered in the system" ) , NULL);
            return ADAK_FAIL;//Bu dongle sistemde kayıtlı
        }
    }

    bool ayni_mac_adresi_girilmis = false;

    if ( m_ui->line_edit_mac_1->text().replace(":", "" ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_mac_1->text().replace(":", "" ) EQ
             m_ui->line_edit_mac_2->text().replace(":", "" ) ) {
            ayni_mac_adresi_girilmis = true;
        }

        if ( m_ui->line_edit_mac_1->text().replace(":", "" ) EQ
             m_ui->line_edit_mac_3->text().replace(":", "" ) ) {

            ayni_mac_adresi_girilmis = true;
        }
    }

    if ( m_ui->line_edit_mac_2->text().replace(":", "" ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_mac_2->text().replace(":", "" ) EQ
             m_ui->line_edit_mac_3->text().replace(":", "" ) ) {

            ayni_mac_adresi_girilmis = true;
        }
    }


    if ( ayni_mac_adresi_girilmis EQ true ) {

        MSG_ERROR(QString( tr (" At least two mac addresses is the same."
                                  "Wired, wireless or 3G not identical none of the mac address.")),NULL);
        return ADAK_FAIL;//Girilen mac adreslerinden en az ikisi aynı.Kablolu, kablosuz veya 3G mac adreslerinin hiç biri birbirinin aynısı olamaz.
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::ADD_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::ADD_RECORD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    QString mac_kablolu;
    QString mac_kablosuz;
    QString mac_3g;

    if ( m_ui->line_edit_mac_1->text().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_kablolu = m_ui->line_edit_mac_1->text();
    }
    if ( m_ui->line_edit_mac_2->text().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_kablosuz = m_ui->line_edit_mac_2->text();
    }
    if ( m_ui->line_edit_mac_3->text().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_3g = m_ui->line_edit_mac_3->text();
    }


    sql_query.PREPARE_INSERT ( "ynt_bilgisayarlar","bilgisayar_id","bilgisayar_kodu, bilgisayar_adi, "
                               "dongle_ip, mac_adresi_kablolu, mac_adresi_kablosuz, mac_adresi_3g, silinmis_mi");

    sql_query.SET_VALUE      ( ":bilgisayar_kodu"        , m_ui->lineEdit_bilgisayar_kodu->text() );
    sql_query.SET_VALUE      ( ":bilgisayar_adi"         , m_ui->lineEdit_bilgisayar_adi->text()  );
    sql_query.SET_VALUE      ( ":dongle_ip"              , m_ui->lineEdit_dongle_IP->text()       );
    sql_query.SET_VALUE      ( ":mac_adresi_kablolu"     , mac_kablolu  );
    sql_query.SET_VALUE      ( ":mac_adresi_kablosuz"    , mac_kablosuz );
    sql_query.SET_VALUE      ( ":mac_adresi_3g"          , mac_3g       );
    sql_query.SET_VALUE      ( ":silinmis_mi"            , 0            );

    int bilgisayar_id = sql_query.INSERT();

    int table_row_count = m_ui->table_widget_kullanicilar->rowCount();
    if ( table_row_count NE 0 ) {

        for ( int i = 0; i < table_row_count; i++ ) {
            int kullanici_id = m_ui->table_widget_kullanicilar->item(i,KULLANICI_ID_COLUMN)->text().toInt();
            sql_query.PREPARE_INSERT ( "ynt_bilgisayar_kullanicilari","id","bilgisayar_id, kullanici_id");
            sql_query.SET_VALUE      ( ":bilgisayar_id", bilgisayar_id );
            sql_query.SET_VALUE      ( ":kullanici_id" , kullanici_id  );
            sql_query.INSERT();
        }
    }

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();

    QString     mac_adresi      = "";
    QStringList mac_adressleri  = GET_MAC_ADDRESS();

    for ( int i =0 ; i < mac_adressleri.size() ; i++ ) {
        mac_adresi.append(mac_adressleri.at(0));
    }

    QString log_detaylari = "Bilgisayar ID: " + QVariant (  bilgisayar_id ).toString() + " # Bilgisayar Kodu: "
                           + m_ui->lineEdit_bilgisayar_kodu->text() + "# Bilgisayar Adı: " + m_ui->lineEdit_bilgisayar_adi->text()
                           + " # MAC Adresi : " + mac_adresi +
                           " # DONGLE IP: " + m_ui->lineEdit_dongle_IP->text();

    YONETIM_007_KAYIT ( LOG_BILGISAYARLAR, LOG_KAYIT_EKLEME, log_detaylari );
    return bilgisayar_id;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::CHECK_UPDATE
***************************************************************************************/

int BILGISAYARLAR_FORMU::CHECK_UPDATE ( int p_bilgisayar_id )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","bilgisayar_kodu = :bilgisayar_kodu "
                                "AND bilgisayar_id != :bilgisayar_id AND silinmis_mi = 0 ","");
    sql_query.SET_VALUE       ( ":bilgisayar_kodu", m_ui->lineEdit_bilgisayar_kodu->text() );
    sql_query.SET_VALUE       ( ":bilgisayar_id", p_bilgisayar_id );
    if ( sql_query.SELECT () > 0 ) {
        MSG_ERROR(tr ( "This code registered in a computer present in the system" ) , NULL );
        return ADAK_FAIL;//bu kodla kayıtlı bir bilgisayar sistemde mevcut
    }

    if  ( m_ui->lineEdit_dongle_IP->text() NE "" ) {
         sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","dongle_ip = :dongle_ip "
                                    "AND bilgisayar_id != :bilgisayar_id AND silinmis_mi = 0","");
        sql_query.SET_VALUE       ( ":dongle_ip", m_ui->lineEdit_dongle_IP->text() );
        sql_query.SET_VALUE       ( ":bilgisayar_id", p_bilgisayar_id );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(tr ( "This dongle registered in a computer present in the system" ) , NULL);
            return ADAK_FAIL;
        }
    }

    if ( m_ui->line_edit_mac_1->text().replace ( ":", "" ).isEmpty() EQ false ) {

        sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id",
                                    "(mac_adresi_kablolu = :mac_adresi_kablolu OR mac_adresi_kablosuz = :mac_adresi_kablosuz OR mac_adresi_3g = :mac_adresi_3g) "
                                    "AND bilgisayar_id != :bilgisayar_id AND silinmis_mi = 0","");

        sql_query.SET_VALUE       ( ":mac_adresi_kablolu" , m_ui->line_edit_mac_1->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_kablosuz", m_ui->line_edit_mac_1->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_3g"      , m_ui->line_edit_mac_1->text() );
        sql_query.SET_VALUE       ( ":bilgisayar_id", p_bilgisayar_id );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(QString( m_ui->line_edit_mac_1->text() + " " +
                                    tr ( "This MAC address registered in a computer present in the system" )) , NULL);
            return ADAK_FAIL;
        }
    }

    if ( m_ui->line_edit_mac_2->text().replace ( ":", "" ).isEmpty() EQ false ) {

        sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar", "bilgisayar_id", "(mac_adresi_kablolu = :mac_adresi_kablolu OR mac_adresi_kablosuz = :mac_adresi_kablosuz OR mac_adresi_3g = :mac_adresi_3g) "
                                    "AND bilgisayar_id != :bilgisayar_id AND silinmis_mi = 0","" );

        sql_query.SET_VALUE       ( ":mac_adresi_kablolu" , m_ui->line_edit_mac_2->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_kablosuz", m_ui->line_edit_mac_2->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_3g"      , m_ui->line_edit_mac_2->text() );
        sql_query.SET_VALUE       ( ":bilgisayar_id", p_bilgisayar_id );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(QString( m_ui->line_edit_mac_2->text() + " " +
                                     tr ( "This MAC address registered in a computer present in the system" ) ),NULL);
            return ADAK_FAIL;
        }
    }

    if ( m_ui->line_edit_mac_3->text().replace ( ":", "" ).isEmpty() EQ false ) {

        sql_query.PREPARE_SELECT  ( "ynt_bilgisayarlar","bilgisayar_id","(mac_adresi_kablolu = :mac_adresi_kablolu OR mac_adresi_kablosuz = :mac_adresi_kablosuz OR mac_adresi_3g = :mac_adresi_3g) "
                                    "AND bilgisayar_id != :bilgisayar_id AND silinmis_mi = 0" ,"");

        sql_query.SET_VALUE       ( ":mac_adresi_kablolu" , m_ui->line_edit_mac_3->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_kablosuz", m_ui->line_edit_mac_3->text() );
        sql_query.SET_VALUE       ( ":mac_adresi_3g"      , m_ui->line_edit_mac_3->text() );
        sql_query.SET_VALUE       ( ":bilgisayar_id", p_bilgisayar_id );

        if ( sql_query.SELECT () > 0 ) {
            MSG_ERROR(QString(  m_ui->line_edit_mac_3->text() + " " +
                                      tr ( "This MAC address registered in a computer present in the system" ) ) , NULL);
            return ADAK_FAIL;
        }
    }

    bool ayni_mac_adresi_girilmis = false;

    if ( m_ui->line_edit_mac_1->text().replace(":", "" ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_mac_1->text().replace(":", "" ) EQ
             m_ui->line_edit_mac_2->text().replace(":", "" ) ) {
            ayni_mac_adresi_girilmis = true;
        }

        if ( m_ui->line_edit_mac_1->text().replace(":", "" ) EQ
             m_ui->line_edit_mac_3->text().replace(":", "" ) ) {

            ayni_mac_adresi_girilmis = true;
        }
    }

    if ( m_ui->line_edit_mac_2->text().replace(":", "" ).isEmpty() EQ false ) {

        if ( m_ui->line_edit_mac_2->text().replace(":", "" ) EQ
             m_ui->line_edit_mac_3->text().replace(":", "" ) ) {

            ayni_mac_adresi_girilmis = true;
        }
    }


    if ( ayni_mac_adresi_girilmis EQ true ) {

        MSG_ERROR(tr (" At least two mac addresses is the same."
                      "Wired, wireless or 3G not identical none of the mac address."), NULL);
        return ADAK_FAIL;
    }

    ADAK_MSG_ENUM answer = MSG_YES_NO_CANCEL(tr ( "Do you approve of the changes you made?" ) ,NULL);//yapılan değişiklikleri onaylıyor musunuz?
    if ( answer EQ ADAK_NO OR answer EQ ADAK_CANCEL) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::UPDATE_RECORD
***************************************************************************************/

void BILGISAYARLAR_FORMU::UPDATE_RECORD ( int p_bilgisayar_id )
{
    SQL_QUERY sql_query (G_YONETIM_DB );

    QString mac_kablolu;
    QString mac_kablosuz;
    QString mac_3g;

    if ( m_ui->line_edit_mac_1->text().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_kablolu = m_ui->line_edit_mac_1->text();
    }
    if ( m_ui->line_edit_mac_2->text().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_kablosuz = m_ui->line_edit_mac_2->text();
    }
    if ( m_ui->line_edit_mac_3->text().replace ( ":", "" ).isEmpty() EQ false ) {
        mac_3g = m_ui->line_edit_mac_3->text();
    }

    sql_query.PREPARE_UPDATE ( "ynt_bilgisayarlar", "bilgisayar_id","bilgisayar_kodu,bilgisayar_adi,dongle_ip,mac_adresi_kablolu,"
                                "mac_adresi_kablosuz,mac_adresi_3g","bilgisayar_id = :bilgisayar_id");

    sql_query.SET_VALUE      ( ":bilgisayar_kodu"       , m_ui->lineEdit_bilgisayar_kodu->text() );
    sql_query.SET_VALUE      ( ":bilgisayar_adi"        , m_ui->lineEdit_bilgisayar_adi->text()  );
    sql_query.SET_VALUE      ( ":dongle_ip"             , m_ui->lineEdit_dongle_IP->text()       );
    sql_query.SET_VALUE      ( ":mac_adresi_kablolu"    , mac_kablolu                            );
    sql_query.SET_VALUE      ( ":mac_adresi_kablosuz"   , mac_kablosuz                           );
    sql_query.SET_VALUE      ( ":mac_adresi_3g"         , mac_3g                                 );
    sql_query.SET_VALUE      ( ":bilgisayar_id"         , p_bilgisayar_id                        );

    sql_query.UPDATE();

    QString     mac_adresi      = "";
    QStringList mac_adressleri  = GET_MAC_ADDRESS();

    for ( int i =0 ; i < mac_adressleri.size() ; i++ ) {
        mac_adresi.append(QString(mac_adressleri.at(0) + "-"));
    }

    QString log_detaylari = tr("Computer ID: ") + QVariant (  p_bilgisayar_id ).toString() + tr(" # Computer Code: ")
                          + m_ui->lineEdit_bilgisayar_kodu->text() + tr("|AD: ") + m_ui->lineEdit_bilgisayar_adi->text()
                          + tr(" # MAC Address : ")  + mac_adresi
                          + tr(" # DONGLE IP: ") + m_ui->lineEdit_dongle_IP->text();

    YONETIM_007_KAYIT ( LOG_BILGISAYARLAR, LOG_KAYIT_GUNCELLEME, log_detaylari );

    sql_query.PREPARE_DELETE ( "ynt_bilgisayar_kullanicilari","bilgisayar_id = :bilgisayar_id ");
    sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );
    sql_query.DELETE         ();

    int table_row_count = m_ui->table_widget_kullanicilar->rowCount();
    if ( table_row_count NE 0 ) {

        for ( int i = 0; i < table_row_count; i++ ) {
            int kullanici_id = m_ui->table_widget_kullanicilar->item (  i,KULLANICI_ID_COLUMN )->text().toInt();
            sql_query.PREPARE_INSERT ( "ynt_bilgisayar_kullanicilari","id","bilgisayar_id, kullanici_id");
            sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );
            sql_query.SET_VALUE      (":kullanici_id", kullanici_id      );
            sql_query.INSERT();
        }
    }

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::CHECK_DELETE
***************************************************************************************/

int BILGISAYARLAR_FORMU::CHECK_DELETE ( int )
{
    SQL_QUERY query( G_YONETIM_DB );

    query.PREPARE_SELECT( "ynt_bilgisayarlar", "bilgisayar_id", "");
    int bilgisayar_sayisi =  query.SELECT();

    if ( bilgisayar_sayisi EQ 1 ) {
        MSG_WARNING("you can not delete the last remaining computer in the system.!", NULL );
        return ADAK_FAIL;//"Sistemde kalan tek Bilgisayar Silemezsiniz.!"
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::DELETE_RECORD
***************************************************************************************/

void BILGISAYARLAR_FORMU::DELETE_RECORD ( int p_bilgisayar_id )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_UPDATE ( "ynt_bilgisayarlar", "bilgisayar_id","silinmis_mi","bilgisayar_id = :bilgisayar_id " );
    sql_query.SET_VALUE      ( ":silinmis_mi"  , 1);
    sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );
    sql_query.UPDATE();

    QString     mac_adresi      = "";
    QStringList mac_adressleri  = GET_MAC_ADDRESS();

    for ( int i =0 ; i < mac_adressleri.size() ; i++ ) {
        mac_adresi.append(QString(mac_adressleri.at(0) + "-"));
    }


    QString log_detaylari = tr("Computer ID: ")+ QVariant( p_bilgisayar_id).toString() + tr(" # Computer Code: ")
                          + m_ui->lineEdit_bilgisayar_kodu->text() + tr("|AD: ") + m_ui->lineEdit_bilgisayar_adi->text()
                          + tr(" # MAC Address : ")  + mac_adresi
                          + tr(" # DONGLE IP: ") + m_ui->lineEdit_dongle_IP->text();


    YONETIM_007_KAYIT ( LOG_BILGISAYARLAR, LOG_KAYIT_SILME, log_detaylari );

    sql_query.PREPARE_DELETE ( "ynt_bilgisayar_kullanicilari","bilgisayar_id = :bilgisayar_id ");
    sql_query.SET_VALUE      ( ":bilgisayar_id", p_bilgisayar_id );
    sql_query.DELETE();

    BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::FIND_FIRST_RECORD()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ("ynt_bilgisayarlar","bilgisayar_id","silinmis_mi = 0","");

    if ( sql_query.SELECT("bilgisayar_kodu",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::FIND_LAST_RECORD ()
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ("ynt_bilgisayarlar","bilgisayar_id","silinmis_mi = 0","");

    if ( sql_query.SELECT("bilgisayar_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::FIND_NEXT_RECORD ( )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_id","bilgisayar_kodu > :bilgisayar_kodu AND silinmis_mi = 0","");

    sql_query.SET_VALUE  ( ":bilgisayar_kodu", m_ui->lineEdit_bilgisayar_kodu->text() );

    if ( sql_query.SELECT("bilgisayar_kodu",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::FIND_PREV_RECORD ( )
{
    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar" , "bilgisayar_id", "bilgisayar_kodu < :bilgisayar_kodu AND silinmis_mi = 0","");

    sql_query.SET_VALUE  ( ":bilgisayar_kodu", m_ui->lineEdit_bilgisayar_kodu->text() );

    if ( sql_query.SELECT("bilgisayar_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::FIND_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::FIND_RECORD()
{
    if ( m_ui->lineEdit_bilgisayar_kodu->text().isEmpty() EQ true ) {
        return 0;
    }
    SQL_QUERY sql_query      ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_id","bilgisayar_kodu = :bilgisayar_kodu AND silinmis_mi = 0" ,"" );
    sql_query.SET_VALUE      ( ":bilgisayar_kodu", m_ui->lineEdit_bilgisayar_kodu->text() );
    if ( sql_query.SELECT() EQ 0 ) {
         return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::SELECT_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::SELECT_RECORD()
{
   return BILGISAYAR_SEC ( this );
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::LOCK_RECORD
***************************************************************************************/

int BILGISAYARLAR_FORMU::LOCK_RECORD ( int p_bilgisayar_id )
{
    return G_YONETIM_DB->LOCK_ROW ( "bilgisayar_id", "ynt_bilgisayarlar", QString ( "bilgisayar_id = %1" ).arg ( p_bilgisayar_id ) );
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::UNLOCK_RECORD
***************************************************************************************/

void BILGISAYARLAR_FORMU::UNLOCK_RECORD (  int p_bilgisayar_id )
{
    G_YONETIM_DB->UNLOCK_ROW ( "bilgisayar_id", "ynt_bilgisayarlar", QString ( "bilgisayar_id = %1" ).arg ( p_bilgisayar_id ) );
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::LISTEYE_KULLANICI_EKLE
***************************************************************************************/

int BILGISAYARLAR_FORMU::LISTEYE_KULLANICI_EKLE ()
{
    m_secilen_kullanici_id = KULLANICI_SEC ( false, this );

    if ( m_secilen_kullanici_id  < 1 ) {
        return ADAK_RECORD_UNCHANGED;
    }
    for ( int i = 0; i < m_ui->table_widget_kullanicilar->rowCount(); i++ ) {

        int current_kullanici_id = m_ui->table_widget_kullanicilar->item ( i,KULLANICI_ID_COLUMN )->text().toInt();

        if ( current_kullanici_id NE m_secilen_kullanici_id ) {
           continue;
        }

        MSG_INFO(tr ( "On the no. %n line of the selected user is already available","",i+1 ) , NULL );
        return ADAK_RECORD_UNCHANGED;//Seçilen kullanıcı %n nolu satırda zaten mevcut.

    }

    SQL_QUERY sql_query ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT (  "ynt_kullanicilar","kullanici_kodu, kullanici_adi","kullanici_id = :kullanici_id","" );
    sql_query.SET_VALUE      ( ":kullanici_id", m_secilen_kullanici_id );

    if (sql_query.SELECT() > 0) {
        sql_query.NEXT();
        int new_row_num                     = m_ui->table_widget_kullanicilar->rowCount();
        m_ui->table_widget_kullanicilar->insertRow ( new_row_num );
        m_ui->table_widget_kullanicilar->setItem   ( new_row_num, KULLANICI_ID_COLUMN   , new QTableWidgetItem ( QVariant ( m_secilen_kullanici_id).toString() ) );
        m_ui->table_widget_kullanicilar->setItem   ( new_row_num, KULLANICI_KODU_COLUMN , new QTableWidgetItem (sql_query.VALUE(0).toString()) );
        m_ui->table_widget_kullanicilar->setItem   ( new_row_num, KULLANICI_ADI_COLUMN  , new QTableWidgetItem (sql_query.VALUE(1).toString()) );
    }
    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::KULLANICILAR_LISTESINDEN_SATIR_SIL
***************************************************************************************/

int BILGISAYARLAR_FORMU::KULLANICILAR_LISTESINDEN_SATIR_SIL ( int p_clicked_row, int p_clicked_column )
{
    if ( p_clicked_row < 0 OR p_clicked_column < 0 ) {
        return ADAK_FAIL;
    }

    m_ui->table_widget_kullanicilar->removeRow (  p_clicked_row );
    return ADAK_OK;
}

/**************************************************************************************
                   BILGISAYARLAR_FORMU::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int BILGISAYARLAR_FORMU::CHANGER_BUTTON_CLICKED ( QAbstractButton * p_button )
{
    if ( p_button EQ m_ui->button_kullanici_sec ) {
        return LISTEYE_KULLANICI_EKLE ();
    }
    else if ( p_button EQ m_ui->button_current_dongle_kopyala ) {
        m_ui->lineEdit_dongle_IP->setText (  m_ui->lineEdit_current_dongle->text());
        return ADAK_RECORD_CHANGED;
    }
    else if ( p_button EQ m_ui->button_mac_kopyala ) {
        m_ui->line_edit_mac_1->setText ( m_ui->line_edit_mac_giris_1->text() );
        m_ui->line_edit_mac_2->setText ( m_ui->line_edit_mac_giris_2->text() );
        m_ui->line_edit_mac_3->setText ( m_ui->line_edit_mac_giris_3->text() );
        return ADAK_RECORD_CHANGED;
    }

    return ADAK_RECORD_UNCHANGED;
}
