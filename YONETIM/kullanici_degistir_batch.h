#ifndef KULLANICI_DEGISTIR_BATCH_H
#define KULLANICI_DEGISTIR_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class KULLANICI_DEGISTIR_BATCH;
}

class KULLANICI_DEGISTIR_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public :
                           KULLANICI_DEGISTIR_BATCH                 ( int * kullanici_id , QWidget * p_parent = 0 );
                           ~KULLANICI_DEGISTIR_BATCH                () {}
private :



    void                             SETUP_FORM                    ();
    Ui::KULLANICI_DEGISTIR_BATCH *   m_ui;

    int                              m_kullanici_id;
    int *                            m_kullanici_id_ptr;

    int                              CHECK_VAR                     ( QObject * p_object );
    int                              CHECK_RUN                     ();
    void                             RUN_BATCH                     ();

};

#endif // KULLANICI_DEGISTIR_BATCH_H
