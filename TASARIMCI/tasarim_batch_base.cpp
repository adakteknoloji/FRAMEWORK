#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include "belge_kernel_utils.h"
#include "belge_kernel_defines.h"
#include "tasarim_batch_base.h"
#include "tasarim_degiskeni.h"

/**************************************************************************************
                   TASARIM_BATCH_BASE::TASARIM_BATCH_BASE
***************************************************************************************/

TASARIM_BATCH_BASE::TASARIM_BATCH_BASE(QWidget * parent) : BATCH_KERNEL (parent)
{
    m_fare_hassasiyeti = 0.99;
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::SAYFA_DEGISKENLERINI_EKLE
***************************************************************************************/

void TASARIM_BATCH_BASE::SAYFA_YAZI_BUTONU_EKLE( QPushButton * p_button )
{
    m_push_button_text_ekle = p_button;
    m_push_button_text_ekle->setIcon(QIcon(":/kernel_icons/kernel_ekle_ikonu.png"));

    connect( m_push_button_text_ekle, SIGNAL( clicked() ), this, SLOT(SLOT_TEXT_EKLE_BUTTON_CLICKED() ));
}

void TASARIM_BATCH_BASE::SAYFA_RESIM_BUTONU_EKLE(QPushButton *p_button, int printer_type)
{
    if (printer_type NE DOT_MATRIX_PRINTER) {
        m_push_button_resim_ekle = p_button;
        m_push_button_resim_ekle->setIcon(QIcon(":/kernel_icons/kernel_ekle_ikonu.png"));

        connect( m_push_button_resim_ekle, SIGNAL(clicked()), this, SLOT(SLOT_RESIM_EKLE_BUTTON_CLICKED() ));
    }
}

void TASARIM_BATCH_BASE::SAYFA_SIL_BUTONU_EKLE(QPushButton *p_button)
{
    m_push_button_sil = p_button;
    m_push_button_sil->setIcon(QIcon(":/kernel_icons/kernel_sil_ikonu.png"));

    connect(m_push_button_sil, SIGNAL(clicked()), this, SLOT( SLOT_SECILI_DEGISKENI_SIL() ));
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::RESIM_EKLE_BUTTON_CLICKED
***************************************************************************************/

void TASARIM_BATCH_BASE::SLOT_RESIM_EKLE_BUTTON_CLICKED()
{
    m_design_widget->RESET_VARIABLE();

    QString file_name = QFileDialog::getOpenFileName( NULL, tr("Open Picture"), "", tr("Picture File (*.png *.jpg *.bmp)"));

    if ( file_name.isEmpty() EQ true ) {
        return;
    }

    QLabel * user_label = m_design_widget->CREATE_NEW_LABEL("");

    user_label->setPixmap(file_name);
    user_label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored  );
    user_label->setScaledContents(true);

    TASARIM_DEGISKENI * degisken =  new TASARIM_DEGISKENI(true);

    degisken->SET_TASARIM_ID( m_tasarim_id );
    degisken->SET_TEXT( file_name );
    degisken->SET_GRUP( USER_IMG );
    degisken->SET_BELGE_DEGISKEN_ID( -1 );
    degisken->SET_FONT_SIZE( 13 );
    degisken->SET_ALIGN( 10 );
    degisken->SET_VISIBLE( true );
    degisken->SET_POS_X( 0.00 );
    degisken->SET_POS_Y( 0.00 );
    degisken->SET_SIZE_X( 30 );
    degisken->SET_SIZE_Y( 30 );
    degisken->SET_IMAGES( *user_label->pixmap() );
    degisken->SET_LABEL( user_label );
    degisken->SET_DEGISKEN_ID(-1);

    user_label->setObjectName( QString::number(degisken->GET_PRIMARY_ID() ) + "\t"+ QString::number( degisken->GET_GRUP() ) );

    m_all_variables.append( degisken );

    m_design_widget->ADD_NEW_VARIABLE(0, 0, user_label);
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::TEXT_EKLE_BUTTON_CLICKED
***************************************************************************************/

void TASARIM_BATCH_BASE::SLOT_TEXT_EKLE_BUTTON_CLICKED()
{
    m_design_widget->RESET_VARIABLE();

    QDialog     * dialog      = new QDialog;
    QGridLayout * box_layout  = new QGridLayout;
    QLabel      * label       = new QLabel;
    QLineEdit   * line_edit   = new QLineEdit;
    QComboBox   * combobox_grup = new QComboBox;
    QPushButton * push_button = new QPushButton("Yazı Ekle");

    line_edit->setMinimumWidth(350);

    combobox_grup->addItem(tr("Add the Form "));
    combobox_grup->addItem("Add the Line");

    dialog->setLayout     (box_layout);
    box_layout->addWidget (label,         0, 0);
    box_layout->addWidget (line_edit,     0, 1);
    box_layout->addWidget (push_button,   0, 2);
    box_layout->addWidget (combobox_grup, 1, 1);

    combobox_grup->setVisible(false);

    connect( push_button, SIGNAL(clicked()), dialog, SLOT(close()));

    dialog->exec();

    if ( line_edit->text().isEmpty() EQ true ) {
        return;
    }

//    int text_grup = combobox_grup->currentIndex();

    QLabel * user_label = m_design_widget->CREATE_NEW_LABEL(line_edit->text());

    TASARIM_DEGISKENI * degisken =  new TASARIM_DEGISKENI(true);

    degisken->SET_TASARIM_ID( m_tasarim_id );
    degisken->SET_TEXT( line_edit->text() );
    degisken->SET_TEXT_SIZE( line_edit->text().size() ); // txt uzunlugu kadar
    degisken->SET_GRUP( USER_TEXT);

//    if ( text_grup EQ 0 ) {
//        degisken->SET_GRUP( USER_TEXT);
//    }
//    else {
//        degisken->SET_GRUP(LINE);
//    }
    degisken->SET_BELGE_DEGISKEN_ID( -1 );
    degisken->SET_DEGISKEN_ID(-1);

    degisken->SET_FONT_SIZE( 11 );
    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
        degisken->SET_FONT_SIZE( 10 );
    }

    degisken->SET_ALIGN( 10 );
    degisken->SET_VISIBLE( true );
    degisken->SET_POS_X( 0.00 );
    degisken->SET_POS_Y( 0.00 );
    degisken->SET_SIZE_X( 0 );
    degisken->SET_SIZE_Y( 0 );

    degisken->SET_LABEL( user_label );

    if ( m_printer_type EQ DOT_MATRIX_PRINTER ) {
        degisken->SET_FONT_FAMILY("Monospace");
    }

    QFont font = user_label->font();
    font.setFamily(degisken->GET_FONT_FAMILY());
    user_label->setFont(font);

    user_label->setObjectName( QString::number( degisken->GET_PRIMARY_ID())+ "\t"+QString::number( degisken->GET_GRUP() ) );

    m_all_variables.append( degisken );

    m_design_widget->ADD_NEW_VARIABLE( 0, 0, user_label );

}

/**************************************************************************************
                   TASARIM_BATCH_BASE::SLOT_SECILI_DEGISKENI_SIL
***************************************************************************************/

void TASARIM_BATCH_BASE::SLOT_SECILI_DEGISKENI_SIL()
{
    QLabel * selected_label = m_design_widget->GET_SELECTED_LABEL();

    if ( selected_label EQ NULL ) {
        return;
    }

    TASARIM_DEGISKENI *degisken = GET_LABEL_DEGISKEN( selected_label );

    int secim = MSG_YES_NO( tr("<b>%1</b> The variable will be deleted.Do you confirm?").arg( GET_PIXMAP_FILE_NAME( degisken->GET_TEXT()) ), m_design_widget );
//<b>%1</b> Değişkeni Silinicektir. Onaylıyormusunuz ?
    if ( secim EQ ADAK_YES ) {

        m_silinecek_degiskenler << degisken->GET_PRIMARY_ID();

        m_design_widget->DELETE_SELECTED_VARIABLE();

        //m_all_variables.removeAt( degisken->GET_PRIMARY_ID() );

        degisken->SET_SILINECEK( true );
        degisken->SET_VISIBLE(false);

        emit SIGNAL_VARIABLE_DELETED(selected_label);
    }
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::SET_USER_VARIABLE_POS
***************************************************************************************/

void TASARIM_BATCH_BASE::SET_USER_VARIABLE_POS(double x, double y, int index, QDoubleSpinBox * p_new_pos_x , QDoubleSpinBox * p_new_pos_y)
{
    TASARIM_DEGISKENI *degisken = m_all_variables.at(index);

    if (x > ( degisken->GET_POS_X() - m_fare_hassasiyeti) AND x < ( degisken->GET_POS_X() + m_fare_hassasiyeti) ) {
        x = degisken->GET_POS_X();
    }
    if (y > ( degisken->GET_POS_Y() - m_fare_hassasiyeti) AND y < ( degisken->GET_POS_Y() + m_fare_hassasiyeti) ) {
        y = degisken->GET_POS_Y();
    }

    degisken->SET_POS_X( x );
    degisken->SET_POS_Y( y );

    if (p_new_pos_x NE NULL ) {
        p_new_pos_x->setValue(x);
    }

    if (p_new_pos_y NE NULL ) {
        p_new_pos_y->setValue(y);
    }
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::GET_PIXMAP_FILE_NAME
***************************************************************************************/

QString TASARIM_BATCH_BASE::GET_PIXMAP_FILE_NAME(QString image_path)
{
    QString image_name = image_path.split("/").last().split(".").at(0);

    return image_name;
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::GET_LABEL_DEGISKEN
***************************************************************************************/

TASARIM_DEGISKENI * TASARIM_BATCH_BASE::GET_LABEL_DEGISKEN(QLabel *p_label)
{
    for (int i = 0; i < m_all_variables.size(); ++i) {
        if ( m_all_variables.at(i)->GET_LABEL() EQ p_label ) {
            return m_all_variables.at(i);
        }
    }

    return NULL;
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::GET_DEGISKEN
***************************************************************************************/

TASARIM_DEGISKENI * TASARIM_BATCH_BASE::GET_DEGISKEN(int p_degisken_id)
{
    for (int i = 0; i < m_all_variables.size(); ++i) {
        if ( m_all_variables.at(i)->GET_PRIMARY_ID() EQ p_degisken_id ) {
            return m_all_variables.at(i);
        }
    }

    return NULL;
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::GET_DEGISKEN
***************************************************************************************/

TASARIM_DEGISKENI * TASARIM_BATCH_BASE::GET_DEGISKEN( int p_grup, int p_index)
{
    for (int i = 0; i < m_all_variables.size(); ++i) {
        if ( m_all_variables.at(i)->GET_GRUP() EQ p_grup AND m_all_variables.at(i)->GET_PRIMARY_ID() EQ p_index  ) {
            return m_all_variables.at(i);
        }
    }

    return NULL;
}

/**************************************************************************************
                   TASARIM_BATCH_BASE::GET_LINE_VARIABLE_LIST
***************************************************************************************/

QList<TASARIM_DEGISKENI*> TASARIM_BATCH_BASE::GET_LINE_VARIABLE_LIST()
{
    QList<TASARIM_DEGISKENI*> satir_dergiskenleri;
    for (int i = 0; i < m_all_variables.size(); ++i) {
        if ( m_all_variables.at(i)->GET_GRUP() EQ LINE ) {
            satir_dergiskenleri.append( m_all_variables.at(i));
        }
    }

    return satir_dergiskenleri;
}
