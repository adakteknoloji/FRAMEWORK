#ifndef SMS_SERVER_H
#define SMS_SERVER_H

class QString;
class QStringList;

enum ENUM_SMS_PROTOCOL {
    SMS_XML        = 0,
    SMS_SOAP       = 1,
    SMS_SMPP       = 2,
    SMS_SMTP       = 3,
    SMS_FTP        = 4,
    SMS_HTTP_POST  = 5,
    SMS_HTTPS_POST = 6,
    SMS_UNKNOWN    = 7
};

class SMS_SERVER_ABSTRACT
{
public:
                         SMS_SERVER_ABSTRACT             ();

    void                 SET_SMS_PROTOCOL                ( ENUM_SMS_PROTOCOL  protocol  );

    void                 SET_USER                        ( QString username , QString password );
    void                 SET_FROM                        ( QString name , QString number );
    void                 SET_SERVER_ID                   ( QString server_id );
    void                 SET_SMS_SUNUCU                  ( QString sunucu    );

    void                 SET_SMS                         ( QString message , QString to_name , QString to_number);

    ENUM_SMS_PROTOCOL    SMS_PROTOCOL                    ();

    virtual QString      GET_REQUEST_TEXT                () = 0;
    virtual QString      GET_REQUEST_URL                 () = 0;

    virtual QStringList  GET_SUPPORTED_PROTOCOLS         () = 0;

protected:
    QString              m_server_id;
    QString              m_username;
    QString              m_password;
    QString              m_from;
    QString              m_to;
    QString              m_from_number;
    QString              m_to_number;
    QString              m_message;

private:
    ENUM_SMS_PROTOCOL    m_enum_protol;
};

#endif // SMS_SERVER_H
