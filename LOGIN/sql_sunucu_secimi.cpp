#include <QTableWidget>
#include <QSettings>
#include "adak_sql.h"
#include "sql_sunucu_secimi.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   SQL_SUNUCU_SEC
***************************************************************************************/

QString SQL_SUNUCU_SEC ( QWidget * parent )
{
    QString secilen_sunucu = "";

    SQL_SUNUCU_SECIMI * F = new SQL_SUNUCU_SECIMI ( &secilen_sunucu , parent );
    if ( secilen_sunucu NE tr("New Server")) {
        F->EXEC (NOT_FULL_SCREEN);
    }

    return secilen_sunucu;
}

/**************************************************************************************
                   SQL_SUNUCU_SECIMI::SQL_SUNUCU_SECIMI
***************************************************************************************/

SQL_SUNUCU_SECIMI::SQL_SUNUCU_SECIMI( QString * secilen_sunucu ,QWidget * parent ) : SECIM_KERNELI ( parent )
{
    m_secilen_sunucu = secilen_sunucu;

    SET_WINDOW_SIZE ( 300, 800 );

    SET_HEADERS (QStringList() << "Sunucular" );

    SET_PAGE_TITLE    ( QObject::tr ( "SERVER SELECTION" ) );
    SET_SETTING_NAME  ( "SUNUCU_SECIMI" );
    SET_HELP_PAGE     ("sql_sunucu_secimi" );

    INIT_KERNEL ( DB );
}

/**************************************************************************************
                   SQL_SUNUCU_SECIMI::FILL_TABLE
***************************************************************************************/

void SQL_SUNUCU_SECIMI::FILL_TABLE()
{
    QTableWidget * table_widget  = GET_TABLE_WIDGET();

    QSettings settings ("ADAK_SETTINGS", ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()));

    QString sunucu_str_list = settings.value("kayitli_sql_sunuculari","").toString();

    if ( sunucu_str_list.isEmpty() EQ true ) {
        *m_secilen_sunucu = "New Server";
        close();
        return;
    }

    QStringList sunucu_str = sunucu_str_list.split("-");
    sunucu_str << "New Server";

    int current_row = -1;

    for ( int i = 0 ; i < sunucu_str.size() ; i++ ) {
        current_row = ADD_NEW_ROW();
        table_widget->item(current_row,0)->setText( sunucu_str.at(i) );
    }
    //Bağlantı kurulamadı Başka bir sunucu seçin veya yeni bir sunucu ekleyin
    MSG_ERROR("Unable to connect. Select another server or add a new server.",NULL);
}

/**************************************************************************************
                   SQL_SUNUCU_SECIMI::SINGLE_LINE_SELECTED
***************************************************************************************/

int SQL_SUNUCU_SECIMI::SINGLE_LINE_SELECTED(int selected_row_number)
{
    QTableWidget * table_widget = GET_TABLE_WIDGET();

    *m_secilen_sunucu           = table_widget->item ( selected_row_number, 0 )->text();

    return ADAK_EXIT;
}
