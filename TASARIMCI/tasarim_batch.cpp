#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QSlider>
#include <QFontDialog>
#include "tasarim_defines.h"
#include "tasarim_batch.h"
#include "adak_utils.h"
#include "ui_tasarim_batch.h"
#include "tasarim_utils.h"
#include "belge_kernel_utils.h"
#include "belge_kernel_open.h"
#include "belge_kernel_defines.h"
#include "print.h"
#include "tasarim.h"
#include "tasarim_degiskeni.h"
#include "belge_kernel_defines.h"
#include "adak_utils.h"

extern ADAK_SQL * G_YONETIM_DB;

#define NORMAL_MODE_CARPAN   1.000
#define LARGE_MODE_CARPAN    2.000
#define CONDANSE_MODE_CARPAN 0.597

/**************************************************************************************
                   OPEN_TASARIM_BATCH
***************************************************************************************/

void OPEN_TASARIM_BATCH ( double belge_genisligi , double belge_yuksekligi , int tasarim_id, int p_printer_type, QWidget * parent )
{
    Q_INIT_RESOURCE(tasarimci_icons);

    TASARIM_BATCH * F = new TASARIM_BATCH( belge_genisligi , belge_yuksekligi , tasarim_id, p_printer_type, parent );
    F->EXEC( FULL_SCREEN );
}

/**************************************************************************************
                   TASARIM_BATCH::TASARIM_BATCH
***************************************************************************************/

TASARIM_BATCH::TASARIM_BATCH(int belge_width, double belge_height, int tasarim_id, int p_printer_type, QWidget *parent) : TASARIM_BATCH_BASE (parent), m_ui(new Ui::TASARIM_BATCH)
{
    m_belge_height    = belge_height;
    m_belge_width     = belge_width;
    m_design_widget   = NULL;
    m_tasarim_id      = tasarim_id;
    m_design_changed  = false;
    m_tasarimci_modu  = DESIGNER_MODU;
    m_printer_type    = p_printer_type;
    m_slot_change_sol_bosluk_enable = true;

    m_ui->setupUi(this);

    START_BATCH_KERNEL(this, G_YONETIM_DB);
}

/**************************************************************************************
                   TASARIM_BATCH::SETUP_FORM
***************************************************************************************/

void TASARIM_BATCH::SETUP_FORM ()
{
    if ( m_printer_type EQ LAZER_PRINTERS ) {
        m_ui->label_soldan_dot_matrix->setVisible(false);
        m_ui->label_soldan_dot_matrix_krk->setVisible(false);
        m_ui->doubleSpinBox_soldan_bosluk->setVisible(false);

        m_ui->label_usten_dot_matrix->setVisible(false);
        m_ui->label_usten_dot_matrix_2->setVisible(false);
        m_ui->doubleSpinBox_satir_sayisi->setVisible(false);

        m_ui->checkBox_nlq_aktif_mi->setVisible(false);
        m_ui->radioButton_mode_condense->setVisible(false);
        m_ui->radioButton_mode_large->setVisible(false);
        m_ui->radioButton_mode_normal->setVisible(false);
    }
    else { // DOT_MATRIX_PRINTER
        //m_ui->checkBox_monospace_mi->setEnabled(false);
        HIDE_LASER_WIDGETS();
        GET_BELGE_BOYUT_BILGILERI_SATIR();

        m_ui->pushButton_resim_ekle->setEnabled(false);
        m_ui->doubleSpinBox_satir_sayisi->setEnabled(false);
        m_ui->doubleSpinBox_soldan_bosluk->setEnabled(false);
    }

    CREATE_DESIGN_WIDGET();
    GET_SAVED_VARIABLE_INFO( m_tasarim_id );

    SET_PAGE_TITLE( GET_TASARIM_ADI());
    SET_HELP_PAGE ( tr("document-designer_design-laser") );

    SET_FIRST_FOCUS_WIDGET(m_ui->pushButton_belge_ozellikleri );

    m_header_struct         = GET_BELGE_HEADER_STRUCT();
    m_header_struct_size    = GET_BELGE_HEADER_STRUCT_SIZE();
    m_footer_struct         = GET_BELGE_FOOTER_STRUCT();
    m_footer_struct_size    = GET_BELGE_FOOTER_STRUCT_SIZE();
    m_line_struct           = GET_BELGE_LINE_STRUCT();
    m_line_struct_size      = GET_BELGE_LINE_STRUCT_SIZE();

    //! TASARIMIN DEGISKENLERI OKUNUYOR.
    FILL_TASARIM_VARIABLE();

    //! KULLANICININ DEGISKENLERI OKUNDU
    GET_KAYITLI_DEGISKENLER( &m_all_variables );

    REGISTER_SAVER_BUTTON   ( m_ui->push_button_test_ciktisi );
    REGISTER_SAVER_BUTTON   ( m_ui->pushButton_belge_ozellikleri );
    REGISTER_SAVER_BUTTON   ( m_ui->pushButton_onizleme_modu );
    REGISTER_CHANGER_BUTTON ( m_ui->pushButton_bilgi_sec );

    m_ui->spinBox_font_size->setMinimum(1);
    m_ui->spinBox_karakter_sayisi->setMinimum(1);
    m_ui->spinBox_picture_height->setMinimum(1);
    m_ui->spinBox_picture_width->setMinimum(1);

    m_ui->label_degisken_adi->clear();

    m_align = LEFT_ALIGN;

    connect( m_ui->pushButton_kaydet, SIGNAL( clicked()), this, SLOT( SLOT_RUN_BATCH()));
    connect( m_ui->comboBox_tasarim_yuzdesi, SIGNAL( currentIndexChanged(int) ), this, SLOT( CREATE_DESIGN_WIDGET() ) );
    connect( this, SIGNAL(SIGNAL_VARIABLE_DELETED(QLabel*)), this, SLOT( SLOT_VARIABLE_UNSELECTED()));


    SAYFA_YAZI_BUTONU_EKLE (m_ui->pushButton_text_ekle );
    SAYFA_RESIM_BUTONU_EKLE(m_ui->pushButton_resim_ekle, LAZER_PRINTERS);
    SAYFA_SIL_BUTONU_EKLE  (m_ui->pushButton_sil_laser );

    // %75 setlendi  ve OPEN_TASARIM_BATCH 'te CREATE_DESIGN_WIDGET( int ) setlenmeli
    m_ui->comboBox_tasarim_yuzdesi->setCurrentIndex( 3 ); // %75

    if ( GET_IS_TASARIM_READONLY() EQ true ) {
        m_ui->frame_design_widget->setDisabled(true);
        m_ui->pushButton_kaydet->setDisabled( true );
        m_ui->frame->setEnabled(false);
    }

    m_ui->pushButton_kaydet->setIcon( QPixmap( ":/kernel_icons/kernel_kaydet_ikonu.png" ) );
    m_ui->push_button_test_ciktisi->setIcon(QPixmap( ":/kernel_icons/kernel_yazdir_ikonu.png" ));

    m_ui->groupBox_laser->setEnabled(false);

}

/**************************************************************************************
                   TASARIM_BATCH::CHECK_VAR
***************************************************************************************/

