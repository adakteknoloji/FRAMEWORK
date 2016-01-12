#include "adak_utils.h"
#include <QObject>

/******************************************************************************************/
/*                  GET_ADAK_MODULE_NAME                                */
/******************************************************************************************/

QString GET_ADAK_MODULE_NAME(int modul_id)
{
    switch ( modul_id ) {
        case MUHASEBE_MODULU :
            return QObject::tr ("ACCOUNTING MODULE");
        case ISLETME_MODULU  :
            return QObject::tr ("BUSINESS MODULE");
        case DOVIZ_MODULU    :
            return QObject::tr ("CURRENCY MODULE");
        case ADRES_MODULU    :
            return QObject::tr ("ADDRESS MODULE");

        case CARI_MODULU     :
            return QObject::tr ("CURRENT MODULE");
        case CEKSENET_MODULU :
            return QObject::tr ("CHEQUE & BOND MODULE");
        case BANKA_MODULU    :
            return QObject::tr ("BANK MODULE");
        case SUBE_MODULU    :
            return QObject::tr ("SUBOFFICE MODULE");

        case STOK_MODULU     :
            return QObject::tr ("STOCK MODULE");
        case IRSALIYE_MODULU     :
            return QObject::tr ("DISPATCH NOTE MODULE");
        case FATURA_MODULU     :
            return QObject::tr ("BILL MODULE");

        case DEMIRBAS_MODULU :
            return QObject::tr ("FIXTURE MODULE");
        case PERSONEL_MODULU :
            return QObject::tr ("STAFF MODULE");
        case FAIZ_MODULU     :
            return QObject::tr ("INTEREST MODULE");
        default              :
            return QObject::tr ("UNKNOWN MODULE");
    }
}











