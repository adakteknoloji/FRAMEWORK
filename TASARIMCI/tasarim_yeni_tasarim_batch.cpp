#include "tasarim_yeni_tasarim_batch.h"
#include "ui_tasarim_yeni_tasarim_batch.h"
#include "print.h"
#include "tasarim_utils.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_TASARIM_YENI_TASARIM_BATCH
***************************************************************************************/

void OPEN_TASARIM_YENI_TASARIM_BATCH ( int p_belge_id, QWidget * parent )
{
    TASARIM_YENI_TASARIM_BATCH * F = new TASARIM_YENI_TASARIM_BATCH ( p_belge_id, parent );
    F->EXEC( NOT_FULL_SCREEN );
}

/**************************************************************************************
                   TASARIM_YENI_TASARIM_BATCH::TASARIM_YENI_TASARIM_BATCH
***************************************************************************************/

TASARIM_YENI_TASARIM_BATCH::TASARIM_YENI_TASARIM_BATCH ( int p_belge_id, QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::TASARIM_YENI_TASARIM_BATCH )
{
    m_belge_id      = p_belge_id;

    m_ui->setupUi      ( this );    
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   TASARIM_YENI_TASARIM_BATCH::SETUP_FORM
***************************************************************************************/

void TASARIM_YENI_TASARIM_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Create") );
    SET_AUTO_EXIT_BATCH ( true );

    SET_FIRST_FOCUS_WIDGET ( m_ui->widget_batch_buttons );

    REGISTER_BUTTON_WIDGET ( m_ui->widget_batch_buttons );

    SET_PAGE_TITLE    (tr("NEW DESIGN"));
    SET_SETTING_NAME  ("TASARIM_YENI_TASARIM_BATCH");
    SET_HELP_PAGE     ("belge-tasarimcisi_tasarim-yeni-tasarim");


    FILL_PRINTER_TYPE_COMBOBOX( m_ui->comboBox_yazici_tipi );

    QStringList belgeler;
    SQL_QUERY query( G_YONETIM_DB );

    query.PREPARE_SELECT("ynt_belge_tasarimlari","tasarim_id,tasarim_adi ","belge_id = :belge_id","");
    query.SET_VALUE(":belge_id", m_belge_id );

    if( query.SELECT("tasarim_adi") EQ 0 ) {
        return;
    }
    int current_index = 0;
    m_copy_belge_id_list << "-1";
    belgeler << tr( " << DON'T COPY, I want to design from scratch. >>" );//KOPYALAMA , sıfırdan tasarlamak istiyorum
    while ( query.NEXT() ) {
        if ( current_index EQ 0 ) {
            current_index = query.VALUE( 0 ).toInt();
        }
        belgeler << query.VALUE( 1 ).toString();
        m_copy_belge_id_list << query.VALUE( 0 ).toString();
    }

    m_ui->comboBox_belgeler->addItems( belgeler );
}

/**************************************************************************************
                   TASARIM_YENI_TASARIM_BATCH::CHECK_VAR
***************************************************************************************/

