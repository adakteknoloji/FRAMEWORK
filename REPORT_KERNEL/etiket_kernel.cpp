#include <QString>
#include "etiket_kernel.h"
#include "adak_utils.h"

/**************************************************************************************
                   ETIKET_KERNEL::ETIKET_KERNEL
***************************************************************************************/

ETIKET_KERNEL::ETIKET_KERNEL() : REPORT_KERNEL ( "ETIKET_KERNEL" )
{
    p_etiket_width               = 10;
    p_etiket_height              = 10;
    p_page_left_space            = 0;
    p_page_top_space             = 0;
    p_margin_right               = 5;
    p_margin_bottom              = 5;
    p_satirdaki_etiket_sayisi    = 2;
    p_eklenen_etiket_sayisi      = 0;

    p_finish_report              = false;

    SET_HEIGHTS ( 0,0 );
    SET_WRAP ( WRAP );
}


/**************************************************************************************
                   ETIKET_KERNEL::ETIKETLERI_OLUSTUR
***************************************************************************************/

QString ETIKET_KERNEL::ETIKETLERI_OLUSTUR ( QStringList etiketler )
{
    p_etiketler            = etiketler;
    p_toplam_etiket_sayisi = etiketler.size();
    p_max_line_count       = 13;

    return CREATE_HTML_PAGES();
}

/**************************************************************************************
                   ETIKET_KERNEL::SET_PAGE_SPACE
**************************************************************************************/

void ETIKET_KERNEL::SET_PAGE_SPACE ( double top_space,double left_space )
{
    p_page_top_space  = CONVERT_MM_TO_PT ( top_space );
    p_page_left_space = CONVERT_MM_TO_PT ( left_space );
}

/**************************************************************************************
                   ETIKET_KERNEL::SET_ETIKET_SIZE
***************************************************************************************/

void ETIKET_KERNEL::SET_ETIKET_SIZE ( double etiket_width,double etiket_height )
{
    p_etiket_width  = CONVERT_MM_TO_PT ( etiket_width - 3 );
    p_etiket_height = CONVERT_MM_TO_PT ( etiket_height - 3 );
}

/**************************************************************************************
                   ETIKET_KERNEL::SET_ETIKET_MARGIN
***************************************************************************************/

void ETIKET_KERNEL::SET_ETIKET_MARGIN ( double margin_right,double margin_bottom )
{
    p_margin_bottom = CONVERT_MM_TO_PT ( margin_bottom );
    p_margin_right  = CONVERT_MM_TO_PT ( margin_right );
}

/**************************************************************************************
                   ETIKET_KERNEL::SET_SATIRDAKI_ETIKET_SAYISI
***************************************************************************************/

void ETIKET_KERNEL::SET_SATIRDAKI_ETIKET_SAYISI ( int etiket_sayisi )
{
    p_satirdaki_etiket_sayisi = etiket_sayisi;
}

/**************************************************************************************
                   ETIKET_KERNEL::SET_SAYFADAKI_ETIKET_SAYISI
***************************************************************************************/

void ETIKET_KERNEL::SET_SAYFADAKI_SATIR_SAYISI ( int satir_sayisi )
{
    p_max_line_count = satir_sayisi;
}

/**************************************************************************************
                   ETIKET_KERNEL::GET_BODY
***************************************************************************************/

