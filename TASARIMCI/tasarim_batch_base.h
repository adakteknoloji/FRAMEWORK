#ifndef TASARIM_BATCH_BASE_H
#define TASARIM_BATCH_BASE_H

#include "batch_kernel.h"
#include "tasarim_design_widget.h"
#include "belge_struct.h"

class QGroupBox;
class QDoubleSpinBox;
class TASARIM_DEGISKENI;

class TASARIM_BATCH_BASE : public BATCH_KERNEL
{
    Q_OBJECT

public:
    TASARIM_BATCH_BASE(QWidget * parent);

protected:
    int                             m_tasarim_id;
    int                             m_printer_type;

    double                          m_fare_hassasiyeti;

    ADAK_BELGE_DEFAULTS_STRUCT *   m_header_struct;
    int                             m_header_struct_size;
    ADAK_BELGE_DEFAULTS_STRUCT *   m_footer_struct;
    int                             m_footer_struct_size;
    ADAK_BELGE_DEFAULTS_STRUCT *   m_line_struct;
    int                             m_line_struct_size;

    TASARIM_DESIGN_WIDGET *         m_design_widget;

    QList< TASARIM_DEGISKENI *>     m_variable; // sadece secim ekraninda secilmek icin turuluyor.

    QList<TASARIM_DEGISKENI*>       m_all_variables;

    QList<int>                      m_silinecek_degiskenler;

    TASARIM_DEGISKENI *             GET_LABEL_DEGISKEN( QLabel *p_label );
    TASARIM_DEGISKENI *             GET_DEGISKEN( int p_degisken_id );

    TASARIM_DEGISKENI *             GET_DEGISKEN( int p_grup, int p_index );

    QList<TASARIM_DEGISKENI*>       GET_LINE_VARIABLE_LIST();

    void                            SAYFA_YAZI_BUTONU_EKLE  ( QPushButton * p_button );
    void                            SAYFA_RESIM_BUTONU_EKLE ( QPushButton * p_button, int printer_type );
    void                            SAYFA_SIL_BUTONU_EKLE   ( QPushButton * p_button );

    void                            SET_USER_VARIABLE_POS   ( double x, double y, int index, QDoubleSpinBox * comma_edit_x , QDoubleSpinBox * comma_edit_y);

    QString                         GET_PIXMAP_FILE_NAME    ( QString image_path );

    QPushButton *                   m_push_button_text_ekle;
    QPushButton *                   m_push_button_resim_ekle;
    QPushButton *                   m_push_button_sil;

protected slots:
    void                            SLOT_RESIM_EKLE_BUTTON_CLICKED();
    void                            SLOT_TEXT_EKLE_BUTTON_CLICKED();
    void                            SLOT_SECILI_DEGISKENI_SIL();

signals:
    void                            SIGNAL_VARIABLE_DELETED( QLabel * );

};

#endif // TASARIM_BATCH_BASE_H
