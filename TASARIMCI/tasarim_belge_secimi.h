#ifndef TASARIM_BELGE_SECIMI_H
#define TASARIM_BELGE_SECIMI_H

#include <QDialog>
#include <QTableWidget>
#include "belge_struct.h"
#include "secim_kerneli.h"

class TASARIM_BELGE_SECIMI : public SECIM_KERNELI
{
public:
    TASARIM_BELGE_SECIMI(ADAK_BELGELER_STRUCT * belge_struct,int belge_count, QWidget * parent = 0 );
    ~TASARIM_BELGE_SECIMI() {}

    int *                   m_secilen_id;

private:
    int                     m_belge_count;
    ADAK_BELGELER_STRUCT * m_belge_struct;


    void                    FILL_TABLE              ();
    int                     SINGLE_LINE_SELECTED    (int selected_row_number);

};

#endif // TASARIM_BELGE_SECIMI_H


