#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include "base_tree_kernel.h"
#include "adak_utils.h"
#include "adak_defines.h"
#include "kernel_utils.h"

/**************************************************************************************
                   BASE_TREE_KERNEL::BASE_TREE_KERNEL
***************************************************************************************/

BASE_TREE_KERNEL::BASE_TREE_KERNEL ( QWidget * parent ) : BASE_KERNEL ( parent )
{
    m_tree_widget               = NULL;
    m_last_expanded_item        = NULL;
    m_last_added_item           = NULL;
    m_column_info_start_index   = 0;

    SET_ANA_GRUPLAR_SELECTABLE(false);
}

/**************************************************************************************
                   BASE_TREE_KERNEL::~BASE_TREE_KERNEL
***************************************************************************************/

BASE_TREE_KERNEL::~BASE_TREE_KERNEL()
{

}

/**************************************************************************************
                   BASE_TREE_KERNEL::REGISTER_TREE_WIDGET
***************************************************************************************/

void BASE_TREE_KERNEL::REGISTER_TREE_WIDGET ( QTreeWidget * tree_widget )
{
    m_tree_widget = tree_widget;
    m_tree_widget ->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tree_widget ->setAlternatingRowColors(true);

    connect ( m_tree_widget, SIGNAL ( itemCollapsed ( QTreeWidgetItem *  )) ,this, SLOT (ITEM_COLLAPSED(QTreeWidgetItem *)));
    connect ( m_tree_widget, SIGNAL ( itemExpanded  ( QTreeWidgetItem *  )) ,this, SLOT (ITEM_EXPANDED(QTreeWidgetItem * )));
}

/**************************************************************************************
                   BASE_TREE_KERNEL::SET_RIGHT_ALIGN_COLUMNS
***************************************************************************************/

