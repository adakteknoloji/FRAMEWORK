#ifndef XYZ_ANA_MENU_H
#define XYZ_ANA_MENU_H

#include <QMainWindow>
#include "menu_kernel.h"

namespace Ui {
    class XYZ_ANA_MENU;
}

class XYZ_ANA_MENU : public MENU_KERNEL {
    Q_OBJECT

public:

        XYZ_ANA_MENU   ( QWidget * p_parent = 0 );
        ~XYZ_ANA_MENU();

private:

    Ui::XYZ_ANA_MENU  *    ui;

    void                        SETUP_FORM                              ();
    void                        MENU_ITEM_CLICKED                       ( int enum_id );


private slots:

    void            KULLANICI_YETKILERINI_AYARLA                        ();

};

#endif // XYZ_ANA_MENU_H
