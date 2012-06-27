#ifndef SQL_VERITABANI_AYARLARI_BATCH_H
#define SQL_VERITABANI_AYARLARI_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class SQL_VERITABANI_AYARLARI_BATCH;
}

class SQL_VERITABANI_AYARLARI_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
                           SQL_VERITABANI_AYARLARI_BATCH                            ( QWidget * parent = 0 );
                           ~SQL_VERITABANI_AYARLARI_BATCH                           () {}
private:

    int                    m_program_id;

    Ui::SQL_VERITABANI_AYARLARI_BATCH *        m_ui;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();

    int                    CHECK_EXIT                           ();

    void                   RUN_BATCH                            ();

};

void OPEN_SQL_VERITABANI_AYARLARI_BATCH ( QWidget * parent );

#endif // SQL_VERITABANI_AYARLARI_BATCH_H
