#include <QApplication>
#include <QVariant>
#include <QFile>
#include <QTextStream>

#include "html_tag.h"
#include "report_kernel.h"
#include "adak_defines.h"
#include "kernel_utils.h"
#include "print.h"

extern QStringList G_FORM_NAME;

/***************************************************************************************
                   REPORT_KERNEL::REPORT_KERNEL
****************************************************************************************/

REPORT_KERNEL::REPORT_KERNEL ( QString report_name )
{
    p_table_tag_string     = "";
    p_css_wrap_tag         = "nowrap";
    p_overflow_tag         = "hidden";
    m_printer_html_string  = "";
    p_paper_type_enum      = QPrinter::Custom;
    SET_FOOTER_POSITION_PROPERTY ( POSITION_RELATIVE );
    SET_PAPER_SIZE( QPrinter::A4 );
    G_FORM_NAME << report_name;

    m_orientation          = QPrinter::Portrait;
}

/**************************************************************************************
                      REPORT_KERNEL::~REPORT_KERNEL
***************************************************************************************/

REPORT_KERNEL::~REPORT_KERNEL()
{
    G_FORM_NAME.removeLast();
}

/**************************************************************************************
                   REPORT_KERNEL::CREATE_HTML_PAGES
***************************************************************************************/

QString REPORT_KERNEL::CREATE_HTML_PAGES()
{
    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
    splash->show();
    splash->showMessage ( QString ( QObject::tr("Report is preparing. Please wait...") ) ,Qt::AlignCenter,Qt::white );//Rapor Hazırlanıyor.Lütfen Bekleyiniz...
    qApp->processEvents();

    QString header_string;
    QString footer_string;
    QString body_string;

    first_page_passed = false;
    do {
        header_string  =  GET_HEADER();
        body_string    =  GET_BODY();
        footer_string  =  GET_FOOTER();

        if ( body_string.isNull() EQ true ) {
            if ( first_page_passed EQ false ) {
                 splash->finish( NULL );
                return "";
            }
            break;
        }

        if ( first_page_passed EQ true ) {
            p_page_html_string.append ( "<p class=newpage>" );
        }

        if ( header_string.isNull() EQ false ) {
            SET_HEADER ( header_string );
        }

        SET_BODY ( body_string );

        if ( footer_string.isNull() EQ false ) {
            SET_FOOTER ( footer_string );
        }
        first_page_passed = true;

    } while ( true );

    splash->finish ( NULL );

    m_printer_html_string = GET_HTML();

//    QFile out_file ("m_printer_html_string.html");
//    if (out_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        QTextStream out(&out_file);
//        out << m_printer_html_string;
//        out_file.close();
//    }
    return m_printer_html_string;

}

/**************************************************************************************
                   REPORT_KERNEL::SET_WRAP
***************************************************************************************/

void REPORT_KERNEL::SET_WRAP ( WRAP_TYPE wrap_type )
{
    if  ( wrap_type EQ WRAP ) {
        p_css_wrap_tag = "wrap";
    }
    else if ( wrap_type EQ NO_WRAP ) {
        p_css_wrap_tag = "nowrap";
    }
}

/**************************************************************************************
                   REPORT_KERNEL::SET_OVERFLOW_TAG
***************************************************************************************/

void REPORT_KERNEL::SET_OVERFLOW_TAG ( QString overflow_tag )
{
    p_overflow_tag = overflow_tag;
}

/**************************************************************************************
                   REPORT_KERNEL::CREATE_META_TAG
***************************************************************************************/

