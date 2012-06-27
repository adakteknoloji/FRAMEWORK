#include <QLabel>
#include <QFile>
#include <QTextStream>
#include "belge_kernel_utils.h"
#include "adak_defines.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "belge_struct.h"
#include "belge_kernel_defines.h"
#include "tasarim_degiskeni.h"

static ADAK_BELGELER_STRUCT       * g_adak_belgeler_struct;

static int g_header_struct_size;
static ADAK_BELGE_DEFAULTS_STRUCT * g_header_struct;

static int g_footer_struct_size;
static ADAK_BELGE_DEFAULTS_STRUCT * g_footer_struct;

static int g_line_struct_size;
static ADAK_BELGE_DEFAULTS_STRUCT * g_line_struct;


static int      g_belge_id         = -1;
static QString  g_belge_adi        = "";
static int      g_tasarim_id       = -1;
static QString  g_tasarim_adi      = "";
static bool     g_tasarim_readonly = false;

extern ADAK_SQL * G_YONETIM_DB;


/**************************************************************************************
                   SET_BELGE_TASARIM
***************************************************************************************/

void SET_BELGE_TASARIM( int belge_id, int tasarim_id, ADAK_BELGELER_STRUCT * belgeler_struct )
{
    SET_BELGE_STRUCTURES( belge_id, belgeler_struct );

    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_degiskenleri","belge_degisken_id","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id" ,tasarim_id);
    if(query.SELECT() NE 0) {
        GET_SAVED_VARIABLE_INFO(tasarim_id);
    }

    SET_BELGE_ID  ( belge_id );
    SET_TASARIM_ID( tasarim_id );

}

/**************************************************************************************
                   SET_BELGE_STRUCTURES
***************************************************************************************/

void SET_BELGE_STRUCTURES ( int index, ADAK_BELGELER_STRUCT * belgeler_struct )
{
    g_adak_belgeler_struct = belgeler_struct;

    g_header_struct      = g_adak_belgeler_struct[index].header_struct;
    g_header_struct_size = g_adak_belgeler_struct[index].header_struct_size;
    g_footer_struct      = g_adak_belgeler_struct[index].footer_struct;
    g_footer_struct_size = g_adak_belgeler_struct[index].footer_struct_size;
    g_line_struct        = g_adak_belgeler_struct[index].line_struct;
    g_line_struct_size   = g_adak_belgeler_struct[index].line_struct_size;

    g_belge_adi = g_adak_belgeler_struct[index].belge_adi;
    g_belge_id  = g_adak_belgeler_struct[index].belge_id;
}

/**************************************************************************************
                   GET_BELGE_HEADER_STRUCT_SIZE
***************************************************************************************/

int GET_BELGE_HEADER_STRUCT_SIZE()
{
    return g_header_struct_size;
}

/**************************************************************************************
                   GET_BELGE_FOOTER_STRUCT_SIZE
***************************************************************************************/

int GET_BELGE_FOOTER_STRUCT_SIZE()
{
    return g_footer_struct_size;
}

/**************************************************************************************
                   GET_BELGE_LINE_STRUCT_SIZE
***************************************************************************************/

int GET_BELGE_LINE_STRUCT_SIZE()
{
    return g_line_struct_size;
}

/**************************************************************************************
                   GET_BELGE_HEADER_STRUCT
***************************************************************************************/

ADAK_BELGE_DEFAULTS_STRUCT * GET_BELGE_HEADER_STRUCT()
{
    return g_header_struct;
}

/**************************************************************************************
                   GET_BELGE_FOOTER_STRUCT
***************************************************************************************/

ADAK_BELGE_DEFAULTS_STRUCT * GET_BELGE_FOOTER_STRUCT()
{
    return g_footer_struct;
}

/**************************************************************************************
                   GET_BELGE_LINE_STRUCT
***************************************************************************************/

ADAK_BELGE_DEFAULTS_STRUCT * GET_BELGE_LINE_STRUCT()
{
    return g_line_struct;
}
/**************************************************************************************
                   GET_KULLANICI_DEGISKENLERI
***************************************************************************************/

