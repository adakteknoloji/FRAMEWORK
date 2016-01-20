#include "tasarim_export.h"

extern ADAK_SQL * G_YONETIM_DB;


/***********************************************************************************
                            OPEN_TASARIM_EXPORT
************************************************************************************/

void OPEN_TASARIM_EXPORT( int p_tasarim_id, QWidget * parent )
{
    TASARIM_EXPORT *W = new TASARIM_EXPORT(p_tasarim_id, parent );
    W->START_EXPORT();
}

/***********************************************************************************
                            TASARIM_EXPORT::TASARIM_EXPORT
************************************************************************************/

TASARIM_EXPORT::TASARIM_EXPORT( int p_tasarim_id, QWidget * parent ): EXPORT_KERNEL( "", parent )
{
    m_tasarim_id = p_tasarim_id;
}

/***********************************************************************************
                            TASARIM_EXPORT::EXPORT_ROWS
************************************************************************************/

void TASARIM_EXPORT::EXPORT_ROWS()
{
    QStringList row_title;
    QStringList row_values;

    SQL_QUERY query(G_YONETIM_DB);

    row_title << QObject::tr("Id");
    row_title << QObject::tr("Font Size");
    row_title << QObject::tr("Align");
    row_title << QObject::tr("PX");
    row_title << QObject::tr("PY");
    row_title << QObject::tr("Vertical Size");
    row_title << QObject::tr("Horizontal Size");
    row_title << QObject::tr("Text");
    row_title << QObject::tr("Text Size");
    row_title << QObject::tr("Visible");
    row_title << QObject::tr("Group");
    row_title << QObject::tr("Is Monospace");
    row_title << QObject::tr("Font");
    row_title << QObject::tr("Bold");
    row_title << QObject::tr("Under Line");
    row_title << QObject::tr("İtalic");
    row_title << QObject::tr("Strikeout");
    row_title << QObject::tr("Row");
    row_title << QObject::tr("Left Space");

    SET_ROW_TITLE(row_title);

    query.PREPARE_SELECT("ynt_belge_degiskenleri",
                         "degisken_id, font_size, align, pos_x, pos_y, "
                         "size_vertical, size_horizontal, text, text_size, is_visible_variable, "
                         "grup_enum, monospace_mi, font_family, is_bold, is_under_line, is_italic, is_strikeout, "
                         "satir, soldan_bosluk",
                         "tasarim_id = :tasarim_id");

    query.SET_VALUE     ( ":tasarim_id", m_tasarim_id );

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while( query.NEXT() EQ true ) {

        row_values.clear();

        row_values << query.VALUE(0).toString();
        row_values << query.VALUE(1).toString();
        row_values << query.VALUE(2).toString();
        row_values << query.VALUE(3).toString();
        row_values << query.VALUE(4).toString();
        row_values << query.VALUE(5).toString();
        row_values << query.VALUE(6).toString();
        row_values << query.VALUE(7).toString();
        row_values << query.VALUE(8).toString();
        row_values << query.VALUE(9).toString();
        row_values << query.VALUE(10).toString();
        row_values << query.VALUE(11).toString();
        row_values << query.VALUE(12).toString();
        row_values << query.VALUE(13).toString();
        row_values << query.VALUE(14).toString();
        row_values << query.VALUE(15).toString();
        row_values << query.VALUE(16).toString();
        row_values << query.VALUE(17).toString();
        row_values << query.VALUE(18).toString();

        ADD_ROW(row_values);
    }

}
