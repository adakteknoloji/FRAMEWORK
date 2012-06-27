#ifndef BELGE_SECIMI_H
#define BELGE_SECIMI_H

#include <QDialog>
#include <QTableWidget>

#include "secim_kerneli.h"

class BELGE_SECIMI : public SECIM_KERNELI
{
public:
    BELGE_SECIMI(int * secilen_id , QWidget* parent = 0);
    ~BELGE_SECIMI() {}

private:
    int *                m_secilen_id;

    void                 FILL_TABLE              ();
    int                  SINGLE_LINE_SELECTED    (int selected_row_number);
};

#endif // BELGE_SECIMI_H


