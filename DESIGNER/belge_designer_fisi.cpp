#include "adak_sql.h"
#include "adak_defines.h"
#include "belge_designer.h"
#include "belge.h"
#include "belge_designer_fisi.h"
#include "tasarim_utils.h"

extern ADAK_SQL *              DB;

#define ROW_ID_COLUMN               0
#define FIS_ID_COLUMN               1
#define ORDER_COLUMN                2
#define DEGISKEN_GRUBU              3
#define DEGISKEN_KODU_COLUMN        4
#define DEGISKEN_ADI_COLUMN         5
#define DEGISKEN_FONT_SIZE_COLUMN   6
#define DEGISKEN_FONT_TYPE_COLUMN   7
#define DEGISKEN_TEXT_BOYUTU_COLUMN 8
#define DEGISKEN_ALIGN_COLUMN       9

/**************************************************************************************
                   OPEN_BELGE_DESIGNER
***************************************************************************************/

void OPEN_BELGE_DESIGNER( int record_id, QWidget * parent )
{
    BELGE_DESIGNER_FISI * F = new BELGE_DESIGNER_FISI(record_id,parent);
    F->setModal(false);
    F->SHOW (FULL_SCREEN);

}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::BELGE_DESIGNER_FISI
***************************************************************************************/

BELGE_DESIGNER_FISI::BELGE_DESIGNER_FISI(int record_id, QWidget * parent) :FIS_KERNEL(parent)
{
    setupUi         ( this   );

    m_record_start_id = record_id;

    START_FIS_KERNEL (this, DB);
 }

/**************************************************************************************
                   BELGE_DESIGNER_FISI::SETUP_FORM
***************************************************************************************/

