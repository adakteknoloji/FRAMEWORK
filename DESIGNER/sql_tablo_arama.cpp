#include "adak_utils.h"
#include "adak_sql.h"

#include "sql_tablo_arama.h"
#include "ui_sql_tablo_arama.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   OPEN_SQL_TABLO_ARAMA
***************************************************************************************/

int OPEN_SQL_TABLO_ARAMA ( QWidget * parent )
{
    int    secilen_id    = -1;
    SQL_TABLO_ARAMA *  F  = new SQL_TABLO_ARAMA( parent );
    F->m_secilen_id   = &secilen_id;
    F->EXEC (NOT_FULL_SCREEN);

    return secilen_id;
}

/**************************************************************************************
                   SQL_TABLO_ARAMA::SQL_TABLO_ARAMA
***************************************************************************************/

SQL_TABLO_ARAMA::SQL_TABLO_ARAMA ( QWidget * parent ) : ARAMA_KERNEL ( parent ), m_ui ( new Ui::SQL_TABLO_ARAMA )
{
    m_ui->setupUi      ( this );
    START_ARAMA_KERNEL ( this, DB );
}

/**************************************************************************************
                   SQL_TABLO_ARAMA::CHECK_VAR
***************************************************************************************/

void SQL_TABLO_ARAMA::SETUP_FORM ()
{
    REGISTER_TABLE_WIDGET ( m_ui->table_widget_tablolar);

    REGISTER_ARAMA_BUTTONS_WIDGET ( m_ui->widget );

    SET_HEADERS ( QStringList() << tr("tablo_id") << tr("Table Name")<<tr("Description"));

    SET_SORTING ( false );

    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_tablo_adi );

    SET_PAGE_TITLE    (tr("SEARCH SQL TABLE "));

    SET_HELP_PAGE     ("sql_tablo_arama.html");

    m_ui->table_widget_tablolar->setColumnWidth( 1 , 40);
    m_ui->table_widget_tablolar->setColumnWidth( 2 , 60);
    m_ui->table_widget_tablolar->hideColumn(0);

    SET_SETTING_NAME  ("SQL_TABLO_ARAMA");

    SET_ENTER_KEY_FOR_RUN_BATCH ( true );

    SET_FIND_BUTTON_NAME ( "Find" );

    m_ui->line_edit_tablo_adi->setMaxLength(50);
}

/**************************************************************************************
                   SQL_TABLO_ARAMA::CHECK_VAR
***************************************************************************************/

int SQL_TABLO_ARAMA::CHECK_VAR ( QObject *  )
{
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLO_ARAMA::CHECK_RUN
***************************************************************************************/

int SQL_TABLO_ARAMA::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLO_ARAMA::SEARCH
***************************************************************************************/

void SQL_TABLO_ARAMA::SEARCH ()
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("sql_tablolar","tablo_id, tablo_adi, aciklama","tablo_adi LIKE :tablo_adi");
    sql_query.SET_LIKE      (":tablo_adi",m_ui->line_edit_tablo_adi->text());

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_ROW();

        m_ui->table_widget_tablolar->item(current_row,0)->setText(sql_query.VALUE(0).toString());
        m_ui->table_widget_tablolar->item(current_row,1)->setText(sql_query.VALUE(1).toString());
        m_ui->table_widget_tablolar->item(current_row,2)->setText(sql_query.VALUE(2).toString());
    }
}

/**************************************************************************************
                   SQL_TABLO_ARAMA::SINGLE_LINE_SELECTED
***************************************************************************************/

int SQL_TABLO_ARAMA::SINGLE_LINE_SELECTED ( int selected_row_number )
{
    *m_secilen_id = m_ui->table_widget_tablolar->item ( selected_row_number, 0 )->text().toInt();

    return ADAK_EXIT;
}
