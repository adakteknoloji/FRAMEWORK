#include <QtCore/QCoreApplication>
#include <QTextCodec>
#include <iostream>
#include "adak_sql.h"
#include "adak_std.h"
#include "adak_sql_struct.h"
#include "adak_utils.h"
//#include "yonetim_db.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //INIT_ADAKLIB (argc, argv, &yonetim_db);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString   hedef_firma_kodu      = argv[1];
    QString   sunucu                = argv[2];
    QString   database_username     = argv[3];
    QString   sql_port_number       = argv[4];
    DB_DRIVER db_driver             = GET_DB_DRIVER( argv[5] );
    QString database_user_pass      = "";

//    if ( argc < 6 ) {
//        cout << endl << endl << endl ;
//        cout << "********************************     EKSİK PARAMETRE GİRİLDİ!!!     ********************************" << endl;
//        cout << endl << endl ;
//        cout << "Sırasıyla : Hedef firmanın kodu | Sunucu | Veritabanı kullanıcısı | Sql Port Numarası | Sql Motoru";
//        cout << endl << "şeklindeki parametreleri giriniz." << endl;
//        return 0;
//    }
//
//    if ( db_driver EQ UNKNOWN ) {
//        cout << " Geçersiz SQL Motoru parametresi " << endl;
//        return 0;
//    }

    string password;
    cout << database_username.toStdString() <<" için veritabanı kullanıcı parolası : " << endl;
    cin.unsetf(ios::skipws);
    //cin >> password;
    //if( cin.good()) {
        database_user_pass = QString( password.c_str());
    //}

    //if ( IS_DB_EXISTS ( "222", "localhost", "root", "", QVariant(11).toInt(), db_driver, 0 ) EQ false ) {
        cout << "adak_" << hedef_firma_kodu.toStdString()  <<  " isimli veritabanı bulunamadı " << endl;
        return 0;
    //}

    //G_YONETIM_DB = CONNECT_DATABASE ( &yonetim_db , "" , sunucu , database_username ,database_user_pass , QVariant(sql_port_number).toInt(), db_driver );

    ADD_ADAK_STD();

    //delete G_YONETIM_DB;
    return 0;
}
