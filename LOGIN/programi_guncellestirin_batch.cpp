#include <QDesktopServices>
#include <QString>
#include <QSettings>
#include <QUrl>

#include "programi_guncellestirin_batch.h"
#include "ui_programi_guncellestirin_batch.h"

extern ADAK_SQL *           DB;

//Dikkat.Standart batch ekrani degildir. Run batch butonu yok.

/**************************************************************************************
                   OPEN_PROGRAMI_GUNCELLESTIRIN_BATCH
***************************************************************************************/

void OPEN_PROGRAMI_GUNCELLESTIRIN_BATCH ( QString url, QString new_version, QWidget *parent )
{
    PROGRAMI_GUNCELLESTIRIN_BATCH * F = new PROGRAMI_GUNCELLESTIRIN_BATCH ( url ,new_version, parent );
    F->EXEC (NOT_FULL_SCREEN);
}

/**************************************************************************************
                   PROGRAMI_GUNCELLESTIRIN_BATCH::PROGRAMI_GUNCELLESTIRIN_BATCH
***************************************************************************************/

PROGRAMI_GUNCELLESTIRIN_BATCH::PROGRAMI_GUNCELLESTIRIN_BATCH (QString url, QString new_version, QWidget *parent) :  BATCH_KERNEL ( parent ), m_ui ( new Ui::PROGRAMI_GUNCELLESTIRIN_BATCH )
{
    m_url             = url;
    m_new_version     = new_version;

    m_ui->setupUi      ( this );

    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   PROGRAMI_GUNCELLESTIRIN_BATCH::SETUP_FORM
***************************************************************************************/

void PROGRAMI_GUNCELLESTIRIN_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON (QObject::tr( "Update Programme" ));

    SET_PAGE_TITLE    (tr("New version - %1").arg(m_new_version));
    SET_SETTING_NAME  ("YENI_VERSIYON");
    SET_HELP_PAGE     ("guncellestirme.html");

}

/**************************************************************************************
                   PROGRAMI_GUNCELLESTIRIN_BATCH::CHECK_VAR
***************************************************************************************/

int PROGRAMI_GUNCELLESTIRIN_BATCH::CHECK_VAR ( QObject * obj )
{
    if ( obj EQ m_ui->push_button_devam_et ) {
        close();
    }
    else if ( obj EQ m_ui->push_button_guncelle ){
        RUN_BATCH();
    }

    return ADAK_OK;
}

/**************************************************************************************
                   PROGRAMI_GUNCELLESTIRIN_BATCH::CHECK_RUN
***************************************************************************************/

int PROGRAMI_GUNCELLESTIRIN_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   PROGRAMI_GUNCELLESTIRIN_BATCH::RUN_BATCH
***************************************************************************************/

void PROGRAMI_GUNCELLESTIRIN_BATCH::RUN_BATCH ()
{
    QDesktopServices::openUrl(QUrl( m_url ));
    exit(0);
}
