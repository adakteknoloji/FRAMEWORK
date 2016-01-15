#include <QHeaderView>
#include "bilgisayar_secimi.h"
#include "adak_sql.h"
#include "yonetim.h"


extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   BILGISAYAR_SEC
***************************************************************************************/

int BILGISAYAR_SEC ( QDialog * parent )
{
    int      secilen_id = -1;

    BILGISAYAR_SECIMI * F = new BILGISAYAR_SECIMI ( parent );
    F->m_secilen_id = &secilen_id;

    F->EXEC(NOT_FULL_SCREEN);

    return secilen_id;
}

/**************************************************************************************
    BILGISAYAR_SECIMI::BILGISAYAR_SECIMI
***************************************************************************************/

BILGISAYAR_SECIMI::BILGISAYAR_SECIMI( QWidget * parent ) : SECIM_KERNELI ( parent )
{
    SET_PAGE_TITLE   ( tr ( "COMPUTER SELECTION" )  );
    SET_HELP_PAGE    ( "bilgisayar_secimi" );

    SET_WINDOW_SIZE  ( 500, 1000 );

    SET_HEADERS ( QStringList()<<tr ( "Computer Id" )<<tr ( "Computer Code" )<<tr ( "Computer Name" )
                  <<tr ( "Dongle IP" )<<tr ( "Wired Network Mac" )<<tr ( "Wireless Network Mac" )
                  <<tr ( "3G Mac" ) );

    SET_SORTING     ( false );

    INIT_KERNEL ( G_YONETIM_DB );

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn(0);

    table_widget->setColumnWidth ( 1,  120 );
    table_widget->setColumnWidth ( 2,  280 );
    table_widget->setColumnWidth ( 3,  150 );
    table_widget->setColumnWidth ( 4,  150 );
    table_widget->setColumnWidth ( 5,  150 );
    table_widget->setColumnWidth ( 6,  150 );

    SET_SETTING_NAME  ( "BILGISAYAR_SECIMI" );
}

/**************************************************************************************
                   BILGISAYAR_SECIMI::~BILGISAYAR_SECIMI
***************************************************************************************/

BILGISAYAR_SECIMI::~BILGISAYAR_SECIMI()
{
}

/**************************************************************************************
                   BILGISAYAR_SECIMI::FILL_TABLE
***************************************************************************************/

void BILGISAYAR_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    SQL_QUERY sql_query      ( G_YONETIM_DB );

    sql_query.PREPARE_SELECT ( "ynt_bilgisayarlar","bilgisayar_id, bilgisayar_kodu, bilgisayar_adi, dongle_ip, "
                               "mac_adresi_kablolu, mac_adresi_kablosuz, mac_adresi_3g","silinmis_mi = 0","");

    if (sql_query.SELECT( "bilgisayar_kodu") EQ 0 ) {
        return;
    }
    int current_row       = 0;

    while ( sql_query.NEXT () EQ true ) {

        current_row = ADD_NEW_ROW();

        QString mac_kablolu;
        QString mac_kablosuz;
        QString mac_3g;

        if ( sql_query.VALUE(4).toString().replace ( ":", "" ).isEmpty() EQ false ) {
            mac_kablolu = sql_query.VALUE(4).toString();
        }
        if ( sql_query.VALUE(5).toString().replace ( ":", "" ).isEmpty() EQ false ) {
            mac_kablosuz = sql_query.VALUE(5).toString();
        }
        if ( sql_query.VALUE(6).toString().replace ( ":", "" ).isEmpty() EQ false ) {
            mac_3g = sql_query.VALUE(6).toString();
        }

        table_widget->item ( current_row,0 )->setText ( sql_query.VALUE(0).toString() );
        table_widget->item ( current_row,1 )->setText ( sql_query.VALUE(1).toString() );
        table_widget->item ( current_row,2 )->setText ( sql_query.VALUE(2).toString() );
        table_widget->item ( current_row,3 )->setText ( sql_query.VALUE(3).toString() );
        table_widget->item ( current_row,4 )->setText ( mac_kablolu                   );
        table_widget->item ( current_row,5 )->setText ( mac_kablosuz                  );
        table_widget->item ( current_row,6 )->setText ( mac_3g                        );
    }
}

/**************************************************************************************
                   BILGISAYAR_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int BILGISAYAR_SECIMI::SINGLE_LINE_SELECTED (int current_row )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    *m_secilen_id = table_widget->item ( current_row, 0 )->text().toInt();
    return ADAK_EXIT;
}
