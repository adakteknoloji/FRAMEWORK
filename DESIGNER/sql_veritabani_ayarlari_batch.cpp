#include "adak_utils.h"
#include "yonetim.h"

#include "sql_veritabani_ayarlari_batch.h"
#include "ui_sql_veritabani_ayarlari_batch.h"

extern ADAK_SQL *           DB;

/**************************************************************************************
                   OPEN_SQL_VERITABANI_AYARLARI_BATCH
***************************************************************************************/

void OPEN_SQL_VERITABANI_AYARLARI_BATCH ( QWidget * parent )
{
    SQL_VERITABANI_AYARLARI_BATCH * F = new SQL_VERITABANI_AYARLARI_BATCH ( parent );
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   SQL_VERITABANI_AYARLARI_BATCH::SQL_VERITABANI_AYARLARI_BATCH
***************************************************************************************/

SQL_VERITABANI_AYARLARI_BATCH::SQL_VERITABANI_AYARLARI_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::SQL_VERITABANI_AYARLARI_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   SQL_VERITABANI_AYARLARI_BATCH::SETUP_FORM
***************************************************************************************/

void SQL_VERITABANI_AYARLARI_BATCH::SETUP_FORM ()
{
    SET_AUTO_EXIT_BATCH ( true );
    SET_NAME_OF_RUN_BATCH_BUTTON ( "Kaydet" );

    SET_FIRST_FOCUS_WIDGET ( m_ui->push_button_program_degistir );

    REGISTER_BUTTON_WIDGET ( m_ui->widget );

    SET_PAGE_TITLE    (tr("VERİTABANI AYARLARI - %1 / %2").arg(VERITABANI_ISMI()).arg(VERITABANI_TANIMI()));
    SET_SETTING_NAME  ("VERITABANI_AYARLARI");
    SET_HELP_PAGE     ("veritabani_ayarlari.html");

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("sql_db_info","program_id");

    if ( query.SELECT() EQ 0 ) {

        DB->START_TRANSACTION();

        query.PREPARE_INSERT("sql_db_info","db_info_id","program_id");
        query.SET_VALUE     (":program_id",-1);
        query.INSERT();

        DB->COMMIT_TRANSACTION();

        m_program_id = -1;
        m_ui->label_adi->setText("Program Şeçili Değil");
        return;
    }

    query.NEXT();

    m_program_id = query.VALUE(0).toInt();

    m_ui->label_adi->setText(ADAK_PROGRAM_LONGNAME(m_program_id));
}

/**************************************************************************************
                   SQL_VERITABANI_AYARLARI_BATCH::CHECK_VAR
***************************************************************************************/

int SQL_VERITABANI_AYARLARI_BATCH::CHECK_VAR ( QObject * object )
{
    if ( object EQ m_ui->push_button_program_degistir ) {
        int program_id = PROGRAM_SEC(this);

        if ( program_id NE -1 ) {
            m_program_id = program_id;
            m_ui->label_adi->setText(ADAK_PROGRAM_LONGNAME(m_program_id));
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQL_VERITABANI_AYARLARI_BATCH::CHECK_RUN
***************************************************************************************/

int SQL_VERITABANI_AYARLARI_BATCH::CHECK_RUN ()
{
    if ( m_program_id EQ -1 ) {
        MSG_ERROR("Bir program seçilmesi gereklidir.",m_ui->push_button_program_degistir);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

/**************************************************************************************
                   SQL_VERITABANI_AYARLARI_BATCH::RUN_BATCH
***************************************************************************************/

void SQL_VERITABANI_AYARLARI_BATCH::RUN_BATCH ()
{
    SQL_QUERY query (DB);

    DB->START_TRANSACTION();

    query.PREPARE_UPDATE("sql_db_info","db_info_id","program_id","");
    query.SET_VALUE     (":program_id",m_program_id);
    query.UPDATE();

    DB->COMMIT_TRANSACTION();
}

/**************************************************************************************
                   SQL_VERITABANI_AYARLARI_BATCH::CHECK_EXIT
***************************************************************************************/

int SQL_VERITABANI_AYARLARI_BATCH::CHECK_EXIT ()
{
    if ( m_program_id EQ -1 ) {
        MSG_ERROR("Bir program seçilmesi gereklidir.",m_ui->push_button_program_degistir);
        return ADAK_FAIL;
    }

    return ADAK_OK;
}

