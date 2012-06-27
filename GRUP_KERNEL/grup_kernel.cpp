#include <stdarg.h>
#include "adak_utils.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_gruplar.h"
#include "grup_kernel.h"
#include "grup_kernel_grup_fisi.h"
#include "ui_grup_kernel.h"

/**************************************************************************************
                   GRUP_KERNEL::GRUP_KERNEL
***************************************************************************************/

void    OPEN_GRUPLAR ( int program_id , int module_id , QString p_win_title, QString p_help_page, ADAK_SQL * db , QWidget * parent)
{
    GRUP_KERNEL * grup_kernel = new GRUP_KERNEL( program_id, module_id, p_win_title, p_help_page, db, parent );
    grup_kernel->SHOW( FULL_SCREEN );

    // SEKME MENU KERNEL OTOMATIK BUYUTMUYOR. BATCH, FIS, FORM'DAN MIRAS ALMADIGI ICIN BUYURMUYOR
    // OTOMATIK BUYUMESI ICIN EKLENDI
    grup_kernel->setWindowState(Qt::WindowMaximized);
}

/**************************************************************************************
                   GRUP_KERNEL::GRUP_KERNEL
***************************************************************************************/

GRUP_KERNEL::GRUP_KERNEL (int program_id , int module_id , QString p_win_title, QString p_help_page, ADAK_SQL * db, QWidget * parent) : TREE_KERNEL ( parent ) , BASE_GRUP_KERNEL(), m_ui ( new Ui::GRUP_KERNEL )
{
    m_ui->setupUi     ( this );

    REGISTER_TREE_WIDGET   ( m_ui->tree_widget              );
    REGISTER_ADD_BUTTON    ( m_ui->push_button_add_alt_grup );
    REGISTER_ADD_BUTTON    ( m_ui->push_button_add_ana_grup );
    REGISTER_DELETE_BUTTON ( m_ui->push_button_grubu_sil    );
    REGISTER_HELP_BUTTON   ( m_ui->toolButton_help_button   );

    SET_FIRST_FOCUS_WIDGET( m_ui->push_button_add_ana_grup );
    m_grubu_parentine_aktar = false;

    SET_RIGHT_ALIGN_COLUMNS ( QList<int>() << 2 << 3 << 4);

    SET_COLUMN_HEADERS ( QStringList()<<tr ( "Grup Kodu")           <<tr ( "Grup İsmi" )
                                      <<tr ( "Alt Grup Sayısı")     <<tr ( "Gruptaki Kayıt Sayısı")
                                      <<tr ( "grup_id")             <<tr ( "grup_parent_id" )
                                      <<tr ( "grup_son_seviye_mi ") <<tr ( "tam_grup_kodu" ) );

    m_grp_kodu_column              = 0;
    m_grp_isim_column              = 1;
    m_grp_alt_grup_sayisi_column   = 2;
    m_grp_kayit_sayisi_column      = 3;
    m_grp_id_column                = 4;
    m_grp_parent_id_column         = 5;
    m_grp_son_seviye_mi_column     = 6;
    m_grp_tam_grup_kodu_column     = 7;

    GRP_SET_VISIBLE_PROG_MODULE(program_id,module_id);
    SET_DELIMETER ( " " );

    if ( p_win_title.isEmpty() EQ false  ) {
        SET_PAGE_TITLE( p_win_title );
    }
    else {
        SET_PAGE_TITLE(tr("GRUPLAR"));
    }

    if( p_help_page.isEmpty() EQ true ) {
        SET_HELP_PAGE( tr("gruplar") );
    }
    else {
        SET_HELP_PAGE( p_help_page );
    }
    SET_SETTING_NAME    ( tr("GRUPLAR") );

    INIT_GRUP_KERNEL( this , db );
}

/**************************************************************************************
                   GRUP_KERNEL::~GRUP_KERNEL
***************************************************************************************/

GRUP_KERNEL::~GRUP_KERNEL()
{

}

/**************************************************************************************
                   GRUP_KERNEL::INIT_GRUP_KERNEL
***************************************************************************************/

