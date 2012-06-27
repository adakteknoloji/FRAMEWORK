#include <iostream>
#include <stdlib.h>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QTime>

#include "adak_defines.h"
#include "adak_utils.h"
#include "adak_sql_error.h"

using std::cerr;
using std::endl;



void ( * SQL_ERROR_FUNCTION )( const QString& hostname    , const QString& db_name ,
                               const QString& driver_name , const QString& os ,
                               int   error_number     , const QString& error_type,
                               const QString& error_text  , const QString& aciklama, const QString& hatali_query ) = NULL;

/********************************************/
/*              ADAK_SQL_ERROR             */
/********************************************/

void ADAK_SQL_ERROR ( QSqlDatabase database ,         int         error_type,
                       int          error_in_function, QSqlQuery * query,
                       QString      extra_info_text)
{
    if (SQL_ERROR_FUNCTION EQ NULL) {
        SQL_ERROR_FUNCTION = ADAK_SQL_CONSOLE_ERROR;
    }

    QString error_type_text = "";
    QString aciklama = "";
    QString error_text ="";
    switch ( error_type ) {
        case CONNECTION_ERROR :
        {
            error_type_text = "CONNECTION ERROR";
            aciklama   = "Veritabanı bağlantısı kurulamadı";
            switch( error_in_function ) {
                case ERROR_IN_CONSTRUCTOR :
                {
                    error_text += "ADAK_SQL kurucu fonksiyonundan hata döndürüldü. ";
                    break;
                }
                case ERROR_IN_CREATE_DATABASE :
                {
                    error_text += "CREATE_DATABASE fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_CREATE_TABLE :
                {
                    error_text += "CREATE_TABLE fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_DROP_DATABASE :
                {
                    error_text += "DROP_DATABASE fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_DROP_TABLES :
                {
                    error_text += "DROP_TABLES fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_USER_EXISTS :
                {
                    error_text += "USER_EXISTS fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_CREATE_USER :
                {
                    error_text += "CREATE_USER fonksiyonundan hata döndürüldü.";
                    break;
                }
                default :
                {
                    error_text += "Bilinmeyen bir fonksiyonundan hata döndürüldü.";
                    break;
                }
            }
            break;
        }
        case CREATION_ERROR :
        {
            error_type_text = "CREATION ERROR";
            switch (error_in_function ) {
                case ERROR_IN_CONSTRUCTOR :
                {
                    error_text += "ADAK_SQL kurucu fonksiyonundan hata döndürüldü. ";
                    break;
                }
                case  ERROR_IN_CREATE_DATABASE :
                {
                    error_text += "CREATE_DATABASE fonksiyonundan hata döndürüldü. ";
                    break;
                }
                case ERROR_IN_CREATE_TABLE :
                {
                    error_text += "CREATE_TABLE fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_CREATE_USER :
                {
                    error_text += "CREATE_USER fonksiyonundan hata döndürüldü.";
                    break;
                }
                default :
                {
                    error_text += "Bilinmeyen bir fonksiyonundan hata döndürüldü.";
                    break;
                }
            }
            break;
        }
        case TRANSACTION_ERROR :
        {
            error_type_text = "TRANSACTION ERROR";
            switch( error_in_function ) {
                case ERROR_IN_START_TRANSACTION :
                {
                    error_text = "START_TRANSACTION() fonksiyonundan hata döndürüldü";
                    aciklama   = "TRANSACTION açıkken yeni TRANSACTION başlatılmak istendi. İç içe TRANSACTION ' lar olamaz";
                    break;
                }
                case ERROR_IN_COMMIT_TRANSACTION :
                {
                    error_text = "COMMIT_TRANSACTION() fonksiyonundan hata döndürüldü";
                    aciklama   = "TRANSACTION yokken COMMIT_TRANSACTION çağrıldı";
                    break;
                }
                case ERROR_IN_CANCEL_TRANSACTION :
                {
                    error_text = "CANCEL_TRANSACTION() fonksiyonundan hata döndürüldü";
                    aciklama   = "TRANSACTION yokken CANCEL_TRANSACTION çağrıldı";
                    break;
                }
                case ERROR_IN_PREPARE_SELECT :
                {
                    error_text = "PREPARE_SELECT() fonksiyonundan hata döndürüldü";
                    aciklama   = "SELECT esnasinda LIMIT icin PREPARE_LIMIT_SELECT kullanmalisiniz";
                    break;
                }
                case ERROR_IN_PREPARE_INSERT :
                {
                    error_text = "PREPARE_INSERT() fonksiyonundan hata döndürüldü";
                    aciklama   = "Transaction başlatılmadan bu işlem gerçekleştirilemez";
                    break;
                }
                case ERROR_IN_PREPARE_UPDATE :
                {
                    error_text = "PREPARE_UPDATE() fonksiyonundan hata döndürüldü";
                    aciklama   = "Transaction başlatılmadan bu işlem gerçekleştirilemez";
                    break;
                }
                case ERROR_IN_PREPARE_DELETE :
                {
                    error_text = "PREPARE_DELETE() fonksiyonundan hata döndürüldü";
                    aciklama   = "Transaction başlatılmadan bu işlem gerçekleştirilemez";
                    break;
                }
                case ERROR_IN_INSERT :
                {
                    error_text = "INSERT() fonksiyonundan hata döndürüldü";
                    aciklama   = "Transaction başlatılmadan bu işlem gerçekleştirilemez";
                    break;
                }
                case ERROR_IN_UPDATE :
                {
                    error_text = "UPDATE() fonksiyonundan hata döndürüldü";
                    aciklama   = "Transaction başlatılmadan bu işlem gerçekleştirilemez";
                    break;
                }
                case ERROR_IN_DELETE :
                {
                    error_text = "DELETE() fonksiyonundan hata döndürüldü";
                    aciklama   = "Transaction başlatılmadan bu işlem gerçekleştirilemez";
                    break;
                }
                default :
                    error_text = "Bilinmeyen bir fonksiyondan hata döndürüldü";
                    break;
            }
            break;
        }
        case SQL_QUERY_ERROR :
        {
            error_type_text = "SQL QUERY ERROR";
            error_text     += database.lastError().text();
            switch ( error_in_function ) {
                case ERROR_IN_INSERT :
                {
                    aciklama     = "INSERT FONKSIYONUNDA HATA \n";
                    break;
                }
                case ERROR_IN_UPDATE :
                {
                    aciklama     = "UPDATE FONKSIYONUNDA HATA \n";
                    break;
                }
                case ERROR_IN_DELETE :
                {
                    aciklama     = "DELETE FONKSIYONUNDA HATA \n";
                    break;
                }
                case ERROR_IN_SELECT :
                {
                    aciklama     = "SELECT FONKSIYONUNDA HATA \n";
                    break;
                }
                case ERROR_IN_SET_TABLE_ID :
                {
                    aciklama     = "Belirtilen tablo adı bulunamadı ";
                    break;
                }
                case ERROR_IN_GET_COLUMN_TYPE :
                {
                    aciklama     = "SET_VALUE() fonksiyonunda belirtilen kolon adı bulunamadı ";
                    break;
                }
                case ERROR_IN_FIND_TABLE_ID :
                {
                    aciklama   = "FIND_TABLE_ID fonksiyonundan hata döndürüldü.";
                    break;
                }
                case ERROR_IN_VALUE         :
                {
                    aciklama     = "VALUE()'da hata. Ya NEXT() unutulmus ya da VALUE(x) teki x yanlis";
                    break;
                }
                case ERROR_IN_SET_VALUE_EXTRA     :
                {
                    aciklama     = "SET_VALUE gereksiz veya yanlis alan adi ile kullanilmis";
                    break;
                }
                case ERROR_IN_SET_VALUE_MISSING   :
                {
                    aciklama     = "SET_VALUE atamasi unutulmus";
                    break;
                }
                case ERROR_IN_NEXT          :
                {
                    aciklama     = "Ya sorgu SELECT degil, yada SELECT() cagrilmadan NEXT() cagrildi";
                    break;
                }
                default :
                    aciklama = "Bilinmeyen bir fonksiyondan hata döndürüldü";
                    break;
            }
            break;
        }
        case EXEC_DDL_ERROR :
        {
            error_type_text  = "EXEC DDL ERROR";
            break;
        }
        case DRIVER_TYPE_ERROR :
        {
            error_type_text = "DRIVER_TYPE_ERROR";
            error_text      = "UNKNOWN DRIVER";
            aciklama        = "ADAK_SQL belirttiginiz SQL motorunu desteklemiyor!\n"
                              "Desteklenenler: PostgreSQL, MySQL, SQLite, Microsoft SQL , ORACLE";
            break;
        }
        default :
            error_type_text = "UNKNOWN TYPE OF ERROR";
            error_text      = "HATA TIPI BELIRSIZ";
            aciklama        = "HATA TESPİT EDİLEMEDİ";
            break;
    }
    QString query_text = "";
    int     error_number = -1;
    if (query NE NULL) {
        error_number = query->lastError().number();
        query_text += "<br><br>" + query->lastQuery() + "<br><br>" + query->executedQuery() + "<br><br>" ;
        query_text += "*********";
        query_text += query->lastError().text();
        query_text += "*********<br><br>";
    }
    query_text += "#########";
    query_text += database.lastError().text();
    query_text += "#########<br><br>";
    query_text += extra_info_text;
    SQL_ERROR_FUNCTION ( database.hostName(), database.databaseName(),
                         database.driverName(),
                         ADAK_ISLETIM_SISTEMI(),
                         error_number,
                         error_type_text, error_text, aciklama, query_text);
    exit (99); // abort();
}


/************************************************/
/*           SET_SQL_ERROR_FUNC                 */
/************************************************/

void SET_SQL_ERROR_FUNC( void ( * error_func_address ) ( const QString& hostname    , const QString& db_name  ,
                                                         const QString& driver_name , const QString& os ,
                                                         int   error_number      , const QString& error_type,
                                                         const QString& error_text  , const QString& aciklama , const QString& hatali_query ) )
{
    SQL_ERROR_FUNCTION = error_func_address;
}

/**************************************************/
/*            ADAK_CONSOLE_ERROR                 */
/**************************************************/

void ADAK_SQL_CONSOLE_ERROR   ( const QString& hostname    ,    const QString& db_name,
                                 const QString& driver_name ,    const QString& os ,
                                 int   error_number,        const QString& error_type,
                                 const QString& error_text ,     const QString& aciklama,   const QString& hatali_query )
{
    cerr << "************************************************************************************"  << endl;
    cerr << "Kullanılan Veritabanı   :" << db_name.toStdString()                          << endl;
    cerr << "Sunucu                  :" << hostname.toStdString()                         << endl;
    cerr << "SQL Motoru              :" << driver_name.toStdString()                      << endl;
    cerr << "İşletim Sistemi         :" << os.toStdString()                               << endl;
    cerr << "Hata Mesajı             :" << error_text.toStdString()                       << endl;
    cerr << "Hata Türü               :" << error_type.toStdString()                       << endl;
    cerr << "Hata Numarası           :" << error_number                                   << endl;
    cerr << "Açıklama                :" << aciklama.toStdString()                         << endl;
    if ( hatali_query NE NULL ) {
        cerr << "Hatalı Sorgu            :"  << hatali_query.toStdString()                << endl;
    }
    cerr << "Hata Tarihi             :"  << QDate::currentDate().toString().toStdString() << endl;
    cerr << "Hata Saati              :"  << QTime::currentTime().toString().toStdString() << endl;
    cerr << "*************************************************************************************" << endl;
}
