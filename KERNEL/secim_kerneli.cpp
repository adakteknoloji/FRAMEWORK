#include <QTableWidget>
#include <QHeaderView>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QEvent>
#include <QAction>
#include "secim_kerneli.h"
#include "adak_defines.h"
#include "kernel_private_utils.h"
#include "kernel_utils.h"

/**************************************************************************************
                   SECIM_KERNELI::SECIM_KERNELI
***************************************************************************************/

SECIM_KERNELI::SECIM_KERNELI ( QWidget * parent ) : BASE_KERNEL ( parent )
{
    m_kernel_button_text        = "Seçim Ekranı";

    p_window_title      = "... SECIMI";
    // FIXME : p_height ve p_width kaldirilacak
    p_height            = 580;
    p_width             = 780;
    p_h_box_layout      = new QHBoxLayout;
    m_is_multi_select   = false;

    SET_SORTING(true);

    setModal(true);

    DISABLE_MULTI_ROW_SELECTION();

    m_label_status = KERNEL_CREATE_STATUS_LABEL(p_h_box_layout);
}

/**************************************************************************************
                   SECIM_KERNELI::~SECIM_KERNELI
***************************************************************************************/

SECIM_KERNELI::~SECIM_KERNELI()
{

}

/**************************************************************************************
                   SECIM_KERNELI::INIT_KERNEL
***************************************************************************************/

void SECIM_KERNELI::INIT_KERNEL  ( ADAK_SQL * database )
{
    QVBoxLayout * vertical_layout = new QVBoxLayout  ( this ) ;
    p_table_widget                = new QTableWidget ( this );

    setLayout      ( vertical_layout   );
    setWindowTitle ( p_window_title    ) ;

    setMinimumWidth ( p_width  );
    setMinimumHeight( p_height );

    SET_ADAK_WINDOW_DEFAULTS ( this );
    SET_DB_CONN ( database );

    SET_FIRST_FOCUS_WIDGET( p_table_widget );

    p_table_widget->setColumnCount            ( p_num_of_columns);
    p_table_widget->setHorizontalHeaderLabels ( p_headers );
    p_table_widget->setSelectionBehavior      ( QAbstractItemView::SelectRows );

    FILL_TABLE();

    KERNEL_UPDATE_STATUS_LABEL ( m_label_status,p_table_widget->rowCount() );

    if ( p_sorting_value ) {
         p_table_widget->setSortingEnabled(true);
         p_table_widget->sortByColumn ( m_sorting_column, Qt::AscendingOrder);
    }

    p_table_widget->setEditTriggers ( QTableWidget::NoEditTriggers);

    vertical_layout->addWidget ( p_table_widget);
    vertical_layout->addLayout ( p_h_box_layout);

    KERNEL_SET_TABLE_SELECTION_MODE ( p_table_widget,m_is_multi_select);
    KERNEL_SET_TABLE_CHECK_BOX_COLUMN ( p_table_widget,m_is_multi_select,p_num_of_columns );

    if ( m_is_multi_select EQ true ) {
        connect(KERNEL_CREATE_SEC_BUTTON ( p_h_box_layout,m_is_multi_select ), SIGNAL( clicked())  ,this, SLOT(FIND_SELECTED_ROWS()));
    }

    QAction * enter_action = new QAction(p_table_widget);
    QAction * return_action = new QAction(p_table_widget);

    p_table_widget->addAction(enter_action);
    enter_action->setShortcut(QKeySequence(Qt::Key_Enter));
    p_table_widget->addAction(return_action);
    return_action->setShortcut(QKeySequence(Qt::Key_Return));

    connect(enter_action  , SIGNAL ( triggered() )                        ,this, SLOT(FIND_SELECTED_ROWS()));
    connect(return_action , SIGNAL ( triggered() )                        ,this, SLOT(FIND_SELECTED_ROWS()));

    connect(p_table_widget, SIGNAL ( itemClicked ( QTableWidgetItem*))    ,this, SLOT(SLOT_ROW_CLICKED()));
    connect(p_table_widget, SIGNAL ( itemSelectionChanged())              ,this, SLOT(SLOT_ROW_CLICKED()));
    connect(p_table_widget, SIGNAL ( itemDoubleClicked(QTableWidgetItem*)),this, SLOT(FIND_SELECTED_ROWS()));

    p_table_widget->setFocus();

    if ( p_table_widget->rowCount() > 0 AND p_table_widget->columnCount() > 0) {
        QTableWidgetItem * item = NULL;

        for ( int i = 0 ; i < p_table_widget->columnCount() ; i++ ) {
            if ( p_table_widget->columnWidth(i) > 0 ) {
                item = p_table_widget->item(0,i);
            }
        }
        if ( item NE NULL ) {
            p_table_widget->setCurrentItem(item);
        }
    }
}

