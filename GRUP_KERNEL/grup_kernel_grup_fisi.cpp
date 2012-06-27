#include <QHeaderView>
#include "adak_utils.h"
#include "kernel_utils.h"
#include "adak_defines.h"
#include "grup_kernel_grup_fisi.h"
#include "adak_gruplar.h"

#define   ROW_ID_COLUMN                       0
#define   ANA_GRUP_ID_COLUMN                  1
#define   ORDER_COLUMN                        2
#define   ANA_GRUP_KODU_COLUMN                3
#define   GRUP_KODU_COLUMN                    4
#define   TAM_GRUP_KODU_COLUMN                5
#define   GRUP_ISMI_COLUMN                    6

/**************************************************************************************
                   F_GRUP_KERNEL_GRUP_FISI
***************************************************************************************/

bool F_GRUP_KERNEL_GRUP_FISI ( int parent_id, ISLEM_TURU p_islem_turu, BASE_GRUP_KERNEL * base_grup,QWidget * parent,ADAK_SQL * db )
{
    GRUP_KERNEL_GRUP_FISI * alt_grup_ekleme = new GRUP_KERNEL_GRUP_FISI( parent_id, p_islem_turu, base_grup,parent,db );
    alt_grup_ekleme->SHOW          ( NOT_FULL_SCREEN );

    return true;
}

/**************************************************************************************
                   GRUP_KERNEL_GRUP_FISI::GRUP_KERNEL_GRUP_FISI
***************************************************************************************/

GRUP_KERNEL_GRUP_FISI::GRUP_KERNEL_GRUP_FISI ( int parent_id, ISLEM_TURU p_islem_turu, BASE_GRUP_KERNEL * base_grup,QWidget * parent,ADAK_SQL * db ) : FIS_KERNEL(parent)
{
    setupUi          ( this );

    m_islem_turu           = p_islem_turu;

    m_base_grup_kernel_ptr = base_grup;
    m_grup_id              = parent_id;
    grup_ekleme_db         = db;

    START_FIS_KERNEL ( this, grup_ekleme_db );
}

