#include <QByteArray>
#include <QVariant>
#include <QDate>
#include <QCryptographicHash>
#include "adak_sql.h"
#include "email_sender.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "adak_utils.h"
#include "yonetim.h"
#include "email.h"

//Eger gonderilen email icin size buyuk oldugu uyarisi aliniyorsa bu deger kucultulerek saglikli calismasi saglanabilir.
#define MAX_LINE_SIZE 2500

int               EMAIL_SENDER::p_port_number         = 25;
bool              EMAIL_SENDER::p_need_authentication = false;
bool              EMAIL_SENDER::p_toplu_gonder        = true;
bool              EMAIL_SENDER::p_add_log             = true;
QString           EMAIL_SENDER::p_kimden              = "";
QString           EMAIL_SENDER::p_kime                = "";
QString           EMAIL_SENDER::p_gonderen_ismi       = QObject::tr("Nameless");
QString           EMAIL_SENDER::p_alici_ismi          = "";
QString           EMAIL_SENDER::p_mail_server         = QObject::tr("Not Described");
QString           EMAIL_SENDER::p_username            = "";
QString           EMAIL_SENDER::p_password            = "";
SSL_PROTOCOL      EMAIL_SENDER::p_protocol            = NO_SSL;

static QStringList g_gonderilen_listesi;
static QStringList g_gonderilemeyen_listesi;
static QStringList g_hata_listesi;
static bool        g_wait_for_email = true;

extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   EMAIL_SENDER::EMAIL_SENDER
***************************************************************************************/

EMAIL_SENDER::EMAIL_SENDER()
{
    p_socket              = new QSslSocket ( this );
    starttls              = false;

    p_splash_screen = CREATE_SPLASH_SCREEN();

    p_mime_string = QString ( "MIME-Version: 1.0" ) +
                    QString ( "\nContent-Type: text/html; charset=" ) +
                    QString ( "utf-8") +
                    QString ( "\nContent-Transfer-Encoding: quoted-printable" );

    connect( p_socket, SIGNAL( readyRead() ),                                 this, SLOT ( READY_TO_READ() ) );
    connect( p_socket, SIGNAL( connected() ),                                 this, SLOT ( CONNECTED() ) );
    connect( p_socket, SIGNAL( error( QAbstractSocket::SocketError) ),        this, SLOT ( ERROR_RECIEVED  ( QAbstractSocket::SocketError) ) );
    connect( p_socket, SIGNAL( stateChanged( QAbstractSocket::SocketState) ), this, SLOT ( STATE_CHANGED  ( QAbstractSocket::SocketState) ) );
    connect( p_socket, SIGNAL( disconnected() ),                              this, SLOT ( DISCONNETED() ) );
}

/**************************************************************************************
                   EMAIL_SENDER::~EMAIL_SENDER
***************************************************************************************/

