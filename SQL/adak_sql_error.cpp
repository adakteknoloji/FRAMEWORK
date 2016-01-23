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
            error_type_text = QObject::tr("CONNECTION ERROR");
            aciklama   = QObject::tr("Database connection failed.");//Veritabanı bağlantısı kurulamadı
            switch( error_in_function ) {
                case ERROR_IN_CONSTRUCTOR :
                {
                    error_text += QObject::tr("An error was returned from the ADAK_SQL function.");//ADAK_SQL kurucu fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_CREATE_DATABASE :
                {
                    error_text += QObject::tr("An error was returned from the CREATE_DATABASE function.");//CREATE_DATABASE fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_CREATE_TABLE :
                {
                    error_text += QObject::tr("An error was returned from the CREATE_TABLE function.");//CREATE_TABLE fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_DROP_DATABASE :
                {
                    error_text += QObject::tr("An error was returned from the DROP_DATABASE function.");//DROP_DATABASE fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_DROP_TABLES :
                {
                    error_text += QObject::tr("An error was returned from the DROP_TABLES function.");//DROP_TABLES fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_USER_EXISTS :
                {
                    error_text += QObject::tr("An error was returned from the USER_EXISTS function.");//USER_EXISTS fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_CREATE_USER :
                {
                    error_text += QObject::tr("An error was returned from the CREATE_USER function.");//CREATE_USER fonksiyonundan hata döndürüldü.
                    break;
                }
                default :
                {
                    error_text += QObject::tr("An error was returned from the unknown function.");
                    break;
                }
            }
            break;
        }
        case CREATION_ERROR :
        {
            error_type_text = QObject::tr("CREATION ERROR");
            switch (error_in_function ) {
                case ERROR_IN_CONSTRUCTOR :
                {
                    error_text += QObject::tr("An error was returned from the ADAK_SQL function.");//ADAK_SQL kurucu fonksiyonundan hata döndürüldü.
                    break;
                }
                case  ERROR_IN_CREATE_DATABASE :
                {
                    error_text += QObject::tr("An error was returned from the CREATE_DATABASE function. ");//CREATE_DATABASE fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_CREATE_TABLE :
                {
                    error_text += QObject::tr("An error was returned from the CREATE_TABLE function.");//CREATE_TABLE fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_CREATE_USER :
                {
                    error_text += QObject::tr("An error was returned from the CREATE_USER function.");//CREATE_USER fonksiyonundan hata döndürüldü
                    break;
                }
                default :
                {
                    error_text += QObject::tr("An error was returned from the unknown function.");
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
                    error_text = QObject::tr("An error was returned from the START_TRANSACTION() function.");//START_TRANSACTION() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("TRANSACTION to start desired when TRANSACTION opened. Intertwined TRANSACTION's can not be opened.");//TRANSACTION açıkken yeni TRANSACTION başlatılmak istendi. İç içe TRANSACTION ' lar olamaz
                    break;
                }
                case ERROR_IN_COMMIT_TRANSACTION :
                {
                    error_text = QObject::tr("An error was returned from the COMMIT_TRANSACTION() function.");//COMMIT_TRANSACTION() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("COMMIT_TRANSACTION was called without TRANSACTION.");//TRANSACTION yokken COMMIT_TRANSACTION çağrıldı
                    break;
                }
                case ERROR_IN_CANCEL_TRANSACTION :
                {
                    error_text = QObject::tr("An error was returned from the CANCEL_TRANSACTION() function.");//CANCEL_TRANSACTION() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("CANCEL_TRANSACTION was called without TRANSACTION.");//TRANSACTION yokken CANCEL_TRANSACTION çağrıldı
                    break;
                }
                case ERROR_IN_PREPARE_SELECT :
                {
                    error_text = QObject::tr("An error was returned from the PREPARE_SELECT() function.");//PREPARE_SELECT() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("Meantime SELECT, you can use PREPARE_LIMIT SELECT for LIMIT.");//SELECT esnasinda LIMIT icin PREPARE_LIMIT_SELECT kullanmalisiniz
                    break;
                }
                case ERROR_IN_PREPARE_INSERT :
                {
                    error_text = QObject::tr("An error was returned from the PREPARE_INSERT() function.");//PREPARE_INSERT() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("This operation can not be performed before the transaction starts.");//Transaction başlatılmadan bu işlem gerçekleştirilemez
                    break;
                }
                case ERROR_IN_PREPARE_UPDATE :
                {
                    error_text = QObject::tr("An error was returned from the PREPARE_UPDATE() function.");//PREPARE_UPDATE() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("This operation can not be performed before the transaction starts.");//Transaction başlatılmadan bu işlem gerçekleştirilemez
                    break;
                }
                case ERROR_IN_PREPARE_DELETE :
                {
                    error_text = QObject::tr("An error was returned from the PREPARE_DELETE() function.");//PREPARE_DELETE() fonksiyonundan hata döndürüldü"
                    aciklama   = QObject::tr("This operation can not be performed before the transaction starts.");//Transaction başlatılmadan bu işlem gerçekleştirilemez
                    break;
                }
                case ERROR_IN_INSERT :
                {
                    error_text = QObject::tr("An error was returned from the INSERT() function.");//INSERT() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("This operation can not be performed before the transaction starts.");//Transaction başlatılmadan bu işlem gerçekleştirilemez
                    break;
                }
                case ERROR_IN_UPDATE :
                {
                    error_text = QObject::tr("An error was returned from the UPDATE() function.");//UPDATE() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("This operation can not be performed before the transaction starts.");//Transaction başlatılmadan bu işlem gerçekleştirilemez
                    break;
                }
                case ERROR_IN_DELETE :
                {
                    error_text = QObject::tr("An error was returned from the DELETE()function.");//DELETE() fonksiyonundan hata döndürüldü
                    aciklama   = QObject::tr("This operation can not be performed before the transaction starts.");//Transaction başlatılmadan bu işlem gerçekleştirilemez
                    break;
                }
                default :
                    error_text = QObject::tr("An error was returned from the unknown function.");//Bilinmeyen bir fonksiyonda hata döndürüldü.
                    break;
            }
            break;
        }
        case SQL_QUERY_ERROR :
        {
            error_type_text = QObject::tr("SQL QUERY ERROR");
            error_text     += database.lastError().text();
            switch ( error_in_function ) {
                case ERROR_IN_INSERT :
                {
                    aciklama     = QObject::tr("INSERT FUNCTION ERROR \n");
                    break;
                }
                case ERROR_IN_UPDATE :
                {
                    aciklama     = QObject::tr("UPDATE FUNCTION ERROR \n");
                    break;
                }
                case ERROR_IN_DELETE :
                {
                    aciklama     = QObject::tr("DELETE FUNCTION ERROR \n");
                    break;
                }
                case ERROR_IN_SELECT :
                {
                    aciklama     = QObject::tr("SELECT FUNCTION ERROR \n");
                    break;
                }
                case ERROR_IN_SET_TABLE_ID :
                {
                    aciklama     = QObject::tr("The table name not found.");//Belirtilen tablo adı bulunamadı.
                    break;
                }
                case ERROR_IN_GET_COLUMN_TYPE :
                {
                    aciklama     = QObject::tr("The column name found in SET_VALUE function.");//SET_VALUE() fonksiyonunda belirtilen kolon adı bulunamadı.
                    break;
                }
                case ERROR_IN_FIND_TABLE_ID :
                {
                    aciklama   = QObject::tr("An error was returned from FIND_TABLE_ID function.");//FIND_TABLE_ID fonksiyonundan hata döndürüldü.
                    break;
                }
                case ERROR_IN_VALUE         :
                {
                    aciklama     = QObject::tr("VALUE() Error. Query NEXT() forgotten or x is wrong in VALUE(x).");//VALUE()'da hata. Ya NEXT() unutulmus ya da VALUE(x) teki x yanlis"
                    break;
                }
                case ERROR_IN_SET_VALUE_EXTRA     :
                {
                    aciklama     = QObject::tr("SET_VALUE has been used to unnecessary or incorrect domain name.");//SET_VALUE gereksiz veya yanlis alan adi ile kullanilmis
                    break;
                }
                case ERROR_IN_SET_VALUE_MISSING   :
                {
                    aciklama     = QObject::tr("To assign SET_VALUE forgotten.");//SET_VALUE atamasi unutulmus.
                    break;
                }
                case ERROR_IN_NEXT          :
                {
                    aciklama     = QObject::tr("Query is not SELECT(), or SELECT () is called before the NEXT () was called.");//Ya sorgu SELECT degil, yada SELECT() cagrilmadan NEXT() cagrildi
                    break;
                }
                default :
                    aciklama = QObject::tr("An error was returned from the unknown function.");//Bilinmeyen bir fonksiyondan hata döndürüldü
                    break;
            }
            break;
        }
        case EXEC_DDL_ERROR :
        {
            error_type_text  = QObject::tr("EXEC DDL ERROR");
            break;
        }
        case DRIVER_TYPE_ERROR :
        {
            error_type_text = "DRIVER_TYPE_ERROR";
            error_text      = QObject::tr("UNKNOWN DRIVER");
            aciklama        = QObject::tr("ADAK_SQL does not support this SQL engine.\n");//ADAK_SQL belirttiginiz SQL motorunu desteklemiyor!\n
                              QObject::tr("Supported: PostgreSQL, MySQL, SQLite, Microsoft SQL , ORACLE.");//Desteklenenler: PostgreSQL, MySQL, SQLite, Microsoft SQL , ORACLE
            break;
        }
        default :
            error_type_text = "UNKNOWN TYPE OF ERROR";
            error_text      = QObject::tr("UNKNOWN ERROR TYPE.");
            aciklama        = QObject::tr("ERROR TYPE IS NOT DETECTED.");
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
    cerr << QObject::tr("Used Database           :").toStdString() << db_name.toStdString()                          << endl;
    cerr << QObject::tr("Server                  :").toStdString() << hostname.toStdString()                         << endl;
    cerr << QObject::tr("SQL Engine              :").toStdString() << driver_name.toStdString()                      << endl;
    cerr << QObject::tr("Operating System        :").toStdString() << os.toStdString()                               << endl;
    cerr << QObject::tr("Error Message           :").toStdString() << error_text.toStdString()                       << endl;
    cerr << QObject::tr("Error Type              :").toStdString() << error_type.toStdString()                       << endl;
    cerr << QObject::tr("Error No                :").toStdString() << error_number                                   << endl;
    cerr << QObject::tr("Comment                 :").toStdString() << aciklama.toStdString()                         << endl;
    if ( hatali_query NE NULL ) {
        cerr << QObject::tr("Incorrect Query         :").toStdString()  << hatali_query.toStdString()                << endl;
    }
    cerr << QObject::tr("Error Date              :").toStdString()  << QDate::currentDate().toString().toStdString() << endl;
    cerr << QObject::tr("Error Time              :").toStdString()  << QTime::currentTime().toString().toStdString() << endl;
    cerr << "*************************************************************************************" << endl;
}
