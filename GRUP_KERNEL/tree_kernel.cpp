#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDialog>
#include <QMdiSubWindow>
#include <QTreeWidget>
#include <QTextEdit>
#include <QAbstractButton>
#include <QAction>

#include "tree_kernel.h"
#include "adak_utils.h"
#include "adak_defines.h"
#include "kernel_utils.h"

/**************************************************************************************
                   TREE_KERNEL::TREE_KERNEL
***************************************************************************************/

TREE_KERNEL::TREE_KERNEL ( QWidget * parent ) : BASE_TREE_KERNEL ( parent )
{
    m_kernel_button_text        = "";
}

/**************************************************************************************
                   TREE_KERNEL::~TREE_KERNEL
***************************************************************************************/

TREE_KERNEL::~TREE_KERNEL()
{

}

/**************************************************************************************
                   TREE_KERNEL::INIT_KERNEL
***************************************************************************************/

void TREE_KERNEL::INIT_KERNEL ( QObject * parent,ADAK_SQL * database )
{
    SET_ADAK_WINDOW_DEFAULTS ( parent   );
    SET_DB_CONN               ( database );

    m_tree_widget->setHeaderLabels(m_column_headers);

    SET_TREE_WIDGET_DEFAULTS();

    ADD_PARENT_ITEMS();
    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   TREE_KERNEL::REGISTER_BUTTON
***************************************************************************************/

void TREE_KERNEL::REGISTER_BUTTON ( QAbstractButton * button )
{
    connect(button, SIGNAL ( clicked()), SLOT ( SLOT_BUTTON_CLICKED() ) );
}

/**************************************************************************************
                   TREE_KERNEL::REGISTER_ADD_BUTTON
***************************************************************************************/

void TREE_KERNEL::REGISTER_ADD_BUTTON ( QAbstractButton * button )
{
    connect ( button, SIGNAL ( clicked()), SLOT ( SLOT_ADD_ITEM() ) );
}

/**************************************************************************************
                   TREE_KERNEL::REGISTER_UPDATE_BUTTON
***************************************************************************************/

void TREE_KERNEL::REGISTER_UPDATE_BUTTON ( QAbstractButton * button )
{
    connect ( button, SIGNAL ( clicked() ) , SLOT ( SLOT_UPDATE_ITEM() ) );
}

/**************************************************************************************
                   TREE_KERNEL::REGISTER_DELETE_BUTTON
***************************************************************************************/

void TREE_KERNEL::REGISTER_DELETE_BUTTON ( QAbstractButton * button)
{
    connect(button, SIGNAL( clicked() ), SLOT ( SLOT_DELETE_ITEM() ) );
}

/**************************************************************************************
                   TREE_KERNEL::REGISTER_DELETE_BUTTON
***************************************************************************************/

void TREE_KERNEL::REGISTER_HELP_BUTTON ( QAbstractButton * button )
{
    button -> setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);

    button -> setText(tr("Yardım"));
    button -> setIcon(QIcon(":/kernel_icons/kernel_yardim_ikonu.png"));
    button -> setIconSize(QSize(24,32));
    button -> setToolTip("F1");
    button -> setShortcut(QKeySequence(Qt::Key_F1));

    connect ( button, SIGNAL ( clicked() ),   this, SLOT ( YARDIM_BUTTON_CLICKED()));
}

/**************************************************************************************
                   TREE_KERNEL::SLOT_BUTTON_CLICKED
***************************************************************************************/

void TREE_KERNEL::SLOT_BUTTON_CLICKED()
{
    QTreeWidgetItem * item   = m_tree_widget->currentItem();
    QAbstractButton * button = ( QAbstractButton * )sender();

    QStringList column_datas;
    column_datas = GET_COLUMN_DATAS_FROM_ITEM(item);

    BUTTON_CLICKED ( button, column_datas );
}

/**************************************************************************************
                   TREE_KERNEL::BUTTON_CLICKED
***************************************************************************************/

