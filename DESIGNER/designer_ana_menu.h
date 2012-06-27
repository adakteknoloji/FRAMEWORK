#ifndef DESIGNER_ANA_MENU_H
#define DESIGNER_ANA_MENU_H

#include <QMainWindow>
#include "menu_kernel.h"

namespace Ui {
    class DESIGNER_ANA_MENU;
}

class DESIGNER_ANA_MENU : public MENU_KERNEL {
    Q_OBJECT

public:

        DESIGNER_ANA_MENU   ( QWidget * p_parent = 0 );
        ~DESIGNER_ANA_MENU();

private:

    Ui::DESIGNER_ANA_MENU  *    ui;

    void                        SETUP_FORM                              ();
    void                        MENU_ITEM_CLICKED                       ( int enum_id );


private slots:

    void            SLOT_SQL_DESIGNER                                   ();
    void            SLOT_BELGE_DESIGNER                                 ();
    void            SLOT_HAKKINDA                                       ();
    void            SLOT_HEADER_OLUSTUR                                 ();

    void            SLOT_KULLANICILAR                                   ();
    void            SLOT_BILGISAYARLAR                                  ();
    void            SLOT_VERITABANLARI                                  ();
    void            SLOT_YONETIM_007                                    ();

    void            KULLANICI_YETKILERINI_AYARLA                        ();

};

#endif // DESIGNER_ANA_MENU_H