int TASARIM_YENI_TASARIM_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->comboBox_belgeler ) {
        if ( m_ui->comboBox_belgeler->currentIndex() EQ 0 ) {
            m_ui->label_yazici_tipi->setVisible( true );
            m_ui->comboBox_yazici_tipi->setVisible( true );
        }
        else {
            m_ui->label_yazici_tipi->setVisible( false );
            m_ui->comboBox_yazici_tipi->setVisible( false );
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_YENI_TASARIM_BATCH::CHECK_RUN
***************************************************************************************/

int TASARIM_YENI_TASARIM_BATCH::CHECK_RUN ()
{
    if ( m_ui->lineEdit_belge_adi->text().isEmpty() EQ true ) {
        MSG_WARNING( tr( "You must enter the document name." ), m_ui->lineEdit_belge_adi );//Belge Adını girmelisiniz...
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   TASARIM_YENI_TASARIM_BATCH::RUN_BATCH
***************************************************************************************/

void TASARIM_YENI_TASARIM_BATCH::RUN_BATCH ()
{

    int record_id = m_copy_belge_id_list.at( m_ui->comboBox_belgeler->currentIndex() ).toInt();

    SQL_QUERY select_query(G_YONETIM_DB);
    SQL_QUERY insert_query(G_YONETIM_DB);

    G_YONETIM_DB->START_TRANSACTION();


    if ( record_id NE -1 ) {

        select_query.PREPARE_SELECT("ynt_belge_tasarimlari","belge_id,html_str,belge_width,belge_height,kopya_sayisi,kopya_konumu,"
                                    "row_count,row_space_mm,row_space_satir, belge_satir_sayisi,belge_toplam_karakter_sayisi,printer_type",
                                    "tasarim_id=:tasarim_id","");
        select_query.SET_VALUE(":tasarim_id", record_id);
        if ( select_query.SELECT() EQ 0 ) {
            G_YONETIM_DB->COMMIT_TRANSACTION();
            return;
        }
        select_query.NEXT();

        insert_query.PREPARE_INSERT("ynt_belge_tasarimlari","tasarim_id","belge_id,tasarim_adi,html_str,belge_width,belge_height,kopya_sayisi,kopya_konumu, "
                                    "row_count,row_space_mm,row_space_satir,belge_satir_sayisi,belge_toplam_karakter_sayisi,printer_type,readonly");

        insert_query.SET_VALUE(":belge_id"                      ,select_query.VALUE(0).toString());
        insert_query.SET_VALUE(":tasarim_adi"                   ,m_ui->lineEdit_belge_adi->text());
        insert_query.SET_VALUE(":html_str"                      ,select_query.VALUE(1).toString());
        insert_query.SET_VALUE(":belge_width"                   ,select_query.VALUE(2).toString());
        insert_query.SET_VALUE(":belge_height"                  ,select_query.VALUE(3).toString());
        insert_query.SET_VALUE(":kopya_sayisi"                  ,select_query.VALUE(4).toString());
        insert_query.SET_VALUE(":kopya_konumu"                  ,select_query.VALUE(5).toString());
        insert_query.SET_VALUE(":row_count"                     ,select_query.VALUE(6).toString());
        insert_query.SET_VALUE(":row_space_mm"                  ,select_query.VALUE(7).toString());
        insert_query.SET_VALUE(":row_space_satir"               ,select_query.VALUE(8).toString());
        insert_query.SET_VALUE(":belge_satir_sayisi"            ,select_query.VALUE(9).toString());
        insert_query.SET_VALUE(":belge_toplam_karakter_sayisi"  ,select_query.VALUE(10).toString());
        insert_query.SET_VALUE(":printer_type"                  ,select_query.VALUE(11).toString());
        insert_query.SET_VALUE(":readonly"                      ,0);

        int yeni_tasarim_id = insert_query.INSERT();

        select_query.PREPARE_SELECT("ynt_belge_degiskenleri","degisken_id,font_size,align,pos_x,pos_y,is_visible_variable,grup_enum,size_vertical, "
                                    "size_horizontal,pixmap,text, text_size" , "tasarim_id = :tasarim_id","");
        select_query.SET_VALUE(":tasarim_id",record_id);
        if ( select_query.SELECT() EQ 0 ) {
            G_YONETIM_DB->CANCEL_TRANSACTION();
            return;
        }

        while (select_query.NEXT() EQ true) {
            insert_query.PREPARE_INSERT("ynt_belge_degiskenleri","tasarim_id","tasarim_id,degisken_id,font_size,align,pos_x,pos_y, "
                                        "is_visible_variable,grup_enum,size_vertical,size_horizontal,pixmap,text, text_size");


            insert_query.SET_VALUE(":tasarim_id",               yeni_tasarim_id                                         );
            insert_query.SET_VALUE(":degisken_id",              select_query.VALUE("degisken_id").toString()            );
            insert_query.SET_VALUE(":font_size",                select_query.VALUE("font_size").toString()              );
            insert_query.SET_VALUE(":align",                    select_query.VALUE("align").toString()                  );
            insert_query.SET_VALUE(":pos_x",                    select_query.VALUE("pos_x").toString()                  );
            insert_query.SET_VALUE(":pos_y",                    select_query.VALUE("pos_y").toString()                  );
            insert_query.SET_VALUE(":is_visible_variable",      select_query.VALUE("is_visible_variable").toInt()       );
            insert_query.SET_VALUE(":grup_enum",                select_query.VALUE("grup_enum").toString()                   );
            insert_query.SET_VALUE(":size_vertical",            select_query.VALUE("size_vertical").toString()          );
            insert_query.SET_VALUE(":size_horizontal",          select_query.VALUE("size_horizontal").toString()        );
            insert_query.SET_VALUE(":pixmap",                   select_query.VALUE("pixmap").toString()                 );
            insert_query.SET_VALUE(":text",                     select_query.VALUE("text").toString()                   );
            insert_query.SET_VALUE(":text_size",                select_query.VALUE("text_size").toString()              );

            insert_query.INSERT();
        }
    }
    else {

        SQL_QUERY query(G_YONETIM_DB);

        query.PREPARE_INSERT("ynt_belge_tasarimlari",
                             "tasarim_id","tasarim_adi, belge_id, kopya_konumu, readonly, "
                             "printer_type, belge_width, belge_height, kopya_sayisi, row_count, belge_satir_sayisi, belge_toplam_karakter_sayisi, row_space_satir");

        query.SET_VALUE(":tasarim_adi"                  , m_ui->lineEdit_belge_adi->text() );
        query.SET_VALUE(":belge_id"                     , m_belge_id);
        query.SET_VALUE(":kopya_konumu"                 , 0);
        query.SET_VALUE(":readonly"                     , 0);
        query.SET_VALUE(":printer_type"                 , GET_PRINTER_TYPE_ENUM( m_ui->comboBox_yazici_tipi->currentText() ));
        query.SET_VALUE(":belge_width"                  , 210.00 );
        query.SET_VALUE(":belge_height"                 , 295.00 );
        query.SET_VALUE(":kopya_sayisi"                 , 1 );
        query.SET_VALUE(":row_count"                    , 5);
        query.SET_VALUE(":belge_satir_sayisi"           , 66);
        query.SET_VALUE(":belge_toplam_karakter_sayisi" , 80);
        query.SET_VALUE(":row_space_satir"              , 1);

        query.INSERT();
    }
    G_YONETIM_DB->COMMIT_TRANSACTION();


    MSG_INFO( tr( "Created" ), NULL );
    close();
}

/**************************************************************************************
                   TASARIM_YENI_TASARIM_BATCH::CHECK_EXIT
***************************************************************************************/

int TASARIM_YENI_TASARIM_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}
