#include "tasarim_menu_batch.h"
#include "ui_tasarim_menu_batch.h"
#include "print.h"
#include "belge_kernel_utils.h"
#include "belge_kernel_open.h"
#include "tasarim.h"
#include "tasarim_yeni_tasarim_batch.h"
#include "tasarim_export.h"
#include "tasarim_import.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_TASARIM_MENU_BATCH
***************************************************************************************/

void OPEN_TASARIM_MENU_BATCH ( ADAK_BELGELER_STRUCT * p_belge_struct, int p_belge_count, QWidget * parent )
{
    TASARIM_MENU_BATCH * F = new TASARIM_MENU_BATCH ( p_belge_struct, p_belge_count, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::TASARIM_MENU_BATCH
***************************************************************************************/

TASARIM_MENU_BATCH::TASARIM_MENU_BATCH ( ADAK_BELGELER_STRUCT * p_belge_struct, int p_belge_count, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::TASARIM_MENU_BATCH )
{
    m_ui->setupUi      ( this );

    m_belge_struct = p_belge_struct;
    m_belge_count  = p_belge_count;
    m_tasarim_id   = -1;

    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::SETUP_FORM
***************************************************************************************/

void TASARIM_MENU_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kapat" );
    SET_AUTO_EXIT_BATCH ( true );

    SET_FIRST_FOCUS_WIDGET ( m_ui->tableWidget_tasarimlar );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_run_batch_button );

    SET_PAGE_TITLE    (tr("BELGE TASARIMCI"));
    SET_SETTING_NAME  ("TASARIM_MENU_BATCH");
    SET_HELP_PAGE     ("yonetim-islemleri_belge-tasarimcisi");

    m_ui->pushButton_export->setEnabled(false);
    m_ui->pushButton_import->setEnabled(false);

    m_ui->tableWidget_tasarimlar->setRowCount( m_belge_count );
    m_ui->tableWidget_tasarimlar->setColumnCount( 2 );

    m_ui->tableWidget_tasarimlar->setHorizontalHeaderLabels( QStringList() << tr( "Belge_id" ) << tr( "Belgeler" ) );

    m_ui->tableWidget_tasarimlar->hideColumn( 0 );

    for (int row_count = 0 ; row_count < m_belge_count ; row_count++ ) {
         // belge_id
         QTableWidgetItem* item_belge_id = new QTableWidgetItem;
         item_belge_id->setText( QVariant ( m_belge_struct[row_count].belge_id ).toString() );
         item_belge_id->setFlags( item_belge_id->flags() &~ Qt::ItemIsEditable );
         m_ui->tableWidget_tasarimlar->setItem( row_count, 0, item_belge_id );
         //belge_adi
         QTableWidgetItem* item_belge_adi = new QTableWidgetItem;
         item_belge_adi->setText( m_belge_struct[row_count].belge_adi  );
         item_belge_adi->setFlags( item_belge_adi->flags() &~ Qt::ItemIsEditable );
         m_ui->tableWidget_tasarimlar->setItem( row_count, 1, item_belge_adi );

    }

    //! ILK SECENEK SETLENIYOR
    m_ui->tableWidget_tasarimlar->setCurrentCell( 0, 1);

    SET_BELGE_STRUCTURES( 0, m_belge_struct );
    FILL_ALT_MENU( m_ui->tableWidget_tasarimlar->item( 0, 0 )->text().toInt() );


    connect( m_ui->tableWidget_alt_tasarimlar, SIGNAL( cellClicked      (int,int)), this, SLOT( SLOT_YENI_BELGE( int, int ) ));
    connect( m_ui->tableWidget_alt_tasarimlar, SIGNAL( cellDoubleClicked(int,int)), this, SLOT( SLOT_ALT_TASARIM_DOUBLE_CLICKED(int, int) ));
    connect( m_ui->tableWidget_tasarimlar    , SIGNAL( currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT( SLOT_TASARIM_CHANGE  ( QTableWidgetItem*,QTableWidgetItem* ) ));

}

/**************************************************************************************
                   TASARIM_MENU_BATCH::CHECK_VAR
***************************************************************************************/

int TASARIM_MENU_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->tableWidget_tasarimlar ) {
        int belge_id = m_ui->tableWidget_tasarimlar->item( m_ui->tableWidget_tasarimlar->currentRow(), 0 )->text().toInt();
        int enum_id = m_ui->tableWidget_tasarimlar->currentRow();

        SET_BELGE_STRUCTURES( enum_id , m_belge_struct );
        FILL_ALT_MENU( belge_id );
    }
    else if ( object EQ m_ui->pushButton_export ) {
        if (m_tasarim_id NE -1) {
            OPEN_TASARIM_EXPORT(m_tasarim_id, this );
        }
    }
    else if ( object EQ m_ui->pushButton_import ) {
        if (m_tasarim_id NE -1) {

            SQL_QUERY query(G_YONETIM_DB);
            query.PREPARE_SELECT("ynt_belge_degiskenleri", "belge_degisken_id", "tasarim_id = :tasarim_id");
            query.SET_VALUE     (":tasarim_id", m_tasarim_id );

            if ( query.SELECT() NE 0 ) {
                MSG_WARNING("Tasarımın boş olması gerekir. Lütfen boş bir tasarım seçip tekrar deneyin.", NULL );
                return ADAK_FAIL;
            }

            OPEN_TASARIM_IMPORT(m_tasarim_id, NULL );
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::CHECK_RUN
***************************************************************************************/

int TASARIM_MENU_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::RUN_BATCH
***************************************************************************************/

void TASARIM_MENU_BATCH::RUN_BATCH ()
{
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::CHECK_EXIT
***************************************************************************************/

int TASARIM_MENU_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::FILL_ALT_MENU
***************************************************************************************/

void TASARIM_MENU_BATCH::FILL_ALT_MENU ( int p_belge_id )
{

    m_ui->pushButton_export->setEnabled(false);
    m_ui->pushButton_import->setEnabled(false);

    m_last_belge_id = p_belge_id;

    SQL_QUERY query(G_YONETIM_DB);

    query.PREPARE_SELECT("ynt_belge_tasarimlari","tasarim_id,tasarim_adi, printer_type, readonly","belge_id = :belge_id","");
    query.SET_VALUE(":belge_id", p_belge_id );

    int record_count = query.SELECT("tasarim_adi");
    if( record_count EQ 0 ) {
        return;
    }

    int current_row = -1;

    m_ui->tableWidget_alt_tasarimlar->clear();

    m_ui->tableWidget_alt_tasarimlar->setRowCount( record_count + 1 );
    m_ui->tableWidget_alt_tasarimlar->setColumnCount( 4 );

    m_ui->tableWidget_alt_tasarimlar->hideColumn( 0 );

    m_ui->tableWidget_alt_tasarimlar->setHorizontalHeaderLabels( QStringList() << tr( "tasarim_id" ) << tr( "Tasarım Adı" ) << tr("Yazici Tipi") << tr(""));

    m_ui->tableWidget_alt_tasarimlar->setColumnWidth( 1 , 300 );
    m_ui->tableWidget_alt_tasarimlar->setColumnWidth( 2 , 120 );
    m_ui->tableWidget_alt_tasarimlar->setColumnWidth( 3 , 40  );

    while (query.NEXT()) {
        current_row ++;

        int printer_type = query.VALUE( 2 ).toInt();
        int tasarim_id   = query.VALUE( 0 ).toInt();
        int read_only    = query.VALUE( 3 ).toInt();

        QTableWidgetItem* item_belge_adi = new QTableWidgetItem;
        item_belge_adi->setText( query.VALUE(0).toString() );
        item_belge_adi->setFlags( item_belge_adi->flags() &~ Qt::ItemIsEditable );

        m_ui->tableWidget_alt_tasarimlar->setItem( current_row, 0, item_belge_adi );

        QTableWidgetItem* item_belge_id = new QTableWidgetItem;
        item_belge_id->setText( query.VALUE(1).toString() );
        item_belge_id->setFlags( item_belge_id->flags() &~ Qt::ItemIsEditable );

        m_ui->tableWidget_alt_tasarimlar->setItem( current_row, 1, item_belge_id );

        QString yazici_tipi_str = "";
        if ( printer_type EQ DOT_MATRIX_PRINTER ) {
            yazici_tipi_str = "Nokta Vuruşlu";
        }
        else if ( printer_type EQ  LAZER_PRINTERS ) {
            yazici_tipi_str = "Lazer";
        }
        QTableWidgetItem* item_yazici_tipi = new QTableWidgetItem;
        item_yazici_tipi->setText( yazici_tipi_str );
        item_yazici_tipi->setFlags( item_yazici_tipi->flags() &~ Qt::ItemIsEditable );

        m_ui->tableWidget_alt_tasarimlar->setItem( current_row, 2, item_yazici_tipi );

        QToolButton * delete_button = new QToolButton;
        delete_button->setObjectName( QVariant( tasarim_id ).toString() );

        delete_button->setIcon( QIcon( ":/kernel_icons/kernel_sil_ikonu.png"));

        if ( read_only EQ 1 ) {
            delete_button->setEnabled( false );
        }

        m_ui->tableWidget_alt_tasarimlar->setCellWidget( current_row, 3, delete_button );

        connect( delete_button, SIGNAL( clicked()), this, SLOT( SLOT_TASARIMI_SIL()));
    }

    current_row++;

    QLabel * label_yeni_belge = new QLabel( "<< Yeni Belge Tasarla >>" );
    label_yeni_belge->setStyleSheet( "color: rgb(0, 0, 255);");
    label_yeni_belge->setObjectName( QVariant( p_belge_id ).toString() );

    m_ui->tableWidget_alt_tasarimlar->setCellWidget( current_row, 1, label_yeni_belge );

}

/**************************************************************************************
                   TASARIM_MENU_BATCH::FILL_ALT_MENU
***************************************************************************************/

void TASARIM_MENU_BATCH::SLOT_ALT_TASARIM_DOUBLE_CLICKED( int p_row, int p_colomn )
{
    if ( p_row NE m_ui->tableWidget_alt_tasarimlar->rowCount() -1 ) {

        QString tasarim_adi= m_ui->tableWidget_alt_tasarimlar->item( p_row, p_colomn )->text();
        int     tasarim_id = m_ui->tableWidget_alt_tasarimlar->item( p_row, 0 )->text().toInt();

        SET_TASARIM_ID ( tasarim_id );
        SET_TASARIM_ADI( tasarim_adi );

        SQL_QUERY query(G_YONETIM_DB);

        query.PREPARE_SELECT("ynt_belge_tasarimlari","belge_width, belge_height, printer_type, readonly",
                             "tasarim_id = :tasarim_id" );

        query.SET_VALUE(":tasarim_id", tasarim_id );

        if (query.SELECT() EQ 0) {
            return;
        }

        query.NEXT();

        double belge_genislik  = query.VALUE( "belge_width" ).toDouble();
        double belge_yukseklik = query.VALUE( "belge_height" ).toDouble();
        int    read_only       = query.VALUE( "readonly" ).toInt();
        int    printer_type    = query.VALUE( "printer_type" ).toInt();

        SET_TASARIM_READONLY_VALUE( read_only );

        OPEN_TASARIM_BATCH( belge_genislik, belge_yukseklik, tasarim_id, printer_type, nativeParentWidget());
    }
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::SLOT_BELGE_KOPYALA
***************************************************************************************/

void TASARIM_MENU_BATCH::SLOT_YENI_BELGE( int p_row, int p_colomn)
{
    Q_UNUSED ( p_colomn );
    if ( p_row EQ m_ui->tableWidget_alt_tasarimlar->rowCount() -1 ) {
        QLabel * label = static_cast< QLabel * >( m_ui->tableWidget_alt_tasarimlar->cellWidget( p_row, 1 ) );

        int belge_id = QVariant( label->objectName() ).toInt();

        OPEN_TASARIM_YENI_TASARIM_BATCH( belge_id, NULL );

        FILL_ALT_MENU( belge_id );

        return;
    }

    m_tasarim_id =  m_ui->tableWidget_alt_tasarimlar->item(p_row, 0)->text().toInt();

    m_ui->pushButton_export->setEnabled(true);
    m_ui->pushButton_import->setEnabled(true);
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::SLOT_ALT_TASARIM_CHANGE
***************************************************************************************/

void TASARIM_MENU_BATCH::SLOT_TASARIM_CHANGE( QTableWidgetItem *p_current_item, QTableWidgetItem *p_previous_item )
{
    Q_UNUSED( p_previous_item );

    int row_id = m_ui->tableWidget_tasarimlar->row( p_current_item );

    FILL_ALT_MENU( m_ui->tableWidget_tasarimlar->item( row_id, 0 )->text().toInt() );
}

/**************************************************************************************
                   TASARIM_MENU_BATCH::SLOT_ALT_TASARIM_CHANGE
***************************************************************************************/

void TASARIM_MENU_BATCH::SLOT_TASARIMI_SIL()
{
    QToolButton * delete_button = static_cast< QToolButton *>( sender() );

    int tasarim_id = delete_button->objectName().toInt();

    int secim = MSG_YES_NO( "Tasarımı silinecek devam edilsin mi ?", NULL );

    if ( secim EQ ADAK_YES ) {
        SQL_QUERY query(G_YONETIM_DB);
        G_YONETIM_DB->START_TRANSACTION();
        query.PREPARE_DELETE("ynt_belge_tasarimlari","tasarim_id = :tasarim_id");
        query.SET_VALUE(":tasarim_id", tasarim_id );
        query.DELETE();

        query.PREPARE_DELETE("ynt_belge_degiskenleri","tasarim_id = :tasarim_id");
        query.SET_VALUE(":tasarim_id", tasarim_id );
        query.DELETE();
        G_YONETIM_DB->COMMIT_TRANSACTION();

        FILL_ALT_MENU( m_last_belge_id );
    }
}
