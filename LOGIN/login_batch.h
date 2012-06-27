#ifndef LOGIN_BATCH_H
#define LOGIN_BATCH_H

#include "batch_kernel.h"

struct USER_LOGIN_INFO_STRUCT;

namespace Ui {
    class LOGIN_BATCH;
}

class QMainWindow;
class QMenu;
class PROGRAM_CORE;


class LOGIN_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

private:

    Ui::LOGIN_BATCH     *   m_ui;

public:
    LOGIN_BATCH ( USER_LOGIN_INFO_STRUCT * user_informations, bool * all_informations_completed , QWidget * parent = 0 );
    ~LOGIN_BATCH() {}

private:
    int     m_veritabani_sayisi;
    bool *  m_all_informations_completed;

    USER_LOGIN_INFO_STRUCT * m_user_info;

    void    SETUP_FORM                ();
    int     CHECK_VAR                 ( QObject * object );
    int     CHECK_RUN                 ();
    void    RUN_BATCH                 ();

    void    SET_WIDGETS_MAX_LENGTH    ();

    void    REORGANIZE_SCREEN         ();

    void    FILL_COMPUTER_WIDGETS     (QString bilgisayar_adi  );
    void    FILL_USER_WIDGETS         (QString kullanici_kodu  );
    void    FILL_DATABASE_WIDGETS     (QString veritabani_kodu );
    void    FILL_SQL_PROFILLERI       ();

    void    KULLANICI_SIFRESI_DEGISTIR();
    void    SECILI_PROFILI_YUKLE      (QString profil);

};

#endif // LOGIN_BATCH_H
