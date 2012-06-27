#include "tasarim_secimi.h"
#include "secim_kerneli.h"

extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   TASARIM_SEC
***************************************************************************************/

int TASARIM_SEC (int belge_id,QWidget * parent)
{
    int secilen_id = -1;
    TASARIM_SECIMI *F = new TASARIM_SECIMI (&secilen_id,belge_id,parent);
    F->EXEC(NOT_FULL_SCREEN);

    return secilen_id;
}


/**************************************************************************************
                   TASARIM_SECIMI::TASARIM_SECIMI             
***************************************************************************************/

TASARIM_SECIMI::TASARIM_SECIMI(int * secilen_id , int belge_id, QWidget *parent ) : SECIM_KERNELI(parent)
{
    m_belge_id   = belge_id;
    m_secilen_id = secilen_id;

    SET_WINDOW_SIZE(400,300);

    SET_PAGE_TITLE("TASARIM SEÇİMİ");

    SET_HEADERS(QStringList()<<"tasarim_id"<<"Tasarım Adı");

    INIT_KERNEL(G_YONETIM_DB);
}


/**************************************************************************************
                   TASARIM_SECIMI::FILL_TABLE            
***************************************************************************************/

void TASARIM_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn(0);

    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","tasarim_id,tasarim_adi","belge_id = :belge_id","");
    query.SET_VALUE(":belge_id",m_belge_id);

    if( query.SELECT("tasarim_adi") EQ 0 ) {
        return;
    }

    int current_row = -1;

    while (query.NEXT()) {

        current_row = ADD_NEW_ROW();

        table_widget->item(current_row,0)->setText(query.VALUE(0).toString());
        table_widget->item(current_row,1)->setText(query.VALUE(1).toString());
    }
}

/**************************************************************************************
                   TASARIM_SECIMI::SINGLE_LINE_SELECTED      
***************************************************************************************/

int TASARIM_SECIMI::SINGLE_LINE_SELECTED (int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_id = table_widget->item(selected_row_number,0)->text().toInt();

    return ADAK_EXIT;
}
