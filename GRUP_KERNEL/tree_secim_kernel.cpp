#include <QHeaderView>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QEvent>
#include <QAction>
#include <QDebug>
#include <QMdiSubWindow>
#include <QSortFilterProxyModel>
#include "tree_secim_kernel.h"
#include "adak_utils.h"
#include "adak_defines.h"
#include "kernel_utils.h"

/**************************************************************************************
                   TREE_SECIM_KERNEL::TREE_SECIM_KERNEL
***************************************************************************************/

TREE_SECIM_KERNEL::TREE_SECIM_KERNEL ( QWidget * parent ) : BASE_TREE_KERNEL ( parent )
{
    m_kernel_button_text        = "";

    p_height                    = 400;
    p_width                     = 400;

    setModal(true);
    p_h_box_layout              = new QHBoxLayout;

    p_h_box_layout->addItem( new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) );

    DISABLE_MULTI_ROW_SELECTION();
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::~TREE_SECIM_KERNEL
***************************************************************************************/

TREE_SECIM_KERNEL::~TREE_SECIM_KERNEL()
{

}

/**************************************************************************************
                   TREE_SECIM_KERNEL::INIT_KERNEL
***************************************************************************************/

void TREE_SECIM_KERNEL::INIT_KERNEL ( QObject * parent,ADAK_SQL * database )
{
    Q_UNUSED  ( parent );

    m_secilen_check_box_column = -1;

    if (m_is_multi_row EQ true) {
        m_secilen_check_box_column = m_column_headers.size() - 1;
    }

    QVBoxLayout * vertical_layout = new QVBoxLayout(this);

    QTreeWidget * widget = new QTreeWidget;
    widget->clear();

    REGISTER_TREE_WIDGET(widget);
    m_tree_widget->setHeaderLabels(m_column_headers);

    widget->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    setLayout(vertical_layout);

    resize(p_width, p_height);

    SET_ADAK_WINDOW_DEFAULTS(this);
    SET_DB_CONN ( database );

    FILL_TREE_TABLE();

    vertical_layout->addWidget ( m_tree_widget  );
    vertical_layout->addLayout ( p_h_box_layout );

    SET_TREE_WIDGET_DEFAULTS();    
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::GET_TREE_VIEW
***************************************************************************************/

QTreeWidget * TREE_SECIM_KERNEL::GET_TREE_VIEW()
{
    return m_tree_widget;
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::ADD_BUTTON
***************************************************************************************/

QToolButton * TREE_SECIM_KERNEL::ADD_BUTTON ( QString button_text,Qt::ToolButtonStyle style,QString icon,int height,int width )
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
                   TREE_SECIM_KERNEL::SET_WINDOW_SIZE
***************************************************************************************/

void TREE_SECIM_KERNEL::SET_WINDOW_SIZE ( int height, int width )
{
    p_height = height;
    p_width  = width;
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::FIND_SELECTED_ROW
***************************************************************************************/

void TREE_SECIM_KERNEL::FIND_SELECTED_ROW ()
{
    int return_value = ADAK_CONTINUE;

    if (sender()->metaObject()->className() NE QToolButton::staticMetaObject.className()) {
        QTreeWidgetItem * item = m_tree_widget->currentItem();
        if (m_selected_items.contains(item) EQ false AND ( item->childCount() EQ 0 OR GET_ANA_GRUPLAR_SELECTABLE() > 0 ) ) {
            m_selected_items << item;
        }
    }

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
                   TREE_SECIM_KERNEL::SLOT_ROW_CLICKED
***************************************************************************************/

void TREE_SECIM_KERNEL::SLOT_ROW_CLICKED ()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    if ( item EQ NULL ) {
        return;
    }

    SELECTED_LINE ( GET_COLUMN_DATAS_FROM_ITEM(item) );

    if (m_is_multi_row EQ false) {
        m_selected_items.clear();
        m_selected_items << item;
        return;
    }

    QList<QTreeWidgetItem *> old_selected_items = m_selected_items;

    m_selected_items.clear();

    for ( int i = 0 ; i < m_tree_widget->topLevelItemCount() ; i++ ) {
        FIND_SELECTED_ITEMS(m_tree_widget->topLevelItem(i) , &old_selected_items );
    }
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::FIND_SELECTED_ITEMS
***************************************************************************************/

void TREE_SECIM_KERNEL::FIND_SELECTED_ITEMS(QTreeWidgetItem * parent_item , QList<QTreeWidgetItem *> * old_selected_items )
{
    if ( parent_item->childCount() EQ 0 OR GET_ANA_GRUPLAR_SELECTABLE() > 0 ) {
        if ( old_selected_items->contains(parent_item) EQ true ) {
            if ( parent_item->checkState(0) EQ Qt::Checked AND parent_item->checkState(m_secilen_check_box_column) EQ Qt::Checked) {
                m_selected_items << parent_item;
            }
            else {
                parent_item->setCheckState(0                          , Qt::Unchecked);
                parent_item->setCheckState(m_secilen_check_box_column , Qt::Unchecked);
            }
        }
        else {
            if ( parent_item->checkState(0) EQ Qt::Checked OR parent_item->checkState(m_secilen_check_box_column) EQ Qt::Checked) {
                m_selected_items << parent_item;
                parent_item->setCheckState(0                          , Qt::Checked);
                parent_item->setCheckState(m_secilen_check_box_column , Qt::Checked);
            }
        }
    }

    for ( int j = 0 ; j < parent_item->childCount() ; j++ ) {
        QTreeWidgetItem * child_item = parent_item->child(j);

        if ( old_selected_items->contains(child_item) EQ true ) {
            if ( child_item->checkState(0) EQ Qt::Checked AND child_item->checkState(m_secilen_check_box_column) EQ Qt::Checked) {
                m_selected_items << child_item;
            }
            else {
                child_item->setCheckState(0                          , Qt::Unchecked);
                child_item->setCheckState(m_secilen_check_box_column , Qt::Unchecked);
            }
        }
        else {
            if ( child_item->checkState(0) EQ Qt::Checked OR child_item->checkState(m_secilen_check_box_column) EQ Qt::Checked) {
                m_selected_items << child_item;
                child_item->setCheckState(0                          , Qt::Checked);
                child_item->setCheckState(m_secilen_check_box_column , Qt::Checked);
            }
        }

        if (child_item->childCount() > 0) {
            FIND_SELECTED_ITEMS(child_item , old_selected_items);
        }
    }
}


/**************************************************************************************
                   TREE_SECIM_KERNEL::SELECTED_LINE
***************************************************************************************/

void TREE_SECIM_KERNEL::SELECTED_LINE (QStringList )
{

}

/**************************************************************************************
                   TREE_SECIM_KERNEL::BUTTON_CLICKED
***************************************************************************************/

void TREE_SECIM_KERNEL::BUTTON_CLICKED (QAbstractButton * , QStringList )
{

}

/**************************************************************************************
                   TREE_SECIM_KERNEL::SLOT_BUTTON_CLICKED
***************************************************************************************/

void TREE_SECIM_KERNEL::SLOT_BUTTON_CLICKED()
{
    QAbstractButton * button = ( QAbstractButton * )sender();
    BUTTON_CLICKED ( button, QStringList());
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::SET_TREE_WIDGET_DEFAULTS
***************************************************************************************/

void TREE_SECIM_KERNEL::SET_TREE_WIDGET_DEFAULTS()
{
    m_tree_widget->setContextMenuPolicy ( Qt::ActionsContextMenu );

    connect(m_tree_widget , SIGNAL ( clicked       ( QModelIndex)),this, SLOT ( SLOT_ROW_CLICKED()  ) );
    connect(m_tree_widget , SIGNAL ( activated     ( QModelIndex)),this, SLOT ( FIND_SELECTED_ROW() ) );
    connect(m_tree_widget , SIGNAL ( doubleClicked ( QModelIndex)),this, SLOT ( FIND_SELECTED_ROW() ) );

    if (m_is_multi_row EQ true) {
        QToolButton * button = new QToolButton;
        button->setText ( tr("Add Selected Row(s)") );
        button->setIcon ( QIcon ( ":/kernel_icons/kernel_ekle_ikonu.png" ) );
        button->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

        p_h_box_layout->addWidget ( button );

        connect(button, SIGNAL ( clicked() ), this, SLOT ( FIND_SELECTED_ROW() ) );
    }
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::ENABLE_MULTI_ROW_SELECTION
***************************************************************************************/

void TREE_SECIM_KERNEL::ENABLE_MULTI_ROW_SELECTION()
{
    m_is_multi_row            = true;
    m_column_info_start_index = 1;
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::DISABLE_MULTI_ROW_SELECTION
***************************************************************************************/

void TREE_SECIM_KERNEL::DISABLE_MULTI_ROW_SELECTION()
{
    m_is_multi_row = false;
}

/**************************************************************************************
                   TREE_SECIM_KERNEL::DISABLE_MULTI_ROW_SELECTION
***************************************************************************************/

int TREE_SECIM_KERNEL::MULTI_LINE_SELECTED(QList<QStringList> )
{
    return ADAK_CONTINUE;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ADD_PARENT_ITEM
***************************************************************************************/

void TREE_SECIM_KERNEL::ADD_PARENT_ITEM (QStringList column_datas,int number_of_child, bool select_item_if_multi_row_enable)
{
    bool item_is_selected = false;

    if (m_is_multi_row EQ true) {
        if (select_item_if_multi_row_enable EQ false) {
            column_datas.insert   (0 , QString("ADD_CHECK_BOX"));
            column_datas       <<      QString("ADD_CHECK_BOX");
        }
        else {
            column_datas.insert   (0 , QString("ADD_SELECTED_CHECK_BOX"));
            column_datas            << QString("ADD_SELECTED_CHECK_BOX");
            item_is_selected       = true;
        }
    }

    BASE_TREE_KERNEL::ADD_PARENT_ITEM(column_datas,number_of_child);

    if (item_is_selected EQ true) {
        QTreeWidgetItem * item = GET_LAST_ADDED_ITEM();
        if ( item NE NULL ) {
            m_selected_items << item;
        }
    }
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ADD_CHILD_ITEM
***************************************************************************************/

void TREE_SECIM_KERNEL::ADD_CHILD_ITEM (QStringList column_datas,int number_of_child, bool select_item_if_multi_row_enable)
{
    bool item_is_selected = false;

    if (m_is_multi_row EQ true) {
        if (select_item_if_multi_row_enable EQ false) {
            column_datas.insert   (0 , QString("ADD_CHECK_BOX"));
            column_datas       <<      QString("ADD_CHECK_BOX");
        }
        else {
            column_datas.insert   (0 , QString("ADD_SELECTED_CHECK_BOX"));
            column_datas            << QString("ADD_SELECTED_CHECK_BOX");
            item_is_selected       = true;
        }
    }

    BASE_TREE_KERNEL::ADD_CHILD_ITEM(column_datas,number_of_child);

    if (item_is_selected EQ true) {
        QTreeWidgetItem * item = GET_LAST_ADDED_ITEM();
        if ( item NE NULL ) {
            m_selected_items << item;
        }
    }
}

/**************************************************************************************
                   BASE_TREE_KERNEL::UPDATE_SELECTED_ITEM
***************************************************************************************/

void TREE_SECIM_KERNEL::UPDATE_SELECTED_ITEM (QStringList column_datas)
{
    BASE_TREE_KERNEL::UPDATE_SELECTED_ITEM(column_datas);
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::ADD_PARENT_ITEMS
***************************************************************************************/

void TREE_SECIM_KERNEL::ADD_PARENT_ITEMS()
{
    FILL_TREE_TABLE();
}
