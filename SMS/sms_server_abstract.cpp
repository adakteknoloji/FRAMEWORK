#include <QString>

#include "sms_server_abstract.h"

/************************************************************************************
                            SMS_SENDER::SMS_SERVER_ABSTRACT
************************************************************************************/

SMS_SERVER_ABSTRACT::SMS_SERVER_ABSTRACT()
{

}

/************************************************************************************
                            SMS_SENDER::SET_PROTOCOL
************************************************************************************/

void SMS_SERVER_ABSTRACT::SET_SMS_PROTOCOL( ENUM_SMS_PROTOCOL  protocol  )
{
    m_enum_protol = protocol;
}

/************************************************************************************
                            SMS_SENDER::PROTOCOL
************************************************************************************/

ENUM_SMS_PROTOCOL SMS_SERVER_ABSTRACT::SMS_PROTOCOL()
{
    return m_enum_protol;
}

/************************************************************************************
                            SMS_SENDER::GET_REQUEST_URL
************************************************************************************/

void SMS_SERVER_ABSTRACT::SET_SERVER_ID ( QString server_id )
{
    m_server_id = server_id;
}

/************************************************************************************
                            SMS_SENDER::GET_REQUEST_URL
************************************************************************************/

void SMS_SERVER_ABSTRACT::SET_USER ( QString username , QString password )
{
    m_username = username;
    m_password = password;
}

/************************************************************************************
                            SMS_SENDER::GET_REQUEST_URL
************************************************************************************/

void SMS_SERVER_ABSTRACT::SET_FROM ( QString name , QString number )
{
    m_from          = name;
    m_from_number   = number;
}

/************************************************************************************
                            SMS_SENDER::GET_REQUEST_URL
************************************************************************************/

void SMS_SERVER_ABSTRACT::SET_SMS ( QString message , QString to_name , QString to_number)
{
    m_message     = message;
    m_to          = to_name;
    m_to_number   = to_number;
}
