#include "arama_kernel.h"
#include "adak_defines.h"

#include "kernel_private_utils.h"

/**************************************************************************************
                   ARAMA_KERNEL::ARAMA_KERNEL
***************************************************************************************/

ARAMA_KERNEL::ARAMA_KERNEL ( QWidget *parent ) :  BATCH_KERNEL ( parent )
{
    m_kernel_button_text        = tr("Search Screen");
    m_auto_start                = false;
    p_find_button_name          = tr("Search");

    SET_SORTING(true,1);
    DISABLE_MULTI_ROW_SELECTION();
}

/**************************************************************************************
                   ARAMA_KERNEL::~ARAMA_KERNEL
***************************************************************************************/

ARAMA_KERNEL::~ARAMA_KERNEL()
{

}

/**************************************************************************************
                   ARAMA_KERNEL::START_ARAMA_KERNEL
***************************************************************************************/

void ARAMA_KERNEL::START_ARAMA_KERNEL(QObject *parent, ADAK_SQL *database)
{
    START_BATCH_KERNEL(parent,database);
}

/**************************************************************************************
                   ARAMA_KERNEL::INIT_KERNEL
***************************************************************************************/

void ARAMA_KERNEL::INIT_KERNEL ()
{
    p_table_widget->setSelectionBehavior                      ( QAbstractItemView::SelectRows);
    p_table_widget->setEnabled                                ( false );

    if ( m_is_multi_select EQ true ) {
        connect(KERNEL_CREATE_SEC_BUTTON ( m_arama_button_widget->layout(),m_is_multi_select), SIGNAL(clicked())  ,this, SLOT(FIND_SELECTED_ROWS()));
    }

    m_arama_button_widget->layout()->addWidget ( m_batch_kernel_buttons );

    SET_FIND_BUTTON_NAME ( p_find_button_name );

    connect(p_table_widget, SIGNAL ( itemClicked(QTableWidgetItem*))      ,this, SLOT ( SLOT_CLICKED() ) );
    connect(p_table_widget, SIGNAL ( itemSelectionChanged() )              ,this, SLOT ( SLOT_CLICKED() ) );
    connect(p_table_widget, SIGNAL ( itemDoubleClicked ( QTableWidgetItem* ) ),this, SLOT ( FIND_SELECTED_ROWS() ) );

    BATCH_KERNEL::INIT_KERNEL();

    if ( m_auto_start EQ true ) {
        emit run_batch_button->click();
    }
}

/**************************************************************************************
                   ARAMA_KERNEL::REGISTER_TABLE_WIDGET
***************************************************************************************/

void ARAMA_KERNEL::REGISTER_TABLE_WIDGET ( QTableWidget * table_widget )
{
    p_table_widget = table_widget;
}

/**************************************************************************************
                   ARAMA_KERNEL::AUTO_START
***************************************************************************************/

void ARAMA_KERNEL::AUTO_START()
{
    m_auto_start = true;
}

/**************************************************************************************
                   ARAMA_KERNEL::REGISTER_ARAMA_BUTTONS_WIDGET
***************************************************************************************/

void ARAMA_KERNEL::REGISTER_ARAMA_BUTTONS_WIDGET ( QWidget * widget,bool is_yardim_visible )
{
    ////
    m_batch_kernel_buttons = new QWidget();
    REGISTER_BUTTON_WIDGET ( m_batch_kernel_buttons,is_yardim_visible );
    ////

    m_arama_button_widget = widget;

    QHBoxLayout * layout = new QHBoxLayout();
    m_arama_button_widget->setLayout ( layout );

    m_label = KERNEL_CREATE_STATUS_LABEL ( layout );
    DISABLE_EVENTS_ON_WIDGET (m_label);
}

/**************************************************************************************
                   ARAMA_KERNEL::SET_FIND_BUTTON_NAME
***************************************************************************************/

void ARAMA_KERNEL::SET_FIND_BUTTON_NAME ( QString find_button_name )
{
    p_find_button_name = find_button_name;

    if ( run_batch_button NE NULL ) {
        run_batch_button->setText ( find_button_name );
    }
}

/**************************************************************************************
                   ARAMA_KERNEL::SET_HEADERS
***************************************************************************************/

void ARAMA_KERNEL::SET_HEADERS ( QStringList headers )
{
    for ( int i = 0; i < headers.size(); i++  ) {
        p_headers<<tr ( headers.at(i).toUtf8().data() );
    }

    p_num_of_columns = headers.size();

    if ( p_table_widget NE NULL ) {
        p_table_widget->setColumnCount                            ( p_num_of_columns );
        p_table_widget->setHorizontalHeaderLabels                 ( p_headers );
    }
}

/**************************************************************************************
                   ARAMA_KERNEL::FIND_SELECTED_ROWS
***************************************************************************************/

void ARAMA_KERNEL::FIND_SELECTED_ROWS()
{
    QList<int> selected_rows = KERNEL_TABLE_SELECTED_ROWS ( p_table_widget );

    int return_answer = -1;

    if ( selected_rows.size() EQ 1 ) {
         return_answer = SINGLE_LINE_SELECTED ( selected_rows.at(0) );
    }
    else if  ( selected_rows.size() > 1 ) {
        return_answer = MULTI_LINE_SELECTED ( selected_rows );
    }

    if ( return_answer EQ -1 ) {
        return;
    }

    switch ( return_answer ) {
        case ADAK_EXIT:
            close();
            break;
        case ADAK_CONTINUE:
        default:
            break;
    }
}

/**************************************************************************************
                   ARAMA_KERNEL::SLOT_CLICKED
***************************************************************************************/

