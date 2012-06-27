#include <QApplication>
#include <QVariant>
#include <QPrintDialog>
#include <QProcess>

#include "html_tag.h"
#include "belge_kernel_defines.h"
#include "belge_kernel_open.h"
#include "belge_kernel_class.h"
#include "adak_defines.h"
#include "kernel_utils.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "tasarim_degiskeni.h"
#include "belge_kernel_utils.h"
#include "dot_matrix.h"
#include "dot_matrix_sabitler.h"

#define REPORT_BUFFER_SIZE 1000

extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   PRINT_BELGE
***************************************************************************************/

void PRINT_BELGE (int belge_turu, int tasarim_id , ADAK_BELGELER_STRUCT * belgeler_struct ,
                  QString printer_name, BELGE_PRINT_MOD p_mode )
{
    if (belge_turu NE -1) {
        SET_BELGE_TASARIM ( belge_turu, tasarim_id, belgeler_struct );
    }
    BELGE_KERNEL * F = new BELGE_KERNEL ( printer_name, p_mode );
    F->START_PRINT ();
}

/**************************************************************************************
                   BELGE_KERNEL::BELGE_KERNEL
***************************************************************************************/

BELGE_KERNEL::BELGE_KERNEL ( QString printer_name, BELGE_PRINT_MOD p_mode )
{
    m_printer_name      = printer_name;
    m_printer_type      = GET_BELGE_PRINTER_TYPE();
    m_line_variables    = NULL;
    m_mode              = p_mode;
}

/**************************************************************************************
                   BELGE_KERNEL::~BELGE_KERNEL
***************************************************************************************/

BELGE_KERNEL::~BELGE_KERNEL ()
{
    TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(false);
    degisken->RESET_STATIC_DEGISKEN();
    delete degisken;
}

/**************************************************************************************
                   BELGE_KERNEL::INIT_REPORT
***************************************************************************************/

void BELGE_KERNEL::INIT_REPORT ()
{
    m_header_struct      = GET_BELGE_HEADER_STRUCT();
    m_header_struct_size = GET_BELGE_HEADER_STRUCT_SIZE();
    m_footer_struct      = GET_BELGE_FOOTER_STRUCT();
    m_footer_struct_size = GET_BELGE_FOOTER_STRUCT_SIZE();
    m_line_struct        = GET_BELGE_LINE_STRUCT();
    m_line_struct_size   = GET_BELGE_LINE_STRUCT_SIZE();

    GET_KAYITLI_DEGISKENLER( &m_all_variables );

    m_row_count          = GET_BELGE_PRINT_ROW_COUNT();
    m_belge_satir_count  = GET_BELGE_ROW_COUNT();

    for (int i = 0; i < m_row_count; ++i) {
        QString *line_str = new QString();
        m_line_print_txt.append(line_str);
    }
    m_print_txt.setInsertInOrder(true);
}

/**************************************************************************************
                   BELGE_KERNEL::START_PRINT
***************************************************************************************/

