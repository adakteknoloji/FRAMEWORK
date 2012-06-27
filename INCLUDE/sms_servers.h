#ifndef SMS_SERVERS_H
#define SMS_SERVERS_H

class SMS_SERVER_ABSTRACT;
class QString;
class QStringList;

#include "sms_server_abstract.h"

SMS_SERVER_ABSTRACT *       CREATE_SMS_OBJECT       (QString sunucu);
QStringList                 GET_SMS_SERVER_LIST     ();
QStringList                 GET_SMS_PROTOCOL_LIST   (QString server_name);
ENUM_SMS_PROTOCOL           GET_PROTOCOL_ENUM       (QString protocol);


#endif // SMS_SERVERS_H
