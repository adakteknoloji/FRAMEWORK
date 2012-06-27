#include <QDialog>
#include <QMdiArea>
#include <QStringList>
#include <QPixmap>
#include "kernel_utils.h"
#include "adak_defines.h"

/**************************************************************************************
                   SET_MDI_WINDOW
***************************************************************************************/

void SET_MDI_WINDOW ( QMdiArea * mdi_area,QWidget * sub_window )
{
    mdi_area->addSubWindow ( sub_window );
    ( ( QDialog * )sub_window )->setModal ( false );
    sub_window->show();
    mdi_area->tileSubWindows();
}

/**************************************************************************************
                   SET_WINDOW_FLAGS
***************************************************************************************/

void SET_WINDOW_FLAGS ( QDialog * dialog )
{
    if ( dialog EQ NULL ) {
        return;
    }
    #ifdef ADAK_MACOSX
        dialog->setWindowFlags ( Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    #else
        dialog->setWindowModality ( Qt::WindowModal );
        dialog->setWindowFlags ( Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    #endif

}

/**************************************************************************************
                   CREATE_SPLASH_SCREEN
***************************************************************************************/

QSplashScreen * CREATE_SPLASH_SCREEN()
{
    QPixmap pixmap ( ":/kernel_icons/kernel_splash_background.png" );
    QSplashScreen * splash = new QSplashScreen ( pixmap );
    splash->setMaximumSize ( 500,200 );
    splash->setMinimumSize ( 500,200 );

    return splash;
}
