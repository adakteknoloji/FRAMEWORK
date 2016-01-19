#include <QHeaderView>
#include <QCloseEvent>
#include <QComboBox>
#include <QVBoxLayout>
#include <QEvent>
#include <QAction>
#include <QDebug>
#include <QMdiSubWindow>
#include <QSortFilterProxyModel>
#include "adak_utils.h"
#include "tree_arama_kernel.h"
#include "adak_defines.h"
#include "kernel_utils.h"
#include "kernel_private_utils.h"

/**************************************************************************************
                   TREE_ARAMA_KERNEL::TREE_ARAMA_KERNEL
***************************************************************************************/

TREE_ARAMA_KERNEL::TREE_ARAMA_KERNEL ( QWidget* parent ) : BASE_EVENT_KERNEL ( parent )
{
    m_kernel_button_text        = "";

    p_name_of_run_search_button = "Search";
    p_icon_file_name            = ":/kernel_icons/kernel_run_icon.png";
    close_event_started         = false;
    p_run_search_on_enter       = false;

    p_height                    = 400;
    p_width                     = 400;

    setModal(true);
    p_h_box_layout              = new QHBoxLayout;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::~TREE_ARAMA_KERNEL
***************************************************************************************/

TREE_ARAMA_KERNEL::~TREE_ARAMA_KERNEL()
{

}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::INIT_KERNEL
***************************************************************************************/

void TREE_ARAMA_KERNEL::INIT_KERNEL ( QTreeWidget * tree_widget ,QObject * parent,ADAK_SQL * database )
{
    Q_UNUSED  ( parent );

    m_tree_widget = tree_widget;

    m_tree_widget->setHeaderLabels(m_headers);

    SET_ADAK_WINDOW_DEFAULTS ( parent );
    SET_EVENTS_AND_SIGNALS ( parent, ADAK_BATCH_KERNEL );
    SET_DB_CONN ( database );

    m_tree_widget->setEditTriggers ( QTreeView::NoEditTriggers );

    SET_TREE_WIDGET_DEFAULTS();

    FOCUS_FIRST_WIDGET();

    connect(m_tree_widget   , SIGNAL ( clicked ( QModelIndex))      ,this, SLOT ( SLOT_ROW_CLICKED() ) );
    connect(m_tree_widget   , SIGNAL ( doubleClicked ( QModelIndex)),this, SLOT ( FIND_SELECTED_ROW() ) );
    connect(m_tree_widget   , SIGNAL ( activated ( QModelIndex))    ,this, SLOT ( FIND_SELECTED_ROW() ) );
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::ADD_BUTTON
***************************************************************************************/

QToolButton * TREE_ARAMA_KERNEL::ADD_BUTTON ( QString button_text,Qt::ToolButtonStyle style,QString icon,int height,int width )
{
    QToolButton * button = new QToolButton;
    button->setText ( button_text );
    button->setIcon ( QIcon ( icon ) );
    button->setToolButtonStyle( style );

    if ( height NE -1 AND width NE -1 ) {
         button->setMinimumSize(height,width);
         button->setMaximumSize(height,width);
    }
    p_h_box_layout->addWidget ( button );

    connect(button, SIGNAL ( clicked() ), this, SLOT ( SLOT_BUTTON_CLICKED() ) );

    return button;
}

/**************************************************************************************
                   TREE_KERNEL::SET_RIGHT_ALIGN_COLUMNS
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_RIGHT_ALIGN_COLUMNS ( QList<int>  )
{

}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SET_WINDOW_SIZE
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_WINDOW_SIZE ( int height, int width )
{
    p_height = height;
    p_width  = width;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::FIND_SELECTED_ROW
***************************************************************************************/

void TREE_ARAMA_KERNEL::FIND_SELECTED_ROW ()
{
    int return_value = ADAK_CONTINUE;

    if (m_selected_items.size() EQ 1) {
        return_value = SINGLE_LINE_SELECTED(GET_COLUMN_DATAS_FROM_ITEM(m_selected_items.at(0)));
    }
    else {
        QList<QStringList> selected_datas;

        for ( int i = 0 ; i < m_selected_items.count() ; i++ ) {
            selected_datas << GET_COLUMN_DATAS_FROM_ITEM(m_selected_items.at(i));
        }

        return_value = MULTI_LINE_SELECTED ( selected_datas );
    }

    switch ( return_value ) {
        case ADAK_EXIT:
            close();
            break;
        case ADAK_CONTINUE:
        default:
            break;
    }
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SLOT_ROW_CLICKED
***************************************************************************************/

void TREE_ARAMA_KERNEL::SLOT_ROW_CLICKED ()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    if ( item EQ NULL ) {
        return;
    }

    if (m_selected_items.contains(item) EQ true) {
        m_selected_items << item;
        SELECTED_LINE ( GET_COLUMN_DATAS_FROM_ITEM(item) );
        if (m_is_multi_row EQ true) {
            QCheckBox * check_box = static_cast<QCheckBox *>(m_tree_widget->itemWidget(item,m_secilen_check_box_column));
            check_box->setCheckState(Qt::Checked);
        }
    }
    else {
        m_selected_items.removeOne(item);
        if (m_is_multi_row EQ true) {
            QCheckBox * check_box = static_cast<QCheckBox *>(m_tree_widget->itemWidget(item,m_secilen_check_box_column));
            check_box->setCheckState(Qt::Unchecked);
        }
    }
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SELECTED_LINE
***************************************************************************************/

void TREE_ARAMA_KERNEL::SELECTED_LINE (QStringList selected_item)
{
    Q_UNUSED ( selected_item );
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::BUTTON_CLICKED
***************************************************************************************/

void TREE_ARAMA_KERNEL::BUTTON_CLICKED (QAbstractButton *, QStringList )
{

}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::REFRESH_TABLE
***************************************************************************************/

void TREE_ARAMA_KERNEL::REFRESH_TREE_WIDGET()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    if ( item EQ NULL ) {
        return;
    }

    m_tree_widget->collapseItem(item->parent());
    m_tree_widget->expandItem  (item->parent());
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SLOT_BUTTON_CLICKED
***************************************************************************************/

void TREE_ARAMA_KERNEL::SLOT_BUTTON_CLICKED()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    if (item EQ NULL) {
        return;
    }

    QAbstractButton * button = ( QAbstractButton * )sender();
    BUTTON_CLICKED ( button, GET_COLUMN_DATAS_FROM_ITEM(item) );
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SET_COLUMN_HEADERS
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_COLUMN_HEADERS ( QStringList column_headers )
{
    m_headers = column_headers;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::ADD_PARENT_ITEM
***************************************************************************************/

void TREE_ARAMA_KERNEL::ADD_PARENT_ITEM (QStringList column_datas, int number_of_child)
{
    QTreeWidgetItem * parent_item = new QTreeWidgetItem(column_datas);
    m_tree_widget->addTopLevelItem(parent_item);

    if (column_datas.last() EQ "ADD_CHECK_BOX") {
        QCheckBox * check_box = new QCheckBox;
        check_box->setCheckState(Qt::Unchecked);
        m_tree_widget->setItemWidget(parent_item,column_datas.size() - 1 , check_box);
    }

    if (number_of_child > 0) {
        QTreeWidgetItem * child_item = new QTreeWidgetItem(column_datas);
        parent_item->addChild(child_item);
    }
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::ADD_CHILD_ITEM
***************************************************************************************/

void TREE_ARAMA_KERNEL::ADD_CHILD_ITEM (QStringList column_datas, int number_of_child)
{
    QTreeWidgetItem * parent_item = m_last_expanded_item;

    if (parent_item EQ NULL) {
        return;
    }

    QTreeWidgetItem * child_item = new QTreeWidgetItem(column_datas);
    parent_item->addChild(child_item);

    if (column_datas.last() EQ "ADD_CHECK_BOX") {
        QCheckBox * check_box = new QCheckBox;
        check_box->setCheckState(Qt::Unchecked);
        m_tree_widget->setItemWidget(child_item,column_datas.size() - 1 , check_box);
    }

    if (number_of_child > 0) {
        child_item->addChild(new QTreeWidgetItem(column_datas));
    }
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::UPDATE_SELECTED_ITEM
***************************************************************************************/

void TREE_ARAMA_KERNEL::UPDATE_SELECTED_ITEM (QStringList column_datas)
{
    QTreeWidgetItem * parent_item = m_tree_widget->currentItem();

    if (parent_item EQ NULL) {
        return;
    }

    QTreeWidgetItem * child_item = new QTreeWidgetItem(column_datas);
    m_tree_widget->setCurrentItem(child_item);
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SET_TREE_WIDGET_DEFAULTS
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_TREE_WIDGET_DEFAULTS()
{

}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::ITEM_COLLAPSED
***************************************************************************************/

void TREE_ARAMA_KERNEL::HIDE_COLUMN ( int column_name )
{
    m_tree_widget->hideColumn ( column_name );
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ITEM_EXPANDED
***************************************************************************************/

void TREE_ARAMA_KERNEL::ITEM_EXPANDED (QTreeWidgetItem * item)
{
    for ( int i = item->childCount() - 1 ; i >= 0 ; i-- ) {
        item->removeChild(item->child(i));
    }

    m_last_expanded_item = item;

    QStringList column_datas;
    column_datas = GET_COLUMN_DATAS_FROM_ITEM(item);

    ADD_CHILD_ITEMS ( column_datas );

    m_last_expanded_item = NULL;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ITEM_COLLAPSED
***************************************************************************************/

void TREE_ARAMA_KERNEL::ITEM_COLLAPSED (QTreeWidgetItem * )
{

}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SET_NAME_OF_RUN_SEARCH_BUTTON
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_NAME_OF_SEARCH_BUTTON ( QString name_of_run_search_button )
{
    p_name_of_run_search_button = name_of_run_search_button;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SET_ENTER_KEY_FOR_RUN_SEARCH
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_ENTER_KEY_FOR_SEARCH ( bool enable )
{
    p_run_search_on_enter = enable;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SET_ICON_OF_SEARCH_BUTTON
***************************************************************************************/

void TREE_ARAMA_KERNEL::SET_ICON_OF_SEARCH_BUTTON ( QString filename )
{
    p_icon_file_name = filename;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::REGISTER_BUTTON_WIDGET
***************************************************************************************/

void TREE_ARAMA_KERNEL::REGISTER_BUTTON_WIDGET ( QWidget * search_buttons_widget, bool is_yardim_button_visible )
{
    QHBoxLayout * search_buttons_widget_layout        = new QHBoxLayout ( search_buttons_widget);

    search_buttons_widget ->setObjectName ( "kernel_widget");
    search_buttons_widget ->setLayout ( search_buttons_widget_layout );

    if ( is_yardim_button_visible EQ true ) {
        QToolButton * yardim_button = KERNEL_CREATE_DEFAULT_HELP_BUTTON(search_buttons_widget);
        yardim_button -> installEventFilter(this);
        search_buttons_widget_layout -> addWidget( yardim_button );
        connect ( yardim_button   , SIGNAL(clicked()), this, SLOT ( YARDIM_BUTTON_CLICKED() ) );
    }

    run_search_button = new QToolButton ( search_buttons_widget );
    run_search_button -> setSizePolicy ( QSizePolicy::Minimum,QSizePolicy::Minimum );
    run_search_button -> setToolButtonStyle ( Qt::ToolButtonTextUnderIcon );
    run_search_button -> setText ( p_name_of_run_search_button );
    run_search_button -> setIcon ( QIcon ( p_icon_file_name ) );
    run_search_button -> setIconSize ( QSize(32,32) );
    run_search_button -> installEventFilter ( this );

    search_buttons_widget_layout -> addWidget( run_search_button );

    connect(run_search_button, SIGNAL(clicked()), this, SLOT(SLOT_SEARCH_BUTTON()));
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::SLOT_SEARCH_BUTTON
***************************************************************************************/

void TREE_ARAMA_KERNEL::SLOT_SEARCH_BUTTON()
{
    run_search_button->setDisabled(true);

    if ( CHECK_VAR_CONTROL() EQ false ) {
        run_search_button->setDisabled(false);
        return;
    }

    if ( CHECK_EMPTY() EQ ADAK_FAIL ) {
        run_search_button->setDisabled ( false );
        return;
    }

    SEARCH();

    run_search_button->setDisabled ( false );
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::eventFilter
***************************************************************************************/

bool TREE_ARAMA_KERNEL::eventFilter ( QObject *obj, QEvent *event )
{
    //--------------------- USER VE RESIZE EVENTS

    if  ( event->type() EQ QEvent::User OR event->type() EQ QEvent::Resize ) {
         return BASE_KERNEL::eventFilter ( obj,event );
    }


    // ---------------------------- KEY PRESS

    if ( event->type() EQ QEvent::KeyPress ) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ( keyEvent->key() EQ Qt::Key_Escape ) {
            ADAK_CLOSE();
            return true;
        }

        if ( obj->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
            if ( event->type() EQ QEvent::Wheel ) {
                 return true;
            }
        }

        if (  ( keyEvent->key() EQ Qt::Key_Enter OR keyEvent->key() EQ Qt::Key_Return) AND p_run_search_on_enter ) {
            SLOT_SEARCH_BUTTON();
            return true;
        }
    }

    // ---------------------------- CLOSE EVENT

    if ( event->type() EQ QEvent::Close ) {
        return BASE_KERNEL::eventFilter(obj,event);
    }

    if ( close_event_started EQ true) {
         return false;
    }

    // ---------------------------- FOCUS IN EVENT

    if ( event->type() EQ QEvent::FocusIn ) {
        return CHECK_VAR_CONTROL ( obj );
    }

    return false;
}

/**************************************************************************************
                   TREE_ARAMA_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE
***************************************************************************************/

void TREE_ARAMA_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE ( int return_value )
{
    switch( return_value ) {
        case ADAK_FAIL:
            return;
            break;
        case ADAK_EXIT:
            abort();
        case ADAK_OK:
        default:
            break;
    }
    return;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::GET_COLUMN_DATAS_FROM_ITEM
***************************************************************************************/

QStringList TREE_ARAMA_KERNEL::GET_COLUMN_DATAS_FROM_ITEM(QTreeWidgetItem * item)
{
    if (item EQ NULL) {
        return QStringList();
    }

    QStringList column_datas;

    for ( int i = 0 ; i < item->columnCount() ; i++ ) {
        column_datas << item->text(i);
    }

    return column_datas;
}
