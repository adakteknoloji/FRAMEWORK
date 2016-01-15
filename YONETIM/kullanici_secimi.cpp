#include <QHeaderView>
#include "kullanici_secimi.h"
#include "adak_sql.h"
#include "yonetim.h"


extern ADAK_SQL *   G_YONETIM_DB;

/**************************************************************************************
                   KULLANICI_SEC
***************************************************************************************/

int KULLANICI_SEC  ( bool p_yonetici_goster, QDialog * p_parent, QStringList * P_VAROLAN_KULLANICI_ID_LIST )
{
    int      secilen_id = -1;
    KULLANICI_SECIMI  * F  = new KULLANICI_SECIMI ( p_yonetici_goster, p_parent, P_VAROLAN_KULLANICI_ID_LIST );
    F->m_secilen_id = &secilen_id;
    F->EXEC(NOT_FULL_SCREEN);
    return secilen_id;
}

/**************************************************************************************
    KULLANICI_SECIMI::KULLANICI_SECIMI
***************************************************************************************/

KULLANICI_SECIMI::KULLANICI_SECIMI( bool p_yonetici_goster, QWidget * p_parent, QStringList * P_VAROLAN_KULLANICI_ID_LIST )
: SECIM_KERNELI ( p_parent ), m_yonetici_goster ( p_yonetici_goster )
{

    M_VAROLAN_KULLANICI_ID_LIST = P_VAROLAN_KULLANICI_ID_LIST;

    SET_WINDOW_SIZE                      ( 500, 700 );

    SET_PAGE_TITLE                       ( tr ("SELECTING USER") );
    SET_HELP_PAGE                        ( "kullanici_secimi");

    SET_SORTING                          ( false );

    SET_HEADERS                          ( QStringList()<<tr("User Id")<<tr("User Code")
                                          <<tr("User Name")<<tr("Email Address"));

    INIT_KERNEL                          ( G_YONETIM_DB );

    QTableWidget * table_widget = GET_TABLE_WIDGET ();

    table_widget->hideColumn(0);

    table_widget->setColumnWidth ( 1,  100 );
    table_widget->setColumnWidth ( 2,  250 );
    table_widget->setColumnWidth ( 3,  250 );

    SET_SETTING_NAME  ("KULLANICI_SECIMI");
}

/*************************************************
    KULLANICI_SECIMI::~KULLANICI_SECIMI
*************************************************/

KULLANICI_SECIMI::~KULLANICI_SECIMI()
{
}

/*************************************************
         KULLANICI_SECIMI::FILL_TABLE
*************************************************/

void KULLANICI_SECIMI::FILL_TABLE()
{

    SQL_QUERY sql_query ( G_YONETIM_DB );
    QString where_condition_str = "silinmis_mi = 0 ";


    if ( m_yonetici_goster EQ false ) {
        where_condition_str += "AND kullanici_id != 1 ";
    }

    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","kullanici_id, kullanici_kodu, kullanici_adi, email_adresi",where_condition_str,"" );
    if ( sql_query.SELECT("kullanici_kodu") EQ 0 ) {
        return;
    }

    QTableWidget * table_widget = GET_TABLE_WIDGET();
    int current_row = 0;

    while ( sql_query.NEXT() EQ true ) {

        int kullanici_id = sql_query.VALUE (0).toInt();

        if ( M_VAROLAN_KULLANICI_ID_LIST NE NULL ) {
            if ( KULLANICI_ID_VAROLAN_KUL_LISTESINDE_VAR_MI ( kullanici_id ) EQ true ) {
                continue;
            }
        }

        current_row = ADD_NEW_ROW();

        table_widget->item ( current_row, 0 )->setText ( QVariant ( kullanici_id ).toString()  );
        table_widget->item ( current_row, 1 )->setText ( sql_query.VALUE (1).toString()        );
        table_widget->item ( current_row, 2 )->setText ( sql_query.VALUE (2).toString()        );
        table_widget->item ( current_row, 3 )->setText ( sql_query.VALUE (3).toString()        );
    }
}

/**************************************************************************************
                   KULLANICI_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int KULLANICI_SECIMI::SINGLE_LINE_SELECTED ( int current_row )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    *m_secilen_id = table_widget->item ( current_row, 0)->text().toInt();
    return ADAK_EXIT;
}

/**************************************************************************************
                   KULLANICI_SECIMI::KULLANICI_ID_VAROLAN_KUL_LISTESINDE_VAR_MI
***************************************************************************************/

bool KULLANICI_SECIMI::KULLANICI_ID_VAROLAN_KUL_LISTESINDE_VAR_MI ( int p_kullanici_id )
{
    if ( M_VAROLAN_KULLANICI_ID_LIST->size() < 1 ) {
        return false;
    }

    for ( int i = 0; i < M_VAROLAN_KULLANICI_ID_LIST->size(); i++ ) {
        if ( p_kullanici_id EQ QVariant ( M_VAROLAN_KULLANICI_ID_LIST->at(i)).toInt() ) {
            return true;
        }
    }
    return false;
}

