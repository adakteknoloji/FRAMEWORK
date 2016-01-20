#include <QLabel>
#include <QFile>
#include <QTextStream>
#include "adak_utils.h"
#include "adak_sql.h"
#include "tasarim_utils.h"
#include "belge_struct.h"
#include "tasarim_degiskeni.h"
#include "belge_kernel_utils.h"
#include "belge_kernel_defines.h"


extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   FILL_DEGISKEN_GRUBU_COMBOBOX
***************************************************************************************/

void FILL_DEGISKEN_GRUBU_COMBOBOX (QComboBox * combo_box)
{
    combo_box->addItems(QStringList() << "Header" << "Line" << "Footer" );
}


/**************************************************************************************
                   FILL_ALIGN_COMBOBOX
***************************************************************************************/

void FILL_ALIGN_COMBOBOX (QComboBox * combo_box)
{
    combo_box->addItems(QStringList() << QObject::tr("Align Left") << QObject::tr("Centered") << QObject::tr("Align Right") );
}

/**************************************************************************************
                   GET_SAVED_VARIABLE_INFO                      
***************************************************************************************/

void GET_SAVED_VARIABLE_INFO ( int tasarim_id )
{
    //! DB KAYITLI DEGISKENLERI AYRI AYRI OLARAK

    SQL_QUERY query(G_YONETIM_DB);

    int header_struct_size = GET_BELGE_HEADER_STRUCT_SIZE();
    ADAK_BELGE_DEFAULTS_STRUCT * header_orj = GET_BELGE_HEADER_STRUCT();

    int footer_struct_size = GET_BELGE_FOOTER_STRUCT_SIZE();
    ADAK_BELGE_DEFAULTS_STRUCT * footer_orj = GET_BELGE_FOOTER_STRUCT();

    int line_struct_size   = GET_BELGE_LINE_STRUCT_SIZE();
    ADAK_BELGE_DEFAULTS_STRUCT * line_orj   = GET_BELGE_LINE_STRUCT();


    query.PREPARE_SELECT("ynt_belge_degiskenleri",
                         "belge_degisken_id, degisken_id, font_size, align, pos_x, pos_y, is_visible_variable, "
                         "grup_enum, monospace_mi, text_size","tasarim_id =:tasarim_id AND degisken_id != -1 ","");

    query.SET_VALUE(":tasarim_id",tasarim_id);

    if ( query.SELECT() EQ 0 ) {
        for ( int i = 0 ; i < header_struct_size ; i++ )  {
            header_orj[i].is_visible_variable   = 0;
            header_orj[i].x_position            = 0;
            header_orj[i].y_position            = 0;
        }
        for ( int i = 0 ; i < footer_struct_size ; i++ )  {
            footer_orj[i].is_visible_variable   = 0;
            footer_orj[i].x_position            = 0;
            footer_orj[i].y_position            = 0;
        }
        for ( int i = 0 ; i < line_struct_size ; i++ )  {
            line_orj[i].is_visible_variable     = 0;
            line_orj[i].x_position              = 0;
            line_orj[i].y_position              = 0;
        }
        return;
    }
    while( query.NEXT() EQ true ) {
        switch(query.VALUE(7).toInt()){
            case HEADER:
                for ( int i = 0 ; i < header_struct_size ; i++ )  {
                    if (header_orj[i].degisken_id EQ query.VALUE(1).toInt()) {
                        header_orj[i].align                 = query.VALUE(3).toInt();
                        header_orj[i].font_size             = query.VALUE(2).toInt();
                        header_orj[i].is_visible_variable   = query.VALUE(6).toInt();
                        header_orj[i].x_position            = query.VALUE(4).toDouble();
                        header_orj[i].y_position            = query.VALUE(5).toDouble();
                        header_orj[i].monospace_mi          = query.VALUE(8).toDouble();
                        header_orj[i].karakter_sayisi       = query.VALUE(9).toDouble();
                        break;
                    }
                }
                break;
            case FOOTER:
                for ( int i = 0 ; i < footer_struct_size ; i++ )  {
                    if (footer_orj[i].degisken_id EQ query.VALUE(1).toInt()) {
                        footer_orj[i].align                 = query.VALUE(3).toInt();
                        footer_orj[i].font_size             = query.VALUE(2).toInt();
                        footer_orj[i].is_visible_variable   = query.VALUE(6).toInt();
                        footer_orj[i].x_position            = query.VALUE(4).toDouble();
                        footer_orj[i].y_position            = query.VALUE(5).toDouble();
                        footer_orj[i].monospace_mi          = query.VALUE(8).toDouble();
                        footer_orj[i].karakter_sayisi       = query.VALUE(9).toDouble();
                        break;
                    }
                }
                break;
            case LINE:
                for ( int i = 0 ; i < line_struct_size ; i++ )  {
                    if (line_orj[i].degisken_id EQ query.VALUE(1).toInt()) {
                        line_orj[i].align                 = query.VALUE(3).toInt();
                        line_orj[i].font_size             = query.VALUE(2).toInt();
                        line_orj[i].is_visible_variable   = query.VALUE(6).toInt();
                        line_orj[i].x_position            = query.VALUE(4).toDouble();
                        line_orj[i].y_position            = query.VALUE(5).toDouble();
                        line_orj[i].monospace_mi          = query.VALUE(8).toDouble();
                        line_orj[i].karakter_sayisi       = query.VALUE(9).toDouble();
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }
}

/**************************************************************************************
                   GET_BELGE_GRUP_STR
***************************************************************************************/

QString GET_BELGE_GRUP_STR (int belge_grup_enum)
{
    switch(belge_grup_enum) {
        case HEADER:
            return "Header";
        case LINE:
            return "Line";
        case FOOTER:
            return "Footer";
        default:
            break;
    }

    return "";
}

/**************************************************************************************
                   GET_BELGE_GRUP_ENUM
***************************************************************************************/

int GET_BELGE_GRUP_ENUM (QString belge_grup_str)
{
    if (belge_grup_str EQ "Header") {
        return HEADER;
    }

    if (belge_grup_str EQ "Line") {
        return LINE;
    }

    if (belge_grup_str EQ "Footer") {
        return FOOTER;
    }

    return -1;
}

/**************************************************************************************
                   GET_BELGE_ALIGN_STR
***************************************************************************************/

QString GET_BELGE_ALIGN_STR (int belge_align_enum)
{
    switch(belge_align_enum) {
        case LEFT_ALIGN:
            return QObject::tr("Align Left");
        case RIGHT_ALIGN:
            return QObject::tr("Align Right");
        case CENTER_ALIGN:
            return QObject::tr("Centered");
        default:
            break;
    }

    return "";
}

/**************************************************************************************
                   GET_BELGE_ALIGN_ENUM
***************************************************************************************/

int GET_BELGE_ALIGN_ENUM (QString belge_align_str)
{
    if (belge_align_str EQ QObject::tr("Align Left")) {
        return LEFT_ALIGN;
    }

    if (belge_align_str EQ QObject::tr("Align Right")) {
        return RIGHT_ALIGN;
    }

    if (belge_align_str EQ QObject::tr("Centered")) {
        return CENTER_ALIGN;
    }

    return -1;
}

/**************************************************************************************
                   FILL_PRINTER_TYPE_COMBOBOX
***************************************************************************************/

void FILL_PRINTER_TYPE_COMBOBOX (QComboBox * combo_box)
{
    //Burdaki degisikler GET_PRINTER_TYPE_STR ve GET_PRINTER_TYPE_ENUM fonksiyonlarinada
    //yansitilmali
    combo_box->addItems(QStringList() << QObject::tr("Laser Printers")
                                      << QObject::tr("Dot Matrix Printers") );
    combo_box->setCurrentIndex(0);
}

/**************************************************************************************
                   GET_PRINTER_TYPE_ENUM
***************************************************************************************/

int GET_PRINTER_TYPE_ENUM (QString printer_type_str)
{
    if (printer_type_str EQ QObject::tr("Laser Printers") ) {
        return LAZER_PRINTERS;
    }
    if (printer_type_str EQ QObject::tr("Dot Matrix Printers" )) {
        return DOT_MATRIX_PRINTER;
    }
    return -1;
}

/**************************************************************************************
                   GET_PRINTER_TYPE_STR
***************************************************************************************/

QString GET_PRINTER_TYPE_STR (int printer_type_enum)
{
    switch (printer_type_enum) {
        case LAZER_PRINTERS:
            return QObject::tr("Laser Printers");
        case DOT_MATRIX_PRINTER:
            return QObject::tr("Dot Matrix Printers" );
        default:
            return "";
    }
}

/**************************************************************************************
                   FILL_DOTMATRIX_FONT_COMBOBOX
***************************************************************************************/

void FILL_DOTMATRIX_FONT_COMBOBOX (QComboBox * combo_box)
{
    combo_box->addItems(QStringList() << QObject::tr("Small Font" )<< QObject::tr("Normal Font" ) << QObject::tr("Big Font") );
    combo_box->setCurrentIndex(1);
}

/**************************************************************************************
                   GET_DOTMATRIX_FONT_ENUM
***************************************************************************************/

int GET_DOTMATRIX_FONT_ENUM(QString font_type_str)
{
    if (font_type_str EQ QObject::tr("Small Font" ) ) {
        return DOTMATRIX_CONDENSED_FONT;
    }
    else if (font_type_str EQ QObject::tr("Normal Font" )) {
        return DOTMATRIX_NORMAL_FONT;
    }
    else if (font_type_str EQ QObject::tr("Big Font") ) {
        return DOTMATRIX_LARGE_FONT;
    }
    return -1;
}

/**************************************************************************************
                   GET_DOTMATRIX_FONT_STR
***************************************************************************************/

QString GET_DOTMATRIX_FONT_STR (int font_type_enum)
{
    switch (font_type_enum) {
        case DOTMATRIX_CONDENSED_FONT:
            return QObject::tr("Small Font");
        case DOTMATRIX_NORMAL_FONT:
            return QObject::tr("Normal Font");
        case DOTMATRIX_LARGE_FONT:
            return QObject::tr("Big Font");
         default:
            return "";
    }
}

/**************************************************************************************
                   CREATE_CPP_CODE_SAVED_DESIGNS
***************************************************************************************/

void CREATE_CPP_CODE_SAVED_DESIGNS ( void )
{
    SQL_QUERY f_query (G_YONETIM_DB);
    SQL_QUERY s_query (G_YONETIM_DB);

    f_query.PREPARE_SELECT("ynt_belge_tasarimlari",
                           "tasarim_id, belge_id, tasarim_adi, html_str,       "
                           "belge_width, belge_height,row_count, row_space_mm, "
                           "row_space_satir, belge_satir_sayisi,               "
                           "belge_toplam_karakter_sayisi, kopya_sayisi,        "
                           "kopya_konumu, printer_type, line_headers_visible", "", "" );

    if ( f_query.SELECT() EQ 0 ) {
        return;
    }

    QString belgeler_code_str    = "BELGELER_STRUCT belgeler[] = {\n";
    QString degiskenler_code_str = "BELGE_DEGISKENLERI_STRUCT belgeler_degiskenleri[] = {\n";

    int eklenecek_belge_sayisi = 0;
    bool degiskenler_eklendi = false;

    while ( f_query.NEXT() EQ true ) {
        /////////////////////////////// DEGISKENLER

        s_query.PREPARE_SELECT("ynt_belge_degiskenleri",
                               "degisken_id,font_size,align,pos_x,pos_y,size_vertical, "
                               "size_horizontal,text,is_visible_variable,grup_enum, monospace_mi, "
                               "text_size, font_family, is_bold, is_under_line, is_italic, is_strikeout, satir, soldan_bosluk ",
                               "tasarim_id = :tasarim_id","");

        s_query.SET_VALUE(":tasarim_id" , f_query.VALUE("tasarim_id").toString() );

        if ( s_query.SELECT()  EQ 0) {
            continue;
        }

        int tasarim_sayisi = 0;

        while ( s_query.NEXT() EQ true ) {
            if ( tasarim_sayisi NE 0 OR degiskenler_eklendi EQ true) {
                degiskenler_code_str.append(",\n");
            }

            degiskenler_code_str.append("\t{ ");
            degiskenler_code_str.append(QString ("\"%1\",\"%2\",\"%3\",\"%4\",%5,"
                                                 "\"%6\",\"%7\",\"%8\",\"%9\",\"%10\","
                                                 "%11, %12, %13, \"%14\", %15, %16, %17, %18, %19, %20")
                                        .arg(s_query.VALUE("degisken_id").toString())
                                        .arg(s_query.VALUE("font_size").toString())
                                        .arg(s_query.VALUE("align").toString())
                                        .arg(s_query.VALUE("is_visible_variable").toString())
                                        .arg(s_query.VALUE("grup_enum").toString())
                                        .arg(s_query.VALUE("size_vertical").toString())
                                        .arg(s_query.VALUE("size_horizontal").toString())
                                        .arg(s_query.VALUE("pos_x").toString())
                                        .arg(s_query.VALUE("pos_y").toString())
                                        .arg(s_query.VALUE("text").toString())
                                        .arg(QVariant(eklenecek_belge_sayisi).toString())
                                        .arg(s_query.VALUE("monospace_mi").toString())
                                        .arg(s_query.VALUE("text_size").toString())
                                        .arg(s_query.VALUE("font_family").toString())
                                        .arg(s_query.VALUE("is_bold").toInt())
                                        .arg(s_query.VALUE("is_under_line").toInt())
                                        .arg(s_query.VALUE("is_italic").toInt())
                                        .arg(s_query.VALUE("is_strikeout").toInt())
                                        .arg(s_query.VALUE("satir").toInt())
                                        .arg(s_query.VALUE("soldan_bosluk").toInt())

                                        );

            degiskenler_code_str.append(" }");
            degiskenler_eklendi = true;
            tasarim_sayisi++;
        }

        ////////////////////////////////////////////

        if ( eklenecek_belge_sayisi NE 0) {
            belgeler_code_str.append(",\n");
        }

        belgeler_code_str.append("\t{ ");
        belgeler_code_str.append(QString ("\"%1\",\"%2\",\"%3\",\"%4\",\"%6\",\"%8\",\"%9\",\"%10\",\""
                                          "%11\",%12,\"%13\",\"%14\",\"%15\",\"%16\",\"%17\"")
                                    .arg(f_query.VALUE("belge_id").toString())
                                    .arg(f_query.VALUE("row_count").toString())
                                    .arg(f_query.VALUE("row_space_satir").toString())
                                    .arg(f_query.VALUE("belge_satir_sayisi").toString())
                                    .arg(f_query.VALUE("belge_toplam_karakter_sayisi").toString())
                                    .arg(f_query.VALUE("kopya_sayisi").toString())
                                    .arg(f_query.VALUE("kopya_konumu").toString())
                                    .arg(f_query.VALUE("printer_type").toString())
                                    .arg(f_query.VALUE("line_headers_visible").toString())
                                    .arg(QVariant(tasarim_sayisi).toString())
                                    .arg(f_query.VALUE("belge_width").toString())
                                    .arg(f_query.VALUE("belge_height").toString())
                                    .arg(f_query.VALUE("row_space_mm").toString())
                                    .arg(f_query.VALUE("tasarim_adi").toString())
                                    .arg(f_query.VALUE("html_str").toString()) );

        belgeler_code_str.append(" }");

        eklenecek_belge_sayisi++;
    }

    belgeler_code_str.append(",\n{ \"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",-1,\"\",\"\",\"\",\"\",\"\" }");
    belgeler_code_str.append("\n};\n\n");

    degiskenler_code_str.append(",\n{ \"\",\"\",\"\",\"\",-1,\"\",\"\",\"\",\"\",\"\", -1, 0, 0, \"\", 0, 0, 0, 0, 0, 0 }");
    degiskenler_code_str.append("\n};\n\n");
    //degiskenler_code_str.append(QString("int eklenecek_belge_sayisi = %1;").arg(eklenecek_belge_sayisi));

    QFile degiskenler_file ("e9_default_belge_degiskenleri.h");

    if (!degiskenler_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QString header_begin = "#ifndef E9_DEFAULT_BELGE_DEGISKENLERI_H \n"
                           "#define E9_DEFAULT_BELGE_DEGISKENLERI_H \n\n";


    QString header_end   = "\n #endif";


    QTextStream write_file (&degiskenler_file);
    write_file << header_begin;
    write_file << belgeler_code_str;
    write_file << degiskenler_code_str;
    write_file << header_end;

    degiskenler_file.close();

    ADAK_INFO (QObject::tr("e9_default_belge.h was rebuilt."),NULL,NULL);//e9_default_belge.h yeniden oluşturuldu.
}
