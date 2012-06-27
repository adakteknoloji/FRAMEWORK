#ifndef SMS_SENDER_GUI_H
#define SMS_SENDER_GUI_H

#include <QtGui/QDialog>
#include "batch_kernel.h"
#include "ui_sms_sender_gui.h"

namespace Ui {
    class SMS_SENDER_GUI;
}

class SMS_SENDER_GUI : public BATCH_KERNEL
{
    Q_OBJECT

public:
                           SMS_SENDER_GUI                            (QString from, QString to, QString message, QWidget *parent = 0);
                           ~SMS_SENDER_GUI                           () {};
private:
    Ui::SMS_SENDER_GUI *   m_ui;

    void                   SETUP_FORM                           ();
    int                    CHECK_VAR                            (QObject * object);
    int                    CHECK_RUN                            ();
    void                   RUN_BATCH                            ();

    QString                p_from_gsm_no;
    QString                p_to_gsm_no;
    QString                p_message;

};

void ADAK_SMS_SENDER(const QString &from, const QString &to, const QString &messagebody, QWidget * parent = 0);

#endif // SMS_SENDER_GUI_H
