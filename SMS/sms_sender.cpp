#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QTimer>
#include <QUrl>

#include "adak_defines.h"
#include "sms_sender.h"
#include "sms_servers.h"
#include "sms.h"

/****************************************************************/
/*             SMS_SENDER::SMS_SENDER                           */
/****************************************************************/

SMS_SENDER::SMS_SENDER(QString sms_sunucu, QString m_server_id, QString m_username, QString m_password)
{
    QStringList server_info = sms_sunucu.split("\t\t");

    m_sms_obj = CREATE_SMS_OBJECT       (server_info.at(0));
    m_sms_obj->SET_SMS_PROTOCOL         (GET_PROTOCOL_ENUM(server_info.at(1)));
    m_sms_obj->SET_SERVER_ID            (m_server_id);
    m_sms_obj->SET_USER                 (m_username,m_password);
}

/****************************************************************/
/*             SMS_SENDER::~SMS_SENDER                          */
/****************************************************************/

SMS_SENDER::~SMS_SENDER()
{

}

/****************************************************************/
/*             SMS_SENDER::SEND_MESSAGE_XML                     */
/****************************************************************/

bool SMS_SENDER::SEND_SMS_HTTP_POST()
{
    QByteArray xml;

    QNetworkAccessManager * network_access_manager = new QNetworkAccessManager();

    QUrl url(m_sms_obj->GET_REQUEST_URL());

    QNetworkRequest request(url);

    xml = QVariant(m_sms_obj->GET_REQUEST_TEXT()).toByteArray();

    if ( m_sms_obj->SMS_PROTOCOL() EQ SMS_SOAP ) {
        request.setRawHeader("Content-Type", "application/soap+xml; charset=utf-8");
    }

    QNetworkReply * network_reply = network_access_manager->post(request,xml);

    QEventLoop answer_loop;

    connect(network_reply, SIGNAL(finished()), &answer_loop, SLOT(quit()));
    QTimer bekleme_suresi;
    connect(&bekleme_suresi, SIGNAL(timeout()), &answer_loop, SLOT(quit()));
    bekleme_suresi.start(10000);
    answer_loop.exec();

    network_reply->readAll();

    if ( network_reply->error() != QNetworkReply::NoError ) {
        qDebug() << QObject::tr("Error:") << network_reply->errorString();
        return false;
    }

    return true;
}

/****************************************************************/
/*             SMS_SENDER::SEND_MESSAGE_HTML                    */
/****************************************************************/

bool SMS_SENDER::SEND_SMS_HTTP_GET()
{
    QNetworkAccessManager * network_access_manager = new QNetworkAccessManager();

    QUrl url(m_sms_obj->GET_REQUEST_URL());

    qDebug(m_sms_obj->GET_REQUEST_URL().toUtf8().data());

    QNetworkReply * network_reply = network_access_manager->get(QNetworkRequest(url));

    QEventLoop answer_loop;
    connect(network_reply, SIGNAL(finished()), &answer_loop, SLOT(quit()));
    QTimer bekleme_suresi;
    connect(&bekleme_suresi, SIGNAL(timeout()), &answer_loop, SLOT(quit()));
    bekleme_suresi.start(10000);
    answer_loop.exec();

    network_reply->readAll();

    if ( network_reply->error() != QNetworkReply::NoError ) {
        qDebug() << "Error:" << network_reply->errorString();
        return false;
    }

    return true;
}

/****************************************************************/
/*             SMS_SENDER::SEND_SMS                             */
/****************************************************************/

bool SMS_SENDER::SEND_SMS(const QString &from, const QString &to, const QString &message)
{
    m_sms_obj->SET_FROM("",from);
    m_sms_obj->SET_SMS(message,"",to);

    if ( m_sms_obj->SMS_PROTOCOL() EQ SMS_XML ) {
        return SEND_SMS_HTTP_GET();
    }
    else {
        return SEND_SMS_HTTP_POST();
    }
}

