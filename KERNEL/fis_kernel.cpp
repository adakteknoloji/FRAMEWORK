#include <QToolButton>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QKeyEvent>
#include <QObject>
#include <QtGui>
#include <QCloseEvent>
#include "fis_kernel.h"
#include "adak_sql_query.h"
#include "adak_utils.h"


/**************************************************************************************
                   FIS_KERNEL::FIS_KERNEL
***************************************************************************************/

FIS_KERNEL::FIS_KERNEL ( QWidget * parent ) : FORM_KERNEL ( parent )
{
    m_kernel_button_text        = "Fiş Ekranı";
    is_fis_kernel               = true; // FIXME
    p_table_widget              = NULL;
    p_is_table_filling          = false;
    p_last_added_row            = -1;
}

/**************************************************************************************
                   FIS_KERNEL::~FIS_KERNEL
***************************************************************************************/

FIS_KERNEL::~FIS_KERNEL()
{

}

/**************************************************************************************
                   FIS_KERNEL::START_FIS_KERNEL
***************************************************************************************/

void FIS_KERNEL::START_FIS_KERNEL( QObject * parent,ADAK_SQL * database)
{
    START_FORM_KERNEL (parent,database);
}

/**************************************************************************************
                   FIS_KERNEL::INIT_KERNEL
***************************************************************************************/

void FIS_KERNEL::INIT_KERNEL ()
{
    if (p_line_buttons_icon_name.isEmpty() EQ true) {
        p_line_buttons_icon_name << ":/kernel_icons/kernel_ekle_ikonu.png";
    }

    if (p_line_check_box_names.isEmpty() EQ true) {
        p_line_check_box_names << "";
    }

    p_this_object_name   = objectName();

    p_table_widget->setFocus            ( Qt::OtherFocusReason      );
    p_table_widget->hideColumn          ( p_row_status_column_number);

    for( int i = 0 ; i <  p_number_of_hidden_column ; i++ ) {
        p_table_widget->hideColumn( i );
    }

    p_table_widget->setColumnWidth(p_insert_button_column, 30);
    p_table_widget->setColumnWidth(p_delete_button_column, 30);

    m_satir_degisen_widget_count = 0;
    p_current_row                = 0;
    p_block_signals              = false;
    p_is_table_filling           = false;
    p_focus_in_event_status      = false;
    m_search_clicked_started     = false;

    DISABLE_EVENTS_ON_WIDGET(p_table_widget);
    p_table_widget->installEventFilter(this);

    //bulundugumuz cell e bir ust satirdaki cell i kopyalar.
    QKeySequence key_sequnce_ust_cell_kopyala ( "Ctrl+K" );
    QAction * action_ust_cell_kopyala = new QAction(this);
    action_ust_cell_kopyala->setShortcut ( key_sequnce_ust_cell_kopyala );
    p_table_widget->addAction(action_ust_cell_kopyala);
    connect ( action_ust_cell_kopyala,SIGNAL ( triggered()),this,SLOT ( COPY_PREVIOUS_CELL_CURRENT() ) );
    //----------

    //Satiri silme kisayolu
    QKeySequence key_sequnce_delete_row ( "Alt+Delete" );
    QAction * action_delete_row = new QAction(this);
    action_delete_row->setShortcut ( key_sequnce_delete_row );
    p_table_widget->addAction ( action_delete_row );
    connect ( action_delete_row,SIGNAL ( triggered()),this,SLOT(DELETE_ROW()));
    //------------

    //Satir ekleme kisayolu
    QAction * action_add_row = new QAction(this);
    action_add_row->setShortcut( QKeySequence( Qt::ALT + Qt::Key_Insert ));
    p_table_widget->addAction ( action_add_row );
    connect ( action_add_row,SIGNAL ( triggered()),this,SLOT( INSERT_ROW()));
    //------------

    connect ( p_table_widget, SIGNAL ( currentCellChanged ( int,int,int,int ) ), this, SLOT ( CURRENT_CELL_CHANGED(int, int, int, int)));

    p_table_widget->setSelectionMode ( QAbstractItemView::NoSelection );
    p_table_widget->setFocus         ( Qt::OtherFocusReason );

    p_table_widget->horizontalHeader()->setClickable  (false);
    p_table_widget->verticalHeader  ()->setClickable  (false);

    FORM_KERNEL::INIT_KERNEL();
}

/**************************************************************************************
                   FIS_KERNEL::AUTOSTART_FIS_KERNEL
***************************************************************************************/

void FIS_KERNEL::AUTOSTART_FIS_KERNEL ( int record_id )
{
    AUTOSTART_FORM_KERNEL ( record_id ) ;
}

/**************************************************************************************
                   FIS_KERNEL::SET_COMMA_EDIT_PRECISION
***************************************************************************************/


void FIS_KERNEL::SET_COMMA_EDIT_PRECISION ( int column,int precision )
{
    if ( p_comma_edit_precision.size() < p_number_of_column ) {
        for (int i=0; i < p_number_of_column + 1 ; i++ ) {
            p_comma_edit_precision << 2;
        }
    }
    
    p_comma_edit_precision[column] = precision;
}


/**************************************************************************************
                   FIS_KERNEL::ADD_NEW_LINE
***************************************************************************************/

int FIS_KERNEL::ADD_NEW_LINE ( int current_row, bool close_events )
{
    if (current_row EQ -1 OR (current_row > p_table_widget->rowCount())) {
        current_row = p_table_widget->rowCount();
        if (current_row EQ 0) {
            p_last_added_row = current_row;
        }
        else {
            p_last_added_row = current_row - 1;
        }
    }

    p_block_signals = true;

    p_table_widget->insertRow(current_row);

    for ( int i = 0; i < p_number_of_column - 1; i++ ) {
        if ( i EQ p_insert_button_column ) {
            ADD_INSERT_BUTTON( current_row );
        }
        else if ( i EQ p_delete_button_column ) {
            ADD_DELETE_BUTTON( current_row );
        }
        else if ( p_widget_types.at(i) EQ NO_TYPE ) {
            continue;
        }
        else {
            SET_CELL_WIDGET(p_widget_types.at(i),i,current_row);
        }
    }

    CLOSE_ALL_EVENTS();
    SET_LINE_DEFAULTS(current_row);
    OPEN_ALL_EVENTS();
    UPDATE_ROW_STATUS(current_row, EMPTY);
    ADD_ORDER_NUMBER (current_row);

    p_block_signals = false;

    if ( p_is_table_filling EQ false) {
        if ((p_table_widget->rowCount() EQ (current_row + 1) AND p_last_added_row NE current_row - 1) EQ false) {
            int focus_current_row = p_table_widget->currentRow();
            if ( current_row EQ focus_current_row + 1 ) {
                SET_TABLE_WIDGET_FOCUS(current_row,p_first_visible_column);
            }
        }

        if (p_table_widget->rowCount() NE (current_row + 1)) {
           ENABLE_DELETE_BUTTON(current_row);
        }
    }

    if (close_events EQ true) {
        CLOSE_ALL_EVENTS();
    }
    return current_row;
}

/**************************************************************************************
                   FIS_KERNEL::ADD_ORDER_NUMBER
***************************************************************************************/

void FIS_KERNEL::ADD_ORDER_NUMBER ( int current_row )
{
    QTableWidgetItem * table_item;
    int                last_row_number;
    int                after_order_number;
    int                before_order_number;
    int                current_order_number;

    last_row_number = ( p_table_widget->rowCount() ) - 1;

    if ( current_row EQ 0 ) {
        before_order_number  = 0;
    }
    else {
        before_order_number  = p_table_widget->item(current_row - 1, p_fis_order_column_number)->text().toInt();
    }

    if ( current_row  EQ last_row_number) {
        current_order_number = before_order_number + ORDER_NUMBER_RANGE_VALUE;
    }
    else {
        after_order_number   = p_table_widget->item(current_row + 1, p_fis_order_column_number)->text().toInt();
        current_order_number = (after_order_number >> 1) + (before_order_number >> 1);
    }

    table_item = new QTableWidgetItem(tr("%1").arg(current_order_number));
    p_table_widget->setItem(current_row, p_fis_order_column_number, table_item);

    if ( current_order_number - before_order_number > 8 ) {
        return;
    }

    for ( int i = p_table_widget->rowCount(); i NE 0  ; i-- ) {
        current_order_number = i * ORDER_NUMBER_RANGE_VALUE;
        table_item           = new QTableWidgetItem(tr("%1").arg(current_order_number));
        p_table_widget->setItem(i - 1, p_fis_order_column_number, table_item);
        REORGANIZE_ORDER_NUMBER(i - 1, current_order_number );
    }
}

/**************************************************************************************
                   FIS_KERNEL::REORGANIZE_ORDER_NUMBER
***************************************************************************************/

void FIS_KERNEL::REORGANIZE_ORDER_NUMBER ( int row_number,int new_order_number )
{
    int row_status;

    row_status = p_table_widget->item(row_number, p_row_status_column_number)->text().toInt();

    if ( row_status EQ EMPTY OR row_status EQ NEED_ADD ) {
        return;
    }

    int id = p_table_widget->item(row_number, p_row_id_column_number )->text().toInt();

    START_KERNEL_TRANSACTION();

    SQL_QUERY sql_query (m_db_conn);
    sql_query.PREPARE_UPDATE (p_db_table_name,p_db_line_id_column_name,p_db_fis_order_column_name,QString("%1 = :id").arg(p_db_line_id_column_name));

    sql_query.SET_VALUE      (":"+p_db_fis_order_column_name      , new_order_number );
    sql_query.SET_VALUE      (":id"                 , id );

    sql_query.UPDATE();

    COMMIT_KERNEL_TRANSACTION();
}

/**************************************************************************************
                   FIS_KERNEL::TABLE_WIDGETS_CHANGED
***************************************************************************************/

void FIS_KERNEL::TABLE_WIDGETS_CHANGED()
{
    if ( m_widget_changed_active EQ false ) {
         return;
    }

    int current_row;
    int row_status;

    current_row = p_table_widget->currentRow();

    if ( p_block_signals EQ true ) {
        return;
    }

    if ( current_row < 0 ) {
        return;
    }

    row_status  = p_table_widget->item(current_row, p_row_status_column_number)->text().toInt();

    if (m_satir_degisen_widget_count EQ 0) {
        if( IS_WIDGET_CHANGED() EQ false ) {
            if ( row_status EQ NEED_ADD ) {
                UPDATE_ROW_STATUS( current_row, EMPTY);
                DISABLE_DELETE_BUTTON( current_row );
            }
            if ( row_status EQ NEED_UPDATE ) {
                UPDATE_ROW_STATUS( current_row, SAVED);
            }
            if (m_degisen_widget_counter EQ 0) {
                RESET_WIDGET_CHANGED();
            }
            return;
        }
    }

    if ( row_status EQ EMPTY ) {
        UPDATE_ROW_STATUS( current_row, NEED_ADD);
        ENABLE_DELETE_BUTTON( current_row );
    }
    if ( row_status EQ SAVED ) {
        UPDATE_ROW_STATUS( current_row, NEED_UPDATE);
    }

    FORM_KERNEL::WIDGET_CHANGED();
}