void ARAMA_KERNEL::SLOT_CLICKED ()
{
    if ( m_is_multi_select EQ true ) {
        QList<int> selected_rows = KERNEL_TABLE_SELECTED_ROWS ( p_table_widget );

        for ( int i = 0 ; i < p_table_widget->rowCount() ; i++ ) {
            QTableWidgetItem * item = p_table_widget->item ( i,p_num_of_columns );
            if ( item NE NULL) {
                item->setCheckState ( Qt::Unchecked );
            }
        }

        for ( int i = 0 ; i < selected_rows.size() ; i++ ) {
            QTableWidgetItem * item = p_table_widget->item ( selected_rows.at(i),p_num_of_columns );
            if ( item NE NULL) {
                 item->setCheckState ( Qt::Checked );
            }
        }
    }

    int selected_row = p_table_widget->currentRow();

    CLOSE_CHECK_VAR_CONTROL();
    SELECTED_LINE ( selected_row );
    OPEN_CHECK_VAR_CONTROL();
}

/**************************************************************************************
                   ARAMA_KERNEL::SELECTED_LINE
***************************************************************************************/

void ARAMA_KERNEL::SELECTED_LINE ( int line_id )
{
    Q_UNUSED ( line_id );
}

/**************************************************************************************
                   ARAMA_KERNEL::ADD_NEW_ROW
***************************************************************************************/

int ARAMA_KERNEL::ADD_NEW_ROW()
{
   int new_row_number = p_table_widget->rowCount();

   p_table_widget->insertRow ( new_row_number );

   for ( int i = 0; i < p_num_of_columns ; i++ ) {

       QTableWidgetItem * column_item = new QTableWidgetItem();
       p_table_widget->setItem ( new_row_number, i, column_item );
   }

   return new_row_number;
}

/**************************************************************************************
                   ARAMA_KERNEL::SET_SORTING
***************************************************************************************/

void ARAMA_KERNEL::SET_SORTING ( bool value , int sorting_column )
{
    p_sorting_value  = value;
    m_sorting_column = sorting_column;
}

/**************************************************************************************
                   ARAMA_KERNEL::RUN_BATCH
***************************************************************************************/

void ARAMA_KERNEL::RUN_BATCH()
{
    KERNEL_SET_TABLE_SELECTION_MODE ( p_table_widget,m_is_multi_select );

    for ( int i = p_table_widget->rowCount()-1; i >= 0 ; i-- ) {
        p_table_widget->removeRow( i );
    }

    p_table_widget->setEnabled(true);

    if ( p_sorting_value EQ true ) {
         p_table_widget->setSortingEnabled ( false );
    }
    SEARCH();
    KERNEL_SET_TABLE_CHECK_BOX_COLUMN ( p_table_widget,m_is_multi_select,p_num_of_columns );
    KERNEL_UPDATE_STATUS_LABEL ( m_label,p_table_widget->rowCount() );

    if ( p_table_widget->rowCount() <= 0 ) {
        MSG_ERROR(tr("No records were found according to your search criteria.") , NULL);//Aradığınız kriterlere uygun kayıt bulunamadı.
    }

    p_table_widget->setEditTriggers ( QTableWidget::NoEditTriggers );

    if ( p_sorting_value EQ true ) {
         p_table_widget->setSortingEnabled ( true );
         p_table_widget->sortByColumn ( m_sorting_column, Qt::AscendingOrder );
    }
    p_table_widget->setFocus();
}

/**************************************************************************************
                   ARAMA_KERNEL::ADD_BUTTON
***************************************************************************************/

QToolButton * ARAMA_KERNEL::ADD_BUTTON ( QString button_text,Qt::ToolButtonStyle style,QString icon,int height,int width )
{
    QToolButton * button = new QToolButton;
    button->setText ( button_text );
    button->setIcon ( QIcon ( icon ) );
    button->setToolButtonStyle ( style );

    if (height NE -1 AND width NE -1) {
        button->setMinimumSize ( height,width );
        button->setMaximumSize ( height,width );
    }

    m_arama_button_widget->layout()->addWidget ( button );

    return button;
}

/**************************************************************************************
                   ARAMA_KERNEL::REFRESH_ARAMA         
***************************************************************************************/

void ARAMA_KERNEL::REFRESH_ARAMA ()
{
    RUN_BATCH ();
}

/**************************************************************************************
                   ARAMA_KERNEL::MULTI_LINE_SELECTED
***************************************************************************************/

int ARAMA_KERNEL::MULTI_LINE_SELECTED ( QList<int> selected_rows )
{
    Q_UNUSED ( selected_rows );

    return ADAK_CONTINUE;
}

/**************************************************************************************
                   ARAMA_KERNEL::ENABLE_MULTI_ROW_SELECTION
***************************************************************************************/

void ARAMA_KERNEL::ENABLE_MULTI_ROW_SELECTION()
{
    m_is_multi_select = true;
}

/**************************************************************************************
                   ARAMA_KERNEL::DISABLE_MULTI_ROW_SELECTION
***************************************************************************************/

void ARAMA_KERNEL::DISABLE_MULTI_ROW_SELECTION()
{
    m_is_multi_select = false;
}

/**************************************************************************************
                   ARAMA_KERNEL::eventFilter
***************************************************************************************/

bool ARAMA_KERNEL::eventFilter ( QObject *obj, QEvent *event )
{
    // ---------------------------- KEY PRESS
    if ( event->type() EQ QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ( keyEvent->key() EQ Qt::Key_Enter OR keyEvent->key() EQ Qt::Key_Return ) {
            if (obj EQ p_table_widget) {
                FIND_SELECTED_ROWS();
                return true;
            }            
        }
    }

    return BATCH_KERNEL::eventFilter(obj,event);
}
