#ifndef XYZ_FORMU_CLASS_H
#define XYZ_FORMU_CLASS_H

#include <form_kernel.h>
#include "ui_xyz_formu.h"

class XYZ_FORMU : public FORM_KERNEL , public Ui::XYZ_FORMU
{
    Q_OBJECT
public:
    XYZ_FORMU                                           ( QWidget * parent = 0 );
    ~XYZ_FORMU                                          (){}

private:

    void           SETUP_FORM                           ();
    void           NEW_RECORD                           ();
    void           CLEAR_FORM_MEMBERS			();
    int            GET_RECORD                           (int record_id);
    int            SELECT_RECORD                        ();
    int            FIND_RECORD                          ();

    int            CHECK_VAR                            (QObject * object);
    int            CHECK_EMPTY                          ();

    int            CHECK_ADD                            ();
    int            ADD_RECORD                           ();
    int            CHECK_UPDATE                         (int record_id);
    void           UPDATE_RECORD                        (int record_id);
    int            CHECK_DELETE                         (int record_id);
    void           DELETE_RECORD                        (int record_id);

    int            FIND_NEXT_RECORD                     ();
    int            FIND_PREV_RECORD                     ();
    int            FIND_FIRST_RECORD                    ();
    int            FIND_LAST_RECORD                     ();

    int            LOCK_RECORD                          (int record_id);
    void           UNLOCK_RECORD                        (int record_id);
    //virtual void   SAVER_BUTTON_CLICKED                 ( QAbstractButton * p_button, int p_record_id );
};

#endif // XYZ_FORMU_CLASS_H
