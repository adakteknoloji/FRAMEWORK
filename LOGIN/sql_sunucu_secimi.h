#ifndef SQL_SUNUCU_SECIMI_H
#define SQL_SUNUCU_SECIMI_H

#include "secim_kerneli.h"

class SQL_SUNUCU_SECIMI : public SECIM_KERNELI
{
public:
    SQL_SUNUCU_SECIMI                                ( QString * secilen_id , QWidget *  parent = 0 );
    ~SQL_SUNUCU_SECIMI                               () {}

private:
    QString     *        m_secilen_sunucu;

    void                 FILL_TABLE           ();

    int                  SINGLE_LINE_SELECTED ( int selected_row_number );
};

QString SQL_SUNUCU_SEC ( QWidget * parent );

#endif // SQL_SUNUCU_SECIMI_H


