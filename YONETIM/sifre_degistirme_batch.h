#ifndef SIFRE_DEGISTIRME_BATCH_H
#define SIFRE_DEGISTIRME_BATCH_H

#include <QDialog>
#include "batch_kernel.h"

namespace Ui {
    class SIFRE_DEGISTIRME_BATCH;
}

class SIFRE_DEGISTIRME_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           SIFRE_DEGISTIRME_BATCH                            ( int kullanici_id , QWidget * parent = 0 );
                           ~SIFRE_DEGISTIRME_BATCH                           () {};

private:

    int                    m_kullanici_id;

    Ui::SIFRE_DEGISTIRME_BATCH *   m_ui;

    void                   SETUP_FORM                                       ();
    int                    CHECK_VAR                                        (QObject * object);
    int                    CHECK_RUN                                        ();
    void                   RUN_BATCH                                        ();

};


#endif // SIFRE_DEGISTIRME_BATCH_H