void BELGE_KERNEL::START_PRINT ()
{
    INIT_REPORT();

    if (m_printer_type EQ LAZER_PRINTERS) {
        m_row_space_mm          = GET_BELGE_ROW_SPACE_MM();
        m_printer.setPaperSize(QSizeF(210,297),QPrinter::Millimeter);
        m_printer.setFullPage(true);

        if (m_printer_name.isEmpty() EQ false) {
            m_printer.setPrinterName(m_printer_name);
        }
        else {
            QPrintDialog printDialog(&m_printer);
            if (printDialog.exec() NE QDialog::Accepted) {
                return;
            }
        }

        m_painter.begin(&m_printer);
        CALCULATE_DEVICE_PIXEL();


        GET_VARIABLES();
        CREATE_LAZER_BELGE();

        m_painter.end();
    }
    else { // DOT_MATRIX_PRINTER

        m_row_space_satir = GET_BELGE_ROW_SPACE_SATIR();
        GET_BELGE_BOYUT_BILGILERI_SATIR(&m_belge_satir_sayisi,&m_belge_toplam_karakter_sayisi);

        GET_VARIABLES();
        CREATE_DOTMATRIX_BELGE();

        //! ONCE HEADER VE FOOTER SATIR INDISINE GORE BIR MAPTE TUTULUYOR
        //! SATIRLAR ISE AYRI BIR QString tutuluyor.
        //! ISLEMLER BITTIKTEN SATIRLARIN BASLAGICINDAN ITIBAREN
        //! SATIRLARDA MAP EKLENIYOR.SONRA MAP KENDI ICINDE SIRALANIP
        //! EN BUYUK DEGER BULUNUYOR. VE DEGER KADAR BILGI YAZILIYOR EGER O
        //! INDIS ICIN MAPTE DEGER YOKSA BOS SATIR ATILIYOR.

        if ( m_line_begin_row NE 0 ) {
            for (int j = 0; j < m_line_print_txt.size(); ++j) {
                QString *line_str = m_line_print_txt.at(j);
                line_str->append("\n");
                m_print_txt.insert(m_line_begin_row+j, line_str );

                QString *code = new QString();
                *code = m_line_print_code;

                m_print_code.insert(m_line_begin_row+j, code );
            }
        }

        int son_index = m_print_txt.keys().last();

        DOT_MATRIX printer;

        #ifdef WIN32   // Kullanicidan alinmali
            printer.SET_PRINTER_NAME  ("LPT1");
        #else
            printer.SET_PRINTER_NAME  ("IBM-ProPrinterII");
        #endif

        printer.OPEN_PRINTER_FILE();

        for (int i = 0; i < son_index+1; ++i) {
            if ( m_print_txt.keys().contains(i) EQ false ) {
                printer.WRITE_PRINTER("\n");
            }
            else {
                QString *data = m_print_txt.values(i).at(0);
                QStringList data_list = data->split("\\t");
                QString *code = m_print_code.values(i).at(0);
                QStringList code_list = code->split("\\t");
                for (int j = 0; j < data_list.size(); ++j) {
                    if (data_list.at(j).isEmpty() EQ false ) {
                        printer.WRITE_PRINTER(code_list.at(j).toAscii());
                        printer.WRITE_PRINTER(data_list.at(j).toAscii());
                    }
                }

                printer.WRITE_PRINTER("\n");
            }
        }

        printer.FINISH_PRINTER();
    }

    ADAK_INFO(qApp->tr("Belge Yazdırıldı.\t"),NULL, NULL);
}

/**************************************************************************************
                   BELGE_KERNEL::CREATE_DOTMATRIX_BELGE
***************************************************************************************/

void BELGE_KERNEL::CREATE_DOTMATRIX_BELGE()
{
    ADD_VARIABLES();
}

/**************************************************************************************
                   BELGE_KERNEL::ADD_DOTMATRIX_VARIABLES
***************************************************************************************/

void BELGE_KERNEL::ADD_DOTMATRIX_VARIABLES( ADAK_BELGE_DEFAULTS_STRUCT * adak_struct,int index,QStringList * variables, int grup, bool p_is_nlq, int p_printer_mode )
{
    if ( adak_struct[index].is_visible_variable EQ false) {
        return;
    }

    QString variable        = "";
    QString struct_variable = "";

    int row_count = 1;

    if (grup EQ LINE) {
        row_count = m_row_count;
    }

    if ( m_mode EQ PRINT_TEST_VARS ) {
        for (int i = 0 ; i < row_count ; i++ ) {
            if (i > 0) {
                variable.append('\n');
            }

            variable += adak_struct[index].degisken_adi;
        }
    }
    else {
        if (row_count EQ 1) { // HEADER AND FOOTER
            if(variables->at(index).isEmpty() EQ true) {
                return;
            }
            for ( int i = 0 ; i < variables->size() ; i++ ) {
                QStringList variable_info = variables->at(i).split("\\t");
                if ( variable_info.at(0).toInt() EQ adak_struct[index].degisken_id ) {
                    QString value = variable_info.at(1);
                    variable = ALING_STR(value, adak_struct[index].karakter_sayisi, adak_struct[index].align );
                    break;
                }
            }
        }
        else { // LINE
            for (int i = 0 ; i < row_count ; i++ ) {
                if(variables[i].size() <= index) {
                    break;
                }

                if(variables[i].at(index).isEmpty() EQ true) {
                    return;
                }

                if (i > 0) {
                    variable.append('\n');
                }

                for ( int j = 0 ; j < variables[i].size() ; j++ ) {
                    QStringList variable_info = variables[i].at(j).split("\\t");
                    if ( variable_info.at(0).toInt() EQ adak_struct[index].degisken_id ) {
                        struct_variable = variable_info.at(1);
                    }
                }
                variable += ALING_STR(struct_variable, adak_struct[index].karakter_sayisi, adak_struct[index].align );
            }
        }
    }

    SET_DOTMATRIX_VALUE(variable,adak_struct[index].x_position,adak_struct[index].y_position, grup, p_is_nlq, p_printer_mode );
}

