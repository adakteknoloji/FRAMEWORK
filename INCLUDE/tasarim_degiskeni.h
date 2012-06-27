#ifndef TASARIM_DEGISKENI_H
#define TASARIM_DEGISKENI_H

#include <QLabel>

class QString;
class QPixmap;

enum USER_VARIABLE_TYPE {
    USER_TEXT_VARIABLE   = 1,
    USER_PIXMAP_VARIABLE = 2
};

enum DEGISKEN_TYPE {
    TASARIMCI_VARIABLE = 0,
    USER_VARIABLE      = 1
};

class TASARIM_DEGISKENI
{

private:

    static int s_degisken_count;

    QLabel *m_label;

    int     m_degisken_id;
    int     m_belge_degisken_id;

    //! primary_id degiskeni olusan her degisken isin
    //! eger constructor da true gonderilmis ise ona bir id verir.
    int     m_primary_id;

    QString m_text;
    int     m_text_size;
    QString m_kodu;

    int     m_grup;
    int     m_tasarim_id;
    QPixmap m_images;

    // PROPERTY
    int     m_font_size;
    int     m_align;
    bool    m_is_visible;
    double  m_pos_x;
    double  m_pos_y;
    int     m_size_x;
    int     m_size_y;
    bool    m_monospace_mi;
    bool    m_is_silinecek;

    DEGISKEN_TYPE m_degisken_type;

    int     m_display_int;
    QString m_display_karakter;


    QString m_font_family;
    bool    m_is_bold;
    bool    m_is_under_line;
    bool    m_is_italic;
    bool    m_is_strikeout;

    //! DOT_MATRIX_PRINTER
    bool    m_is_nlq;
    int     m_dot_matrix_mode;

    int     m_satir;
    int     m_soldan_bosluk;

public:

    TASARIM_DEGISKENI( bool p_primary_id_enable );

    void    SET_LABEL( QLabel *p_label );
    QLabel* GET_LABEL();

    void    SET_TEXT( QString p_name );
    QString GET_TEXT();

    void    SET_KODU( QString p_kodu );
    QString GET_KODU();

    void    SET_GRUP( int p_grup );
    int     GET_GRUP();

    int     GET_PRIMARY_COUNT();
    int     GET_PRIMARY_ID();

    void    SET_BELGE_DEGISKEN_ID( int p_id );
    int     GET_BELGE_DEGISKEN_ID();

    void    SET_TASARIM_ID( int p_tasarim_id );
    int     GET_TASARIM_ID();

    void    SET_IMAGES( QPixmap p_images );
    QPixmap GET_IMAGES();

    void    SET_FONT_SIZE( int p_font_size );
    int     GET_FONT_SIZE();

    void    SET_ALIGN( int p_align );
    int     GET_ALIGN();

    void    SET_VISIBLE( bool p_visible );
    bool    IS_VISIBLE();

    void    SET_POS_X( double p_pos_x );
    double  GET_POS_X();

    void    SET_POS_Y( double p_pos_y );
    double  GET_POS_Y();

    void    SET_SIZE_X( int p_size_x );
    int     GET_SIZE_X();

    void    SET_SIZE_Y( int p_size_y );
    int     GET_SIZE_Y();

    void    SET_MONOSPACE_MI( bool p_monospace_mi );
    bool    IS_MONOSPACE_MI();

    int     GET_DEGISKEN_COUNT();

    void    SET_DEGISKEN_TYPE( DEGISKEN_TYPE p_type );
    DEGISKEN_TYPE GET_DEGISKEN_TYPE();

    void    SET_SILINECEK( bool p_value );
    bool    IS_SILINECEK();

    void    SET_TEXT_SIZE( int p_text_size );
    int     GET_TEXT_SIZE();

    void    COPY(TASARIM_DEGISKENI * p_degisken );

    void    SET_DISPLAY_INT( int p_display_int );
    int     GET_DISPLAY_INT();

    void    SET_DISPLAY_KARAKTER( QString p_display_txt );
    QString GET_DISPLAY_TXT();

    void    SET_DEGISKEN_ID( int p_degisken_id );
    int     GET_DEGISKEN_ID();

    void    RESET_STATIC_DEGISKEN();

    void    SET_FONT_FAMILY( QString p_font_family );
    QString GET_FONT_FAMILY();

    void    SET_BOLD( bool p_bold );
    bool    IS_BOLD();

    void    SET_UNDER_LINE( bool p_under_line );
    bool    IS_UNDER_LINE();

    void    SET_ITALIC( bool p_italic );
    bool    IS_ITALIC();

    void    SET_STRIKEOUT( bool p_strikeout );
    bool    IS_STRIKEOUT();

    //! dot_matrix
    void    SET_NLQ( bool p_is_nlq );
    bool    IS_NLQ();

    void    SET_DOT_MARIX_MODE( int p_mode );
    int     GET_DOT_MARIX_MODE();

    void    SET_SOLDAN_BOSLUK( int p_bosluk_sayisi );
    int     GET_SOLDAN_BOSLUK();

    void    SET_SATIR( int p_satir );
    int     GET_SATIR();


private:
    void    SET_PRIMARY_ID( int p_primary_id );

};

#endif // TASARIM_DEGISKENI_H
