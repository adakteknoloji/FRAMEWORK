#include "xyz.h"
#include "xyz_ana_menu_class.h"
#include "xyz_ana_menu_open.h"
#include "ui_xyz_ana_menu.h"
#include "adak_sql.h"
#include "kernel_utils.h"


extern ADAK_SQL * DB;

enum {
    MENU_DOSYA                    = 10,
        MENU_CIKIS                = 11,

    MENU_YARDIM                   = 20,
        MENU_HAKKINDA             = 21,
};

/**************************************************************************************
                   OPEN_XYZ_ANA_MENU
****************************************************************************************/

void OPEN_XYZ_ANA_MENU ( QWidget * p_parent )
{
    XYZ_ANA_MENU * F = new XYZ_ANA_MENU ( p_parent );
    F->setWindowTitle ( QObject::tr ("XYZ" ) );
    F->show();
    F->setWindowState ( Qt::WindowMaximized );
}

/**************************************************************************************
                   XYZ_ANA_MENU::XYZ_ANA_MENU
***************************************************************************************/

XYZ_ANA_MENU::XYZ_ANA_MENU ( QWidget * p_parent ) : MENU_KERNEL ( p_parent ), ui ( new Ui::XYZ_ANA_MENU )
{
    ui->setupUi ( this );
    START_KERNEL();
}

/**************************************************************************************
                   XYZ_ANA_MENU::~XYZ_ANA_MENU
***************************************************************************************/

XYZ_ANA_MENU::~XYZ_ANA_MENU ( )
{
}

/***************************************************************************************
                   XYZ_ANA_MENU::SETUP_FORM
****************************************************************************************/

void XYZ_ANA_MENU::SETUP_FORM()
{
    struct MENU_STRUCT MENULER[] = {

        { MENU_DOSYA,                tr ("File")            , 0,         "" , "" , 0, "", 0, MENU_TOPMENUDE }  ,
            { MENU_CIKIS,            tr ( "Exit" )          , MENU_DOSYA,"" , "" , 0, "", 0, MENU_TOPMENUDE},

        { MENU_YARDIM,               tr ("Help")           , 0,         "" , "" , 0, "", 0, MENU_TOPMENUDE },
            {MENU_HAKKINDA,          tr ( "About" )       , MENU_YARDIM,"", "" , 0, "", 0, MENU_TOPMENUDE },

        { 0       , ""   , 0, "","",0,"",0,MENU_SAYFADA }
    };

    SET_KERNEL_STRUCT                       ( MENULER );
    REGISTER_MENU_WIDGET                    ( ui->main_widget );

    SHOW_MENU_ITEM                          ( MENU_CIKIS      );
    SHOW_MENU_ITEM                          ( MENU_HAKKINDA   );

}

/**************************************************************************************
                   XYZ_ANA_MENU::KULLANICI_YETKILERINI_AYARLA
****************************************************************************************/

void XYZ_ANA_MENU::KULLANICI_YETKILERINI_AYARLA ()
{

}

/**************************************************************************************
                   XYZ_ANA_MENU::MENU_ITEM_CLICKED
****************************************************************************************/

void XYZ_ANA_MENU::MENU_ITEM_CLICKED ( int p_enum_id )
{
    switch ( p_enum_id ) {
        case MENU_CIKIS :
            close ();
        default :
            break;
    }
}


