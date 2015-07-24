#include <QString>
#include <QCryptographicHash>
#include <adak_utils.h>

/**************************************************************************************
                   SQL_HOSTNAME
***************************************************************************************/

static QString             g_sql_hostname = "localhost";
QString SQL_HOSTNAME ()
{
    return g_sql_hostname;
}

/**************************************************************************************
                   SET_SQL_HOSTNAME
***************************************************************************************/

void SET_SQL_HOSTNAME (QString sql_hostname)
{
    g_sql_hostname = sql_hostname; // .toLower();
}

/**************************************************************************************
                   SQL_DEFAULT_USERNAME
***************************************************************************************/

QString SQL_DEFAULT_USERNAME ()
{
    return "adak_sql";
}

/**************************************************************************************
                   SQL_USERNAME
***************************************************************************************/

static QString g_sql_username = "adak_sql";
QString SQL_USERNAME ()
{
    return g_sql_username;
}

/**************************************************************************************
                   SET_SQL_USERNAME
***************************************************************************************/

void SET_SQL_USERNAME (QString sql_username)
{
    g_sql_username = sql_username;
}

/**************************************************************************************
                   SQL_PASSWORD
***************************************************************************************/

static QString             g_sql_password = "";
QString SQL_PASSWORD ()
{
    return g_sql_password;
}

/**************************************************************************************
                   SQL_DEFAULT_PASSWORD
***************************************************************************************/

QString SQL_DEFAULT_PASSWORD ()
{
    QString tmp_str = QString("z")+QString("5")+QString("t")+QString("q")+QString("g")+
                      g_sql_hostname + g_sql_username + 
                      QString("a")+QString("z")+QString("8")+QString("w")+QString("h");

    QByteArray xyz;
    xyz.append (tmp_str);
    QByteArray hash = (QCryptographicHash::hash (xyz, QCryptographicHash::Md5)).toHex();
    QString sql_password  = "";
    sql_password += hash.at(7);
    sql_password += hash.at(3);
    sql_password += hash.at(11);
    sql_password += hash.at(27);
    sql_password += hash.at(9);
    sql_password += hash.at(19);
    sql_password += hash.at(28);
    sql_password += hash.at(16);
    sql_password += hash.at(4);
    sql_password += hash.at(21);
    sql_password += hash.at(8);
    sql_password += hash.at(14);

    return sql_password;
}

/**************************************************************************************
                   SET_SQL_PASSWORD
***************************************************************************************/

void SET_SQL_PASSWORD (QString sql_password)
{
    g_sql_password = sql_password;
}

/**************************************************************************************
                   SQL_MOTORU
***************************************************************************************/

//translate static QString             g_sql_motoru = "YOK";
static QString             g_sql_motoru = QT_TR_NOOP("NONE");
QString SQL_MOTORU ()
{
    return g_sql_motoru;
}

/**************************************************************************************
                   SET_SQL_MOTORU
***************************************************************************************/

void SET_SQL_MOTORU (QString sql_motoru)
{
    g_sql_motoru = sql_motoru.toUpper();
    //translate if (g_sql_motoru == "SQLITE" OR g_sql_motoru == tr("NONE")) {
    if (g_sql_motoru == "SQLITE" OR g_sql_motoru == "YOK") {
        //g_sql_driver = SQLITE;
    }
    else if (g_sql_motoru == "MYSQL") {
        //g_sql_driver = MYSQL;
    }
    else if (g_sql_motoru == "POSTGRESQL") {
        //g_sql_driver = PSQL;
    }
    else if (g_sql_motoru == "MICROSOFT SQL") {
        //g_sql_driver = MSSQL;
    }
    else if (g_sql_motoru == "ORACLE") {
        //g_sql_driver = ORACLE;
    }
    else {
        //g_sql_driver = UNKNOWN;
    }
    g_sql_motoru = sql_motoru; // Not toUpper !..
}

/**************************************************************************************
                   SQL_PORT
***************************************************************************************/

static int             g_sql_port = 5432; // PSQL
int SQL_PORT ()
{
    return g_sql_port;
}

/**************************************************************************************
                   SET_SQL_PORT
***************************************************************************************/

void SET_SQL_PORT (int sql_port)
{
    g_sql_port = sql_port;
}

/**************************************************************************************
                   SQL_DBNAME_ONEK
***************************************************************************************/

static QString             g_sql_dbname_onek = "";
QString SQL_DBNAME_ONEK ()
{
    return g_sql_dbname_onek;
}

/**************************************************************************************
                   SET_SQL_DBNAME_ONEK
***************************************************************************************/

void SET_SQL_DBNAME_ONEK (QString sql_dbname_onek)
{
    g_sql_dbname_onek = sql_dbname_onek;
}

