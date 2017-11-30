#include <QtWidgets/QFileDialog>

#include "adak_exporter.h"

#include "batch_kernel.h"
#include "export_kernel.h"

/**************************************************************************************
                   EXPORT_KERNEL::EXPORT_KERNEL
***************************************************************************************/

EXPORT_KERNEL::EXPORT_KERNEL ( QString p_help_page, QWidget * parent ) :  BATCH_KERNEL ( parent )
{
    m_help_page = p_help_page;

    START_BATCH_KERNEL ( this, NULL );

}

/**************************************************************************************
                   EXPORT_KERNEL::SETUP_FORM
***************************************************************************************/

void EXPORT_KERNEL::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Transfer to OpenOffice.") );

    SET_PAGE_TITLE    (tr("EXPORT"));
    SET_SETTING_NAME  ("IHRAÇ_ET_BATCH");
    SET_HELP_PAGE     ( m_help_page );
}

/**************************************************************************************
                   EXPORT_KERNEL::CHECK_VAR
***************************************************************************************/

int EXPORT_KERNEL::CHECK_VAR ( QObject *  )
{
    return ADAK_OK;
}

/**************************************************************************************
                   EXPORT_KERNEL::CHECK_RUN
***************************************************************************************/

int EXPORT_KERNEL::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   EXPORT_KERNEL::RUN_BATCH
***************************************************************************************/

void EXPORT_KERNEL::RUN_BATCH ()
{
}

/**************************************************************************************
                   EXPORT_KERNEL::ADD_ROW
***************************************************************************************/

void EXPORT_KERNEL::ADD_ROW(QStringList rows)
{
    m_export_rows << rows ;
}

/**************************************************************************************
                   EXPORT_KERNEL::START_EXPORT
***************************************************************************************/

void EXPORT_KERNEL::START_EXPORT()
{
    QString file_path = QFileDialog::getSaveFileName(NULL, tr("Save"),"",tr("ODS File(*.ods)"));

    if ( file_path.isEmpty() EQ false ) {
        ADAK_CURSOR_BUSY();

        EXPORT_ROWS();

        ADAK_EXPORTER * adak_exporter = new ADAK_EXPORTER();

        adak_exporter->EXPORT( m_export_rows,file_path);

        ADAK_CURSOR_NORMAL();

        MSG_INFO(tr("Transferred Information."),NULL);
    }
}

/**************************************************************************************
                   EXPORT_KERNEL::SET_ROW_TITLE
***************************************************************************************/

void EXPORT_KERNEL::SET_ROW_TITLE( QStringList row_title )
{

   m_export_rows << row_title;
}
