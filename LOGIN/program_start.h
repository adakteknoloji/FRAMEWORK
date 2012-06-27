#ifndef PROGRAM_CORE_H
#define PROGRAM_CORE_H

#include <QList>

class QWidget;
class QString;
class QStringList;
class ADAK_SQL;

struct ADAK_SQL_STRUCT;
struct USER_LOGIN_INFO_STRUCT;

int         START_PROGRAM               ( int argc, char *argv[] , USER_LOGIN_INFO_STRUCT *P_USER_INFO, QWidget * parent );
QString     GET_DB_CONNECTION_STATUS    ( );
void        SET_DB_CONNECTION_STATUS    ( QString p_conn_status );

int         RESET_PROGRAM_CORE          ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

int         SET_BILGISAYAR_INFO         ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );
int         SET_KULLANICI_INFO          ( USER_LOGIN_INFO_STRUCT *P_USER_INFO  );
int         SET_KULLANICI_PAROLA        ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );
int         SET_VERITABANI_INFO         ( USER_LOGIN_INFO_STRUCT *P_USER_INFO  );

int         CHECK_KULLANICI_BILGILERI   ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

int         CHECK_USER_ACCESS           ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

int         CHECK_KULLANICI_BILGISAYARA_KAYITLI_MI ( int p_ynt_bilgisayar_id, int p_ynt_kullanici_id );

int         GET_USER_DB_COUNT           ( int p_ynt_kullanici_id );
int         GET_PROGRAM_DB_COUNT        ( int program_id, int p_ynt_kullanici_id );

int         TRY_TO_CONNECT_DB           ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

QString     GET_VERITABANI_ISMI         ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

int         CONNECT_TO_DB               ( int db_type, USER_LOGIN_INFO_STRUCT *P_USER_INFO, bool p_sql_error );

int         PROCESS_CONSOLE_ARGUMENTS   ( int argc, char *argv[], USER_LOGIN_INFO_STRUCT *P_USER_INFO );
int         SET_PROGRAM_DEFAULTS        ( int program_id );
int         SET_YONETIM_DEFAULTS        ( void F_KULLANICI_TAM_YETKI_FONK() );

int         GET_VERITABANI_ID           ( QString p_veritabani_ismi );

int         TRY_TO_SET_USER             ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );
int         TRY_TO_SET_DB               ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

int         LOGIN                       ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );


int         TRY_AUTO_LOGIN              ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );
int         IS_DB_CONNECTED             ( int db_type );

void        INIT_PROGRAM_DEFAULTS       ();
bool        FIRST_LOGINE_GIRECEK_MI      ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );

void        SET_STATIC_VALUES           ( USER_LOGIN_INFO_STRUCT *P_USER_INFO );
void        CLEAR_STATIC_VALUES         ();

ADAK_SQL * CONNECT_TO_DATABASE         ( int p_db_type, QString p_db_name, QString p_sql_motoru, bool p_sql_error = true );

int         CREATE_TABLES_OF_PROGRAM    ( USER_LOGIN_INFO_STRUCT *P_USER_INFO, bool start_yonetim_transaction = true );
int         CREATE_TABLES_OF_YONETIM    ( QString p_db_name );

int         CREATE_TABLES_OF_DATABASE   ( QList<ADAK_SQL_STRUCT *> , int db_type, QString p_db_name, QWidget * parent );

void        KULLANICI_TAM_YETKILENDIR   ( int p_kullanici_id );
void        ADD_YONETICI_USER           ( QString p_yonetici_parolasi );

void        SET_USER_LOGIN_STATUS       ( int p_login_status );
int         GET_USER_LOGIN_STATUS       ();

bool        PROGRAM_DB_VAR_MI           ( QString p_veritabani_kodu );

bool        YONETICI_IZNI_AL            ();

#endif // PROGRAM_CORE_H
