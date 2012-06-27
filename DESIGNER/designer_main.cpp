#include <QtGui/QApplication>
#include <QTextCodec>

#include "designer_ana_menu.h"
#include "adak_utils.h"
#include "yonetim.h"

#include "designer_db.h"

#include "designer.h"

       GLOBAL_VARS   GV;
extern ADAK_SQL *   DB;
extern ADAK_SQL *   G_YONETIM_DB;

/**************************************************************************************
                   main
***************************************************************************************/

int main ( int argc, char *argv[] )
{
    QApplication a ( argc, argv );

    INIT_PROGRAM (&designer_db, NULL);
    if ( PROGRAMA_GIRIS ( argc, argv , ADAK_DESIGNER, ADAK_DESIGNER, NULL) EQ ADAK_OK ) {

        OPEN_DESIGNER_ANA_MENU();

        a.exec();
    }

    if ( DB NE NULL) {
        DB->~ADAK_SQL();
    }

    if ( G_YONETIM_DB NE NULL ) {
        G_YONETIM_DB->~ADAK_SQL();
    }

    // Programin main ' e donerek DATABASE kapatmasi en ideal cikis yontemi
    
    return 0;
}