void GET_KAYITLI_DEGISKENLER ( QList< TASARIM_DEGISKENI *> *P_VARIABLE )
{
    //! static degiskeni sifirlamasi icin
    TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(false);
    degisken->RESET_STATIC_DEGISKEN();
    delete degisken;

    SQL_QUERY query (G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_degiskenleri",
                         "belge_degisken_id, degisken_id, font_size, align, pos_x, pos_y, is_visible_variable, "
                         "grup_enum, size_vertical, size_horizontal, pixmap, text, text_size, "
                         "monospace_mi, is_bold, is_under_line, is_italic, is_strikeout, font_family, is_nlq, dot_matrix_mode, satir, soldan_bosluk ",
                         "tasarim_id =:tasarim_id ", "" );

    query.SET_VALUE(":tasarim_id",GET_TASARIM_ID());

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    while( query.NEXT() EQ true ) {

        TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(true);

        degisken->SET_TASARIM_ID    ( GET_TASARIM_ID() );
        degisken->SET_DEGISKEN_ID   ( query.VALUE("degisken_id").toInt() );
        degisken->SET_GRUP          ( query.VALUE("grup_enum").toInt() );
        degisken->SET_BELGE_DEGISKEN_ID( query.VALUE("belge_degisken_id").toInt() );
        degisken->SET_FONT_SIZE     ( query.VALUE("font_size").toInt() );
        degisken->SET_ALIGN         ( query.VALUE("align").toInt() );
        degisken->SET_VISIBLE       ( query.VALUE("is_visible_variable").toBool() );
        degisken->SET_POS_X         ( query.VALUE("pos_x").toDouble());
        degisken->SET_POS_Y         ( query.VALUE("pos_y").toDouble());
        degisken->SET_SIZE_X        ( query.VALUE("size_horizontal").toInt());
        degisken->SET_SIZE_Y        ( query.VALUE("size_vertical").toInt());
        degisken->SET_IMAGES        ( ADAK_BYTE_ARRAY_2_PIXMAP( query.VALUE("pixmap").toByteArray() ) );
        degisken->SET_TEXT          ( query.VALUE("text").toString()   );
        degisken->SET_TEXT_SIZE     ( query.VALUE("text_size").toInt() );
        degisken->SET_NLQ           ( query.VALUE("is_nlq").toBool() );
        degisken->SET_DOT_MARIX_MODE( query.VALUE("dot_matrix_mode").toInt() );

        if ( query.VALUE("text").toString().contains( ".png" ) EQ true ) {
            degisken->SET_IMAGES( QPixmap( query.VALUE("text").toString() ) );
        }
        if ( degisken->GET_GRUP() EQ USER_TEXT OR degisken->GET_GRUP() EQ USER_IMG ) {
            degisken->SET_DEGISKEN_TYPE( USER_VARIABLE );
        }
        else {
            degisken->SET_DEGISKEN_TYPE(TASARIMCI_VARIABLE);
            degisken->SET_DISPLAY_KARAKTER("x");
        }

        degisken->SET_MONOSPACE_MI  ( query.VALUE("monospace_mi").toBool() );
        degisken->SET_BOLD          ( query.VALUE("is_bold").toBool() );
        degisken->SET_UNDER_LINE    ( query.VALUE("is_under_line").toBool() );
        degisken->SET_ITALIC        ( query.VALUE("is_italic").toBool() );
        degisken->SET_STRIKEOUT     ( query.VALUE("is_strikeout").toBool() );
        degisken->SET_FONT_FAMILY   ( query.VALUE("font_family").toString() );
        degisken->SET_SATIR         ( query.VALUE("satir").toInt() );
        degisken->SET_SOLDAN_BOSLUK ( query.VALUE("soldan_bosluk").toInt() );

        P_VARIABLE->append( degisken );
    }
}

/**************************************************************************************
                   SET_BELGE_STRUCTURES
***************************************************************************************/

void SET_BELGE_STRUCTURES (ADAK_BELGE_DEFAULTS_STRUCT * header_struct, int header_struct_size,
                           ADAK_BELGE_DEFAULTS_STRUCT * footer_struct, int footer_struct_size,
                           ADAK_BELGE_DEFAULTS_STRUCT * line_struct,   int line_struct_size) {

    g_header_struct      = header_struct;
    g_header_struct_size = header_struct_size;
    g_footer_struct      = footer_struct;
    g_footer_struct_size = footer_struct_size;
    g_line_struct        = line_struct;
    g_line_struct_size   = line_struct_size;
}

/**************************************************************************************
                   GET_BELGE_ROW_COUNT
***************************************************************************************/

