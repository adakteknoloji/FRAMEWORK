#ifndef YONETIM_UTILS_H
#define YONETIM_UTILS_H

class QString;

#include "adak_sql.h"

struct YONETIM_KULLANICI_VALUES_STRUCT {

    int     old_kullanici_id;
    int     new_kullanici_id;
    int     is_used;
};

QString     KULLANICI_BILGISI_OLUSTUR   ( int kullanici_id    );
QString     BILGISAYAR_BILGISI_OLUSTUR  ( int bilgisayar_id   );

int         KULLANICI_SAYISI            ();

void        BILGISAYARDA_KAYITLI_KUL_SAY_VE_KULLANICININ_KAYITLI_VERITABANI_SAY_GUNCELLE ();

#endif // YONETIM_UTILS_H
