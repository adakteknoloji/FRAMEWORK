#ifndef XYZ_BATCH_CLASS_H
#define XYZ_BATCH_CLASS_H

#include "batch_kernel.h"
#include "ui_xyz_batxh.h"

namespace Ui {
    class XYZ_BATCH;
}

class XYZ_BATCH : public BATCH_KERNEL, public Ui::XYZ_BATCH
{
    Q_OBJECT


public:
                           XYZ_BATCH                            ( QWidget * parent = 0 );
                           ~XYZ_BATCH                           () {}
private:

    Ui::XYZ_BATCH *        m_ui;

    void    		   SETUP_FORM                           ();
    int                    CHECK_VAR                            ( QObject * object );
    int                    CHECK_RUN                            ();
    int                    CHECK_EXIT                           ();
    void                   RUN_BATCH                            ();

};

#endif // XYZ_BATCH_CLASS_H
