#include "adak_utils.h"

/******************************************************************************************/
/*                  GET_ADAK_MODULE_NAME                                */
/******************************************************************************************/

QString GET_ADAK_MODULE_NAME(int modul_id)
{
    switch ( modul_id ) {
        case MUHASEBE_MODULU :
            return "MUHASEBE MODÜLÜ";
        case ISLETME_MODULU  :
            return "İŞLETME MODÜLÜ";
        case DOVIZ_MODULU    :
            return "DÖVİZ MODÜLÜ";
        case ADRES_MODULU    :
            return "ADRES MODÜLÜ";

        case CARI_MODULU     :
            return "CARİ MODÜLÜ";
        case CEKSENET_MODULU :
            return "ÇEK-SENET MODÜLÜ";
        case BANKA_MODULU    :
            return "BANKA MODÜLÜ";
        case SUBE_MODULU    :
            return "ŞUBE MODÜLÜ";

        case STOK_MODULU     :
            return "STOK MODÜLÜ";
        case IRSALIYE_MODULU     :
            return "İRSALİYE MODÜLÜ";
        case FATURA_MODULU     :
            return "FATURA MODÜLÜ";

        case DEMIRBAS_MODULU :
            return "DEMİRBAŞ MODÜLÜ";
        case PERSONEL_MODULU :
            return "PERSONEL MODÜLÜ";
        case FAIZ_MODULU     :
            return "FAİZ MODÜLÜ";
        default              :
            return "Meçhul MODÜL";
    }
}