void TREE_KERNEL::BUTTON_CLICKED ( QAbstractButton * ,QStringList )
{

}

/**************************************************************************************
                   TREE_KERNEL::SET_ACTION_MENU_TEXT
***************************************************************************************/

void TREE_KERNEL::SET_ACTION_MENU_TEXT ( QString add_button,QString update_button,QString delete_button )
{
    item_add->setText    ( add_button    );
    item_update->setText ( update_button );
    item_delete->setText ( delete_button );
}

/**************************************************************************************
                   TREE_KERNEL::SET_TREE_WIDGET_DEFAULTS
***************************************************************************************/

void TREE_KERNEL::SET_TREE_WIDGET_DEFAULTS()
{
    item_add     = new QAction(this);
    item_delete  = new QAction(this);
    item_update  = new QAction(this);

    item_add->setText    ( "Add Item" );
    item_delete->setText ( "Delete Item" );
    item_update->setText ( "Update Item" );

    m_tree_widget->addAction ( item_add );
    m_tree_widget->addAction ( item_delete );
    m_tree_widget->addAction ( item_update );

    m_tree_widget->setContextMenuPolicy ( Qt::ActionsContextMenu );

    connect ( item_add   , SIGNAL ( triggered() ),this, SLOT ( SLOT_ADD_ITEM() ) );
    connect ( item_update, SIGNAL ( triggered() ),this, SLOT ( SLOT_UPDATE_ITEM() ) );
    connect ( item_delete, SIGNAL ( triggered() ),this, SLOT ( SLOT_DELETE_ITEM()) );
}

/**************************************************************************************
                   TREE_KERNEL::SLOT_ADD_ITEM
***************************************************************************************/

void TREE_KERNEL::SLOT_ADD_ITEM()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    QStringList column_datas = GET_COLUMN_DATAS_FROM_ITEM(item);

    ADD_ITEM ( sender(), column_datas );
}

/**************************************************************************************
                   TREE_KERNEL::SLOT_UPDATE_ITEM
***************************************************************************************/

void TREE_KERNEL::SLOT_UPDATE_ITEM()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    QStringList column_datas;
    column_datas = GET_COLUMN_DATAS_FROM_ITEM(item);

    UPDATE_ITEM ( column_datas );
}

/**************************************************************************************
                   TREE_KERNEL::SLOT_DELETE_ITEM
***************************************************************************************/

void TREE_KERNEL::SLOT_DELETE_ITEM()
{
    QTreeWidgetItem * item = m_tree_widget->currentItem();

    QStringList column_datas;
    column_datas = GET_COLUMN_DATAS_FROM_ITEM(item);

    if ( CHECK_DELETE_ITEM( column_datas ) EQ ADAK_OK) {
        DELETE_ITEM( column_datas );
        m_last_deleted_item_parent = item->parent();
        int item_index = m_tree_widget->indexOfTopLevelItem(item);
        m_tree_widget->takeTopLevelItem(item_index);
        delete item;
    }
}

/**************************************************************************************
                   TREE_KERNEL::ADD_VALUE_TO_ADDED_ITEM_PARENT
***************************************************************************************/

void TREE_KERNEL::ADD_VALUE_TO_ADDED_ITEM_PARENT(int added_column, int value)
{
    if ( m_last_added_item_parent EQ NULL ) {
        return;
    }

    int previous_value = m_last_added_item_parent->text(added_column).toInt();
    m_last_added_item_parent->setText(added_column,QVariant((previous_value + value)).toString());
}

/**************************************************************************************
                   TREE_KERNEL::ADD_VALUE_TO_ADDED_ITEM_PARENT
***************************************************************************************/

void TREE_KERNEL::ADD_VALUE_TO_DELETED_ITEM_PARENT(int added_column, int value)
{
    QTreeWidgetItem * item = m_tree_widget->currentItem()->parent();

    if ( item EQ NULL ) {
        return;
    }

    int previous_value = item->text(added_column).toInt();
    item->setText(added_column,QVariant((previous_value + value)).toString());
}