/**************************************************************************************
                   BELGE_KERNEL::SET_DOTMATRIX_VALUE
***************************************************************************************/

void BELGE_KERNEL::SET_DOTMATRIX_VALUE(QString value, int soldan_bosluk, int satir, int grup, bool p_is_nlq, int p_printer_mode )
{

    //! SOLDAN BOSLUK EKLENIYOR
    if ( grup EQ LINE ) {
         QStringList line_variables = value.split('\n');
         value.clear();
         for (int i = 0; i < line_variables.size(); ++i) {

             QString temp = line_variables.at(i);
             for (int j = 0; j < soldan_bosluk; ++j) {
                 temp.prepend(" ");
             }
             if ( i NE line_variables.size() -1 ) {
                temp.append("\n");
             }
             value.append(temp);
         }
    }
    else {
        for (int i = 0; i < soldan_bosluk; ++i) {
            value.prepend(" ");
        }
    }
    //! END SOLDAN BOSLUK

    QString *variable;
    QString *code;

    if ( grup EQ HEADER  OR grup EQ USER_TEXT OR grup EQ FOOTER ) {

        if (m_print_txt.keys().contains(satir) EQ true ) {
            variable = m_print_txt.values(satir).at(0);
            code     = m_print_code.values(satir).at(0);
        }
        else {
            variable = new QString();
            code     = new QString();

            m_print_txt.insert(satir, variable);
            m_print_code.insert(satir, code   );
        }
        QString temp = *variable;
        variable->clear();
        variable->append( MERGE_STRING( temp, value ));
        *code   = MERGE_CODE( *code, DOT_MATRIX_PREPARE_VALUE(p_is_nlq, p_printer_mode ) );
    }

    if ( grup EQ LINE ) {
        m_line_begin_row = satir;
        QStringList line_variables = value.split('\n');
        m_line_print_code = MERGE_CODE( m_line_print_code, DOT_MATRIX_PREPARE_VALUE(p_is_nlq, p_printer_mode ) );
        for (int i = 0 ; i < line_variables.size() ; i++ ) {
            variable = m_line_print_txt.at(i);
            QString temp = *variable;
            variable->clear();
            m_line_print_txt.at(i)->append(MERGE_STRING( temp, line_variables.at(i)));
        }
    }
}

/**************************************************************************************
                   BELGE_KERNEL::ALING_STR
***************************************************************************************/

QString BELGE_KERNEL::ALING_STR(QString p_str, int p_str_size, int p_align)
{
    int space_size = 0;
    if ( p_str.size() EQ p_str_size ) {
        return p_str;
    }

    space_size = p_str_size - p_str.size();

    if ( p_align EQ LEFT_ALIGN ) {
        for (int i = 0; i < space_size; ++i) {
            p_str.append(" ");
        }
        return p_str;
    }
    if ( p_align EQ RIGHT_ALIGN ) {
        for (int i = 0; i < space_size; ++i) {
            p_str.prepend(" ");
        }
        return p_str;
    }
    if ( p_align EQ CENTER_ALIGN ) {
        for (int i = 0; i < space_size; ++i) {
            if ( i%2 EQ 0 ) {
                p_str.prepend(" ");
            }
            else {
                p_str.append(" ");
            }
        }
        return p_str;
    }

    return "#";
}

