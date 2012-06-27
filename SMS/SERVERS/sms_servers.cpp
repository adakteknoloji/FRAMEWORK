#include <QStringList>
#include <QString>

#include "adak_defines.h"

#include "clickatell.h"

#include "sms_servers.h"
#include "sms_sender.h"

/****************************************************************/
/*             GET_SMS_SERVER_LIST                              */
/****************************************************************/

QStringList GET_SMS_SERVER_LIST()
{
    QStringList servers;

    servers << "Yurtdışı - Clickatell";

    return servers;
}

/****************************************************************/
/*             GET_SMS_SERVER_LIST                              */
/****************************************************************/

QStringList GET_SMS_PROTOCOL_LIST(QString server_name)
{
    SMS_SERVER_ABSTRACT * sms_obj = CREATE_SMS_OBJECT(server_name);

    if (sms_obj EQ NULL) {
        return QStringList();
    }
    return sms_obj->GET_SUPPORTED_PROTOCOLS();
}

/****************************************************************/
/*             GET_SMS_SERVER_LIST                              */
/****************************************************************/

SMS_SERVER_ABSTRACT * CREATE_SMS_OBJECT(QString sunucu)
{
    if (sunucu EQ "Yurtdışı - Clickatell") {
        return new CLICKATELL();
    }

    return NULL;
}

/****************************************************************/
/*             GET_PROTOCOL_ENUM                                */
/****************************************************************/

ENUM_SMS_PROTOCOL GET_PROTOCOL_ENUM(QString protocol)
{
    if ( protocol EQ "SMS_XML" OR protocol EQ "XML" ) {
        return SMS_XML;
    }
    else if ( protocol EQ "SMS_SOAP" OR protocol EQ "SOAP" ) {
        return SMS_SOAP;
    }
    else if ( protocol EQ "SMS_SMPP" OR protocol EQ "SMPP" ) {
        return SMS_SMPP;
    }
    else if ( protocol EQ "SMS_SMTP" OR protocol EQ "SMTP" ) {
        return SMS_SMTP;
    }
    else if ( protocol EQ "SMS_FTP" OR protocol EQ "FTP" ) {
        return SMS_FTP;
    }
    else if ( protocol EQ "SMS_HTTP_POST" OR protocol EQ "HTTP POST" ) {
        return SMS_HTTP_POST;
    }
    else if ( protocol EQ "SMS_HTTPS_POST" OR protocol EQ "HTTPS POST" ) {
        return SMS_HTTPS_POST;
    }

    return SMS_UNKNOWN;
}