/**************************************************************************************
                   FIS_KERNEL::WIDGET_CHANGED
***************************************************************************************/

void FIS_KERNEL::WIDGET_CHANGED()
{
    if (sender()->objectName() EQ "table_widget_child") {
        TABLE_WIDGETS_CHANGED();
    }

    FORM_KERNEL::WIDGET_CHANGED();
}

/**************************************************************************************
                   FIS_KERNEL::CURRENT_CELL_CHANGED
***************************************************************************************/

void FIS_KERNEL::CURRENT_CELL_CHANGED ( int current_row, int current_column, int previous_row, int previous_column)
{
    Q_UNUSED(current_column);

    int row_status;

    if ( current_row EQ -1  ) {
        return;
    }

    if ( previous_row EQ -1  OR previous_column EQ -1) {
        row_status         = p_table_widget->item(current_row, p_row_status_column_number )->text().toInt();
        CLOSE_ALL_EVENTS();
        SELECTED_LINE(current_row,row_status);
        OPEN_ALL_EVENTS();
        return;
    }

    if ( p_block_signals EQ true ) {
        return;
    }

    if ( current_row EQ previous_row ) {
        return;
    }

    int last_row_status;
    int last_row_number;

    if ( CHECK_ADD_NEW_ROW() EQ false ) {
        p_block_signals = true;        
        SET_TABLE_WIDGET_FOCUS(previous_row,previous_column);
        p_block_signals = false;
        return;
    }

    if (GET_RECORD_SAVE_STATUS() EQ EMPTY OR GET_RECORD_SAVE_STATUS() EQ NEED_ADD) {
        SET_RECORD_STATUS(NEED_ADD);
    }

    if ( GET_RECORD_SAVE_STATUS() EQ NEED_ADD OR GET_RECORD_SAVE_STATUS() EQ NEED_UPDATE ) {
        if (IS_FORM_CHANGED() EQ true) {
            int save_return_value = SAVE_RECORD();
            if ( save_return_value EQ RECORD_CANT_SAVED OR save_return_value EQ RECORD_EMPTY) {
                return;
            }
        }
    }

    last_row_number    = ( p_table_widget->rowCount() ) - 1;
    row_status         = p_table_widget->item(previous_row, p_row_status_column_number )->text().toInt();
    last_row_status    = p_table_widget->item(last_row_number, p_row_status_column_number )->text().toInt();

    switch ( row_status ) {
        case NEED_ADD:
            START_KERNEL_TRANSACTION();
            CLOSE_ALL_EVENTS();
            //if ( CHECK_ADD_LINE(GET_RECORD_ID(),previous_row) EQ ADAK_FAIL ) {
            //    CANCEL_KERNEL_TRANSACTION();
            //    OPEN_ALL_EVENTS();
            //    return;                
            //}
            ADAK_CURSOR_BUSY();
            ADD_LINE(GET_RECORD_ID(),previous_row);
            ADAK_CURSOR_NORMAL();
            OPEN_ALL_EVENTS();
            UPDATE_ROW_STATUS(previous_row, SAVED);
            ENABLE_DELETE_BUTTON(previous_row);
            COMMIT_KERNEL_TRANSACTION();
            SET_RECORD_STATUS(SAVED);
            UPDATE_BUTTON_STATUS();
            break;
        case NEED_UPDATE:
            START_KERNEL_TRANSACTION();
            CLOSE_ALL_EVENTS();
            //if ( CHECK_UPDATE_LINE(GET_RECORD_ID(),previous_row) EQ ADAK_FAIL ) {
            //    CANCEL_KERNEL_TRANSACTION();
            //    OPEN_ALL_EVENTS();
            //    return;                
            //}
            ADAK_CURSOR_BUSY();
            UPDATE_LINE(GET_RECORD_ID(),previous_row);
            ADAK_CURSOR_NORMAL();
            OPEN_ALL_EVENTS();
            UPDATE_ROW_STATUS(previous_row, SAVED);
            ENABLE_DELETE_BUTTON(previous_row);
            COMMIT_KERNEL_TRANSACTION();
            SET_RECORD_STATUS(SAVED);
            UPDATE_BUTTON_STATUS();
            break;
        case SAVED:
            SET_RECORD_STATUS(SAVED);
            UPDATE_BUTTON_STATUS();
        default:
            break;
    }

    m_satir_degisen_widget_count = 0;
    m_degisen_widget_counter     = 0;

    if (current_column NE -2) {
        row_status         = p_table_widget->item(current_row, p_row_status_column_number )->text().toInt();
        CLOSE_ALL_EVENTS();
        SELECTED_LINE(current_row,row_status);
        OPEN_ALL_EVENTS();
    }

    for ( int i = 0; i < p_table_widget->rowCount() ; i++ ) {
        row_status = p_table_widget->item(i, p_row_status_column_number)->text().toInt();
        if ( row_status EQ EMPTY OR row_status EQ NEED_ADD) {
            return;
        }
    }

    p_last_added_row = ADD_NEW_LINE( p_last_added_row + 1 , false);
}

/**************************************************************************************
                   FIS_KERNEL::CELL_BUTTON_CLICKED
***************************************************************************************/

void FIS_KERNEL::CELL_BUTTON_CLICKED()
{
    if(sender() EQ NULL) {
        return;
    }

    int focused_row = -1;

    for (int i = 0 ; i < p_push_button_columns.size() ; i++ ) {
        focused_row = FIND_FOCUSED_ROW(p_push_button_columns.at(i));
    }

    if (focused_row EQ -1) {
        return;
    }

    QAbstractButton * button = static_cast<QAbstractButton *>(sender());

    CALL_CHECK_LINE_VAR(focused_row,button);

}

/**************************************************************************************
                   FIS_KERNEL::LINE_SEARCH_BUTTON_CLICKED
***************************************************************************************/

void FIS_KERNEL::LINE_SEARCH_BUTTON_CLICKED(QWidget * widget,QLineEdit * line_edit,QToolButton * tool_button)
{
    if (m_search_clicked_started EQ true) {
        return;
    }

    m_search_clicked_started = true;

    int current_row;

    current_row = p_table_widget->currentRow();

    if (widget NE NULL) {
        for ( int i=0; i < p_search_edit_columns.size() ; i++  ) {
            for (int j=0; j < p_table_widget->rowCount() ; j++ ) {
                if (p_table_widget->cellWidget(j,p_search_edit_columns.at(i)) EQ widget ) {
                    current_row = j;
                    break;
                }
            }
        }
    }

    QString widget_degeri = line_edit->text();
    if (tool_button NE NULL) {
        LINE_SEARCH_EDIT_CLICKED(current_row,widget,line_edit);
        if ( widget_degeri NE line_edit->text() ) {

            int return_value = CALL_CHECK_LINE_VAR(current_row,widget);
            if (return_value EQ ADAK_OK AND line_edit->text().isEmpty() EQ false) {
                INCREASE_DEGISEN_WIDGET_COUNT(widget);
                TABLE_WIDGETS_CHANGED();
                SEND_TAB_KEY_EVENT();
                m_search_clicked_started = false;
                return;
            }
            else if(return_value EQ ADAK_FAIL_UNDO) {
                UNDO_CHANGES();
            }            
        }

        widget->setFocus();
    }

    m_search_clicked_started = false;
}

/**************************************************************************************
                   FIS_KERNEL::LINE_SEARC_EDIT_LINEEDIT_CHANGHED
***************************************************************************************/

void FIS_KERNEL::LINE_SEARCH_EDIT_LINEEDIT_CHANGED(QWidget * widget, QLineEdit * line_edit, QToolButton * tool_button)
{
    Q_UNUSED(tool_button);

    if (m_search_clicked_started EQ true) {
        return;
    }

    int current_row;

    current_row = p_table_widget->currentRow();

    if (widget NE NULL) {
        for ( int i=0; i < p_search_edit_columns.size() ; i++  )
            for (int j=0; j < p_table_widget->rowCount() ; j++ ) {
                if (p_table_widget->cellWidget(j,p_search_edit_columns.at(i)) EQ widget ) {
                    current_row = j;
                    break;
                }
            }
    }

    int return_value = CALL_CHECK_LINE_VAR(current_row,widget);

    if ( return_value EQ ADAK_FAIL ) {
        QString value = line_edit->text();
        line_edit->clear();
        if ( line_edit->hasFocus() EQ false) {
            line_edit->setFocus();
        }
        line_edit->setText(value);
    }
    else if (return_value EQ ADAK_OK) {
        ((QSearchEdit *)widget)->SET_TEXT(line_edit->text());
    }
}

/**************************************************************************************
                   FIS_KERNEL::SET_TABLE_WIDGET_FOCUS
***************************************************************************************/

void FIS_KERNEL::SET_TABLE_WIDGET_FOCUS (int row, int column)
{    
    QWidget * widget = p_table_widget->cellWidget(row,column);
    if ( widget NE NULL ) {        
        if ( widget->isEnabled() EQ false ) {
            SEND_TAB_KEY_EVENT(row,column);
            return;
        }
        if (widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() OR
            widget->metaObject()->className() EQ QCommaEdit::staticMetaObject.className()) {
            if ( static_cast<QLineEdit *>(widget)->isReadOnly() EQ true ) {
                SEND_TAB_KEY_EVENT(row,column);
                return;
            }
        }

        SET_LAST_FOCUSED_WIDGET(NULL);
        if ( widget NE NULL) {
            widget->clearFocus();
            widget->setFocus();
        }
    }
    else {
        SEND_TAB_KEY_EVENT(row,column);
    }
}


/**************************************************************************************
                   FIS_KERNEL::CALL_CHECK_LINE_VAR
***************************************************************************************/

int FIS_KERNEL::CALL_CHECK_LINE_VAR(int row_number, QObject *obj)
{
    CLOSE_ALL_EVENTS();
    int return_value = CHECK_LINE_VAR(row_number,obj);
    OPEN_ALL_EVENTS();

    return return_value;
}

/**************************************************************************************
                   FIS_KERNEL::LINE_SEARCH_EDIT_CLICKED
***************************************************************************************/

void FIS_KERNEL::LINE_SEARCH_EDIT_CLICKED ( int current_row, QWidget * widget,QLineEdit * line_edit )
{
    Q_UNUSED ( current_row );
    Q_UNUSED ( widget );
    Q_UNUSED ( line_edit);
}

/**************************************************************************************
                   FIS_KERNEL::REGISTER_TABLE_WIDGET
***************************************************************************************/