/**************************************************************************************
                   BELGE_KERNEL::DOT_MATRIX_PREPARE_VALUE
***************************************************************************************/

QString BELGE_KERNEL::DOT_MATRIX_PREPARE_VALUE(bool p_is_nlq, int p_printer_mode )
{
    QString pre_code;

    pre_code.append(GET_IBM_RESET_MODE());

    if ( p_is_nlq EQ true ) {
        pre_code.append(GET_IBM_NLQ());
    }
    else {
        pre_code.append(GET_IBM_NORMAL());
    }

    if ( p_printer_mode EQ CONDENSED_MODE ) {
        pre_code.append(GET_IBM_CONDENSEN_MODE());
    }
    else if ( p_printer_mode EQ LARGE_MODE ){
        pre_code.append(GET_IBM_LARGE_MODE());
    }

    return pre_code;
}

/**************************************************************************************
                   BELGE_KERNEL::CREATE_LAZER_BELGE
***************************************************************************************/

void BELGE_KERNEL::CREATE_LAZER_BELGE()
{
    ADD_VARIABLES();
}

/**************************************************************************************
                   BELGE_KERNEL::ADD_LAZER_VARIABLES
***************************************************************************************/

void BELGE_KERNEL::ADD_LAZER_VARIABLES( ADAK_BELGE_DEFAULTS_STRUCT * adak_struct,int index,QStringList * variables,int grup)
{
    if ( adak_struct[index].is_visible_variable EQ false) {
        return;
    }

    QString variable        = "";
    QString struct_variable = "";

    int row_count = 1;

    if (grup EQ LINE) {
        row_count = m_row_count;
    }

    if ( m_mode EQ PRINT_TEST_VARS ) {
        for (int i = 0 ; i < row_count ; i++ ) {
            if (i > 0) {
                variable.append("<br>");
            }
            variable += adak_struct[index].degisken_adi;
        }
    }
    else if ( m_mode EQ PRINT_TEST_XXX ) {
        for (int i = 0 ; i < row_count ; i++ ) {
            if (i > 0) {
                variable.append("<br>");
            }

            for (int i = 0; i < adak_struct[index].karakter_sayisi; ++i) {
                variable.append("x");
            }
        }
    }
    else {
        if (row_count EQ 1) {
            if(variables->at(index).isEmpty() EQ true) {
                return;
            }
            for ( int i = 0 ; i < variables->size() ; i++ ) {
                QStringList variable_info = variables->at(i).split("\\t");
                if ( variable_info.at(0).toInt() EQ adak_struct[index].degisken_id ) {
                    variable = variable_info.at(1);
                    break;
                }
            }
        }
        else {
            for (int i = 0 ; i < row_count ; i++ ) {
                if(variables[i].size() <= index) {
                    break;
                }

                if(variables[i].at(index).isEmpty() EQ true) {
                    return;
                }

                if (i > 0) {
                    variable.append("<br>");
                }
                for ( int j = 0 ; j < variables[i].size() ; j++ ) {
                    QStringList variable_info = variables[i].at(j).split("\\t");
                    if ( variable_info.at(0).toInt() EQ adak_struct[index].degisken_id ) {
                        struct_variable = variable_info.at(1);
                    }
                }

                variable += struct_variable;
            }
        }
    }

    m_painter.setPen( Qt::black);

    QFont font = m_painter.font();

    font.setPointSize(adak_struct[index].font_size );
    m_painter.setFont(font);

    variable.replace("\n","<br>");

    QStringList lines = variable.split("<br>");

    if (lines.size() > 0) {
        double mmx = m_device_pixel_rate_x * adak_struct[index].x_position;
        m_last_add_line_y_pos = adak_struct[index].y_position;

        int space = (QVariant(adak_struct[index].font_size).toDouble() * 0.336 ) + m_row_space_mm;

        for (int i = 0 ; i < lines.size();i++) {
            QString str_data = lines.at( i );
            double mmy = m_device_pixel_rate_y * m_last_add_line_y_pos;

            bool monospace_mi = false;

            QFont font = m_painter.font();

            if ( adak_struct[index].monospace_mi EQ 1 ) {
                monospace_mi = true;
                font.setFamily("Monospace");
            }

            m_painter.setFont( font );

            if ( adak_struct[index].karakter_sayisi NE 0 ) {
                QString space = " "; // 2 tane bosluk
                int carpan = 2;
                if ( monospace_mi EQ true ) {
                    carpan = 1;
                }
                int space_size = carpan*(adak_struct[index].karakter_sayisi - str_data.size());
                    if (monospace_mi EQ false ) {
                    if ( str_data.count(".") NE 0 ) {
                        space_size += str_data.count(".");
                    }
                    if ( str_data.count(",") NE 0 ) {
                        space_size += str_data.count(",");
                    }
                    if ( str_data.count(":") NE 0 ) {
                        space_size += str_data.count(":");
                    }
                    if ( str_data.count(" ") NE 0 ) {
                        space_size += str_data.count(" ");
                    }
                }
                for ( int j = 0; j < space_size; j++ ) {
                    switch( adak_struct[index].align ) {
                        case LEFT_ALIGN: {
                            str_data.append( space );
                            break;
                        }
                        case CENTER_ALIGN: {
                            // gelen ilk bosluk onune ekleniyor 2. sonuna
                            if ( j%2 EQ 0 ) {
                                str_data.append( space );
                            }
                            else {
                                str_data.prepend( space );
                            }
                            break;
                        }
                        case RIGHT_ALIGN: {
                            str_data.prepend( space );
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            m_painter.drawText(QPointF(mmx,mmy), str_data );
            m_last_add_line_y_pos += space;
        }

    }
    else {
        double mmx = m_device_pixel_rate_x * adak_struct[index].x_position;
        double mmy = m_device_pixel_rate_y * adak_struct[index].y_position;
        m_painter.drawText(QPointF(mmx,mmy),variable);
    }
}

/**************************************************************************************
                   BELGE_KERNEL::ADD_USER_VARIABLES
***************************************************************************************/

void BELGE_KERNEL::ADD_VARIABLES()
{

    ADAK_BELGE_DEFAULTS_STRUCT * belge_struct = new ADAK_BELGE_DEFAULTS_STRUCT;

    // degiskenler once siralanmali ------------> satirlar
/*    1....N
      --------->
 N+1  |
      |
      |
 N+M  v

*/

    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
        QList<TASARIM_DEGISKENI*> sirali_liste;

        for (int k = 0; k < m_belge_satir_count; ++k) {
            int aranan_satir = k;

            QList<TASARIM_DEGISKENI*> satirdaki_degiskenler;
            for (int i = 0; i < m_all_variables.size(); ++i) {
                TASARIM_DEGISKENI *aranan_degisken = m_all_variables.at(i);
                if ( aranan_degisken->GET_SATIR() EQ aranan_satir ) {
                    satirdaki_degiskenler.append(aranan_degisken);
                }
            }
            //! hesaplama

            //! AYNI SATIRDAKI DEGISKENLERI SIRALA ASC

            DEGISLENLERI_SIRALA(&satirdaki_degiskenler);

            sirali_liste.append(satirdaki_degiskenler);
        }

        m_all_variables.clear();

        m_all_variables.append(sirali_liste);
    }

    for ( int i = 0 ; i < m_all_variables.size() ; i++ ) {

        TASARIM_DEGISKENI *degisken = m_all_variables.at(i);

        double x = degisken->GET_POS_X();
        double y = degisken->GET_POS_Y();

        double width  = degisken->GET_SIZE_X();
        double height = degisken->GET_SIZE_Y();

        if (m_printer_type EQ LAZER_PRINTERS) {


            m_painter.setPen( Qt::black);
            QFont font = m_painter.font();
            font.setFamily      (degisken->GET_FONT_FAMILY());
            font.setBold        (degisken->IS_BOLD());
            font.setUnderline   (degisken->IS_UNDER_LINE());
            font.setItalic      (degisken->IS_ITALIC());
            font.setStrikeOut   (degisken->IS_STRIKEOUT());
            font.setPointSize   (degisken->GET_FONT_SIZE());
            m_painter.setFont(font);

            double mmx = m_device_pixel_rate_x * x;
            double mmy = m_device_pixel_rate_y * y;

            if ( degisken->IS_VISIBLE() EQ false ) {
                continue;
            }

            if ( degisken->GET_GRUP() EQ USER_TEXT ) {
                m_painter.drawText(QPointF(mmx,mmy), degisken->GET_TEXT() );
            }
            else if ( degisken->GET_GRUP() EQ USER_IMG ) {
                m_painter.drawImage(QRect(mmx, mmy, width, height), degisken->GET_IMAGES().toImage() );
            }
            else  {

                belge_struct->degisken_id         = degisken->GET_DEGISKEN_ID();
                belge_struct->align               = degisken->GET_ALIGN();
                belge_struct->degisken_adi        = degisken->GET_TEXT().toStdString().c_str();
                belge_struct->karakter_sayisi     = degisken->GET_TEXT_SIZE();
                belge_struct->degisken_kodu       = degisken->GET_KODU().toStdString().c_str();
                belge_struct->font_size           = degisken->GET_FONT_SIZE();
                belge_struct->is_visible_variable = degisken->IS_VISIBLE();
                belge_struct->monospace_mi        = degisken->IS_MONOSPACE_MI();
                belge_struct->x_position          = degisken->GET_POS_X();
                belge_struct->y_position          = degisken->GET_POS_Y();

                if ( degisken->GET_GRUP() EQ HEADER ){
                    ADD_LAZER_VARIABLES( belge_struct, 0, &m_header_variables, HEADER );
                }
                if ( degisken->GET_GRUP() EQ LINE ){
                    ADD_LAZER_VARIABLES( belge_struct, 0, &*m_line_variables, LINE );
                }
                if ( degisken->GET_GRUP() EQ FOOTER ){
                    ADD_LAZER_VARIABLES( belge_struct, 0, &m_footer_variables, FOOTER );
                }
            }
        }
        else {
            if ( degisken->IS_VISIBLE() EQ true) {

                if ( degisken->GET_GRUP() EQ USER_TEXT ) {
                    SET_DOTMATRIX_VALUE( degisken->GET_TEXT(), degisken->GET_SOLDAN_BOSLUK(), degisken->GET_SATIR(), degisken->GET_GRUP(), degisken->IS_NLQ(), degisken->GET_DOT_MARIX_MODE() );
                }

                belge_struct->degisken_id         = degisken->GET_DEGISKEN_ID();
                belge_struct->align               = degisken->GET_ALIGN();
                belge_struct->degisken_adi        = degisken->GET_TEXT().toStdString().c_str();
                belge_struct->karakter_sayisi     = degisken->GET_TEXT_SIZE();
                belge_struct->degisken_kodu       = degisken->GET_KODU().toStdString().c_str();
                belge_struct->font_size           = degisken->GET_FONT_SIZE();
                belge_struct->is_visible_variable = degisken->IS_VISIBLE();
                belge_struct->monospace_mi        = degisken->IS_MONOSPACE_MI();
                belge_struct->x_position          = degisken->GET_SOLDAN_BOSLUK();
                belge_struct->y_position          = degisken->GET_SATIR();

                if ( degisken->GET_GRUP() EQ HEADER ){
                    ADD_DOTMATRIX_VARIABLES(belge_struct,0,&m_header_variables,HEADER, degisken->IS_NLQ(), degisken->GET_DOT_MARIX_MODE() );
                }

                if ( degisken->GET_GRUP() EQ LINE ){
                    ADD_DOTMATRIX_VARIABLES(belge_struct,0,&*m_line_variables,LINE, degisken->IS_NLQ(), degisken->GET_DOT_MARIX_MODE() );
                }

                if ( degisken->GET_GRUP() EQ FOOTER ){
                    ADD_DOTMATRIX_VARIABLES(belge_struct,0,&m_footer_variables,FOOTER, degisken->IS_NLQ(), degisken->GET_DOT_MARIX_MODE());
                }
            }
        }
    }

    delete belge_struct;
}

/**************************************************************************************
                   BELGE_KERNEL::CLEAR_VARIABLES_STRING
***************************************************************************************/

void BELGE_KERNEL::SET_HEADER_DATA(int header_enum, QString value)
{
    m_header_variables << QString( QVariant(header_enum).toString() + "\\t"+ value );
}

/**************************************************************************************
                   BELGE_KERNEL::CLEAR_VARIABLES_STRING
***************************************************************************************/

void BELGE_KERNEL::SET_LINE_DATA(int line_enum, QString value)
{
    m_line_variables[m_kullanilan_satir_sayisi] << QString( QVariant(line_enum).toString() + "\\t" + value );
}

/**************************************************************************************
                   BELGE_KERNEL::CLEAR_VARIABLES_STRING
***************************************************************************************/

void BELGE_KERNEL::SET_FOOTER_DATA(int footer_enum, QString value)
{
    m_footer_variables << QString( QVariant(footer_enum).toString() + "\\t" + value );
}

/**************************************************************************************
                   BELGE_KERNEL::GET_VARIABLES
***************************************************************************************/

void BELGE_KERNEL::GET_VARIABLES ()
{
    if ( m_mode EQ PRINT_TEST_VARS OR m_mode EQ PRINT_TEST_XXX ) {
        return;
    }

    m_header_variables.clear();
    GET_HEADER_INFO();

    // Satir sayisi kadar stringlist ayarla
    if (m_line_variables NE NULL) {
        delete m_line_variables;
    }
    m_line_variables     = new QStringList[m_row_count];
    m_kullanilan_satir_sayisi = 0;

    for (int i = 0 ; i < m_row_count ; i++) {
        m_kullanilan_satir_sayisi = i;
        GET_LINE_INFO();
    }

    m_footer_variables.clear();
    GET_FOOTER_INFO();
}

/**************************************************************************************
                   BELGE_KERNEL::GET_HEADER_INFO
***************************************************************************************/

void BELGE_KERNEL::GET_HEADER_INFO ()
{

}

/**************************************************************************************
                   BELGE_KERNEL::GET_FOOTER_INFO
***************************************************************************************/

void BELGE_KERNEL::GET_FOOTER_INFO ()
{

}

/**************************************************************************************
                   BELGE_KERNEL::GET_LINE_INFO
***************************************************************************************/

void BELGE_KERNEL::GET_LINE_INFO ()
{

}

/**************************************************************************************
                   BELGE_KERNEL::CALCULATE_DEVICE_PIXEL
***************************************************************************************/

void BELGE_KERNEL::CALCULATE_DEVICE_PIXEL ()
{
    QRectF  milimiter    = m_printer.paperRect(QPrinter::Millimeter);
    QRectF  device_pixel = m_printer.paperRect();

    m_device_pixel_rate_x = device_pixel.width()  / milimiter.width();
    m_device_pixel_rate_y = device_pixel.height() / milimiter.height();
}

/**************************************************************************************
                   BELGE_KERNEL::GET_LIST_VALUE
***************************************************************************************/

QString BELGE_KERNEL::GET_LIST_VALUE(QStringList p_list, int p_degisken_id)
{
    for (int i = 0; i < p_list.size(); ++i) {
        QStringList degisken_data_list = p_list.at(i).split("\\t");
        int degisken_id = degisken_data_list.at(0).toInt();
        if ( p_degisken_id EQ degisken_id  ) {
            QString data = degisken_data_list.at(1);
            data.replace("\n","<br>");
            return data;
        }
    }

    return "NULL";
}

/**************************************************************************************
                   BELGE_KERNEL::MERGE_STRING
***************************************************************************************/

QString BELGE_KERNEL::MERGE_STRING(QString p_str1, QString p_eklenecek_str)
{
    QString new_str;

    if ( p_str1.isEmpty() EQ false ) {
        p_str1.append("\\t");
    }

    new_str.append(p_str1);
    new_str.append(p_eklenecek_str);

    return new_str;
}

/**************************************************************************************
                   BELGE_KERNEL::MERGE_CODE
***************************************************************************************/

QString BELGE_KERNEL::MERGE_CODE(QString p_code1, QString p_eklenecek)
{
    return MERGE_STRING(p_code1, p_eklenecek);
}
