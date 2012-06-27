#ifndef TASARIM_IMPORT_H
#define TASARIM_IMPORT_H

#include "import_kernel.h"

class TASARIM_IMPORT : public IMPORT_KERNEL
{
public:
    TASARIM_IMPORT( int p_tasarim_id, QWidget * parent = 0 );

private:
    int m_tasarim_id;

private:
    void IMPORT_ROW(QStringList row );
};

void OPEN_TASARIM_IMPORT(int p_tasarim_id, QWidget *parent);
#endif // TASARIM_IMPORT_H