/**************************************************************************************
                   SECIM_KERNELI::GET_TABLE_WIDGET
***************************************************************************************/

QTableWidget * SECIM_KERNELI::GET_TABLE_WIDGET()
{
    return p_table_widget;
}

/**************************************************************************************
                   SECIM_KERNELI::ADD_BUTTON
***************************************************************************************/

QToolButton * SECIM_KERNELI::ADD_BUTTON ( QString button_text,Qt::ToolButtonStyle style,QString icon,int height,int width)
{
    QToolButton * button = new QToolButton;
    button->setText ( button_text );
    button->setIcon ( QIcon ( icon ) );
    button->setToolButtonStyle( style );

    if ( height NE -1 AND width NE -1 ) {
         button->setMinimumSize ( height,width );
         button->setMaximumSize ( height,width );
    }

    p_h_box_layout->addWidget ( button );

    connect ( button, SIGNAL ( clicked() ), this, SLOT ( SLOT_BUTTON_CLICKED() ) );

    return button;
}

/**************************************************************************************
                   SECIM_KERNELI::SET_HEADERS
***************************************************************************************/

void SECIM_KERNELI::SET_HEADERS ( QStringList headers )
{
    for (int i = 0; i < headers.size(); i++ ) {
        p_headers<<tr ( headers.at(i).toUtf8().data());
    }
    p_num_of_columns = headers.size();
}

/**************************************************************************************
                   SECIM_KERNELI::SET_SORTING
***************************************************************************************/

void SECIM_KERNELI::SET_SORTING (bool value, int sorting_column)
{
    p_sorting_value   = value;
    m_sorting_column  = sorting_column;
}

/**************************************************************************************
                   SECIM_KERNELI::HIDE_COLUMN_HEADER
***************************************************************************************/

void SECIM_KERNELI::HIDE_COLUMN_HEADER ( bool hide )
{
    if ( hide EQ true ) {
         p_table_widget->horizontalHeader()->setVisible(false);
    }
    else {
        p_table_widget->horizontalHeader()->setVisible(true);
    }
}

/**************************************************************************************
                   SECIM_KERNELI::SET_WINDOW_SIZE
***************************************************************************************/

void SECIM_KERNELI::SET_WINDOW_SIZE ( int height, int width )
{
    p_height            = height;
    p_width             = width;
    // FIXME
    // setMinimumSize(height, width);
    // setMaximumSize(height, width);
}

/**************************************************************************************
                   SECIM_KERNELI::FIND_SELECTED_ROWS
***************************************************************************************/