int GET_BELGE_PRINT_ROW_COUNT()
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","row_count","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id", GET_TASARIM_ID() );

    if ( query.SELECT() EQ 0 ) {
        return 1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   GET_BELGE_ROW_COUNT
***************************************************************************************/

int GET_BELGE_ROW_COUNT()
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","belge_satir_sayisi","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id", GET_TASARIM_ID() );

    if ( query.SELECT() EQ 0 ) {
        return 1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   GET_BELGE_PRINTER_TYPE
***************************************************************************************/

int GET_BELGE_PRINTER_TYPE()
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","printer_type","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id", GET_TASARIM_ID());

    if ( query.SELECT() EQ 0 ) {
        return 1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   GET_BELGE_ROW_SPACE
***************************************************************************************/

double GET_BELGE_ROW_SPACE_MM()
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","row_space_mm","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id",GET_TASARIM_ID());

    if ( query.SELECT() EQ 0 ) {
        return 1.00;
    }

    query.NEXT();

    return query.VALUE(0).toDouble();
}

/**************************************************************************************
                   GET_BELGE_ROW_SPACE_SATIR
***************************************************************************************/

int GET_BELGE_ROW_SPACE_SATIR()
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","row_space_satir","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id",GET_TASARIM_ID());

    if ( query.SELECT() EQ 0 ) {
        return 1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   GET_BELGE_BOYUT_BILGILERI_SATIR
***************************************************************************************/

void GET_BELGE_BOYUT_BILGILERI_SATIR(int * belge_satir_sayisi,int * belge_toplam_karakter_sayisi)
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","belge_satir_sayisi,belge_toplam_karakter_sayisi","tasarim_id = :tasarim_id","");
    query.SET_VALUE(":tasarim_id",GET_TASARIM_ID());

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    *belge_satir_sayisi           = query.VALUE(0).toInt();
    *belge_toplam_karakter_sayisi = query.VALUE(1).toInt();
}


/**************************************************************************************
                   GET_BELGE_ID
***************************************************************************************/

int GET_BELGE_ID()
{
    return g_belge_id;
}

/**************************************************************************************
                   GET_BELGE_ADI
***************************************************************************************/

QString GET_BELGE_ADI()
{
    return g_belge_adi;
}

/**************************************************************************************
                   GET_TASARIM_ID
***************************************************************************************/

int GET_TASARIM_ID()
{
    return g_tasarim_id;
}

/**************************************************************************************
                   GET_TASARIM_ADI
***************************************************************************************/

QString GET_TASARIM_ADI()
{
    return g_tasarim_adi;
}

/**************************************************************************************
                   GET_IS_TASARIM_READONLY
***************************************************************************************/

bool GET_IS_TASARIM_READONLY()
{
    return g_tasarim_readonly;
}

/**************************************************************************************
                   SET_BELGE_ID
***************************************************************************************/

void SET_BELGE_ID (int belge_id)
{
    g_belge_id = belge_id;
}

/**************************************************************************************
                   SET_BELGE_ADI
***************************************************************************************/

void SET_BELGE_ADI(QString belge_adi)
{
    g_belge_adi = belge_adi;
}

/**************************************************************************************
                   SET_TASARIM_ID
***************************************************************************************/

void SET_TASARIM_ID(int tasarim_id)
{
    g_tasarim_id = tasarim_id;
}

/**************************************************************************************
                   SET_TASARIM_ADI
***************************************************************************************/

void SET_TASARIM_READONLY_VALUE(int value)
{
    if ( value EQ 0 ) {
        g_tasarim_readonly = false;
        return;
    }

    g_tasarim_readonly = true;
}

/**************************************************************************************
                   SET_TASARIM_ADI
***************************************************************************************/

void SET_TASARIM_ADI(QString tasarim_adi)
{
    g_tasarim_adi = tasarim_adi;
}


/**************************************************************************************
                   DEGISLENLERI_SIRALA
***************************************************************************************/

void DEGISLENLERI_SIRALA(QList<TASARIM_DEGISKENI *> *p_list)
{

    //! AYNI SATIRDAKI DEGISKENLERI SIRALA ASC

    for (int i = 0; i < p_list->size(); ++i) {
        TASARIM_DEGISKENI *ek_degisken = p_list->at(i);
        for (int j = i+1; j < p_list->size(); ++j) {
            if (p_list->at(j)->GET_POS_X() < ek_degisken->GET_POS_X() ) {
                ek_degisken = p_list->at(j);
                p_list->move(j,i);
            }
        }
    }

}
