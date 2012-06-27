#ifndef BILGISAYARLAR_FORMU_H
#define BILGISAYARLAR_FORMU_H

#include <QtGui/QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include "QCommaEdit.h"
#include <form_kernel.h>

namespace Ui {
    class BILGISAYARLAR_FORMU;
}

class BILGISAYARLAR_FORMU : public FORM_KERNEL
{
    Q_OBJECT
public:
                        BILGISAYARLAR_FORMU                    ( QWidget * parent = 0 );
                        ~BILGISAYARLAR_FORMU                   () {};

private :

     Ui::BILGISAYARLAR_FORMU    *   m_ui;
     int                        m_secilen_kullanici_id;

     void                       SETUP_FORM                           ();
     void                       CLEAR_FORM_MEMBERS                   ();
     void                       NEW_RECORD                           ();
     int                        GET_RECORD                           ( int p_bilgisayar_id );
     int                        FIND_RECORD                          ();
     int                        SELECT_RECORD                        ();


     int                        CHECK_VAR                            ( QObject * p_object );
     int                        CHECK_EMPTY                          ();

     int                        CHECK_ADD                            ();
     int                        ADD_RECORD                           ();
     int                        CHECK_UPDATE                         ( int p_bilgisayar_id );
     void                       UPDATE_RECORD                        ( int p_bilgisayar_id );
     int                        CHECK_DELETE                         ( int p_bilgisayar_id );
     void                       DELETE_RECORD                        ( int p_bilgisayar_id );

     int                        FIND_NEXT_RECORD                     ();
     int                        FIND_PREV_RECORD                     ();
     int                        FIND_FIRST_RECORD                    ();
     int                        FIND_LAST_RECORD                     ();

     int                        LOCK_RECORD                          ( int p_bilgisayar_id );
     void                       UNLOCK_RECORD                        ( int p_bilgisayar_id );

     int                        LISTEYE_KULLANICI_EKLE               ();
     int                        CHANGER_BUTTON_CLICKED               ( QAbstractButton * p_button );

     int                        KULLANICILAR_LISTESINDEN_SATIR_SIL   ( int p_clicked_row, int p_clicked_column );

};


#endif // BILGISAYARLAR_FORMU_H
