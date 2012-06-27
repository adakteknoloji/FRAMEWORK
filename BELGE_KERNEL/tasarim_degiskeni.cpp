#include <QString>
#include <QPixmap>
#include <QLabel>
#include "adak_defines.h"
#include "belge_kernel_defines.h"
#include "tasarim_degiskeni.h"

/**************************************************************************************
                   TASARIM_DEGISKENI::TASARIM_DEGISKENI
***************************************************************************************/

int TASARIM_DEGISKENI::s_degisken_count = -1;

TASARIM_DEGISKENI::TASARIM_DEGISKENI(bool p_primary_id_enable)
{
    m_text       = "adız değişken";
    m_belge_degisken_id = -1;
    m_grup       = USER_TEXT;
    m_tasarim_id = -1;
    m_degisken_id= -1;
    m_primary_id = -1;

    // default values
    m_font_size = 13;
    m_align     = LEFT_ALIGN;
    m_is_visible= 1;
    m_pos_x     = 0.00;
    m_pos_y     = 0.00;
    m_size_x    = 0;
    m_size_y    = 0;

    m_font_family   = "Arial";
    m_is_bold       = false;
    m_is_italic     = false;
    m_is_under_line = false;
    m_is_strikeout  = false;

    m_monospace_mi   = false;
    m_degisken_type  = USER_VARIABLE;
    m_is_silinecek   = false;

    m_text.clear();
    m_text_size      = 0;

    m_display_int    = 8;
    m_display_karakter = "X";

    m_is_nlq         = false;
    m_dot_matrix_mode= 0;

    m_satir          = 0;
    m_soldan_bosluk  = 0;

    if ( p_primary_id_enable EQ true ) {
        s_degisken_count++;

        SET_PRIMARY_ID( s_degisken_count );
    }
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_LABEL
***************************************************************************************/

void TASARIM_DEGISKENI::SET_LABEL(QLabel *p_label)
{
    m_label = p_label;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_LABEL
***************************************************************************************/

QLabel * TASARIM_DEGISKENI::GET_LABEL()
{
    return m_label;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_NAME
***************************************************************************************/

void TASARIM_DEGISKENI::SET_TEXT( QString p_name )
{
    m_text = p_name;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_NAME
***************************************************************************************/

QString TASARIM_DEGISKENI::GET_TEXT()
{
    return m_text;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_NAME
***************************************************************************************/

void TASARIM_DEGISKENI::SET_GRUP( int p_grup )
{
    m_grup = p_grup;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_NAME
***************************************************************************************/

int TASARIM_DEGISKENI::GET_GRUP()
{
    return m_grup;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_ID
***************************************************************************************/

void TASARIM_DEGISKENI::SET_BELGE_DEGISKEN_ID( int p_id )
{
    m_belge_degisken_id = p_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_ID
***************************************************************************************/

int TASARIM_DEGISKENI::GET_BELGE_DEGISKEN_ID()
{
    return m_belge_degisken_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_TASARIM_ID
***************************************************************************************/

void TASARIM_DEGISKENI::SET_TASARIM_ID( int p_tasarim_id )
{
    m_tasarim_id = p_tasarim_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_TASARIM_ID
***************************************************************************************/

int TASARIM_DEGISKENI::GET_TASARIM_ID()
{
    return m_tasarim_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_IMG_PATH
***************************************************************************************/

void TASARIM_DEGISKENI::SET_IMAGES( QPixmap p_images )
{
    m_images = p_images;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_IMG_PATH
***************************************************************************************/

QPixmap TASARIM_DEGISKENI::GET_IMAGES()
{
    return m_images;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_FONT_SIZE
***************************************************************************************/

void TASARIM_DEGISKENI::SET_FONT_SIZE( int p_font_size )
{
    m_font_size = p_font_size;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_FONT_SIZE
***************************************************************************************/

int TASARIM_DEGISKENI::GET_FONT_SIZE()
{
    return m_font_size;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_ALIGN
***************************************************************************************/

void TASARIM_DEGISKENI::SET_ALIGN( int p_align )
{
    m_align = p_align;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_ALIGN
***************************************************************************************/

int TASARIM_DEGISKENI::GET_ALIGN()
{
    return m_align;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_VISIBLE
***************************************************************************************/

void TASARIM_DEGISKENI::SET_VISIBLE( bool p_visible )
{
    m_is_visible = p_visible;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_VISIBLE
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_VISIBLE()
{
    return m_is_visible;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_POS_X
***************************************************************************************/

void TASARIM_DEGISKENI::SET_POS_X( double p_pos_x )
{
    m_pos_x = p_pos_x;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_POS_X
***************************************************************************************/

double TASARIM_DEGISKENI::GET_POS_X()
{
    return m_pos_x;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_POS_Y
***************************************************************************************/

void TASARIM_DEGISKENI::SET_POS_Y( double p_pos_y )
{
    m_pos_y = p_pos_y;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_POS_Y
***************************************************************************************/

double TASARIM_DEGISKENI::GET_POS_Y()
{
    return m_pos_y;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_SIZE_X
***************************************************************************************/

void TASARIM_DEGISKENI::SET_SIZE_X( int p_size_x )
{
    m_size_x = p_size_x;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_SIZE_X
***************************************************************************************/

int TASARIM_DEGISKENI::GET_SIZE_X()
{
    return m_size_x;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_SIZE_Y
***************************************************************************************/

void TASARIM_DEGISKENI::SET_SIZE_Y( int p_size_y )
{
    m_size_y = p_size_y;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_SIZE_Y
***************************************************************************************/

int TASARIM_DEGISKENI::GET_SIZE_Y()
{
    return m_size_y;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_MONOSPACE_MI
***************************************************************************************/

void TASARIM_DEGISKENI::SET_MONOSPACE_MI( bool p_monospace_mi )
{
    m_monospace_mi = p_monospace_mi;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_MONOSPACE_MI
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_MONOSPACE_MI()
{
    return m_monospace_mi;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_KODU
***************************************************************************************/

void TASARIM_DEGISKENI::SET_KODU( QString p_kodu )
{
    m_kodu = p_kodu;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_KODU
***************************************************************************************/

QString TASARIM_DEGISKENI::GET_KODU()
{
    return m_kodu;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_DEGISKEN_TYPE
***************************************************************************************/

void TASARIM_DEGISKENI::SET_DEGISKEN_TYPE(DEGISKEN_TYPE p_type)
{
    m_degisken_type = p_type;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_DEGISKEN_TYPE
***************************************************************************************/

DEGISKEN_TYPE TASARIM_DEGISKENI::GET_DEGISKEN_TYPE()
{
    return m_degisken_type;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_SILINECEK
***************************************************************************************/

void TASARIM_DEGISKENI::SET_SILINECEK(bool p_value)
{
    m_is_silinecek = p_value;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_SILINECEK
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_SILINECEK()
{
    return m_is_silinecek;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_TEXT_SIZE
***************************************************************************************/

void TASARIM_DEGISKENI::SET_TEXT_SIZE(int p_text_size)
{
    m_text_size = p_text_size;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_TEXT_SIZE
***************************************************************************************/

int TASARIM_DEGISKENI::GET_TEXT_SIZE()
{
    return m_text_size;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::CLONE
***************************************************************************************/

void TASARIM_DEGISKENI::COPY( TASARIM_DEGISKENI * p_degisken )
{ 
    p_degisken->SET_ALIGN( m_align );
    p_degisken->SET_DEGISKEN_TYPE( m_degisken_type );
    p_degisken->SET_FONT_SIZE( m_font_size );
    p_degisken->SET_GRUP( m_grup );
    p_degisken->SET_IMAGES( m_images);
    p_degisken->SET_KODU( m_kodu);
    p_degisken->SET_LABEL( m_label);
    p_degisken->SET_MONOSPACE_MI( m_monospace_mi);
    p_degisken->SET_POS_X( m_pos_x);
    p_degisken->SET_POS_Y( m_pos_y);
    p_degisken->SET_SILINECEK(m_is_silinecek);
    p_degisken->SET_SIZE_X( m_size_x);
    p_degisken->SET_SIZE_Y(m_size_y);
    p_degisken->SET_TASARIM_ID(m_tasarim_id);
    p_degisken->SET_TEXT( m_text);
    p_degisken->SET_TEXT_SIZE( m_text_size);
    p_degisken->SET_VISIBLE( m_is_visible);
    p_degisken->SET_DEGISKEN_ID( m_degisken_id );
    p_degisken->SET_FONT_FAMILY(m_font_family);
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_DISPLAY_INT
***************************************************************************************/

void TASARIM_DEGISKENI::SET_DISPLAY_INT(int p_display_int)
{
    m_display_int = p_display_int;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_DISPLAY_INT
***************************************************************************************/

int TASARIM_DEGISKENI::GET_DISPLAY_INT()
{
    return m_display_int;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_DISPLAY_TXT
***************************************************************************************/

void TASARIM_DEGISKENI::SET_DISPLAY_KARAKTER(QString p_display_txt)
{
    m_display_karakter = p_display_txt;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_DISPLAY_TXT
***************************************************************************************/

QString TASARIM_DEGISKENI::GET_DISPLAY_TXT()
{
    QString display_txt;
    for (int i = 0; i < m_text_size; ++i) {
        display_txt.append( m_display_karakter );
    }
    return display_txt;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_PRIMARY_COUNT
***************************************************************************************/

int TASARIM_DEGISKENI::GET_PRIMARY_COUNT()
{
    return s_degisken_count;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_PRIMARY_ID
***************************************************************************************/

int TASARIM_DEGISKENI::GET_PRIMARY_ID()
{
    return m_primary_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_PRIMARY_ID
***************************************************************************************/

void TASARIM_DEGISKENI::SET_PRIMARY_ID(int p_primary_id)
{
    m_primary_id = p_primary_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_DEGISKEN_ID
***************************************************************************************/

void TASARIM_DEGISKENI::SET_DEGISKEN_ID(int p_degisken_id)
{
    m_degisken_id = p_degisken_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_DEGISKEN_ID
***************************************************************************************/

int TASARIM_DEGISKENI::GET_DEGISKEN_ID()
{
    return m_degisken_id;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::RESET_STATIC_DEGISKEN
***************************************************************************************/

void TASARIM_DEGISKENI::RESET_STATIC_DEGISKEN()
{
    s_degisken_count = -1;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_BOLD
***************************************************************************************/

void TASARIM_DEGISKENI::SET_BOLD( bool p_bool )
{
    m_is_bold = p_bool;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_BOLD
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_BOLD()
{
    return m_is_bold;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_UNDER_LINE
***************************************************************************************/

void TASARIM_DEGISKENI::SET_UNDER_LINE( bool p_under_line )
{
    m_is_under_line = p_under_line;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_UNDER_LINE
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_UNDER_LINE()
{
    return m_is_under_line;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_ITALIC
***************************************************************************************/

void TASARIM_DEGISKENI::SET_ITALIC( bool p_italic )
{
    m_is_italic = p_italic;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_ITALIC
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_ITALIC()
{
    return m_is_italic;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_STRIKEOUT
***************************************************************************************/

void TASARIM_DEGISKENI::SET_STRIKEOUT(bool p_strikeout)
{
    m_is_strikeout = p_strikeout;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_STRIKEOUT
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_STRIKEOUT()
{
    return m_is_strikeout;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_FONT_FAMILY
***************************************************************************************/

void TASARIM_DEGISKENI::SET_FONT_FAMILY(QString p_font_family)
{
    m_font_family = p_font_family;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_FONT_FAMILY
***************************************************************************************/

QString TASARIM_DEGISKENI::GET_FONT_FAMILY()
{
    return m_font_family;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_NLQ
***************************************************************************************/

void TASARIM_DEGISKENI::SET_NLQ(bool p_is_nlq)
{
    m_is_nlq = p_is_nlq;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::IS_NLQ
***************************************************************************************/

bool TASARIM_DEGISKENI::IS_NLQ()
{
    return m_is_nlq;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_DOT_MARIX_MODE
***************************************************************************************/
void TASARIM_DEGISKENI::SET_DOT_MARIX_MODE(int p_mode)
{
    m_dot_matrix_mode = p_mode;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_DOT_MARIX_MODE
***************************************************************************************/

int TASARIM_DEGISKENI::GET_DOT_MARIX_MODE()
{
    return m_dot_matrix_mode;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_SOLDAN_BOSLUK
***************************************************************************************/

void TASARIM_DEGISKENI::SET_SOLDAN_BOSLUK(int p_bosluk_sayisi)
{
    m_soldan_bosluk = p_bosluk_sayisi;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_SOLDAN_BOSLUK
***************************************************************************************/

int TASARIM_DEGISKENI::GET_SOLDAN_BOSLUK()
{
    return m_soldan_bosluk;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::SET_SATIR
***************************************************************************************/

void TASARIM_DEGISKENI::SET_SATIR(int p_satir)
{
    m_satir = p_satir;
}

/**************************************************************************************
                   TASARIM_DEGISKENI::GET_SATIR
***************************************************************************************/

int TASARIM_DEGISKENI::GET_SATIR()
{
    return m_satir;
}
