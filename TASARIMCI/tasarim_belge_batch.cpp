#include <QAction>
#include "tasarim.h"
#include "tasarim_belge_batch.h"
#include "kernel_utils.h"
#include "ui_tasarim_belge_batch.h"
#include "adak_sql.h"
#include "belge_kernel_utils.h"

extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_TASARIM_BELGE_BATCH
***************************************************************************************/

void OPEN_TASARIM_BELGE_BATCH ( int p_tasarim_id , QWidget * parent )
{
    TASARIM_BELGE_BATCH * F = new TASARIM_BELGE_BATCH ( p_tasarim_id, parent);
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::TASARIM_BELGE_BATCH
***************************************************************************************/

TASARIM_BELGE_BATCH::TASARIM_BELGE_BATCH( int p_tasarim_id, QWidget *parent) : BATCH_KERNEL(parent), m_ui( new Ui:: TASARIM_BELGE_BATCH )
{
    m_ui->setupUi(this);

    m_belge_id     = p_tasarim_id;
    m_printer_type = LAZER_PRINTERS;

    START_BATCH_KERNEL( this, G_YONETIM_DB);
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::SETUP_FORM
***************************************************************************************/

void TASARIM_BELGE_BATCH::SETUP_FORM ()
{
    SET_FIRST_FOCUS_WIDGET( m_ui->line_edit_tasarim_adi);

    REGISTER_BUTTON_WIDGET( m_ui->widget_navigation_buttons, true );

    CLEAR_FORM_MEMBERS();

    SET_PAGE_TITLE(tr("DOCUMENT PROPERTIES"));
    SET_HELP_PAGE (tr("document_design"));
    SET_AUTO_EXIT_BATCH ( true );

    m_ui->comma_edit_row_count->SET_PRECISION(0);
    m_ui->comma_edit_copy_count->SET_PRECISION(0);
    m_ui->comma_edit_space_between_rows->SET_PRECISION(0);
    m_ui->comma_edit_space_between_rows->SET_RETURN_EMPTY(false);

    m_ui->widget_belge_boyutlari_satir->hide();

    m_ui->line_edit_tasarim_adi->setMaxLength(30);
    m_ui->comma_edit_belge_satir_sayisi->SET_PRECISION(0);
    m_ui->comma_edit_satirdaki_toplam_karakter_sayisi->SET_PRECISION(0);
    m_ui->comma_edit_satirlar_arasi_satir_sayisi->SET_PRECISION(0);

    QAction * action_usr_variables = new QAction(this);
    action_usr_variables->setShortcut(QKeySequence("Ctrl+7"));
    connect( action_usr_variables ,SIGNAL(triggered()) ,this ,SLOT (CREATE_CPP_CODE_USER_VARIABLES()));
    addAction(action_usr_variables);

    m_ui->label_belge_turu->setText( GET_BELGE_ADI() );

    GET_RECORD( m_belge_id );

    if ( m_printer_type EQ LAZER_PRINTERS ) {
        m_ui->widget_belge_boyutlari_mm->show();
        m_ui->widget_belge_boyutlari_satir->hide();
    }
    else {
        m_ui->widget_belge_boyutlari_mm->hide();
        m_ui->widget_belge_boyutlari_satir->show();
    }
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::NEW_RECORD
***************************************************************************************/

void  TASARIM_BELGE_BATCH::NEW_RECORD()
{

}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::CLEAR_FORM_MEMBERS
***************************************************************************************/

void TASARIM_BELGE_BATCH::CLEAR_FORM_MEMBERS()
{
    m_ui->line_edit_tasarim_adi->clear();

    m_ui->comma_edit_belge_genisligi->SET_INTEGER(210);
    m_ui->comma_edit_belge_yuksekligi->SET_INTEGER(270);
    m_ui->comma_edit_copy_count->SET_INTEGER(1);
    m_ui->comma_edit_row_count->SET_INTEGER(1);
    m_ui->comma_edit_space_between_rows->SET_INTEGER(0);
    m_ui->comma_edit_space_between_rows->setDisabled(true);

    m_ui->comma_edit_belge_satir_sayisi->clear();
    m_ui->comma_edit_satirdaki_toplam_karakter_sayisi->clear();
    m_ui->comma_edit_satirlar_arasi_satir_sayisi->clear();

    m_ui->widget_belge_boyutlari_mm->show();
    m_ui->widget_belge_boyutlari_satir->hide();

    SET_TASARIM_ADI("");
    SET_TASARIM_READONLY_VALUE(0);
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::GET_RECORD
***************************************************************************************/

int TASARIM_BELGE_BATCH::GET_RECORD(int record_id)
{
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari",
                         "tasarim_adi,html_str,belge_width,belge_height,kopya_sayisi,"
                         "kopya_konumu, row_count,row_space_mm,row_space_satir, "
                         "belge_satir_sayisi,belge_toplam_karakter_sayisi,printer_type,readonly",
                         "tasarim_id=:tasarim_id","");

    query.SET_VALUE(":tasarim_id",record_id);

    if (query.SELECT() EQ 0) {
        return ADAK_FAIL;
    }

    SET_TASARIM_ID(record_id);    
    query.NEXT();

    m_ui->line_edit_tasarim_adi->setText(query.VALUE(0).toString());
    SET_TASARIM_ADI (query.VALUE(0).toString());

    m_ui->comma_edit_copy_count->SET_INTEGER(query.VALUE(4).toInt());
    m_ui->comma_edit_row_count->SET_INTEGER(query.VALUE(6).toInt());

    m_printer_type = query.VALUE("printer_type").toInt();

    m_ui->label_yazici_turu->setText( GET_PRINTER_TYPE_STR(m_printer_type) );

    SET_TASARIM_READONLY_VALUE(query.VALUE("readonly").toInt());

    if ( m_printer_type EQ LAZER_PRINTERS ) {
        m_ui->comma_edit_belge_genisligi->SET_DOUBLE(query.VALUE(2).toDouble());
        m_ui->comma_edit_belge_yuksekligi->SET_DOUBLE(query.VALUE(3).toDouble());

        if (m_ui->comma_edit_row_count->GET_INTEGER() > 1 ) {
            m_ui->comma_edit_space_between_rows->SET_INTEGER(query.VALUE(7).toDouble());
            m_ui->comma_edit_space_between_rows->setEnabled(true);
        }

        m_ui->widget_belge_boyutlari_mm->show();
        m_ui->widget_belge_boyutlari_satir->hide();
        return ADAK_OK;
    }
    m_ui->comma_edit_belge_satir_sayisi-> SET_INTEGER(query.VALUE("belge_satir_sayisi").toInt());
    m_ui->comma_edit_satirdaki_toplam_karakter_sayisi-> SET_INTEGER(query.VALUE("belge_toplam_karakter_sayisi").toInt());
    m_ui->comma_edit_satirlar_arasi_satir_sayisi-> SET_INTEGER(query.VALUE("row_space_satir").toInt());
    m_ui->widget_belge_boyutlari_mm->hide();
    m_ui->widget_belge_boyutlari_satir->show();

    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::CHECK_VAR
***************************************************************************************/

int TASARIM_BELGE_BATCH::CHECK_VAR (QObject * object)
{
    Q_UNUSED(object);

    if (object EQ m_ui->comma_edit_row_count) {
        if (m_ui->comma_edit_row_count->GET_INTEGER() > 1) {
            m_ui->comma_edit_space_between_rows->setEnabled(true);
        }
        else {
            m_ui->comma_edit_space_between_rows->SET_INTEGER(0);
            m_ui->comma_edit_space_between_rows->setDisabled(true);
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::CHECK_RUN
***************************************************************************************/

int TASARIM_BELGE_BATCH::CHECK_RUN ()
{
    if (m_ui->line_edit_tasarim_adi->text().isEmpty() EQ true) {
        MSG_WARNING(tr("Document name can not be empty. "),m_ui->line_edit_tasarim_adi);//Belge adı boş bırakılamaz.
        return ADAK_FAIL;
    }

    if (m_ui->comma_edit_copy_count->GET_INTEGER() EQ 0) {
        MSG_WARNING(tr("Number of copies field can not be empty. "),m_ui->comma_edit_copy_count);//Kopya sayısı alanı boş bırakılamaz.
        return ADAK_FAIL;
    }

    if ( m_printer_type EQ LAZER_PRINTERS) {
        if (m_ui->comma_edit_belge_genisligi->GET_DOUBLE() EQ 0) {
            MSG_WARNING(tr("Document width field can not be empty."),m_ui->comma_edit_belge_genisligi);//Belge genişliği alanı boş bırakılamaz.
            return ADAK_FAIL;
        }

        if (m_ui->comma_edit_belge_yuksekligi->GET_DOUBLE() EQ 0) {
            MSG_WARNING(tr("Document height field can not be empty."),m_ui->comma_edit_belge_yuksekligi);//Belge yüksekliği alanı boş bırakılamaz.
            return ADAK_FAIL;
        }

        if (m_ui->comma_edit_row_count->GET_INTEGER() EQ 0) {
            MSG_WARNING(tr("Number of line field can not be empty. "),m_ui->comma_edit_row_count);//Satır sayısı boş bırakılamaz.
            return ADAK_FAIL;
        }
    }
    else {
        if (m_ui->comma_edit_belge_satir_sayisi->GET_INTEGER() EQ 0) {
            MSG_WARNING(tr("The number of rows to be printed can not be empty space."),m_ui->comma_edit_belge_satir_sayisi);//Yazdırılacak alandaki satır sayısı boş bırakılamaz.
            return ADAK_FAIL;
        }
        if (m_ui->comma_edit_satirdaki_toplam_karakter_sayisi->GET_INTEGER() EQ 0) {
            MSG_WARNING(tr("The total number of characters in line can not be empty."),m_ui->comma_edit_satirdaki_toplam_karakter_sayisi);//Satırdaki toplam karakter sayısı boş bırakılamaz.
            return ADAK_FAIL;
        }
        if (m_ui->comma_edit_satirlar_arasi_satir_sayisi->GET_INTEGER() EQ 0) {
            MSG_WARNING(tr("The number of lines can not be empty between rows."),m_ui->comma_edit_satirlar_arasi_satir_sayisi);//Satırlar arası satır sayısı boş bırakılamaz.
            return ADAK_FAIL;
        }
    }

    if (GET_IS_TASARIM_READONLY() EQ true) {
        MSG_ERROR(tr("The program of design are not modified. Copying design is necessary for made changes."),NULL);
        //Programa ait tasarımlar üzerinde değişiklik yapılamaz.\nDeğişiklik yapılabilmesi için tasarımın kopyalanması gereklidir.
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::RUN_BATCH
***************************************************************************************/

void TASARIM_BELGE_BATCH::RUN_BATCH()
{

    G_YONETIM_DB->START_TRANSACTION();
    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_UPDATE("ynt_belge_tasarimlari","tasarim_id","tasarim_adi,belge_width,belge_height,kopya_sayisi,kopya_konumu,"
                         "row_count,row_space_mm,printer_type,belge_satir_sayisi,belge_toplam_karakter_sayisi,row_space_satir",
                         "tasarim_id = :tasarim_id");

    query.SET_VALUE(":tasarim_adi"                  , m_ui->line_edit_tasarim_adi->text());
    query.SET_VALUE(":belge_width"                  , m_ui->comma_edit_belge_genisligi->GET_DOUBLE());
    query.SET_VALUE(":belge_height"                 , m_ui->comma_edit_belge_yuksekligi->GET_DOUBLE());
    query.SET_VALUE(":kopya_sayisi"                 , m_ui->comma_edit_copy_count->GET_INTEGER());
    query.SET_VALUE(":row_count"                    , m_ui->comma_edit_row_count->GET_INTEGER());
    query.SET_VALUE(":row_space_mm"                 , m_ui->comma_edit_space_between_rows->GET_INTEGER());
    query.SET_VALUE(":belge_satir_sayisi"           , m_ui->comma_edit_belge_satir_sayisi ->GET_INTEGER());
    query.SET_VALUE(":belge_toplam_karakter_sayisi" , m_ui->comma_edit_satirdaki_toplam_karakter_sayisi->GET_INTEGER());
    query.SET_VALUE(":row_space_satir"              , m_ui->comma_edit_satirlar_arasi_satir_sayisi->GET_INTEGER());
    query.SET_VALUE(":kopya_konumu"                 , 0);
    query.SET_VALUE(":printer_type"                 , m_printer_type );
    query.SET_VALUE(":tasarim_id"                   , m_belge_id );

    query.UPDATE();
    G_YONETIM_DB->COMMIT_TRANSACTION();

    MSG_INFO( tr( "The values updated." ), NULL );
    close();
}

/**************************************************************************************
                   TASARIM_BELGE_BATCH::CREATE_CPP_CODE_USER_VARIABLES
***************************************************************************************/

void TASARIM_BELGE_BATCH::CREATE_CPP_CODE_USER_VARIABLES()
{
    CREATE_CPP_CODE_SAVED_DESIGNS();
}
