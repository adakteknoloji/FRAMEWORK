#include "xyz.h"
#include "xyz_formu_open.h"
#include "xyz_formu_class.h"
#include "kernel_utils.h"
#include "ui_xyz_formu.h"
#include "adak_sql.h"

extern ADAK_SQL *           DB;



// Not - 1: Tum fonksiyonlar ve kullanim detaylari ayni zamanda form_kernel.h dosyasindada mevcuttur.

// Not - 2: Eger pencerelerin tum isletim sistemlerinde duzgun tam ekran olmasini istiyorsak xxx.show() komutundan sonra
// setWindowState(Qt::WindowMaximized); komutunu cagirmaliyiz.Mdi kullaniyorsak ise SET_MDI_WINDOW dan sonra
// setWindowState(Qt::WindowMaximized); cagirmaliyiz.Tam ekran olmayanlarda ise max min sizelari designer dan ve ya koddan
// sabitleyerek resize olmasini engellemeliyiz.

// Not - 3: Fis ve form kernellarinda kullanilan kernel button widgetlarinin horizontal sizepolicy expanding vertical size policy fixed olmali.

// mdi area eger fisimizin mdi window olmasi gerekiyorsa kullanilmalidir.
// aksi takdir de fonsiyonu void OPEN_XYZ_FISI(int fis_id,QWidget * parent) seklinde tanimlamaliyiz.ve parenti
// parametre olarak almali.

/**************************************************************************************
                   OPEN_XYZ_FORMU
***************************************************************************************/

