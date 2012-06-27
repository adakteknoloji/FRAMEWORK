#ifndef PROGRAMI_GUNCELLESTIRIN_BATCH_H
#define PROGRAMI_GUNCELLESTIRIN_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class PROGRAMI_GUNCELLESTIRIN_BATCH;
}

class PROGRAMI_GUNCELLESTIRIN_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           PROGRAMI_GUNCELLESTIRIN_BATCH                            ( QString url,QString new_version,QWidget * parent = 0 );
                           ~PROGRAMI_GUNCELLESTIRIN_BATCH                           () {}
private:
    Ui::PROGRAMI_GUNCELLESTIRIN_BATCH *        m_ui;

    QString                m_url;
    QString                m_new_version;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();    
};

void OPEN_PROGRAMI_GUNCELLESTIRIN_BATCH (  QString url,QString new_version,QWidget * parent );

#endif // PROGRAMI_GUNCELLESTIRIN_BATCH_H
