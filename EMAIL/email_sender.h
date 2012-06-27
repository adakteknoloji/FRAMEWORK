#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H

#include <QApplication>
#include <QSplashScreen>
#include <QtDebug>
#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QSslSocket>
#include <QTextStream>
#include "adak_defines.h"

class ADAK_SQL;

class EMAIL_SENDER : public QObject
{
    Q_OBJECT

public:
    EMAIL_SENDER();
    ~EMAIL_SENDER();

    /*
     * Burda tanimlanan from ve to degerleri static degerlerin uzerine yazilmadan, direk burda tanimlanan from ve to degerlerine
     * gore email gonderilir. Eger bu degerler "" empty gonderilirse otomatik olarak static olan kimden ve kime degerleri kullanilir.
     */

    void                     SEND_EMAIL          (const QString &email_adresi_from,const QString &gonderenin_gercek_ismi, const QString &email_adresi_to,  const QString &subject, const QString &message,const int program_id , const int module_id );


    ////////////////////////////////////////SET FONKSIYONLARI
    /*
     * Static degerler set fonksiyonlari ile atanabilir.Boylelikle degismeyen degerler tekrar tekrar her class icin atanmak zorunda
     * degildir.Bu metodla bir class icin atanan static degerler tum program boyunca tekrar atana kadar ayni kalacaklardir.
     *
     * Ornek;
     * EMAIL_SENDER::SET_MAIL_SERVER("mail.adak.com.tr",25,NO_SSL);
     */
    //Bu fonksiyonla kullanilacak server,port ve protokeller atanir.Protocoller SSL_PROTOCOL enum da tanimlidir.
    static void              SET_EMAIL_SERVER    (const QString email_server_adress,int port,SSL_PROTOCOL protocol);
    //Kullanici bilgileri setlenir.
    static void              SET_USER_EMAIL_INFO (const QString login_name,const QString login_password,QString gonderenin_gercek_ismi,bool need_authentication = true);
    static void              SET_PORT(int port);
    static void              SET_EMAIL_ADRESI_FROM(QString email_adresi_from);
    static void              SET_EMAIL_ADRESI_TO(QString email_adresi_to);
    static void              SET_GONDERENIN_GERCEK_ISMI(QString gonderenin_gercek_ismi);
    //static void              SET_ALICININ_GERCEK_ISMI(QString alicinin_gercek_ismi);
    static void              SET_EMAIL_SERVER_ADRESS(QString mail_server_adress);
    static void              SET_LOGIN_NAME(QString login_name);
    static void              SET_LOGIN_PASSWORD(QString login_password);
    static void              SET_PROTOCOL(SSL_PROTOCOL protocol);
    static void              SET_TOPLU_GONDER_FLAG(bool value);
    static void              START_EMAIL_LOG    ();
    static void              STOP_EMAIL_LOG     ();

    /////////////////////////////////////////GET Fonksiyonlari
    static int               GET_PORT();
    static bool              GET_TOPLU_GONDER_FLAG();
    static QString           GET_EMAIL_ADRESI_FROM();
    static QString           GET_EMAIL_ADRESI_TO();
    static QString           GET_GONDERENIN_GERCEK_ISMI();
    //static QString           GET_ALICININ_GERCEK_ISMI(QString alicinin_gercek_ismi);
    static QString           GET_EMAIL_SERVER_ADRESS();
    static QString           GET_LOGIN_NAME();
    static QString           GET_LOGIN_PASSWORD();
    static SSL_PROTOCOL      GET_PROTOCOL();    

private slots:
    void                     READY_TO_READ();
    void                     CONNECTED();
    void                     STATE_CHANGED       (QAbstractSocket::SocketState socketState );
    void                     ERROR_RECIEVED      (QAbstractSocket::SocketError socketError );
    void                     DISCONNETED();

private:
    enum State {
        None,
        Stls,
	Init,
        Auth,
        User,
        Pass,
	Mail,
	Rcpt,
	Data,
	Body,
	Quit,
	Close
    };

    int                      p_module_id;
    int                      p_program_id;

    static int               p_port_number;
    static bool              p_need_authentication;
    static bool              p_toplu_gonder;
    static bool              p_add_log;
    static QString           p_kimden;
    static QString           p_kime;
    static QString           p_gonderen_ismi;
    static QString           p_alici_ismi;
    static QString           p_mail_server;
    static QString           p_username;
    static QString           p_password;
    static SSL_PROTOCOL      p_protocol;

    QString                  p_mime_string;
    QString                  p_header;
    QString                  p_message;

    QString                  p_from;
    QString                  p_to;
    QString                  p_gonderilen_str;
    QString                  p_sender_name;
    QString                  p_recipter_name;

    QSslSocket *             p_socket;
    QStringList              p_gonderilen_listesi;
    QTextStream *            p_t;
    QSplashScreen *          p_splash_screen;
    QString                  p_response;
    int                      p_state;
    bool                     p_toplu_mesaj_mi;

    bool                     starttls;

    QString                  CREATE_QUOTED_PRINTABLE_STRING(QString string);
    QString                  CREATE_MESSAGE_ID(QString str = "");
};

QString      GET_SSL_PROTOCOL_NAME(SSL_PROTOCOL protocol);
QString      GET_TIME_OFFSET();

#endif
