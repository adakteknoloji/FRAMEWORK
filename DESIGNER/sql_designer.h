#ifndef SQL_DESIGNER_H
#define SQL_DESIGNER_H

#include <QDate>
#include <QString>
#include "adak_sql.h"

class QWidget;
class QDialog;

#define INDEXLER_ARRAY_SIZE         40

int         OPEN_SQL_TABLO_ARAMA             ( QWidget * parent );
void        OPEN_INDEX_BATCH                 ( int       tablo_id, QWidget * parent );

QString     GET_ADAK_SQL_ENUM_STRING        ( int     adak_sql_enum );
int         GET_ADAK_SQL_ENUM               ( QString adak_sql_enum_string );
int         GET_TABLO_ID                     ( QString tablo_adi             );
QString     GET_TABLO_ADI                    ( int     tablo_id              );
int         GET_ALAN_ID                      ( int     tablo_id,  QString alan_adi );
QString     GET_ALAN_ADI                     ( int     tablo_id,      int alan_id );
int         INDEX_SAYISINI_BUL               ( int     tablo_id );


int         KAYIT_EKLEME_YETKISI_VAR_MI     ( int kullanici_id );
int         KAYIT_GUNCELLEME_YETKISI_VAR_MI ( int kullanici_id );
int         KAYIT_SILME_YETKISI_VAR_MI      ( int kullanici_id );

void        SQL_YETKILI_KULLANICI_FISI_VE_YETKILI_KULLANICI_ORDER_NUM_OLUSTUR ();



#endif // SQL_DESIGNER_H
