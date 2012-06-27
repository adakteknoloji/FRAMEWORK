#ifndef CLICKATELL_H
#define CLICKATELL_H

#include "sms_server_abstract.h"

class CLICKATELL : public SMS_SERVER_ABSTRACT
{
public:
    CLICKATELL();

    QStringList     GET_SUPPORTED_PROTOCOLS();
    QString         GET_REQUEST_TEXT();
    QString         GET_REQUEST_URL();

};

#endif // CLICKATELL_H
