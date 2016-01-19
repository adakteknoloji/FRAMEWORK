#include <QFileDialog>

#include "adak_sql.h"
#include "adak_importer.h"
#include "adak_gruplar.h"

#include "batch_kernel.h"
#include "import_kernel.h"
#include "ui_import_kernel.h"
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include "adak_std_utils.h"

extern ADAK_SQL * DB;

/**************************************************************************************
                   IMPORT_KERNEL::IMPORT_KERNEL
***************************************************************************************/

IMPORT_KERNEL::IMPORT_KERNEL ( QString p_help_page, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::IMPORT_KERNEL )
{
    m_help_page = p_help_page;
    m_ui->setupUi      ( this );
    M_DB    = DB;
    START_BATCH_KERNEL ( this, M_DB );
}

/**************************************************************************************
                   IMPORT_KERNEL::SETUP_FORM
***************************************************************************************/

void IMPORT_KERNEL::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Import") );

    SET_AUTO_EXIT_BATCH ( true );

    SET_FIRST_FOCUS_WIDGET ( m_ui->line_edit_dosya_path );
    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_PAGE_TITLE    ( tr ( "IMPORT" ) );
    SET_SETTING_NAME  ("E9_AKTAR");

    SET_HELP_PAGE     ( m_help_page );

    m_kayitlar_onaylandi_mi = false;

    m_ui->line_edit_dosya_path->setDisabled (true);
    m_ui->line_edit_dosya_path->setMaxLength(250);

    m_ui->table_widget_onizleme->setObjectName("NO_EVENT_ON_WIDGET");
    m_ui->table_widget_kaydedilmisler->setObjectName("NO_EVENT_ON_WIDGET");
    m_ui->table_widget_kaydedilmisler->hide();
    m_ui->label_kaydedilmeyecek_veriler->hide();

    QList< QByteArray > available_coding_list = QTextCodec::availableCodecs() ;
    for( int i = 0 ; i < available_coding_list.size() ; ++i ){
        m_ui->comboBox_dosya_encoding->addItem( available_coding_list.at(i) );
    }

    FIND_AND_SET_COMBOBOX_TEXT(m_ui->comboBox_dosya_encoding, tr("UTF-8"));
}

/**************************************************************************************
                   IMPORT_KERNEL::CHECK_VAR
***************************************************************************************/

int IMPORT_KERNEL::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->push_button_dosya_sec ) {
        QString file_name = "";
        file_name = QFileDialog::getOpenFileName(this,tr("Select File"), "", tr("ODS File(*.ods)"));

        if ( file_name.isEmpty() EQ false ) {
            ADAK_CURSOR_BUSY();

            unzFile hArchive;
            if( ( hArchive = unzOpen(file_name.toUtf8().data()) ) EQ NULL ){
                MSG_INFO(tr("The selected file format is not appropriate!") , this ) ;//Seçilen dosya Formatı uygun değil!
                ADAK_CURSOR_NORMAL();
                return ADAK_FAIL;
            }

            ADAK_IMPORT * adak_import = new ADAK_IMPORT();
            CHECK_MAX_SIZE_KONTROL( adak_import->IMPORT( file_name , m_ui->comboBox_dosya_encoding->currentText(), m_zorunlu_alanlar.size()));

            if( m_import_rows.size() EQ 0 ){
                m_ui->table_widget_onizleme->clear();
                m_kayitlar_onaylandi_mi = false;
                MSG_INFO(tr("The selected file is empty.") ,this);//Seçilen dosyanın içi boştur
                ADAK_CURSOR_NORMAL();
                return ADAK_FAIL;
            }

            m_import_rows.removeFirst(); // ilk satir title
            m_ui->line_edit_dosya_path->setText( file_name );
            if ( ROW_DATA_KONTROL() EQ false ) {
                m_kayitlar_onaylandi_mi = false;
                return ADAK_FAIL;
            }
            m_kayitlar_onaylandi_mi = true;


            ONIZLEMEYI_GOSTER();
            ADAK_CURSOR_NORMAL();
        }
    }
    else if ( object EQ m_ui->push_button_gruplar ) {
        OPEN_GRUP_SECIM(m_program_id,m_modul_id,&m_grup_idleri,DB,this,false);
        m_ui->text_edit_gruplar->setText(GRP_GRUP_ADLARINI_BUL(&m_grup_idleri,new QStringList(),new QStringList()));
    }

    return ADAK_OK;
}