int TASARIM_BATCH::CHECK_VAR(QObject * object)
{
    if ( m_design_widget->GET_LAST_MOVE_LABEL() EQ NULL ) {
        return ADAK_FAIL;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    if ( object EQ m_ui->pushButton_text_left OR object EQ m_ui->pushButton_text_center OR object EQ m_ui->pushButton_text_right ) {

        int font_size = m_ui->spinBox_font_size->value();
        int align     = LEFT_ALIGN;

        if ( object EQ m_ui->pushButton_text_center ) {
            align = CENTER_ALIGN;
        }
        else if ( object EQ m_ui->pushButton_text_right ) {
            align = RIGHT_ALIGN;
        }

        m_align =  (ALIGN_ELEMENTS)align;

        m_design_widget->SET_ALIGN_SELECTED_VARIABLE(align);

        degisken->SET_FONT_SIZE( font_size );
        degisken->SET_ALIGN( align );

        m_design_widget->SET_SELECTED_FONT_SIZE(font_size);

        SET_ALING_BUTTON_STATE( m_align );
    }
    else if ( object EQ m_ui->doubleSpinBox_x OR object EQ m_ui->doubleSpinBox_y) {

        if (m_ui->doubleSpinBox_x->value() > m_belge_width OR m_ui->doubleSpinBox_y->value() > m_belge_height ) {
            MSG_WARNING(tr("You can not enter the document size you have given great value."),NULL);//Vermiş olduğunuz belge boyutundan büyük değer giremezsiniz.
            return ADAK_FAIL_UNDO;
        }

        CONSOLE_POS_SELECTED_LABEL();
    }
    else if ( object EQ m_ui->line_edit_yazi_laser ) {

        degisken->SET_TEXT( m_ui->line_edit_yazi_laser->text() );

        m_design_widget->SET_OBJNAME_SELECTED_VARIABLE( degisken->GET_LABEL()->objectName() );
        m_design_widget->SET_TEXT_SELECTED_VARIABLE(m_ui->line_edit_yazi_laser->text());
    }
    else if ( object EQ m_ui->pushButton_text_bold ) {

        degisken->SET_BOLD( m_ui->pushButton_text_bold->isChecked() );
        QFont font = degisken->GET_LABEL()->font();
        font.setBold(m_ui->pushButton_text_bold->isChecked());
        degisken->GET_LABEL()->setFont( font );

        SET_BOLD_ITALIC_UNDER_LINE_STATE();
    }
    else if ( object EQ m_ui->pushButton_text_under_line ) {
        degisken->SET_UNDER_LINE( m_ui->pushButton_text_under_line->isChecked() );
        QFont font = degisken->GET_LABEL()->font();
        font.setUnderline(m_ui->pushButton_text_under_line->isChecked());
        degisken->GET_LABEL()->setFont( font );

        SET_BOLD_ITALIC_UNDER_LINE_STATE();
    }
    else if ( object EQ m_ui->pushButton_text_italic ) {
        degisken->SET_ITALIC( m_ui->pushButton_text_italic->isChecked() );
        QFont font = degisken->GET_LABEL()->font();
        font.setItalic(m_ui->pushButton_text_italic->isChecked());
        degisken->GET_LABEL()->setFont( font );

        SET_BOLD_ITALIC_UNDER_LINE_STATE();
    }
    else if ( object EQ m_ui->pushButton_text_strikeout ) {
        degisken->SET_STRIKEOUT( m_ui->pushButton_text_strikeout->isChecked() );
        QFont font = degisken->GET_LABEL()->font();
        font.setStrikeOut(m_ui->pushButton_text_strikeout->isChecked());
        degisken->GET_LABEL()->setFont( font );

        SET_BOLD_ITALIC_UNDER_LINE_STATE();
    }
    else if ( object EQ m_ui->pushButton_font_family ) {
        bool ok;
         QFont font = QFontDialog::getFont( &ok, degisken->GET_LABEL()->font(), this);
         if (ok EQ true ) {
             degisken->SET_FONT_FAMILY(font.family());
             degisken->SET_BOLD(font.bold());
             degisken->SET_ITALIC(font.italic());
             degisken->SET_UNDER_LINE(font.underline());
             degisken->SET_FONT_SIZE(font.pointSize());

             degisken->GET_LABEL()->setFont( font );
         }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_BATCH::CHECK_RUN
***************************************************************************************/

int TASARIM_BATCH::CHECK_RUN()
{
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_BATCH::RUN_BATCH
***************************************************************************************/

void TASARIM_BATCH::RUN_BATCH()
{
    m_design_changed = false;
    G_YONETIM_DB->START_TRANSACTION();
    SAVE_ALL_VARIABLES();
    G_YONETIM_DB->COMMIT_TRANSACTION();
}

/**************************************************************************************
                   TASARIM_BATCH::CONVERT_MMX_TO_PX_X
***************************************************************************************/

int TASARIM_BATCH::CONVERT_MMX_TO_PX_X(double mmx)
{
    int px_x = QVariant(mmx).toDouble() * (QVariant(m_ui->frame_design_widget->width()).toDouble() / QVariant(m_belge_width).toDouble());

    return px_x;
}

/**************************************************************************************
                   TASARIM_BATCH::CONVERT_MMY_TO_PX_Y
***************************************************************************************/

int TASARIM_BATCH::CONVERT_MMY_TO_PX_Y(double mmy)
{
    int px_y = QVariant(mmy).toDouble() * (QVariant(m_ui->frame_design_widget->height()).toDouble() / QVariant(m_belge_height).toDouble());

    return px_y;
}

/**************************************************************************************
                   TASARIM_BATCH::SET_VARIABLE_POSITION
***************************************************************************************/

void TASARIM_BATCH::MOUSE_VARIABLE_POSITION( int p_degisken_id,double x, double y)
{
    TASARIM_DEGISKENI *degisken = GET_DEGISKEN( p_degisken_id );
    int index = degisken->GET_PRIMARY_ID();

    m_design_changed =  true;

    CLOSE_ALL_EVENTS();
    double mm_y = y * ( m_belge_height / QVariant(m_ui->frame_design_widget->height()).toDouble() );
    double mm_x = x * ( m_belge_width  / QVariant(m_ui->frame_design_widget->width()).toDouble());

    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
        FIND_SATIR_AND_BOSLUK( index, mm_x, mm_y);
    }

    SET_USER_VARIABLE_POS(mm_x, mm_y, index, m_ui->doubleSpinBox_x, m_ui->doubleSpinBox_y );

    if ( degisken->GET_GRUP() EQ LINE) {
        //! Satirda degistirilen
        double current_line_y = degisken->GET_POS_Y();

        //! Satir degiskenleri tespit ediliyor.
        QList<TASARIM_DEGISKENI*> satir_degiskenleri = GET_LINE_VARIABLE_LIST();

        for (int i = 0 ; i < satir_degiskenleri.size() ; i++) {

            TASARIM_DEGISKENI* satir_degiskeni = satir_degiskenleri.at(i);

            if ( degisken EQ satir_degiskeni ) {
                continue;
            }
            if ( satir_degiskeni->IS_VISIBLE() EQ false) {
                continue;
            }

            satir_degiskeni->SET_SATIR( degisken->GET_SATIR() );

            //! Satirdaki labeller sirayla hareket ettiriliyor.
            QLabel * label = satir_degiskeni->GET_LABEL();
            double line_x_pos = satir_degiskeni->GET_POS_X();

            satir_degiskeni->SET_POS_Y( current_line_y );

            if (label NE NULL ) {
                m_design_widget->MOVE_VARIABLE( label, CONVERT_MMX_TO_PX_X(line_x_pos), CONVERT_MMY_TO_PX_Y(current_line_y), true);
            }
            else {
                m_design_widget->MOVE_SELECTED_VARIABLE( CONVERT_MMX_TO_PX_X(line_x_pos), CONVERT_MMY_TO_PX_Y(current_line_y), true);
            }
        }
    }
    OPEN_ALL_EVENTS();
}

/**************************************************************************************
                   TASARIM_BATCH::SET_POS_SELECTED_LABEL
***************************************************************************************/

void TASARIM_BATCH::CONSOLE_POS_SELECTED_LABEL(double x, double y, QLabel * label)
{

    if ( label EQ NULL ) {
        label = m_design_widget->GET_SELECTED_LABEL();
        if ( label EQ NULL ) {
            return;
        }
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN(label);

    double mm_x = x;
    double mm_y = y;

    if ( x EQ -1 AND y EQ -1 ) {
        mm_x = m_ui->doubleSpinBox_x->value();
        mm_y = m_ui->doubleSpinBox_y->value();
    }

    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
        FIND_SATIR_AND_BOSLUK( degisken->GET_PRIMARY_ID(), mm_x, mm_y);
    }
    if (label NE NULL) {
        label = m_design_widget->MOVE_VARIABLE(label, CONVERT_MMX_TO_PX_X(mm_x), CONVERT_MMY_TO_PX_Y(mm_y), true);
    }
    else {
        label = m_design_widget->MOVE_SELECTED_VARIABLE(CONVERT_MMX_TO_PX_X(mm_x), CONVERT_MMY_TO_PX_Y(mm_y), true);
    }

    SET_USER_VARIABLE_POS( mm_x, mm_y, degisken->GET_PRIMARY_ID(), m_ui->doubleSpinBox_x, m_ui->doubleSpinBox_y );
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_KEY_DIRECTION_PRESS
***************************************************************************************/

void TASARIM_BATCH::SLOT_KEY_DIRECTION_PRESS(Qt::Key p_key)
{
    switch( p_key ) {
        case Qt::Key_Up : {            
            double mm_y = m_ui->doubleSpinBox_y->value() - 1;
            double mm_x = m_ui->doubleSpinBox_x->value();

            QLabel * label = m_design_widget->GET_SELECTED_LABEL();
            TASARIM_DEGISKENI * degisken = GET_LABEL_DEGISKEN( label );

            MOUSE_VARIABLE_POSITION( degisken->GET_PRIMARY_ID(), MM_X2PX_X(mm_x), MM_Y2PX_Y(mm_y));
            break;
        }
        case Qt::Key_Down: {

            double mm_y = m_ui->doubleSpinBox_y->value() + 1;
            double mm_x = m_ui->doubleSpinBox_x->value();

            QLabel * label = m_design_widget->GET_SELECTED_LABEL();
            TASARIM_DEGISKENI * degisken = GET_LABEL_DEGISKEN( label );

            MOUSE_VARIABLE_POSITION( degisken->GET_PRIMARY_ID(), MM_X2PX_X(mm_x), MM_Y2PX_Y(mm_y));
            break;
        }
        case Qt::Key_Left:
            m_ui->doubleSpinBox_x->setValue( m_ui->doubleSpinBox_x->value() - 1.00 );
            break;
        case Qt::Key_Right: {
            m_ui->doubleSpinBox_x->setValue( m_ui->doubleSpinBox_x->value() + 1.00 );
            break;
        }
        default:
            break;
    }
}

/**************************************************************************************
                   TASARIM_BATCH::SET_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_BATCH::SET_SELECTED_VARIABLE(QLabel * label)
{
    CLOSE_CHECK_VAR_CONTROL();

    m_ui->line_edit_yazi_laser->disconnect();
    m_ui->spinBox_font_size->disconnect();
    m_ui->spinBox_karakter_sayisi->disconnect();
    m_ui->doubleSpinBox_x->disconnect();
    m_ui->doubleSpinBox_y->disconnect();
    m_ui->spinBox_picture_height->disconnect();
    m_ui->spinBox_picture_width->disconnect();

    m_ui->checkBox_monospace_mi->disconnect();
    m_ui->checkBox_nlq_aktif_mi->disconnect();
    m_ui->radioButton_mode_normal->disconnect();
    m_ui->radioButton_mode_condense->disconnect();
    m_ui->radioButton_mode_large->disconnect();
    m_ui->doubleSpinBox_satir_sayisi->disconnect();
    m_ui->doubleSpinBox_soldan_bosluk->disconnect();

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( label );

    m_ui->line_edit_yazi_laser->clear();

    double pos_x = 0.0;
    double pos_y = 0.0;

    int    font_size      = 5;
    bool   is_visible     = true;
    int    align          = 1;
    int    monospace_mi   = 0;
    int    karakter_sayisi= 1;

    int    images_height  = 1;
    int    images_weight  = 1;

    QString degisken_adi;

    ///////////////////////////////
    m_ui->groupBox_laser          ->setEnabled( true );
    m_ui->spinBox_font_size       ->setEnabled( true  );
    m_ui->pushButton_text_left    ->setEnabled( true  );
    m_ui->pushButton_text_center  ->setEnabled( true  );
    m_ui->pushButton_text_right   ->setEnabled( true  );
    m_ui->spinBox_picture_height  ->setEnabled( false );
    m_ui->spinBox_picture_width   ->setEnabled( false );
    m_ui->line_edit_yazi_laser    ->setEnabled( false );
    m_ui->spinBox_karakter_sayisi ->setEnabled( true  );

    if ( m_printer_type EQ LAZER_PRINTERS ) {
        m_ui->checkBox_monospace_mi->setEnabled( true  );
    }
    else { // DOT_MATRIX_PRINTER
        m_ui->checkBox_nlq_aktif_mi->setChecked( degisken->IS_NLQ() );

        if ( degisken->GET_DOT_MARIX_MODE() EQ NORMAL_MODE ) {
            m_ui->radioButton_mode_normal->setChecked(true);
        }
        else if ( degisken->GET_DOT_MARIX_MODE() EQ CONDENSED_MODE ) {
            m_ui->radioButton_mode_condense->setChecked(true);
        }
        else if ( degisken->GET_DOT_MARIX_MODE() EQ LARGE_MODE ){
            m_ui->radioButton_mode_large->setChecked(true);
        }
    }

    ///////////////////////////////

    if (degisken->GET_GRUP() EQ LINE OR degisken->GET_GRUP() EQ HEADER OR degisken->GET_GRUP() EQ FOOTER ) {

        font_size           = degisken->GET_FONT_SIZE();
        is_visible          = degisken->IS_VISIBLE();
        align               = degisken->GET_ALIGN();
        pos_x               = degisken->GET_POS_X();
        pos_y               = degisken->GET_POS_Y();
        monospace_mi        = degisken->IS_MONOSPACE_MI();
        karakter_sayisi     = degisken->GET_TEXT_SIZE();
        degisken_adi        = degisken->GET_TEXT();

        if ( degisken->GET_GRUP() EQ LINE ) {
            degisken_adi.prepend("[[");
            degisken_adi.append("]]");
        }
        else {
            degisken_adi.prepend("[");
            degisken_adi.append("]");
        }
    }
    else if (degisken->GET_GRUP() EQ USER_TEXT ) {

        is_visible      = degisken->IS_VISIBLE();
        font_size       = degisken->GET_FONT_SIZE();
        align           = degisken->GET_ALIGN();
        pos_x           = degisken->GET_POS_X();
        pos_y           = degisken->GET_POS_Y();
        degisken_adi    = degisken->GET_TEXT();
        karakter_sayisi = degisken->GET_TEXT_SIZE();

        m_ui->line_edit_yazi_laser->setText( degisken->GET_TEXT() );
        m_ui->line_edit_yazi_laser->setEnabled(true);

        m_ui->spinBox_karakter_sayisi->setEnabled( false );
        m_ui->checkBox_monospace_mi->setEnabled( false );
    }
    else if (degisken->GET_GRUP() EQ USER_IMG) {

        is_visible    = degisken->IS_VISIBLE();
        font_size     = degisken->GET_FONT_SIZE();
        align         = degisken->GET_ALIGN();
        pos_x         = degisken->GET_POS_X();
        pos_y         = degisken->GET_POS_Y();
        images_height = degisken->GET_SIZE_Y();
        images_weight = degisken->GET_SIZE_X();
        degisken_adi  = degisken->GET_TEXT();

        m_ui->spinBox_font_size->setEnabled(false);
        m_ui->pushButton_text_left->setEnabled(false);

        m_ui->pushButton_text_center->setEnabled(false);
        m_ui->pushButton_text_right->setEnabled(false);

        m_ui->spinBox_picture_height->setEnabled(true);
        m_ui->spinBox_picture_width->setEnabled(true);

        m_ui->pushButton_text_bold->setEnabled(false);
        m_ui->pushButton_text_under_line->setEnabled(false);
        m_ui->pushButton_text_italic->setEnabled(false);
        m_ui->pushButton_text_strikeout->setEnabled(false);
    }

    m_ui->spinBox_font_size->setValue( font_size );
    m_ui->spinBox_karakter_sayisi->setValue( karakter_sayisi );
    m_ui->spinBox_picture_height->setValue( images_height );
    m_ui->spinBox_picture_width->setValue( images_weight );
    m_ui->label_degisken_adi->setText( GET_PIXMAP_FILE_NAME( degisken_adi ));

    m_align = (ALIGN_ELEMENTS)align;

    SET_ALING_BUTTON_STATE( m_align );

    m_ui->pushButton_text_bold->setChecked(degisken->IS_BOLD());
    m_ui->pushButton_text_under_line->setChecked(degisken->IS_UNDER_LINE());
    m_ui->pushButton_text_italic->setChecked(degisken->IS_ITALIC());
    m_ui->pushButton_text_strikeout->setChecked(degisken->IS_STRIKEOUT());

    SET_BOLD_ITALIC_UNDER_LINE_STATE();

    if ( monospace_mi EQ 1 ) {
        m_ui->checkBox_monospace_mi->setChecked( true );
        m_ui->pushButton_font_family->setEnabled(false);
    }
    else {
        m_ui->checkBox_monospace_mi->setChecked( false );
        m_ui->pushButton_font_family->setEnabled(true);
    }

    m_ui->doubleSpinBox_soldan_bosluk->setValue(degisken->GET_SOLDAN_BOSLUK());

    SET_USER_VARIABLE_POS( pos_x, pos_y, degisken->GET_PRIMARY_ID(), m_ui->doubleSpinBox_x, m_ui->doubleSpinBox_y );

    connect( m_ui->line_edit_yazi_laser,        SIGNAL( textChanged(QString) ), this, SLOT( SLOT_DEGISKEN_DISPLAY_CHANGE(QString) ) );
    connect( m_ui->spinBox_font_size,           SIGNAL( valueChanged(int)    ), this, SLOT( SLOT_CHANGE_FONT_SIZE() ) );
    connect( m_ui->spinBox_karakter_sayisi,     SIGNAL( valueChanged(int)    ), this, SLOT( SLOT_KARAKTER_UZUNLUGU_CHANGE() ) );
    connect( m_ui->doubleSpinBox_x,             SIGNAL( valueChanged(double) ), this, SLOT( SLOT_X_AND_Y_CHANGE( double ) ) );
    connect( m_ui->doubleSpinBox_y,             SIGNAL( valueChanged(double) ), this, SLOT( SLOT_X_AND_Y_CHANGE( double ) ) );
    connect( m_ui->spinBox_picture_height,      SIGNAL( valueChanged(int)    ), this, SLOT( SLOT_IMAGE_HEIGHT_AND_WEIGHT(int)) );
    connect( m_ui->spinBox_picture_width,       SIGNAL( valueChanged(int)    ), this, SLOT( SLOT_IMAGE_HEIGHT_AND_WEIGHT(int)) );
    connect( m_ui->checkBox_monospace_mi,       SIGNAL( toggled(bool)        ), this, SLOT( SLOT_MONOSPACE_MI()));
    connect( m_ui->checkBox_nlq_aktif_mi,       SIGNAL( toggled(bool)        ), this, SLOT( SLOT_NLQ()));

    connect( m_ui->radioButton_mode_normal,     SIGNAL( toggled(bool)        ), this, SLOT( SLOT_DOT_MATRIX_MODE(bool)));
    connect( m_ui->radioButton_mode_condense,   SIGNAL( toggled(bool)        ), this, SLOT( SLOT_DOT_MATRIX_MODE(bool)));
    connect( m_ui->radioButton_mode_large,      SIGNAL( toggled(bool)        ), this, SLOT( SLOT_DOT_MATRIX_MODE(bool)));

    //! DOT_MATRIX_PRINTER
    connect( m_ui->doubleSpinBox_soldan_bosluk,  SIGNAL( valueChanged(double) ), this, SLOT( SLOT_CHANGE_SOL_BOSLUK()) );

    OPEN_CHECK_VAR_CONTROL();
}

/**************************************************************************************
                   TASARIM_BATCH::SET_VARIABLE_DEFAULTS
***************************************************************************************/

void TASARIM_BATCH::SET_VARIABLE_DEFAULTS(QLabel *label)
{
    CLOSE_CHECK_VAR_CONTROL();

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( label );

    int     font_size       = 5;
    bool    is_visible      = true;
    int     align           = 1;
    double  pos_x           = 0.0;
    double  pos_y           = 0.0;
    int     monospace_mi    = 0;
    int     karakter_sayisi = 0;

    if ( degisken->GET_GRUP() EQ LINE OR  degisken->GET_GRUP() EQ HEADER OR degisken->GET_GRUP() EQ FOOTER ) {

        monospace_mi   = degisken->IS_MONOSPACE_MI();
        karakter_sayisi= degisken->GET_TEXT_SIZE();
        font_size      = degisken->GET_FONT_SIZE();
        is_visible     = degisken->IS_VISIBLE();
        align          = degisken->GET_ALIGN();
        pos_x          = degisken->GET_POS_X();
        pos_y          = degisken->GET_POS_Y();
    }
    else if ( degisken->GET_GRUP() EQ USER_TEXT) {

        label->setText( degisken->GET_TEXT() );
        label->setStyleSheet("color:rgb(128, 128, 0); background-color:rgba(255, 255, 255,0);");

        font_size      = degisken->GET_FONT_SIZE();
        is_visible     = degisken->IS_VISIBLE();
        align          = degisken->GET_ALIGN();
        pos_x          = degisken->GET_POS_X();
        pos_y          = degisken->GET_POS_Y();

    }
    else if ( degisken->GET_GRUP() EQ USER_IMG) {

        int width  = degisken->GET_SIZE_X();
        int height = degisken->GET_SIZE_Y();
        is_visible = degisken->IS_VISIBLE();
        align      = degisken->GET_ALIGN();
        pos_x      = degisken->GET_POS_X();
        pos_y      = degisken->GET_POS_Y();

        label->setMaximumSize(width,height);
        label->setMinimumSize(width,height);

        label->setPixmap( degisken->GET_IMAGES() );
    }

    QFont font = label->font();

    font.setFamily      (degisken->GET_FONT_FAMILY());
    font.setBold        (degisken->IS_BOLD());
    font.setUnderline   (degisken->IS_UNDER_LINE());
    font.setItalic      (degisken->IS_ITALIC());
    font.setStrikeOut   (degisken->IS_STRIKEOUT());
    label->setFont(font);

    m_design_widget->SET_FONT( label, font_size );

    if (is_visible EQ false) {
        label->hide();
    }

    label->adjustSize();

    m_design_widget->SET_ALIGN_SELECTED_VARIABLE(align, label);

    CONSOLE_POS_SELECTED_LABEL(pos_x, pos_y, label);

    OPEN_CHECK_VAR_CONTROL();
}

/**************************************************************************************
                   TASARIM_BATCH::SET_DEGISKEN_POS
***************************************************************************************/

void TASARIM_BATCH::SET_DEGISKEN_POS ( ADAK_BELGE_DEFAULTS_STRUCT * belge_struct,int index, double pos_x,double pos_y,bool mouse_hassasiyeti_aktif )
{    
    if (pos_x > (belge_struct[index].x_position - m_fare_hassasiyeti) AND
        pos_x < (belge_struct[index].x_position + m_fare_hassasiyeti) AND mouse_hassasiyeti_aktif EQ true) {

        pos_x = belge_struct[index].x_position;
    }

    if (pos_y > (belge_struct[index].y_position - m_fare_hassasiyeti) AND
        pos_y < (belge_struct[index].y_position + m_fare_hassasiyeti) AND  mouse_hassasiyeti_aktif EQ true) {

        pos_y = belge_struct[index].y_position;
    }

    SET_DEGISKEN_POS_STRUCT(belge_struct,index,pos_x,pos_y);
}

/**************************************************************************************
                   TASARIM_BATCH::SET_DEGISKEN_POS_STRUCT
***************************************************************************************/

void TASARIM_BATCH::SET_DEGISKEN_POS_STRUCT ( ADAK_BELGE_DEFAULTS_STRUCT *belge_struct, int index, double pos_x, double pos_y )
{
    if (pos_x NE -1.00) {
        m_ui->doubleSpinBox_x->setValue(VIRGUL_EKLE(QVariant(ROUND(pos_x)).toString()).toDouble());
        belge_struct[index].x_position = pos_x;
    }
    if (pos_y NE -1.00) {
        m_ui->doubleSpinBox_y->setValue(VIRGUL_EKLE(QVariant(ROUND(pos_y)).toString()).toDouble());
        belge_struct[index].y_position = pos_y;
    }
}

/**************************************************************************************
                   TASARIM_BATCH::SET_DESIGN_WIDGET
***************************************************************************************/

void TASARIM_BATCH::SET_DESIGN_WIDGET ( double oran )
{    
    QVBoxLayout * vertical_layout;

    if (m_ui->frame_design_widget->layout() EQ NULL ) {
        vertical_layout = new QVBoxLayout;
        m_ui->frame_design_widget->setLayout(vertical_layout);
        setWindowState(Qt::WindowMaximized);
    }
    else {
        vertical_layout = static_cast<QVBoxLayout *>(m_ui->frame_design_widget->layout());
    }

    int frame_width  = QVariant(m_belge_width).toDouble() * 3.8;
    int frame_height = QVariant(m_belge_height).toDouble() * 3.8;

    m_ui->frame_design_widget->setMinimumSize(frame_width * oran,frame_height * oran);
    m_ui->frame_design_widget->setMaximumSize(frame_width * oran,frame_height * oran);

    m_design_widget = new TASARIM_DESIGN_WIDGET( oran, this );

    m_design_widget->installEventFilter( this );

    vertical_layout->addWidget( m_design_widget );

    connect( m_design_widget, SIGNAL(VARIABLE_POSITION_CHANGED( int, double, double)),this,SLOT(MOUSE_VARIABLE_POSITION(int, double, double)));
    connect( m_design_widget, SIGNAL(VARIABLE_SELECTED(QLabel*)), this, SLOT(SET_SELECTED_VARIABLE(QLabel*)));
    connect( m_design_widget, SIGNAL(VARIABLE_CREATED (QLabel*)), this, SLOT(SET_VARIABLE_DEFAULTS(QLabel*)));
    connect( m_design_widget, SIGNAL(SIGNAL_KEY_DIRECTION_PRESS ( Qt::Key )), this, SLOT(SLOT_KEY_DIRECTION_PRESS(Qt::Key)));

    // Degisken ozelliklerini aktif pasif yapmak icin kullanilmistir.
    connect( m_design_widget, SIGNAL(VARIABLE_UNSELECTED()), this, SLOT(SLOT_VARIABLE_UNSELECTED()));

    m_design_widget->SET_WIDGET_SIZE(m_belge_width,m_belge_height);

    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
        for (int i = 0; i < m_all_variables.size(); ++i) {
            m_all_variables.at(i)->SET_MONOSPACE_MI(true);

            QLabel label;

            if ( m_all_variables.at(i)->GET_DOT_MARIX_MODE() EQ NORMAL_MODE ) {
                m_all_variables.at(i)->SET_FONT_SIZE(label.font().pointSize());
            }
            else if ( m_all_variables.at(i)->GET_DOT_MARIX_MODE() EQ CONDENSED_MODE ) {
                ADAK_SET_FONT_SIZE( &label, CONDANSE_MODE_CARPAN );
                m_all_variables.at(i)->SET_FONT_SIZE( label.font().pointSize());
            }
            else if ( m_all_variables.at(i)->GET_DOT_MARIX_MODE() EQ LARGE_MODE ) {
                ADAK_SET_FONT_SIZE( &label, LARGE_MODE_CARPAN );
                m_all_variables.at(i)->SET_FONT_SIZE(label.font().pointSize());
            }
        }
    }

    //! TASARIMCI DEGISKENLERI BURDA OLUSTURULUYOR
    m_design_widget->INIT_WIDGET( m_all_variables, m_tasarimci_modu );

}

/**************************************************************************************
                   TASARIM_BATCH::SAVE_USER_DEFAULTS
***************************************************************************************/

void TASARIM_BATCH::SAVE_ALL_VARIABLES()
{
    SQL_QUERY query(G_YONETIM_DB);

    for ( int i = 0 ; i < m_all_variables.size() ; i++ ) {

        TASARIM_DEGISKENI *degisken = m_all_variables.at( i );

        if (degisken->IS_SILINECEK() EQ true ) {
            if ( degisken->GET_BELGE_DEGISKEN_ID() > 0) {
                SQL_QUERY delete_query (G_YONETIM_DB);
                delete_query.PREPARE_DELETE("ynt_belge_degiskenleri","belge_degisken_id = :belge_degisken_id");
                delete_query.SET_VALUE(":belge_degisken_id" , degisken->GET_BELGE_DEGISKEN_ID() );
                delete_query.DELETE();
                degisken->SET_BELGE_DEGISKEN_ID(-1);
            }
            continue;
        }

        if ( degisken->GET_BELGE_DEGISKEN_ID() EQ -1 ) {
            query.PREPARE_INSERT("ynt_belge_degiskenleri","belge_degisken_id",
                                 "tasarim_id,font_size,align,pos_x,pos_y,is_visible_variable,grup_enum,size_vertical,"
                                 "size_horizontal,text,pixmap,degisken_id, text_size, monospace_mi, "
                                 "is_bold, is_under_line, is_italic, is_strikeout, font_family, is_nlq, dot_matrix_mode, satir, soldan_bosluk");

            query.SET_VALUE(":tasarim_id"             ,m_tasarim_id);
            query.SET_VALUE(":font_size"              ,degisken->GET_FONT_SIZE()  );
            query.SET_VALUE(":align"                  ,degisken->GET_ALIGN()  );
            query.SET_VALUE(":pos_x"                  ,ROUND(degisken->GET_POS_X()) );
            query.SET_VALUE(":pos_y"                  ,ROUND(degisken->GET_POS_Y()) );
            query.SET_VALUE(":size_vertical"          ,degisken->GET_SIZE_Y()  );
            query.SET_VALUE(":size_horizontal"        ,degisken->GET_SIZE_X()  );
            query.SET_VALUE(":text"                   ,degisken->GET_TEXT()    );
            query.SET_VALUE(":is_visible_variable"    , QVariant ( degisken->IS_VISIBLE() ).toInt()  );
            query.SET_VALUE(":is_bold"                , QVariant ( degisken->IS_BOLD() ).toInt()  );
            query.SET_VALUE(":is_under_line"          , QVariant ( degisken->IS_UNDER_LINE() ).toInt()  );
            query.SET_VALUE(":is_italic"              , QVariant ( degisken->IS_ITALIC() ).toInt()  );
            query.SET_VALUE(":is_strikeout"           , QVariant ( degisken->IS_STRIKEOUT() ).toInt()  );
            query.SET_VALUE(":font_family"            , degisken->GET_FONT_FAMILY() );
            query.SET_VALUE(":is_nlq"                 , QVariant ( degisken->IS_NLQ() ).toInt() );
            query.SET_VALUE(":dot_matrix_mode"        , degisken->GET_DOT_MARIX_MODE() );
            query.SET_VALUE(":satir"                  , degisken->GET_SATIR() );
            query.SET_VALUE(":soldan_bosluk"          , degisken->GET_SOLDAN_BOSLUK() );

            if ( degisken->GET_GRUP() EQ USER_IMG ) {
                query.SET_VALUE(":pixmap", ADAK_PIXMAP_2_BYTE_ARRAY( degisken->GET_TEXT() ));

            }
            else {
                query.SET_VALUE(":pixmap", "");
            }

            query.SET_VALUE(":grup_enum"   , degisken->GET_GRUP() );
            query.SET_VALUE(":degisken_id" , degisken->GET_DEGISKEN_ID() );
            query.SET_VALUE(":monospace_mi", QVariant(degisken->IS_MONOSPACE_MI()).toInt() );
            query.SET_VALUE(":text_size"   , degisken->GET_TEXT_SIZE() );

            int belge_degisken_id = query.INSERT();

            degisken->SET_BELGE_DEGISKEN_ID( belge_degisken_id );

        }
        else {
            query.PREPARE_UPDATE("ynt_belge_degiskenleri","belge_degisken_id",
                                 "font_size,align,pos_x,pos_y,is_visible_variable, "
                                 "grup_enum,size_vertical,size_horizontal,text,pixmap, "
                                 "text_size, monospace_mi, is_bold, is_under_line, "
                                 "is_italic, is_strikeout, font_family, is_nlq, "
                                 "dot_matrix_mode, satir, soldan_bosluk",
                                 "belge_degisken_id = :belge_degisken_id" );

            query.SET_VALUE(":font_size"              ,degisken->GET_FONT_SIZE()  );
            query.SET_VALUE(":align"                  ,degisken->GET_ALIGN()  );
            query.SET_VALUE(":pos_x"                  ,ROUND(degisken->GET_POS_X()) );
            query.SET_VALUE(":pos_y"                  ,ROUND(degisken->GET_POS_Y()) );
            query.SET_VALUE(":size_vertical"          ,degisken->GET_SIZE_Y()  );
            query.SET_VALUE(":size_horizontal"        ,degisken->GET_SIZE_X()  );
            query.SET_VALUE(":text"                   ,degisken->GET_TEXT()    );

            query.SET_VALUE(":is_visible_variable"    ,QVariant (degisken->IS_VISIBLE() ).toInt());
            if ( degisken->GET_GRUP() EQ USER_IMG ) {
                query.SET_VALUE(":pixmap"             ,ADAK_PIXMAP_2_BYTE_ARRAY( degisken->GET_TEXT() ));
            }
            else {
                query.SET_VALUE(":pixmap", "");
            }

            query.SET_VALUE(":grup_enum",           degisken->GET_GRUP()  );
            query.SET_VALUE(":monospace_mi",        QVariant( degisken->IS_MONOSPACE_MI()).toInt() );
            query.SET_VALUE(":is_bold",             QVariant( degisken->IS_BOLD() ).toInt()  );
            query.SET_VALUE(":is_under_line",       QVariant( degisken->IS_UNDER_LINE() ).toInt()  );
            query.SET_VALUE(":is_italic",           QVariant( degisken->IS_ITALIC() ).toInt()  );
            query.SET_VALUE(":is_strikeout",        QVariant( degisken->IS_STRIKEOUT() ).toInt()  );
            query.SET_VALUE(":font_family",         degisken->GET_FONT_FAMILY() );
            query.SET_VALUE(":text_size",           degisken->GET_TEXT_SIZE() );
            query.SET_VALUE(":is_nlq",              QVariant ( degisken->IS_NLQ() ).toInt() );
            query.SET_VALUE(":dot_matrix_mode",     degisken->GET_DOT_MARIX_MODE() );
            query.SET_VALUE(":satir",               degisken->GET_SATIR() );
            query.SET_VALUE(":soldan_bosluk",       degisken->GET_SOLDAN_BOSLUK() );
            query.SET_VALUE(":belge_degisken_id",   degisken->GET_BELGE_DEGISKEN_ID() );

            query.UPDATE();
        }
    }
    if ( m_silinecek_degiskenler.size() > 0 ) {

        for (int i = 0; i < m_silinecek_degiskenler.size(); ++i) {
            query.PREPARE_DELETE( "ynt_belge_degiskenleri", " belge_degisken_id = :belge_degisken_id" );
            query.SET_VALUE     ( ":belge_degisken_id", m_silinecek_degiskenler.at( i ) );
            query.DELETE();
        }
    }

}

/**************************************************************************************
                   TASARIM_BATCH::SET_DEFAULT_VALUES
***************************************************************************************/

void TASARIM_BATCH::SET_DEFAULT_VALUES()
{
    CLOSE_CHECK_VAR_CONTROL();
    if ( m_all_variables.size() EQ 0 ) {
        return;
    }

    TASARIM_DEGISKENI * degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL()  );
    if ( degisken EQ NULL ) {
        return;
    }

    m_design_widget->SELECT_VARIABLE( degisken->GET_PRIMARY_ID() );
    OPEN_CHECK_VAR_CONTROL();
}

/**************************************************************************************
                   TASARIM_BATCH::CREATE_DESIGN_WIDGET
***************************************************************************************/

void TASARIM_BATCH::CREATE_DESIGN_WIDGET()
{
    if (m_design_widget NE NULL) {

        m_design_widget->close();
        m_design_widget = NULL;
    }

    double oran = ROUND( m_ui->comboBox_tasarim_yuzdesi->currentText().toDouble() / 100.00 );

    SET_DESIGN_WIDGET( oran );
    SET_DEFAULT_VALUES();
}

/**************************************************************************************
                   TASARIM_BATCH::CREATE_DESIGN_WIDGET
***************************************************************************************/

bool TASARIM_BATCH::eventFilter( QObject *p_obj, QEvent *p_event)
{
    if ( p_obj->metaObject()->className() EQ TASARIM_DESIGN_WIDGET::staticMetaObject.className() ) {
        return  m_design_widget->eventFilter( p_obj, p_event );
    }

    return BATCH_KERNEL::eventFilter( p_obj, p_event );
}

/**************************************************************************************
                   TASARIM_BATCH::CHECK_EXIT
***************************************************************************************/

int TASARIM_BATCH::CHECK_EXIT()
{
    if ( m_design_changed EQ true ) {
       int secim = MSG_YES_NO( tr("Changes were made to the design. Do you want to quit?"), NULL );
//Tasarımda Değişiklikler yapıldı. Çıkmak istiyormusunuz ?
       if ( secim NE ADAK_YES ) {
           return ADAK_FAIL;
       }
    }

    //! STATIC DEGISKENI RESETLEMEK ICIN.
    TASARIM_DEGISKENI *degisken  =  new TASARIM_DEGISKENI(false);
    degisken->RESET_STATIC_DEGISKEN();
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_SPINBOX_CHANGE
***************************************************************************************/

void TASARIM_BATCH::SLOT_CHANGE_FONT_SIZE()
{
    int font_size = m_ui->spinBox_font_size->value();
    int align     = m_align;

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL());

    m_design_widget->SET_ALIGN_SELECTED_VARIABLE(align);

    degisken->SET_FONT_SIZE( font_size );
    degisken->SET_ALIGN( align );

    m_design_widget->SET_SELECTED_FONT_SIZE(font_size);
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_X_AND_Y_CHANGE
***************************************************************************************/

void TASARIM_BATCH::SLOT_X_AND_Y_CHANGE( double p_value )
{
    Q_UNUSED( p_value );

    if (m_ui->doubleSpinBox_x->value() > m_belge_width OR m_ui->doubleSpinBox_y->value() > m_belge_height ) {
        MSG_WARNING(tr("You can not enter the document size you have given great value."),NULL);
        return;//Vermiş olduğunuz belge boyutundan büyük değer giremezsiniz.
    }

    CONSOLE_POS_SELECTED_LABEL();
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_KARAKTER_UZUNLUGU_CHANGE
***************************************************************************************/

void TASARIM_BATCH::SLOT_KARAKTER_UZUNLUGU_CHANGE()
{
    if ( m_design_widget->GET_LAST_MOVE_LABEL() EQ NULL ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    int text_size = m_ui->spinBox_karakter_sayisi->value();

    if ( m_tasarimci_modu EQ ONIZLEME_MODU ){
        QLabel *label = degisken->GET_LABEL();
        label->hide();
        degisken->SET_TEXT_SIZE( text_size );
        QString text = degisken->GET_TEXT();
        if ( m_tasarimci_modu EQ ONIZLEME_MODU ) {
            text = degisken->GET_DISPLAY_TXT();
        }
        label->setText( text.mid(0, text_size ));
        label->show();
    }

    degisken->SET_TEXT_SIZE( text_size );
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_DEGISKEN_DISPLAY_CHANGE
***************************************************************************************/

void TASARIM_BATCH::SLOT_DEGISKEN_DISPLAY_CHANGE( QString p_value )
{
    Q_UNUSED( p_value );
    CLOSE_CHECK_VAR_CONTROL();

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    if ( degisken EQ NULL ) {
        return;
    }

    degisken->SET_TEXT( m_ui->line_edit_yazi_laser->text());

    m_design_widget->SET_OBJNAME_SELECTED_VARIABLE( QString::number( degisken->GET_PRIMARY_ID() )+ "\t" +QString::number( degisken->GET_GRUP()) );
    m_design_widget->SET_TEXT_SELECTED_VARIABLE   ( m_ui->line_edit_yazi_laser->text() );

    OPEN_CHECK_VAR_CONTROL();
}

/**************************************************************************************
                   TASARIM_BATCH::SAVER_BUTTON_CLICKED
***************************************************************************************/

void TASARIM_BATCH::SAVER_BUTTON_CLICKED(QAbstractButton * button)
{
    if (button EQ m_ui->push_button_test_ciktisi) {
        if ( m_tasarimci_modu EQ DESIGNER_MODU ) {
            PRINT_BELGE( -1, -1, NULL, "", PRINT_TEST_VARS );
        }
        else {
            PRINT_BELGE( -1, -1, NULL, "", PRINT_TEST_XXX );
        }
    }
    else if ( button EQ m_ui->pushButton_belge_ozellikleri ) {
        OPEN_TASARIM_BELGE_BATCH ( m_tasarim_id, this );
    }
    else if ( button EQ m_ui->pushButton_onizleme_modu ) {
        if ( m_tasarimci_modu EQ DESIGNER_MODU ) {
            m_ui->pushButton_onizleme_modu->setText( tr("Designer Mode"));
            m_ui->pushButton_onizleme_modu->setIcon( QPixmap(":/tasarimci_icons/ICONS/tasarimci_designer_modu.png"));
            m_tasarimci_modu = ONIZLEME_MODU;
        }
        else {
            m_ui->pushButton_onizleme_modu->setText( tr("Preview Mode"));
            m_ui->pushButton_onizleme_modu->setIcon( QPixmap(":/tasarimci_icons/ICONS/tasarimci_onizleme_modu.png"));
            m_tasarimci_modu = DESIGNER_MODU;
        }

        CREATE_DESIGN_WIDGET();
    }
}

/**************************************************************************************
                   TASARIM_BATCH::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int TASARIM_BATCH::CHANGER_BUTTON_CLICKED (QAbstractButton *button)
{
    if ( button EQ m_ui->pushButton_bilgi_sec ) {

        m_design_widget->RESET_VARIABLE();
        SLOT_VARIABLE_UNSELECTED();

        TASARIM_DEGISKENI *secilen_degisken = OPEN_TASARIM_BELGE_DEGISKENLERI_SEC( m_variable, this );

        if ( secilen_degisken EQ NULL ) {
            return ADAK_RECORD_UNCHANGED;
        }

        TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(true);

        secilen_degisken->COPY( degisken );

        degisken->SET_BELGE_DEGISKEN_ID(-1);
        degisken->SET_POS_X(0);
        degisken->SET_POS_Y(0);

        QString label_txt = degisken->GET_TEXT();
        if ( m_tasarimci_modu EQ ONIZLEME_MODU ) {
            label_txt = degisken->GET_DISPLAY_TXT();
        }

        QLabel * user_label = m_design_widget->CREATE_NEW_LABEL(label_txt);

        QFont font = user_label->font();
        font.setFamily      (degisken->GET_FONT_FAMILY());
        font.setBold        (degisken->IS_BOLD());
        font.setUnderline   (degisken->IS_UNDER_LINE());
        font.setItalic      (degisken->IS_ITALIC());
        font.setStrikeOut   (degisken->IS_STRIKEOUT());
        user_label->setFont(font);

        degisken->SET_LABEL( user_label );

        user_label->setObjectName( QString::number( degisken->GET_PRIMARY_ID()) + "\t"+ QString::number( degisken->GET_GRUP() ) );

        m_all_variables.append( degisken );

        m_design_widget->ADD_NEW_VARIABLE( 0, 0, user_label );
    }

    return ADAK_RECORD_CHANGED;
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_RUN_BATCH
***************************************************************************************/

void TASARIM_BATCH::SLOT_RUN_BATCH()
{
    RUN_BATCH();
    MSG_INFO( tr("New values were recorded."), NULL );//Yeni değerler kaydedildi.
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_IMAGE_HEIGHT_AND_WEIGHT
***************************************************************************************/

void TASARIM_BATCH::SLOT_IMAGE_HEIGHT_AND_WEIGHT(int p_height)
{
    Q_UNUSED( p_height );

    if ( m_design_widget->GET_LAST_MOVE_LABEL() EQ NULL ) {
        return;
    }

    int width  = m_ui->spinBox_picture_width->value();
    int height = m_ui->spinBox_picture_height->value();

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    degisken->SET_SIZE_X( width );
    degisken->SET_SIZE_Y( height );

    m_design_widget->SET_SIZE_SELECTED_VARIABLE(width,height);
}

/**************************************************************************************
                   TASARIM_BATCH::SET_ALING_BUTTON_STATE
***************************************************************************************/

void TASARIM_BATCH::SET_ALING_BUTTON_STATE(ALIGN_ELEMENTS p_align)
{
    m_ui->pushButton_text_left->setChecked( false );
    m_ui->pushButton_text_center->setChecked( false );
    m_ui->pushButton_text_right->setChecked( false );

    QString reset_style_sheet = "background-color: rgb(255, 255, 255);";

    m_ui->pushButton_text_left->setStyleSheet(reset_style_sheet);
    m_ui->pushButton_text_center->setStyleSheet(reset_style_sheet);
    m_ui->pushButton_text_right->setStyleSheet(reset_style_sheet);

    QString style_sheet = "background-color: rgb(76, 169, 255);";

    switch( p_align ) {
        case LEFT_ALIGN:
            m_ui->pushButton_text_left->setChecked( true );
            m_ui->pushButton_text_left->setStyleSheet( style_sheet );
            break;
        case CENTER_ALIGN:
            m_ui->pushButton_text_center->setChecked( true );
            m_ui->pushButton_text_center->setStyleSheet( style_sheet );
            break;
        case RIGHT_ALIGN:
            m_ui->pushButton_text_right->setChecked( true );
            m_ui->pushButton_text_right->setStyleSheet( style_sheet );
            break;
        default:
            break;
    }
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_VARIABLE_UNSELECTED
***************************************************************************************/

void TASARIM_BATCH::SLOT_VARIABLE_UNSELECTED()
{
    CLOSE_ALL_EVENTS();
    m_ui->doubleSpinBox_x->clear();
    m_ui->doubleSpinBox_y->clear();
    m_ui->spinBox_picture_height->clear();
    m_ui->spinBox_picture_width->clear();
    m_ui->spinBox_font_size->clear();
    m_ui->spinBox_karakter_sayisi->clear();
    m_ui->line_edit_yazi_laser->clear();
    m_ui->label_degisken_adi->clear();

    m_ui->groupBox_laser->setEnabled( false );
    OPEN_ALL_EVENTS();
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_MONOSPACE_MI
***************************************************************************************/

void TASARIM_BATCH::SLOT_MONOSPACE_MI()
{
    if ( m_design_widget->GET_LAST_MOVE_LABEL() EQ NULL ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    degisken->SET_MONOSPACE_MI(m_ui->checkBox_monospace_mi->isChecked());

    if ( m_ui->checkBox_monospace_mi->isChecked() EQ true ) {
        degisken->GET_LABEL()->setFont( QFont("Monospace", m_ui->spinBox_font_size->value() ));
        m_ui->pushButton_font_family->setEnabled(false);
    }
    else {
        degisken->GET_LABEL()->setFont( QFont("Arial", m_ui->spinBox_font_size->value() ));
        m_ui->pushButton_font_family->setEnabled(true);
    }
}

/**************************************************************************************
                   TASARIM_BATCH::FILL_TASARIM_VARIABLE
***************************************************************************************/

void TASARIM_BATCH::FILL_TASARIM_VARIABLE()
{
    //! TASARIMIN DEGISKENLERI OKUNUYOR.
    for ( int i = 0 ; i < m_header_struct_size ; i++ ) {

        TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(false);

        degisken->SET_TASARIM_ID( m_tasarim_id );
        degisken->SET_TEXT( m_header_struct[i].degisken_adi );
        degisken->SET_KODU( m_header_struct[i].degisken_kodu );
        degisken->SET_GRUP( HEADER );
        degisken->SET_DEGISKEN_ID( m_header_struct[i].degisken_id );
        degisken->SET_DEGISKEN_TYPE( TASARIMCI_VARIABLE );
        degisken->SET_TEXT_SIZE( m_header_struct[i].karakter_sayisi );
        degisken->SET_FONT_SIZE(m_header_struct[i].font_size);
        if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
            QLabel label;
            degisken->SET_FONT_SIZE( label.font().pointSize());
        }

        if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
            degisken->SET_MONOSPACE_MI(true);
            degisken->SET_FONT_FAMILY("Monospace");
        }

        m_variable.append( degisken );
    }

    for ( int i = 0 ; i < m_footer_struct_size ; i++ ) {

        TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(false);

        degisken->SET_TASARIM_ID( m_tasarim_id );
        degisken->SET_TEXT( m_footer_struct[i].degisken_adi );
        degisken->SET_KODU( m_footer_struct[i].degisken_kodu );
        degisken->SET_GRUP( FOOTER );
        degisken->SET_DEGISKEN_ID( m_footer_struct[i].degisken_id );
        degisken->SET_DEGISKEN_TYPE( TASARIMCI_VARIABLE );
        degisken->SET_TEXT_SIZE( m_footer_struct[i].karakter_sayisi );
        degisken->SET_FONT_SIZE(m_footer_struct[i].font_size);
        if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
            QLabel label;
            degisken->SET_FONT_SIZE(label.font().pointSize());
        }

        if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
            degisken->SET_MONOSPACE_MI(true);
            degisken->SET_FONT_FAMILY("Monospace");
        }

        m_variable.append( degisken );
    }

    for ( int i = 0 ; i < m_line_struct_size ; i++ ) {

        TASARIM_DEGISKENI *degisken = new TASARIM_DEGISKENI(false);

        degisken->SET_TASARIM_ID( m_tasarim_id );
        degisken->SET_TEXT( m_line_struct[i].degisken_adi );
        degisken->SET_KODU( m_line_struct[i].degisken_kodu );
        degisken->SET_GRUP( LINE );
        degisken->SET_DEGISKEN_ID( m_line_struct[i].degisken_id );
        degisken->SET_DEGISKEN_TYPE( TASARIMCI_VARIABLE );
        degisken->SET_TEXT_SIZE( m_line_struct[i].karakter_sayisi );
        degisken->SET_DISPLAY_INT(9);
        degisken->SET_DISPLAY_KARAKTER("x");
        degisken->SET_FONT_SIZE(m_line_struct[i].font_size);
        if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
            QLabel label;
            degisken->SET_FONT_SIZE(label.font().pointSize());
        }

        if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
            degisken->SET_MONOSPACE_MI(true);
            degisken->SET_FONT_FAMILY("Monospace");
        }
        m_variable.append( degisken );
    }
}

/**************************************************************************************
                   TASARIM_BATCH::SET_BOLD_ITALIC_UNDER_LINE_STATE
***************************************************************************************/

void TASARIM_BATCH::SET_BOLD_ITALIC_UNDER_LINE_STATE()
{
    QString reset_css  = "background-color: rgb(255, 255, 255);";
    QString active_css = "background-color: rgb(76, 169, 255);";

    m_ui->pushButton_text_bold->setStyleSheet(reset_css);
    m_ui->pushButton_text_italic->setStyleSheet(reset_css);
    m_ui->pushButton_text_under_line->setStyleSheet(reset_css);
    m_ui->pushButton_text_strikeout->setStyleSheet(reset_css);

    if ( m_ui->pushButton_text_bold->isChecked() EQ true ) {
        m_ui->pushButton_text_bold->setStyleSheet(active_css);
    }
    if ( m_ui->pushButton_text_italic->isChecked() EQ true ) {
        m_ui->pushButton_text_italic->setStyleSheet(active_css);
    }
    if ( m_ui->pushButton_text_under_line->isChecked() EQ true ) {
        m_ui->pushButton_text_under_line->setStyleSheet(active_css);
    }
    if ( m_ui->pushButton_text_strikeout->isChecked() EQ true ) {
        m_ui->pushButton_text_strikeout->setStyleSheet(active_css);
    }
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_CHANGE_SOL_BOSLUK
***************************************************************************************/

void TASARIM_BATCH::SLOT_CHANGE_SOL_BOSLUK()
{
    if ( m_slot_change_sol_bosluk_enable EQ false ) {
        return;
    }
    QLabel *label = m_design_widget->GET_SELECTED_LABEL();
    if ( label EQ NULL ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN(label);
    int soldan_bosluk = m_ui->doubleSpinBox_soldan_bosluk->value();

    degisken->SET_SOLDAN_BOSLUK(soldan_bosluk);

    SATIR_BOSLUKLARINI_HESAPLA(degisken->GET_PRIMARY_ID());
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_CHANGE_SOL_BOSLUK_USTEN_SATIR
***************************************************************************************/

void TASARIM_BATCH::SLOT_CHANGE_SOL_BOSLUK_USTEN_SATIR()
{
    if (m_ui->doubleSpinBox_soldan_bosluk->value() > m_belge_toplam_karakter_sayisi OR m_ui->doubleSpinBox_satir_sayisi->value() > m_belge_satir_sayisi) {
        MSG_WARNING(tr("You can not enter the document size you have given great value."),NULL);//Vermiş olduğunuz belge boyutundan büyük değer giremezsiniz.
        return;
    }

    CONSOLE_POS_SELECTED_LABEL_DOT_MATRIX();
}

/**************************************************************************************
                   TASARIM_BATCH::GET_BELGE_BOYUT_BILGILERI_SATIR
***************************************************************************************/

void TASARIM_BATCH::GET_BELGE_BOYUT_BILGILERI_SATIR()
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari", "belge_satir_sayisi, belge_toplam_karakter_sayisi", "tasarim_id = :tasarim_id");
    query.SET_VALUE     (":tasarim_id", m_tasarim_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();
        m_belge_satir_sayisi = query.VALUE(0).toInt();
        m_belge_toplam_karakter_sayisi = query.VALUE(1).toInt();
    }
}

/**************************************************************************************
                   TASARIM_BATCH::SET_VARIABLE_POSITION
***************************************************************************************/

void TASARIM_BATCH::MOUSE_VARIABLE_POSITION_DOT_MATRIX( int p_degisken_id, double x, double y)
{

    CLOSE_ALL_EVENTS();
    int satiri        = ROUND(QVariant(y).toDouble() * (1.00/(QVariant(m_ui->frame_design_widget->height()).toDouble() / QVariant(m_belge_satir_sayisi).toDouble())));
    int soldan_bosluk = ROUND(QVariant(x).toDouble() * (1.00/(QVariant(m_ui->frame_design_widget->width()).toDouble() / QVariant(m_belge_toplam_karakter_sayisi).toDouble())));

    // Fare sapmasi icin 1 bosluk farklari kabul etmiyor.Sadece lineeditten atanabilir.
    if (soldan_bosluk + 1 EQ m_ui->doubleSpinBox_soldan_bosluk->value() OR soldan_bosluk -1 EQ m_ui->doubleSpinBox_soldan_bosluk->value()) {
        return;
    }

    if (satiri EQ 0) {
        satiri++;
    }
    if (soldan_bosluk EQ 0) {
        soldan_bosluk++;
    }

    TASARIM_DEGISKENI *degisken = GET_DEGISKEN( p_degisken_id );

    int index = degisken->GET_PRIMARY_ID();

    SET_USER_VARIABLE_POS(soldan_bosluk, satiri, index, m_ui->doubleSpinBox_soldan_bosluk, m_ui->doubleSpinBox_satir_sayisi );

    if ( degisken->GET_GRUP() EQ LINE) {
        //! Satirda degistirilen
        double current_line_y = degisken->GET_POS_Y();

        //! Satir degiskenleri tespit ediliyor.
        QList<TASARIM_DEGISKENI*> satir_degiskenleri = GET_LINE_VARIABLE_LIST();

        for (int i = 0 ; i < satir_degiskenleri.size() ; i++) {

            TASARIM_DEGISKENI* satir_degiskeni = satir_degiskenleri.at(i);

            if ( degisken EQ satir_degiskeni ) {
                continue;
            }
            if ( satir_degiskeni->IS_VISIBLE() EQ false) {
                continue;
            }

            //! Satirdaki labeller sirayla hareket ettiriliyor.
            QLabel * label = satir_degiskeni->GET_LABEL();
            double line_x_pos = satir_degiskeni->GET_POS_X();

            satir_degiskeni->SET_POS_Y( current_line_y );

            if (label NE NULL ) {
                m_design_widget->MOVE_VARIABLE( label, CONVERT_SOL_BOSLUK_PX_X(line_x_pos ), CONVERT_SATIR_BOSLUK_PX_Y(current_line_y), true);
            }
            else {
                m_design_widget->MOVE_SELECTED_VARIABLE( CONVERT_SOL_BOSLUK_PX_X(line_x_pos ), CONVERT_SATIR_BOSLUK_PX_Y(current_line_y), true);
            }
        }
    }

    OPEN_ALL_EVENTS();
}

/**************************************************************************************
                   TASARIM_DOTMATRIX_BATCH::SET_POS_SELECTED_LABEL
***************************************************************************************/

void TASARIM_BATCH::CONSOLE_POS_SELECTED_LABEL_DOT_MATRIX(double x,double y,QLabel * label)
{
    if ( label EQ NULL ) {
        label = m_design_widget->GET_SELECTED_LABEL();
        if ( label EQ NULL ) {
            return;
        }
    }

    int soldan_bosluk = x;
    int satiri        = y;

    if (x EQ -1 AND y EQ -1) {
        soldan_bosluk = m_ui->doubleSpinBox_soldan_bosluk->value();
        satiri        = m_ui->doubleSpinBox_satir_sayisi->value();
    }

    if (label NE NULL) {
        label = m_design_widget->MOVE_VARIABLE(label,CONVERT_SOL_BOSLUK_PX_X(soldan_bosluk),CONVERT_SATIR_BOSLUK_PX_Y(satiri),true);
    }
    else {
        label = m_design_widget->MOVE_SELECTED_VARIABLE(CONVERT_SOL_BOSLUK_PX_X(soldan_bosluk),CONVERT_SATIR_BOSLUK_PX_Y(satiri),true);
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN(label);
    SET_USER_VARIABLE_POS( soldan_bosluk, satiri, degisken->GET_PRIMARY_ID(), m_ui->doubleSpinBox_soldan_bosluk, m_ui->doubleSpinBox_satir_sayisi );
}


/**************************************************************************************
                   TASARIM_BATCH::CONVERT_SOL_BOSLUK_PX_X
***************************************************************************************/

int TASARIM_BATCH::CONVERT_SOL_BOSLUK_PX_X(int satirdaki_bosluk_sayisi )
{
    int px_x = ROUND(QVariant(satirdaki_bosluk_sayisi).toDouble() * (QVariant(m_ui->frame_design_widget->width()).toDouble() / QVariant(m_belge_toplam_karakter_sayisi).toDouble()));

    return px_x;
}

/**************************************************************************************
                   TASARIM_BATCH::CONVERT_SATIR_BOSLUK_PX_Y
***************************************************************************************/

int TASARIM_BATCH::CONVERT_SATIR_BOSLUK_PX_Y(int satir)
{
    double px_y = ROUND(QVariant(satir).toDouble() * (QVariant(m_ui->frame_design_widget->height()).toDouble() / QVariant(m_belge_satir_sayisi).toDouble()));

    return px_y;
}

/**************************************************************************************
                   TASARIM_BATCH::SET_DEGISKEN_POS_STRUCT
***************************************************************************************/

void TASARIM_BATCH::SET_DEGISKEN_POS_STRUCT_DOT_MATRIX ( ADAK_BELGE_DEFAULTS_STRUCT *belge_struct, int index, double pos_x, double pos_y )
{
    if (pos_x NE -1.00) {
        m_ui->doubleSpinBox_soldan_bosluk->setValue(QVariant(pos_x).toInt());
        belge_struct[index].x_position = pos_x;
    }
    if (pos_y NE -1.00) {
        m_ui->doubleSpinBox_satir_sayisi->setValue(QVariant(pos_y).toInt());
        belge_struct[index].y_position = pos_y;
    }
}

/**************************************************************************************
                   TASARIM_BATCH::HIDE_LASER_WIDGETS
***************************************************************************************/

void TASARIM_BATCH::HIDE_LASER_WIDGETS()
{

    m_ui->label_soldan_bosluk_laser->setVisible(false);
    m_ui->doubleSpinBox_x->setVisible(false);
    m_ui->label_mm_soldan_lazer->setVisible(false);

    m_ui->label_usten_bosluk_laser->setVisible(false);
    m_ui->doubleSpinBox_y->setVisible(false);
    m_ui->label_mm_ust_laser->setVisible(false);

    m_ui->label_yazi_tipi_boyutu->setVisible(false);
    m_ui->spinBox_font_size->setVisible(false);
    m_ui->label_font_size->setVisible(false);

    m_ui->label_font_family->setVisible(false);
    m_ui->pushButton_font_family->setVisible(false);
    m_ui->label_font_family_null->setVisible(false);

    m_ui->pushButton_text_bold->setVisible(false);
    m_ui->pushButton_text_under_line->setVisible(false);
    m_ui->pushButton_text_italic->setVisible(false);
    m_ui->pushButton_text_strikeout->setVisible(false);

    m_ui->label_picture_width->setVisible(false);
    m_ui->spinBox_picture_width->setVisible(false);
    m_ui->label_picture_mm_1->setVisible(false);

    m_ui->label_picture_height->setVisible(false);
    m_ui->spinBox_picture_height->setVisible(false);
    m_ui->label_picture_mm_2->setVisible(false);
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_NLQ
***************************************************************************************/

void TASARIM_BATCH::SLOT_NLQ()
{
    if ( m_design_widget->GET_LAST_MOVE_LABEL() EQ NULL ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    degisken->SET_NLQ(m_ui->checkBox_nlq_aktif_mi->isChecked());
}

/**************************************************************************************
                   TASARIM_BATCH::SLOT_DOT_MATRIX_MODE
***************************************************************************************/

void TASARIM_BATCH::SLOT_DOT_MATRIX_MODE( bool p_state )
{
    if ( p_state EQ false ) {
        return;
    }
    if ( m_design_widget->GET_LAST_MOVE_LABEL() EQ NULL ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( m_design_widget->GET_LAST_MOVE_LABEL() );

    QRadioButton *radio = static_cast<QRadioButton*>(sender());

    QLabel label;

    QFont font = degisken->GET_LABEL()->font();
    if ( radio->text() EQ "Normal") {
        degisken->SET_DOT_MARIX_MODE(NORMAL_MODE);
        font.setPointSize(label.font().pointSize());
        degisken->SET_FONT_SIZE(font.pointSize());
    }
    else if ( radio->text() EQ "Condense") {
        ADAK_SET_FONT_SIZE(&label, CONDANSE_MODE_CARPAN);
        degisken->SET_DOT_MARIX_MODE(CONDENSED_MODE);
        font.setPointSize(label.font().pointSize());
        degisken->SET_FONT_SIZE(font.pointSize());
    }
    else if ( radio->text() EQ "Large") {
        ADAK_SET_FONT_SIZE(&label, LARGE_MODE_CARPAN);
        degisken->SET_DOT_MARIX_MODE(LARGE_MODE);
        font.setPointSize(label.font().pointSize());
        degisken->SET_FONT_SIZE(font.pointSize());
    }

    degisken->GET_LABEL()->setFont(font);

    SATIR_BOSLUKLARINI_HESAPLA(degisken->GET_PRIMARY_ID());
}

/**************************************************************************************
                   TASARIM_BATCH::SATIR_BOSLUKLARINI_HESAPLA
***************************************************************************************/

void TASARIM_BATCH::SATIR_BOSLUKLARINI_HESAPLA(int p_degisken_index)
{
    TASARIM_DEGISKENI *degisken = m_all_variables.at(p_degisken_index);

    int degisken_index = degisken->GET_PRIMARY_ID();

    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {

        int aranan_satir = degisken->GET_SATIR();
        QList<TASARIM_DEGISKENI*> satirdaki_degiskenler;
        for (int i = 0; i < m_all_variables.size(); ++i) {
            TASARIM_DEGISKENI *aranan_degisken = m_all_variables.at(i);
            if ( aranan_degisken->GET_SATIR() EQ aranan_satir ) {
                if ( m_silinecek_degiskenler.contains(aranan_degisken->GET_PRIMARY_ID()) EQ false ) {
                    satirdaki_degiskenler.append(aranan_degisken);
                }
            }
        }
        //! hesaplama

        //! AYNI SATIRDAKI DEGISKENLERI SIRALA ASC
        DEGISLENLERI_SIRALA(&satirdaki_degiskenler);

        // goto
        int with_space = 0;
        int with_txt   = 0;
        for (int i = 0; i < satirdaki_degiskenler.size(); ++i) {

            TASARIM_DEGISKENI *satir_degisken = satirdaki_degiskenler.at(i);
            m_design_widget->SELECT_VARIABLE(satir_degisken->GET_LABEL());

            with_space += FONT2PX_X("", satir_degisken->GET_FONT_SIZE(), satir_degisken->GET_SOLDAN_BOSLUK() );

            if ( i NE 0 ) {
                if ( satir_degisken->GET_GRUP() EQ USER_TEXT ) {
                    with_txt   += FONT2PX_X(satirdaki_degiskenler.at(i-1)->GET_TEXT(), satirdaki_degiskenler.at(i-1)->GET_FONT_SIZE(), 0);
                }
                else {
                    with_txt   += FONT2PX_X(satirdaki_degiskenler.at(i-1)->GET_DISPLAY_TXT(), satirdaki_degiskenler.at(i-1)->GET_FONT_SIZE(), 0);
                }
            }

            double mm_x_space = PX_X2MM_X(with_space);
            double mm_x       = PX_X2MM_X(with_txt);
            m_ui->doubleSpinBox_x->setValue( mm_x + mm_x_space ) ;
        }
    }

    CREATE_DESIGN_WIDGET();
    m_design_widget->SELECT_VARIABLE(m_all_variables.at(degisken_index)->GET_LABEL());
}

/**************************************************************************************
                   TASARIM_BATCH::FIND_SATIR_AND_BOSLUK
***************************************************************************************/

void TASARIM_BATCH::FIND_SATIR_AND_BOSLUK( int p_degisken_index, double mm_x, double mm_y )
{
    if ( mm_x EQ -1 OR mm_y EQ -1 ) {
        return;
    }

    // MM -> PX
    double px_y = MM_Y2PX_Y(mm_y);

    int satir  = ROUND( px_y * ( QVariant(m_belge_satir_sayisi).toDouble() / QVariant(m_ui->frame_design_widget->height()).toDouble()));
    int bosluk = SOLDAN_BOSLUK_HESAPLA(p_degisken_index, satir, mm_x);

    if ( bosluk EQ m_ui->doubleSpinBox_soldan_bosluk->value() AND satir EQ m_ui->doubleSpinBox_satir_sayisi->value() ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = m_all_variables.at(p_degisken_index);

    degisken->SET_SATIR(satir);
    bosluk = SOLDAN_BOSLUK_HESAPLA(p_degisken_index, satir, mm_x);
    degisken->SET_SOLDAN_BOSLUK(bosluk);

    m_ui->doubleSpinBox_satir_sayisi->setValue(satir);

    m_slot_change_sol_bosluk_enable = false;
    m_ui->doubleSpinBox_soldan_bosluk->setValue(bosluk);
    m_slot_change_sol_bosluk_enable = true;

}

/**************************************************************************************
                   TASARIM_BATCH::SOLDAN_BOSLUK_HESAPLA
***************************************************************************************/

int TASARIM_BATCH::SOLDAN_BOSLUK_HESAPLA(int p_degisken_index, int p_satir, double p_x )
{
    TASARIM_DEGISKENI *degisken = m_all_variables.at(p_degisken_index);

    QList<TASARIM_DEGISKENI*> satirdaki_degiskenler;
    for (int i = 0; i < m_all_variables.size(); ++i) {
        TASARIM_DEGISKENI *aranan_degisken = m_all_variables.at(i);
        if ( aranan_degisken->GET_SATIR() EQ p_satir ) {
            if ( m_silinecek_degiskenler.contains(aranan_degisken->GET_PRIMARY_ID()) EQ true ) {
                continue;
            }
            if ( aranan_degisken EQ degisken ) {
                continue;
            }
            satirdaki_degiskenler.append(aranan_degisken);
        }
    }

    DEGISLENLERI_SIRALA(&satirdaki_degiskenler);

    TASARIM_DEGISKENI * onceki_degisken = degisken;
    for (int i = 0; i < satirdaki_degiskenler.size(); ++i) {
        if ( satirdaki_degiskenler.at(i)->GET_POS_X() < degisken->GET_POS_X() ){
            onceki_degisken = satirdaki_degiskenler.at(i);
        }
    }

    double onceki_px_x = 0.00;

    if ( satirdaki_degiskenler.size() NE 0 AND onceki_degisken NE degisken ) {
        onceki_px_x   = MM_X2PX_X(onceki_degisken->GET_POS_X());
        if ( onceki_degisken->GET_GRUP() EQ USER_TEXT ) {
            onceki_px_x += FONT2PX_X(onceki_degisken->GET_TEXT(), onceki_degisken->GET_FONT_SIZE(), 0);
        }
        else {
            onceki_px_x += FONT2PX_X(onceki_degisken->GET_DISPLAY_TXT(), onceki_degisken->GET_FONT_SIZE(), 0);
        }
    }

    double px_x = MM_X2PX_X(p_x);

    int bir_karakter_px = FONT2PX_X("X", degisken->GET_FONT_SIZE(), 0);

    return (int)( (px_x - onceki_px_x )/bir_karakter_px);
}

/**************************************************************************************
                   TASARIM_BATCH::PX_X2MM_X
***************************************************************************************/

double TASARIM_BATCH::PX_X2MM_X(double p_px_x)
{
    return p_px_x * ( m_belge_width  / QVariant(m_ui->frame_design_widget->width()).toDouble());
}

/**************************************************************************************
                   TASARIM_BATCH::MM_X2PX_X
***************************************************************************************/

double TASARIM_BATCH::MM_X2PX_X(double p_mm_x)
{
    return p_mm_x / ( QVariant(m_belge_width).toDouble() / QVariant(m_ui->frame_design_widget->width()).toDouble());
}

/**************************************************************************************
                   TASARIM_BATCH::MM_Y2PX_Y
***************************************************************************************/

double TASARIM_BATCH::MM_Y2PX_Y(double p_mm_y)
{
    return p_mm_y / ( QVariant(m_belge_height).toDouble() / QVariant(m_ui->frame_design_widget->height()).toDouble());
}

/**************************************************************************************
                   TASARIM_BATCH::FONT2PX_X
***************************************************************************************/

int TASARIM_BATCH::FONT2PX_X( QString p_str, int p_font_size, int p_pre_space_size )
{
    QFont font("Monospace", p_font_size);
    QFontMetrics fm(font);

    if ( p_str.isEmpty() EQ true AND p_pre_space_size EQ 0 ) {
        return 0;
    }

    for (int i = 0; i < p_pre_space_size; ++i) {
        p_str.prepend("X"); // space yerine kullandim
    }

    return fm.width(p_str);
}

/**************************************************************************************
                   TASARIM_BATCH::FONT2PX_Y
***************************************************************************************/

int TASARIM_BATCH::FONT2PX_Y(int p_font_size)
{
    QFont font("Monospace", p_font_size);
    QFontMetrics fm(font);

    return fm.height();
}
