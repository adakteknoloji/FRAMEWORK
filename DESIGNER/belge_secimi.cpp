#include "belge_secimi.h"
#include "secim_kerneli.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_BELGE_SEC
***************************************************************************************/

int OPEN_BELGE_SEC (QDialog * parent)
{
    int secilen_id = -1;
    BELGE_SECIMI * F = new BELGE_SECIMI (&secilen_id , parent);
    F->EXEC (NOT_FULL_SCREEN);
    return secilen_id;
}

/**************************************************************************************
                   BELGE_SECIMI::BELGE_SECIMI
***************************************************************************************/

BELGE_SECIMI::BELGE_SECIMI( int * secilen_id , QWidget *parent ) : SECIM_KERNELI(parent)
{
    SET_WINDOW_SIZE(300,800);
    QStringList headers <<QObject::tr("document id")<<QObject::tr("Document Name");
    SET_HEADERS(headers);

    m_secilen_id = secilen_id;

    INIT_KERNEL(DB);

    SET_PAGE_TITLE ( QObject::tr("DOCUMENT SELECTION"));

    QTableWidget * tableWidget = GET_TABLE_WIDGET();

    tableWidget->setColumnWidth ( 2, 200 );
}

/**************************************************************************************
                   BELGE_SECIMI::FILL_TABLE
***************************************************************************************/

void BELGE_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn(0);

    SQL_QUERY query(DB);

    query.PREPARE_SELECT("blg_belgeler","belge_id, belge_adi");

    if( query.SELECT() EQ 0 ) {
        return;
    }

    int current_row;

    while (query.NEXT()) {
        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText(query.VALUE(0).toString());
        table_widget->item(current_row,1)->setText(query.VALUE(1).toString());

    }
}

/**************************************************************************************
                   BELGE_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int BELGE_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item(selected_row_number,0)->text().toInt();

    return ADAK_EXIT;
}
