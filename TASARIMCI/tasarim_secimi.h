#ifndef TASARIM_SECIMI_H
#define TASARIM_SECIMI_H

#include <QDialog>
#include <QTableWidget>
#include "secim_kerneli.h"

class TASARIM_SECIMI : public SECIM_KERNELI
{
public:
    TASARIM_SECIMI(int * secilen_id,int belge_id,QWidget* parent = 0);
    ~TASARIM_SECIMI() {}

private:
    int                  m_belge_id;
    int *                m_secilen_id;

    void                 FILL_TABLE              ();
    int                  SINGLE_LINE_SELECTED    (int selected_row_number);
};

#endif // TASARIM_SECIMI_H


