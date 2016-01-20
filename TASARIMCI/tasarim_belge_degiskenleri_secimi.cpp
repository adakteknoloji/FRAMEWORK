#include <QTableWidget>
#include <QPushButton>
#include "adak_sql.h"
#include "tasarim_belge_degiskenleri_secimi.h"
#include "secim_kerneli.h"
#include "tasarim.h"
#include "tasarim_degiskeni.h"
#include "belge_kernel_defines.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_TASARIM_BELGE_DEGISKENLERI_SEC
***************************************************************************************/

TASARIM_DEGISKENI* OPEN_TASARIM_BELGE_DEGISKENLERI_SEC ( QList< TASARIM_DEGISKENI*> p_degisken_listesi, QWidget * parent )
{
    TASARIM_BELGE_DEGISKENLERI_SECIMI * F = new TASARIM_BELGE_DEGISKENLERI_SECIMI ( p_degisken_listesi, parent );
    F->secilen_degisken = NULL;
    F->EXEC( NOT_FULL_SCREEN );
    return F->secilen_degisken;
}

/**************************************************************************************
                   TASARIM_BELGE_DEGISKENLERI_SECIMI::TASARIM_BELGE_DEGISKENLERI_SECIMI
***************************************************************************************/

TASARIM_BELGE_DEGISKENLERI_SECIMI::TASARIM_BELGE_DEGISKENLERI_SECIMI( QList< TASARIM_DEGISKENI*> p_degisken_listesi, QWidget * parent ) : SECIM_KERNELI ( parent )
{

    m_degisken_listesi = p_degisken_listesi;

    SET_SORTING( false );

    SET_WINDOW_SIZE ( 400, 350 );
    SET_HEADERS (QStringList() << QObject::tr("Variable Id") << QObject::tr("Variable Name") );

    SET_PAGE_TITLE    ( QObject::tr( "VARIABLE SELECTION" ) );
    SET_SETTING_NAME  ( "DEGISKEN_SECIMI" );
    SET_HELP_PAGE     ( QObject::tr("variable_selection") );

    INIT_KERNEL ( DB );



    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn( 0 );
    table_widget->setColumnWidth( 1, 100 );
}

/**************************************************************************************
                   TASARIM_BELGE_DEGISKENLERI_SECIMI::FILL_TABLE
***************************************************************************************/

void TASARIM_BELGE_DEGISKENLERI_SECIMI::FILL_TABLE()
{
    QTableWidget *table_widget = GET_TABLE_WIDGET();

    int current_row = 0;

    for (int i = 0; i < m_degisken_listesi.size(); ++i) {

        current_row = ADD_NEW_ROW();

        TASARIM_DEGISKENI *degisken = m_degisken_listesi.at(i);

        table_widget->item(current_row, 0 )->setText ( QString::number( current_row ) );

        if ( degisken->GET_DEGISKEN_TYPE() NE USER_VARIABLE ) {
            QString degisken_text = degisken->GET_TEXT();

            degisken_text.prepend("[");
            degisken_text.append("]");

            if( degisken->GET_GRUP() EQ LINE ) {
                degisken_text.prepend("[");
                degisken_text.append("]");
            }

            table_widget->item(current_row, 1 )->setText ( degisken_text );
        }
        else {
            table_widget->item(current_row, 1 )->setText ( degisken->GET_TEXT() );
        }
    }
}

/**************************************************************************************
                   TASARIM_BELGE_DEGISKENLERI_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int TASARIM_BELGE_DEGISKENLERI_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    int degisken_id = table_widget->item ( selected_row_number, 0 )->text().toInt();

    secilen_degisken = m_degisken_listesi.at( degisken_id );

    return ADAK_EXIT;
}

/**************************************************************************************
                   TASARIM_BELGE_DEGISKENLERI_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int TASARIM_BELGE_DEGISKENLERI_SECIMI::MULTI_LINE_SELECTED(QList<int> selected_rows)
{
    Q_UNUSED( selected_rows );
    return 0;
}

/**************************************************************************************
                   TASARIM_BELGE_DEGISKENLERI_SECIMI::SLOT_DEGISKEN_KULLAN
***************************************************************************************/

void TASARIM_BELGE_DEGISKENLERI_SECIMI::SLOT_DEGISKEN_KULLAN()
{
    QPushButton *button = static_cast<QPushButton*>(sender());

    m_degisken_listesi.at( button->objectName().split("\t").at(0).toInt())->SET_VISIBLE( true );
    button->setEnabled( false );

}
