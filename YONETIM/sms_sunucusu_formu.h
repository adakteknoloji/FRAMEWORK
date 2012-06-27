#ifndef SMS_SUNUCUSU_FORMU_H
#define SMS_SUNUCUSU_FORMU_H

#include <QtGui/QDialog>
#include <form_kernel.h>

namespace Ui {
    class SMS_SUNUCUSU_FORMU;
}

class SMS_SUNUCUSU_FORMU : public FORM_KERNEL
{
    Q_OBJECT

public:
                    SMS_SUNUCUSU_FORMU                       (QWidget *parent = 0);
                    ~SMS_SUNUCUSU_FORMU                      (){};
protected:

     void           SETUP_FORM                            ();
     void           CLEAR_FORM_MEMBERS                    ();
     void           NEW_RECORD                            ();
     int            GET_RECORD                            (int p_kullanici_id);
     int            FIND_RECORD                           ();
     int            SELECT_RECORD                         ();


     int            CHECK_VAR                             (QObject * object);
     int            CHECK_EMPTY                           ();

     int            CHECK_ADD                             ();
     int            ADD_RECORD                            ();
     int            CHECK_UPDATE                          (int p_kullanici_id);
     void           UPDATE_RECORD                         (int p_kullanici_id);
     int            CHECK_DELETE                          (int p_kullanici_id);
     void           DELETE_RECORD                         (int p_kullanici_id);

     int            FIND_NEXT_RECORD                      ();
     int            FIND_PREV_RECORD                      ();
     int            FIND_FIRST_RECORD                     ();
     int            FIND_LAST_RECORD                      ();

     int            LOCK_RECORD                           (int p_kullanici_id);
     void           UNLOCK_RECORD                         (int p_kullanici_id);

private:
         Ui::SMS_SUNUCUSU_FORMU   *    m_ui;

};

#endif // SMS_SUNUCUSU_FORMU_H
