#ifndef TASARIM_YENI_TASARIM_BATCH_H
#define TASARIM_YENI_TASARIM_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class TASARIM_YENI_TASARIM_BATCH;
}

class TASARIM_YENI_TASARIM_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           TASARIM_YENI_TASARIM_BATCH                            ( int p_belge_id, QWidget * parent = 0 );
                           ~TASARIM_YENI_TASARIM_BATCH                           () {}
private:

    Ui::TASARIM_YENI_TASARIM_BATCH *        m_ui;

    int                    m_belge_id;
    QStringList            m_copy_belge_id_list;


    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

};

void OPEN_TASARIM_YENI_TASARIM_BATCH( int p_belge_id, QWidget * parent );

#endif // TASARIM_YENI_TASARIM_BATCH_H
