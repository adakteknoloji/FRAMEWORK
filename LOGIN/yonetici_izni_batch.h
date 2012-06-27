#ifndef YONETICI_IZNI_BATCH_H
#define YONETICI_IZNI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class YONETICI_IZNI_BATCH;
}

class YONETICI_IZNI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           YONETICI_IZNI_BATCH                            ( bool * onay_alindi, QWidget * parent = 0 );
                           ~YONETICI_IZNI_BATCH                           () {}
private:
    Ui::YONETICI_IZNI_BATCH *        m_ui;

    bool *                 m_onay_alindi;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

};

#endif // YONETICI_IZNI_BATCH_H
