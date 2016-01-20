#include "tc.h"
#include "xyz_ayarlar_batch_open.h"
#include "xyz_ayarlar_batch_class.h"
#include "ui_xyz_ayarlar_batch.h"
#include "xyz_utils.h"

extern ADAK_SQL *           DB;
extern XYZ_AYARLAR_STRUCT    XYZ_AYARLAR;

/**************************************************************************************
                   OPEN_XYZ_AYARLAR_BATCH
***************************************************************************************/

void OPEN_XYZ_AYARLAR_BATCH ( QWidget * parent )
{
    XYZ_AYARLAR_BATCH * F = new XYZ_AYARLAR_BATCH ( parent );
    F->EXEC(NOT_FULL_SCREEN);
}

/**************************************************************************************
                   XYZ_AYARLAR_BATCH::XYZ_AYARLAR_BATCH
***************************************************************************************/

XYZ_AYARLAR_BATCH::XYZ_AYARLAR_BATCH ( QWidget * parent ) :  BATCH_KERNEL ( parent )
{
    setupUi      ( this );
    START_BATCH_KERNEL ( this, DB );
}

/**************************************************************************************
                   XYZ_AYARLAR_BATCH::SETUP_FORM
***************************************************************************************/

void XYZ_AYARLAR_BATCH::SETUP_FORM ()
{
    SET_NAME_OF_RUN_BATCH_BUTTON ( tr("Save") );
    SET_FIRST_FOCUS_WIDGET ( adakDate_mali_yil_baslangic_tarihi );
    REGISTER_BUTTON_WIDGET ( widget_batch_buttons );

    SET_PAGE_TITLE    (tr("SETTING SCREEN"));
    SET_SETTING_NAME  ("XYZ_AYARLAR_BATCH");
    SET_HELP_PAGE     ("xyz_ayarlar.html");

    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("xyz_ayarlar",
                         "mali_yil_baslangic_tarihi"
                         ",mali_yil_bitis_tarihi",
                         "ayarlar_id = :ayarlar_id");

    query.SET_VALUE( ":ayarlar_id", 1 );

    query.SELECT();
    query.NEXT();
    adakDate_mali_yil_baslangic_tarihi->MALI_YIL_KISITI_YOK(true);
    adakDate_mali_yil_bitis_tarihi->MALI_YIL_KISITI_YOK(true);

    adakDate_mali_yil_baslangic_tarihi->SET_DATE(query.VALUE(0).toString());
    adakDate_mali_yil_bitis_tarihi->SET_DATE(query.VALUE(1).toString());
}

/**************************************************************************************
                   XYZ_AYARLAR_BATCH::CHECK_VAR
***************************************************************************************/

int XYZ_AYARLAR_BATCH::CHECK_VAR ( QObject * object )
{
    Q_UNUSED (object)
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_AYARLAR_BATCH::CHECK_RUN
***************************************************************************************/

int XYZ_AYARLAR_BATCH::CHECK_RUN ()
{
    return ADAK_OK;
}

/**************************************************************************************
                   XYZ_AYARLAR_BATCH::RUN_BATCH
***************************************************************************************/

void XYZ_AYARLAR_BATCH::RUN_BATCH ()
{
    XYZ_AYARLAR_STRUCTINI_DOLDUR ();
}

/**************************************************************************************
                   XYZ_AYARLAR_BATCH::CHECK_EXIT
***************************************************************************************/

int XYZ_AYARLAR_BATCH::CHECK_EXIT ()
{
    return ADAK_OK;
}
