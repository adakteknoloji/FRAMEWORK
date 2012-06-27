#ifndef KULLANICILAR_FORMU_H
#define KULLANICILAR_FORMU_H

#include <form_kernel.h>

namespace Ui {
    class KULLANICILAR_FORMU;
}

class KULLANICILAR_FORMU : public FORM_KERNEL
{
    Q_OBJECT

    public:
                                        KULLANICILAR_FORMU                       ( void    (*F_PROGRAM_KULLANICI_YETKILERI_FONK) ( int,bool *, QWidget * ),bool * yetkileri_tazele, QWidget * p_parent = 0 );
                                        ~KULLANICILAR_FORMU                      () {};
    private :

         Ui::KULLANICILAR_FORMU    *    m_ui;
         int                            m_secilen_bilgisayar_id;
         int                            m_secilen_veritabani_id;
         QString                        m_kullanici_parolasi;

         bool                           * m_yetkileri_tazele;

         void                          (*M_KULLANICI_YETKILERI_FONK)          ( int,bool *, QWidget * );


         void                           SETUP_FORM                            ();
         void                           CLEAR_FORM_MEMBERS                    ();
         void                           NEW_RECORD                            ();
         int                            GET_RECORD                            ( int p_kullanici_id );
         int                            FIND_RECORD                           ();
         int                            SELECT_RECORD                         ();


         int                            CHECK_VAR                             ( QObject * p_object );
         int                            CHECK_EMPTY                           ();

         int                            CHECK_ADD                             ();
         int                            ADD_RECORD                            ();
         int                            CHECK_UPDATE                          ( int p_kullanici_id );
         void                           UPDATE_RECORD                         ( int p_kullanici_id );
         int                            CHECK_DELETE                          ( int p_kullanici_id );
         void                           DELETE_RECORD                         ( int p_kullanici_id );

         int                            FIND_NEXT_RECORD                      ();
         int                            FIND_PREV_RECORD                      ();
         int                            FIND_FIRST_RECORD                     ();
         int                            FIND_LAST_RECORD                      ();

         int                            LOCK_RECORD                           ( int p_kullanici_id );
         void                           UNLOCK_RECORD                         ( int p_kullanici_id );

         int                            CHANGER_BUTTON_CLICKED                ( QAbstractButton * p_button );
         void                           SAVER_BUTTON_CLICKED                  ( QAbstractButton * p_button, int p_record_id );

         int                            LISTEYE_BILGISAYAR_EKLE               ();
         int                            LISTEYE_VERITABANI_EKLE               ();

         int                            BILGISAYAR_LISTESINDEN_SATIR_SIL      ( int p_clicked_row, int p_clicked_column );
         int                            VERITABANI_LISTESINDEN_SATIR_SIL      ( int p_clicked_row, int p_clicked_column );

};

#endif // KULLANICILAR_FORMU_H
