#include "tc.h"
#include "xyz_utils.h"
#include "adak_sql.h"
#include "adak_utils.h"
#include "QDate"

extern ADAK_SQL *           DB;
extern XYZ_AYARLAR_STRUCT   XYZ_AYARLAR;

/**************************************************************************************
                   XYZ_AYARLAR_STRUCTINI_DOLDUR
***************************************************************************************/

bool XYZ_AYARLAR_STRUCTINI_DOLDUR ()
{
    SQL_QUERY query ( DB );

    query.PREPARE_SELECT("xyz_ayarlar",
                         "mali_yil_baslangic_tarihi"
                         ",mali_yil_bitis_tarihi",
                         "ayarlar_id = :ayarlar_id");

    query.SET_VALUE( ":ayarlar_id", 1 );

    if ( query.SELECT() > 0 ) {
        query.NEXT();

        XYZ_AYARLAR.mali_yil_baslangic_tarihi = QDate::fromString ( query.VALUE(0).toString(), "yyyy.MM.dd" );
        XYZ_AYARLAR.mali_yil_bitis_tarihi    = QDate::fromString ( query.VALUE(1).toString(), "yyyy.MM.dd" );

        return true;
    }

    DB->START_TRANSACTION();
    query.PREPARE_INSERT ("xyz_ayarlar",
                          "ayarlar_id",
                          "mali_yil_baslangic_tarihi"
                          ",mali_yil_bitis_tarihi");
    query.SET_VALUE( ":mali_yil_baslangic_tarihi", QString::number(QDate::currentDate().year())+ ".01.01");
    query.SET_VALUE( ":mali_yil_bitis_tarihi"    , QString::number(QDate::currentDate().year())+ ".12.31");
    int id = query.INSERT();
    DB->COMMIT_TRANSACTION();
    if (id > 1) {
        return false; // Id 1 silindiyse sonsuz döngüde kalıyor.
    }
    return XYZ_AYARLAR_STRUCTINI_DOLDUR();
}