/**************************************************************************************
                   GRUP_KERNEL_GRUP_FISI_FISI::SETUP_FORM
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::SETUP_FORM()
{

    AUTOSTART_FIS_KERNEL( m_grup_id );

    if ( m_islem_turu EQ ENUM_ANA_GRUP ) {
        SET_PAGE_TITLE   ( tr ("ANA GRUPLAR" ) );
        SET_HELP_PAGE    ( tr ("ana-gruplar") );
        SET_SETTING_NAME ( "ANA_GRUPLAR" );
    }
    else {
        SET_PAGE_TITLE   ( tr ("ALT GRUPLAR" ) );
        SET_HELP_PAGE    ( tr ("alt-gruplar") );
        SET_SETTING_NAME ( "ALT_GRUPLAR" );
    }

    if ( m_islem_turu EQ ENUM_ALT_GRUP ) {

        line_edit_grup_kodu->setEnabled( false );
        lineEdit_grup_adi->setEnabled( false );
    }

    lineEdit_grup_adi->setMaxLength     ( 60 );
    line_edit_grup_kodu->setMaxLength   ( 15 );

    if ( grup_ekleme_db EQ NULL ) {
        qDebug ( "Database null olarak setlenmis" );
        return;
    }

    // program id ve modul id setlendi
    m_modul_id      = m_base_grup_kernel_ptr->GRP_GET_MODUL_ID();
    m_program_id    = m_base_grup_kernel_ptr->GRP_GET_PROGRAM_ID();

    SQL_QUERY query( grup_ekleme_db );

    query.PREPARE_SELECT( "adak_gruplar", "grup_parent_id, grup_seviyesi", "grup_id = :grup_id");
    query.SET_VALUE     ( ":grup_id", m_grup_id );

    if ( query.SELECT() NE 0 ) {
        query.NEXT();

        m_grup_parent_id = query.VALUE( 0 ).toInt();
        m_grup_seviyesi  = query.VALUE( 1 ).toInt();
    }


    p_grubun_parentini_aktar = false;

    p_delimeter         = m_base_grup_kernel_ptr->GET_DELIMETER();
    p_num_of_columns    = 7;

    REGISTER_TABLE_WIDGET   ( tablewidget_alt_gruplar, p_num_of_columns );
    REGISTER_BUTTONS_WIDGET ( kernel_buttons_widget );

    SET_ROW_ID_COLUMN_POSITION(ROW_ID_COLUMN);
    SET_FORM_ID_COLUMN_POSITION(ANA_GRUP_ID_COLUMN);
    SET_FIS_ORDER_COLUMN_NUMBER(ORDER_COLUMN);

    SET_FIRST_FOCUS_WIDGET  ( tablewidget_alt_gruplar );

    SET_NUMBER_OF_HIDDEN_COLUMN(3);

    SET_TABLE_ROW_WIDGETS(ANA_GRUP_KODU_COLUMN       ,WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(GRUP_KODU_COLUMN           ,WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(TAM_GRUP_KODU_COLUMN       ,WIDGET_LINE_EDIT);
    SET_TABLE_ROW_WIDGETS(GRUP_ISMI_COLUMN           ,WIDGET_LINE_EDIT);


    tablewidget_alt_gruplar->setColumnWidth ( ANA_GRUP_KODU_COLUMN            , 137 );
    tablewidget_alt_gruplar->setColumnWidth ( GRUP_KODU_COLUMN                , 138 );
    tablewidget_alt_gruplar->setColumnWidth ( TAM_GRUP_KODU_COLUMN            , 150 );
    tablewidget_alt_gruplar->setColumnWidth ( GRUP_ISMI_COLUMN                , 280 );

    tablewidget_alt_gruplar->setHorizontalHeaderLabels ( QStringList()<<"row_id"<<"grup_id"<<"satir_order"<< tr("Ana Grup Kodu") << tr("Grup Kodu")
                                                                      <<tr("Tam Grup Kodu")<<tr("Grup Adı")<<""<<"");

    tablewidget_alt_gruplar->setSelectionMode ( QAbstractItemView::NoSelection );
    tablewidget_alt_gruplar->setFocus         ( Qt::OtherFocusReason );

}

/**************************************************************************************
                   GRUP_KERNEL_GRUP_FISI::REOGNIZE_ORDER_NUMBER
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::REORGANIZE_ORDER_NUMBER (  int row_number, int new_order_number )
{
    Q_UNUSED ( row_number );
    Q_UNUSED ( new_order_number );
}

/**************************************************************************************
                   GRUP_KERNEL_GRUP_FISI::CLEAR_FORM_MEMBERS
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::CLEAR_FORM_MEMBERS()
{

}

/**************************************************************************************
                   GRUP_KERNEL_GRUP_FISI::GRUP_KERNEL_GRUP_FISI
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::NEW_FIS_RECORD()
{

}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::GET_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::GET_FIS_RECORD ( int ana_grup_id )
{
    SQL_QUERY select_query ( grup_ekleme_db );

    QString table_name  = "";
    QString column_name = "";
    QString where_name  = "";
    QString order_by    = "";

    m_base_grup_kernel_ptr->CREATE_SELECT_QUERY( &table_name, &column_name, &where_name, &order_by , -1, ana_grup_id );

    select_query.PREPARE_SELECT ( table_name, column_name, where_name, "" );

    if ( select_query.SELECT( order_by ) EQ 0 ) {
        return ADAK_FAIL;
    }

    select_query.NEXT();

    //"grup_id,grup_parent_id,grup_seviyesi,grup_kodu,grup_adi,tam_grup_kodu,grup_son_seviye_mi,gruptaki_kayit_sayisi,alt_grup_sayisi,modul_id"

    p_ust_tam_grup_kodu = select_query.VALUE(5).toString();
    p_ust_grup_seviyesi = select_query.VALUE(2).toInt();

    line_edit_grup_kodu->setText ( p_ust_tam_grup_kodu );
    lineEdit_grup_adi->setText   ( select_query.VALUE(4).toString() );
    line_edit_grup_kodu->setObjectName ( select_query.VALUE(3).toString());

    m_base_grup_kernel_ptr->CREATE_SELECT_QUERY(&table_name,&column_name,&where_name,&order_by ,ana_grup_id);

    select_query.PREPARE_SELECT( table_name,column_name,where_name,"");

    if ( select_query.SELECT() EQ 0 ) {
        return ADAK_OK;
    }

    while ( select_query.NEXT() EQ true ) {

        int current_row = ADD_NEW_LINE();

        QLineEdit * lineEdit_ana_grup_kodu                = ( QLineEdit *  ) tablewidget_alt_gruplar->cellWidget(current_row,ANA_GRUP_KODU_COLUMN);
        QLineEdit * lineEdit_ust_grup_kodu                = ( QLineEdit *  ) tablewidget_alt_gruplar->cellWidget(current_row,TAM_GRUP_KODU_COLUMN);
        QLineEdit * lineEdit_grup_kodu                    = ( QLineEdit *  ) tablewidget_alt_gruplar->cellWidget(current_row,GRUP_KODU_COLUMN);
        QLineEdit * lineEdit_grup_adi                     = ( QLineEdit *  ) tablewidget_alt_gruplar->cellWidget(current_row,GRUP_ISMI_COLUMN);

        lineEdit_ana_grup_kodu->setText ( p_ust_tam_grup_kodu );
        lineEdit_ust_grup_kodu->setText ( select_query.VALUE(5).toString() );
        lineEdit_grup_kodu->setText     ( select_query.VALUE(3).toString() );
        lineEdit_grup_adi->setText      ( select_query.VALUE(4).toString() );

        QTableWidgetItem * new_item = new QTableWidgetItem( select_query.VALUE(0).toString() );
        tablewidget_alt_gruplar->setItem(current_row,ROW_ID_COLUMN,new_item);

        new_item = new QTableWidgetItem(QVariant(ana_grup_id).toString());
        tablewidget_alt_gruplar->setItem(current_row,ANA_GRUP_ID_COLUMN,new_item);
    }
    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_ANA_GRUP_EKLEME::SET_LINE_DEFAULTS
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::SET_LINE_DEFAULTS ( int row_number )
{
    QLineEdit * lineEdit_ust_grup_kodu = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget ( row_number, TAM_GRUP_KODU_COLUMN );
    lineEdit_ust_grup_kodu->setMaxLength( 30 );
    lineEdit_ust_grup_kodu->setReadOnly ( true );

    QStringList ust_grup_tam_grup_kodu = line_edit_grup_kodu->text().split("-");
    lineEdit_ust_grup_kodu->setText( ust_grup_tam_grup_kodu.at(0) );


    QLineEdit * lineEdit_ana_grup_kodu = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget ( row_number, ANA_GRUP_KODU_COLUMN );
    lineEdit_ana_grup_kodu->setMaxLength(30);
    lineEdit_ana_grup_kodu->setDisabled(true);
    lineEdit_ana_grup_kodu->setText( ust_grup_tam_grup_kodu.at(0) );

    QLineEdit * lineEdit_grup_kodu     = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget ( row_number, GRUP_KODU_COLUMN );
    lineEdit_grup_kodu->setMaxLength((30 - ust_grup_tam_grup_kodu.at(0).size()));
    QLineEdit * lineEdit_grup_adi      = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget( row_number, GRUP_ISMI_COLUMN );
    lineEdit_grup_adi->setMaxLength(60);

}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_FIS_FORM_VAR
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_FIS_FORM_VAR ( QObject * object )
{
    if ( object EQ line_edit_grup_kodu ) {
        QString tam_grup_kodu = line_edit_grup_kodu->text();
        SQL_QUERY select_query ( grup_ekleme_db );

        select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                      QString("%3 = :tam_grup_kodu AND %4 = :modul_id AND %5 = :program_id")
                                     .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                     .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME())
                                     .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME()),"");

        select_query.SET_VALUE      (":tam_grup_kodu" , tam_grup_kodu);
        select_query.SET_VALUE      (":modul_id"      , m_base_grup_kernel_ptr->GRP_GET_MODUL_ID());
        select_query.SET_VALUE      (":program_id"    , m_base_grup_kernel_ptr->GRP_GET_PROGRAM_ID());

        if ( select_query.SELECT() > 0 ) {
            MSG_WARNING(tr ( "Aynı grup koduyla kayıtlı başka bir grup sistemde mevcuttur."
                                                  "Lütfen grup kodunuzu değiştiriniz.."),NULL);
            line_edit_grup_kodu->undo();
            return ADAK_FAIL;
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_FIS_FORM_EMPTY
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_FIS_FORM_EMPTY()
{

    if ( line_edit_grup_kodu->text().isEmpty() EQ true) {
        MSG_WARNING(tr ( "Grup kodu boş bırakılamaz" ) ,NULL);
        return ADAK_FAIL;
    }
    if ( lineEdit_grup_adi->text().isEmpty() EQ true ) {
        MSG_WARNING(tr ( "Grup ismi boş bırakılamaz" ) ,NULL);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_ANA_GRUP_EKLEME::CHECK_FIS_RECORD_ADD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_FIS_RECORD_ADD()
{
    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_ANA_GRUP_EKLEME::ADD_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::ADD_FIS_RECORD()
{

    SQL_QUERY insert_query ( grup_ekleme_db );

    QString columns = QString ("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10")    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME())
                                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME());

    insert_query.PREPARE_INSERT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(), m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(), columns );

    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()       , 0 );
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()   , 1 );
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()       , line_edit_grup_kodu->text());
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()        , lineEdit_grup_adi->text()  );
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()   , line_edit_grup_kodu->text());
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()    , 1);
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()      , 0);
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME() , 0);
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME()      , m_base_grup_kernel_ptr->GRP_GET_PROGRAM_ID()    );
    insert_query.SET_VALUE ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME()        , m_base_grup_kernel_ptr->GRP_GET_MODUL_ID()      );

    int grup_id = insert_query.INSERT();

    m_base_grup_kernel_ptr->UPDATE_GRUP_GUI_EKLENEN_KAYIT_SATIR ( QStringList () << line_edit_grup_kodu->text() <<lineEdit_grup_adi->text()
                                                                 << "0" << "0" << QVariant ( grup_id ).toString() << "0" << "1" << line_edit_grup_kodu->text());


    line_edit_grup_kodu->setObjectName( line_edit_grup_kodu->text() );

    // yeni grupta grup seviyesi 1 dir
    p_ust_tam_grup_kodu = line_edit_grup_kodu->text();
    p_ust_grup_seviyesi = 1;

    if ( tablewidget_alt_gruplar->rowCount() EQ 1 ) {
        SET_LINE_DEFAULTS( 0 );
    }

    return grup_id;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_FIS_RECORD_UPDATE
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_FIS_RECORD_UPDATE ( int fis_id )
{
    Q_UNUSED ( fis_id );
    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::UPDATE_FIS_RECORD
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::UPDATE_FIS_RECORD ( int fis_id )
{
    QString tam_grup_kodu;

    SQL_QUERY select_query ( grup_ekleme_db );

    QString columns = QString("%1,%2").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                            .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME());

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),columns,
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME() + "= :grup_id"), "" );

    select_query.SET_VALUE ( ":grup_id" , fis_id );

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }

    select_query.NEXT();

    int     p_parent_id           = select_query.VALUE(0).toInt();
    int     p_grup_son_seviyemi   = select_query.VALUE(1).toInt();


    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME(),
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id" ),"");

    select_query.SET_VALUE      ( ":grup_id", fis_id );
    if ( select_query.SELECT() > 0 ) {
        select_query.NEXT();
        int p_parent_id = select_query.VALUE(0).toInt();
        if ( p_parent_id EQ 0 ) {
            tam_grup_kodu.clear();
        }

        else {
            select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME(),
                                          QString (  m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME() + "= :grup_id" ),"");

            select_query.SET_VALUE      ( ":grup_id", p_parent_id );
            if ( select_query.SELECT() > 0 ) {
                select_query.NEXT();
                tam_grup_kodu.append ( select_query.VALUE(0).toString());
                tam_grup_kodu.append ( p_delimeter);
            }
        }
    }
    tam_grup_kodu.append ( line_edit_grup_kodu->text());

    int gruptaki_kayit_sayisi = 0;

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME(),
                                  QString (m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME() +"= :grup_id"),"");

    select_query.SET_VALUE      ( ":grup_id", fis_id );
    if ( select_query.SELECT() > 0  ) {
         select_query.NEXT();
         gruptaki_kayit_sayisi = select_query.VALUE(0).toInt();
    }

    SQL_QUERY update_query ( grup_ekleme_db );

    columns = QString ( "%1,%2,%3" ).arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME());


    update_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),columns,
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id" ));

    update_query.SET_VALUE     ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()      , line_edit_grup_kodu->text() );
    update_query.SET_VALUE     ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()       , lineEdit_grup_adi->text() );
    update_query.SET_VALUE     ( ":" + m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()  , tam_grup_kodu );
    update_query.SET_VALUE     ( ":grup_id" , fis_id );
    update_query.UPDATE();

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  QString( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"= :grup_parent_id"),"");
    select_query.SET_VALUE     ( ":grup_parent_id" , fis_id );


    m_base_grup_kernel_ptr->UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP ( QStringList () << line_edit_grup_kodu->text()          << lineEdit_grup_adi->text() <<
                                                       QVariant ( select_query.SELECT()).toString() <<QVariant(gruptaki_kayit_sayisi).toString()<<
                                                       QVariant ( fis_id ).toString()             <<QVariant(p_parent_id).toString()<<
                                                       QVariant ( p_grup_son_seviyemi ).toString()   <<tam_grup_kodu);

    if ( line_edit_grup_kodu->text() NE line_edit_grup_kodu->objectName()) {

         select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),
                                       QString(m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()+","+ m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()),
                                       QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"= :grup_id" ),"");

        select_query.SET_VALUE       ( ":grup_id", fis_id );
        if ( select_query.SELECT() EQ 0 ) {
            close();
            return;
        }

        SQL_QUERY update_query ( grup_ekleme_db );

        while ( select_query.NEXT() EQ true ) {
            QString tam_grup_kodu = select_query.VALUE(0).toString();
            int     grup_id       = select_query.VALUE(1).toInt();
            tam_grup_kodu.replace ( line_edit_grup_kodu->objectName(), line_edit_grup_kodu->text());

            update_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                          m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME(),
                                          QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));

            update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME(),tam_grup_kodu );
            update_query.SET_VALUE      ( ":grup_id"      ,grup_id );

            update_query.UPDATE();
        }
    }
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_FIS_RECORD_DELETE
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_FIS_RECORD_DELETE(int fis_id)
{
    SQL_QUERY query ( grup_ekleme_db );

    query.PREPARE_SELECT( "adak_gruplar", "grup_id, grup_parent_id", " grup_parent_id = :grup_parent_id");
    query.SET_VALUE     ( ":grup_parent_id",  fis_id );

    int alt_grup_sayisi = query.SELECT();

    if ( alt_grup_sayisi NE 0 ) {
        MSG_WARNING( tr( "Önce Alt Grupları Mevcut Silinemez" ), NULL );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::DELETE_FIS_RECORD
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::DELETE_FIS_RECORD ( int grup_id )
{
    SQL_QUERY query ( grup_ekleme_db );

    query.PREPARE_DELETE( "adak_gruplar", " grup_id = :grup_id");
    query.SET_VALUE     ( ":grup_id", grup_id );
    query.DELETE();

}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::SELECT_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::SELECT_FIS_RECORD()
{
    return 0;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::FIND_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::FIND_FIS_RECORD()
{
    return 1;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::FIND_FIRST_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::FIND_FIRST_FIS_RECORD()
{

    SQL_QUERY query ( grup_ekleme_db );

    query.PREPARE_SELECT ( "adak_gruplar", "grup_id",
                           "grup_seviyesi = :grup_seviyesi "
                           "AND grup_parent_id = :grup_parent_id "
                           "AND program_id = :program_id "
                           "AND modul_id   = :modul_id "  );

    query.SET_VALUE      ( ":grup_seviyesi" , m_grup_seviyesi       );
    query.SET_VALUE      ( ":grup_parent_id", m_grup_parent_id      );
    query.SET_VALUE      ( ":program_id"    , m_program_id          );
    query.SET_VALUE      ( ":modul_id"      , m_modul_id            );

    if ( query.SELECT("grup_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::FIND_LAST_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::FIND_LAST_FIS_RECORD()
{
    SQL_QUERY query ( grup_ekleme_db );

    query.PREPARE_SELECT ( "adak_gruplar", "grup_id",
                           "grup_seviyesi  = :grup_seviyesi "
                           "AND grup_parent_id  = :grup_parent_id "
                           "AND program_id = :program_id "
                           "AND modul_id   = :modul_id "
                                );

    query.SET_VALUE      ( ":grup_seviyesi" , m_grup_seviyesi   );
    query.SET_VALUE      ( ":grup_parent_id", m_grup_parent_id  );
    query.SET_VALUE      ( ":program_id"    , m_program_id      );
    query.SET_VALUE      ( ":modul_id"      , m_modul_id        );

    if ( query.SELECT("grup_kodu DESC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();

}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::FIND_PREV_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::FIND_PREV_FIS_RECORD()
{
    SQL_QUERY query ( grup_ekleme_db );

    query.PREPARE_SELECT ( "adak_gruplar", "grup_id",
                           "grup_seviyesi       = :grup_seviyesi "
                           "AND grup_parent_id  = :grup_parent_id "
                           "AND program_id      = :program_id "
                           "AND modul_id        = :modul_id "
                           "AND grup_kodu       < :grup_kodu " );

    query.SET_VALUE      ( ":grup_seviyesi" , m_grup_seviyesi               );
    query.SET_VALUE      ( ":grup_parent_id", m_grup_parent_id              );
    query.SET_VALUE      ( ":program_id"    , m_program_id                  );
    query.SET_VALUE      ( ":modul_id"      , m_modul_id                    );
    query.SET_VALUE      ( ":grup_kodu"     , line_edit_grup_kodu->text()   );

    if ( query.SELECT("grup_kodu DESC", 0, 1 ) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::FIND_NEXT_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::FIND_NEXT_FIS_RECORD()
{
    SQL_QUERY query ( grup_ekleme_db );

    query.PREPARE_SELECT ( "adak_gruplar", "grup_id",
                           "grup_seviyesi       = :grup_seviyesi "
                           "AND grup_parent_id  = :grup_parent_id "
                           "AND program_id      = :program_id "
                           "AND modul_id        = :modul_id "
                           "AND grup_kodu       > :grup_kodu " );

    query.SET_VALUE      ( ":grup_seviyesi" , m_grup_seviyesi               );
    query.SET_VALUE      ( ":grup_parent_id", m_grup_parent_id              );
    query.SET_VALUE      ( ":program_id"    , m_program_id                  );
    query.SET_VALUE      ( ":modul_id"      , m_modul_id                    );
    query.SET_VALUE      ( ":grup_kodu"     , line_edit_grup_kodu->text()   );

    if ( query.SELECT("grup_kodu ASC",0,1) EQ 0 ) {
        return 0;
    }

    query.NEXT();

    return query.VALUE(0).toInt();
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_LINE_VAR
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_LINE_VAR ( int row_number, QObject * object )
{
    QLineEdit * lineEdit_grup_kodu     =  ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget(row_number,GRUP_KODU_COLUMN);
    QLineEdit * lineEdit_tam_grup_kodu =  ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget(row_number,TAM_GRUP_KODU_COLUMN);

    if ( object EQ lineEdit_grup_kodu ) {
        QString tam_grup_kodu = line_edit_grup_kodu->text().split("-").at(0);
        if ( lineEdit_grup_kodu->text().isEmpty() EQ false ) {
             QString tam_hesap_kodu = tam_grup_kodu + p_delimeter + lineEdit_grup_kodu->text();
             lineEdit_tam_grup_kodu->setText ( tam_hesap_kodu );
        }
        else {
            lineEdit_tam_grup_kodu->setText ( tam_grup_kodu );
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_LINE_EMPTY
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_LINE_EMPTY ( int row_number )
{
    QLineEdit * lineEdit_grup_kodu =  ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget(row_number,GRUP_KODU_COLUMN);
    QLineEdit * lineEdit_grup_adi  =  ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget(row_number,GRUP_ISMI_COLUMN);

    if ( lineEdit_grup_kodu->text().isEmpty() EQ true ) {
        MSG_WARNING(tr ( "Grup kodunu boş bırakamazsınız!.."), lineEdit_grup_kodu);
        return ADAK_FAIL;
    }
    if ( lineEdit_grup_adi->text().isEmpty() EQ true) {
         MSG_WARNING(tr ( "Grup ismini boş bırakamazsınız!.." ) , lineEdit_grup_adi );
         return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_ADD_LINE
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_ADD_LINE ( int fis_id, int row_number )
{
    Q_UNUSED ( fis_id );

    QLineEdit * lineEdit_grup_kodu  = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget( row_number, GRUP_KODU_COLUMN );
    SQL_QUERY select_query ( grup_ekleme_db );


    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),QString(m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME()+","+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()),
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    select_query.SET_VALUE ( ":grup_id",GET_RECORD_ID());
    if ( select_query.SELECT() EQ 0 ){
        return ADAK_FAIL;
    }
    select_query.NEXT();

    if ( select_query.VALUE(0).toInt() EQ 0 AND select_query.VALUE(1).toInt() EQ 1) {
        p_grubun_parentini_aktar = true;
    }

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME(),
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    select_query.SET_VALUE      ( ":grup_id",GET_RECORD_ID() );
    if ( select_query.SELECT() EQ 0 ){
        return ADAK_FAIL;
    }
    select_query.NEXT();


    QString tam_grup_kodu = select_query.VALUE(0).toString() + p_delimeter + lineEdit_grup_kodu->text();

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  QString("%3 = :tam_grup_kodu AND %4 = :modul_id AND %5 = :program_id")
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME())
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME()),"");


    select_query.SET_VALUE      (":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME() , tam_grup_kodu);
    select_query.SET_VALUE      (":"+m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME()      , m_base_grup_kernel_ptr->GRP_GET_MODUL_ID());
    select_query.SET_VALUE      (":"+m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME()    , m_base_grup_kernel_ptr->GRP_GET_PROGRAM_ID());

    if ( select_query.SELECT() > 0 ) {
        MSG_WARNING(tr ( "Aynı grup koduna sahip,başka bir alt grup bulunmaktadır.Lütfen grup kodunuzu değiştiriniz!.."), lineEdit_grup_kodu );
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::ADD_LINE
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::ADD_LINE ( int fis_id, int row_number )
{
    QLineEdit * lineedit_grup_kodu =  ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget(row_number,GRUP_KODU_COLUMN);
    QString      alt_grup_kodu     = lineedit_grup_kodu->text();

    QLineEdit * lineedit_grup_adi  =  ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget(row_number,GRUP_ISMI_COLUMN);
    QString     alt_grup_adi       = lineedit_grup_adi->text();

    QString     alt_tam_grup_kodu;

    int         alt_grup_sayisi    = 0;

    alt_tam_grup_kodu.append ( p_ust_tam_grup_kodu );
    alt_tam_grup_kodu.append ( p_delimeter );
    alt_tam_grup_kodu.append ( alt_grup_kodu );

    int parent_grup_kayit_sayisi = 0;

    SQL_QUERY select_query ( grup_ekleme_db );
    SQL_QUERY update_query ( grup_ekleme_db );

    int parent_id = 0;

    QString columns = QString ("%1,%2,%3").arg(m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME())
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME());

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),columns,QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    select_query.SET_VALUE       ( ":grup_id",fis_id);
    if ( select_query.SELECT() NE 0 ) {
        select_query.NEXT();
        alt_grup_sayisi = select_query.VALUE(0).toInt();
        alt_grup_sayisi++;
        parent_grup_kayit_sayisi = select_query.VALUE(1).toInt();
        parent_id                = select_query.VALUE(2).toInt();
    }

    update_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME(),QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));

    update_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME(), alt_grup_sayisi);
    update_query.SET_VALUE(":grup_id "                                                          , fis_id );
    update_query.UPDATE();

    SQL_QUERY insert_query ( grup_ekleme_db );

    columns = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME())
                                                               .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME());

    insert_query.PREPARE_INSERT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),columns);
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()           , fis_id                         );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()       , p_ust_grup_seviyesi + 1        );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()           , alt_grup_kodu                  );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()            , alt_grup_adi                   );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()       , alt_tam_grup_kodu              );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()        , 1                              );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()          , 0                              );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME()     , 0                              );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME()          , m_base_grup_kernel_ptr->GRP_GET_PROGRAM_ID()  );
    insert_query.SET_VALUE(":"+m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME()            , m_base_grup_kernel_ptr->GRP_GET_MODUL_ID()    );

    int alt_grup_id = insert_query.INSERT();

    if ( p_grubun_parentini_aktar EQ true ) {

        columns = QString ("%1,%2,%3,%4,%5").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                            .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                            .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                            .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                            .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME());

        select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),columns,QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+" = :grup_id"),"");
        select_query.SET_VALUE      ( ":grup_id", fis_id );

        if ( select_query.SELECT() EQ 0 ) {
             MSG_WARNING(tr ( "Üst grup bilgileri alınırken hata oluştu" ) ,NULL);
             return;
        }

        select_query.NEXT();
        int     temp_grup_parent_id            = select_query.VALUE(0).toInt();
        QString temp_grup_kodu                 = select_query.VALUE(1).toString();
        QString temp_grup_adi                  = select_query.VALUE(2).toString();
        QString temp_tam_grup_kodu             = select_query.VALUE(3).toString();
        int     temp_grupta_kayitli_uye_sayisi = select_query.VALUE(4).toInt();

        columns = QString("%1,%2,%3,%4,%5,%6,%7,%8").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                                    .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME());

        update_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                      columns,QString (m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+" = :grup_id"));

        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()       , temp_grup_parent_id      );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()   , p_ust_grup_seviyesi      );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()       , temp_grup_kodu           );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()        , temp_grup_adi            );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()   , temp_tam_grup_kodu       );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()    , 0                        );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()      , parent_grup_kayit_sayisi );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME() , alt_grup_sayisi          );
        update_query.SET_VALUE      ( ":grup_id"                                                            , alt_grup_id              );

        update_query.UPDATE();

        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()       , alt_grup_id                    );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()   , p_ust_grup_seviyesi + 1        );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()       , alt_grup_kodu                  );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()        , alt_grup_adi                   );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()   , alt_tam_grup_kodu              );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()    , 1                              );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()      , temp_grupta_kayitli_uye_sayisi );
        update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME() , 0                              );
        update_query.SET_VALUE      ( ":grup_id"                                                            , fis_id                         );


        update_query.UPDATE();

        m_base_grup_kernel_ptr->UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP ( QStringList() << line_edit_grup_kodu->objectName()
                           << QString(" ")
                           << QVariant(alt_grup_sayisi).toString()
                           << QVariant(parent_grup_kayit_sayisi).toString()
                           << QVariant(alt_grup_id).toString()
                           << QVariant(temp_grup_parent_id).toString()
                           << QVariant(0).toString()
                           << line_edit_grup_kodu->objectName() );

        QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( fis_id ).toString() );
        tablewidget_alt_gruplar->setItem ( row_number,ROW_ID_COLUMN,new_item );

        new_item = new QTableWidgetItem ( QVariant ( alt_grup_id ).toString() );
        tablewidget_alt_gruplar->setItem ( row_number,ANA_GRUP_ID_COLUMN,new_item );

        SET_CURRENT_RECORD_ID( alt_grup_id );
        p_grubun_parentini_aktar = false;

        int last_column = tablewidget_alt_gruplar->rowCount() -1;
        QLineEdit * lineEdit = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget( last_column , GRUP_KODU_COLUMN);
        lineEdit->setFocus();
        tablewidget_alt_gruplar->setCurrentCell ( last_column,GRUP_KODU_COLUMN );

        return;
    }

    QTableWidgetItem * new_item = new QTableWidgetItem ( QVariant ( alt_grup_id ).toString() );
    tablewidget_alt_gruplar->setItem ( row_number,ROW_ID_COLUMN,new_item );

    new_item = new QTableWidgetItem ( QVariant ( fis_id ).toString() );
    tablewidget_alt_gruplar->setItem ( row_number,ANA_GRUP_ID_COLUMN,new_item );


    update_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME(),QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+" = :grup_id" ));
    update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME(), 0 );
    update_query.SET_VALUE      ( ":grup_id", fis_id );
    update_query.UPDATE();

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(), m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  QString (  m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"=:grup_parent_id"),"");
    select_query.SET_VALUE      ( ":grup_parent_id" , fis_id);

    QStringList ust_label_info = line_edit_grup_kodu->text().split("-");

    m_base_grup_kernel_ptr->UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP ( QStringList () << ust_label_info.at(0)          << QString( " " ) + "\t" +
                                             QVariant ( select_query.SELECT()).toString()<<QVariant ( parent_grup_kayit_sayisi).toString() <<
                                             QVariant(fis_id).toString()                 << QVariant ( parent_id ).toString()              <<
                                             QVariant(0).toString()                      << ust_label_info.at(0));

}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_UPDATE_LINE ( int fis_id, int row_number )
{
    QLineEdit * lineEdit_grup_kodu  = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget( row_number, GRUP_KODU_COLUMN );
    SQL_QUERY select_query ( grup_ekleme_db );

    int row_id = tablewidget_alt_gruplar->item ( row_number,ROW_ID_COLUMN )->text().toInt();

    select_query.PREPARE_SELECT(m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME(),
                                QString(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    select_query.SET_VALUE(":grup_id",fis_id);
    if ( select_query.SELECT() EQ 0 ){
        return ADAK_FAIL;
    }
    select_query.NEXT();

    QString tam_grup_kodu = select_query.VALUE(0).toString() + p_delimeter + lineEdit_grup_kodu->text();

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  QString(" %3 = :tam_grup_kodu AND %4 = :modul_id AND %5 = :program_id AND %6 != :grup_id")
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_MODUL_ID_COLUMN_NAME())
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME())
                                 .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()),"");


    select_query.SET_VALUE      (":tam_grup_kodu" , tam_grup_kodu);
    select_query.SET_VALUE      (":modul_id"      , m_base_grup_kernel_ptr->GRP_GET_MODUL_ID());
    select_query.SET_VALUE      (":program_id"    , m_base_grup_kernel_ptr->GRP_GET_PROGRAM_ID());
    select_query.SET_VALUE      (":grup_id"       , row_id);

    if ( select_query.SELECT() > 0 ) {
         MSG_WARNING(tr ( "Aynı grup koduna sahip, başka bir alt grup bulunmaktadır.Lütfen grup kodunuzu değiştiriniz!.."),NULL);
         return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_UPDATE_LINE
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::UPDATE_LINE ( int fis_id, int row_number )
{
    QLineEdit * grup_kodu_lineEdit        = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget( row_number, GRUP_KODU_COLUMN );
    QLineEdit * grup_adi_lineEdit         = ( QLineEdit * ) tablewidget_alt_gruplar->cellWidget( row_number, GRUP_ISMI_COLUMN );

    SQL_QUERY select_query ( grup_ekleme_db );

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME(),
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");

    select_query.SET_VALUE(":grup_id", fis_id);

    if ( select_query.SELECT() EQ 0 ) {
        return;
    }
    select_query.NEXT();

    QString tam_grup_kodu = select_query.VALUE(0).toString() + p_delimeter + grup_kodu_lineEdit->text();

    int grup_id           = tablewidget_alt_gruplar->item(row_number,ROW_ID_COLUMN)->text().toInt();


    int gruptaki_kayit_sayisi = 0;

    select_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME(),
                                  QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    select_query.SET_VALUE     ( ":grup_id", fis_id );
    if ( select_query.SELECT() > 0 ) {
        select_query.NEXT();
        gruptaki_kayit_sayisi = select_query.VALUE(0).toInt();
    }

    SQL_QUERY update_query ( grup_ekleme_db );

    QString columns = QString ("%1,%2,%3").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                          .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                          .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME());

    update_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                  columns,QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));
    update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()     , grup_kodu_lineEdit->text());
    update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()      , grup_adi_lineEdit->text());
    update_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME() , tam_grup_kodu );
    update_query.SET_VALUE      ( ":grup_id"       , grup_id );
    update_query.UPDATE();
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::CHECK_DELETE_LINE
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::CHECK_DELETE_LINE ( int fis_id, int row_number )
{
    p_grubun_parentini_aktar = false;

    SQL_QUERY sql_query ( grup_ekleme_db );

    int grup_id        = tablewidget_alt_gruplar->item ( row_number,ROW_ID_COLUMN )->text().toInt();

    sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                               QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"= :grup_parent_id"),"");
    sql_query.SET_VALUE     (":grup_parent_id" , grup_id );

    if ( sql_query.SELECT() NE 0 ) {
        MSG_WARNING(tr ( "Silmek istediğiniz grubun alt grupları var,önce onları silmelisiniz!.."),NULL);
        return ADAK_FAIL;
    }

    sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),
                               QString (m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME() +","+ m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()),
                               m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+" = :grup_id" ,"");
    sql_query.SET_VALUE      ( ":grup_id", grup_id );
    if (sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();

        ADAK_MSG_ENUM answer = MSG_YES_NO(tr("'%1 %2' grubu silinecek.").arg(sql_query.VALUE(0).toString()).arg(sql_query.VALUE(1).toString()),NULL);

        if (answer EQ ADAK_NO OR answer EQ ADAK_CANCEL) {
            return ADAK_FAIL;
        }
    }

    sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME(),
                               QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    sql_query.SET_VALUE     (":grup_id", grup_id );

    if ( sql_query.SELECT() EQ 0 ) {
        MSG_ERROR(tr ( "Grupta kayıtlı üye sayısı alınırken hata oluştu."),NULL);
        return ADAK_FAIL;
    }
    sql_query.NEXT();
    if ( sql_query.VALUE(0).toInt() EQ 0 ) {
        return ADAK_OK;
    }

    sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                               QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"= :grup_parent_id"),"");
    sql_query.SET_VALUE      ( ":grup_parent_id", fis_id );

    int parent_alt_grup_sayisi = sql_query.SELECT();

    if ( parent_alt_grup_sayisi EQ 0 ) {
        MSG_ERROR(tr ( "Ana grubun alt grup sayısı alınırken hata oluştu."  ) ,NULL );
        return ADAK_FAIL;
    }

    if ( parent_alt_grup_sayisi > 1 ) {
        MSG_ERROR(tr ( "Grupta kayıt var. Grubu silemezsiniz." ) , NULL);
        return ADAK_FAIL;
    }

    p_grubun_parentini_aktar = true;
    return ADAK_OK;
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::DELETE_LINE
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::DELETE_LINE ( int fis_id, int row_number )
{
    int gruptaki_kayit_sayisi = 0;
    int alt_grup_sayisi       = 0;

    SQL_QUERY sql_query   ( grup_ekleme_db );

    sql_query.PREPARE_SELECT(m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME(),
                             QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");

    sql_query.SET_VALUE(":grup_id", fis_id);
    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        gruptaki_kayit_sayisi = sql_query.VALUE(0).toInt();
    }

    int alt_grup_id = tablewidget_alt_gruplar->item(row_number,ROW_ID_COLUMN)->text().toInt();
    int grup_uye_sayisi = 0;
    int parent_id       = 0;

    QString columns = QString("%1,%2,%3").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME())
                                         .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                         .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME());

    sql_query.PREPARE_SELECT(m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),columns,QString(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"),"");
    sql_query.SET_VALUE(":grup_id",fis_id);
    if ( sql_query.SELECT() NE 0 ) {
        sql_query.NEXT();
        alt_grup_sayisi = sql_query.VALUE(0).toInt();
        alt_grup_sayisi--;
        grup_uye_sayisi = sql_query.VALUE(1).toInt();
        parent_id       = sql_query.VALUE(2).toInt();
    }

    sql_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME(),
                               QString(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));
    sql_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME() , alt_grup_sayisi);
    sql_query.SET_VALUE     ( ":grup_id"         , fis_id );
    sql_query.UPDATE();

    if ( p_grubun_parentini_aktar EQ false ) {

        sql_query.PREPARE_DELETE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));
        sql_query.SET_VALUE     ( ":grup_id", alt_grup_id );
        sql_query.DELETE();

        sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                   QString(m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()+"= :grup_parent_id" ),"");
        sql_query.SET_VALUE      ( ":grup_parent_id", fis_id );

        if ( sql_query.SELECT() EQ 0 ) {
            sql_query.PREPARE_UPDATE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                       m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME(),QString(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id" ));
            sql_query.SET_VALUE      ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME(), 1);
            sql_query.SET_VALUE      ( ":grup_id", fis_id );
            sql_query.UPDATE();
        }

        m_base_grup_kernel_ptr->UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP ( QStringList()    << line_edit_grup_kodu->objectName()        << QString("") <<
                                                 QVariant ( alt_grup_sayisi).toString() << QVariant ( grup_uye_sayisi).toString()   <<
                                                 QVariant ( fis_id ).toString()         <<  QVariant ( parent_id).toString()        <<
                                                 QVariant(0).toString()                 <<  line_edit_grup_kodu->objectName() );

        return;
    }

    columns = QString("%1,%2,%3,%4,%5,%6").arg(m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME() )
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME() )
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME() )
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME() )
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME() )
                                          .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME() );

    sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),columns, QString (m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+ "= :grup_id" ),"");
    sql_query.SET_VALUE      ( ":grup_id", fis_id );

    if ( sql_query.SELECT() > 0 ) {
        sql_query.NEXT();
        int     grup_parent_id  = sql_query.VALUE(0).toInt();
        int     grup_seviyesi   = sql_query.VALUE(1).toInt();
        QString grup_kodu       = sql_query.VALUE(2).toString();
        QString grup_adi        = sql_query.VALUE(3).toString();
        QString tam_grup_kodu   = sql_query.VALUE(4).toString();
        int     grup_uye_sayisi = sql_query.VALUE(5).toInt();

        columns = QString("%1,%2,%3,%4,%5,%6,%7").arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME())
                                                 .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME())
                                                 .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME())
                                                 .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME())
                                                 .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME())
                                                 .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME())
                                                 .arg ( m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME());

        SQL_QUERY update_query ( grup_ekleme_db );

        update_query.PREPARE_UPDATE (m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(), m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                                     columns,QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id"));

        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME()        , grup_parent_id  );
        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME()    , grup_seviyesi   );
        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_KODU_COLUMN_NAME()        , grup_kodu       );
        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_GRUP_ADI_COLUMN_NAME()         , grup_adi        );
        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME()    , tam_grup_kodu   );
        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME()       , grup_uye_sayisi );
        update_query.SET_VALUE     ( ":"+m_base_grup_kernel_ptr->GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME()     , 1               );
        update_query.SET_VALUE     ( ":grup_id"                                                             , alt_grup_id     );

        update_query.UPDATE();

        sql_query.PREPARE_DELETE ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),QString ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()+"= :grup_id " ));
        sql_query.SET_VALUE      ( ":grup_id", fis_id );
        sql_query.DELETE();
    }

    sql_query.PREPARE_SELECT ( m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(),m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(),
                               QString("%1 = :grup_parent_id AND %2 != :grup_id")
                               .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_PARENT_ID_COLUMN_NAME() )
                               .arg(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME() ) ,"");
    sql_query.SET_VALUE      ( ":grup_parent_id" , fis_id     );
    sql_query.SET_VALUE      ( ":grup_id"        , alt_grup_id );

    QStringList ust_grup_info = line_edit_grup_kodu->text().split("-");

    m_base_grup_kernel_ptr->UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP(QStringList () << line_edit_grup_kodu->objectName() <<ust_grup_info.at(1)  <<
                                             QVariant(alt_grup_sayisi).toString()      << QVariant(grup_uye_sayisi).toString()          <<
                                             QVariant(fis_id).toString()               << QVariant(parent_id).toString()                <<
                                             QVariant(0).toString()                    << line_edit_grup_kodu->objectName());
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::LOCK_FIS_RECORD
***************************************************************************************/

int GRUP_KERNEL_GRUP_FISI::LOCK_FIS_RECORD ( int fis_id )
{
    return grup_ekleme_db->LOCK_ROW ( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(), m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(), QString ( "%1 = %2" ).arg(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()).arg ( fis_id ));
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::UNLOCK_FIS_RECORD
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::UNLOCK_FIS_RECORD ( int fis_id )
{
    grup_ekleme_db->UNLOCK_ROW( m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME(), m_base_grup_kernel_ptr->GET_GRUPLAR_TABLE_NAME(), QString ( "%1 = %2" ).arg(m_base_grup_kernel_ptr->GET_GRUPLAR_ID_COLUMN_NAME()).arg ( fis_id ));
}

/**************************************************************************************
                GRUP_KERNEL_GRUP_FISI::LINE_CHANGER_BUTTON_CLICKED
***************************************************************************************/

void GRUP_KERNEL_GRUP_FISI::LINE_SAVER_BUTTON_CLICKED(QAbstractButton *button, int row_id, int record_id, int row_number)
{
    Q_UNUSED(record_id);
    Q_UNUSED(button);
    Q_UNUSED(row_id);
    Q_UNUSED(row_number);

}
