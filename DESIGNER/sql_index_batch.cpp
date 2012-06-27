#include <QHeaderView>

#include "adak_utils.h"
#include "sql_index_batch.h"
#include "ui_sql_index_batch.h"

#include "sql_designer.h"

#define INDEX_ADI_COLUMN     0
#define ALAN_ADI_1_COLUMN    1
#define ALAN_ADI_2_COLUMN    2
#define ALAN_ADI_3_COLUMN    3
#define ALAN_ADI_4_COLUMN    4

#define NUMBER_OF_ROWS       9
#define NUMBER_OF_COMBOBOX   4


int         INDEXLER_ARRAY[INDEXLER_ARRAY_SIZE];
QComboBox * COMBOBOX_ARRAY[NUMBER_OF_ROWS][NUMBER_OF_COMBOBOX];
QString     INDEX_NAMES[] = {"INDEX 2","INDEX 3","INDEX 4","INDEX 5","INDEX 6","INDEX 7","INDEX 8","INDEX 9","INDEX 10"};

extern ADAK_SQL *   DB;

/**************************************************************************************
                   OPEN_INDEX_BATCH
***************************************************************************************/

void OPEN_INDEX_BATCH ( int tablo_id, QWidget * parent )
{
    SQL_INDEX_BATCH * B = new SQL_INDEX_BATCH(tablo_id, parent);
    B->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   SQL_INDEX_BATCH::SQL_INDEX_BATCH
***************************************************************************************/

SQL_INDEX_BATCH::SQL_INDEX_BATCH(int tablo_id, QWidget *parent) :  BATCH_KERNEL (parent), m_ui(new Ui::SQL_INDEX_BATCH),
p_tablo_id (tablo_id)
{
    m_ui->setupUi     (this);

    START_BATCH_KERNEL(this, DB);
}

/**************************************************************************************
                   SQL_INDEX_BATCH::SETUP_FORM
***************************************************************************************/

void SQL_INDEX_BATCH::SETUP_FORM()
{
    SET_NAME_OF_RUN_BATCH_BUTTON(tr("Kaydet"));
    SET_AUTO_EXIT_BATCH ( true );
    SET_ENTER_KEY_FOR_RUN_BATCH(true);

    REGISTER_BUTTON_WIDGET (m_ui->widget_batch_buttons);
    SET_TABLE_WIDGET_DEFAULTS();
    READ_INDEXES();
}

/**************************************************************************************
                   SQL_INDEX_BATCH::CHECK_VAR
***************************************************************************************/

int SQL_INDEX_BATCH::CHECK_VAR(QObject * object)
{
    Q_UNUSED (object);
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_INDEX_BATCH::CHECK_RUN
***************************************************************************************/

int SQL_INDEX_BATCH::CHECK_RUN()
{
    DB->START_TRANSACTION();
    if ( CHECK_TABLE_VALUES() EQ false ) {
        DB->CANCEL_TRANSACTION();
        return ADAK_FAIL;
    }
    return ADAK_OK;
}

/**************************************************************************************
                   SQL_INDEX_BATCH::RUN_BATCH
***************************************************************************************/

void SQL_INDEX_BATCH::RUN_BATCH()
{
    for ( int i = 0; i < NUMBER_OF_ROWS; i++ ) {
        for( int k = 0; k < NUMBER_OF_COMBOBOX; k++ ) {
            if (COMBOBOX_ARRAY[i][k]->currentIndex() EQ 0 ) {
                INDEXLER_ARRAY[NUMBER_OF_COMBOBOX + k + NUMBER_OF_COMBOBOX*i] = 0;
            }
            else {
                INDEXLER_ARRAY[NUMBER_OF_COMBOBOX + k + NUMBER_OF_COMBOBOX*i] =
                        GET_ALAN_ID(p_tablo_id, COMBOBOX_ARRAY[i][k]->currentText());
            }
        }
    }

    QString array_content = PACK_INT_ARRAY(INDEXLER_ARRAY,INDEXLER_ARRAY_SIZE);

    SQL_QUERY query (DB);

    query.PREPARE_UPDATE("sql_tablolar","tablo_id","indexler_array","tablo_id=:tablo_id");

    query.SET_VALUE     (":indexler_array",array_content);
    query.SET_VALUE     (":tablo_id"      ,p_tablo_id);

    query.UPDATE();

    DB->COMMIT_TRANSACTION();
}

/**************************************************************************************
                   SQL_INDEX_BATCH::SET_TABLE_WIDGET_DEFAULTS
***************************************************************************************/

void SQL_INDEX_BATCH::SET_TABLE_WIDGET_DEFAULTS()
{
    QHeaderView * header =  m_ui->tableWidget_indexler->horizontalHeader();
    header->setStretchLastSection(QHeaderView::Stretch);

    m_ui->tableWidget_indexler->setColumnWidth ( ALAN_ADI_1_COLUMN ,  220 );
    m_ui->tableWidget_indexler->setColumnWidth ( ALAN_ADI_2_COLUMN ,  220 );
    m_ui->tableWidget_indexler->setColumnWidth ( ALAN_ADI_3_COLUMN ,  220 );
    m_ui->tableWidget_indexler->setColumnWidth ( ALAN_ADI_4_COLUMN ,  220 );

    for ( int i = 0; i < NUMBER_OF_ROWS; i++ ) {

        m_ui->tableWidget_indexler->insertRow(i);

        QTableWidgetItem * new_item = new QTableWidgetItem(QString("%1").arg(INDEX_NAMES[i]));
        new_item->setFlags(Qt::ItemIsSelectable);
        m_ui->tableWidget_indexler->setItem( i, INDEX_ADI_COLUMN, new_item );

        QComboBox * alan_combobox1 = new QComboBox();
        QComboBox * alan_combobox2 = new QComboBox();
        QComboBox * alan_combobox3 = new QComboBox();
        QComboBox * alan_combobox4 = new QComboBox();

        alan_combobox1->addItem("");
        alan_combobox2->addItem("");
        alan_combobox3->addItem("");
        alan_combobox4->addItem("");

        m_ui->tableWidget_indexler->setCellWidget(i, ALAN_ADI_1_COLUMN, alan_combobox1 );
        m_ui->tableWidget_indexler->setCellWidget(i, ALAN_ADI_2_COLUMN, alan_combobox2 );
        m_ui->tableWidget_indexler->setCellWidget(i, ALAN_ADI_3_COLUMN, alan_combobox3 );
        m_ui->tableWidget_indexler->setCellWidget(i, ALAN_ADI_4_COLUMN, alan_combobox4 );

        COMBOBOX_ARRAY[i][0] = alan_combobox1;
        COMBOBOX_ARRAY[i][1] = alan_combobox2;
        COMBOBOX_ARRAY[i][2] = alan_combobox3;
        COMBOBOX_ARRAY[i][3] = alan_combobox4;

    }

    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT("sql_alanlar","alan_adi","tablo_id = :tablo_id");
    sql_query.SET_VALUE(":tablo_id", p_tablo_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    while( sql_query.NEXT() EQ true ) {
        for ( int i = 0; i < NUMBER_OF_ROWS; i++ ) {
            for (int k = 0; k < NUMBER_OF_COMBOBOX; k++ ) {
                COMBOBOX_ARRAY[i][k]->addItem(sql_query.VALUE(0).toString());
                COMBOBOX_ARRAY[i][k]->setCurrentIndex(0);
            }
        }
    }
}

/**************************************************************************************
                   SQL_INDEX_BATCH::READ_INDEXES
***************************************************************************************/

void SQL_INDEX_BATCH::READ_INDEXES()
{
    SQL_QUERY query (DB);

    query.PREPARE_SELECT("sql_tablolar","indexler_array","tablo_id=:tablo_id");
    query.SET_VALUE     (":tablo_id",p_tablo_id);

    if ( query.SELECT() EQ 0 ) {
        return;
    }

    query.NEXT();

    UNPACK_INT_ARRAY(query.VALUE(0).toString(),INDEXLER_ARRAY,INDEXLER_ARRAY_SIZE);

    int line_number = 1;
    for ( int i = 4; i < INDEXLER_ARRAY_SIZE; i++ ) {
        if ( INDEXLER_ARRAY[i] EQ 0 ) {
            if ( i+1 MOD 4 EQ 0 ) {
                line_number++;
            }
            continue;
        }
        COMBOBOX_ARRAY[line_number-1][i-(4*line_number)]->setCurrentIndex
                (COMBOBOX_ARRAY[line_number][i-(4*line_number)]->findText(GET_ALAN_ADI(p_tablo_id,INDEXLER_ARRAY[i])));

        if ( i+1 MOD 4 EQ 0 ) {
            line_number++;
        }
    }
}

/**************************************************************************************
                   SQL_INDEX_BATCH::CHECK_TABLE_VALUES
***************************************************************************************/

bool SQL_INDEX_BATCH::CHECK_TABLE_VALUES()
{
    for ( int i = 0; i < NUMBER_OF_ROWS; i++ ) {
        for ( int k = 0; k < NUMBER_OF_COMBOBOX; k++ ) {
            if ( COMBOBOX_ARRAY[i][k]->currentIndex() EQ 0 ) {
                for ( int l = k; l < NUMBER_OF_COMBOBOX; l++ ) {
                    if ( COMBOBOX_ARRAY[i][l]->currentIndex() NE 0 ) {
                        MSG_ERROR(tr("%n nolu index satırı hatalı","",i+1),NULL);
                        return false;
                    }
                }
            }
        }
    }
    return true;
}


