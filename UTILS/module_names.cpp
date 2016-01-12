#include "adak_utils.h"
#include <QObject>

/******************************************************************************************/
/*                  GET_ADAK_MODULE_NAME                                */
/******************************************************************************************/

QString GET_ADAK_MODULE_NAME(int modul_id)
{
    switch ( modul_id ) {
        case MUHASEBE_MODULU :
            return QObject::tr ("MUHASEBE MODÜLÜ");
        case ISLETME_MODULU  :
            return QObject::tr ("İŞLETME MODÜLÜ");
        case DOVIZ_MODULU    :
            return QObject::tr ("DÖVİZ MODÜLÜ");
        case ADRES_MODULU    :
            return QObject::tr ("ADRES MODÜLÜ");

        case CARI_MODULU     :
            return QObject::tr ("CARİ MODÜLÜ");
        case CEKSENET_MODULU :
            return QObject::tr ("ÇEK-SENET MODÜLÜ");
        case BANKA_MODULU    :
            return QObject::tr ("BANKA MODÜLÜ");
        case SUBE_MODULU    :
            return QObject::tr ("ŞUBE MODÜLÜ");

        case STOK_MODULU     :
            return QObject::tr ("STOK MODÜLÜ");
        case IRSALIYE_MODULU     :
            return QObject::tr ("İRSALİYE MODÜLÜ");
        case FATURA_MODULU     :
            return QObject::tr ("FATURA MODÜLÜ");

        case DEMIRBAS_MODULU :
            return QObject::tr ("DEMİRBAŞ MODÜLÜ");
        case PERSONEL_MODULU :
            return QObject::tr ("PERSONEL MODÜLÜ");
        case FAIZ_MODULU     :
            return QObject::tr ("FAİZ MODÜLÜ");
        default              :
            return QObject::tr ("Meçhul MODÜL");
    }
}











