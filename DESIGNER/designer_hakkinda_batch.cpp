#include "designer_hakkinda_batch.h"
#include "designer.h"
#include "print.h"

#include "ui_designer_hakkinda_batch.h"

extern GLOBAL_VARS GV;

/**************************************************************************************
                   OPEN_DESIGNER_HAKKINDA
***************************************************************************************/

void OPEN_DESIGNER_HAKKINDA ( QWidget * parent )
{
    DESIGNER_HAKKINDA_BATCH * F = new DESIGNER_HAKKINDA_BATCH ( parent );
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   DESIGNER_HAKKINDA_BATCH::DESIGNER_HAKKINDA_BATCH
***************************************************************************************/

DESIGNER_HAKKINDA_BATCH::DESIGNER_HAKKINDA_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::DESIGNER_HAKKINDA_BATCH )
{
    m_ui->setupUi      ( this );
    START_BATCH_KERNEL (this, NULL );
}

/**************************************************************************************
                   DESIGNER_HAKKINDA_BATCH::SETUP_FORM
***************************************************************************************/

void DESIGNER_HAKKINDA_BATCH::SETUP_FORM()
{
    QString surum = QString (QObject::tr( "Version : ") ).append ( GV.versiyon );
    QFont font;
    font.setPointSize(10);
    m_ui->label_surum->setFont ( font );
    m_ui->label_surum->setStyleSheet ( QString::fromUtf8 ( "color: rgb(0, 0, 0);" ) );
    m_ui->label_surum->setFrameShape ( QFrame::StyledPanel );
    m_ui->label_surum->setTextFormat ( Qt::RichText );
    m_ui->label_surum->setText ( surum );

    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Okey") );
    SET_AUTO_EXIT_BATCH ( true );

    // Programa ilk girildiginde focuslanacak widget setlenmelidir.
    SET_FIRST_FOCUS_WIDGET ( m_ui->widget );

    // Butonumuzun eklenecegi widgeti register ediyoruz.
    REGISTER_BUTTON_WIDGET ( m_ui->widget );

    SET_PAGE_TITLE    (tr("ABOUT"));
    SET_SETTING_NAME  ("DESIGNER_HAKKINDA");
    SET_HELP_PAGE     ("hakkinda.html");
}

/**************************************************************************************
                   DESIGNER_HAKKINDA_BATCH::CHECK_VAR
***************************************************************************************/

int DESIGNER_HAKKINDA_BATCH::CHECK_VAR ( QObject *  )
{
    return ADAK_OK;
}

/**************************************************************************************
                   DESIGNER_HAKKINDA_BATCH::CHECK_RUN
***************************************************************************************/

int DESIGNER_HAKKINDA_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   DESIGNER_HAKKINDA_BATCH::RUN_BATCH
***************************************************************************************/

void DESIGNER_HAKKINDA_BATCH::RUN_BATCH ()
{
}

/**************************************************************************************
                   DESIGNER_HAKKINDA_BATCH::CHECK_EXIT
***************************************************************************************/

int DESIGNER_HAKKINDA_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}