QString ETIKET_KERNEL::GET_BODY()
{
    if ( p_finish_report EQ true ) {
        return NULL;
    }

    //sayfanin sinirlarina kadar genisleyebilecek bir table
    QString table_tag_string="<table style=\"border:1px solid #FFFFFF;\">";
    SET_TABLE_TAG_STRING ( table_tag_string );

    QStringList td_tag_string;
    QStringList header_string;

    //sol tarafdaki boslugu tutacak column
    td_tag_string << QString ( "style=\"width:0; height:%1pt; border:1px solid #FFFFFF;\"" ).arg ( p_etiket_height);
    header_string << "" ;

    for  ( int i=0;i < p_satirdaki_etiket_sayisi; i++ ) {
        td_tag_string << QString ( "style=\"width:%1pt; height:%2pt; border:1px solid #FFFFFF;\"" ).arg ( p_etiket_width).arg(p_etiket_height);
        td_tag_string << QString ( "style=\"width:%1pt; height:%2pt; border:1px solid #FFFFFF;\"" ).arg ( p_margin_right).arg(p_etiket_height);
        header_string  << "" << "";
    }

    CREATE_TABLE ( header_string,0,0 );

    QStringList top_space;
    QStringList top_string;

    for (int i=0;i < p_satirdaki_etiket_sayisi ; i++ ) {
        top_space  << ""<< QString ( "style=\"width:%1pt; height:0; border:1px solid #FFFFFF;\"" ).arg ( p_margin_right)<<"";
        top_string << ""<< "" << "" ;
    }

    SET_TD_TAG_STRING ( top_space  );
    ADD_ROW_TO_TABLE  ( top_string );
    SET_TD_TAG_STRING ( td_tag_string );

    for ( int i=0;i < p_max_line_count;i++ ) {
       QStringList row_etiketleri;
       QStringList bottom_margin;
       QStringList bottom_string;

       row_etiketleri << "";
       bottom_margin  << "";
       bottom_string  << "";

       for ( int i=0;i < p_satirdaki_etiket_sayisi; i++ ) {
            if ( p_eklenen_etiket_sayisi >= p_toplam_etiket_sayisi ) {
                p_finish_report = true;
                row_etiketleri << "" << "";
                bottom_margin  << QString ( "style=\"width:%1pt; height:%2pt; border:1px solid #FFFFFF;\"").arg(p_margin_right).arg(p_margin_bottom)<<"";
                bottom_string  << "" << "" ;
                continue;
            }
            QString etiket_str = "";
            etiket_str.append ( QString ( "<div style=\"width:%1pt; height:%2pt; overflow:hidden;\">" ).arg (p_etiket_width)
                                                                                                  .arg(p_etiket_height));
            etiket_str.append ( p_etiketler.at(p_eklenen_etiket_sayisi ) );

            etiket_str.append ( "</div>" );

            row_etiketleri << etiket_str << "";
            bottom_margin  << QString ( "style=\"width:%1pt; height:%2pt; border:1px solid #FFFFFF;\"").arg(p_margin_right).arg(p_margin_bottom)<<"";
            bottom_string  << "" << "" ;
            p_eklenen_etiket_sayisi++;
       }

       SET_TD_TAG_STRING ( td_tag_string );
       ADD_ROW_TO_TABLE  ( row_etiketleri);
       SET_TD_TAG_STRING ( bottom_margin );
       ADD_ROW_TO_TABLE  ( bottom_string );
    }

    return GET_TABLE_HTML_STRING();
}

/**************************************************************************************
                   ETIKET_KERNEL::CONVERT_MM_TO_PT
***************************************************************************************/

int ETIKET_KERNEL::CONVERT_MM_TO_PT ( double mm )
{    
    return ( ( ( ( double )mm )/ ( POINT_MM_ORANI ) )*1.4 );
}

/**************************************************************************************
                   ETIKET_KERNEL::GET_FOOTER
***************************************************************************************/

QString ETIKET_KERNEL::GET_FOOTER()
{
    return NULL;
}

/**************************************************************************************
                   ETIKET_KERNEL::GET_HEADER
***************************************************************************************/
QString ETIKET_KERNEL::GET_HEADER()
{
    return NULL;
}

/**************************************************************************************
                   ETIKET_KERNEL::ETIKET_SABLONLARINI_OLUSTUR
***************************************************************************************/

void ETIKET_KERNEL::ETIKET_SABLONLARINI_OLUSTUR()
{
    etiket_sablonlari etiket;

    etiket.name                    = QObject::tr ( "Düz Köşeli Etiket - 52lik" );
    etiket.etiket_width            = 50.0;
    etiket.etiket_height           = 22.0;
    etiket.page_left_space         =  5.0;
    etiket.page_top_space          =  6.0;
    etiket.margin_right            =  0;
    etiket.margin_bottom           =  0;
    etiket.satirdaki_etiket_sayisi =  4;
    etiket.sayfadaki_satir_sayisi  = 13;
    etiket.icon_path               = "duz_52_50_22.jpg";
    p_sablonlar << etiket;

    etiket.name                    = QObject::tr ( "Oval Köşeli Etiket - 65lik" );
    etiket.etiket_width            = 38.1;
    etiket.etiket_height           = 21.2;
    etiket.page_left_space         =  5.0;
    etiket.page_top_space          = 14.0;
    etiket.margin_right            =  0;
    etiket.margin_bottom           =  0;
    etiket.satirdaki_etiket_sayisi =  5;
    etiket.sayfadaki_satir_sayisi  = 13;
    etiket.icon_path               = "oval_65_39.1_21.2.jpg";
    p_sablonlar << etiket;
}

/**************************************************************************************
                   QList<etiket_sablonlari> GET_ETIKET_SABLONLARI()
***************************************************************************************/

QList<etiket_sablonlari> GET_ETIKET_SABLONLARI()
{
    ETIKET_KERNEL * etiket_kernel = new ETIKET_KERNEL;
    etiket_kernel->ETIKET_SABLONLARINI_OLUSTUR();
    return etiket_kernel->p_sablonlar;
}