void FIS_KERNEL::REGISTER_TABLE_WIDGET ( QTableWidget * tablewidget,int column_number)
{
    tablewidget->setObjectName("FIS_KERNEL_TABLE_WIDGET");

    // +1 kernelin ekleyecegi row status icin
    p_number_of_column         = column_number + 3;
    p_table_widget             = tablewidget;
    p_row_status_column_number = p_number_of_column - 1;

    p_insert_button_column     = p_number_of_column - 3;
    p_delete_button_column     = p_insert_button_column + 1;

    tablewidget->setAccessibleName(QVariant(p_insert_button_column).toString() + "\t" + QVariant(p_delete_button_column).toString());

    SET_COMMA_EDIT_PRECISION(0,2);

    for(int i = 0; i < column_number ; i++) {
        p_widget_types << NO_TYPE;
    }

    p_table_widget->setColumnCount      ( p_number_of_column        );
}

/**************************************************************************************
                   FIS_KERNEL::GET_LAST_COLUMN_FOR_NEW_LINE
***************************************************************************************/

int FIS_KERNEL::GET_LAST_COLUMN_FOR_NEW_LINE()
{
    int current_row = p_table_widget->currentRow();

    for ( int i = (p_table_widget->columnCount() - 1) ; i >=0 ; i-- ) {
        QWidget * cell_widget = p_table_widget->cellWidget( current_row, i );

        if ( p_table_widget->columnWidth(i) < 2) {
            continue;
        }

        if ( cell_widget EQ NULL ) {
            continue;
        }

        if ( cell_widget->isEnabled() EQ true ) {
            if (cell_widget->metaObject()->className() EQ QToolButton::staticMetaObject.className() OR
                cell_widget->metaObject()->className() EQ QPushButton::staticMetaObject.className() ) {
                continue;
            }

            if ( p_widget_types.at(i) EQ WIDGET_COMMA_EDIT ) {
                QCommaEdit * comma_edit_current = static_cast<QCommaEdit *>(cell_widget);
                if ( comma_edit_current->isReadOnly() EQ true ) {
                    continue;
                }
            }
            else if ( p_widget_types.at(i) EQ WIDGET_LINE_EDIT ) {
                QLineEdit * line_edit_current = static_cast<QLineEdit *>(cell_widget);
                if ( line_edit_current->isReadOnly() EQ true ) {
                    continue;
                }
            }
            else if ( p_widget_types.at(i) EQ WIDGET_DATE_EDIT) {
                QDateEdit * date_edit_current  = static_cast<QDateEdit *>(cell_widget);
                if ( date_edit_current->isReadOnly() EQ true ) {
                    continue;
                }
            }

            return i;
        }
    }

    return -1;
}

/**************************************************************************************
                   FIS_KERNEL::SET_NUMBER_OF_HIDDEN_COLUMN
***************************************************************************************/

void FIS_KERNEL::SET_NUMBER_OF_HIDDEN_COLUMN ( int number_of_hidden_column )
{
    p_number_of_hidden_column = number_of_hidden_column;
    p_first_visible_column    = number_of_hidden_column;
}

/**************************************************************************************
                   FIS_KERNEL::SET_ROW_ID_COLUMN_POSITION
***************************************************************************************/

void FIS_KERNEL::SET_ROW_ID_COLUMN_POSITION ( int row_id_column_number )
{
    p_row_id_column_number = row_id_column_number;
}

/**************************************************************************************
                   FIS_KERNEL::SET_FIS_ORDER_COLUMN_NUMBER
***************************************************************************************/

void FIS_KERNEL::SET_FIS_ORDER_COLUMN_NUMBER ( int fis_order_column_number )
{
    p_fis_order_column_number = fis_order_column_number;
}

/**************************************************************************************
                   FIS_KERNEL::SET_FORM_ID_COLUMN_POSITION
***************************************************************************************/

void FIS_KERNEL::SET_FORM_ID_COLUMN_POSITION ( int form_id_column_number )
{
    p_form_id_column_number = form_id_column_number;
}

/**************************************************************************************
                   FIS_KERNEL::SET_FIS_ORDER_DATABASE
***************************************************************************************/

void FIS_KERNEL::SET_FIS_ORDER_DATABASE ( QString table_name,QString fis_order_column_name,QString line_id_column_name)
{
    p_db_table_name            = table_name;
    p_db_fis_order_column_name = fis_order_column_name;
    p_db_line_id_column_name   = line_id_column_name;
}

/**************************************************************************************
                   FIS_KERNEL::SET_TABLE_ROW_WIDGETS
***************************************************************************************/

void FIS_KERNEL::SET_TABLE_ROW_WIDGETS ( int column_number,ROW_WIDGET_TYPE type)
{
    if (p_widget_types.size() < (column_number + 1) ) {
        for(int i = p_widget_types.size() ; i < (column_number + 1) ; i++) {
            p_widget_types << NO_TYPE;
        }
    }

    p_widget_types.replace(column_number,type);
}
/**************************************************************************************
                   FIS_KERNEL::CHECK_UNSAVED_LINES
***************************************************************************************/

void FIS_KERNEL::CHECK_UNSAVED_LINES()
{
    int row_status;
    int table_has_empty_row;

    if ( CHECK_ADD_NEW_ROW() EQ false ) {
        SET_RECORD_STATUS(NEED_UPDATE);
        return;
    }

    table_has_empty_row = false;

    for ( int i = 0; i < p_table_widget->rowCount() ; i++ ) {
        row_status = p_table_widget->item(i, p_row_status_column_number)->text().toInt();
        switch( row_status ) {
            case NEED_ADD:
                CLOSE_ALL_EVENTS();
                //if ( CHECK_ADD_LINE(GET_RECORD_ID(),i) EQ ADAK_FAIL ) {
                //    OPEN_ALL_EVENTS();
                //    SET_RECORD_STATUS(NEED_UPDATE);
                //    return;
                //}
                ADAK_CURSOR_BUSY();
                ADD_LINE( GET_RECORD_ID(),i );
                ADAK_CURSOR_NORMAL();
                OPEN_ALL_EVENTS();
                UPDATE_ROW_STATUS(i, SAVED);
                ENABLE_DELETE_BUTTON( i );
                break;
            case NEED_UPDATE:
                CLOSE_ALL_EVENTS();
                //if ( CHECK_UPDATE_LINE(GET_RECORD_ID(),i) EQ ADAK_FAIL ) {
                //    OPEN_ALL_EVENTS();
                //    SET_RECORD_STATUS(NEED_UPDATE);
                //    return;
                //}
                ADAK_CURSOR_BUSY();
                UPDATE_LINE( GET_RECORD_ID(),i );
                ADAK_CURSOR_NORMAL();
                OPEN_ALL_EVENTS();
                UPDATE_ROW_STATUS(i, SAVED);
                ENABLE_DELETE_BUTTON( i );
                if (i+1 < p_table_widget->rowCount()) {
                    SET_TABLE_WIDGET_FOCUS(i+1,p_first_visible_column);
                }
                break;
            case EMPTY:
                table_has_empty_row = true;
                break;
            default:
                break;
        }
    }

    if ( table_has_empty_row EQ false ) {
        CHECK_ADD_EMPTY_ROW();
    }

    m_satir_degisen_widget_count = 0;
    m_degisen_widget_counter     = 0;
}

/**************************************************************************************
                   FIS_KERNEL::NEW_RECORD
***************************************************************************************/

