#ifndef TASARIM_BELGE_BATCH_H
#define TASARIM_BELGE_BATCH_H

#include <QDialog>
#include "batch_kernel.h"
#include "belge_struct.h"
#include "tasarim_utils.h"

namespace Ui {
    class TASARIM_BELGE_BATCH;
}

class TASARIM_BELGE_BATCH : public BATCH_KERNEL
{
    Q_OBJECT
public:
    TASARIM_BELGE_BATCH ( int p_tasarim_id ,QWidget *parent = 0);
    ~ TASARIM_BELGE_BATCH(){}

protected:
    void                    SETUP_FORM                           ();
    void                    CLEAR_FORM_MEMBERS                   ();
    void                    NEW_RECORD                           ();
    int                     GET_RECORD                           (int record_id);

    int                     CHECK_VAR                            (QObject * object);
    int                     CHECK_RUN                          ();

    void                    RUN_BATCH                            ();

private:
    Ui::TASARIM_BELGE_BATCH  *m_ui;

    int m_belge_id;
    int m_printer_type;

private slots:
    void                    CREATE_CPP_CODE_USER_VARIABLES();

};

#endif // TASARIM_BELGE_BATCH_H
