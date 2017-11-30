#ifndef EMAIL_SENDER_GUI_H
#define EMAIL_SENDER_GUI_H

#include <QDialog>
#include "batch_kernel.h"



namespace Ui {
    class EMAIL_SENDER_GUI;
}

class EMAIL_SENDER_GUI : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           EMAIL_SENDER_GUI                            (QString email_adresi_from,QString gonderenin_gercek_ismi, QString email_adresi_to, QString subject, QString message, QWidget *parent = 0);
                           ~EMAIL_SENDER_GUI                           () {};
private:
    Ui::EMAIL_SENDER_GUI *   m_ui;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    QString                p_from;
    QString                p_to;
    QString                p_subject;
    QString                p_body;
    QString                p_gonderenin_adi;
};

#endif // EMAIL_SENDER_GUI_H
