#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include "yonetim.h"
#include "yonetim_utils.h"
#include "program_secimi.h"


extern     ADAK_SQL            * G_YONETIM_DB;


/**************************************************************************************
                   PROGRAM_SEC
***************************************************************************************/

int PROGRAM_SEC ( QDialog * parent, bool check_exe )
{
    int     secilen_id  = -1;

    PROGRAM_SECIMI  * F = new PROGRAM_SECIMI ( check_exe,  parent);
    F->m_secilen_id = &secilen_id;
    F->EXEC (NOT_FULL_SCREEN);
    return secilen_id;
}


/**************************************************************************************
                   PROGRAM_SECIMI::PROGRAM_SECIMI
***************************************************************************************/

PROGRAM_SECIMI::PROGRAM_SECIMI(bool check_exe, QWidget *parent) : SECIM_KERNELI ( parent )
{
    m_check_exe = check_exe;

    SET_WINDOW_SIZE  (300, 300 );

    SET_HEADERS ( QStringList()<<"Program_Id"<<"PROGRAM_SECIMI");

    INIT_KERNEL (G_YONETIM_DB);

    HIDE_COLUMN_HEADER ( true );

    SET_PAGE_TITLE  ( "PROGRAM_SECIMI"  );
    SET_HELP_PAGE   ( "program_secimi" );

    QTableWidget * table_widget = GET_TABLE_WIDGET();

    table_widget->hideColumn(0);

    table_widget->setColumnWidth(1,250);

    SET_SETTING_NAME  ("PROGRAM_SECIMI");
}

/**************************************************************************************
                   PROGRAM_SECIMI::~PROGRAM_SECIMI
***************************************************************************************/

PROGRAM_SECIMI::~PROGRAM_SECIMI()
{
}

/**************************************************************************************
                   PROGRAM_SECIMI::FILL_TABLE
***************************************************************************************/

void PROGRAM_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    int row = 0;

    for ( int i = 0; i < NUM_OF_ADAK_PROGRAMS; i++ ) {
        if (  ADAK_PROGRAM_LONGNAME(i) EQ "") {
		continue;
	}
        if ( m_check_exe EQ true ) {
            QString program_path = QDir::currentPath() + "/" + ADAK_PROGRAM_EXE(i);
            #ifdef WIN32
                    program_path += ".exe";
            #endif

            if ( FILE_EXISTS( program_path ) EQ false ) {
                continue;
            }
        }
        row = ADD_NEW_ROW();
        table_widget->item(row,0)->setText ( QVariant(i).toString() );
        table_widget->item(row,1)->setText ( ADAK_PROGRAM_LONGNAME(i) );
    }
}

/**************************************************************************************
                   PROGRAM_SECIMI::~SINGLE_LINE_SELECTED
***************************************************************************************/

int PROGRAM_SECIMI::SINGLE_LINE_SELECTED ( int current_row )
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();
    *m_secilen_id = table_widget->item ( current_row, 0)->text().toInt();
    return ADAK_EXIT;
}