/**************************************************************************************
                   IMPORT_KERNEL::SET_HEADERS
***************************************************************************************/

void IMPORT_KERNEL::SET_HEADERS( QList<QStringList> headers)
{
    QStringList table_widget_header;
    for (int i = 0; i < headers.size(); ++i) {
        QStringList data = headers.at(i);
        table_widget_header << data.at(0);
        m_alan_uzunluklari  << data.at(1).toInt();
        m_zorunlu_alanlar   << data.at(2);
    }

    m_ui->table_widget_onizleme->setColumnCount                    (table_widget_header.size());
    m_ui->table_widget_onizleme->setHorizontalHeaderLabels         (table_widget_header);
    m_ui->table_widget_kaydedilmisler->setColumnCount              (table_widget_header.size());
    m_ui->table_widget_kaydedilmisler->setHorizontalHeaderLabels   (table_widget_header);
}

/**************************************************************************************
                   IMPORT_KERNEL::CHECK_RUN
***************************************************************************************/

int IMPORT_KERNEL::CHECK_RUN ()
{
    if ( m_import_rows.size() EQ 0) {
        MSG_INFO( tr("No data will be saved") , NULL);//Kaydedilecek veri yok
        return ADAK_FAIL;
    }
    else if( m_kayitlar_onaylandi_mi EQ false ){
        MSG_WARNING( tr( "Required fields are left empty ") , this ) ;//Zorunlu Alanlar boş bırakılmış
        return ADAK_FAIL ;
    }
    else {
        ADAK_MSG_ENUM msg_enum = MSG_YES_NO(tr("All information will be transferred to E9.Continue? "),NULL);//Tüm bilgiler E9 a aktarılacaktır. Devam edilsin mi?

        if ( msg_enum NE ADAK_YES ) {
            return ADAK_FAIL;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   IMPORT_KERNEL::RUN_BATCH
***************************************************************************************/

void IMPORT_KERNEL::RUN_BATCH ()
{
    ADAK_CURSOR_BUSY();

    M_DB->START_TRANSACTION();
    for ( int i = 0 ; i < m_import_rows.size(); i++ ) {
        IMPORT_ROW( m_import_rows.at(i) );
    }
    M_DB->COMMIT_TRANSACTION();

    ADAK_CURSOR_NORMAL();

    MSG_INFO(tr("All information transferred. "),NULL);//Tüm bilgiler aktarıldı.

}

/**************************************************************************************
                   IMPORT_KERNEL::ONIZLEMEYI_GOSTER
***************************************************************************************/

void IMPORT_KERNEL::ONIZLEMEYI_GOSTER()
{
    // Kaydelilmeyeye uygun
    TABLO_DOLDUR(m_ui->table_widget_onizleme, m_import_rows );
    m_ui->label_bulunan_kayit_sayisi->setText( QString(tr("Number of Transferred Records: %1")).arg( m_import_rows.size() ) );//Aktarılacak kayıt sayısı

    // Varolan kayitlar

}

/**************************************************************************************
                   IMPORT_KERNEL::SET_PROGRAM_ID
***************************************************************************************/

void IMPORT_KERNEL::SET_PROGRAM_ID(int program_id)
{
    m_program_id = program_id;
}

/**************************************************************************************
                   IMPORT_KERNEL::SET_MODUL_ID
***************************************************************************************/

void IMPORT_KERNEL::SET_MODUL_ID(int modul_id)
{
    m_modul_id  = modul_id;
}

/**************************************************************************************
                   IMPORT_KERNEL::GET_GRUP_IDLERI
***************************************************************************************/

QList<int> * IMPORT_KERNEL::GET_GRUP_IDLERI()
{
    return &m_grup_idleri;
}

/**************************************************************************************
                   IMPORT_KERNEL::TABLO_DOLDUR
***************************************************************************************/

void IMPORT_KERNEL::TABLO_DOLDUR( QTableWidget *table_widget, QList<QStringList> list )
{
    table_widget->setRowCount(0);

    table_widget->setRowCount(list.size());

    for ( int i = 0; i < list.size(); i++ ) {
        for ( int j = 0 ; j < list.at(i).size() ; j++ ) {
            QString cell_data = list.at(i).at(j);
            table_widget->setItem( i, j, new QTableWidgetItem( cell_data ) );
        }
    }

    table_widget->resizeColumnsToContents();
}

/**************************************************************************************
                   IMPORT_KERNEL::SET_LIST_VALUES
***************************************************************************************/

void IMPORT_KERNEL::SET_LIST_VALUES( int p_row_id, int p_at_id, QString p_value )
{
    QStringList  update_row;

    for ( int i = 0; i < m_import_rows.at( p_row_id ).size(); i++ ) {
        if ( i EQ p_at_id ) {
            update_row << p_value;
        }
        else {
            update_row <<  m_import_rows.at( p_row_id ).at( i );
        }
    }

    m_import_rows.replace( p_row_id, update_row );
}

/**************************************************************************************
                   IMPORT_KERNEL::ROW_DATA_KONTROL
***************************************************************************************/

bool IMPORT_KERNEL::ROW_DATA_KONTROL()
{
    for( int i = 0; i < m_import_rows.size(); i++ ) {
        for ( int j = 0; j < m_zorunlu_alanlar.size(); j++ ) {
            // zorunlu alan bos gecilemez
            if ( m_zorunlu_alanlar.at( j ) EQ "*" ) {
                if ( j < m_import_rows.at(i).size() AND m_import_rows.at( i ).at( j ) EQ tr( "@" ) ) {
                    ADAK_CURSOR_NORMAL();
                    MSG_WARNING( QString( tr("In number %1 line, %2 Required fields were left blank")).arg( i ).arg(m_import_rows.at(0).at( j )), NULL );//" %1 Nolu satırında %2 Zorunlu Alan boş bırakılmış
                    return false;
                }
            }
            // eger bos ise default deger ataniyor.
            if ( j < m_import_rows.at(i).size() ) {
                if ( m_import_rows.at( i ).at( j ) EQ tr("@") OR m_import_rows.at( i ).at( j ) EQ tr("") )  {
                    SET_LIST_VALUES( i, j, m_zorunlu_alanlar.at( j ) );
                }
            }
        }
    }

    return true;
}

/**************************************************************************************
                   IMPORT_KERNEL::SET_DB
***************************************************************************************/

void IMPORT_KERNEL::SET_DB(ADAK_SQL *P_DB)
{
    M_DB = P_DB;
}

/**************************************************************************************
                   IMPORT_KERNEL::CHECK_MAX_SIZE_KONTROL
***************************************************************************************/

void IMPORT_KERNEL::CHECK_MAX_SIZE_KONTROL( QList<QStringList> p_import_rows )
{
    for (int i = 0; i < p_import_rows.size(); ++i) {
        QStringList row;
        for (int j = 0; j < m_alan_uzunluklari.size(); ++j) {
            QString cell_data = p_import_rows.at(i).at(j);
            if ( m_alan_uzunluklari.at(j) NE 0 ) {
                if ( cell_data.size() > m_alan_uzunluklari.at(j)) {
                    cell_data = cell_data.mid(0, m_alan_uzunluklari.at(j));
                }
            }
            row << cell_data;
        }

        m_import_rows << row;
    }
}
