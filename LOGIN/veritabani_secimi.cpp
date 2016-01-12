#include <QHeaderView>
#include "veritabani_secimi.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "login.h"

extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   VERITABANI_SEC
***************************************************************************************/

int VERITABANI_SEC ( int kullanici_id, QString * veritabani_ismi , QString * veritabani_tanimi , QDialog * parent )
{
    int        secilen_id = -1;

    VERITABANI_SECIMI  * F = new VERITABANI_SECIMI  ( kullanici_id , veritabani_ismi , veritabani_tanimi , parent );
    F->m_secilen_id = &secilen_id;
    F->EXEC (NOT_FULL_SCREEN);

    return secilen_id;
}

/**************************************************************************************
                   VERITABANI_SECIMI::VERITABANI_SECIMI
***************************************************************************************/

VERITABANI_SECIMI::VERITABANI_SECIMI( int kullanici_id , QString * veritabani_ismi , QString * veritabani_tanimi , QWidget *parent ) : SECIM_KERNELI ( parent )
{
    if ( kullanici_id EQ 1 ) {
        m_kullanici_id           = -1;
    }
    else {
        m_kullanici_id           = kullanici_id;
    }

    m_veritabani_ismi        = veritabani_ismi;
    m_veritabani_tanimi      = veritabani_tanimi;

    SET_WINDOW_SIZE  (400, 400 );
    SET_PAGE_TITLE ( QObject::tr ( "VERİTABANI SEÇİMİ" ) );
    SET_HEADERS ( QStringList()<<QObject::tr("Veritabani Id")<<QObject::tr ( "Veritabanı" )<<QObject::tr ( "Veritabanı Tanımı" ) );
    SET_SORTING ( false );

    SET_HELP_PAGE   ( "yonetim-islemleri_veritabani-degistir" );


    INIT_KERNEL (G_YONETIM_DB);

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->horizontalHeader()->setDefaultSectionSize(0);

    table_widget->hideColumn(0);
    table_widget->setColumnWidth ( 1,  30 );
    table_widget->setColumnWidth ( 2,  70 );

    SET_SETTING_NAME   ( "VERITABANI_SECIMI" );

}

/**************************************************************************************
                   VERITABANI_SECIMI::~VERITABANI_SECIMI
***************************************************************************************/

VERITABANI_SECIMI::~VERITABANI_SECIMI()
{
}

/**************************************************************************************
                   VERITABANI_SECIMI::FILL_TABLE
***************************************************************************************/

void VERITABANI_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY sql_query      ( G_YONETIM_DB ) ;

    if ( m_kullanici_id NE -1 ) {
        sql_query.PREPARE_SELECT ( "ynt_veritabanlari,ynt_veritabani_kullanicilari,ynt_veritabani_programlari","ynt_veritabanlari.veritabani_id, veritabani_ismi, veritabani_tanimi",
                                   "ynt_veritabanlari.veritabani_id = ynt_veritabani_kullanicilari.veritabani_id AND kullanici_id =:kullanici_id AND "
                                   "ynt_veritabanlari.veritabani_id = ynt_veritabani_programlari.veritabani_id AND "
                                   "ynt_veritabani_programlari.program_id = :program_id","");

        sql_query.SET_VALUE( ":kullanici_id", m_kullanici_id );
        sql_query.SET_VALUE( ":program_id"  , ADAK_VERITABANI_ID());
    }
    else {
        sql_query.PREPARE_SELECT ( "ynt_veritabanlari","veritabani_id, veritabani_ismi, veritabani_tanimi","","");
    }

    if ( sql_query.SELECT("veritabani_ismi") EQ 0 ) {
        return;
    }
    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {
        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText(sql_query.VALUE(0).toString());
        table_widget->item(current_row,1)->setText(sql_query.VALUE(1).toString());
        table_widget->item(current_row,2)->setText(sql_query.VALUE(2).toString());
    }
}


/**************************************************************************************
                   VERITABANI_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int VERITABANI_SECIMI::SINGLE_LINE_SELECTED ( int current_row )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    *m_secilen_id               = table_widget->item ( current_row, 0)->text().toInt();

    if ( m_veritabani_ismi NE NULL OR m_veritabani_tanimi NE NULL ) {
        *m_veritabani_ismi          = table_widget->item ( current_row, 1)->text();
        *m_veritabani_tanimi        = table_widget->item ( current_row, 2)->text();
    }

    return ADAK_EXIT;
}
