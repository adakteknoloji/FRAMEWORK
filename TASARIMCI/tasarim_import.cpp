#include "tasarim_import.h"

extern ADAK_SQL * G_YONETIM_DB;

void OPEN_TASARIM_IMPORT(int p_tasarim_id, QWidget *parent)
{
    TASARIM_IMPORT *W = new TASARIM_IMPORT( p_tasarim_id, parent );
    W->SHOW(NOT_FULL_SCREEN);
}

/***********************************************************************************
                            TASARIM_IMPORT::TASARIM_IMPORT
************************************************************************************/

TASARIM_IMPORT::TASARIM_IMPORT( int p_tasarim_id, QWidget * parent) : IMPORT_KERNEL( "", parent)
{
    m_tasarim_id = p_tasarim_id;


    QList<QStringList> baslik_ve_kontrol;

    baslik_ve_kontrol.append( QStringList() <<  "Id"                << "" << "*");
    baslik_ve_kontrol.append( QStringList() <<  "Font Size"         << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Align"             << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "PX"                << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "PY"                << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Vertical Size"     << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Horizontal Size"   << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Text"              << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Text Size"         << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Visible"           << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Grup"              << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Monospace mi"      << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Font"              << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Bold"              << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Under Line"        << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "İtalic"            << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Strikeout"         << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Satır"             << "" << "");
    baslik_ve_kontrol.append( QStringList() <<  "Soldan Boşluk"     << "" << "");

    SET_HEADERS    ( baslik_ve_kontrol );

    SET_PROGRAM_ID ( E9_PROGRAMI   );
    SET_MODUL_ID   ( -1 );

    SET_DB         (G_YONETIM_DB);
}

/***********************************************************************************
                            TASARIM_IMPORT::IMPORT_ROW
************************************************************************************/

void TASARIM_IMPORT::IMPORT_ROW(QStringList row)
{
    SQL_QUERY query( G_YONETIM_DB );
    query.PREPARE_INSERT("ynt_belge_degiskenleri", "belge_degisken_id",
                         "tasarim_id, degisken_id, font_size, align, pos_x, pos_y, "
                         "size_vertical, size_horizontal, text, text_size, is_visible_variable, "
                         "grup_enum, monospace_mi, font_family, is_bold, is_under_line, is_italic, is_strikeout, satir, bosluk_sayisi");

    query.SET_VALUE     ( ":tasarim_id",            m_tasarim_id );
    query.SET_VALUE     ( ":degisken_id",           row.at(0).toInt() );
    query.SET_VALUE     ( ":font_size",             row.at(1).toInt() );
    query.SET_VALUE     ( ":align",                 row.at(2).toInt() );
    query.SET_VALUE     ( ":pos_x",                 row.at(3).toDouble());
    query.SET_VALUE     ( ":pos_y",                 row.at(4).toDouble() );
    query.SET_VALUE     ( ":size_vertical",         row.at(5).toInt() );
    query.SET_VALUE     ( ":size_horizontal",       row.at(6).toInt() );
    query.SET_VALUE     ( ":text",                  row.at(7) );
    query.SET_VALUE     ( ":text_size",             row.at(8).toInt() );
    query.SET_VALUE     ( ":is_visible_variable",   row.at(9).toInt() );
    query.SET_VALUE     ( ":grup_enum",             row.at(10).toInt() );
    query.SET_VALUE     ( ":monospace_mi",          row.at(11).toInt() );
    query.SET_VALUE     ( ":font_family",           row.at(12) );
    query.SET_VALUE     ( ":is_bold",               row.at(13).toInt() );
    query.SET_VALUE     ( ":is_under_line",         row.at(14).toInt() );
    query.SET_VALUE     ( ":is_italic",             row.at(15).toInt() );
    query.SET_VALUE     ( ":is_strikeout",          row.at(16).toInt() );
    query.SET_VALUE     ( ":satir",                 row.at(17).toInt() );
    query.SET_VALUE     ( ":bosluk_sayisi",         row.at(18).toInt() );

    query.INSERT();
}

