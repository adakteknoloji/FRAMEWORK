#include <QtCore/QCoreApplication>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <stdlib.h>
#include <QStringList>
#include <iostream>
#include "adak_sql_struct.h"
#include "adak_utils.h"
#include "adak_sql.h"
#include "update_db.h"

using namespace std;

ADAK_SQL * MODUL_DB    = NULL;

/*****************************/
/*           main            */
/*****************************/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    a.addLibraryPath( a.applicationDirPath() );
    a.addLibraryPath( a.applicationDirPath() + "/plugins" );

    QString   database_adi          = argv[1];

    QString   sunucu                = argv[2];
    SET_SQL_HOSTNAME (sunucu);

    QString   database_username     = argv[3];
    SET_SQL_USERNAME (database_username);

    QString   sql_port_number       = argv[4];
    SET_SQL_PORT (sql_port_number.toInt());

    SET_SQL_MOTORU (argv[5]);

    QString   full_file_path        = argv[6];
    QString   database_user_pass    = "";
    int       is_yonetim_db         = 0;
    QString   firma_kodu;
    QString   adak_adi;

    if ( argc NE 7 ) {
        cout << endl << endl << endl ;
        cout << "********************************     EKSİK VEYA YANLIŞ PARAMETRE GİRİLDİ!!!     ********************************" << endl;
        cout << endl << endl ;
        cout << "Sırasıyla : Database adı | Sunucu | Veritabanı kullanıcısı | Sql Port Numarası | Sql Motoru | INCLUDE_DOSYASI_TAM_YOLU";
        cout << endl << "şeklindeki parametreleri giriniz." << endl;
        return 0;
    }

    adak_adi  = database_adi.mid(0,6);

    if ( adak_adi NE "adak_") {
        cout << "Hatalı veritabanı ismi. Veritabanı ismi adak_<firma_kodu> şeklinde olmalıdır.";
        return 0;
    }
    firma_kodu = database_adi.mid(6);

    if ( firma_kodu EQ "yonetim") {
        firma_kodu.clear();
        is_yonetim_db = 1;
    }
    string password;
    cout << database_username.toStdString() <<" için veritabanı kullanıcı parolası : " << endl;
    cin.unsetf(ios::skipws);
    cin >> password;
    if( cin.good()) {
        database_user_pass = QString( password.c_str());
    }

    cout << "Girilen veritabanı ismi  : " << database_adi.toStdString() << endl;
    cout << "Sunucu                   : " << sunucu.toStdString() << endl;
    cout << "Veritabanı kullanıcısı   : " << database_username.toStdString() << endl;
    cout << "SQL Port Numarası        : " << sql_port_number.toStdString() << endl;
    cout << "SQL Driver               : " << argv[5] << endl;
    cout << "Girilen include dosyası  :" << full_file_path.toStdString() << endl << endl;

    QFile * file = new QFile ( full_file_path );
    if ( file->open(QIODevice::ReadOnly|QIODevice::Text) EQ false) {
        cout << "Dosya okunamadı";
        return 0;
    }
    QTextStream * in = new QTextStream(file);

    int         okunan_satir_sayisi = 0;
    QString     okunan_satir;
    QStringList OKUNAN_SATIR_DIZISI;
    QStringList OKUNAN_KOLON_DIZISI;
    QStringList OKUNAN_INDEX_DIZISI;

    QString     struct_name;

    QString     table_name;
    int         table_count;

    QString     column_name;
    int         column_count;
    QString     column_type;
    QString     column_length;
    QString     column_is_array;
    QString     column_array_length;
    QString     column_is_auto_increment;
    QString     column_is_not_null;

    int         index_count;
    QString     index_type;
    int         index_column_count;
    QString     indexed_columns;

    int         first_index;
    int         last_index;

    for ( ; ; ) {
        okunan_satir = in->readLine();
        okunan_satir_sayisi++;
        if ( okunan_satir_sayisi EQ 4 ) {
            break;
        }
    }
    OKUNAN_SATIR_DIZISI  = okunan_satir.split(" ");
    okunan_satir         = in->readLine();
    OKUNAN_SATIR_DIZISI  = okunan_satir.split(" ", QString::SkipEmptyParts);
    okunan_satir         = OKUNAN_SATIR_DIZISI.at(0);
    OKUNAN_SATIR_DIZISI  = okunan_satir.split("\"");
    struct_name          = OKUNAN_SATIR_DIZISI.at(1);
    okunan_satir         = in->readLine();
    first_index          = okunan_satir.indexOf(QRegExp("(\\d+)"), 0 );
    last_index           = okunan_satir.indexOf(",", 0 );
    table_count          = QVariant(okunan_satir.mid(first_index, last_index - first_index )).toInt();
    okunan_satir         = in->readLine();

    ADAK_SQL_STRUCT  * modul_struct;

    modul_struct = (ADAK_SQL_STRUCT *) MALLOC (sizeof (ADAK_SQL_STRUCT));

    modul_struct->name = ( char * ) MALLOC( struct_name.size() + 1 );

    strcpy ( modul_struct->name, struct_name.toLatin1().data() );

    modul_struct->table_count = table_count;

    for ( int i = 0; i < table_count; i++ ) {

        okunan_satir         = in->readLine();
        okunan_satir         = in->readLine();
        OKUNAN_SATIR_DIZISI  = okunan_satir.split( "\"", QString::SkipEmptyParts );
        table_name           = OKUNAN_SATIR_DIZISI.at(1);
        okunan_satir         = in->readLine();
        first_index          = okunan_satir.indexOf(QRegExp("(\\d+)"), 0 );
        last_index           = okunan_satir.indexOf(",", 0 );
        column_count         = QVariant(okunan_satir.mid(first_index, last_index - first_index )).toInt();
        okunan_satir         = in->readLine();

        modul_struct->tables[i].column_count = column_count;

        modul_struct->tables[i].name         = ( char * ) MALLOC ( table_name.size() + 1 );

        strcpy ( modul_struct->tables[i].name, table_name.toLatin1().data() );

        for ( int j = 0; j < column_count; j++ ) {

            okunan_satir             = in->readLine();
            first_index              = okunan_satir.indexOf("{") + 1;
            last_index               = okunan_satir.indexOf("}");
            okunan_satir             = okunan_satir.mid(first_index , last_index - first_index );
            OKUNAN_KOLON_DIZISI      = okunan_satir.split(",", QString::SkipEmptyParts);
            first_index              = OKUNAN_KOLON_DIZISI.at(0).indexOf("\"");
            last_index               = OKUNAN_KOLON_DIZISI.at(0).indexOf("\"",first_index + 1 );
            column_name              = OKUNAN_KOLON_DIZISI.at(0).mid(first_index + 1 , last_index - first_index -1 );

            first_index              = FIND_FIRST_INDEX ( OKUNAN_KOLON_DIZISI.at(1)              );
            last_index               = FIND_LAST_INDEX  ( first_index, OKUNAN_KOLON_DIZISI.at(1) );
            column_type              = OKUNAN_KOLON_DIZISI.at(1).mid( first_index , last_index - first_index + 1 );

            first_index              = FIND_FIRST_INDEX ( OKUNAN_KOLON_DIZISI.at(2)              );
            last_index               = FIND_LAST_INDEX  ( first_index, OKUNAN_KOLON_DIZISI.at(2) );
            column_length            = OKUNAN_KOLON_DIZISI.at(2).mid( first_index , last_index - first_index + 1 );

            first_index              = FIND_FIRST_INDEX ( OKUNAN_KOLON_DIZISI.at(3)              );
            last_index               = FIND_LAST_INDEX  ( first_index, OKUNAN_KOLON_DIZISI.at(3) );
            column_is_array          = OKUNAN_KOLON_DIZISI.at(3).mid( first_index , last_index - first_index  + 1 );

            first_index              = FIND_FIRST_INDEX ( OKUNAN_KOLON_DIZISI.at(4)              );
            last_index               = FIND_LAST_INDEX  ( first_index, OKUNAN_KOLON_DIZISI.at(4) );
            column_array_length      = OKUNAN_KOLON_DIZISI.at(4).mid( first_index , last_index - first_index + 1 );

            first_index              = FIND_FIRST_INDEX ( OKUNAN_KOLON_DIZISI.at(5)              );
            last_index               = FIND_LAST_INDEX  ( first_index, OKUNAN_KOLON_DIZISI.at(5) );
            column_is_auto_increment = OKUNAN_KOLON_DIZISI.at(5).mid( first_index , last_index - first_index + 1);

            first_index              = FIND_FIRST_INDEX ( OKUNAN_KOLON_DIZISI.at(6)              );
            last_index               = FIND_LAST_INDEX  ( first_index, OKUNAN_KOLON_DIZISI.at(6) );
            column_is_not_null       = OKUNAN_KOLON_DIZISI.at(6).mid( first_index , last_index - first_index + 1  );


            modul_struct->tables[i].columns[j].name = ( char * ) MALLOC( column_name.size() + 1 );

            strcpy ( modul_struct->tables[i].columns[j].name, column_name.toLatin1().data());

            modul_struct->tables[i].columns[j].type              = GET_COLUMN_TYPE(column_type);
            modul_struct->tables[i].columns[j].length            = QVariant(column_length).toInt();
            modul_struct->tables[i].columns[j].is_array          = QVariant(column_is_array).toBool();
            modul_struct->tables[i].columns[j].array_length      = QVariant(column_array_length).toInt();
            modul_struct->tables[i].columns[j].is_auto_increment = QVariant(column_is_auto_increment).toBool();
            modul_struct->tables[i].columns[j].is_not_null       = QVariant(column_is_not_null).toBool();

        }
        okunan_satir = in->readLine();
        okunan_satir = in->readLine();
        first_index  = okunan_satir.indexOf(QRegExp("(\\d+)"), 0 );
        last_index   = okunan_satir.indexOf(",", 0 );
        index_count  = QVariant(okunan_satir.mid(first_index, last_index - first_index )).toInt();
        okunan_satir = in->readLine();
        for ( int j = 0; j < index_count; j++ ) {

            okunan_satir           = in->readLine();
            first_index            = okunan_satir.indexOf("{") + 1;
            last_index             = okunan_satir.indexOf("}");
            last_index             = okunan_satir.indexOf("}", last_index + 1 );
            okunan_satir           = okunan_satir.mid(first_index , last_index - first_index );
            OKUNAN_INDEX_DIZISI    = okunan_satir.split(",", QString::SkipEmptyParts);
            first_index            = FIND_FIRST_INDEX ( OKUNAN_INDEX_DIZISI.at(0)              );
            last_index             = FIND_LAST_INDEX  ( first_index, OKUNAN_INDEX_DIZISI.at(0) );
            index_type             = OKUNAN_INDEX_DIZISI.at(0).mid ( first_index, last_index - first_index + 1 );

            first_index            = FIND_FIRST_INDEX ( OKUNAN_INDEX_DIZISI.at(1)              );
            last_index             = FIND_LAST_INDEX  ( first_index, OKUNAN_INDEX_DIZISI.at(1) );
            index_column_count     = QVariant(OKUNAN_INDEX_DIZISI.at(1).mid ( first_index, last_index - first_index + 1 )).toInt();

            first_index            = FIND_FIRST_INDEX ( OKUNAN_INDEX_DIZISI.at(2)              );
            last_index             = FIND_LAST_INDEX  ( first_index, OKUNAN_INDEX_DIZISI.at(2) );
            indexed_columns        = OKUNAN_INDEX_DIZISI.at(2).mid ( first_index, last_index - first_index + 1 );

            first_index            = indexed_columns.indexOf("{");
            last_index             = indexed_columns.indexOf("}");
            okunan_satir           = indexed_columns.mid(first_index + 1 , last_index - first_index -1 );
            QStringList dizi       = okunan_satir.split(",");
            QString     kolonlar;
            for ( int k = 0; k < index_column_count; k++ ) {
                first_index = dizi.at(k).indexOf("\"");
                last_index  = dizi.at(k).indexOf("\"",first_index + 1 );
                column_name = dizi.at(k).mid(first_index + 1 , last_index - first_index -1 );
                kolonlar.append(column_name + " ");

                modul_struct->tables[i].indexes[j].columns[k] = ( char * ) MALLOC ( column_name.size() + 1 );

                strcpy ( modul_struct->tables[i].indexes[j].columns[k], column_name.toLatin1().data());
            }

            modul_struct->tables[i].indexes[j].type = GET_INDEX_TYPE(index_type);
            modul_struct->tables[i].indexes[j].column_count = QVariant(indexed_columns).toInt();
        }
        modul_struct->tables[i].index_count  = index_count;
        in->readLine();
        in->readLine();
    }

    QList< ADAK_SQL_STRUCT * > db_struct_list;

    db_struct_list << modul_struct;

    ADAK_SQL * db_conn = CREATE_ADAK_SQL_CONNECTION( db_struct_list , database_adi, GET_SQL_DB_DRIVER_ENUM(SQL_MOTORU()));

    SET_SQL_USERNAME(database_username );
    SET_SQL_PASSWORD(database_user_pass);

    if ( db_conn->CONNECT_TO_DATABASE(false) EQ ADAK_FAIL ) {
        cout <<  database_adi.toStdString() + " veritabanı bulunamadı";
        return 0;
    }

    if ( db_conn->UPDATE_TABLES() EQ ADAK_FAIL ) {
        cout << "Veritabanı güncelenirken hata oluştu";
        return 0;
    }

    delete db_conn;
    cout<<"İşlem tamam"<<endl;
    return 0;
}


