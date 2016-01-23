#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>

#include "adak_sql_enums.h"
#include "yonetim.h"
#include "ui_sql_tablolar_fisi.h"
#include "sql_tablolar_fisi.h"
#include "sql_designer.h"
#include "sql_struct_generator.h"

#define ROW_ID_COLUMN              0
#define TABLO_ID_COLUMN            1
#define ORDER_COLUMN               2
#define ALAN_ADI_COLUMN            3
#define ALAN_TIPI_COLUMN           4
#define ALAN_BOYUT_COLUMN          5
#define DIZI_MI_COLUMN             6
#define DIZI_BOYUT_COLUMN          7
#define ACIKLAMA_COLUMN            8
#define AUTO_INCREMENT_COLUMN      9
#define NOT_NULL_COLUMN           10

extern ADAK_SQL *   DB;

/**************************************************************************************
                   OPEN_SQL_DESIGNER
***************************************************************************************/

void OPEN_SQL_DESIGNER ( QWidget * p_parent )
{
    SQL_TABLOLAR_FISI * sql_tablolar = new SQL_TABLOLAR_FISI( p_parent);
    sql_tablolar->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::SQL_TABLOLAR_FISI
***************************************************************************************/

SQL_TABLOLAR_FISI::SQL_TABLOLAR_FISI( QWidget * p_parent ) :FIS_KERNEL( p_parent ), m_ui ( new Ui::SQL_TABLOLAR_FISI )
{
    m_ui->setupUi          ( this     );
    START_FIS_KERNEL       ( this, DB );
 }

/**************************************************************************************
                   SQL_TABLOLAR_FISI::SETUP_FORM
***************************************************************************************/

void SQL_TABLOLAR_FISI::SETUP_FORM()
{

    setWindowState(Qt::WindowMaximized);
    REGISTER_BUTTONS_WIDGET ( m_ui->navigation_button_widget );

    num_of_columns   = 11;

    REGISTER_TABLE_WIDGET             ( m_ui->tablewidget_tablo_satirlari, num_of_columns );
    SET_FIS_ORDER_COLUMN_NUMBER       ( ORDER_COLUMN );
    SET_FORM_ID_COLUMN_POSITION       ( TABLO_ID_COLUMN );
    SET_ROW_ID_COLUMN_POSITION        ( ROW_ID_COLUMN );
    SET_NUMBER_OF_HIDDEN_COLUMN       ( 3 );
    SET_FIS_ORDER_DATABASE            ( "sql_alanlar", "alan_order_num", "alan_id" );
    SET_COMMA_EDIT_PRECISION          ( ALAN_BOYUT_COLUMN, 0);
    SET_COMMA_EDIT_PRECISION          ( DIZI_BOYUT_COLUMN, 0);
    SET_FIRST_FOCUS_WIDGET            ( m_ui->lineEdit_tablo_adi );


    SET_TABLE_ROW_WIDGETS ( ALAN_TIPI_COLUMN      , WIDGET_COMBO_BOX  );
    SET_TABLE_ROW_WIDGETS ( DIZI_MI_COLUMN        , WIDGET_CHECK_BOX  );
    SET_TABLE_ROW_WIDGETS ( AUTO_INCREMENT_COLUMN , WIDGET_CHECK_BOX  );
    SET_TABLE_ROW_WIDGETS ( NOT_NULL_COLUMN       , WIDGET_CHECK_BOX  );
    SET_TABLE_ROW_WIDGETS ( ALAN_ADI_COLUMN       , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ACIKLAMA_COLUMN       , WIDGET_LINE_EDIT  );
    SET_TABLE_ROW_WIDGETS ( ALAN_BOYUT_COLUMN     , WIDGET_COMMA_EDIT );
    SET_TABLE_ROW_WIDGETS ( DIZI_BOYUT_COLUMN     , WIDGET_COMMA_EDIT );

    m_ui->lineEdit_tablo_adi->setMaxLength                  ( 27  );
    m_ui->lineEdit_alan_adi->setMaxLength                   ( 30  ); // Oracle 30 hane ile sinirli
    m_ui->limitedTextEdit_aciklama->SET_MAX_STRING_LENGTH   ( 512 );

    SET_PAGE_TITLE        ( tr("SQL DESIGNER - TABLES"));

    REGISTER_SAVER_BUTTON ( m_ui->button_indexler );
    REGISTER_SAVER_BUTTON ( m_ui->button_header_olustur );

    m_ui->tablewidget_tablo_satirlari->setHorizontalHeaderLabels(QStringList()<<"alan_id"<<"tablo_id"
                                                                              <<"alan_order_num"
                                                                              <<tr("Field Name")<<tr("Field Type")
                                                                              <<tr("Length")<<tr("Is Array?")
                                                                              <<tr("Array Size")<<tr("Statement")
                                                                              <<tr("Auto Increment")<<tr("Not Null")
                                                                              <<tr("")<<tr(""));

    QHeaderView * header =  m_ui->tablewidget_tablo_satirlari->horizontalHeader();

     m_ui->tablewidget_tablo_satirlari->setColumnWidth ( ALAN_ADI_COLUMN       ,    300 );
     m_ui->tablewidget_tablo_satirlari->setColumnWidth ( ALAN_TIPI_COLUMN      ,    150 );
     m_ui->tablewidget_tablo_satirlari->setColumnWidth ( ALAN_BOYUT_COLUMN     ,    100 );
     m_ui->tablewidget_tablo_satirlari->setColumnWidth ( DIZI_MI_COLUMN        ,    75  );
     m_ui->tablewidget_tablo_satirlari->setColumnWidth ( DIZI_BOYUT_COLUMN     ,    100 );
     m_ui->tablewidget_tablo_satirlari->setColumnWidth ( ACIKLAMA_COLUMN       ,    446 );


    m_ui->tablewidget_tablo_satirlari->hideColumn ( AUTO_INCREMENT_COLUMN );
    m_ui->tablewidget_tablo_satirlari->hideColumn ( NOT_NULL_COLUMN       );

    header->setStretchLastSection(QHeaderView::Stretch);
    m_ui->tablewidget_tablo_satirlari->setSelectionMode ( QAbstractItemView::NoSelection );
    m_ui->tablewidget_tablo_satirlari->setFocus( Qt::OtherFocusReason );

    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void SQL_TABLOLAR_FISI::CLEAR_FORM_MEMBERS()
{
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::NEW_FIS_RECORD
***************************************************************************************/

void SQL_TABLOLAR_FISI::NEW_FIS_RECORD()
{
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::GET_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::GET_FIS_RECORD ( int p_tablo_id )
{
    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT ( "sql_tablolar","tablo_adi,aciklama","tablo_id = :tablo_id");
    sql_query.SET_VALUE      ( ":tablo_id", p_tablo_id );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_FAIL;
    }

    sql_query.NEXT();

    m_ui->lineEdit_tablo_adi->setText( sql_query.VALUE(0).toString());
    m_ui->limitedTextEdit_aciklama->setText( sql_query.VALUE(1).toString());

    sql_query.PREPARE_SELECT ( "sql_alanlar","alan_id, tablo_id, alan_order_num, alan_adi, veritabanindaki_adi, alan_tipi, "
                               "uzunluk, auto_increment_mi, not_null_mi, dizi_mi, dizi_boyutu, aciklama ",
                               "tablo_id = :tablo_id" );
    sql_query.SET_VALUE ( ":tablo_id" , p_tablo_id );

    if ( sql_query.SELECT("alan_order_num") EQ 0 ) {
        return ADAK_OK;
    }

    while ( sql_query.NEXT() EQ true ) {

        if ( sql_query.VALUE(2).toInt() EQ 1 ) {
            m_ui->lineEdit_alan_adi->setText(sql_query.VALUE(3).toString());
            continue;
        }

        int current_row = ADD_NEW_LINE();

        QString alan_id             = sql_query.VALUE(0).toString();
        QString alan_order_num      = sql_query.VALUE(2).toString();
        QString alan_adi            = sql_query.VALUE(3).toString();
        QString alan_tipi           = sql_query.VALUE(5).toString();
        QString alan_uzunlugu       = sql_query.VALUE(6).toString();
        QString auto_increment_mi   = sql_query.VALUE(7).toString();
        QString not_null_mi         = sql_query.VALUE(8).toString();
        QString dizi_mi             = sql_query.VALUE(9).toString();
        QString dizi_boyutu         = sql_query.VALUE(10).toString();
        QString aciklama            = sql_query.VALUE(11).toString();

        QComboBox * cell_combobox = ( QComboBox * ) m_ui->tablewidget_tablo_satirlari->cellWidget( current_row, ALAN_TIPI_COLUMN );
        cell_combobox->setCurrentIndex( GET_ADAK_SQL_ENUM (alan_tipi) );

        QCheckBox * cell_checkbox = ( QCheckBox * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( current_row, DIZI_MI_COLUMN );
        cell_checkbox->setChecked(dizi_mi.toInt());

        if ( dizi_mi.toInt() > 0 ) {
            QCommaEdit * dizi_boyutu_commaedit = ( QCommaEdit * )  m_ui->tablewidget_tablo_satirlari->cellWidget ( current_row, DIZI_BOYUT_COLUMN );
            dizi_boyutu_commaedit->setEnabled(true);
        }

        cell_checkbox = ( QCheckBox * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( current_row, AUTO_INCREMENT_COLUMN );
        cell_checkbox->setChecked( auto_increment_mi.toInt() );


        cell_checkbox = ( QCheckBox * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( current_row, NOT_NULL_COLUMN );
        cell_checkbox->setChecked( not_null_mi.toInt() );

        QTableWidgetItem * new_item;

        new_item = new QTableWidgetItem(tr("%1").arg(alan_id));
        m_ui->tablewidget_tablo_satirlari->setItem(current_row, ROW_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(p_tablo_id));
        m_ui->tablewidget_tablo_satirlari->setItem(current_row, TABLO_ID_COLUMN, new_item);

        new_item = new QTableWidgetItem(tr("%1").arg(alan_order_num));
        m_ui->tablewidget_tablo_satirlari->setItem(current_row, ORDER_COLUMN, new_item);

        QCommaEdit * commaEdit = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget(current_row, ALAN_BOYUT_COLUMN);
        commaEdit->SET_INTEGER(alan_uzunlugu.toInt());

        commaEdit = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget(current_row, DIZI_BOYUT_COLUMN);
        commaEdit->SET_INTEGER(dizi_boyutu.toInt());

        QLineEdit * lineEdit = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget(current_row, ACIKLAMA_COLUMN );
        lineEdit->setText(aciklama);

        lineEdit = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget(current_row, ALAN_ADI_COLUMN );
        lineEdit->setText(alan_adi);
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::SET_LINE_DEFAULTS
***************************************************************************************/

void SQL_TABLOLAR_FISI::SET_LINE_DEFAULTS ( int p_row_number )
{

    QComboBox  * comboBox_alan_tipi    = ( QComboBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_TIPI_COLUMN  );
    QCommaEdit * commaEdit_dizi_boyutu = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_BOYUT_COLUMN );
    QCheckBox  * checkBox_dizi_mi      = ( QCheckBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_MI_COLUMN    );
    QLineEdit  * lineEdit_alan_adi     = ( QLineEdit  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_ADI_COLUMN   );
    QLineEdit *  lineEdit_aciklama     = ( QLineEdit  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN   );

    comboBox_alan_tipi->setEditable(false);
    comboBox_alan_tipi->addItems(QStringList()<<"INTEGER"<<"TEXT"<<"REAL"<<"BLOB");

    checkBox_dizi_mi->setChecked(false);

    commaEdit_dizi_boyutu->SET_INTEGER   (1);
    commaEdit_dizi_boyutu->setDisabled   (true);
    commaEdit_dizi_boyutu->SET_PRECISION (0);
    lineEdit_aciklama->setMaxLength      ( 128 );
    lineEdit_alan_adi->setMaxLength      ( 30 ); // Oracle 30 hane ile sinirli
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_FIS_FORM_VAR ( QObject * p_object )
{
    if ( p_object EQ m_ui->lineEdit_tablo_adi ) {
        QRegExp non_word_char("(\\W+)");
        if ( m_ui->lineEdit_tablo_adi->text().contains(non_word_char) EQ true ) {
            MSG_ERROR(tr("Table name can contain only letters, numbers and \"_\"."),m_ui->lineEdit_tablo_adi);//Tablo ismi harf, rakam veya \"_\" dışında bir karakter içeremez
            return ADAK_FAIL;
        }
        if ( m_ui->lineEdit_tablo_adi->text().size() > 27 ) { // _id ile alan adi 27 haneyi gecmemeli
            MSG_ERROR(tr("Table name can not be longer than 27 characters."),m_ui->lineEdit_tablo_adi);//Tablo ismi 27 haneyi geçemez
            return ADAK_FAIL;
        }
        if ( m_ui->lineEdit_alan_adi->text().isEmpty() EQ false ) {
            return ADAK_OK;
        }
        QString alan_adi          = m_ui->lineEdit_tablo_adi->text();
        if ( alan_adi.contains("_") EQ true ) {
            QStringList alan_adi_list = alan_adi.split("_", QString::SkipEmptyParts );
            alan_adi.clear();
            for ( int i = 1; i < alan_adi_list.count(); i++ ) {
                alan_adi.append(alan_adi_list[i]);
                alan_adi.append("_");
            }
        }
        else {
            alan_adi.append("_");
        }
        m_ui->lineEdit_alan_adi->setText( alan_adi + "id" );
    }
    else if ( p_object EQ m_ui->lineEdit_alan_adi ) {
        if ( m_ui->lineEdit_alan_adi->text().size() > 30 ) {
            MSG_ERROR(tr("Unique field name can not be longer than 30 characters."),m_ui->lineEdit_alan_adi);//Uniqe alan adi 30 haneyi geçemez
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_FIS_FORM_EMPTY()
{
    if ( m_ui->lineEdit_tablo_adi->text().isEmpty() EQ true ) {
        MSG_ERROR(tr("Table name must not be empty."),m_ui->lineEdit_tablo_adi);//Tablo ismi boş bırakılamaz
        return ADAK_FAIL;
    }
    if ( m_ui->lineEdit_alan_adi->text().isEmpty() EQ true ) {
        MSG_ERROR(tr ("The Column name which in UNIQUE ID must not be empty. "),m_ui->lineEdit_alan_adi);//UNIQUE ID olan kolonun adı boş bırakılamaz
        return ADAK_FAIL;
    }
    return ADAK_OK;
}


/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_FIS_RECORD_ADD()
{
    if ( KAYIT_EKLEME_YETKISI_VAR_MI ( KULLANICI_ID() ) EQ 0 ) {
        MSG_ERROR(tr("This user has no authority to add records. "),m_ui->lineEdit_tablo_adi);//Bu kullanıcının kayıt ekleme yetkisi yok
        return ADAK_FAIL;
    }
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("sql_tablolar","tablo_id","tablo_adi = :tablo_adi");
    sql_query.SET_VALUE ( ":tablo_adi", m_ui->lineEdit_tablo_adi->text());
    if ( sql_query.SELECT() > 0 ) {
        MSG_ERROR(tr("This table name is used for another table."),m_ui->lineEdit_tablo_adi);//Bu isimle bir tablo sistemde mevcut
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::ADD_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::ADD_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_INSERT ( "sql_tablolar","tablo_id","tablo_adi, aciklama");
    sql_query.SET_VALUE (":tablo_adi"    , m_ui->lineEdit_tablo_adi->text()              );
    sql_query.SET_VALUE (":aciklama"     , m_ui->limitedTextEdit_aciklama->toPlainText() );

    int tablo_id = sql_query.INSERT();

    sql_query.PREPARE_INSERT( "sql_alanlar","alan_id", "tablo_id, alan_order_num, alan_adi, veritabanindaki_adi, "
                              "alan_tipi, uzunluk, auto_increment_mi, not_null_mi, dizi_mi, dizi_boyutu, aciklama ");

    sql_query.SET_VALUE ( ":tablo_id"            , tablo_id                         );
    sql_query.SET_VALUE ( ":alan_order_num"      , 1                                );
    sql_query.SET_VALUE ( ":alan_adi"            , m_ui->lineEdit_alan_adi->text()  );
    sql_query.SET_VALUE ( ":veritabanindaki_adi" , m_ui->lineEdit_alan_adi->text()  );
    sql_query.SET_VALUE ( ":alan_tipi"           , "INTEGER"                        );
    sql_query.SET_VALUE ( ":uzunluk"             , 0                                );
    sql_query.SET_VALUE ( ":auto_increment_mi"   , 1                                );
    sql_query.SET_VALUE ( ":not_null_mi"         , 1                                );
    sql_query.SET_VALUE ( ":dizi_mi"             , 0                                );
    sql_query.SET_VALUE ( ":dizi_boyutu"         , 1                                );
    sql_query.SET_VALUE ( ":aciklama"            , tr("Column for Table UNIQUE FIELD ID"));//Tablo UNIQUE ALAN ID kolonu

    int row_id = sql_query.INSERT();

    int indexler_array [INDEXLER_ARRAY_SIZE] = {0};
    indexler_array[0]                        = row_id;

    QString array_content = PACK_INT_ARRAY(indexler_array,INDEXLER_ARRAY_SIZE);

    SQL_QUERY query (DB);

    query.PREPARE_UPDATE("sql_tablolar","tablo_id","indexler_array","tablo_id=:tablo_id");

    query.SET_VALUE     (":indexler_array",array_content);
    query.SET_VALUE     (":tablo_id"      ,tablo_id);

    query.UPDATE();

    return tablo_id;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_FIS_RECORD_UPDATE ( int p_tablo_id )
{
    if ( KAYIT_GUNCELLEME_YETKISI_VAR_MI( KULLANICI_ID() ) EQ 0 ) {
        MSG_ERROR(tr("This user has no authority to update records."),m_ui->lineEdit_tablo_adi);//Bu kullanıcının kayıt güncelleme yetkisi yok
        return ADAK_FAIL;
    }

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("sql_tablolar","tablo_id","tablo_adi = :tablo_adi");
    sql_query.SET_VALUE ( ":tablo_adi", m_ui->lineEdit_tablo_adi->text());

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        if ( p_tablo_id NE sql_query.VALUE(0).toInt() ) {
            MSG_ERROR(tr("This table name is used for another table."),m_ui->lineEdit_tablo_adi);//Bu isimle bir tablo sistemde mevcut
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void SQL_TABLOLAR_FISI::UPDATE_FIS_RECORD ( int p_tablo_id )
{
    SQL_QUERY sql_query( DB );
    sql_query.PREPARE_UPDATE ("sql_tablolar","tablo_id","tablo_adi,aciklama","tablo_id = :tablo_id" );
    sql_query.SET_VALUE (":tablo_adi", m_ui->lineEdit_tablo_adi->text()              );
    sql_query.SET_VALUE (":aciklama" , m_ui->limitedTextEdit_aciklama->toPlainText() );
    sql_query.SET_VALUE (":tablo_id" , p_tablo_id );
    sql_query.UPDATE();


    sql_query.PREPARE_UPDATE( "sql_alanlar","tablo_id","alan_adi,veritabanindaki_adi","tablo_id = :tablo_id AND alan_order_num = 1");

    sql_query.SET_VALUE ( ":alan_adi"            , m_ui->lineEdit_alan_adi->text() );
    sql_query.SET_VALUE ( ":veritabanindaki_adi" , m_ui->lineEdit_alan_adi->text() );
    sql_query.SET_VALUE ( ":tablo_id"            , p_tablo_id                      );

    sql_query.UPDATE();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_FIS_RECORD_DELETE ( int p_tablo_id )
{
    Q_UNUSED ( p_tablo_id );

    if ( KAYIT_SILME_YETKISI_VAR_MI( KULLANICI_ID() ) EQ 0 ) {
        MSG_ERROR(tr("This user has no authority to delete records."),m_ui->lineEdit_tablo_adi);//Bu kullanıcının kayıt silme yetkisi yok"
        return ADAK_FAIL;
    }

    ADAK_MSG_ENUM answer = MSG_YES_NO_CANCEL(tr("Table will be deleted with all field definitions!"),m_ui->lineEdit_tablo_adi);//Tablo tüm alan tanımlarıyla beraber silinecektir!

    if ( answer EQ ADAK_NO OR answer EQ ADAK_CANCEL ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void SQL_TABLOLAR_FISI::DELETE_FIS_RECORD ( int p_tablo_id )
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_DELETE ( "sql_alanlar","tablo_id = :tablo_id" );
    sql_query.SET_VALUE      ( ":tablo_id", p_tablo_id );
    sql_query.DELETE();

    sql_query.PREPARE_DELETE ( "sql_tablolar","tablo_id = :tablo_id" );
    sql_query.SET_VALUE      ( ":tablo_id", p_tablo_id );
    sql_query.DELETE();

}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::SELECT_FIS_RECORD()
{
    return OPEN_SQL_TABLO_ARAMA ( this );
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::FIND_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::FIND_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT("sql_tablolar","tablo_id","tablo_adi = :tablo_adi");
    sql_query.SET_VALUE (":tablo_adi", m_ui->lineEdit_tablo_adi->text() );
    if (sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::FIND_FIRST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "sql_tablolar","tablo_id");

    if ( sql_query.SELECT("tablo_adi ASC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "sql_tablolar","tablo_id");

    if ( sql_query.SELECT("tablo_adi DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::FIND_PREV_FIS_RECORD ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "sql_tablolar","tablo_id","tablo_adi < :tablo_adi");
    sql_query.SET_VALUE     (":tablo_adi", m_ui->lineEdit_tablo_adi->text() );

    if ( sql_query.SELECT("tablo_adi DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::FIND_NEXT_FIS_RECORD ()
{
    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_SELECT( "sql_tablolar","tablo_id","tablo_adi > :tablo_adi");
    sql_query.SET_VALUE     (":tablo_adi", m_ui->lineEdit_tablo_adi->text() );

    if ( sql_query.SELECT("tablo_adi ASC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    sql_query.NEXT();

    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_LINE_VAR
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_LINE_VAR ( int p_row_number, QObject * p_object )
{

    QLineEdit  * alan_adi_lineEdit     = ( QLineEdit  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_ADI_COLUMN   );
    QCommaEdit * commaEdit_alan_boyutu = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_BOYUT_COLUMN );
    QCommaEdit * dizi_boyutu_commaEdit = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_BOYUT_COLUMN );
    QCheckBox  * dizi_mi_checkbox      = ( QCheckBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_MI_COLUMN    );
    QComboBox  * comboBox_alan_tipi    = ( QComboBox * )  m_ui->tablewidget_tablo_satirlari->cellWidget  ( p_row_number, ALAN_TIPI_COLUMN  );


    if ( p_object EQ alan_adi_lineEdit ) {
        QRegExp non_word_char("(\\W+)");
        if ( alan_adi_lineEdit->text().contains(non_word_char) EQ true ) {
            MSG_ERROR(tr("Field name must contain only letters, numbers and \"_\". "),alan_adi_lineEdit);//Alan adı harf, rakam veya \"_\" dışında bir karakter içeremez
            return ADAK_FAIL;
        }
        if ( alan_adi_lineEdit->text().size() > 30 ) {
            MSG_ERROR(tr("Field name can not be longer than 30 characters."),alan_adi_lineEdit);//Alan adı 30 haneyi geçemez
            return ADAK_FAIL;
        }
        if ( dizi_mi_checkbox->isChecked() EQ true AND alan_adi_lineEdit->text().size() > 24 ) {
            MSG_ERROR(tr("Array field name can not be longer than 24 characters."),alan_adi_lineEdit);//Dizi alan adı 24 haneyi geçemez
            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ commaEdit_alan_boyutu ) {

        if ( commaEdit_alan_boyutu->GET_INTEGER() > 9999 ) {
            MSG_ERROR(tr("Field size can not be more than 4 digits."),commaEdit_alan_boyutu);//Alan boyutu 4 haneden fazla olamaz
            return ADAK_FAIL;
        }

        if ( GET_ADAK_SQL_ENUM ( comboBox_alan_tipi->currentText() ) EQ TEXT ) {
            if ( commaEdit_alan_boyutu->GET_INTEGER() EQ 0) {
                MSG_ERROR(tr("Field length must be entered for an area in the TEXT type."),commaEdit_alan_boyutu);//TEXT tipinde bir alan için alan uzunluğu yazılmak zorundadır
                return ADAK_FAIL;
            }
        }
    }
    else if ( p_object EQ dizi_boyutu_commaEdit ) {
        if ( dizi_boyutu_commaEdit->GET_INTEGER()> 9999 ) {
            MSG_ERROR(tr("Array size can not be more than 4 digits."),dizi_boyutu_commaEdit);//Dizi boyutu 4 haneden fazla olamaz
            return ADAK_FAIL;
        }
    }
    else if ( p_object EQ  dizi_mi_checkbox ) {
        if ( dizi_mi_checkbox->isChecked() EQ false ) {
            dizi_boyutu_commaEdit->setDisabled(true);
            dizi_boyutu_commaEdit->SET_INTEGER (1);
        }
        else {
            dizi_boyutu_commaEdit->setEnabled(true);
        }
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_LINE_EMPTY ( int p_row_number )
{

    QCommaEdit * dizi_boyutu_commaEdit = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_BOYUT_COLUMN );
    QLineEdit  * alan_adi_lineEdit     = ( QLineEdit  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_ADI_COLUMN );
    QCheckBox  * dizi_mi_checkbox      = ( QCheckBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_MI_COLUMN    );
    QCommaEdit * commaEdit_alan_boyutu = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_BOYUT_COLUMN );
    QComboBox  * comboBox_alan_tipi    = ( QComboBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_TIPI_COLUMN  );

    if ( dizi_mi_checkbox->isChecked() EQ true ) {
        if ( dizi_boyutu_commaEdit->GET_INTEGER () < 2 ) {
            MSG_ERROR(tr("Array size must be greater than 1 "),dizi_mi_checkbox);//Dizi boyutu 1 den büyük olmalıdır
            return ADAK_FAIL;
        }
    }

    if ( alan_adi_lineEdit->text().isEmpty() EQ true ) {
        MSG_ERROR(tr ("Field name must not be empty."),alan_adi_lineEdit);//Alan adı boş bırakılamaz
        return ADAK_FAIL;
    }

    if ( GET_ADAK_SQL_ENUM ( comboBox_alan_tipi->currentText() ) EQ TEXT ) {
        if ( commaEdit_alan_boyutu->GET_INTEGER () < 1 ) {
            MSG_ERROR(tr("Field length must be entered for a field of type TEXT."),commaEdit_alan_boyutu);//TEXT tipinde bir alan için alan uzunluğu girilmek zorundadır
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_ADD_LINE
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_ADD_LINE ( int p_tablo_id, int p_row_number )
{
    if ( KAYIT_EKLEME_YETKISI_VAR_MI ( KULLANICI_ID() ) EQ 0 ) {
        MSG_ERROR(tr("This user has no authority for add registration."),m_ui->lineEdit_tablo_adi);//Bu kullanıcının kayıt ekleme yetkisi yok
        return ADAK_FAIL;
    }

    QLineEdit * alan_adi_lineEdit    = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget( p_row_number, ALAN_ADI_COLUMN );

    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT( "sql_alanlar","alan_id","tablo_id = :tablo_id AND alan_adi = :alan_adi" );

    sql_query.SET_VALUE ( ":tablo_id", p_tablo_id                  );
    sql_query.SET_VALUE ( ":alan_adi" , alan_adi_lineEdit->text() );

    if ( sql_query.SELECT() > 0 ) {
        MSG_ERROR(tr("The field name used for another field in this table."),alan_adi_lineEdit);//Bu isimle bir alan tabloda mevcut
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::ADD_LINE
***************************************************************************************/

void SQL_TABLOLAR_FISI::ADD_LINE ( int p_tablo_id, int p_row_number)
{

    QLineEdit   * lineEditAlanAdi     = ( QLineEdit  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_ADI_COLUMN );
    QComboBox   * comboBoxAlanTipi    = ( QComboBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_TIPI_COLUMN );
    QCommaEdit  * commaEditAlanBoyutu = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_BOYUT_COLUMN );
    QCommaEdit  * commaEditDiziBoyutu = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_BOYUT_COLUMN );
    QLineEdit   * lineEditAciklama    = ( QLineEdit  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN );
    QCheckBox   * checkBoxDiziMi      = ( QCheckBox  * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_MI_COLUMN );

    QString veritabanindaki_adi = lineEditAlanAdi->text();
    if ( checkBoxDiziMi->isChecked() EQ true ) {
        veritabanindaki_adi.append("_array");
    }

    int alan_order_num          = m_ui->tablewidget_tablo_satirlari->item( p_row_number, ORDER_COLUMN )->text().toInt();

    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_INSERT( "sql_alanlar","alan_id", "tablo_id, alan_order_num, alan_adi, veritabanindaki_adi, "
                              "alan_tipi, uzunluk, auto_increment_mi, not_null_mi, dizi_mi, dizi_boyutu, aciklama ");

    sql_query.SET_VALUE ( ":tablo_id"            , p_tablo_id          );
    sql_query.SET_VALUE ( ":alan_order_num"      , alan_order_num      );
    sql_query.SET_VALUE ( ":alan_adi"            , lineEditAlanAdi->text()            );
    sql_query.SET_VALUE ( ":veritabanindaki_adi" , veritabanindaki_adi                );
    sql_query.SET_VALUE ( ":alan_tipi"           , comboBoxAlanTipi->currentText()    );
    sql_query.SET_VALUE ( ":uzunluk"             , commaEditAlanBoyutu->GET_INTEGER() );
    sql_query.SET_VALUE ( ":auto_increment_mi"   , 0                                  );
    sql_query.SET_VALUE ( ":not_null_mi"         , 0                                  );
    sql_query.SET_VALUE ( ":dizi_mi"             , QVariant(checkBoxDiziMi->isChecked()).toInt());
    sql_query.SET_VALUE ( ":dizi_boyutu"         , commaEditDiziBoyutu->GET_INTEGER()  );
    sql_query.SET_VALUE ( ":aciklama"            , lineEditAciklama->text()            );

    int row_id = sql_query.INSERT();

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( row_id ).toString() );
    m_ui->tablewidget_tablo_satirlari->setItem ( p_row_number, ROW_ID_COLUMN, new_item );
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_UPDATE_LINE ( int p_tablo_id, int p_row_number )
{
    if ( KAYIT_GUNCELLEME_YETKISI_VAR_MI( KULLANICI_ID() ) EQ 0 ) {
        MSG_ERROR(tr("This user has no authority for Update registration."),m_ui->lineEdit_tablo_adi);//Bu kullanıcının kayıt güncelleme yetkisi yok
        return ADAK_FAIL;
    }

    QLineEdit * alan_adi_lineEdit    = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget( p_row_number, ALAN_ADI_COLUMN );
    int         row_id               = m_ui->tablewidget_tablo_satirlari->item( p_row_number, ROW_ID_COLUMN )->text().toInt();


    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT( "sql_alanlar","alan_id","tablo_id = :tablo_id AND alan_adi = :alan_adi" );
    sql_query.SET_VALUE ( ":tablo_id", p_tablo_id                  );
    sql_query.SET_VALUE ( ":alan_adi" , alan_adi_lineEdit->text()   );

    if ( sql_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }
    sql_query.NEXT();
    if ( sql_query.VALUE(0).toInt() NE row_id ) {
        MSG_ERROR(tr("This field name used for another field in this table."),alan_adi_lineEdit);
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::UPDATE_LINE
***************************************************************************************/

void SQL_TABLOLAR_FISI::UPDATE_LINE ( int p_tablo_id, int p_row_number )
{
    Q_UNUSED ( p_tablo_id );

    int alan_id                     = m_ui->tablewidget_tablo_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();


    QLineEdit * cell_lineEdit       = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget( p_row_number, ALAN_ADI_COLUMN );
    QString     alan_adi            = cell_lineEdit->text();
    QString     veritabanindaki_adi = alan_adi;


    QComboBox * cell_combobox   = ( QComboBox * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_TIPI_COLUMN );
    QString     alan_tipi       = cell_combobox->currentText();


    QCommaEdit * cell_commaEdit = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ALAN_BOYUT_COLUMN );
    int          uzunluk        = cell_commaEdit->GET_INTEGER();



    QCheckBox *  cell_checkbox  = ( QCheckBox * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_MI_COLUMN );
    int dizi_mi                 = cell_checkbox->isChecked();

    if ( dizi_mi EQ 1 ) {
        veritabanindaki_adi.append("_array");
    }


    cell_commaEdit              = ( QCommaEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, DIZI_BOYUT_COLUMN );
    int dizi_boyutu             = cell_commaEdit->GET_INTEGER();


    cell_lineEdit               = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget ( p_row_number, ACIKLAMA_COLUMN );
    QString aciklama            = cell_lineEdit->text();

    int alan_order_num          = m_ui->tablewidget_tablo_satirlari->item( p_row_number, ORDER_COLUMN )->text().toInt();

    SQL_QUERY sql_query ( DB );

    sql_query.PREPARE_UPDATE( "sql_alanlar","alan_id","alan_order_num,alan_adi,veritabanindaki_adi,alan_tipi, "
                              "uzunluk,auto_increment_mi,not_null_mi,dizi_mi,dizi_boyutu, aciklama","alan_id = :alan_id");

    sql_query.SET_VALUE ( ":alan_order_num"      , alan_order_num      );
    sql_query.SET_VALUE ( ":alan_adi"            , alan_adi            );
    sql_query.SET_VALUE ( ":veritabanindaki_adi" , veritabanindaki_adi );
    sql_query.SET_VALUE ( ":alan_tipi"           , alan_tipi           );
    sql_query.SET_VALUE ( ":uzunluk"             , uzunluk             );
    sql_query.SET_VALUE ( ":auto_increment_mi"   , 0                   );
    sql_query.SET_VALUE ( ":not_null_mi"         , 0                   );
    sql_query.SET_VALUE ( ":dizi_mi"             , dizi_mi             );
    sql_query.SET_VALUE ( ":dizi_boyutu"         , dizi_boyutu         );
    sql_query.SET_VALUE ( ":aciklama"            , aciklama            );
    sql_query.SET_VALUE ( ":alan_id"             , alan_id             );
    sql_query.UPDATE();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int SQL_TABLOLAR_FISI::CHECK_DELETE_LINE ( int p_tablo_id, int p_row_number )
{
    Q_UNUSED ( p_tablo_id   );
    Q_UNUSED ( p_row_number );

    if ( KAYIT_SILME_YETKISI_VAR_MI( KULLANICI_ID() ) EQ 0 ) {
        MSG_ERROR(tr("This user has no authority for Delete registration."),m_ui->lineEdit_tablo_adi);//Bu kullanıcının kayıt silme yetkisi yok.
        return ADAK_FAIL;
    }
    
    QLineEdit * cell_lineEdit       = ( QLineEdit * ) m_ui->tablewidget_tablo_satirlari->cellWidget( p_row_number, ALAN_ADI_COLUMN );
    QString     alan_adi            = cell_lineEdit->text();

    int return_value = MSG_YES_NO ( QString ( "The line with Field Name '% 1' will be deleted, are you sure?" ).arg(alan_adi), NULL );
    //Alan adı '%1' olan satır silinecektir, emin misiniz?
    if ( return_value EQ ADAK_NO ) {
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::DELETE_LINE
***************************************************************************************/

void SQL_TABLOLAR_FISI::DELETE_LINE ( int p_tablo_id, int p_row_number )
{
    int row_id = m_ui->tablewidget_tablo_satirlari->item ( p_row_number, ROW_ID_COLUMN )->text().toInt();

    SQL_QUERY sql_query( DB );

    sql_query.PREPARE_DELETE ( "sql_alanlar","tablo_id = :tablo_id AND alan_id = :alan_id" );
    sql_query.SET_VALUE      ( ":tablo_id", p_tablo_id );
    sql_query.SET_VALUE      ( ":alan_id" , row_id );
    sql_query.DELETE();
}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int SQL_TABLOLAR_FISI::LOCK_FIS_RECORD ( int p_tablo_id )
{
    return DB->LOCK_ROW( "tablo_id" , "sql_tablolar", QString ( "tablo_id = %1" ).arg ( p_tablo_id ));

}

/**************************************************************************************
                   SQL_TABLOLAR_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void SQL_TABLOLAR_FISI::UNLOCK_FIS_RECORD( int p_tablo_id )
{
    DB->UNLOCK_ROW( "tablo_id" , "sql_tablolar", QString ( "tablo_id = %1" ).arg ( p_tablo_id ));
}


/**************************************************************************************
                   SQL_TABLOLAR_FISI::SAVER_BUTTON_CLICKED
***************************************************************************************/

void SQL_TABLOLAR_FISI::SAVER_BUTTON_CLICKED( QAbstractButton * p_button, int p_tablo_id )
{
    if ( p_button EQ m_ui->button_indexler ) {
        if ( p_tablo_id < 1 ) {
            return;
        }
        OPEN_INDEX_BATCH ( p_tablo_id, this );
    }
    else if ( p_button EQ m_ui->button_header_olustur ) {
        SQL_QUERY sql_query ( DB );
        sql_query.PREPARE_SELECT( "sql_tablolar","tablo_id");
        if ( sql_query.SELECT() EQ 0 ) {
            MSG_ERROR(tr("Header file hasn't been created, because no registered table."),m_ui->lineEdit_tablo_adi);//Kayıtlı tablo olmadığından herhangi bir header dosyası yaratılmadı
            return;
        }
        if ( HEADER_DOSYASI_YARAT(this) EQ true ) {
            MSG_INFO(tr ("Header file has been successfully created."),m_ui->lineEdit_tablo_adi);//Header dosyası başarıyla yaratıldı
            return;
        }

        MSG_ERROR(tr ("File has not been created."),m_ui->lineEdit_tablo_adi);//Dosya yaratılmadı.
    }
}
