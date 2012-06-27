#ifndef VERITABANI_SECIMI_H
#define VERITABANI_SECIMI_H

#include "secim_kerneli.h"

class VERITABANI_SECIMI : public SECIM_KERNELI
{
public:
                      VERITABANI_SECIMI             ( int kullanici_id , QString * veritabani_ismi , QString * veritabani_tanimi, QWidget* parent = 0 );
                     ~VERITABANI_SECIMI             ();

private:
    int                  m_kullanici_id;

    QString *            m_veritabani_ismi;
    QString *            m_veritabani_tanimi;

    void                 FILL_TABLE                 ();
    int                  SINGLE_LINE_SELECTED       ( int current_row );

public:
    int *                m_secilen_id;
};

#endif // VERITABANI_SECIMI_H


