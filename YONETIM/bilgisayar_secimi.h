#ifndef BILGISAYAR_SECIMI_H
#define BILGISAYAR_SECIMI_H

#include <QtGui/QDialog>
#include <QtGui/QTableWidget>
#include "secim_kerneli.h"


class BILGISAYAR_SECIMI : public SECIM_KERNELI
{
public:
                     BILGISAYAR_SECIMI              ( QWidget * parent = 0 );
                    ~BILGISAYAR_SECIMI              ();

private:
    void            FILL_TABLE                      ();
    int             SINGLE_LINE_SELECTED            (int current_row);

public :
    int *           m_secilen_id;
};

#endif // BILGISAYAR_SECIMI_H
