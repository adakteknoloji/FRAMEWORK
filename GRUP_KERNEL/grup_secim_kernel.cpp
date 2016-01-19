#include "grup_secim_kernel.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "adak_gruplar.h"
#include "grup_kernel.h"

/**************************************************************************************
                                 OPEN_GRUP_SECIM
***************************************************************************************/

bool OPEN_GRUP_SECIM (int program_id, int module_id, QList<int> *secilen_idler, ADAK_SQL *db, QWidget *parent,bool ana_gruplar_secilebilir_mi)
{
    bool ids_changed = false;

    GRUP_SECIM_KERNEL * tree_secim = new GRUP_SECIM_KERNEL(program_id , module_id ,&ids_changed, db , parent , ana_gruplar_secilebilir_mi);
    tree_secim->SET_SECILEN_ID_PTR      ( secilen_idler);
    tree_secim->INIT_GRUP_SECIM_KERNEL  ( parent,db    );
    tree_secim->EXEC(NOT_FULL_SCREEN);

    return ids_changed;
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::GRUP_SECIM_KERNEL
***************************************************************************************/

GRUP_SECIM_KERNEL::GRUP_SECIM_KERNEL ( int program_id, int module_id, bool * ids_changed, ADAK_SQL *, QWidget *parent, bool ana_gruplar_secilebilir_mi ) : TREE_SECIM_KERNEL ( parent )
{
    SET_ANA_GRUPLAR_SELECTABLE(ana_gruplar_secilebilir_mi);

    BASE_GRUP_KERNEL::GRP_SET_VISIBLE_PROG_MODULE( program_id , module_id );

    setMinimumWidth     (780);
    setMinimumHeight    (300);

    m_ids_changed      = ids_changed;

    SET_HELP_PAGE    ( tr("Accounting-account-Groups") );
    SET_PAGE_TITLE   ( tr("GROUP SELECTION") );
    SET_SETTING_NAME ( "GRUP_TREE_SECIMI" );    
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::~GRUP_SECIM_KERNEL
***************************************************************************************/

GRUP_SECIM_KERNEL::~GRUP_SECIM_KERNEL()
{

}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::SET_SECILEN_ID_PTR
***************************************************************************************/

void GRUP_SECIM_KERNEL::SET_SECILEN_ID_PTR(int * secilen_id)
{
    m_secilen_id         =   secilen_id;
    m_copy_secilen_idler << *secilen_id;
    DISABLE_MULTI_ROW_SELECTION();
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::SET_SECILEN_ID_PTR
***************************************************************************************/

void GRUP_SECIM_KERNEL::SET_SECILEN_ID_PTR(QList<int> * secilen_idler)
{
    m_secilen_idler = secilen_idler;

    for ( int i = 0 ; i < m_secilen_idler->size() ; i++ ) {
        m_copy_secilen_idler << secilen_idler->at(i);
    }

    ENABLE_MULTI_ROW_SELECTION();
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::INIT_GRUP_SECIM_KERNEL
***************************************************************************************/

void GRUP_SECIM_KERNEL::INIT_GRUP_SECIM_KERNEL ( QWidget * , ADAK_SQL *db )
{
    SET_WINDOW_SIZE ( 300,800 );

    m_id_column = 4;

    int hide_column =  4;

    if ( m_is_multi_row EQ true ) {
        SET_COLUMN_HEADERS ( QStringList()<<tr("")<<tr ( "Group Code")<<tr ( "Group Name" )<<tr ( "Sub-Group \nNumber" )
                                          <<tr( "Number of Records \nIn Group")<< tr ( "id")<<tr(" ") );

        hide_column = 5 ;
    }
    else {
        SET_COLUMN_HEADERS ( QStringList()<<tr ( "Group Code")<<tr ( "Group Name" )<<tr ( "Sub Group \nNumber" )
                                          <<tr ( "Number of Records \nIn Group")<< tr ( "id"));
    }

    if ( db NE NULL ) {
         m_grup_secim_db = db;
    }

    INIT_KERNEL ( this, db );

    if (hide_column EQ 5) {
        m_tree_widget->setColumnWidth( 0, 50  );
        m_tree_widget->setColumnWidth( 1, 140 );
        m_tree_widget->setColumnWidth( 2, 280 );
        m_tree_widget->setColumnWidth( 3, 60  );
        m_tree_widget->setColumnWidth( 4, 80  );
        m_tree_widget->setColumnWidth( 6, 50  );
        // Total                       =  660
    }
    else {
        m_tree_widget->setColumnWidth( 1, 140);
        m_tree_widget->setColumnWidth( 2, 340);
        m_tree_widget->setColumnWidth( 3, 80 );
        m_tree_widget->setColumnWidth( 4, 100);
    }

    HIDE_COLUMN(hide_column);
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::GRUP_SECIM_KERNEL
***************************************************************************************/

void GRUP_SECIM_KERNEL::FILL_TREE_TABLE()
{
    //Buraya dustugunde amacimiz ADD_PARENT_ITEM la parentleri ekrana eklememiz lazim.

    SQL_QUERY sql_query      ( m_grup_secim_db );

    QString table_name  = "";
    QString column_name = "";
    QString where_name  = "";
    QString order_by    = "";

    CREATE_SELECT_QUERY(&table_name,&column_name,&where_name,&order_by,0);

    sql_query.PREPARE_SELECT ( table_name,column_name,where_name,"" );

    if ( sql_query.SELECT(order_by) NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
           QStringList grub_row_str;
           grub_row_str << sql_query.VALUE(3).toString()      << sql_query.VALUE(4).toString() <<
                        sql_query.VALUE(8).toString()         << sql_query.VALUE(7).toString() << sql_query.VALUE(0).toString() ;

           bool select_checkbox = false;

           if (m_is_multi_row EQ true) {
               if (m_secilen_idler->contains(sql_query.VALUE(0).toInt()) EQ true) {
                   select_checkbox = true;
               }
           }

           ADD_PARENT_ITEM ( grub_row_str , sql_query.VALUE(8).toInt(),select_checkbox);
       }
    }
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::GRUP_SECIM_KERNEL
***************************************************************************************/

void GRUP_SECIM_KERNEL::ADD_CHILD_ITEMS (QStringList item)
{
    int item_id = item.at( m_id_column ).toInt();

    SQL_QUERY sql_query      ( m_grup_secim_db );

    QString table_name  = "";
    QString column_name = "";
    QString where_name  = "";
    QString order_by    = "";

    CREATE_SELECT_QUERY(&table_name,&column_name,&where_name,&order_by,item_id);

    sql_query.PREPARE_SELECT ( table_name,column_name,where_name,"" );

    if ( sql_query.SELECT(order_by) NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
           QStringList grub_row_str;
           grub_row_str << sql_query.VALUE(3).toString() << sql_query.VALUE(4).toString() <<
                           sql_query.VALUE(8).toString() << sql_query.VALUE(7).toString() <<
                           sql_query.VALUE(0).toString() ;

           bool select_checkbox = false;

           if (m_is_multi_row EQ true) {
               if (m_secilen_idler->contains(sql_query.VALUE(0).toInt()) EQ true) {
                   select_checkbox = true;
               }
           }

           ADD_CHILD_ITEM( grub_row_str , sql_query.VALUE(8).toInt(),select_checkbox);

       }
    }
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::SINGLE_LINE_SELECTED
***************************************************************************************/

int GRUP_SECIM_KERNEL::SINGLE_LINE_SELECTED(QStringList selected_item)
{
    if (m_is_multi_row EQ false) {
        *m_secilen_id     = selected_item.at( m_id_column ).toInt();
    }
    else {
        m_secilen_idler->clear();
        *m_secilen_idler << selected_item.at( m_id_column ).toInt();
    }

    if (m_copy_secilen_idler.isEmpty() EQ false) {
        if (selected_item.at( m_id_column ).toInt() NE m_copy_secilen_idler.last()) {
            *m_ids_changed = true;
        }
    }
    else {
        *m_ids_changed = true;
    }

    return ADAK_EXIT;
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::MULTI_LINE_SELECTED
***************************************************************************************/

int GRUP_SECIM_KERNEL::MULTI_LINE_SELECTED(QList<QStringList> selected_items)
{
    m_secilen_idler->clear();

    for ( int i = 0 ; i < selected_items.size() ; i++ ) {        
        *m_secilen_idler << selected_items.at(i).at( m_id_column ).toInt();
    }

    if (selected_items.size() EQ m_copy_secilen_idler.size()) {
        for ( int i = 0 ; i < selected_items.size() ; i++ ) {
            if (m_copy_secilen_idler.contains(selected_items.at(i).at( m_id_column ).toInt()) EQ false) {
                *m_ids_changed = true;
                break;
            }
        }
    }
    else {
        *m_ids_changed = true;
    }

    return ADAK_EXIT;
}

/**************************************************************************************
                   GRUP_SECIM_KERNEL::SELECTED_LINE
***************************************************************************************/

void GRUP_SECIM_KERNEL::SELECTED_LINE(QStringList column_datas)
{
    int id = QVariant(column_datas.at(m_id_column)).toInt();

    if ( m_secilen_idler->contains(id) EQ true) {
        m_secilen_idler->removeOne(id);
        return;
    }

    *m_secilen_idler << id;
}
