#ifndef SQL_INDEX_BATCH_H
#define SQL_INDEX_BATCH_H

#include <QtGui/QDialog>
#include "batch_kernel.h"



namespace Ui {
    class SQL_INDEX_BATCH;
}

class SQL_INDEX_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:
    Ui::SQL_INDEX_BATCH *   m_ui;
    int                 p_tablo_id;

public:
                           SQL_INDEX_BATCH                            (int tablo_id, QWidget *parent = 0);
                           ~SQL_INDEX_BATCH                           () {}
private:

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();
    void                   SET_TABLE_WIDGET_DEFAULTS            ();
    void                   READ_INDEXES                         ();
    bool                   CHECK_TABLE_VALUES                   ();

};

#endif // SQL_INDEX_BATCH_H
