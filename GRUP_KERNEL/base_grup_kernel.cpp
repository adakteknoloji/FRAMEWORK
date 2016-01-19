#include <stdarg.h>
#include <QApplication>
#include <QVariant>
#include <QStringList>
#include "adak_utils.h"
#include "base_grup_kernel.h"

/**************************************************************************************
                   BASE_GRUP_KERNEL::BASE_GRUP_KERNEL
***************************************************************************************/

BASE_GRUP_KERNEL::BASE_GRUP_KERNEL ( )
{
    GRP_RESET_STATIC_VARIABLES();
    LOAD_DEFAULT_DB_INFO      ();
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::~BASE_GRUP_KERNEL
***************************************************************************************/

BASE_GRUP_KERNEL::~BASE_GRUP_KERNEL()
{

}

/**************************************************************************************
                   BASE_GRUP_KERNEL::LOAD_DEFAULT_DB_INFO
***************************************************************************************/

void BASE_GRUP_KERNEL::LOAD_DEFAULT_DB_INFO()
{
    M_GRUPLAR_DB_INFO.gruplar_tablo_ismi                     = "adak_gruplar";
    M_GRUPLAR_DB_INFO.grup_id_column_name                    = "grup_id";
    M_GRUPLAR_DB_INFO.grup_parent_id_column_name             = "grup_parent_id";
    M_GRUPLAR_DB_INFO.grup_seviyesi_column_name              = "grup_seviyesi";
    M_GRUPLAR_DB_INFO.grup_kodu_column_name                  = "grup_kodu";
    M_GRUPLAR_DB_INFO.grup_adi_column_name                   = "grup_adi";
    M_GRUPLAR_DB_INFO.tam_grup_kodu_column_name              = "tam_grup_kodu";
    M_GRUPLAR_DB_INFO.grup_son_seviyemi_column_name          = "grup_son_seviye_mi";
    M_GRUPLAR_DB_INFO.grupta_kayitli_uye_sayisi_column_name  = "gruptaki_kayit_sayisi";
    M_GRUPLAR_DB_INFO.alt_grup_sayisi_column_name            = "alt_grup_sayisi";
    M_GRUPLAR_DB_INFO.program_id_column_name                 = "program_id";
    M_GRUPLAR_DB_INFO.modul_id_column_name                   = "modul_id";
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::CREATE_SELECT_QUERY
***************************************************************************************/

void BASE_GRUP_KERNEL::CREATE_SELECT_QUERY ( QString * table_name , QString * column_name , QString *  where_str , QString * order_by,int parent_id,int item_id )
{
    if ( M_GRUPLAR_DB_INFO.gruplar_tablo_ismi.isEmpty() EQ true ) {
        LOAD_DEFAULT_DB_INFO();
    }

    QString item_where_str   = "";
    QString parent_where_str = "";
    QString filter_where_str = "";

    if ( parent_id NE -1 ) {
         parent_where_str    = QString ( "grup_parent_id = %1" ).arg ( QVariant(parent_id).toString() );
    }

    if ( item_id NE -1 ) {
         item_where_str   = QString("grup_id = %1").arg ( QVariant ( item_id ).toString() );
        if ( parent_where_str.isEmpty() EQ true ) {
             item_where_str.prepend ( " " );
        }
        else {
            item_where_str.prepend ( "AND " );
        }
    }
    if ( m_visible_modules.size() > 0  ) {
        if ( parent_where_str.isEmpty() EQ true AND item_where_str.isEmpty() EQ true ) {
             filter_where_str.append ( " ( " );
        }
        else {
            filter_where_str.append ( "AND ( " );
        }
        for ( int i = 0 ; i < m_visible_modules.size() ; i++ ) {
             filter_where_str.append ( ( QString ( "%1 = %2" )
                                     .arg ( BASE_GRUP_KERNEL::GET_GRUPLAR_MODUL_ID_COLUMN_NAME())
                                     .arg ( QVariant ( m_visible_modules.at(i)).toString())));

            if ( ( i+1 ) NE m_visible_modules.size() ) {
                filter_where_str.append ( " OR " );
            }
        }
        filter_where_str.append( QString(" ) AND %1 = %2 ")
                                 .arg(BASE_GRUP_KERNEL::GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME())
                                 .arg(m_program_id));
    }
    else {
        ADAK_WARNING (qApp->QObject::tr("Entries are shown the desired program and the module ID must be set."), NULL, NULL);
        //Gösterilmesi istenen kayitların program ve modul idsini setlenmesi gerekir.
        return;
    }

    *column_name = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10").arg ( M_GRUPLAR_DB_INFO.grup_id_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.grup_parent_id_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.grup_seviyesi_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.grup_kodu_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.grup_adi_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.tam_grup_kodu_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.grup_son_seviyemi_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.grupta_kayitli_uye_sayisi_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.alt_grup_sayisi_column_name )
                                                           .arg ( M_GRUPLAR_DB_INFO.modul_id_column_name );

    *table_name = QString ("%1").arg(M_GRUPLAR_DB_INFO.gruplar_tablo_ismi);

    *where_str  = QString("%1 %2 %3").arg ( parent_where_str )
                                    .arg ( item_where_str)
                                    .arg ( filter_where_str );

    *order_by   = M_GRUPLAR_DB_INFO.grup_kodu_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_TABLE_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_TABLE_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.gruplar_tablo_ismi                     = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::BASE_GRUP_KERNEL::SET_GRUPLAR_ID_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_ID_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grup_id_column_name                    = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_PARENT_ID_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_PARENT_ID_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grup_parent_id_column_name             = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grup_seviyesi_column_name              = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_KODU_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_KODU_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grup_kodu_column_name                  = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_ADI_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_ADI_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grup_adi_column_name                   = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.tam_grup_kodu_column_name              = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grup_son_seviyemi_column_name          = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_UYE_SAYISI_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_UYE_SAYISI_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.grupta_kayitli_uye_sayisi_column_name  = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.alt_grup_sayisi_column_name = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_PROGRAM_ID_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_PROGRAM_ID_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.program_id_column_name  = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_GRUPLAR_MODUL_ID_COLUMN_NAME
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_GRUPLAR_MODUL_ID_COLUMN_NAME ( QString column_name )
{
    M_GRUPLAR_DB_INFO.modul_id_column_name = column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_TABLE_NAME()
{
    return M_GRUPLAR_DB_INFO.gruplar_tablo_ismi;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_ID_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grup_id_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_PARENT_ID_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grup_parent_id_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grup_seviyesi_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_KODU_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grup_kodu_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_ADI_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grup_adi_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.tam_grup_kodu_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grup_son_seviyemi_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.grupta_kayitli_uye_sayisi_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.alt_grup_sayisi_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.program_id_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_GRUPLAR_MODUL_ID_COLUMN_NAME
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_GRUPLAR_MODUL_ID_COLUMN_NAME()
{
    return M_GRUPLAR_DB_INFO.modul_id_column_name;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::UPDATE_GRUP_GUI_EKLENEN_KAYIT_SATIR
***************************************************************************************/

void BASE_GRUP_KERNEL::UPDATE_GRUP_GUI_EKLENEN_KAYIT_SATIR ( QStringList row_str )
{
    m_grup_satir_eklendi_mi = true;
    m_grup_eklenen_kayit = row_str;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::UPDATE_GRUP_GUI_GUNCELLENEN_SATIR
***************************************************************************************/

void BASE_GRUP_KERNEL::UPDATE_GRUP_GUI_GUNCELLENEN_SATIR ( QStringList row_str )
{
    m_grup_satir_guncellendi_mi =  true;
    m_grup_guncellenen_satir = row_str;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP
***************************************************************************************/

void BASE_GRUP_KERNEL::UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP ( QStringList row_str )
{
    m_grup_satir_guncellendi_mi =  true;
    m_grup_guncellenen_ust_grup = row_str;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::RESET_GUI_UPDATE_VALUES
***************************************************************************************/

void BASE_GRUP_KERNEL::RESET_GUI_UPDATE_VALUES()
{
    m_grup_eklenen_kayit.clear();
    m_grup_guncellenen_satir.clear();
    m_grup_guncellenen_ust_grup.clear();
    m_grup_satir_eklendi_mi     = false;
    m_grup_satir_guncellendi_mi = false;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::SET_DELIMETER
***************************************************************************************/

void BASE_GRUP_KERNEL::SET_DELIMETER ( QString delimeter )
{
    m_delimeter = delimeter;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GET_DELIMETER
***************************************************************************************/

QString BASE_GRUP_KERNEL::GET_DELIMETER()
{
    return m_delimeter;
}

///**************************************************************************************
//                   BASE_GRUP_KERNEL::SET_MODUL_ID
//***************************************************************************************/

//void BASE_GRUP_KERNEL::GRP_SET_MODUL_ID ( int modul_id )
//{
//    m_modul_id = modul_id;
//}

/**************************************************************************************
                   GRP_GET_MODUL_ID
***************************************************************************************/

int BASE_GRUP_KERNEL::GRP_GET_MODUL_ID()
{
    if (m_visible_modules.isEmpty() EQ true) {
        return -1;
    }

    return m_visible_modules.last();
}

/**************************************************************************************
                   GRP_GET_PROGRAM_ID
***************************************************************************************/

int BASE_GRUP_KERNEL::GRP_GET_PROGRAM_ID()
{
    return m_program_id;
}

/**************************************************************************************
                   BASE_GRUP_KERNEL::GRP_SET_VISIBLE_PROG_MODULE
***************************************************************************************/

void BASE_GRUP_KERNEL::GRP_SET_VISIBLE_PROG_MODULE(int program_id, int module_id)
{
    m_visible_modules << module_id;
    m_program_id      = program_id;
}

/**************************************************************************************
                   GRP_RESET_STATIC_VARIABLES
***************************************************************************************/

void BASE_GRUP_KERNEL::GRP_RESET_STATIC_VARIABLES()
{
    m_visible_modules.clear();
    m_program_id = -1;
}
