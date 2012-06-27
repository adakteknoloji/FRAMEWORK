#ifndef HATA_ISTEK_BATCH_H
#define HATA_ISTEK_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class HATA_ISTEK_BATCH;
}

class HATA_ISTEK_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           HATA_ISTEK_BATCH                            ( QWidget * parent = 0 );
                           ~HATA_ISTEK_BATCH                           () {}
private:

    Ui::HATA_ISTEK_BATCH * m_ui;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();

    void                   RUN_BATCH                            ();

};


#endif // HATA_ISTEK_BATCH_H