void OPEN_XYZ_FORMU ( int record_id, QWidget * parent )
{
    Q_UNUSED (record_id);
    XYZ_FORMU * F = new XYZ_FORMU ( parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   XYZ_FORMU::XYZ_FORMU
***************************************************************************************/

XYZ_FORMU::XYZ_FORMU ( QWidget * parent ) : FORM_KERNEL ( parent ), m_ui(new Ui::XYZ_FORMU)
{
    setupUi     ( this );
    START_FORM_KERNEL ( this , DB );
}

/**************************************************************************************
                   XYZ_FORMU::SETUP_FORM
***************************************************************************************/

void  XYZ_FORMU::SETUP_FORM ()
{
    // Single record mode setlenirse sadece yenile ve kaydet butonlari aktif olur.
    // record mode atamasi init kerneldan once yapilmalidir.Cunku gui init kernelda olusturulur.
    // single record mode da database de en az 1 kayit oldugu kabul edilir.bu yuzden yoksa bir kere yaratilmalidir.
    //
    // SET_SIGNLE_RECORD_MODE (record_id)

    // Programa ilk girildiginde ve yeni kayitta focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( xyz_widget );

    // buttonlarin koyacagi widgetlar atanir.
    REGISTER_BUTTONS_WIDGET ( kernel_buttons_widget );

    SET_PAGE_TITLE    (tr("BLA BLA EKRANI"));
    SET_SETTING_NAME  ("BLA_BLA_FORMU");
    SET_HELP_PAGE     ("bla_bla.html");

    DISABLE_CLEAR_ON_WIDGET(lineEdit_muhasebe_ayraci);
      
    //Tum lineeditlerin ve limitedtexteditlerin uzunlugu INIT_KERNEL dan sonra setlenmeli.Cunku bu widgetlarin default degeri 1 characterdir.
    //Ayrica eger bu widgetlara setlenmesi gereken degerler varsa bunu da uzunluklar setlendikten sonra yapmaliyiz.
    lineEdit_xyz->setMaxLength(25);
}


/**************************************************************************************
                   XYZ_FORMU::NEW_RECORD
***************************************************************************************/

void  XYZ_FORMU::NEW_RECORD ()
{
    ////Bu fonksiyonla CLEAR_ALL_WIDGETS da clear lanmicak widgetlar setlenir.
    //DISABLE_CLEAR_ON_WIDGET (xyz_widget);
    adakDate_gunun_tarihi->SET_DATE(QDate::currentDate());
}

/**************************************************************************************
                   XYZ_FORMU::CLEAR_FORM_MEMBERS
***************************************************************************************/

void XYZ_FORMU::CLEAR_FORM_MEMBERS()
{
    m_aaaaa = 0;
}

/**************************************************************************************
                   XYZ_FORMU::GET_RECORD
***************************************************************************************/

int XYZ_FORMU::GET_RECORD ( int record_id )
{
    // Basit bir get record ornegi

    SQL_QUERY       sql_query( DB );

    sql_query.PREPARE_SELECT ( "sys_ikramlar" ,"ikram_ismi, fiyat_sekli, aciklama","ikram_id = :ikram_id" );

    sql_query.SET_VALUE ( ":ikram_id", record_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }
    sql_query.NEXT();

    line_edit_ikram_ismi->setText    ( sql_query.VALUE(0).toString() );
    check_box_fiyat_sekli->setChecked( sql_query.VALUE(1).toInt()    );
    text_edit_aciklama->setText      ( sql_query.VALUE(2).toString() );

    FIYAT_SEKLI_SECIMI ( p_gecerli_kisi_araliklari_sayisi, check_box_fiyat_sekli,
                         comma_edit_ikram_fiyati, P_COMMA_EDIT_KISI_BASI_IKRAM_FIYATLARI,
                         P_COMMA_EDIT_IKRAM_FIYATLARI );




    SQL_QUERY query( DB );

    query.PREPARE_SELECT( "sys_hizmetler","kisi_basi_hizmet_fiyatlari_array, hizmet_fiyatlari_array","hizmet_id = :hizmet_id " );

    query.SET_VALUE( ":hizmet_id ", record_id  );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        UNPACK_DOUBLE_ARRAY( query.VALUE( 0 ).toString(), P_KISI_BASI_HIZMET_FIYATLARI, KISI_ARALIKLARI_SAYISI  );
        UNPACK_DOUBLE_ARRAY( query.VALUE( 1 ).toString(), P_HIZMET_FIYATLARI, KISI_ARALIKLARI_SAYISI  );
    }

    if ( check_box_fiyat_sekli->isChecked() EQ true ) {
         comma_edit_ikram_fiyati->clear();
    }
    else {
        comma_edit_ikram_fiyati->SET_DOUBLE( P_IKRAM_FIYATLARI[0] );
    }

    for ( int i = 0; i < p_gecerli_kisi_araliklari_sayisi; i++ ) {
        P_COMMA_EDIT_IKRAM_FIYATLARI[i]->SET_DOUBLE( P_IKRAM_FIYATLARI[i] );
        P_COMMA_EDIT_KISI_BASI_IKRAM_FIYATLARI[i]->SET_DOUBLE( P_KISI_BASI_IKRAM_FIYATLARI[i] );
    }

    for ( int i = 0; i < table_widget_salonlar->rowCount(); i++ ) {

        sql_query.PREPARE_SELECT("sys_ikramin_salonlari","ikramin_salonlari_id","ikram_id= :ikram_id AND salon_id = :salon_id");

        sql_query.SET_VALUE ( ":ikram_id", record_id );
        sql_query.SET_VALUE ( ":salon_id", table_widget_salonlar->item( i, 0 )->text().toInt() );

        if ( sql_query.SELECT() NE 0 ) {
            table_widget_salonlar->item( i, 1 )->setCheckState( Qt::Checked );
            table_widget_salonlar->item( i, 1 )->setTextColor( Qt::darkGreen );
        }
        else {
            table_widget_salonlar->item( i, 1 )->setCheckState( Qt::Unchecked );
            table_widget_salonlar->item( i, 1 )->setTextColor( Qt::red );
        }
    }

    return ADAK_OK;

}

// TableWidget larda item changed yada double clicked eventlari check var da kontrol edilebilir.
// Gelen object daima table widget olacaktir.Bu yuzden objectname kontrol etmeliyiz mesela check var dusen bir table widget icin;
//     if (object EQ tableWidget ) {
//         if (object.objectName() EQ "ITEM_CHANGED") {
//             item changed gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//         }
//         if (object.objectName() EQ "ITEM_DOUBLE_CLICKED") {
//             item double clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//         }
//         if (object.objectName() EQ "ITEM_CLICKED") {
//             item clicked gerekli olanlar yapilir ve adak_fail yada adak_ok dondurulur
//         }
//     }

/**************************************************************************************
                   XYZ_FORMU::CHECK_VAR
***************************************************************************************/

int XYZ_FORMU::CHECK_VAR (QObject * object)
{
    if ( object EQ combobox_para_birimi ) {
        SQL_QUERY query(DB);
        query.PREPARE_SELECT ( "e9_para_birimleri","para_birimi_adi","para_birimi_kodu=:para_birimi_kodu" );
        query.SET_VALUE ( ":para_birimi_kodu", combobox_para_birimi->currentText() );
        if ( query.SELECT() EQ 0 ) {
            MSG_ERROR(tr("Para birimi bulunamadı") , NULL);
            return ADAK_FAIL;
        }
        query.NEXT();
        lineedit_para_birimi->setText ( query.VALUE(0).toString() );
    }
    else if ( object EQ combobox_personel_ayraci ) {
        lineedit_personel_ayraci->setText ( AYRACLAR[combobox_personel_ayraci->currentIndex()][1] );
    }
    else if ( object EQ combobox_stok_ayraci) {
        lineedit_stok_ayraci->setText ( AYRACLAR[combobox_stok_ayraci->currentIndex()][1] );
    }
    else if ( object EQ combobox_cari_ayraci ) {
        lineedit_cari_ayraci->setText ( AYRACLAR[combobox_cari_ayraci->currentIndex()][1] );
    }
    else if (object EQ combobox_muhasebe_ayraci ) {
        lineedit_muhasebe_ayraci->setText ( AYRACLAR[combobox_muhasebe_ayraci->currentIndex()][1] );
    }
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FORMU::CHECK_EMPTY
***************************************************************************************/

int XYZ_FORMU::CHECK_EMPTY ()
{
    if (firmanin_para_birimi EQ NULL ) {
        MSG_WARNING(tr("Temel para birimini tanımlamalısınız!..") , NULL);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FORMU::CHECK_ADD
***************************************************************************************/

int XYZ_FORMU::CHECK_ADD ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FORMU::ADD_RECORD
***************************************************************************************/

int XYZ_FORMU::ADD_RECORD ()
{
    return 1;
}

/**************************************************************************************
                   XYZ_FORMU::CHECK_UPDATE
***************************************************************************************/

int XYZ_FORMU::CHECK_UPDATE ( int record_id )
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FORMU::UPDATE_RECORD
***************************************************************************************/

void XYZ_FORMU::UPDATE_RECORD ( int record_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_UPDATE ( "e9_sabit_degerler","id_column","firma_ismi,yetkili,firma_adresi,stok_ayraci,personel_ayraci" );

    query.SET_VALUE ( ":firma_ismi"        ,firma_ismi );
    query.SET_VALUE ( ":yetkili"           ,yetkili );
    query.SET_VALUE ( ":stok_ayraci"       ,stok_ayraci );
    query.SET_VALUE ( ":personel_ayraci"   ,personel_hsp_ayraci );


    query.UPDATE();

}

/**************************************************************************************
                   XYZ_FORMU::CHECK_DELETE
***************************************************************************************/

int XYZ_FORMU::CHECK_DELETE ( int record_id )
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_FORMU::DELETE_RECORD
***************************************************************************************/

void XYZ_FORMU::DELETE_RECORD ( int record_id )
{
}

/**************************************************************************************
                   XYZ_FORMU::FIND_FIRST_RECORD
***************************************************************************************/

int XYZ_FORMU::FIND_FIRST_RECORD()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore deil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

    // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini degil
    SQL_QUERY query(DB);

    // Sadece bir id ye ihtiyacimiz var bu yuzden limit select yaptik.Daha hizli query icin gereklidir.
    query.PREPARE_SELECT ( "xyz_fis_satirlari","ogrenci_id" );

    if ( query.SELECT("ogrenci_no",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FORMU::FIND_LAST_RECORD
***************************************************************************************/

int XYZ_FORMU::FIND_LAST_RECORD ()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore degil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

   // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini deil
    SQL_QUERY query(DB);

    // Sadece bir id ye ihtiyacimiz var bu yuzden limit select yaptik.Daha hizli query icin gereklidir.
    query.PREPARE_LIMIT_SELECT ( "xyz_fis_satirlari","ogrenci_id");

    if ( query.SELECT("ogrenci_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FORMU::FIND_NEXT_RECORD
***************************************************************************************/

int XYZ_FORMU::FIND_NEXT_RECORD ()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore deil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

    // Onceki ve sonraki kayitlarda formdan siralama kriterini alarak ona gore siralama yapmaliyiz.Mesela burda ogrenci no.
    // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini deil
    SQL_QUERY  query(DB);

    QString ogrenci_no = line_edit_ogrenci_no->text();;

    query.PREPARE_LIMIT_SELECT ( "xyz_fis_satirlari","ogrenci_id","ogrenci_no > :ogrenci_no");

    query.SET_VALUE     ( ":ogrenci_no"  , ogrenci_no );
    if (query.SELECT("ogrenci_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FORMU::FIND_PREV_RECORD
***************************************************************************************/

int XYZ_FORMU::FIND_PREV_RECORD ()
{
    // Burda dikkat edilmesi gereken kesinlikle form da siralama kriteri ne ise ona gore siralama yapilmali.
    // id ye gore deil mesela ogrenci leri kayit ettigimiz bir formda ogrenci numarasina gore siralama yapmaliyiz.

    // Onceki ve sonraki kayitlarda formdan siralama kriterini alarak ona gore siralama yapmaliyiz.Mesela burda ogrenci no.
    // Dikkat siralama kriterine gore sirada hangi kayit varsa onun idsini return etmeliyiz.Siralama kriterini deil
    QString ogrenci_no = line_edit_ogrenci_no->text();

    // Sadece bir id ye ihtiyacimiz var bu yuzden limit select yaptik.Daha hizli query icin gereklidir.
    query.PREPARE_LIMIT_SELECT ("xyz_fis_satirlari","ogrenci_id","ogrenci_no < :ogrenci_no");

    query.SET_VALUE     ( ":ogrenci_no"  , ogrenci_no );
    if (query.SELECT("ogrenci_no DESC",0,1) EQ 0 ) {
        return 0;
    }
    query.NEXT();
    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   XYZ_FORMU::SELECT_RECORD
***************************************************************************************/

int XYZ_FORMU::SELECT_RECORD()
{

    return 0;
}

/**************************************************************************************
                   XYZ_FORMU::LOCK_RECORD
***************************************************************************************/

int XYZ_FORMU::LOCK_RECORD( int record_id )
{
    return DB->LOCK_ROW ( "firma_bilgileri_id", "e9_sabit_degerler", QString ( "firma_bilgileri_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   XYZ_FORMU::UNLOCK_RECORD
***************************************************************************************/

void XYZ_FORMU::UNLOCK_RECORD( int record_id )
{
    DB->UNLOCK_ROW ( "firma_bilgileri_id", "e9_sabit_degerler", QString ( "firma_bilgileri_id = %1").arg ( record_id ) );
}

/**************************************************************************************
                   XYZ_FORMU::FIND_RECORD
***************************************************************************************/

int XYZ_FORMU::FIND_RECORD()
{
    return 0;
}

// search edit kullanilirsa bu fonksiyon tanimlanmali.Button a tikladigimizda buraya duser.Tek yapmamiz gereken degeri line edit e yazmak
// sonrasinda widgettan focus out olundugunda check var a duser.

/**************************************************************************************
                   XYZ_FORMU::SEARCH_EDIT_CLICKED
***************************************************************************************/

void XYZ_FORMU::SEARCH_EDIT_CLICKED(QWidget *,QLineEdit * line_edit)
{
    int xyz_id = OPEN_XYZ_ARAMA ( this );

    if ( xyz_id EQ -1 ) {
        return;
    }

    SQL_QUERY select_query (DB);

    select_query.PREPARE_SELECT ( "xyz_gruplari","xyz_kodu","xyz_id = :xyz_id" );
    select_query.SET_VALUE ( ":xyz_id", xyz_id );

    if (select_query.SELECT() NE 0) {
        select_query.NEXT();
        line_edit->setText(select_query.VALUE(0).toString());
    }
}

