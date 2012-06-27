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

    row_title << "Id";
    row_title << "Font Size";
    row_title << "Align";
    row_title << "PX";
    row_title << "PY";
    row_title << "Vertical Size";
    row_title << "Horizontal Size";
    row_title << "Text";
    row_title << "Text Size";
    row_title << "Visible";
    row_title << "Grup";
    row_title << "Monospace mi";
    row_title << "Font";
    row_title << "Bold";
    row_title << "Under Line";
    row_title << "İtalic";
    row_title << "Strikeout";
    row_title << "Satır";
    row_title << "Soldan Boşluk";

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
