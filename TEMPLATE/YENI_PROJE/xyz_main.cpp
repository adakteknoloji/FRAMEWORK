#include <QtGui/QApplication>
#include <QIcon>
#include "xyz.h"
#include "adak_defines.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "xyz_ana_menu_open.h"
#include "xyz_db.h"

extern ADAK_SQL *       G_YONETIM_DB;
extern ADAK_SQL *       DB;

static void UPGRADE_TO_VERSION (QString version);

/**************************************************************************************
                   main
***************************************************************************************/

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/xyz_icons/xyz_logo.png")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);

    INIT_PROGRAM( &xyz_db, NULL);
    if ( PROGRAMA_GIRIS ( argc, argv, XYZ_PROGRAMI , XYZ_PROGRAMI, NULL ) EQ ADAK_OK ) {
        CHECK_VERSION_UPGRADES ("program_version","xyz_ayarlar",&UPGRADE_TO_VERSION);
        OPEN_XYZ_ANA_MENU (NULL);
        a.exec();
    }

    if ( DB NE NULL ) {
        DB->~ADAK_SQL();
    }

    if ( G_YONETIM_DB NE NULL ) {
        G_YONETIM_DB->~ADAK_SQL();
    }

    // Programin main ' e donerek DATABASE kapatmasi en ideal cikis yontemi

    return 0;
}

/**************************************************************************************
                   UPGRADE_TO_VERSION
***************************************************************************************/

static void UPGRADE_TO_VERSION (QString version)
{

    if ( version < "0.99" ) {
         // ADD_DEFAULT_BELGELER();
    }
}
