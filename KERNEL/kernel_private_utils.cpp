#include "adak_defines.h"

#include "kernel_private_utils.h"

/********************************************************/
/*             KERNEL_GET_SELECTED_ROWS_LIST            */
/********************************************************/

QList<int> KERNEL_TABLE_SELECTED_ROWS (QTableWidget * p_table_widget)
{
    QList<QTableWidgetItem *> rows = p_table_widget->selectedItems();

    QList<int> selected_rows;

    for ( int i = 0 ; i < rows.size() ; i++ ) {
        if (selected_rows.contains(rows.at(i)->row()) EQ false) {
            selected_rows << rows.at(i)->row();
        }
    }

    return selected_rows;
}

/********************************************************/
/*             KERNEL_CREATE_SEC_BUTTON                 */
/********************************************************/

QToolButton * KERNEL_CREATE_SEC_BUTTON(QLayout * buttonun_eklenecegi_layout,bool is_multi_select)
{
    Q_UNUSED(is_multi_select);

    QToolButton * tool_button = new QToolButton;
    tool_button->setText("Seç");

    tool_button->setIcon(QIcon(":/kernel_icons/kernel_getir_ikonu.png"));
    tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    buttonun_eklenecegi_layout->addWidget(tool_button);

    return tool_button;
}

/********************************************************/
/*            KERNEL_SET_TABLE_SELECTION_MODE           */
/********************************************************/

QLabel * KERNEL_CREATE_STATUS_LABEL(QLayout * labelin_eklenecegi_layout)
{
    QLabel * label_status                = new QLabel;
    label_status->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    label_status->setText(QObject::tr("Number of Records : 0"));
    labelin_eklenecegi_layout ->addWidget(label_status);

    return label_status;
}

/********************************************************/
/*            KERNEL_UPDATE_STATUS_LABEL                */
/********************************************************/

void KERNEL_UPDATE_STATUS_LABEL(QLabel * label,int bulunan_kayit_sayisi)
{
    label->setText(QObject::tr("Number of Records: %1").arg(bulunan_kayit_sayisi));//Bulunan Kayıt Sayısı : %1
}

/********************************************************/
/*            KERNEL_SET_TABLE_SELECTION_MODE           */
/********************************************************/

void KERNEL_SET_TABLE_SELECTION_MODE(QTableWidget * p_table_widget,bool is_multi_select)
{
    if (is_multi_select EQ true) {
        p_table_widget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    else {
        p_table_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

/********************************************************/
/*           KERNEL_SET_TABLE_CHECK_BOX_COLUMN          */
/********************************************************/

void KERNEL_SET_TABLE_CHECK_BOX_COLUMN(QTableWidget * p_table_widget,bool is_multi_select,int num_of_columms)
{
    if (is_multi_select EQ false) {
        return;
    }

    if (p_table_widget->columnCount() EQ num_of_columms) {
        p_table_widget->insertColumn(num_of_columms);
    }

    p_table_widget->setHorizontalHeaderItem(num_of_columms,new QTableWidgetItem(""));

    for ( int i = 0 ; i < p_table_widget->rowCount() ; i++ ) {
        QTableWidgetItem * item = new QTableWidgetItem("");
        p_table_widget->setItem(i,num_of_columms,item);
        item->setCheckState(Qt::Unchecked);
    }

    p_table_widget->setColumnWidth(num_of_columms,40);
}

/**************************************************************************************
                   BATCH_KERNEL::REGISTER_BUTTON_WIDGET
***************************************************************************************/

QToolButton * KERNEL_CREATE_DEFAULT_HELP_BUTTON(QWidget * parent)
{
    QToolButton * yardim_button = new QToolButton ( parent );    
    yardim_button -> setToolButtonStyle ( Qt::ToolButtonTextUnderIcon );
    yardim_button -> setText ( QObject::tr("Help" ));
    yardim_button -> setIcon ( QIcon(":/kernel_icons/kernel_yardim_ikonu.png"));
    yardim_button -> setIconSize ( QSize(24,24));
    yardim_button -> setToolTip ("F1");
    yardim_button -> setShortcut ( QKeySequence(Qt::Key_F1));

    return yardim_button;
}

/**************************************************************************************
                   BATCH_KERNEL::REGISTER_BUTTON_WIDGET
***************************************************************************************/

void KERNEL_UPDATE_TREE_VIEW (QTreeView * tree_view)
{
    tree_view->closePersistentEditor ( tree_view->selectionModel()->currentIndex() );
    tree_view->update();
}
