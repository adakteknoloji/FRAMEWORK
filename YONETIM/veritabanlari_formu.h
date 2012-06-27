#ifndef VERITABANLARI_FORMU_H
#define VERITABANLARI_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class VERITABANLARI_FORMU;
}

class VERITABANLARI_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
                        VERITABANLARI_FORMU                                 ( QWidget * p_parent = 0 );
                        ~VERITABANLARI_FORMU                                () {};

private :

    Ui::VERITABANLARI_FORMU   *    m_ui;

    int             p_secilen_kullanici_id;

    void            SETUP_FORM                                  ();
    void            CLEAR_FORM_MEMBERS                          ();
    void            NEW_RECORD                                  ();
    int             GET_RECORD                                  ( int p_veritabani_id );
    int             FIND_RECORD                                 ();
    int             SELECT_RECORD                               ();

    int             CHECK_VAR                                   ( QObject * object);
    int             CHECK_EMPTY                                 ();

    int             CHECK_ADD                                   ();
    int             ADD_RECORD                                  ();
    int             CHECK_UPDATE                                ( int p_veritabani_id );
    void            UPDATE_RECORD                               ( int p_veritabani_id );
    int             CHECK_DELETE                                ( int p_veritabani_id );
    void            DELETE_RECORD                               ( int p_veritabani_id );

    int             FIND_NEXT_RECORD                            ();
    int             FIND_PREV_RECORD                            ();
    int             FIND_FIRST_RECORD                           ();
    int             FIND_LAST_RECORD                            ();

    int             LOCK_RECORD                                 ( int p_veritabani_id );
    void            UNLOCK_RECORD                               ( int p_veritabani_id );
    int             CHANGER_BUTTON_CLICKED                      ( QAbstractButton * p_button );

    int             LISTEYE_KULLANICI_EKLE                      ();
    int             KULLANICILAR_LISTESINDEN_SATIR_SIL          ( int p_clicked_row, int p_clicked_column );

    void            MEVCUT_YARATILACAK_DB_TEXT_GUNCELLE         ( int veritabani_id = -1 );
};

#endif // VERITABANLARI_FORMU_H
