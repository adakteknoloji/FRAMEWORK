#include <QStringList>

#include "adak_defines.h"

#include "clickatell.h"

/************************************************************************************
                            CLICKATELL::CLICKATELL
************************************************************************************/

CLICKATELL::CLICKATELL()
{

}

/************************************************************************************
                            CLICKATELL::GET_REQUEST_TEXT
************************************************************************************/

QString CLICKATELL::GET_REQUEST_TEXT()
{
    if ( SMS_PROTOCOL() EQ SMS_XML ) {
        return QString ("<clickAPI>"
                        "<sendMsg>"
                        "<api_id>%1</api_id>"
                        "<user>%2</user>"
                        "<password>%3</password>"
                        "<to>%4</to>"
                        "<text>%5</text>"
                        "<from>%6</from>"
                        "</sendMsg>"
                        "</clickAPI>").arg(m_server_id).arg(m_username).arg(m_password)
                                      .arg(m_to_number).arg(m_message).arg(m_from_number);
    }
    else if ( SMS_PROTOCOL() EQ SMS_SOAP ) {
        return "";
    }
    else if ( SMS_PROTOCOL() EQ SMS_SMPP ) {
        return "";
    }
    else if ( SMS_PROTOCOL() EQ SMS_SMTP ) {
        return "";
    }
    else if ( SMS_PROTOCOL() EQ SMS_FTP ) {
        return "";
    }
    else if ( SMS_PROTOCOL() EQ SMS_HTTP_POST ) {
        return "";
    }
    else if ( SMS_PROTOCOL() EQ SMS_HTTPS_POST ) {
        return "";
    }

    return "";
}

/************************************************************************************
                            CLICKATELL::GET_REQUEST_URL
************************************************************************************/

QString CLICKATELL::GET_REQUEST_URL()
{
    if ( SMS_PROTOCOL() EQ SMS_XML ) {
        return QString ("http://api.clickatell.com/xml/xml?data=<clickAPI>"
                           "<sendMsg>"
                           "<api_id>%1</api_id>"
                           "<user>%2</user>"
                           "<password>%3</password>"
                           "<to>%4</to>"
                           "<text>%5</text>"
                           "<from>%6</from>"
                           "</sendMsg>"
                           "</clickAPI>").arg(m_server_id).arg(m_username).arg(m_password)
                                         .arg(m_to_number).arg(m_message).arg(m_from_number);
    }
    else if ( SMS_PROTOCOL() EQ SMS_SOAP ) {
        return "http://api.clickatell.com/soap/webservice.php";
    }
    else if ( SMS_PROTOCOL() EQ SMS_SMPP ) {
        return "http://api.clickatell.com/xml/xml?data=";
    }
    else if ( SMS_PROTOCOL() EQ SMS_SMTP ) {
        return "http://api.clickatell.com/xml/xml?data=";
    }
    else if ( SMS_PROTOCOL() EQ SMS_FTP ) {
        return "http://api.clickatell.com/xml/xml?data=";
    }
    else if ( SMS_PROTOCOL() EQ SMS_HTTP_POST ) {
        return "http://api.clickatell.com/xml/xml?data=";
    }
    else if ( SMS_PROTOCOL() EQ SMS_HTTPS_POST ) {
        return "http://api.clickatell.com/xml/xml?data=";
    }

    return "";
}

/************************************************************************************
                            CLICKATELL::GET_REQUEST_URL
************************************************************************************/

QStringList CLICKATELL::GET_SUPPORTED_PROTOCOLS()
{
    QStringList supported_protocols;

    supported_protocols << "XML";

    return supported_protocols;
}
