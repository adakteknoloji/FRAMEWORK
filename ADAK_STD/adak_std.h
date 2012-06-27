#ifndef ADAK_STD_H
#define ADAK_STD_H

#include <adak_sql.h>


class ADAK_STD
{
public:
    ADAK_STD();

    bool ULKELERI_EKLE();
    bool ILLERI_EKLE();
    bool ILCELERI_EKLE();
    bool GSM_KODLARINI_EKLE();
    bool OGRENIM_DURUMLARINI_EKLE();
    bool DOVIZLERI_EKLE();
    bool CINSIYETLERI_EKLE();
    bool VERGI_DAIRELERINI_EKLE();
    bool BANKALARI_EKLE();
    bool BANKA_SUBELERINI_EKLE();
    bool MEDENI_HALLERI_EKLE();
    bool KAN_GRUPLARINI_EKLE();
    bool SEKTORLERI_EKLE();
    bool MESLEKLERI_EKLE();
    bool UNVANLARI_EKLE();

    QStringList il_adlari;
    QList<int>  il_idleri;

};

void ADD_ADAK_STD();

#endif // ADAK_STD_H
