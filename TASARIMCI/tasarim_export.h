#ifndef TASARIM_EXPORT_H
#define TASARIM_EXPORT_H

#include "export_kernel.h"

class TASARIM_EXPORT : public EXPORT_KERNEL
{
public:
    TASARIM_EXPORT( int p_tasarim_id, QWidget * parent );

private:

    int  m_tasarim_id;

    void EXPORT_ROWS();
};

void OPEN_TASARIM_EXPORT( int p_tasarim_id, QWidget * parent );

#endif // TASARIM_EXPORT_H
