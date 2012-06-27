#ifndef TASARIM_BATCH_H
#define TASARIM_BATCH_H

#include <QDialog>
#include "belge_kernel_defines.h"
#include "tasarim_batch_base.h"
#include "belge_struct.h"

namespace Ui {
    class TASARIM_BATCH;
}

    class TASARIM_BATCH : public TASARIM_BATCH_BASE
{
    Q_OBJECT

public:
    TASARIM_BATCH (int belge_width,double belge_height,int tasarim_id, int p_printer_type, QWidget * parent = 0);
    ~TASARIM_BATCH () {}

    void  SET_DESIGN_WIDGET  (double oran = 1.00);
    void  SET_DEFAULT_VALUES ();

    bool  eventFilter(QObject * p_obj, QEvent * p_event);

protected:
    void SETUP_FORM             ();
    int  CHECK_VAR              (QObject * object);
    int  CHECK_RUN              ();
    void RUN_BATCH              ();
    void SAVER_BUTTON_CLICKED   (QAbstractButton * button);
    int  CHANGER_BUTTON_CLICKED (QAbstractButton *button);
    void FILL_TASARIM_VARIABLE  ();

    int  CHECK_EXIT             ();

public slots:
    void CREATE_DESIGN_WIDGET   ();
    void SLOT_RUN_BATCH();

private slots:
    void MOUSE_VARIABLE_POSITION        ( int, double x, double y);
    void SET_VARIABLE_DEFAULTS        (QLabel * label);
    void SET_SELECTED_VARIABLE        (QLabel * label);
    void SLOT_CHANGE_FONT_SIZE        ();
    void SLOT_X_AND_Y_CHANGE          ( double p_value );
    void SLOT_KARAKTER_UZUNLUGU_CHANGE();
    void SLOT_DEGISKEN_DISPLAY_CHANGE ( QString p_value );
    void SLOT_IMAGE_HEIGHT_AND_WEIGHT (int p_height );
    void SLOT_KEY_DIRECTION_PRESS     ( Qt::Key p_key );
    void SLOT_VARIABLE_UNSELECTED     ();

    void SLOT_MONOSPACE_MI            ();

    void SLOT_NLQ                     ();
    void SLOT_DOT_MATRIX_MODE         ( bool p_state );

    //! DOT_MATRIX
    void SLOT_CHANGE_SOL_BOSLUK_USTEN_SATIR();
    void SLOT_CHANGE_SOL_BOSLUK();

private:
    double m_belge_height;
    double m_belge_width;

    bool   m_slot_change_sol_bosluk_enable;

    //! DOT_MATRIX
    int    m_belge_satir_sayisi;
    int    m_belge_toplam_karakter_sayisi;

    void  GET_BELGE_BOYUT_BILGILERI_SATIR();
    void  CONSOLE_POS_SELECTED_LABEL_DOT_MATRIX(double x = -1,double y = -1,QLabel * label = NULL);

    int   CONVERT_SOL_BOSLUK_PX_X   (int satirdaki_bosluk_sayisi );
    int   CONVERT_SATIR_BOSLUK_PX_Y (int satir);
    void  MOUSE_VARIABLE_POSITION_DOT_MATRIX( int p_degisken_id, double x, double y);
    void  SET_DEGISKEN_POS_STRUCT_DOT_MATRIX ( ADAK_BELGE_DEFAULTS_STRUCT *belge_struct, int index, double pos_x, double pos_y );
    //! END

    TASARIMCI_MODU  m_tasarimci_modu;

    ALIGN_ELEMENTS  m_align;

    bool   m_design_changed;

    Ui::TASARIM_BATCH *   m_ui;

    void SET_ALING_BUTTON_STATE  ( ALIGN_ELEMENTS p_align );

    void SET_BOLD_ITALIC_UNDER_LINE_STATE();

    void CONSOLE_POS_SELECTED_LABEL  (double x = -1,double y = -1, QLabel * label = NULL);

    void SET_DEGISKEN_POS        (ADAK_BELGE_DEFAULTS_STRUCT * belge_struct,int index, double pos_x,double pos_y,bool mouse_hassasiyeti_aktif = false);
    void SET_DEGISKEN_POS_STRUCT (ADAK_BELGE_DEFAULTS_STRUCT * belge_struct,int index, double pos_x,double pos_y);

    int  CONVERT_MMX_TO_PX_X     (double mmx);
    int  CONVERT_MMY_TO_PX_Y     (double mmy);

    void SAVE_ALL_VARIABLES      ();

    void HIDE_LASER_WIDGETS();

    void FIND_SATIR_AND_BOSLUK   (int p_degisken_index, double mm_x, double mm_y );

    double MM_X2PX_X             ( double p_mm_x);
    double MM_Y2PX_Y             ( double p_mm_y);

    double PX_X2MM_X             ( double p_mm_x);

    int    FONT2PX_X             ( QString p_str, int p_font_size, int p_pre_space_size );
    int    FONT2PX_Y             ( int p_font_size );

    void   SATIR_BOSLUKLARINI_HESAPLA( int p_degisken_index );

    int    SOLDAN_BOSLUK_HESAPLA ( int p_degisken_index, int p_satir, double p_x );

};

#endif // TASARIM_BATCH_H