EMAIL_SENDER::~EMAIL_SENDER()
{
    delete p_t;
    delete p_socket;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_PORT
***************************************************************************************/

void EMAIL_SENDER::SET_PORT ( int port )
{
    p_port_number = port;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_EMAIL_ADRESI_FROM
***************************************************************************************/

void EMAIL_SENDER::SET_EMAIL_ADRESI_FROM ( QString email_adresi_from )
{
    p_kimden = email_adresi_from;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_EMAIL_ADRESI_TO
***************************************************************************************/

void EMAIL_SENDER::SET_EMAIL_ADRESI_TO ( QString email_adresi_to )
{
    p_kime = email_adresi_to;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_GONDERENIN_GERCEK_ISMI
***************************************************************************************/

void EMAIL_SENDER::SET_GONDERENIN_GERCEK_ISMI ( QString gonderenin_gercek_ismi)
{
    p_gonderen_ismi = gonderenin_gercek_ismi;
}


/**************************************************************************************
                   EMAIL_SENDER::SET_EMAIL_SERVER_ADRESS
***************************************************************************************/

void EMAIL_SENDER::SET_EMAIL_SERVER_ADRESS ( QString mail_server_adress )
{
    p_mail_server = mail_server_adress;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_LOGIN_NAME
***************************************************************************************/

void EMAIL_SENDER::SET_LOGIN_NAME ( QString login_name )
{
    p_username = login_name;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_LOGIN_PASSWORD
***************************************************************************************/

void EMAIL_SENDER::SET_LOGIN_PASSWORD ( QString login_password )
{
    p_password = login_password;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_PROTOCOL
***************************************************************************************/

void EMAIL_SENDER::SET_PROTOCOL ( SSL_PROTOCOL protocol )
{
    p_protocol = protocol;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_MAIL_SERVER
***************************************************************************************/

void EMAIL_SENDER::SET_EMAIL_SERVER ( QString mail_server_adress, int port, SSL_PROTOCOL protocol )
{
    p_mail_server = mail_server_adress;
    p_port_number = port;
    p_protocol    = protocol;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_USER_EMAIL_INFO
***************************************************************************************/

void EMAIL_SENDER::SET_USER_EMAIL_INFO (QString login_name, QString login_password,QString gonderenin_gercek_ismi, bool need_authentication)
{
    p_username             = login_name;
    p_password             = login_password;
    p_gonderen_ismi        = gonderenin_gercek_ismi;
    p_need_authentication  = need_authentication;
}

/**************************************************************************************
                   EMAIL_SENDER::SET_TOPLU_GONDER_FLAG
***************************************************************************************/

void EMAIL_SENDER::SET_TOPLU_GONDER_FLAG(bool value)
{
    p_toplu_gonder = value;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_TOPLU_GONDER_FLAG
***************************************************************************************/

bool EMAIL_SENDER::GET_TOPLU_GONDER_FLAG()
{
    return p_toplu_gonder;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_PORT
***************************************************************************************/

int EMAIL_SENDER::GET_PORT()
{
    return p_port_number;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_EMAIL_ADRESI_FROM
***************************************************************************************/

QString EMAIL_SENDER::GET_EMAIL_ADRESI_FROM()
{
    return p_kimden;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_EMAIL_ADRESI_TO
***************************************************************************************/

QString EMAIL_SENDER::GET_EMAIL_ADRESI_TO()
{
    return p_kime;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_GONDERENIN_GERCEK_ISMI
***************************************************************************************/

QString EMAIL_SENDER::GET_GONDERENIN_GERCEK_ISMI()
{
    return p_gonderen_ismi;
}


/**************************************************************************************
                   EMAIL_SENDER::GET_EMAIL_SERVER_ADRESS
***************************************************************************************/

QString EMAIL_SENDER::GET_EMAIL_SERVER_ADRESS()
{
    return p_mail_server;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_LOGIN_NAME
***************************************************************************************/

QString EMAIL_SENDER::GET_LOGIN_NAME()
{
    return p_username;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_LOGIN_PASSWORD
***************************************************************************************/

QString EMAIL_SENDER::GET_LOGIN_PASSWORD()
{
    return p_password;
}

/**************************************************************************************
                   EMAIL_SENDER::GET_PROTOCOL
***************************************************************************************/

SSL_PROTOCOL EMAIL_SENDER::GET_PROTOCOL()
{
    return p_protocol;
}

/**************************************************************************************
                   EMAIL_SENDER::STATE_CHANGED
***************************************************************************************/

void EMAIL_SENDER::STATE_CHANGED ( QAbstractSocket::SocketState socketState )
{
    Q_UNUSED ( socketState );
    // qDebug() << "Socket durumu: " << socketState;
}

/**************************************************************************************
                   EMAIL_SENDER::ERROR_RECIEVED
***************************************************************************************/

void EMAIL_SENDER::ERROR_RECIEVED (  QAbstractSocket::SocketError socketError )
{
    Q_UNUSED ( socketError );
    // qDebug() << "hata " <<socketError;
}

/**************************************************************************************
                   EMAIL_SENDER::DISCONNETED
***************************************************************************************/

void EMAIL_SENDER::DISCONNETED()
{
    // qDebug() << "Bağlantı kesildi";
    // qDebug() << "hata "  << p_socket->errorString();
}

/**************************************************************************************
                   EMAIL_SENDER::CONNECTED
***************************************************************************************/

void EMAIL_SENDER::CONNECTED()
{
    // qDebug() << "Connected ";
    p_splash_screen->show();
    p_splash_screen->showMessage ( tr ( "Sending Your Message..." ),Qt::AlignCenter,Qt::white );
    qApp->processEvents();
}

/**************************************************************************************
                   EMAIL_SENDER::READY_TO_READ
***************************************************************************************/

void EMAIL_SENDER::READY_TO_READ()
{
    if (  p_socket->canReadLine() EQ false ) {
        return;
    }

    QString response_text;

    do {
        response_text =  p_socket->readLine();
        p_response      += response_text;
    } while ( p_socket->canReadLine() && response_text[3] NE ' '  );

    response_text.truncate( 3 );

    if ( p_state EQ Init && response_text[0] EQ '2'  ) {
        if ( starttls EQ true ) {
             p_socket->setProtocol ( QSsl::TlsV1_2 );
             p_socket->startClientEncryption();
        }
        *p_t << "HELLO server\r\n";
        p_t->flush();

        if ( p_protocol EQ STARTTLS AND starttls EQ false ) {
             p_state  = Stls;
             starttls = true;
        }
        else if ( p_need_authentication EQ false ) {
            p_state = Mail;
        }
        else {
            p_state = Auth;
        }
    }
    else if ( p_state EQ Stls OR response_text EQ "530" ) {
        *p_t << "STARTTLS\r\n";
        p_t->flush();
        p_state = Init;
        starttls = true;
    }
    else if ( p_state EQ Auth && response_text[0] EQ '2' ) {
        *p_t << "AUTH LOGIN\r\n";
        p_t->flush();
        p_state = User;
    }
    else if ( p_state EQ User && response_text EQ "334") {
        QByteArray username = QVariant ( p_username ).toByteArray().toBase64();
        *p_t << username + "\r\n";
        p_t->flush();
        p_state = Pass;
    }
    else if ( p_state EQ Pass &&  response_text EQ "334" ) {
        QByteArray password = QVariant ( p_password ).toByteArray().toBase64();
        *p_t << password + "\r\n";
        p_t->flush();
        p_state = Mail;
    }
    else if ( p_state EQ Mail && response_text[0] EQ '2' ) {
        QString string_from = tr("MAIL FROM: <")+p_from+">\r\n";
        *p_t << string_from;
        p_t->flush();
        p_state = Rcpt;
    }
    else if ( p_state EQ Rcpt && response_text[0] EQ '2' ) {
        QString string_to = "RCPT TO: <"+p_gonderilen_listesi.last()+">\r\n";
        *p_t << string_to;
        p_t->flush();
        p_gonderilen_listesi.pop_back();
        if (p_gonderilen_listesi.size() EQ 0) {
            p_state = Data;
        }
    }
    else if ( p_state EQ Data && response_text[0] EQ '2' ) {
        *p_t << "DATA\r\n";
        p_t->flush();
        p_state = Body;
    }
    else if ( p_state EQ Body && response_text[0] EQ '3' ) {
        *p_t << p_header << "\r\n";
        p_t->flush();

        QString quoted_printable_string = CREATE_QUOTED_PRINTABLE_STRING ( p_message );

        *p_t << quoted_printable_string << ".\r\n";
        p_t->flush();
        p_state = Quit;
    }
    else if ( p_state EQ Quit && response_text[0] EQ '2' ) {
        *p_t << "QUIT\r\n";
        p_state = Close;
        p_splash_screen->finish ( NULL );
        if ( g_gonderilen_listesi.contains(p_gonderilen_str) EQ false ) {
            g_gonderilen_listesi << p_gonderilen_str;

            if ( p_add_log EQ true ) {
                QStringList email_adresleri = p_to.split(",",QString::KeepEmptyParts,Qt::CaseInsensitive);
                bool close_transaction = false;

                if ( G_YONETIM_DB->TRANSACTION_IS_ON() EQ false ) {
                    G_YONETIM_DB->START_TRANSACTION();
                    close_transaction  = true;
                }

                for ( int i = 0 ; i < email_adresleri.size() ; i++ ) {
                    QStringList record_email_info = email_adresleri.at(i).split(":",QString::KeepEmptyParts,Qt::CaseInsensitive);

                    SQL_QUERY query (G_YONETIM_DB);
                    query.PREPARE_INSERT("adak_email_log","log_id","gonderilen_email,record_id,kullanici_id,program_id,modul_id,tarih,saat");
                    if ( record_email_info.size() EQ 1 ) {
                        query.SET_VALUE(":gonderilen_email",record_email_info.at(0));
                        query.SET_VALUE(":record_id"       ,0                    );
                    }
                    else {
                        query.SET_VALUE(":gonderilen_email",record_email_info.at(1));
                        query.SET_VALUE(":record_id"       ,record_email_info.at(0));
                    }

                    query.SET_VALUE(":kullanici_id"    ,KULLANICI_ID()       );
                    query.SET_VALUE(":program_id"      ,p_program_id         );
                    query.SET_VALUE(":modul_id"        ,p_module_id          );
                    query.SET_VALUE(":tarih"           ,QDate::currentDate().toString("yyyy.MM.dd"));
                    query.SET_VALUE(":saat"            ,QTime::currentTime().toString("HH:mm:ss"));

                    query.INSERT();
                }

                if ( close_transaction EQ true ) {
                    G_YONETIM_DB->COMMIT_TRANSACTION();
                }
            }
        }
        g_wait_for_email = false;
        //qDebug("Mesaj Gönderildi");
    }
    else if ( p_state EQ Close ) {
        g_wait_for_email = false;
        deleteLater();
        return;
    }
    else if ( response_text[0] NE '2' ){
        p_state = Close;
        p_splash_screen->finish ( NULL );
        if ( g_gonderilemeyen_listesi.contains(p_gonderilen_str) EQ false ) {
            g_gonderilemeyen_listesi << p_gonderilen_str;
        }
        if ( g_hata_listesi.contains(p_response) EQ false ) {
            g_hata_listesi           << p_response;
        }
        g_wait_for_email = false;
        //qDebug(p_response.toUtf8().data());
    }
    p_response = "";
}

/**************************************************************************************
                   EMAIL_SENDER::SEND_EMAIL
***************************************************************************************/

void EMAIL_SENDER::SEND_EMAIL ( const QString &from,const QString &gonderenin_gercek_adi, const QString &to, const QString &subject, const QString &body , const int program_id , const int module_id )
{
    p_program_id = program_id;
    p_module_id  = module_id;

    p_from           = p_kimden;
    p_to             = p_kime;
    p_sender_name    = p_gonderen_ismi;
    p_recipter_name  = p_alici_ismi;

    if ( from.isEmpty() EQ false ) {
        p_from = from;
    }

    if ( to.isEmpty() EQ false ) {
        p_to = to;
    }

    //Emailler record_id:email_adresi formatinda da gelebilir.

    QStringList email_list_str = p_to.split(",",QString::KeepEmptyParts,Qt::CaseInsensitive);

    for ( int i = 0 ; i < email_list_str.size() ; i++ ) {
        if ( email_list_str.at(i).isEmpty() EQ true ) {
            continue;
        }

        QStringList email_bilgisi = email_list_str.at(i).split(":");

        if ( email_bilgisi.size() EQ 1 ) {
            if ( p_gonderilen_str.isEmpty() EQ false ) {
                p_gonderilen_str.append(",");
            }

            p_gonderilen_str.append(email_bilgisi.at(0));
            p_gonderilen_listesi << email_bilgisi.at(0);
        }
        else {
            if ( p_gonderilen_str.isEmpty() EQ false ) {
                p_gonderilen_str.append(",");
            }

            p_gonderilen_str.append(email_bilgisi.at(1));
            p_gonderilen_listesi << email_bilgisi.at(1);
        }
    }


    if ( gonderenin_gercek_adi.isEmpty() EQ false ) {
        p_sender_name = gonderenin_gercek_adi;
    }

    QLocale new_locale ( QLocale::English );

    QString date_header_str = QString ( "%1, %2 %3 %4 %5" ).arg ( new_locale.standaloneDayName ( QDate::currentDate().dayOfWeek(),QLocale::ShortFormat ) )
                                                        .arg ( QDateTime::currentDateTime().toString ( "d" ) )
                                                        .arg ( new_locale.standaloneMonthName ( QDate::currentDate().month(),QLocale::ShortFormat) )
                                                        .arg ( QDateTime::currentDateTime().toString ( "yyyy hh:mm:ss") )
                                                        .arg ( GET_TIME_OFFSET() );

    QString quotable_subject = QString ( "=?utf-8?Q?%1?=" ).arg ( CREATE_QUOTED_PRINTABLE_STRING ( subject ) );

    QStringList from_str = p_from.split ( "@" );

    p_header  = QString ( "Message-ID: <%1@%2>" ).arg ( CREATE_MESSAGE_ID ( from_str.at(0) ) ).arg ( from_str.at(1) ) +
                QString ( "\nDate: %1").arg(date_header_str) +
                QString ( "\nFrom: =?utf-8?Q?%1?=  <" ).arg(CREATE_QUOTED_PRINTABLE_STRING(p_sender_name ) )       + p_from    +">"  +
                QString ( "\nUser-Agent: Adak/1.0 AdakEmailSender/1.0") +
                QString ( "\nTo: ")                + p_gonderilen_str  +
                QString ( "\nSubject: " )           + quotable_subject +"\n" + p_mime_string +
                QString ( "\n\n"        );

    p_message = "<pre>" + body + "</pre>" + "\n";

    p_header.replace ( QString (  "\n" )       , QString::fromLatin1 ( "\r\n"  )  );
    p_header.replace ( QString (  "\r\n.\r\n" ), QString::fromLatin1 (  "\r\n..\r\n" )  );

    if ( p_message.size() > MAX_LINE_SIZE ) {
        int parca_sayisi = p_message.size() / MAX_LINE_SIZE;
        for ( int i = 0; i < parca_sayisi ; i++ ) {
            p_message.insert ( (i+1) * MAX_LINE_SIZE, "\r\n" );
        }
    }

    p_message.replace( QString (  "\n" )       , QString::fromLatin1 ( "\r\n" ) );
    p_message.replace( QString (  "\r\n.\r\n" ), QString::fromLatin1 ( "\r\n..\r\n" ) );

    this->p_from   = p_from;
    p_state        = Init;

    if ( p_protocol EQ NO_SSL OR p_protocol EQ STARTTLS ) {
        p_socket->connectToHost ( p_mail_server, p_port_number );
        if (  p_socket->waitForConnected (  15000 ) EQ false){
            g_hata_listesi << QString (tr("Could not connect to Mail Server. ( STARTTLS/NOSSL ) \nSocket Error : %1 %2 ")).arg(p_socket->error()).arg(p_socket->errorString ()) ;
        }
    }
    else {
        if ( p_protocol EQ TLSV1 ) {
             p_socket->setProtocol ( QSsl::TlsV1_2 );
        }
        else if ( p_protocol EQ SSLV2 ) {
            p_socket->setProtocol ( QSsl::SslV2 );
        }
        else if ( p_protocol EQ SSLV3 ) {
            p_socket->setProtocol ( QSsl::SslV3 );
        }
        p_socket->connectToHostEncrypted ( p_mail_server, p_port_number );
        if( p_socket->waitForEncrypted ( 15000 ) ){
            g_hata_listesi << QString (tr("Could not connect to Mail Server. ( TLS1/SSL2/SSL3 ) \nSocket Error : %1 %2 ")).arg(p_socket->error()).arg(p_socket->errorString ()) ;
        }//Posta Sunucusuna Bağlanılamadı. ( TLS1/SSL2/SSL3 ) \nSocket Hatasi : %1 %2
    }

    p_t = new QTextStream (  p_socket  );
}

/**************************************************************************************
                   EMAIL_SENDER::CREATE_QUOTED_PRINTABLE_STRING
***************************************************************************************/

QString EMAIL_SENDER::CREATE_QUOTED_PRINTABLE_STRING ( QString string )
{
    QString quoted_printable_string = "";

    for ( int i = 0; i < string.length() ; i ++ ) {
        if ( string.at(i) EQ '=' )  {
            quoted_printable_string.append ( "=3D" );
            continue;
        }
        if ( string.at(i) >= 33 AND string.at(i) <= 126 ) {
             quoted_printable_string.append ( string.at(i) );
            continue;
        }
        if ( string.at(i) EQ '\r' || string.at(i) EQ '\n' ) {
             quoted_printable_string.append(string.at(i) );
            continue;
        }
        QString byte_array = QVariant ( QByteArray ( QString ( string.at(i)).toUtf8().data()).toHex()).toString();
        byte_array = byte_array.toUpper();
        for ( int i = 0 ; i < byte_array.length() ; i++ ) {
            if ( i % 3 EQ 0 ) {
                byte_array.insert ( i,"=" );
            }
        }
        quoted_printable_string.append ( byte_array );
    }

    return quoted_printable_string;
}

/**************************************************************************************
                   EMAIL_SENDER::CREATE_MESSAGE_ID
***************************************************************************************/

QString EMAIL_SENDER::CREATE_MESSAGE_ID ( QString str )
{
    Q_UNUSED ( str );

    QTime time;

    QString random_str = QString ( "%1 - %2" ).arg ( time.msecsTo ( QTime::currentTime() ) ).arg ( QTime::currentTime().toString ( "hh:mm:ss" ) );

    QByteArray message_id = QCryptographicHash::hash ( random_str.toUtf8(),QCryptographicHash::Sha1 );

    return QVariant ( message_id.toHex() ).toString();
}

/**************************************************************************************
                   EMAIL_SENDER::START_EMAIL_LOG
***************************************************************************************/

void EMAIL_SENDER::START_EMAIL_LOG()
{
    EMAIL_SENDER::p_add_log = true;
}

/**************************************************************************************
                   EMAIL_SENDER::START_EMAIL_LOG
***************************************************************************************/

void EMAIL_SENDER::STOP_EMAIL_LOG()
{
    EMAIL_SENDER::p_add_log = false;
}

/**************************************************************************************
                   GET_SSL_PROTOCOL_ID
***************************************************************************************/

SSL_PROTOCOL GET_SSL_PROTOCOL_ID ( QString protocol )
{
    if ( protocol EQ "SSLV2" ){
        return SSLV2;
    }
    else if ( protocol EQ "SSLV3" ){
        return SSLV3;
    }
    else if ( protocol EQ "TLSV1" ){
        return TLSV1;
    }
    else if ( protocol EQ "STARTTLS" ){
        return STARTTLS;
    }

    return NO_SSL;
}

/**************************************************************************************
                   GET_SSL_PROTOCOL_NAME
***************************************************************************************/

QString GET_SSL_PROTOCOL_NAME ( SSL_PROTOCOL protocol )
{
    if ( protocol EQ SSLV2 ) {
        return "SSLV2";
    }
    else if  ( protocol EQ SSLV3 ) {
        return "SSLV3";
    }
    else if ( protocol EQ TLSV1 ) {
        return "TLSV1";
    }
    else if  ( protocol EQ STARTTLS ) {
        return "STARTTLS";
    }

    return "NO_SSL";
}

/**************************************************************************************
                   SEND_MAIL
***************************************************************************************/

void SEND_MAIL ( const QString subject, const QString message_to_send ,const int program_id , const int module_id, QString p_uyari_mesaji, const bool add_to_log )
{
    SEND_MAIL( "", "", EMAIL_GET_EMAIL_ADRESI_TO(), subject, message_to_send , program_id, module_id, p_uyari_mesaji, add_to_log );
}

/**************************************************************************************
                   SEND_MAIL
***************************************************************************************/

void SEND_MAIL ( const QString from,const QString gonderenin_gercek_adi, const QString to, const QString subject,
                 const QString message_to_send, const int program_id , const int module_id, QString p_uyari_mesaj, const bool add_to_log )
{
    if (add_to_log EQ false ) {
        EMAIL_SENDER::STOP_EMAIL_LOG();
    }

    QString to_list = to;

    to_list.replace("\r","",Qt::CaseInsensitive);
    to_list.replace("\n","",Qt::CaseInsensitive);
    to_list.replace("\t","",Qt::CaseInsensitive);

    QStringList temp_to_list    = to_list.split(",",QString::SkipEmptyParts);
    QStringList gecersiz_to_list;
    QStringList gecerli_to_list;

    for ( int i = 0 ; i < temp_to_list.size() ; i++ ) {
        if ( temp_to_list.at(i).isEmpty() EQ false AND temp_to_list.at(i).contains("@",Qt::CaseInsensitive) EQ true ) {
            gecerli_to_list  << temp_to_list.at(i);
        }
        else {

            QStringList email_bilgisi = temp_to_list.at(i).split(":",QString::KeepEmptyParts,Qt::CaseInsensitive);

            if (email_bilgisi.size() EQ 1) {
                gecersiz_to_list  << temp_to_list.at(0);
            }
            else {
                gecersiz_to_list  << temp_to_list.at(1);
            }
        }
    }

    if ( gecersiz_to_list.isEmpty() EQ false ) {
        QString message         = QObject::tr("These e-mail address are wrong.");//Aşagıdaki e-mail adres(ler)i hatalıdır.
        QString hatali_adresler = QObject::tr("\nIncorrect E-Mail Addresses: ");

        for ( int i = 0 ; i < gecersiz_to_list.size() ; i++ ) {
            hatali_adresler.append("\n" + gecersiz_to_list.at(i));
        }

        if ( gecerli_to_list.isEmpty() EQ false ) {
            message.append(QObject::tr("Do you want to send the message to current e-mail?"));//Geçerli olan e-maillere mesajı göndermek ister misiniz?
            message.append(hatali_adresler);
            ADAK_MSG_ENUM answer = ADAK_YES_NO(message,NULL,NULL);
            if ( answer NE ADAK_YES ) {
                if (add_to_log EQ false ) {
                    EMAIL_SENDER::START_EMAIL_LOG();
                }
                return;
            }
        }
        else {
            message.append(hatali_adresler);
            ADAK_ERROR(message,NULL,NULL);
            if (add_to_log EQ false ) {
                EMAIL_SENDER::START_EMAIL_LOG();
            }
            return;
        }
    }

    if ( gecerli_to_list.isEmpty() EQ true ) {
        ADAK_ERROR(QObject::tr("No e-mail address will be sent."),NULL,NULL);//Gönderilecek e-mail adresi bulunamadı.
        if (add_to_log EQ false ) {
            EMAIL_SENDER::START_EMAIL_LOG();
        }
        return;
    }

    g_gonderilemeyen_listesi.clear();
    g_gonderilen_listesi.clear();
    g_hata_listesi.clear();

    if ( EMAIL_SENDER::GET_TOPLU_GONDER_FLAG() EQ true ) {
        QString emailler;

        for ( int i = 0 ; i < gecerli_to_list.size() ; i++ ) {
            if ( emailler.isEmpty() EQ false ) {
                emailler.append(",");
            }
            emailler.append(gecerli_to_list.at(i));
        }

        EMAIL_SENDER * sender = new EMAIL_SENDER ();
        sender->SEND_EMAIL ( ( from.toUtf8() ),( gonderenin_gercek_adi.toUtf8() ), ( emailler.toUtf8() ),  ( subject.toUtf8() ), ( message_to_send.toUtf8() ) , program_id, module_id );
        QTime time;
        time.start();
        for ( ; ; ) {
            QApplication::processEvents();
            if ( g_wait_for_email EQ false ) {
                break;
            }
            if ( time.elapsed() > 15000 ) {
                break;
            }
        }
        g_wait_for_email = true;
    }
    else {
        for ( int i = 0 ; i < gecerli_to_list.size() ; i++ ) {
            EMAIL_SENDER * sender = new EMAIL_SENDER ();
            sender->SEND_EMAIL ( ( from.toUtf8() ),( gonderenin_gercek_adi.toUtf8() ), ( gecerli_to_list.at(i).toUtf8() ),  ( subject.toUtf8() ), ( message_to_send.toUtf8() ) , program_id,module_id );
            QTime time;
            time.start();
            for ( ; ; ) {
                QApplication::processEvents();
                if ( g_wait_for_email EQ false ) {
                    break;
                }
                if ( time.elapsed() > 15000 ) {
                    break;
                }
            }
            g_wait_for_email = true;
        }
    }

    if ( g_hata_listesi.isEmpty() EQ false ) {
        QString message = QObject::tr("The below error(s) occured during sending e-mail.");//E-mail gönderimi esnasında aşagıdaki hata(lar) alındı.

        for ( int i = 0 ; i < g_hata_listesi.size() ; i++ ) {
            message.append("\n" + g_hata_listesi.at(i));
        }

        ADAK_ERROR(message,NULL,NULL);
        if (add_to_log EQ false ) {
            EMAIL_SENDER::START_EMAIL_LOG();
        }
        return;
    }

    if ( g_gonderilemeyen_listesi.isEmpty() EQ false ) {
        QString message = QObject::tr("There was an error at e-mails when sending e-mail.");//E-mail gönderimi esnasında aşagıdaki e-mail(ler)de hata oluştu.

        for ( int i = 0 ; i < g_gonderilemeyen_listesi.size() ; i++ ) {
            message.append("\n" + g_gonderilemeyen_listesi.at(i));
        }

        ADAK_ERROR(message,NULL,NULL);
        if (add_to_log EQ false ) {
            EMAIL_SENDER::START_EMAIL_LOG();
        }
        return;
    }

    if ( g_gonderilen_listesi.isEmpty() EQ false ) {
        if( p_uyari_mesaj.isEmpty() EQ false ) {
            ADAK_INFO( p_uyari_mesaj, NULL, NULL );
        }
        if (add_to_log EQ false ) {
            EMAIL_SENDER::START_EMAIL_LOG();
        }
        return;
    }

    ADAK_INFO(QObject::tr("There was an error can not be determined."),NULL,NULL);//Tespit edilemeyen hata oluştu.
}

/**************************************************************************************
                   EMAIL_SET_PORT
***************************************************************************************/

void EMAIL_SET_PORT ( int port )
{
    EMAIL_SENDER::SET_PORT ( port );
}

/**************************************************************************************
                   EMAIL_SET_EMAIL_ADRESI_FROM
***************************************************************************************/

void EMAIL_SET_EMAIL_ADRESI_FROM ( QString email_adresi_from )
{
    EMAIL_SENDER::SET_EMAIL_ADRESI_FROM ( email_adresi_from );
}

/**************************************************************************************
                   EMAIL_SET_EMAIL_ADRESI_TO
***************************************************************************************/

void EMAIL_SET_EMAIL_ADRESI_TO ( QString email_adresi_to )
{
    EMAIL_SENDER::SET_EMAIL_ADRESI_TO ( email_adresi_to );
}

/**************************************************************************************
                   EMAIL_SET_GONDERENIN_GERCEK_ISMI
***************************************************************************************/

void EMAIL_SET_GONDERENIN_GERCEK_ISMI ( QString gonderenin_gercek_ismi )
{
    EMAIL_SENDER::SET_GONDERENIN_GERCEK_ISMI ( gonderenin_gercek_ismi );
}

/**************************************************************************************
                   EMAIL_SET_EMAIL_SERVER_ADRESS
***************************************************************************************/

void EMAIL_SET_EMAIL_SERVER_ADRESS ( QString mail_server_adress )
{
    EMAIL_SENDER::SET_EMAIL_SERVER_ADRESS ( mail_server_adress );
}

/**************************************************************************************
                   EMAIL_SET_LOGIN_NAME
***************************************************************************************/

void EMAIL_SET_LOGIN_NAME ( QString login_name )
{
    EMAIL_SENDER::SET_LOGIN_NAME ( login_name );
}

/**************************************************************************************
                   EMAIL_SET_LOGIN_PASSWORD
***************************************************************************************/

void EMAIL_SET_LOGIN_PASSWORD ( QString login_password )
{
    EMAIL_SENDER::SET_LOGIN_PASSWORD ( login_password );
}

/**************************************************************************************
                   EMAIL_SET_PROTOCOL
***************************************************************************************/

void EMAIL_SET_PROTOCOL ( SSL_PROTOCOL protocol )
{
    EMAIL_SENDER::SET_PROTOCOL ( protocol );
}

/**************************************************************************************
                   EMAIL_SET_PROTOCOL
***************************************************************************************/

void EMAIL_SET_TOPLU_GONDER_FLAG(bool value)
{
    EMAIL_SENDER::SET_TOPLU_GONDER_FLAG(value);
}

/**************************************************************************************
                   EMAIL_SET_EMAIL_SERVER
***************************************************************************************/

void EMAIL_SET_EMAIL_SERVER ( QString mail_server_adress, int port, SSL_PROTOCOL protocol )
{
    EMAIL_SENDER::SET_EMAIL_SERVER ( mail_server_adress,port,protocol );
}

/**************************************************************************************
                   EMAIL_SET_USER_EMAIL_INFO
***************************************************************************************/

void EMAIL_SET_USER_EMAIL_INFO ( QString login_name, QString login_password,QString gonderenin_gercek_ismi, bool need_authentication)
{
    EMAIL_SENDER::SET_USER_EMAIL_INFO ( login_name,login_password,gonderenin_gercek_ismi,need_authentication);
}

/**************************************************************************************
                   EMAIL_GET_PORT
***************************************************************************************/

int EMAIL_GET_PORT()
{
    return EMAIL_SENDER::GET_PORT();
}

/**************************************************************************************
                   EMAIL_GET_EMAIL_ADRESI_FROM
***************************************************************************************/

QString EMAIL_GET_EMAIL_ADRESI_FROM()
{
    return EMAIL_SENDER::GET_EMAIL_ADRESI_FROM();
}

/**************************************************************************************
                   EMAIL_GET_EMAIL_ADRESI_TO
***************************************************************************************/

QString EMAIL_GET_EMAIL_ADRESI_TO()
{
    return EMAIL_SENDER::GET_EMAIL_ADRESI_TO();
}

/**************************************************************************************
                   EMAIL_GET_GONDERENIN_GERCEK_ISMI
***************************************************************************************/

QString EMAIL_GET_GONDERENIN_GERCEK_ISMI()
{
    return EMAIL_SENDER::GET_GONDERENIN_GERCEK_ISMI();
}

/**************************************************************************************
                   EMAIL_GET_EMAIL_SERVER_ADRESS
***************************************************************************************/

QString EMAIL_GET_EMAIL_SERVER_ADRESS()
{
    return EMAIL_SENDER::GET_EMAIL_SERVER_ADRESS();
}

/**************************************************************************************
                   EMAIL_GET_LOGIN_NAME
***************************************************************************************/

QString EMAIL_GET_LOGIN_NAME()
{
    return EMAIL_SENDER::GET_LOGIN_NAME();
}

/**************************************************************************************
                   EMAIL_GET_LOGIN_PASSWORD
***************************************************************************************/

QString EMAIL_GET_LOGIN_PASSWORD()
{
    return EMAIL_SENDER::GET_LOGIN_PASSWORD();
}

/**************************************************************************************
                   EMAIL_GET_PROTOCOL
***************************************************************************************/

SSL_PROTOCOL EMAIL_GET_PROTOCOL()
{
    return EMAIL_SENDER::GET_PROTOCOL();
}

/**************************************************************************************
                   GET_TIME_OFFSET
***************************************************************************************/

QString GET_TIME_OFFSET()
{
    QDateTime date_zone      = QDateTime::currentDateTime();
    QDateTime date_universal = date_zone.toUTC();

    date_zone.setTimeSpec ( Qt::UTC );

    int offset = date_universal.secsTo ( date_zone ) / 3600;

    QTime time;
    time.setHMS ( offset,0,0,0 );

    if ( offset > 0 ) {
        return QString ( "+%1" ).arg ( time.toString ( "hhmm" ) );
    }

    return QString ( "%1" ).arg ( time.toString ( "hhmm" ) );
}






