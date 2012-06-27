#ifndef SMS_SENDER_H
#define SMS_SENDER_H

#include <QString>
#include <QObject>
#include "sms_server_abstract.h"

class SMS_SENDER : public QObject
{
    Q_OBJECT

public:
    SMS_SENDER                                  ( QString sms_sunucu,QString m_server_id, QString m_username,QString m_password );
    ~SMS_SENDER                                 ();

    bool                    SEND_SMS            ( const QString &from_gsm_no, const QString &to_gsm_no,const QString &message);

private:

    SMS_SERVER_ABSTRACT *   m_sms_obj;

    bool                    SEND_SMS_HTTP_POST  ();
    bool                    SEND_SMS_HTTP_GET   ();
};

#endif