void BELGE_DESIGNER_FISI::SETUP_FORM()
{
    REGISTER_BUTTONS_WIDGET          ( navigation_button_widget );

    REGISTER_TABLE_WIDGET             ( table_widget_degiskenler, 10 );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( FIS_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 3 );

    SET_FIS_ORDER_DATABASE ("blg_belge_degiskenleri", "order_num", "degisken_id");

    SET_TABLE_ROW_WIDGETS ( DEGISKEN_GRUBU                  , WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( DEGISKEN_KODU_COLUMN            , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( DEGISKEN_ADI_COLUMN             , WIDGET_LINE_EDIT   );
    SET_TABLE_ROW_WIDGETS ( DEGISKEN_FONT_SIZE_COLUMN       , WIDGET_COMMA_EDIT  );
    SET_TABLE_ROW_WIDGETS ( DEGISKEN_ALIGN_COLUMN           , WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( DEGISKEN_FONT_TYPE_COLUMN       , WIDGET_COMBO_BOX   );
    SET_TABLE_ROW_WIDGETS ( DEGISKEN_TEXT_BOYUTU_COLUMN     , WIDGET_COMMA_EDIT  );

    SET_FIRST_FOCUS_WIDGET(line_edit_belge_adi);

    REGISTER_SAVER_BUTTON(push_button_header_olustur);

    line_edit_belge_adi->setMaxLength(30);

    SET_PAGE_TITLE(tr("BELGE DESIGNER"));

    table_widget_degiskenler->setHorizontalHeaderLabels(QStringList()<<"row_id"<<"fis_id"   <<"order_column"
                                                                     <<tr("Değişken Grubu") <<tr("Değişken Kodu") <<tr("Değişken Adı")
                                                                     <<tr("Yazıtipi Boyutu(pt)") << tr( "Monospace mi ?" )<< tr( "Karakter Boyutu" )
                                                                     <<tr("Değişken Align") <<tr("")<<tr("") );

    QHeaderView * header =  table_widget_degiskenler->horizontalHeader();

     table_widget_degiskenler->setColumnWidth ( DEGISKEN_GRUBU                  , 100 );
     table_widget_degiskenler->setColumnWidth ( DEGISKEN_KODU_COLUMN            , 200 );
     table_widget_degiskenler->setColumnWidth ( DEGISKEN_ADI_COLUMN             , 200 );
     table_widget_degiskenler->setColumnWidth ( DEGISKEN_FONT_SIZE_COLUMN       , 100 );
     table_widget_degiskenler->setColumnWidth ( DEGISKEN_ALIGN_COLUMN           , 100 );
     table_widget_degiskenler->setColumnWidth ( DEGISKEN_FONT_TYPE_COLUMN       , 80  );
     table_widget_degiskenler->setColumnWidth ( DEGISKEN_TEXT_BOYUTU_COLUMN     , 100 );

    header->setStretchLastSection(QHeaderView::Stretch);

    table_widget_degiskenler->setSelectionMode ( QAbstractItemView::NoSelection );
    table_widget_degiskenler->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();

    AUTOSTART_FIS_KERNEL(m_record_start_id);
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void BELGE_DESIGNER_FISI::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::NEW_FIS_RECORD
***************************************************************************************/

void BELGE_DESIGNER_FISI::NEW_FIS_RECORD()
{
    line_edit_belge_adi->clear();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::GET_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::GET_FIS_RECORD ( int record_id )
{
    SQL_QUERY   query(DB);

    query.PREPARE_SELECT( "blg_belgeler","belge_adi","belge_id  = :belge_id " );
    query.SET_VALUE(":belge_id",record_id);

    if ( query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    query.NEXT();

    line_edit_belge_adi->setText(query.VALUE(0).toString());

    query.PREPARE_SELECT ( "blg_belge_degiskenleri","degisken_id, order_num, grubu, adi, font_size, align, kodu, monospace_mi, text_boyutu  ",
                           "belge_id = :belge_id");
    query.SET_VALUE      ( ":belge_id", record_id );

    if ( query.SELECT("order_num") EQ 0 ) {
        return ADAK_OK;
    }

    int row_number  ;

    while ( query.NEXT() EQ true ) {
        row_number = ADD_NEW_LINE ();

        QTableWidgetItem * new_item;

        QString row_id    = query.VALUE(0).toString();
        QString order_num = query.VALUE(1).toString();
        QString fis_id    = QVariant(record_id).toString();

        new_item = new QTableWidgetItem(tr("%1").arg(row_id));
        table_widget_degiskenler->setItem(row_number, ROW_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(fis_id));
        table_widget_degiskenler->setItem(row_number, FIS_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(order_num));
        table_widget_degiskenler->setItem(row_number, ORDER_COLUMN , new_item);

        QComboBox * combo_box_grup   = ( QComboBox * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_GRUBU );
        combo_box_grup->setCurrentIndex( combo_box_grup->findText( GET_BELGE_GRUP_STR(query.VALUE(2).toInt())));

        QLineEdit * line_edit_adi    = ( QLineEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ADI_COLUMN);
        line_edit_adi->setText( query.VALUE(3).toString() );

        QLineEdit * line_edit_kodu   = ( QLineEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_KODU_COLUMN);
        line_edit_kodu->setText( query.VALUE(6).toString() );

        QCommaEdit * comma_edit_size = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_SIZE_COLUMN);
        comma_edit_size->SET_INTEGER( query.VALUE(4).toInt() );

        QComboBox * combo_edit_align = ( QComboBox * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ALIGN_COLUMN);
        combo_edit_align->setCurrentIndex( combo_edit_align->findText(GET_BELGE_ALIGN_STR(query.VALUE(5).toInt())));

        QString monospace_mi = "Hayır";
        if ( query.VALUE( 7 ).toInt() EQ 1 ) {
            monospace_mi = "Evet";
        }
        QComboBox * combo_monospace_mi = ( QComboBox * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_TYPE_COLUMN );
        combo_monospace_mi->setCurrentIndex( combo_monospace_mi->findText( monospace_mi ));

        QCommaEdit * comma_edit_text_boyutu = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_TEXT_BOYUTU_COLUMN );
        comma_edit_text_boyutu->SET_INTEGER( query.VALUE( 8 ).toInt() );

    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void BELGE_DESIGNER_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QComboBox  * combo_box_monospace_mi = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_TYPE_COLUMN );
    QComboBox  * combo_box_grup         = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_GRUBU        );
    QComboBox  * combo_edit_align       = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ALIGN_COLUMN );
    QCommaEdit * comma_edit_size        = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_SIZE_COLUMN  );
    QCommaEdit * comma_edit_boyut       = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_TEXT_BOYUTU_COLUMN  );
    QLineEdit  * lineEdit_degisken_adi  = ( QLineEdit  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ADI_COLUMN   );
    QLineEdit  * lineEdit_degisken_kodu = ( QLineEdit  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_KODU_COLUMN  );

    FILL_DEGISKEN_GRUBU_COMBOBOX( combo_box_grup );
    FILL_ALIGN_COMBOBOX( combo_edit_align );

    combo_box_monospace_mi->addItems( QStringList() << tr("Evet") << tr( "Hayır") );

    comma_edit_size->SET_PRECISION       ( 0    );
    comma_edit_size->VIRGULSUZ_CALIS     ( true );
    comma_edit_size->SET_INTEGER         ( 13   );

    comma_edit_boyut->SET_PRECISION      ( 0    );
    comma_edit_boyut->VIRGULSUZ_CALIS    ( true );
    comma_edit_boyut->SET_INTEGER        ( 15   );

    lineEdit_degisken_adi->setMaxLength  ( 30 );
    lineEdit_degisken_kodu->setMaxLength ( 30 );
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ line_edit_belge_adi ) {
        if ( line_edit_belge_adi->text().size() > 30 ) {
            MSG_ERROR(tr("Belge adı 30 karakterden daha uzun olamaz."),NULL);
            return ADAK_FAIL_UNDO;
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( line_edit_belge_adi->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("Hata! Belge adı boş bırakılamaz."), line_edit_belge_adi );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_FIS_RECORD_ADD()
{
    SQL_QUERY query(DB);
    query.PREPARE_SELECT("blg_belgeler","belge_id","belge_adi=:belge_adi");
    query.SET_VALUE(":belge_adi",line_edit_belge_adi->text());

    if ( query.SELECT() NE 0 ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::ADD_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::ADD_FIS_RECORD()
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_INSERT("blg_belgeler","belge_id","belge_adi");
    sql_query.SET_VALUE     (":belge_adi" , line_edit_belge_adi->text());

    return sql_query.INSERT();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_FIS_RECORD_UPDATE(int record_id)
{
    SQL_QUERY query(DB);
    query.PREPARE_SELECT("blg_belgeler","belge_id","belge_adi = :belge_adi AND belge_id != :belge_id");
    query.SET_VALUE(":belge_adi" , line_edit_belge_adi->text());
    query.SET_VALUE(":belge_id",record_id);

    if ( query.SELECT() NE 0 ) {
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void BELGE_DESIGNER_FISI::UPDATE_FIS_RECORD(int record_id)
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_UPDATE("blg_belgeler","belge_id","belge_adi","belge_id = :belge_id");

    sql_query.SET_VALUE(":belge_adi" , line_edit_belge_adi->text());
    sql_query.SET_VALUE(":belge_id"  , record_id);

    sql_query.UPDATE();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_FIS_RECORD_DELETE(int record_id)
{
    Q_UNUSED(record_id);
    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void BELGE_DESIGNER_FISI::DELETE_FIS_RECORD ( int record_id )
{
    SQL_QUERY query(DB);

    query.PREPARE_DELETE("blg_belgeler","belge_id = :belge_id");
    query.SET_VALUE(":belge_id",record_id);
    query.DELETE();

    query.PREPARE_DELETE("blg_belge_degiskenleri","belge_id = :belge_id");
    query.SET_VALUE(":belge_id",record_id);
    query.DELETE();

}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::SELECT_FIS_RECORD()
{
    int id = OPEN_BELGE_SEC(this);

    return id;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::FIND_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY   query(DB);

    query.PREPARE_SELECT ( "blg_belgeler","belge_id ","belge_adi = :belge_adi " );
    query.SET_VALUE      ( ":belge_adi"   , line_edit_belge_adi->text());

    if ( query.SELECT() EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT( "blg_belgeler","belge_id" );

    if ( query.SELECT("belge_adi") EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT( "blg_belgeler","belge_id" );

    if ( query.SELECT("belge_adi DESC") EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY query(DB);

    query.PREPARE_SELECT( "blg_belgeler","belge_id","belge_adi < :belge_adi" );
    query.SET_VALUE     ( ":belge_adi"  , line_edit_belge_adi->text() );

    if ( query.SELECT("belge_adi DESC") EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY  query(DB);

    query.PREPARE_SELECT( "blg_belgeler","belge_id","belge_adi > :belge_adi" );
    query.SET_VALUE     ( ":belge_adi"  , line_edit_belge_adi->text() );

    if ( query.SELECT("belge_adi") EQ 0 ) {
        return -1;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_LINE_VAR
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_LINE_VAR(int row_number, QObject * object)
{
    Q_UNUSED (row_number);
    Q_UNUSED (object);
    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_LINE_EMPTY(int row_number)
{
    QLineEdit * line_edit_adi    = (QLineEdit *)table_widget_degiskenler->cellWidget(row_number,DEGISKEN_ADI_COLUMN);
    QCommaEdit * comma_edit_size = (QCommaEdit *)table_widget_degiskenler->cellWidget(row_number,DEGISKEN_FONT_SIZE_COLUMN);

    if (line_edit_adi->text().isEmpty() EQ true) {
        MSG_ERROR(tr("Değişken adı boş bırakılamaz."),line_edit_adi);
        return ADAK_FAIL;
    }
    if (comma_edit_size->GET_INTEGER() < 1) {
        MSG_ERROR(tr("Değişken boyutları 0pt dan büyük olmalı."),comma_edit_size);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_ADD_LINE
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_ADD_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);

    QLineEdit * line_edit_adi = (QLineEdit *)table_widget_degiskenler->cellWidget(row_number,DEGISKEN_ADI_COLUMN);

    SQL_QUERY query(DB);
    query.PREPARE_SELECT("blg_belgeler","belge_id","belge_adi = :belge_adi");
    query.SET_VALUE(":belge_adi",line_edit_adi->text());
    if (query.SELECT() NE 0) {
        MSG_ERROR(tr("Değişken adı başka bir kayıtta kullanılmış."),line_edit_adi);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::ADD_LINE
***************************************************************************************/

void BELGE_DESIGNER_FISI::ADD_LINE(int record_id, int row_number)
{
    SQL_QUERY query(DB);

    QString order_num = table_widget_degiskenler->item(row_number, ORDER_COLUMN)->text();

    QComboBox   * combo_box_grup        = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_GRUBU );
    QLineEdit   * line_edit_adi         = ( QLineEdit  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ADI_COLUMN);
    QLineEdit   * line_edit_kodu        = ( QLineEdit  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_KODU_COLUMN);
    QCommaEdit  * comma_edit_size       = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_SIZE_COLUMN);
    QComboBox   * combo_box_align       = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ALIGN_COLUMN);
    QCommaEdit  * comma_edit_boyut      = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_TEXT_BOYUTU_COLUMN );
    QComboBox   * combo_box_monospace_mi= ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_TYPE_COLUMN );

    int monospace_mi = 0;
    if ( combo_box_monospace_mi->currentText() EQ "Evet") {
        monospace_mi = 1;
    }

    query.PREPARE_INSERT("blg_belge_degiskenleri","degisken_id","belge_id, order_num, grubu, adi, font_size, align, kodu, monospace_mi, text_boyutu");

    query.SET_VALUE(":belge_id"         , record_id);
    query.SET_VALUE(":order_num"        , order_num);
    query.SET_VALUE(":grubu"            , GET_BELGE_GRUP_ENUM(combo_box_grup->currentText()));
    query.SET_VALUE(":adi"              , line_edit_adi->text());
    query.SET_VALUE(":kodu"             , line_edit_kodu->text());
    query.SET_VALUE(":font_size"        , comma_edit_size->GET_INTEGER());
    query.SET_VALUE(":align"            , GET_BELGE_ALIGN_ENUM(combo_box_align->currentText()));
    query.SET_VALUE(":monospace_mi"     , monospace_mi);
    query.SET_VALUE(":text_boyutu"      , comma_edit_boyut->GET_INTEGER() );

    int row_id = query.INSERT();

    table_widget_degiskenler->setItem(row_number, ROW_ID_COLUMN,new QTableWidgetItem((QVariant(row_id)).toString()));
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_UPDATE_LINE ( int record_id, int row_number )
{
    QLineEdit * line_edit_adi = (QLineEdit *)table_widget_degiskenler->cellWidget(row_number,DEGISKEN_ADI_COLUMN);

    SQL_QUERY query(DB);
    query.PREPARE_SELECT("blg_belgeler","belge_id","belge_adi = :belge_adi AND belge_id =:belge_id");
    query.SET_VALUE(":belge_adi",line_edit_adi->text());
    query.SET_VALUE(":belge_id" ,record_id);

    if (query.SELECT() NE 0) {
        MSG_ERROR(tr("Değişken adı başka bir kayıtta kullanılmış."),line_edit_adi);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::UPDATE_LINE
***************************************************************************************/

void BELGE_DESIGNER_FISI::UPDATE_LINE ( int record_id, int row_number )
{
    Q_UNUSED(record_id);

    SQL_QUERY query(DB);

    QString order_num;
    order_num  = table_widget_degiskenler->item(row_number, ORDER_COLUMN)->text();

    int row_id = table_widget_degiskenler->item(row_number, ROW_ID_COLUMN)->text().toInt();

    QComboBox   * combo_box_grup            = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_GRUBU );
    QLineEdit   * line_edit_adi             = ( QLineEdit  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ADI_COLUMN);
    QLineEdit   * line_edit_kodu            = ( QLineEdit  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_KODU_COLUMN);
    QCommaEdit  * comma_edit_size           = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_SIZE_COLUMN);
    QComboBox   * combo_box_align           = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_ALIGN_COLUMN);
    QCommaEdit  * comma_edit_boyut          = ( QCommaEdit * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_TEXT_BOYUTU_COLUMN );
    QComboBox   * combo_box_monospace_mi    = ( QComboBox  * ) table_widget_degiskenler->cellWidget( row_number, DEGISKEN_FONT_TYPE_COLUMN );

    int monospace_mi = 0;
    if ( combo_box_monospace_mi->currentText() EQ "Evet") {
        monospace_mi = 1;
    }

    query.PREPARE_UPDATE("blg_belge_degiskenleri","degisken_id","order_num,grubu,adi,font_size,align,kodu, monospace_mi, text_boyutu ","degisken_id=:degisken_id");

    query.SET_VALUE(":order_num"    ,order_num);
    query.SET_VALUE(":grubu"        ,GET_BELGE_GRUP_ENUM(combo_box_grup->currentText()));
    query.SET_VALUE(":adi"          ,line_edit_adi->text());
    query.SET_VALUE(":kodu"         ,line_edit_kodu->text());
    query.SET_VALUE(":font_size"    ,comma_edit_size->GET_INTEGER());
    query.SET_VALUE(":align"        ,GET_BELGE_ALIGN_ENUM(combo_box_align->currentText()));
    query.SET_VALUE(":monospace_mi" , monospace_mi);
    query.SET_VALUE(":text_boyutu"    , comma_edit_boyut->GET_INTEGER() );
    query.SET_VALUE(":degisken_id"  ,row_id);

    query.UPDATE();

    table_widget_degiskenler->setItem(row_number, ROW_ID_COLUMN,new QTableWidgetItem((QVariant(row_id)).toString()));
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int BELGE_DESIGNER_FISI::CHECK_DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(row_number);
    return ADAK_OK;
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::DELETE_LINE
***************************************************************************************/

void BELGE_DESIGNER_FISI::DELETE_LINE(int record_id, int row_number)
{
    Q_UNUSED(record_id);

    SQL_QUERY query(DB);

    int row_id = table_widget_degiskenler->item(row_number, ROW_ID_COLUMN)->text().toInt();

    query.PREPARE_DELETE ( "blg_belge_degiskenleri","degisken_id = :degisken_id");
    query.SET_VALUE      ( ":degisken_id", row_id );
    query.DELETE();
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int BELGE_DESIGNER_FISI::LOCK_FIS_RECORD(int record_id)
{
    return DB->LOCK_ROW( "belge_id","blg_belgeler", QString ( "belge_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void BELGE_DESIGNER_FISI::UNLOCK_FIS_RECORD(int record_id)
{
    DB->UNLOCK_ROW( "belge_id","blg_belgeler", QString ( "belge_id = %1" ).arg ( record_id ));
}

/**************************************************************************************
                   BELGE_DESIGNER_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void BELGE_DESIGNER_FISI::SAVER_BUTTON_CLICKED(QAbstractButton *button, int record_id)
{
    if (button EQ push_button_header_olustur) {
        OPEN_HEADER_DOSYASI_OLUSTUR(record_id);
        MSG_INFO(tr("Header dosyası oluşturuldu."),NULL);
    }
}