void GRUP_KERNEL::INIT_GRUP_KERNEL ( QWidget * , ADAK_SQL * db )
{
    m_db = db;

    INIT_KERNEL ( this, db );

    SET_ACTION_MENU_TEXT ( "Alt Grup Ekle", "Grubu Güncelle", "Grubu Sil" );

    m_ui->tree_widget->hideColumn ( m_grp_id_column );
    m_ui->tree_widget->hideColumn ( m_grp_parent_id_column );
    m_ui->tree_widget->hideColumn ( m_grp_son_seviye_mi_column );
    m_ui->tree_widget->hideColumn ( m_grp_tam_grup_kodu_column );

    m_ui->tree_widget->setColumnWidth ( m_grp_kodu_column               ,230 );
    m_ui->tree_widget->setColumnWidth ( m_grp_isim_column               ,350 );
    m_ui->tree_widget->setColumnWidth ( m_grp_alt_grup_sayisi_column    ,100 );
    m_ui->tree_widget->setColumnWidth ( m_grp_kayit_sayisi_column       ,150 );

    m_ui->tree_widget->header()->setStretchLastSection(false);

}

/**************************************************************************************
                   GRUP_KERNEL::ADD_ITEM
***************************************************************************************/

void GRUP_KERNEL::ADD_ITEM (QObject *button, QStringList column_datas )
{
    int item_id = -1;

    if ( button EQ m_ui->push_button_add_alt_grup OR button->metaObject()->className() EQ QAction::staticMetaObject.className()) {
        if (m_ui->tree_widget->topLevelItemCount() EQ 0) {
            MSG_INFO("Açılmış olan ana grup bulunmamaktadır.Önce ana grup açmalısınız." , NULL);
            return;
        }

        if (m_ui->tree_widget->currentItem() EQ NULL) {
            MSG_INFO("Alt grup ekleyebilmek için önce ana grup seçmelisiniz." , NULL);
            return;
        }

        if ( column_datas.size() EQ 0 ) {
            return;
        }

        item_id = column_datas.at( m_grp_id_column ).toInt();

        F_GRUP_KERNEL_GRUP_FISI ( item_id, ENUM_ALT_GRUP, static_cast< BASE_GRUP_KERNEL *>(this) ,this , m_db );

        if ( m_grup_satir_guncellendi_mi EQ true ) {
            RESET_GUI_UPDATE_VALUES();
        }
        REFRESH_TREE_WIDGET();
    }
    else {
        F_GRUP_KERNEL_GRUP_FISI ( item_id, ENUM_ANA_GRUP, static_cast< BASE_GRUP_KERNEL *>(this) ,this , m_db );

        if ( m_grup_satir_eklendi_mi EQ true ) {
             ADD_PARENT_ITEM ( m_grup_eklenen_kayit, 0 );
             RESET_GUI_UPDATE_VALUES();
        }
        REFRESH_TREE_WIDGET();
    }
}

/**************************************************************************************
                   GRUP_KERNEL::ADD_PARENT_ITEMS
***************************************************************************************/

void GRUP_KERNEL::ADD_PARENT_ITEMS()
{
    SQL_QUERY sql_query ( m_db );

    QString table_name  = "";
    QString column_name = "";
    QString where_name  = "";
    QString order_by    = "";

    CREATE_SELECT_QUERY(&table_name,&column_name,&where_name,&order_by);

    sql_query.PREPARE_SELECT ( table_name,column_name,where_name,"" );

    if ( sql_query.SELECT(order_by) NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
            //QString id = sql_query.VALUE(0).toString();
            QStringList tree_row_str;
            tree_row_str << sql_query.VALUE(3).toString() << sql_query.VALUE(4).toString() <<
                                        sql_query.VALUE(8).toString() << sql_query.VALUE(7).toString() <<
                                        sql_query.VALUE(0).toString() << sql_query.VALUE(1).toString() <<
                                        sql_query.VALUE(6).toString() << sql_query.VALUE(5).toString();

           ADD_PARENT_ITEM ( tree_row_str , sql_query.VALUE(8).toInt() );
       }
    }
}

/**************************************************************************************
                   GRUP_KERNEL::ADD_CHILD_ITEMS
***************************************************************************************/

void GRUP_KERNEL::ADD_CHILD_ITEMS (QStringList item)
{
    int item_id;

    item_id = item.at( m_grp_id_column ).toInt();

    SQL_QUERY sql_query ( m_db );

    QString table_name  = "";
    QString column_name = "";
    QString where_name  = "";
    QString order_by    = "";

    CREATE_SELECT_QUERY(&table_name,&column_name,&where_name,&order_by,item_id);

    sql_query.PREPARE_SELECT ( table_name,column_name,where_name,"" );

    if ( sql_query.SELECT(order_by) NE 0 ) {
        while ( sql_query.NEXT() EQ true ) {
           QStringList tree_row_str;
           tree_row_str << sql_query.VALUE(3).toString() << sql_query.VALUE(4).toString() <<
                          sql_query.VALUE(8).toString() << sql_query.VALUE(7).toString() <<
                          sql_query.VALUE(0).toString() << sql_query.VALUE(1).toString() <<
                          sql_query.VALUE(6).toString() << sql_query.VALUE(5).toString();


           ADD_CHILD_ITEM( tree_row_str , sql_query.VALUE(8).toInt() );
       }
    }
}

