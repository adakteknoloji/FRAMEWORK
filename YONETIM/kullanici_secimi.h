#ifndef KULLANICI_SECIMI_H
#define KULLANICI_SECIMI_H

#include <QtGui/QDialog>
#include <QtGui/QTableWidget>
#include "secim_kerneli.h"

class KULLANICI_SECIMI : public SECIM_KERNELI
{
public:
                   KULLANICI_SECIMI            (bool yonetici_goster, QWidget * parent = 0, QStringList * P_VAROLAN_KULLANICI_ID_LIST = NULL );
                   ~KULLANICI_SECIMI           ();

private:
    bool                 m_yonetici_goster;

    QStringList   *      M_VAROLAN_KULLANICI_ID_LIST;

    void                 FILL_TABLE                                     ();
    int                  SINGLE_LINE_SELECTED                           ( int           current_row    );
    bool                 KULLANICI_ID_VAROLAN_KUL_LISTESINDE_VAR_MI     ( int           p_kullanici_id );

public:
    int *                m_secilen_id;
};

#endif // KULLANICI_SECIMI_H


