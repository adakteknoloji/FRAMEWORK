#ifndef TASARIM_BELGE_DEGISKENLERI_SECIMI_H
#define TASARIM_BELGE_DEGISKENLERI_SECIMI_H

#include "secim_kerneli.h"

class TASARIM_DEGISKENI;

class TASARIM_BELGE_DEGISKENLERI_SECIMI : public SECIM_KERNELI
{
public:
    TASARIM_BELGE_DEGISKENLERI_SECIMI ( QList< TASARIM_DEGISKENI*> p_degisken_listesi, QWidget *  parent = 0 );
    ~TASARIM_BELGE_DEGISKENLERI_SECIMI() {}

public:
    TASARIM_DEGISKENI *secilen_degisken;

private:
    QList< TASARIM_DEGISKENI*> m_degisken_listesi;

    void FILL_TABLE           ();
    int  SINGLE_LINE_SELECTED ( int selected_row_number );
    int  MULTI_LINE_SELECTED  (QList<int> selected_rows );


private slots:
    void    SLOT_DEGISKEN_KULLAN();
};

#endif // TASARIM_BELGE_DEGISKENLERI_SECIMI_H


