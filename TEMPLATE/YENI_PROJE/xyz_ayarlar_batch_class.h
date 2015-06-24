#ifndef XYZ_AYARLAR_BATCH_CLASS_H
#define XYZ_AYARLAR_BATCH_CLASS_H

#include "batch_kernel.h"
#include "ui_xyz_ayarlar_batch.h"

class XYZ_AYARLAR_BATCH : public BATCH_KERNEL, public Ui::XYZ_AYARLAR_BATCH
{
    Q_OBJECT

public:
     XYZ_AYARLAR_BATCH                            ( QWidget * parent = 0 );
     ~XYZ_AYARLAR_BATCH                           () {}

private:
    void    		   SETUP_FORM                           ();
    int                CHECK_VAR                            ( QObject * object );
    int                CHECK_RUN                            ();
    int                CHECK_EXIT                           ();
    void               RUN_BATCH                            ();

};

#endif // XYZ_AYARLAR_BATCH_CLASS_H