void SECIM_KERNELI::FIND_SELECTED_ROWS ()
{
    QList<int> selected_rows = KERNEL_TABLE_SELECTED_ROWS ( p_table_widget );

    int return_answer = -1;

    if ( selected_rows.size() EQ 1 ) {
         return_answer = SINGLE_LINE_SELECTED ( selected_rows.at(0) );
    }
    else if ( selected_rows.size() > 1 ) {
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
                   SECIM_KERNELI::SLOT_ROW_CLICKED
***************************************************************************************/

void SECIM_KERNELI::SLOT_ROW_CLICKED ()
{
    if  ( m_is_multi_select EQ true ) {
         QList<int> selected_rows = KERNEL_TABLE_SELECTED_ROWS ( p_table_widget );

        for ( int i = 0 ; i < p_table_widget->rowCount() ; i++ ) {
             QTableWidgetItem * item = p_table_widget->item ( i,p_num_of_columns );
            if ( item NE NULL ) {
                item->setCheckState ( Qt::Unchecked );
            }
        }

        for ( int i = 0 ; i < selected_rows.size() ; i++  )  {
            QTableWidgetItem * item = p_table_widget->item ( selected_rows.at(i),p_num_of_columns );
            if ( item NE NULL) {
                 item->setCheckState ( Qt::Checked );
            }
        }
    }

    int selected_row = p_table_widget->currentRow();
    SELECTED_LINE ( selected_row );
}

/**************************************************************************************
                   SECIM_KERNELI::SLOT_BUTTON_CLICKED
***************************************************************************************/

void SECIM_KERNELI::SLOT_BUTTON_CLICKED()
{
    QToolButton * button =  ( QToolButton * ) sender();
    BUTTON_CLICKED(button);
}

/**************************************************************************************
                   SECIM_KERNELI::ADD_NEW_ROW
***************************************************************************************/

int SECIM_KERNELI::ADD_NEW_ROW()
{
   int new_row_number = p_table_widget->rowCount();

   p_table_widget->insertRow ( new_row_number );

   for( int i = 0; i < p_num_of_columns ; i++ ) {

       QTableWidgetItem * column_item = new QTableWidgetItem();
       p_table_widget->setItem  ( new_row_number, i, column_item );
   }

   return new_row_number;
}

/**************************************************************************************
                   SECIM_KERNELI::SELECTED_LINE
***************************************************************************************/

void SECIM_KERNELI::SELECTED_LINE ( int row_id )
{
    Q_UNUSED ( row_id ) ;
}

/**************************************************************************************
                   SECIM_KERNELI::BUTTON_CLICKED
***************************************************************************************/

void SECIM_KERNELI::BUTTON_CLICKED ( QToolButton * button )
{
    Q_UNUSED ( button );
}

/**************************************************************************************
                   SECIM_KERNELI::ENABLE_MULTI_ROW_SELECTION
***************************************************************************************/

void SECIM_KERNELI::ENABLE_MULTI_ROW_SELECTION()
{
    m_is_multi_select = true;
}

/**************************************************************************************
                   SECIM_KERNELI::DISABLE_MULTI_ROW_SELECTION
***************************************************************************************/

void SECIM_KERNELI::DISABLE_MULTI_ROW_SELECTION()
{
    m_is_multi_select = false;
}

/**************************************************************************************
                   SECIM_KERNELI::REFRESH_TABLE
***************************************************************************************/

void SECIM_KERNELI::REFRESH_TABLE()
{
    for ( int i=p_table_widget->rowCount(); i > 0; i-- ) {
          p_table_widget->removeRow ( i-1 );
    }
    p_table_widget->setSortingEnabled ( false );

    KERNEL_SET_TABLE_SELECTION_MODE ( p_table_widget,m_is_multi_select );

    FILL_TABLE();

    KERNEL_UPDATE_STATUS_LABEL(m_label_status,p_table_widget->rowCount());
    KERNEL_SET_TABLE_CHECK_BOX_COLUMN(p_table_widget,m_is_multi_select, p_num_of_columns);

    if ( p_sorting_value ) {
         p_table_widget->setSortingEnabled ( true );
         p_table_widget->sortByColumn ( m_sorting_column, Qt::AscendingOrder) ;
    }
    p_table_widget->setEditTriggers ( QTableWidget::NoEditTriggers );
    
    p_table_widget->setFocus();
}

/**************************************************************************************
                   SECIM_KERNELI::MULTI_LINE_SELECTED
***************************************************************************************/

int SECIM_KERNELI::MULTI_LINE_SELECTED ( QList<int> selected_rows )
{
    Q_UNUSED ( selected_rows );

    return ADAK_CONTINUE;
}

/**************************************************************************************
                   SECIM_KERNELI::SELECT_LINES
***************************************************************************************/

void SECIM_KERNELI::SELECT_LINES ( QList< int > p_line_row_list )
{
    if ( m_is_multi_select EQ true ) {

        for ( int i = 0 ; i < p_table_widget->rowCount() ; i++ ) {
             QTableWidgetItem * item = p_table_widget->item ( i, p_num_of_columns  );
            if ( item NE NULL ) {
                item->setCheckState ( Qt::Unchecked );
            }
        }

        QItemSelectionModel *selectionModel = p_table_widget->selectionModel();
        QItemSelection itemSelection = selectionModel->selection();

        for ( int i = 0 ; i < p_line_row_list.size() ; i++  )  {
            QTableWidgetItem * item = p_table_widget->item( p_line_row_list.at(i), p_num_of_columns );
            if ( item NE NULL) {
                 p_table_widget->selectRow( p_line_row_list.at( i ) );
                 itemSelection.merge( selectionModel->selection(), QItemSelectionModel::Select);
            }
        }

        selectionModel->clearSelection();
        selectionModel->select( itemSelection,QItemSelectionModel::Select);
    }
}