/**************************************************************************************
                   GRUP_KERNEL::UPDATE_ITEM
***************************************************************************************/

void GRUP_KERNEL::UPDATE_ITEM (QStringList item)
{
    int item_id = -1;

    if ( item.size() NE 0 ) {
        item_id = item.at( m_grp_id_column ).toInt();
    }

    if ( F_GRUP_KERNEL_GRUP_FISI ( item_id, ENUM_ANA_GRUP, static_cast< BASE_GRUP_KERNEL *>(this) ,this , m_db ) EQ false ) {
        return;
    }

    if ( m_grup_satir_guncellendi_mi EQ true ) {
         UPDATE_SELECTED_ITEM ( m_grup_guncellenen_ust_grup );
         RESET_GUI_UPDATE_VALUES();
    }
}

/**************************************************************************************
                   GRUP_KERNEL::CHECK_DELETE_ITEM
***************************************************************************************/

int GRUP_KERNEL::CHECK_DELETE_ITEM (QStringList item)
{
    int item_id = -1;

    if ( item.size() NE 0 ) {
        item_id = item.at( m_grp_id_column ).toInt();
    }

    if ( item_id EQ -1 ) {
        return ADAK_FAIL;
    }

    if ( item.at(2).toInt() NE 0 ) {
        MSG_ERROR(tr ( "Grubu silebilmek için,önce grubun alt gruplarını silmelisiniz!.."),NULL);
        return ADAK_FAIL;
    }

    ADAK_MSG_ENUM answer = MSG_YES_NO(tr ( "'%1 %2' grubu silinecek." ).arg(item.at(7)).arg(item.at(1)),NULL);

    if ( answer EQ ADAK_NO OR answer EQ ADAK_CANCEL) {
         return ADAK_FAIL;
    }

    SQL_QUERY select_query ( m_db );

    QString table_name  = "";
    QString column_name = "";
    QString where_name  = "";
    QString order_by    = "";

    CREATE_SELECT_QUERY(&table_name,&column_name,&where_name,&order_by,item.at( 5 ).toInt());

    select_query.PREPARE_SELECT ( table_name,column_name,where_name,"" );

    if ( select_query.SELECT(order_by) EQ 1 ) {
        if ( item.at(3).toInt() NE 0 ) {
            m_grubu_parentine_aktar = true;
        }

        return ADAK_OK;
    }

    if ( item.at( 3 ).toInt() NE 0  ) {
        MSG_ERROR(tr ( "Gruba kayıt yapılmış. Grubu silemezsiniz.." ),NULL);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   GRUP_KERNEL::DELETE_ITEM
***************************************************************************************/

void GRUP_KERNEL::DELETE_ITEM (QStringList item)
{
    int item_id = -1;

    if ( item.size() NE 0  ) {
        item_id = item.at( m_grp_id_column ).toInt();
    }

    if ( item_id EQ -1 ) {
        return;
    }

    m_db->START_TRANSACTION();

    SQL_QUERY sql_query  ( m_db );

    sql_query.PREPARE_SELECT ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME(),QString( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"=:grup_id" ),"");
    sql_query.SET_VALUE       ( ":grup_id", item_id );

    if ( sql_query.SELECT() NE 0 ) {
         sql_query.NEXT();
        if ( sql_query.VALUE(0).toInt() > 0 )  {
            int alt_grup_sayisi = 0;
            int parent_id = sql_query.VALUE(0).toInt();

            sql_query.PREPARE_SELECT ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),GRUP_KERNEL::GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME(),QString (  GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"=:grup_id" ),"");
            sql_query.SET_VALUE      ( ":grup_id",parent_id );
            if ( sql_query.SELECT() NE 0 ) {
                sql_query.NEXT();
                alt_grup_sayisi = sql_query.VALUE(0).toInt();
                alt_grup_sayisi--;
            }

            sql_query.PREPARE_UPDATE ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME(),GRUP_KERNEL::GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME(),QString ( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"=:grup_id" ));
            sql_query.SET_VALUE      ( ":"+GRUP_KERNEL::GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME(), alt_grup_sayisi );
            sql_query.SET_VALUE      ( ":grup_id"         , parent_id );
            sql_query.UPDATE();
        }
    }

    sql_query.PREPARE_SELECT ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),QString(GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+","+GRUP_KERNEL::GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()),
                               QString ( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"=:grup_id"),"");
    sql_query.SET_VALUE      ( ":grup_id",item_id );

    if ( sql_query.SELECT() EQ 0 ) {
        m_db->CANCEL_TRANSACTION();
        MSG_ERROR(tr ( "Grubun üst grubu bilgisi alınmaya çalışırken hata oluştu."),NULL);
        return;
    }

    sql_query.NEXT();

    int selected_grup_parent_id  = sql_query.VALUE(0).toInt();
    int grup_uye_sayisi          = sql_query.VALUE(1).toInt();

    if ( m_grubu_parentine_aktar EQ false ) {
         m_grubu_parentine_aktar = false;
         sql_query.PREPARE_DELETE ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),QString (GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+ "=:grup_id "));
        sql_query.SET_VALUE      ( ":grup_id", item_id );
        sql_query.DELETE();

        if ( selected_grup_parent_id > 0 ) {
            sql_query.PREPARE_SELECT ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME(),QString ( GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"=:grup_parent_id"),"");
            sql_query.SET_VALUE      ( ":grup_parent_id", selected_grup_parent_id);

            if ( sql_query.SELECT() EQ 0 ) {
                sql_query.PREPARE_UPDATE ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME(),GRUP_KERNEL::GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME(),QString ( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));
                sql_query.SET_VALUE      ( ":"+GRUP_KERNEL::GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME(), 1);
                sql_query.SET_VALUE      ( ":grup_id", selected_grup_parent_id);
                sql_query.UPDATE();
            }
        }        
    }
    else {
        QString columns = QString("%1,%2,%3,%4,%5,%6").arg ( GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME() )
                                                      .arg ( GRUP_KERNEL::GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME() )
                                                      .arg ( GRUP_KERNEL::GET_GRUPLAR_GRUP_KODU_COLUMN_NAME() )
                                                      .arg ( GRUP_KERNEL::GET_GRUPLAR_GRUP_ADI_COLUMN_NAME() )
                                                      .arg ( GRUP_KERNEL::GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME() )
                                                      .arg ( GRUP_KERNEL::GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME() );

        sql_query.PREPARE_SELECT ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),columns,QString ( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id" ),"");
        sql_query.SET_VALUE      ( ":grup_id", selected_grup_parent_id );

        if ( sql_query.SELECT() > 0 ) {
            sql_query.NEXT();
            int     grup_parent_id  = sql_query.VALUE(0).toInt();
            int     grup_seviyesi   = sql_query.VALUE(1).toInt();
            QString grup_kodu       = sql_query.VALUE(2).toString();
            QString grup_adi        = sql_query.VALUE(3).toString();
            QString tam_grup_kodu   = sql_query.VALUE(4).toString();

            SQL_QUERY update_query ( m_db );

            columns = QString("%1,%2,%3,%4,%5,%6,%7").arg ( GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                                     .arg ( GRUP_KERNEL::GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME())
                                                     .arg ( GRUP_KERNEL::GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                                     .arg ( GRUP_KERNEL::GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                                     .arg ( GRUP_KERNEL::GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                                     .arg ( GRUP_KERNEL::GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                                     .arg ( GRUP_KERNEL::GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME());

            update_query.PREPARE_UPDATE ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME(),columns,QString ( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));

            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME()          , grup_parent_id                );
            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()      , grup_seviyesi                 );
            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()          , grup_kodu                     );
            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()           , grup_adi                      );
            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()      , tam_grup_kodu                 );
            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()         , grup_uye_sayisi               );
            update_query.SET_VALUE    ( ":"+GRUP_KERNEL::GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()       , 1                             );
            update_query.SET_VALUE    ( ":grup_id"                                                    , item_id                       );

            update_query.UPDATE();

            sql_query.PREPARE_DELETE ( GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME(),QString ( GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()+"=:grup_id "));
            sql_query.SET_VALUE      ( ":grup_id", selected_grup_parent_id );
            sql_query.DELETE();            
        }
    }

    m_db->COMMIT_TRANSACTION();

    ADD_VALUE_TO_DELETED_ITEM_PARENT(m_grp_alt_grup_sayisi_column,-1);
}
