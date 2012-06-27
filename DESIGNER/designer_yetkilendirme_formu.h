#ifndef DESIGNER_YETKILENDIRME_FORMU_H
#define DESIGNER_YETKILENDIRME_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class DESIGNER_YETKILENDIRME_FORMU;
}

class DESIGNER_YETKILENDIRME_FORMU : public FORM_KERNEL
{
    Q_OBJECT


public:

                    DESIGNER_YETKILENDIRME_FORMU                 ( int p_kullanici_id,bool * yetkileri_tazele, QWidget * p_parent = 0 );
                    ~DESIGNER_YETKILENDIRME_FORMU                () {}


private:

    Ui::DESIGNER_YETKILENDIRME_FORMU  *       m_ui;
    int            p_kullanici_id;
    int            m_start_record_id;
    
    bool *         m_yetkileri_tazele;

    void           SETUP_FORM                           ();
    void           CLEAR_FORM_MEMBERS                   ();
    void           NEW_RECORD                           ();
    int            GET_RECORD                           ( int yetki_id );
    int            FIND_RECORD                          ();
    int            SELECT_RECORD                        ();

    int            CHECK_VAR                            ( QObject * object );
    int            CHECK_EMPTY                          ();

    int            CHECK_ADD                            ();
    int            ADD_RECORD                           ();
    int            CHECK_UPDATE                         ( int p_kullanici_id );
    void           UPDATE_RECORD                        ( int p_kullanici_id );
    int            CHECK_DELETE                         ( int p_kullanici_id );
    void           DELETE_RECORD                        ( int p_kullanici_id );

    int            FIND_NEXT_RECORD                     ();
    int            FIND_PREV_RECORD                     ();
    int            FIND_FIRST_RECORD                    ();
    int            FIND_LAST_RECORD                     ();

    int            LOCK_RECORD                          ( int p_kullanici_id  );
    void           UNLOCK_RECORD                        ( int p_kullanici_id  );

    void           CHANGE_ALL_COMBOBOX_STATUS           ( int status );




};

#endif // DESIGNER_YETKILENDIRME_FORMU_H