void BASE_TREE_KERNEL::SET_RIGHT_ALIGN_COLUMNS ( QList<int> columns )
{
    m_right_align_columns = columns;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::SET_RIGHT_ALIGN_COLUMNS
***************************************************************************************/

void BASE_TREE_KERNEL::SET_TREE_WIDGET_ITEM_ALIGN(QTreeWidgetItem *item)
{
    for ( int i = 0 ; i < m_right_align_columns.size() ; i++ ) {
        item->setTextAlignment(m_right_align_columns.at(i), Qt::AlignRight);
    }
}

/**************************************************************************************
                   BASE_TREE_KERNEL::SET_COLUMN_HEADERS
***************************************************************************************/

void BASE_TREE_KERNEL::SET_COLUMN_HEADERS ( QStringList column_headers )
{
    m_column_headers = column_headers;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ADD_PARENT_ITEM
***************************************************************************************/

void BASE_TREE_KERNEL::ADD_PARENT_ITEM ( QStringList column_datas, int num_of_child )
{
    QTreeWidgetItem * parent_item = new QTreeWidgetItem(column_datas);
    SET_TREE_WIDGET_ITEM_ALIGN(parent_item);

    m_tree_widget->addTopLevelItem(parent_item);    

    for ( int i = 0 ; i < column_datas.size() ; i++ ) {
        if (column_datas.at(i) EQ "ADD_CHECK_BOX") {
            TREE_KERNEL_ADD_CHECK_BOX( parent_item , i , num_of_child ,false );
        }
        else if(column_datas.at(i) EQ "ADD_SELECTED_CHECK_BOX") {
            TREE_KERNEL_ADD_CHECK_BOX( parent_item , i , num_of_child ,true  );
        }
    }

    m_last_added_item = parent_item;

    if (num_of_child > 0) {
        QTreeWidgetItem * child_item = new QTreeWidgetItem(column_datas);
        parent_item->addChild(child_item);
    }
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ADD_CHILD_ITEM
***************************************************************************************/

void BASE_TREE_KERNEL::ADD_CHILD_ITEM ( QStringList column_datas, int num_of_child )
{
    QTreeWidgetItem * parent_item = m_tree_widget->currentItem();

    if (m_last_expanded_item NE NULL) {
        parent_item = m_last_expanded_item;
    }

    if (parent_item EQ NULL) {
        return;
    }

    QTreeWidgetItem * child_item = new QTreeWidgetItem(column_datas);
    SET_TREE_WIDGET_ITEM_ALIGN(child_item);
    parent_item->addChild(child_item);

    for ( int i = 0 ; i < column_datas.size() ; i++ ) {
        if (column_datas.at(i) EQ "ADD_CHECK_BOX") {
            TREE_KERNEL_ADD_CHECK_BOX( child_item , i , num_of_child ,false );
        }
        else if(column_datas.at(i) EQ "ADD_SELECTED_CHECK_BOX") {
            TREE_KERNEL_ADD_CHECK_BOX( child_item , i , num_of_child ,true  );
        }
    }

    m_last_added_item = child_item;

    if (num_of_child > 0) {
        child_item->addChild(new QTreeWidgetItem(column_datas));
    }
}

/**************************************************************************************
                   BASE_TREE_KERNEL::TREE_KERNEL_ADD_CHECK_BOX
***************************************************************************************/

void BASE_TREE_KERNEL::TREE_KERNEL_ADD_CHECK_BOX(QTreeWidgetItem *item ,int column, int num_of_childs, bool select_item_if_multi_row_enable )
{
    item->setText       ( column , ""            );

    if (m_ana_gruplar_selectable EQ false AND num_of_childs > 0 ) {
        return;
    }

    if (select_item_if_multi_row_enable EQ false) {
        item->setCheckState ( column , Qt::Unchecked );
    }
    else {
        item->setCheckState ( column , Qt::Checked);
    }

    //if (m_ana_gruplar_selectable EQ false AND num_of_childs > 0 ) {
    //    item->setHidden(true);
    //}
}

/**************************************************************************************
                   BASE_TREE_KERNEL::GET_LAST_ADDED_ITEM
***************************************************************************************/

QTreeWidgetItem * BASE_TREE_KERNEL::GET_LAST_ADDED_ITEM()
{
    if (m_last_added_item EQ NULL) {
        return NULL;
    }

    QTreeWidgetItem * item_temp = m_last_added_item;

    m_last_added_item = NULL;

    return item_temp;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::UPDATE_SELECTED_ITEM
***************************************************************************************/

void BASE_TREE_KERNEL::UPDATE_SELECTED_ITEM ( QStringList column_datas )
{
    QTreeWidgetItem * parent_item = m_tree_widget->currentItem();

    if (parent_item EQ NULL OR column_datas.size() EQ 0) {
        return;
    }

    for ( int i = 0 ; i < column_datas.size() ; i++ ) {
        parent_item->setText(i,column_datas.at(i));
    }
}

/**************************************************************************************
                   BASE_TREE_KERNEL::ITEM_EXPANDED
***************************************************************************************/

void BASE_TREE_KERNEL::ITEM_EXPANDED (QTreeWidgetItem * item)
{
    for ( int i = item->childCount() - 1 ; i >= 0 ; i-- ) {
        qDeleteAll(item->takeChildren());
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

void BASE_TREE_KERNEL::ITEM_COLLAPSED (QTreeWidgetItem * )
{

}

/**************************************************************************************
                   BASE_TREE_KERNEL::HIDE_COLUMN
***************************************************************************************/

void BASE_TREE_KERNEL::HIDE_COLUMN ( int column )
{
    m_tree_widget->setColumnHidden( column , true );
}

/**************************************************************************************
                   BASE_TREE_KERNEL::GET_COLUMN_DATAS_FROM_ITEM
***************************************************************************************/

QStringList BASE_TREE_KERNEL::GET_COLUMN_DATAS_FROM_ITEM(QTreeWidgetItem * item)
{
    if (item EQ NULL) {
        return QStringList();
    }

    QStringList column_datas;

    for ( int i = m_column_info_start_index ; i < item->columnCount() ; i++ ) {
        column_datas << item->text(i);
    }

    return column_datas;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::UPDATE_TREE_VIEW
***************************************************************************************/

void BASE_TREE_KERNEL::UPDATE_TREE_VIEW()
{
    //m_tree_widget->update();
}

/**************************************************************************************
                   BASE_TREE_KERNEL::REFRESH-TREE_VIEW
***************************************************************************************/

void BASE_TREE_KERNEL::REFRESH_TREE_WIDGET()
{
    m_tree_widget->clear();
    ADD_PARENT_ITEMS();
}

/**************************************************************************************
                   BASE_TREE_KERNEL::REFRESH-TREE_VIEW
***************************************************************************************/

void BASE_TREE_KERNEL::REFRESH_TREE_SELECTED_ITEM()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    if ( item EQ NULL ) {
        return;
    }

    if (item->childCount() EQ 0) {
        item->addChild(new QTreeWidgetItem(GET_COLUMN_DATAS_FROM_ITEM(item)));
    }

    m_tree_widget->collapseItem(item);
    m_tree_widget->expandItem  (item);
}

/**************************************************************************************
                   BASE_TREE_KERNEL::SET_ANA_GRUPLAR_SELECTABLE
***************************************************************************************/

void BASE_TREE_KERNEL::SET_ANA_GRUPLAR_SELECTABLE(bool value)
{
    m_ana_gruplar_selectable = value;
}

/**************************************************************************************
                   BASE_TREE_KERNEL::GET_ANA_GRUPLAR_SELECTABLE
***************************************************************************************/

int BASE_TREE_KERNEL::GET_ANA_GRUPLAR_SELECTABLE()
{
    return m_ana_gruplar_selectable;
}