void REPORT_KERNEL::CREATE_META_TAG()
{
    p_meta_tag_string.clear();
    p_html_end_string.clear();

    p_meta_tag_string.append ( "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
    p_meta_tag_string.append ( "<html><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"></meta>");

    p_html_end_string.append ( "</body></html>" );
}

/**************************************************************************************
                   REPORT_KERNEL::CREATE_CSS
***************************************************************************************/

void REPORT_KERNEL::CREATE_CSS()
{
    QString content_height_string;
    QString header_height_string;
    QString footer_height_string;

    p_body_height = p_paper_height - p_header_height - p_footer_height;

    content_height_string.setNum( p_body_height );
    header_height_string.setNum ( p_header_height );
    footer_height_string.setNum ( p_footer_height );

    p_css_string.clear();

    p_css_string.append ( "<style type=\"text/css\">"
                          "body{font-size:18px; font-family: helvetica, arial; margin:0; height:"+content_height_string+" mm;padding:0 0 0 0;}"
                          "div#header{position:relative;top:0;left:0;width:100%;height:"+header_height_string+" mm;}"
                          "div#footer{position:"+p_footer_position_name+";bottom:0;left:0;width:100%; }"
                          "* html body{overflow:"+p_overflow_tag+";} "
                          "* html div#content{height:"+content_height_string+"mm; overflow:"+p_overflow_tag+";}"
                          "p.newpage {page-break-before: always}"
                          "p { margin:0 0 0 0; padding: 0 0 0 0; }"
                          "table { table-layout:fixed; } "
                          "td { white-space:"+p_css_wrap_tag+"; overflow:"+p_overflow_tag+"; text-overflow:ellipsis; } "
                          "</style></head><body>" );
}

/**************************************************************************************
                   REPORT_KERNEL::SET_FOOTER_POSITION_PROPERTY
***************************************************************************************/

void REPORT_KERNEL::SET_FOOTER_POSITION_PROPERTY ( CSS_POSITION_VALUE_NAME footer_position )
{
    switch ( footer_position ) {
        case POSITION_ABSOLUTE:
            p_footer_position_name = "absolute";
            break;
        case POSITION_FIXED:
            p_footer_position_name = "fixed";
            break;
        case POSITION_INHERIT:
            p_footer_position_name = "inherit";
            break;
        case POSITION_RELATIVE:
            p_footer_position_name = "relative";
            break;
        case POSITION_STATIC:
            p_footer_position_name = "static";
            break;
        default:
            break;
    }
}

/**************************************************************************************
                   REPORT_KERNEL::SET_PAPER_SIZE
***************************************************************************************/

void REPORT_KERNEL::SET_PAPER_SIZE ( int paper_type_enum )
{
    p_paper_type_enum = paper_type_enum;

    switch( paper_type_enum ) {
        case QPrinter::A3:
            p_paper_height = 420;
            p_paper_width  = 297;
            return;
        case QPrinter::A5:
            p_paper_height = 210;
            p_paper_width  = 148;
            return;
        case QPrinter::B3:
            p_paper_height = 500;
            p_paper_width  = 353;
            return;
        case QPrinter::B4:
            p_paper_height = 353;
            p_paper_width  = 250;
            return;
        case QPrinter::B5:
            p_paper_height = 250;
            p_paper_width  = 176;
            return;
        default:
        case QPrinter::A4:
            p_paper_height = 297;
            p_paper_width  = 210;
            return;
    }
}

/**************************************************************************************
                   REPORT_KERNEL::SET_PAPER_SIZE
***************************************************************************************/

void REPORT_KERNEL::SET_PAPER_SIZE (double paper_width , double paper_height)
{
    p_paper_type_enum   = QPrinter::Custom;
    p_paper_width       = paper_width;
    p_paper_height      = paper_height;
}

/**************************************************************************************
                   REPORT_KERNEL::SET_HEIGHTS
***************************************************************************************/

void REPORT_KERNEL::SET_HEIGHTS ( int header_height, int footer_height )
{
    p_header_height = header_height;
    p_footer_height = footer_height;
}

/**************************************************************************************
                   REPORT_KERNEL::SET_HEADER
***************************************************************************************/

void REPORT_KERNEL::SET_HEADER (  QString value )
{
    SET_TAG_NAME  ( "div" );
    SET_ATTRIBUTE ( "id","header" );
    SET_VALUE     ( value );

    p_page_html_string.append (  GET_HTML_TAG() );
}

/**************************************************************************************
                   REPORT_KERNEL::SET_BODY
***************************************************************************************/

void REPORT_KERNEL::SET_BODY ( QString value )
{
    SET_TAG_NAME  ( "div" );
    SET_ATTRIBUTE ( "id","content" );
    SET_VALUE     ( value );

    p_page_html_string.append (  GET_HTML_TAG() );
}

/**************************************************************************************
                   REPORT_KERNEL::SET_FOOTER
***************************************************************************************/

void REPORT_KERNEL::SET_FOOTER (  QString value )
{
    SET_TAG_NAME  ( "div" );
    SET_ATTRIBUTE ( "id","footer" );
    SET_VALUE     ( value );

    p_page_html_string.append (  GET_HTML_TAG() );
}

/**************************************************************************************
                   REPORT_KERNEL::CALCULATE_MAX_LINE_COUNT
***************************************************************************************/

void REPORT_KERNEL::CALCULATE_MAX_LINE_COUNT ( int line_height_in_point )
{
    float line_height;

    double font_size = QVariant ( line_height_in_point ).toDouble();

    line_height = POINT_MM_ORANI* ( font_size ) * (1.00+ ( ( 1.00/font_size )*2.70 ) );

    p_max_body_line_count = (int) ( QVariant( p_body_height ).toDouble() / line_height );
}

/**************************************************************************************
                   REPORT_KERNEL::GET_HTML
****************************************************************************************/

QString REPORT_KERNEL::GET_HTML()
{
    CREATE_META_TAG();
    CREATE_CSS();

    p_html_string.append ( p_meta_tag_string );
    p_html_string.append ( p_css_string );
    p_html_string.append ( p_page_html_string );
    p_html_string.append ( p_html_end_string );

    return p_html_string;
}

/**************************************************************************************
                   REPORT_KERNEL::GET_MAX_LINE_COUNT
***************************************************************************************/

int REPORT_KERNEL::GET_MAX_LINE_COUNT ( int line_height )
{
    CREATE_CSS();
    CALCULATE_MAX_LINE_COUNT ( line_height );
    return p_max_body_line_count;
}

/**************************************************************************************
                   REPORT_KERNEL::CREATE_TABLE
***************************************************************************************/

void REPORT_KERNEL::CREATE_TABLE ( QStringList headers,int border_height,int font_size_in_point )
{
    p_table_html_string.clear();

    if ( p_table_tag_string.isEmpty() EQ true ) {
        p_table_html_string.append ( QString ( "<table border=%1 style=\"font-size:%2pt;\"><tr>" ).arg ( border_height).arg(font_size_in_point));
    }
    else {
        p_table_html_string.append ( p_table_tag_string );
        p_table_html_string.append ( "<tr>" );
    }

    table_header_size = headers.size();
    int size = p_td_tag_string.size();

    if ( size NE table_header_size ) {
        for ( int i = 0; i < table_header_size - size; i++ ) {
            p_td_tag_string << "";
        }
    }

    QString td_tag = "<td %1>";

    for( int i = 0; i < table_header_size ; i++ ) {
        p_table_html_string.append ( td_tag.arg(p_td_tag_string.at(i))+headers.at(i)+"</td>" );
    }

    p_table_html_string.append ( "</tr>" );
    p_table_tag_string = "";
}

/**************************************************************************************
                      REPORT_KERNEL::ADD_ROW_TO_TABLE
***************************************************************************************/

void REPORT_KERNEL::ADD_ROW_TO_TABLE ( QStringList row_datas )
{
    p_table_html_string.append ( "<tr>" );
    int size = p_td_tag_string.size();

    if ( size NE table_header_size ) {
        for ( int i = 0; i < table_header_size - size; i++ ) {
            p_td_tag_string << "";
        }
    }

    QString td_tag = "<td %1>";

    for ( int i = 0; i < table_header_size ; i++ ) {
         p_table_html_string.append ( td_tag.arg ( p_td_tag_string.at(i) )+row_datas.at(i)+"</td>" );
    }

    p_table_html_string.append ( "</tr>" );
}

/**************************************************************************************
                   REPORT_KERNEL::GET_TABLE_HTML_STRING
***************************************************************************************/

QString REPORT_KERNEL::GET_TABLE_HTML_STRING()
{
    p_table_html_string.append ( "</table>" );
    return p_table_html_string;
}

/**************************************************************************************
                   REPORT_KERNEL::SET_TABLE_TAG_STRING
***************************************************************************************/

void REPORT_KERNEL::SET_TABLE_TAG_STRING ( QString table_tag_string )
{
    p_table_tag_string = table_tag_string;
}

/**************************************************************************************
                   REPORT_KERNEL::SET_TD_TAG_STRING
***************************************************************************************/

void REPORT_KERNEL::SET_TD_TAG_STRING ( QStringList td_tag_string )
{
    p_td_tag_string = td_tag_string;
}

/**************************************************************************************
                   REPORT_KERNEL::ADD_HTML_SPACE
***************************************************************************************/

QString REPORT_KERNEL::ADD_HTML_SPACE ( QString html_string )
{
    return html_string.replace ( " ","&nbsp;" );
}

/**************************************************************************************
                   REPORT_KERNEL::OPEN_PRINTER_PREVIEW
***************************************************************************************/

void REPORT_KERNEL::OPEN_PRINTER_PREVIEW(QWidget * parent, qreal left_margin ,qreal top_margin,qreal bottom_margin,qreal right_margin)
{
    OPEN_REPORT_SHOWER ( m_printer_html_string,parent, m_orientation, left_margin,top_margin,bottom_margin,right_margin,p_paper_type_enum,p_paper_width,p_paper_height );
}


/**************************************************************************************
                   REPORT_KERNEL::SET_ORIENTATION
***************************************************************************************/

void REPORT_KERNEL::SET_ORIENTATION ( QPrinter::Orientation p_paper_orientation )
{
    m_orientation = p_paper_orientation;

    if ( p_paper_orientation EQ QPrinter::Landscape ) {
        int temp = p_paper_height;
        // -10 yatay yapinca diger sayfaya tasma olmasin diye cikarilnistir
        p_paper_height = p_paper_width - 10;
        p_paper_width  = temp;

    }
}
