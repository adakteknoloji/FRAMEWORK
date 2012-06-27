#ifndef DESIGNER_HAKKINDA_BATCH_H
#define DESIGNER_HAKKINDA_BATCH_H

#include "batch_kernel.h"

namespace Ui {
    class DESIGNER_HAKKINDA_BATCH;
}

class DESIGNER_HAKKINDA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
                                    DESIGNER_HAKKINDA_BATCH             (QWidget * parent = 0);
                                    ~DESIGNER_HAKKINDA_BATCH            () {}
private:

    Ui::DESIGNER_HAKKINDA_BATCH *  m_ui;

    void                           SETUP_FORM                           ();
    int                            CHECK_VAR                            (QObject * object);
    int                            CHECK_RUN                            ();
    int                            CHECK_EXIT                           ();
    void                           RUN_BATCH                            ();

};

#endif // DESIGNER_HAKKINDA_BATCH_H