void FIS_KERNEL::NEW_RECORD()
{
    p_current_row    = 0;
    CLEAR_TABLE_WIDGET();
    SET_RECORD_STATUS(EMPTY);
    CLOSE_ALL_EVENTS();
    NEW_FIS_RECORD();
    OPEN_ALL_EVENTS();
    p_last_added_row = ADD_NEW_LINE( p_current_row , false);
    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   FIS_KERNEL::CLEAR_TABLE_WIDGET
***************************************************************************************/

void FIS_KERNEL::CLEAR_TABLE_WIDGET()
{
    p_block_signals = true;

    for ( int i = p_table_widget->rowCount()-1; i >= 0 ; i-- ) {
        p_table_widget->removeRow( i );
    }
    RESET_KERNEL_VARIABLES();
    p_block_signals = false;
}

/**************************************************************************************
                   FIS_KERNEL::GET_RECORD
***************************************************************************************/

int FIS_KERNEL::GET_RECORD ( int fis_id )
{
    CLEAR_TABLE_WIDGET();
    p_is_table_filling = true;
    CLOSE_ALL_EVENTS();
    int return_value = GET_FIS_RECORD(fis_id);
    OPEN_ALL_EVENTS();
    p_is_table_filling = false;

    for ( int i = 0; i < p_table_widget->rowCount(); i++ ) {
        UPDATE_ROW_STATUS( i, SAVED);
        ENABLE_DELETE_BUTTON( i );
    }

    p_last_added_row = ADD_NEW_LINE(p_table_widget->rowCount(), false);

    return return_value;
}

/**************************************************************************************
                   FIS_KERNEL::CHECK_VAR
***************************************************************************************/

int FIS_KERNEL::CHECK_VAR ( QObject *object )
{
    if( object->objectName() EQ "search_edit_line_edit" OR object->objectName() EQ "search_edit_tool_button") {
        if( object->parent()->parent()->parent() EQ p_table_widget ) {
            return ADAK_OK;
        }
    }

    if ( object->parent()->parent() NE NULL) {
        if ( object->parent()->parent()->objectName() NE p_table_widget->objectName()) {
            CLOSE_ALL_EVENTS();
            int return_value = CHECK_FIS_FORM_VAR(object);
            OPEN_ALL_EVENTS();
            return return_value;
        }
    }
    else {
        if ( object->parent()->objectName() NE p_table_widget->objectName()) {
             CLOSE_ALL_EVENTS();
             int return_value = CHECK_FIS_FORM_VAR(object);
             OPEN_ALL_EVENTS();
             return return_value;
        }
    }

    int current_row    = p_table_widget->currentRow();

    if ( current_row < 0 ) {
        return ADAK_OK;
    }

    if ( p_table_widget->rowCount() > 0 ) {
        return CALL_CHECK_LINE_VAR(current_row, object);
    }
    return ADAK_OK;
}

/**************************************************************************************
                   FIS_KERNEL::CHECK_EMPTY
***************************************************************************************/

int FIS_KERNEL::CHECK_EMPTY ()
{
    if (IS_FORM_CHANGED() EQ false) {
        return ADAK_OK;
    }

    CLOSE_ALL_EVENTS();
    int return_value  = CHECK_FIS_FORM_EMPTY();
    OPEN_ALL_EVENTS();
    return return_value;
}

/**************************************************************************************
                   FIS_KERNEL::CHECK_ADD
***************************************************************************************/

int FIS_KERNEL::CHECK_ADD ()
{
    if (IS_FORM_CHANGED() EQ false) {
        return ADAK_OK;
    }

    return CHECK_FIS_RECORD_ADD();
}

/**************************************************************************************
                   FIS_KERNEL::ADD_RECORD
***************************************************************************************/

int FIS_KERNEL::ADD_RECORD ()
{
    p_table_widget->blockSignals(true);

    if (IS_FORM_CHANGED() EQ true) {
        CLOSE_ALL_EVENTS();
        ADAK_CURSOR_BUSY();
        SET_CURRENT_RECORD_ID(ADD_FIS_RECORD());
        ADAK_CURSOR_NORMAL();
        OPEN_ALL_EVENTS();        
    }

    SET_RECORD_STATUS(SAVED);

    CHECK_UNSAVED_LINES();

    p_table_widget->blockSignals(false);

    return GET_RECORD_ID();
}

/**************************************************************************************
                   FIS_KERNEL::CHECK_UPDATE
***************************************************************************************/

int FIS_KERNEL::CHECK_UPDATE ( int fis_id )
{
    if ( IS_FORM_CHANGED() EQ false ) {
         return ADAK_OK;
    }

    return CHECK_FIS_RECORD_UPDATE(fis_id);
}

/**************************************************************************************
                   FIS_KERNEL::UPDATE_RECORD
***************************************************************************************/

void FIS_KERNEL::UPDATE_RECORD ( int fis_id )
{
    p_table_widget->blockSignals(true);

    if ( IS_FORM_CHANGED() EQ true) {
         CLOSE_ALL_EVENTS();
         ADAK_CURSOR_BUSY();
         UPDATE_FIS_RECORD(fis_id);
         ADAK_CURSOR_NORMAL();
         OPEN_ALL_EVENTS();         
    }

    SET_RECORD_STATUS(SAVED);

    CHECK_UNSAVED_LINES();

    p_table_widget->blockSignals(false);
}

/**************************************************************************************
                   FIS_KERNEL::CHECK_DELETE
***************************************************************************************/

int FIS_KERNEL::CHECK_DELETE ( int fis_id )
{
    return CHECK_FIS_RECORD_DELETE ( fis_id );
}

/**************************************************************************************
                   FIS_KERNEL::DELETE_RECORD
***************************************************************************************/

void FIS_KERNEL::DELETE_RECORD ( int fis_id )
{
    CLOSE_ALL_EVENTS();
    ADAK_CURSOR_BUSY();
    DELETE_FIS_RECORD ( fis_id );
    ADAK_CURSOR_NORMAL();
    OPEN_ALL_EVENTS();
}

/**************************************************************************************
                    FIS_KERNEL::FIND_FIRST_RECORD
***************************************************************************************/

int FIS_KERNEL::FIND_FIRST_RECORD()
{
    return FIND_FIRST_FIS_RECORD();
}

/**************************************************************************************
                    FIS_KERNEL::FIND_LAST_RECORD
***************************************************************************************/

int FIS_KERNEL::FIND_LAST_RECORD ()
{
    return FIND_LAST_FIS_RECORD();
}

/**************************************************************************************
                    FIS_KERNEL::FIND_NEXT_RECORD
***************************************************************************************/

int FIS_KERNEL::FIND_NEXT_RECORD ()
{
    return FIND_NEXT_FIS_RECORD();
}

/**************************************************************************************
                    FIS_KERNEL::FIND_PREV_RECORD
***************************************************************************************/

int FIS_KERNEL::FIND_PREV_RECORD ()
{
    return FIND_PREV_FIS_RECORD();
}

/**************************************************************************************
                    FIS_KERNEL::SELECT_RECORD
***************************************************************************************/

int FIS_KERNEL::SELECT_RECORD()
{
    return SELECT_FIS_RECORD();
}

/**************************************************************************************
                    FIS_KERNEL::LOCK_RECORD
***************************************************************************************/

int FIS_KERNEL::LOCK_RECORD ( int fis_id )
{
    return LOCK_FIS_RECORD ( fis_id );
}

/**************************************************************************************
                    FIS_KERNEL::UNLOCK_RECORD
***************************************************************************************/

void FIS_KERNEL::UNLOCK_RECORD ( int fis_id )
{
    UNLOCK_FIS_RECORD ( fis_id );
}

/**************************************************************************************
                    FIS_KERNEL::FIND_RECORD
***************************************************************************************/

int FIS_KERNEL::FIND_RECORD()
{
    return FIND_FIS_RECORD();
}


/**************************************************************************************
                    FIS_KERNEL::SET_LINE_BUTTON_ICON
***************************************************************************************/

void FIS_KERNEL::SET_LINE_BUTTON_ICON ( QStringList icon )
{
    p_line_buttons_icon_name = icon;
}


/**************************************************************************************
                    FIS_KERNEL::SET_LINE_CHECK_BOX_TEXT
***************************************************************************************/


void FIS_KERNEL::SET_LINE_CHECK_BOX_TEXT (  QStringList check_box_names )
{
    p_line_check_box_names = check_box_names;
}


/**************************************************************************************
                    FIS_KERNEL::ADD_PUSH_BUTTON
***************************************************************************************/

void FIS_KERNEL::ADD_PUSH_BUTTON ( int row, int column )
{
    QPushButton * push_button = new QPushButton ( p_table_widget );
    push_button->setMinimumSize ( QSize(0,0) );
    push_button->setMaximumSize ( QSize(9999,32) );
    push_button->setIcon ( QIcon(p_line_buttons_icon_name.front() ) );
    push_button->setIconSize ( QSize(20,20) );
    push_button->setAutoDefault ( false );
    push_button->setText("");
    push_button->setObjectName ( "table_widget_child" );
    push_button->show();
    push_button->installEventFilter(this);

    p_line_buttons_icon_name << p_line_buttons_icon_name.front();
    p_line_buttons_icon_name.pop_front();
    connect(push_button, SIGNAL( clicked()), this, SLOT ( CELL_BUTTON_CLICKED() ) );
    p_table_widget->setCellWidget ( row, column, push_button );

}



/**************************************************************************************
                    FIS_KERNEL::ADD_INSERT_BUTTON
***************************************************************************************/

void FIS_KERNEL::ADD_INSERT_BUTTON ( int current_row )
{
    QPushButton * insert_button = new QPushButton ( p_table_widget );
    insert_button->setMinimumSize(QSize(0,0));
    insert_button->setMaximumSize(QSize(50,32));
    insert_button->setIcon(QIcon(":/kernel_icons/kernel_ust_satira_ekle.png"));
    insert_button->setIconSize(QSize(20,20));
    insert_button->setAutoDefault(false);
    insert_button->setText("");
    insert_button->setToolTip(tr("Tıklanan satırın üstüne satır ekler"));
    insert_button->show();

    if ( m_is_readonly_mode_on EQ true) {
        insert_button->setDisabled(true);
    }

    connect(insert_button, SIGNAL(clicked()), this, SLOT(INSERT_ROW()));
    p_table_widget->setCellWidget(current_row, p_insert_button_column, insert_button);
}

/**************************************************************************************
                    FIS_KERNEL::ADD_DELETE_BUTTON
***************************************************************************************/

void FIS_KERNEL::ADD_DELETE_BUTTON ( int current_row )
{
    QPushButton * delete_button = new QPushButton(p_table_widget);
    delete_button->setMinimumSize(QSize(0,0));
    delete_button->setMaximumSize(QSize(50,32));
    delete_button->setIcon(QIcon(":/kernel_icons/kernel_sil_ikonu.png"));
    delete_button->setIconSize(QSize(20,20));
    delete_button->setAutoDefault(false);
    delete_button->setText("");
    delete_button->setToolTip(tr("Tıklanan satırı siler"));
    delete_button->setEnabled(false);
    delete_button->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    delete_button->show();

    connect(delete_button, SIGNAL(clicked()), this, SLOT(DELETE_ROW()));
    p_table_widget->setCellWidget(current_row, p_delete_button_column, delete_button);
}

/**************************************************************************************
                    FIS_KERNEL::INSERT_ROW
***************************************************************************************/

void FIS_KERNEL::INSERT_ROW()
{
    int row_status;
    int empty_row;
    int focus_current_row;
    int current_row;

    focus_current_row = FIND_FOCUSED_ROW(p_insert_button_column);

    if ( focus_current_row >= 0) {
        current_row = focus_current_row;
    }
    else {
        current_row = p_table_widget->currentRow();
    }

    if ( SAVE_RECORD() EQ RECORD_CANT_SAVED ) {
        return;
    }

    if ( GET_RECORD_SAVE_STATUS() EQ NEED_UPDATE ) {
        return;
    }

    if ( CHECK_ADD_NEW_ROW() EQ false ) {
        return;
    }

    if ( p_table_widget->rowCount() EQ 1 ) {
        row_status = p_table_widget->item(0, p_row_status_column_number)->text().toInt();
        if ( row_status EQ EMPTY) {
            return;
        }
    }

    empty_row = -1;

    if ( current_row EQ -1 ) {
        return;
    }
    
    for ( int i = 0; i < p_table_widget->rowCount() ; i++ ) {
        row_status = p_table_widget->item(i, p_row_status_column_number)->text().toInt();
        if ( row_status EQ EMPTY) {
            empty_row = i;
            break;
        }
    }

    if ( empty_row NE -1 ) {
        p_table_widget->removeRow( empty_row );
        RESET_KERNEL_VARIABLES();
        if ( current_row > empty_row ) {
            current_row -= 1;
        }
    }
    p_last_added_row = ADD_NEW_LINE( current_row , false);
    SET_TABLE_WIDGET_FOCUS(current_row,p_first_visible_column);
    ENABLE_DELETE_BUTTON(p_last_added_row);
}

/**************************************************************************************
                    FIS_KERNEL::DELETE_ROW
***************************************************************************************/

void FIS_KERNEL::DELETE_ROW()
{
    int  row_status;
    int  current_row;
    bool sender_button = false;

    if (sender()->metaObject()->className() EQ QPushButton::staticMetaObject.className()) {
        QPushButton * button = static_cast<QPushButton *>(sender());
        button->setFocus();
        sender_button = true;
    }

    int focused_button_row = FIND_FOCUSED_ROW ( p_delete_button_column );

    if ( focused_button_row >= 0 ) {
        current_row = focused_button_row;
    }
    else {
        current_row = p_table_widget->currentRow();
    }

    if ( current_row EQ -1 ) {
        return;
    }

    if (sender_button EQ true) {
        if (focused_button_row EQ -1) {
            return;
        }
    }

    row_status = p_table_widget->item ( current_row, p_row_status_column_number)->text().toInt();

    if ( row_status EQ NEED_ADD OR row_status EQ EMPTY ) {
        p_block_signals = true;
        p_table_widget->removeRow(current_row);
        RESET_KERNEL_VARIABLES();
        SET_TABLE_WIDGET_FOCUS(p_table_widget->rowCount(),p_first_visible_column);
        p_block_signals = false;
        p_last_added_row = ADD_NEW_LINE(p_table_widget->rowCount(), false);
    }
    else {
        START_KERNEL_TRANSACTION();
        if ( CHECK_DELETE_LINE(GET_RECORD_ID(),current_row) EQ ADAK_OK ) {
            CLOSE_ALL_EVENTS();
            ADAK_CURSOR_BUSY();
            DELETE_LINE(GET_RECORD_ID(),current_row);
            ADAK_CURSOR_NORMAL();
            OPEN_ALL_EVENTS();
            COMMIT_KERNEL_TRANSACTION();
        }
        else {
            CANCEL_KERNEL_TRANSACTION();
	    return;
        }


        p_block_signals = true;
        p_table_widget->removeRow(current_row);
        RESET_KERNEL_VARIABLES();
        p_block_signals = false;
    }

    if (current_row < p_table_widget->rowCount()) {
        SET_TABLE_WIDGET_FOCUS(current_row,p_first_visible_column);
    }
    else {
        for ( int i = 0; i < p_table_widget->rowCount() ; i++ ) {
            row_status = p_table_widget->item(i, p_row_status_column_number)->text().toInt();
            if ( row_status EQ EMPTY ) {
                SET_TABLE_WIDGET_FOCUS(i,p_first_visible_column);
            }
        }
    }
}

/**************************************************************************************
                    FIS_KERNEL::ENABLE_DELETE_BUTTON
***************************************************************************************/

void FIS_KERNEL::ENABLE_DELETE_BUTTON ( int button_row )
{
    // FIX_ME fonksiyon ismi ENABLE_LINE_BUTTONS olarak degistirilmeli

    if ( m_is_readonly_mode_on EQ true) {
        return;
    }

    QWidget * cell_widget     = p_table_widget->cellWidget( button_row, p_delete_button_column);
    QPushButton * push_button = static_cast<QPushButton *>(cell_widget);
    if (push_button NE NULL) {
        push_button->setEnabled(true);
    }

    int row_status  = p_table_widget->item(button_row, p_row_status_column_number)->text().toInt();

    if (row_status EQ EMPTY) {
        return;
    }

    for (int i = 0 ; i < m_saver_buttons_column.size() ; i++ ) {
        QWidget *         widget = p_table_widget->cellWidget(button_row,m_saver_buttons_column.at(i));
        if (widget NE NULL) {
            QAbstractButton * button = KERNEL_FIND_BUTTON_WIDGET(widget);
            if (button NE NULL)  {
                button->setEnabled(true);
            }
        }
    }
}

/**************************************************************************************
                    FIS_KERNEL::DISABLE_DELETE_BUTTON
***************************************************************************************/

void FIS_KERNEL::DISABLE_DELETE_BUTTON ( int button_row )
{
    // FIX_ME fonksiyon ismi DISABLE_LINE_BUTTONS olarak degistirilmeli

    QWidget * cell_widget     = p_table_widget->cellWidget( button_row, p_delete_button_column);
    QPushButton * push_button = static_cast<QPushButton *>(cell_widget);
    if (push_button NE NULL) {
        push_button->setDisabled(true);;
    }

    int row_status  = p_table_widget->item(button_row, p_row_status_column_number)->text().toInt();

    if (row_status NE EMPTY) {
        return;
    }

    for (int i = 0 ; i < m_saver_buttons_column.size() ; i++ ) {
        QWidget *         widget = p_table_widget->cellWidget(button_row,m_saver_buttons_column.at(i));
        if (widget NE NULL) {
            QAbstractButton * button = KERNEL_FIND_BUTTON_WIDGET(widget);
            if (button NE NULL)  {
                button->setDisabled(true);
            }
        }
    }
}

/**************************************************************************************
                    FIS_KERNEL::FIND_FOCUSED_ROW
***************************************************************************************/

int FIS_KERNEL::FIND_FOCUSED_ROW ( int column_number )
{
    int i;

    for( i = 0 ; i <= p_table_widget->rowCount(); i++ ) {
        if ( i EQ p_table_widget->rowCount() ) {
            return -1;
        }
        QWidget * cell_widget = p_table_widget->cellWidget ( i, column_number);
        if ( cell_widget->hasFocus() ) {
            break;
        }
    }
    return i;
}

/**************************************************************************************
                    FIS_KERNEL::UPDATE_ROW_STATUS
***************************************************************************************/

void FIS_KERNEL::UPDATE_ROW_STATUS ( int row_number, RECORD_SAVE_STATUS type )
{
    QTableWidgetItem * new_item;
    switch( type ) {
        case SAVED:
            new_item = new QTableWidgetItem ( tr ( "%1").arg ( SAVED) ) ;
            break;
        case NEED_UPDATE:
            new_item = new QTableWidgetItem ( tr( "%1" ).arg ( NEED_UPDATE));
            break;
        case NEED_ADD:
            new_item = new QTableWidgetItem ( tr ( "%1").arg(NEED_ADD));
            break;
        case EMPTY:
        default:
            new_item = new QTableWidgetItem ( tr ( "%1").arg(EMPTY));
            break;
    }
    p_table_widget->setItem(row_number, p_row_status_column_number, new_item);
}

/**************************************************************************************
                    FIS_KERNEL::CHECK_ADD_EMPTY_ROW
***************************************************************************************/

void FIS_KERNEL::CHECK_ADD_EMPTY_ROW()
{
    int row_status;
    int last_row_number;

    if ( CHECK_ADD_NEW_ROW() EQ false ) {
        return;
    }

    last_row_number = p_table_widget->rowCount();
    row_status      = p_table_widget->item(last_row_number - 1, p_row_status_column_number)->text().toInt();

    if ( row_status NE EMPTY ) {
        //Miras alindigindan form da baslatilan transaction buraya geliyor.
        //Simdilik transaction tamamlanip devam edilecek basekernel da ideal duruma dondurulmeli.
        COMMIT_KERNEL_TRANSACTION();
        p_last_added_row = ADD_NEW_LINE( p_last_added_row + 1 , false);
        START_KERNEL_TRANSACTION();
    }
    else {
        SET_TABLE_WIDGET_FOCUS( last_row_number -1 , p_first_visible_column);
    }

    for ( int i = 0; i < last_row_number ; i++ ) {
        if (i EQ p_last_added_row) {
            continue;
        }
        row_status = p_table_widget->item(i, p_row_status_column_number)->text().toInt();
        if ( row_status EQ EMPTY) {
            p_table_widget->removeRow( i );
            RESET_KERNEL_VARIABLES();
        }
    }
}

/**************************************************************************************
                    FIS_KERNEL::CHECK_ADD_NEW_ROW
***************************************************************************************/

bool FIS_KERNEL::CHECK_ADD_NEW_ROW()
{
    int row_status;

    CLOSE_ALL_EVENTS ();  // FIXME
    for( int i = 0; i < p_table_widget->rowCount(); i++ ) {
        row_status = p_table_widget->item(i, p_row_status_column_number)->text().toInt();
        if ( row_status EQ NEED_ADD OR row_status EQ NEED_UPDATE ) {
            if ( CHECK_LINE_EMPTY( i ) EQ ADAK_FAIL ) {
                OPEN_ALL_EVENTS ();
                return false;
            }
            if ( row_status EQ NEED_ADD ) {
                if ( CHECK_ADD_LINE( GET_RECORD_ID() , i ) EQ ADAK_FAIL ) {
                    OPEN_ALL_EVENTS ();
                    return false;
                }
            }
            else {
                if ( CHECK_UPDATE_LINE(  GET_RECORD_ID() , i ) EQ ADAK_FAIL ) {
                    OPEN_ALL_EVENTS ();
                    return false;
                }
            }
        }
    }
    OPEN_ALL_EVENTS ();
    return true;
}

/**************************************************************************************
                    FIS_KERNEL::eventFilter
***************************************************************************************/

bool FIS_KERNEL::eventFilter ( QObject *obj, QEvent *event )
{
    // !!!! FOCUS IN de fis e ozel bir durum var eger satir daki bir widget sa
    // Checkvar dan onay alindiktan sonra setCurrentCell yapilmali.
    // FIX_ME
    // Bi kac kod duplication var genel bir duzenlenmede bu yapidaki bir kac kod duplication da duzeltilmeli.

    if ( event->type() EQ QEvent::FocusIn ) {
        bool focus_in = FORM_KERNEL::eventFilter(obj,event);

        if ( focus_in EQ false) {
            if ( p_focus_in_event_status EQ true ) {
                return false;
            }
            p_focus_in_event_status = true;
            if ( obj->objectName() NE "table_widget_child" AND obj->objectName() NE "line_changer_button" AND obj->objectName() NE "line_saver_button") {
                QObject * parent = obj->parent();
                if ( parent EQ NULL ) {
                    p_focus_in_event_status = false;
                    return focus_in;
                }
                if ( parent->objectName() NE "table_widget_child") {
                    p_focus_in_event_status = false;
                    return focus_in;
                }
            }

            for (int i = 0; i < p_table_widget->rowCount(); i++ ) {
                for ( int j = 0; j < p_number_of_column; j++ ) {
                    QWidget * cell_widget = p_table_widget->cellWidget( i, j);
                    if (!cell_widget) {
                        continue;
                    }
                    else if ( obj EQ cell_widget ) {
                        if (i NE -1 AND i NE p_table_widget->currentRow() AND p_table_widget->currentRow() NE -1 ) {
                            if ( CHECK_ADD_NEW_ROW() EQ true ) {
                                p_focus_in_event_status = false;
                                p_table_widget->setCurrentCell( i, j );
                                return false;
                            }
                            p_focus_in_event_status = false;
                            return true;
                        }
                        else if ( (i EQ p_table_widget->currentRow() AND j NE p_table_widget->currentColumn()) OR ( p_table_widget->currentRow() EQ -1 ) ) {
                            p_table_widget->setCurrentCell( i, j );
                            p_focus_in_event_status = false;
                            return false;
                        }
                    }
                    else if (cell_widget->findChild<QLineEdit *>("search_edit_line_edit") NE NULL OR cell_widget->findChild<QLineEdit *>("adak_date_line_edit") NE NULL) {
                        if (obj EQ cell_widget->findChild<QLineEdit *>("search_edit_line_edit") OR obj EQ cell_widget->findChild<QLineEdit *>("adak_date_line_edit")) {
                            if (i NE -1 AND i NE p_table_widget->currentRow() AND p_table_widget->currentRow() NE -1 ) {
                                if ( CHECK_ADD_NEW_ROW() EQ false ) {
                                    p_focus_in_event_status = false;
                                    return true;
                                }
                            }
                            p_table_widget->setCurrentCell( i, j );
                        }
                    }
                    else if (cell_widget->findChild<QToolButton *>("search_edit_tool_button") NE NULL OR cell_widget->findChild<QToolButton *>("adak_date_tool_button") NE NULL) {
                        if ( obj EQ cell_widget->findChild<QToolButton *>("search_edit_tool_button") OR obj EQ cell_widget->findChild<QToolButton *>("adak_date_tool_button")) {
                            if (i NE -1 AND i NE p_table_widget->currentRow() AND p_table_widget->currentRow() NE -1 ) {
                                if ( CHECK_ADD_NEW_ROW() EQ false ) {
                                    p_focus_in_event_status = false;
                                    return true;
                                }
                            }
                            p_table_widget->setCurrentCell( i, j );
                        }
                    }
                }
            }
        }

        p_focus_in_event_status = false;
        return focus_in;
    }

    //--------------------- USER VE RESIZE EVENTS

    if (event->type() EQ QEvent::User OR event->type() EQ QEvent::Resize) {
        int row    = p_table_widget->currentRow();
        int column = p_table_widget->currentColumn();
        bool return_value = FORM_KERNEL::eventFilter(obj,event);
        if (row NE -1 AND column NE -1) {
           SET_TABLE_WIDGET_FOCUS(row,column);
        }
        return return_value;
    }

    if ( event->type() EQ QEvent::MouseButtonPress ) {
        for (int i = 0; i < p_table_widget->rowCount(); i++ ) {
            for ( int j = 0; j < p_number_of_column; j++ ) {
                QWidget * cell_widget = p_table_widget->cellWidget( i, j);
                if (!cell_widget) {
                    continue;
                }
                if ( cell_widget->hasFocus() EQ true) {
                    SET_TABLE_WIDGET_FOCUS( i, j);
                    return false;
                }
                QLineEdit * line_edit = cell_widget->findChild<QLineEdit *>("search_edit_line_edit");
                if ( line_edit EQ NULL ) {
                    line_edit = cell_widget->findChild<QLineEdit *>("adak_date_line_edit");
                }

                if (line_edit NE NULL) {
                    if (line_edit->hasFocus() EQ true) {
                        SET_TABLE_WIDGET_FOCUS( i, j);
                        return false;
                    }
                }
                QToolButton * tool_button = cell_widget->findChild<QToolButton *>("search_edit_tool_button");
                if ( tool_button EQ NULL ) {
                    tool_button = cell_widget->findChild<QToolButton *>("adak_date_tool_button");
                }
                if (tool_button NE NULL) {
                    if (tool_button->hasFocus() EQ true) {
                        SET_TABLE_WIDGET_FOCUS( i, j);
                        tool_button->setFocus();
                        return false;
                    }
                }
            }
        }
    }

    if ( event->type() NE QEvent::KeyPress ) {
        if (obj EQ p_table_widget) {
            return false;
        }
        return FORM_KERNEL::eventFilter(obj,event);
    }

    if (obj->parent() NE NULL) {
        if (((obj->objectName() EQ "table_widget_child")
        OR (obj->parent()->objectName()  EQ "table_widget_child")
        OR (p_registered_line_widget.contains(static_cast<QWidget *>(obj)))
        OR (obj EQ p_table_widget )) EQ false){
            return FORM_KERNEL::eventFilter(obj,event);
        }
    }
    else {
        if (((obj->objectName() EQ "table_widget_child")
        OR (p_registered_line_widget.contains(static_cast<QWidget *>(obj)))
        OR (obj EQ p_table_widget )) EQ false){
            return FORM_KERNEL::eventFilter(obj,event);
        }
    }

    QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

    if (keyEvent->key() EQ Qt::Key_Down) {
        if (obj->metaObject()->className() EQ QComboBox::staticMetaObject.className()) {
            return FORM_KERNEL::eventFilter(obj,event);
        }

        int p_row     = p_table_widget->currentRow();
        int p_column  = p_table_widget->currentColumn();

        if (p_row + 1 < p_table_widget->rowCount() ) {
            QWidget * widget = p_table_widget->cellWidget(p_row + 1 , p_column);
            if (widget NE NULL) {
                widget->setFocus();
            }
            return true;
        }
    }
    else if (keyEvent->key() EQ Qt::Key_Up) {
        if (obj->metaObject()->className() EQ QComboBox::staticMetaObject.className()) {
            return FORM_KERNEL::eventFilter(obj,event);
        }

        int p_row     = p_table_widget->currentRow();
        int p_column  = p_table_widget->currentColumn();

        if (p_row - 1 < p_table_widget->rowCount() ) {
            QWidget * widget = p_table_widget->cellWidget(p_row - 1 , p_column);
            if (widget NE NULL) {
                widget->setFocus();
            }
            return true;
        }
    }


    if (( (keyEvent->key() EQ Qt::Key_Enter) OR (keyEvent->key() EQ Qt::Key_Return) OR (keyEvent->key() EQ Qt::Key_Tab)) EQ false ) {
        return FORM_KERNEL::eventFilter(obj,event);
    }

    int current_column;
    current_column = p_table_widget->currentColumn();

    int list_size = p_registered_line_widget.size();
    int visible_widgets = list_size;
    int first_visible_widget_index = -1;
    for (int i=0; i < list_size; i++) {
        if (p_registered_line_widget.at(i)->isHidden() EQ true) {
            visible_widgets--;
        }
        if (p_registered_line_widget.at(i)->isHidden() EQ false AND first_visible_widget_index EQ -1 ) {
            first_visible_widget_index = i;
        }

    }

    if (visible_widgets > 0) {
        for (int i=0; i < list_size; i++) {
            if (p_registered_line_widget.at(i)->isHidden() EQ true) {
                continue;
            }
            if(p_registered_line_widget.at(i)->hasFocus() EQ true) {
                if (i + 1 < visible_widgets) {
                    p_registered_line_widget.at(i+1)->setFocus();
                }
                else {
                    int save_return_value = SAVE_FIS_RECORD();
                    if(save_return_value EQ RECORD_ALREADY_SAVED OR save_return_value EQ RECORD_SAVED ) {
                        int current_row;
                        current_row = p_table_widget->currentRow();
                        SET_TABLE_WIDGET_FOCUS(current_row,p_first_visible_column);
                    }
                }
                return true;
            }
        }

        if( current_column EQ GET_LAST_COLUMN_FOR_NEW_LINE()) {
            p_registered_line_widget.at(first_visible_widget_index)->setFocus();
            return true;//FORM_KERNEL::eventFilter(obj,event);
        }
    }

    if( current_column EQ GET_LAST_COLUMN_FOR_NEW_LINE()) {
        int save_return_value = SAVE_FIS_RECORD();

        if(save_return_value EQ RECORD_CANT_SAVED OR save_return_value EQ RECORD_EMPTY) {
            return true;//FORM_KERNEL::eventFilter(obj,event);
        }
        else if(save_return_value EQ RECORD_ALREADY_SAVED OR save_return_value EQ RECORD_SAVED ) {
            int current_row = p_table_widget->currentRow();
            if (current_row < p_table_widget->rowCount()) {
                SET_TABLE_WIDGET_FOCUS(current_row,p_first_visible_column);
            }
        }
    }
    else {
        SEND_TAB_KEY_EVENT();
        return true;
    }

    return true;//FORM_KERNEL::eventFilter(obj,event);
}

/**************************************************************************************
                    FIS_KERNEL::SEND_TAB_KEY_EVENT
***************************************************************************************/

void FIS_KERNEL::SEND_TAB_KEY_EVENT ( int current_row, int current_column )
{
    if ( current_row EQ -1) {
        current_row = 0;
        if (p_table_widget->currentRow() NE -1) {
            current_row    = p_table_widget->currentRow();
        }
    }

    if ( current_column EQ -1) {
         current_column = p_table_widget->currentColumn();
    }

    int next_column    = current_column + 1;

    if (next_column > GET_LAST_COLUMN_FOR_NEW_LINE()) {
        return;
    }

    for ( ; ; ) {
        if ( p_table_widget->columnWidth(next_column) > 0 OR next_column >= p_table_widget->columnCount() ) {
            break;
        }
        next_column++;
    }

    if ( next_column > p_table_widget->columnCount()) {
         next_column = 0;
    }

    SET_TABLE_WIDGET_FOCUS(current_row,next_column);
}

/**************************************************************************************
                    FIS_KERNEL::REGISTER_BUTTON_SHORT_CUT
***************************************************************************************/

void FIS_KERNEL::REGISTER_BUTTON_SHORT_CUT ( int column,QKeySequence key )
{
    REMOVE_BUTTON_SHORT_CUT (column);

    QAction * action_short_cut = new QAction(this);
    action_short_cut->setShortcut(key);
    action_short_cut->setObjectName(QVariant(column).toString());

    p_table_widget_actions << action_short_cut;
    p_table_widget->addAction(action_short_cut);

    connect(action_short_cut,SIGNAL(triggered()),this,SLOT(SLOT_SHORT_CUT_PRESSED()));
}

/**************************************************************************************
                    FIS_KERNEL::REMOVE_BUTTON_SHORT_CUT
***************************************************************************************/

void FIS_KERNEL::REMOVE_BUTTON_SHORT_CUT ( int column )
{
    for ( int i = 0 ; i < p_table_widget_actions.size() ; i++ ) {
        if (p_table_widget_actions.at(i)->objectName().toInt() EQ column) {
            p_table_widget->removeAction(p_table_widget_actions.at(i));
            p_table_widget_actions.at(i)->disconnect();
            p_table_widget_actions.removeAt(i);
            break;
        }
    }
}

/**************************************************************************************
                    FIS_KERNEL::SELECTED_LINE
***************************************************************************************/

void FIS_KERNEL::SELECTED_LINE ( int line_id,int row_save_status )
{
    Q_UNUSED ( line_id );
    Q_UNUSED ( row_save_status );
}

/**************************************************************************************
                    FIS_KERNEL::REFRESH_FIS_RECORD
***************************************************************************************/

void FIS_KERNEL::REFRESH_FIS_RECORD ( int record_id )
{
    REFRESH_FORM_RECORD ( record_id );
}

/**************************************************************************************
                    FIS_KERNEL::REGISTER_LINE_WIDGET
***************************************************************************************/

void FIS_KERNEL::REGISTER_LINE_WIDGET ( QWidget * widget )
{
    if ( widget EQ NULL ) {
         return;
    }

    if ( widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
        QLineEdit * line_edit = static_cast<QLineEdit *>(widget);
        connect(line_edit, SIGNAL(textEdited(QString)) , this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(line_edit, SIGNAL(textChanged(QString)), this, SLOT(WIDGET_TEXT_CHANGED()));
        connect(line_edit, SIGNAL(destroyed(QObject*)) , this, SLOT(WIDGET_DESTROYED(QObject *)));

        if (line_edit->isReadOnly() EQ false AND line_edit->isEnabled() EQ true) {
            p_registered_line_widget << widget;
        }
        return;
    }
    else if ( widget->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
        QTextEdit * text_edit = static_cast<QTextEdit *>(widget);
        connect(text_edit, SIGNAL(textChanged())      , this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(text_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
        if (text_edit->isReadOnly() EQ false AND text_edit->isEnabled() EQ true) {
            p_registered_line_widget << widget;
        }
        return;
    }
    else if ( widget->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
        QPlainTextEdit * plain_text_edit = static_cast<QPlainTextEdit *>(widget);
        connect(plain_text_edit, SIGNAL(textChanged()), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(plain_text_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
        if (plain_text_edit->isReadOnly() EQ false AND plain_text_edit->isEnabled() EQ true) {
            p_registered_line_widget << widget;
        }
        return;
    }
    else if( widget->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
        QCommaEdit * comma_edit = static_cast<QCommaEdit *>(widget);
        connect(comma_edit, SIGNAL(textEdited(QString)) , this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(comma_edit, SIGNAL(destroyed(QObject*)) , this, SLOT(WIDGET_DESTROYED(QObject *)));
        connect(comma_edit, SIGNAL(textChanged(QString)), this, SLOT(WIDGET_TEXT_CHANGED()));
        if (comma_edit->isReadOnly() EQ false AND comma_edit->isEnabled() EQ true) {
            p_registered_line_widget << widget;
        }
        return;
    }
    else if ( widget->metaObject()->className() EQ QCheckBox::staticMetaObject.className() ) {
        QCheckBox * check_box = static_cast<QCheckBox *>(widget);
        connect(check_box, SIGNAL(stateChanged(int)), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(check_box, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
    }
    else if ( widget->metaObject()->className() EQ QRadioButton::staticMetaObject.className() ) {
        QRadioButton * radio_button = static_cast<QRadioButton *>(widget);
        connect(radio_button, SIGNAL(toggled(bool)), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(radio_button, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
    }
    else if ( widget->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
        QComboBox * combo_box = static_cast<QComboBox *>(widget);
        connect(combo_box, SIGNAL(currentIndexChanged(int)),      this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(combo_box, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
        if (combo_box->lineEdit() NE NULL) {
            combo_box->setInsertPolicy(QComboBox::NoInsert);
            connect(combo_box->lineEdit(),SIGNAL(editingFinished()),this,SLOT(COMBO_BOX_LINE_EDIT_CHANGED()));            
            connect(combo_box->lineEdit(),SIGNAL( textEdited  ( QString ) ), this, SLOT ( TABLE_WIDGETS_CHANGED() ) );
        }
    }
    else if( widget->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() ) {
        QTimeEdit * time_edit = static_cast<QTimeEdit *>(widget);
        connect(time_edit, SIGNAL(timeChanged(QTime)), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(time_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
    }
    else if( widget->metaObject()->className() EQ QDateEdit::staticMetaObject.className() ) {
        QDateEdit * date_edit = static_cast<QDateEdit *>(widget);
        connect(date_edit, SIGNAL(dateChanged(QDate)), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(date_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
    }

    if (widget->isEnabled() EQ true) {
        p_registered_line_widget << widget;
    }
}

/**************************************************************************************
                    FIS_KERNEL::COPY_PREVIOUS_CELL_CURRENT
***************************************************************************************/

void FIS_KERNEL::COPY_PREVIOUS_CELL_CURRENT()
{
    int current_row;
    int current_column;

    current_row = p_table_widget->currentRow();
    current_column = p_table_widget->currentColumn();

    if (current_row <= 0) {
        return;
    }

    QWidget * current_widget  = p_table_widget->cellWidget(current_row,current_column);
    QWidget * previous_widget = p_table_widget->cellWidget((current_row - 1),current_column);

    if (current_widget EQ NULL OR previous_widget EQ NULL) {
        return;
    }
    
    if ( p_widget_types.at(current_column) EQ WIDGET_COMBO_BOX OR p_widget_types.at(current_column) EQ WIDGET_COMBO_BOX_EDITABLE) {
        QComboBox * combo_box_current = static_cast<QComboBox *>(current_widget);
        QComboBox * combo_box_previous = static_cast<QComboBox *>(previous_widget);
        if (combo_box_current->currentIndex() NE combo_box_previous->currentIndex()) {
            combo_box_current->setCurrentIndex(combo_box_previous->currentIndex());
            TABLE_WIDGETS_CHANGED();
        }
    }
    else if ( p_widget_types.at(current_column) EQ WIDGET_COMMA_EDIT ) {
        QCommaEdit * comma_edit_current = static_cast<QCommaEdit *>(current_widget);
        QCommaEdit * comma_edit_previous = static_cast<QCommaEdit *>(previous_widget);
        if (comma_edit_current->GET_TEXT() NE comma_edit_previous->GET_TEXT()){
            comma_edit_current->UNDO_TEXT(comma_edit_previous->GET_TEXT());
            TABLE_WIDGETS_CHANGED();
        }
    }
    else if ( p_widget_types.at(current_column) EQ WIDGET_LINE_EDIT ) {
        QLineEdit * line_edit_current = static_cast<QLineEdit *>(current_widget);
        QLineEdit * line_edit_previous = static_cast<QLineEdit *>(previous_widget);
        if (line_edit_current->text() NE line_edit_previous->text()){
            line_edit_current->setText(line_edit_previous->text());
            TABLE_WIDGETS_CHANGED();
        }
    }
    else if ( p_widget_types.at(current_column) EQ WIDGET_CHECK_BOX ) {
        QCheckBox * check_box_current = static_cast<QCheckBox *>(current_widget);
        QCheckBox * check_box_previous = static_cast<QCheckBox *>(previous_widget);
        if (check_box_current->checkState() NE check_box_previous->checkState()){
            check_box_current->setCheckState(check_box_previous->checkState());
            TABLE_WIDGETS_CHANGED();
        }
    }
    else if ( p_widget_types.at(current_column) EQ WIDGET_DATE_EDIT) {
        QDateEdit * date_edit_current  = static_cast<QDateEdit *>(current_widget);
        QDateEdit * date_edit_previous = static_cast<QDateEdit *>(previous_widget);
        if (date_edit_current->date() NE date_edit_previous->date()) {
            date_edit_current->setDate(date_edit_previous->date());
            TABLE_WIDGETS_CHANGED();
        }
    }
    else if (p_widget_types.at(current_column) EQ WIDGET_SEARCH_EDIT ) {
        QSearchEdit * search_edit_current = static_cast<QSearchEdit *>(current_widget);
        QSearchEdit * search_edit_previous = static_cast<QSearchEdit *>(previous_widget);
        if (search_edit_current->GET_TEXT() NE search_edit_previous->GET_TEXT()){
            search_edit_current->SET_TEXT(search_edit_previous->GET_TEXT());
            TABLE_WIDGETS_CHANGED();
        }
    }
    else if (p_widget_types.at(current_column) EQ WIDGET_ADAK_DATE) {
        QAdakDate * adak_date_current = static_cast<QAdakDate *>(current_widget);
        QAdakDate * adak_date_previous = static_cast<QAdakDate *>(previous_widget);
        if (adak_date_current->DATE() NE adak_date_previous->DATE()){
            adak_date_current->SET_DATE(adak_date_previous->QDATE());
            TABLE_WIDGETS_CHANGED();
        }
    }

    CALL_CHECK_LINE_VAR(current_row,((QObject *)current_widget));
    SEND_TAB_KEY_EVENT(current_row,current_column);

    if (current_column EQ GET_LAST_COLUMN_FOR_NEW_LINE()) {
        SAVE_RECORD();
    }
}

/**************************************************************************************
                    FIS_KERNEL::SET_CELL_WIDGET
***************************************************************************************/

void FIS_KERNEL::SET_CELL_WIDGET ( int widget_type,int column,int row ) {

    if ( row EQ -1) {
         row = p_table_widget->currentRow();
    }

    if (widget_type EQ WIDGET_COMBO_BOX OR widget_type EQ WIDGET_COMBO_BOX_EDITABLE) {
        QComboBox * combo_box = new QComboBox(p_table_widget);
        combo_box->setObjectName("table_widget_child");
        combo_box->installEventFilter(this);
        p_table_widget->setCellWidget(row, column, combo_box);
        connect(combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(CHECK_VAR_SLOT_EVENT()));
        connect(combo_box, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
        if ( widget_type EQ WIDGET_COMBO_BOX_EDITABLE ) {
            combo_box->setEditable(true);
            combo_box->setInsertPolicy(QComboBox::NoInsert);
            if (combo_box->lineEdit() NE NULL) {
                connect(combo_box->lineEdit(),SIGNAL(editingFinished()),this,SLOT(COMBO_BOX_LINE_EDIT_CHANGED()));
            }
        }
    }
    else if ( widget_type EQ WIDGET_COMMA_EDIT ) {
        QCommaEdit * comma_edit = new QCommaEdit(p_table_widget);
        comma_edit->setObjectName("table_widget_child");
        p_table_widget->setCellWidget(row, column, comma_edit);
        connect(comma_edit, SIGNAL(textEdited(QString)), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(comma_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
        connect(comma_edit, SIGNAL(textChanged(QString)), this, SLOT(WIDGET_TEXT_CHANGED()));
        comma_edit->installEventFilter(this);
        if (p_comma_edit_precision.at(column) NE 2 ) {
            comma_edit->SET_PRECISION(p_comma_edit_precision.at(column));
        }
    }
    else if ( widget_type EQ WIDGET_LINE_EDIT ) {
        QLineEdit * line_edit = new QLineEdit(p_table_widget);
        line_edit->setObjectName("table_widget_child");
        int column_width = p_table_widget->columnWidth(column);
        if (line_edit->isReadOnly() EQ false AND line_edit->isEnabled() EQ true AND line_edit->isHidden() EQ false AND column_width > 2) {
            if ( line_edit->maxLength() EQ 32767 ) {
                line_edit->setMaxLength(0);
            }
        }
        p_table_widget->setCellWidget(row, column, line_edit);
        connect(line_edit, SIGNAL(textEdited(QString)) , this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(line_edit, SIGNAL(destroyed(QObject*)) , this, SLOT(WIDGET_DESTROYED(QObject *)));
        connect(line_edit, SIGNAL(textChanged(QString)), this, SLOT(WIDGET_TEXT_CHANGED()));
        line_edit->installEventFilter(this);
    }
    else if (widget_type EQ WIDGET_CHECK_BOX ) {
        QCheckBox * check_box = new QCheckBox(p_table_widget);
        check_box->setObjectName("table_widget_child");
        check_box->setText(p_line_check_box_names.front());
        p_line_check_box_names << p_line_check_box_names.front();
        p_line_check_box_names.pop_front();
        p_table_widget->setCellWidget(row, column, check_box);
        check_box->installEventFilter(this);
        connect(check_box, SIGNAL(stateChanged(int)), this, SLOT(CHECK_VAR_SLOT_EVENT()));
        connect(check_box, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
    }
    else if ( widget_type EQ WIDGET_DATE_EDIT) {
        QDateEdit * date_edit = new QDateEdit(p_table_widget);
        date_edit->setObjectName("table_widget_child");
        date_edit->setCalendarPopup(true);
        p_table_widget->setCellWidget(row, column, date_edit);
        date_edit->installEventFilter(this);
        connect(date_edit, SIGNAL(dateChanged(QDate)), this, SLOT(CHECK_VAR_SLOT_EVENT()),Qt::QueuedConnection);
        connect(date_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
    }
    else if ( widget_type EQ WIDGET_ADAK_DATE) {
        QAdakDate * adak_date = new QAdakDate(p_table_widget);
        adak_date->setObjectName("table_widget_child");
        p_table_widget->setCellWidget(row, column, adak_date);
        adak_date->installEventFilter(this);

        QLineEdit * line_edit = adak_date->findChild<QLineEdit *>("adak_date_line_edit");
        QToolButton * tool_button = adak_date->findChild<QToolButton *>("adak_date_tool_button");

        line_edit->installEventFilter(this);
        tool_button->installEventFilter(this);

        connect ( adak_date, SIGNAL ( SIGNAL_DATE_CHANGED () ), this, SLOT ( CHECK_VAR_SLOT_EVENT()));
        connect ( adak_date, SIGNAL ( destroyed ( QObject*)), this, SLOT ( WIDGET_DESTROYED(QObject *)));
    }
    else if (widget_type EQ WIDGET_SEARCH_EDIT ) {
        QSearchEdit * search_edit = new QSearchEdit(p_table_widget);
        search_edit->installEventFilter(this);
        search_edit->setObjectName("table_widget_child");
        p_search_edit_columns << column;
        p_table_widget->setCellWidget(row, column, search_edit);
        QToolButton * tool_button = search_edit->findChild<QToolButton *>("search_edit_tool_button");
        QLineEdit * line_edit = search_edit->findChild<QLineEdit *>("search_edit_line_edit");
        tool_button->installEventFilter(this);
        line_edit->installEventFilter(this);
        connect(line_edit,   SIGNAL(textEdited(QString)), this, SLOT(TABLE_WIDGETS_CHANGED()));
        connect(line_edit, SIGNAL(textChanged(QString)), this, SLOT(WIDGET_TEXT_CHANGED()));
        connect(line_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
        connect(search_edit, SIGNAL(SIGNAL_BUTTON_CLICKED(QWidget*,QLineEdit*,QToolButton*))   ,this, SLOT(LINE_SEARCH_BUTTON_CLICKED(QWidget*,QLineEdit*,QToolButton*)));

    }
    else if ( widget_type EQ WIDGET_PUSH_BUTTON ) {
        ADD_PUSH_BUTTON(row, column);
        p_push_button_columns << column;
    }
}

/**************************************************************************************
                    FIS_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT
***************************************************************************************/

void FIS_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT ( QWidget * widget )
{
    if ( widget->objectName() EQ "table_widget_child" OR widget->objectName() EQ "line_changer_button") {
         m_satir_degisen_widget_count++;
    }
    else if(widget->parent() NE NULL) {
        if ( widget->parent()->objectName() EQ "table_widget_child" ){
            m_satir_degisen_widget_count++;
        }
    }

    FORM_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT(widget);
}

/**************************************************************************************
                    FIS_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT
***************************************************************************************/

void FIS_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT ( QWidget * widget )
{
    if (widget->objectName() EQ "table_widget_child") {
        m_satir_degisen_widget_count--;
        return;
    }
    else if(widget->parent() NE NULL) {
        if ( widget->parent()->objectName() EQ "table_widget_child" ){
            m_satir_degisen_widget_count--;
        }
    }

    FORM_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT(widget);
}

/**************************************************************************************
                    FIS_KERNEL::REGISTER_LINE_CHANGER_BUTTON
***************************************************************************************/

void FIS_KERNEL::REGISTER_LINE_CHANGER_BUTTON ( int row, int column )
{
    LINE_BUTTONS_REGISTER_FUNCTION ( row,column,&m_changer_buttons_column,CHANGER_LINE_SLOT);
}

/**************************************************************************************
                    FIS_KERNEL::REGISTER_LINE_CHANGER_BUTTON
***************************************************************************************/

void FIS_KERNEL::REGISTER_LINE_SAVER_BUTTON ( int row, int column)
{
    LINE_BUTTONS_REGISTER_FUNCTION ( row,column,&m_saver_buttons_column,SAVER_LINE_SLOT);
}

/**************************************************************************************
                    FIS_KERNEL::FOCUS_LAST_FOCUSED_WIDGET
***************************************************************************************/

void FIS_KERNEL::SLOT_LINE_CHANGER_BUTTON_CLICKED()
{
    LINE_BUTTONS_SLOT_FUNCTION ( sender(),&m_changer_buttons_column,CHANGER_LINE_SLOT);
}

/**************************************************************************************
                    FIS_KERNEL::SLOT_LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

void FIS_KERNEL::SLOT_LINE_SAVER_BUTTON_CLICKED()
{
    LINE_BUTTONS_SLOT_FUNCTION ( sender(),&m_saver_buttons_column,SAVER_LINE_SLOT);
}

/**************************************************************************************
                    BASE_EVENT_KERNEL::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

int FIS_KERNEL::LINE_CHANGER_BUTTON_CLICKED ( QAbstractButton * button,int row_number)
{
    Q_UNUSED ( button );
    Q_UNUSED ( row_number );

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                    BASE_EVENT_KERNEL::LINE_SAVER_BUTTON_CLICKED
***************************************************************************************/

void FIS_KERNEL::LINE_SAVER_BUTTON_CLICKED ( QAbstractButton * button, int row_id, int record_id, int row_number)
{
    Q_UNUSED ( button );
    Q_UNUSED ( row_number );
    Q_UNUSED ( row_id );
    Q_UNUSED ( record_id) ;
}

/**************************************************************************************
                    BASE_EVENT_KERNEL::LINE_BUTTONS_SLOT_FUNCTION
***************************************************************************************/

void FIS_KERNEL::LINE_BUTTONS_SLOT_FUNCTION ( QObject * button_obj,QList<int> * button_columns,int slot_type)
{
    if(button_obj EQ NULL) {
        return;
    }

    int focused_row     = -1;
    int focused_column  = -1;

    for (int i = 0 ; i < button_columns->size() ; i++ ) {
        focused_row     = FIND_FOCUSED_ROW(button_columns->at(i));
        focused_column  = button_columns->at(i);
        if (focused_row NE -1 AND focused_column NE -1) {
            break;
        }
    }

    if (focused_row EQ -1 OR focused_column EQ -1) {
        return;
    }

    SET_TABLE_WIDGET_FOCUS(focused_row,focused_column);

    QAbstractButton * button = static_cast<QAbstractButton *>(button_obj);

    if ( slot_type EQ CHANGER_LINE_SLOT) {
         CLOSE_ALL_EVENTS();
         int return_value = LINE_CHANGER_BUTTON_CLICKED ( button,focused_row);
         OPEN_ALL_EVENTS();

        if ( return_value EQ ADAK_RECORD_UNCHANGED ) {
             return;
        }

        INCREASE_DEGISEN_WIDGET_COUNT ( button );
        TABLE_WIDGETS_CHANGED();

        return;
    }

    else if ( slot_type EQ SAVER_LINE_SLOT) {
        int save_return_value = SAVE_FIS_RECORD();

        if ( save_return_value EQ RECORD_CANT_SAVED OR save_return_value EQ RECORD_EMPTY) {
            return;
        }

        int row_id      = -1;
        int record_id   = -1;

        QTableWidgetItem * record_id_item   = p_table_widget->item(focused_row,p_form_id_column_number);
        QTableWidgetItem * row_id_item      = p_table_widget->item(focused_row,p_row_id_column_number);

        if ( record_id_item NE NULL ) {
             record_id = record_id_item->text().toInt();
        }

        if ( row_id_item NE NULL ) {
             row_id    = row_id_item->text().toInt();
        }

        if ( row_id EQ -1 OR record_id EQ -1) {
            return;
        }

        CLOSE_ALL_EVENTS();
        LINE_SAVER_BUTTON_CLICKED ( button,row_id,record_id,focused_row);
        OPEN_ALL_EVENTS();
    }
}

/**************************************************************************************
                    BASE_EVENT_KERNEL::LINE_BUTTONS_REGISTER_FUNCTION
***************************************************************************************/

void FIS_KERNEL::LINE_BUTTONS_REGISTER_FUNCTION ( int row,int column,QList<int> * button_columns,int slot_type)
{
    QWidget * widget = p_table_widget->cellWidget ( row,column );

    if ( widget EQ NULL ) {
        return;
    }

    if ( button_columns->contains(column) EQ false) {
        *button_columns << column;
    }

    QAbstractButton * button = KERNEL_FIND_BUTTON_WIDGET(widget);

    if (button EQ NULL) {
        return;
    }

    //Baglanmis olan diger slotlari kaldirir.
    disconnect ( button,SIGNAL( clicked() ),0,0 );

    if ( slot_type EQ SAVER_LINE_SLOT ) {
         button->setObjectName ( "line_saver_button" );
         button->setDisabled(true);
         connect ( button,SIGNAL ( clicked() ),this,SLOT ( SLOT_LINE_SAVER_BUTTON_CLICKED() ) );
        return;
    }
    else if ( slot_type EQ CHANGER_LINE_SLOT ) {
        button->setObjectName ( "line_changer_button" );
        connect ( button,SIGNAL ( clicked() ),this,SLOT ( SLOT_LINE_CHANGER_BUTTON_CLICKED() ) );
    }
}

/**************************************************************************************
                    BASE_EVENT_KERNEL::IS_FORM_CHANGED
***************************************************************************************/

bool FIS_KERNEL::IS_FORM_CHANGED()
{
    if ( ( m_degisen_widget_counter - m_satir_degisen_widget_count ) > 0 ) {
        return true;
    }

    if ( GET_RECORD_SAVE_STATUS() EQ NEED_ADD ) {
        return true;
    }

    return false;
}

/**************************************************************************************
                    BASE_EVENT_KERNEL::SHORT_CUT_PRESSED
***************************************************************************************/

void FIS_KERNEL::SLOT_SHORT_CUT_PRESSED()
{
    int current_row = p_table_widget->currentRow();

    if (current_row < 0) {
        return;
    }

    int current_column = sender()->objectName().toInt();

    QWidget * widget = p_table_widget->cellWidget ( current_row,current_column );

    if (widget EQ NULL) {
        return;
    }

    if ( widget->objectName() EQ "line_saver_button") {
         widget->setFocus();
         LINE_BUTTONS_SLOT_FUNCTION ( widget,&m_saver_buttons_column,SAVER_LINE_SLOT );
    }
    else if ( widget->objectName() EQ "line_changer_button" ) {
        widget->setFocus();
        LINE_BUTTONS_SLOT_FUNCTION ( widget,&m_changer_buttons_column,CHANGER_LINE_SLOT );
    }
    else {
        CALL_CHECK_LINE_VAR ( current_row, widget );
    }
}
