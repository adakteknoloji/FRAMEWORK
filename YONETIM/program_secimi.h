#ifndef PROGRAM_SECIMI_H
#define PROGRAM_SECIMI_H

#include <QtGui/QDialog>
#include <QtGui/QTableWidget>
#include "secim_kerneli.h"


class PROGRAM_SECIMI : public SECIM_KERNELI
{
public:
                      PROGRAM_SECIMI                ( bool check_exe ,QWidget * parent = 0 );
                     ~PROGRAM_SECIMI                ();

    int *             m_secilen_id;

private:
    bool              m_check_exe;

    void              FILL_TABLE ();
    int               SINGLE_LINE_SELECTED          (int current_row);

};

#endif // PROGRAM_SECIMI_H


