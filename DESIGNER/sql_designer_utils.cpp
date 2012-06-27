#include <QString>

#include "adak_sql_enums.h"
#include "adak_utils.h"
#include "sql_designer.h"


extern ADAK_SQL *   DB;

/**************************************************************************************
                   GET_ADAK_SQL_ENUM_STRING
***************************************************************************************/

QString GET_ADAK_SQL_ENUM_STRING ( int adak_sql_enum )
{
    switch ( adak_sql_enum ) {
        case INTEGER :
            return "INTEGER";
        case TEXT :
            return "TEXT";
        case REAL :
            return "REAL";
         case BLOB :
            return "BLOB";
         default :
             return NULL;
    }
}

/**************************************************************************************
                   GET_ADAK_SQL_ENUM
***************************************************************************************/

int GET_ADAK_SQL_ENUM ( QString adak_sql_enum_string )
{
    if ( adak_sql_enum_string EQ "INTEGER" ) {
        return INTEGER;
    }
    else if ( adak_sql_enum_string EQ "TEXT" ) {
        return TEXT;
    }
    else if ( adak_sql_enum_string EQ "REAL" ) {
        return REAL;
    }
    else if ( adak_sql_enum_string EQ "BLOB" ) {
        return BLOB;
    }
    else {
        return -1;
    }

}

/**************************************************************************************
                   GET_TABLO_ID
***************************************************************************************/

int GET_TABLO_ID ( QString tablo_adi )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("sql_tablolar","tablo_id","tablo_adi = :tablo_adi");
    sql_query.SET_VALUE(":tablo_adi", tablo_adi );
    if ( sql_query.SELECT() EQ 0 ) {
        return -1;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   GET_TABLO_ADI
***************************************************************************************/

QString GET_TABLO_ADI ( int tablo_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("sql_tablolar","tablo_adi","tablo_id = :tablo_id");
    sql_query.SET_VALUE(":tablo_id", tablo_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return NULL;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toString();
}

/**************************************************************************************
                   GET_ALAN_ID
***************************************************************************************/

int GET_ALAN_ID ( int tablo_id,  QString alan_adi )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("sql_alanlar","alan_id","tablo_id = :tablo_id AND alan_adi = :alan_adi");
    sql_query.SET_VALUE(":tablo_id", tablo_id );
    sql_query.SET_VALUE(":alan_adi", alan_adi );
    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   GET_ALAN_ADI
***************************************************************************************/

QString GET_ALAN_ADI ( int tablo_id,  int alan_id )
{
    SQL_QUERY sql_query ( DB );
    sql_query.PREPARE_SELECT("sql_alanlar","alan_adi","tablo_id = :tablo_id AND alan_id = :alan_id");
    sql_query.SET_VALUE(":tablo_id", tablo_id );
    sql_query.SET_VALUE(":alan_id" , alan_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return NULL;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toString();
}

/**************************************************************************************
                   INDEX_SAYISINI_BUL
***************************************************************************************/

int INDEX_SAYISINI_BUL ( int tablo_id )
{
    int INDEXLER_ARRAY[INDEXLER_ARRAY_SIZE] = {0};

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("sql_tablolar","indexler_array","tablo_id=:tablo_id");
    query.SET_VALUE     (":tablo_id",tablo_id);

    if ( query.SELECT() EQ 0 ) {
        return 0;
    }

    query.NEXT();

    UNPACK_INT_ARRAY(query.VALUE(0).toString(),INDEXLER_ARRAY,INDEXLER_ARRAY_SIZE);

    int index_sayisi = 0;
    for (int i = 0; i < INDEXLER_ARRAY_SIZE; i += 4 ) {
        if ( INDEXLER_ARRAY[i] > 0 ) {
            index_sayisi++;
        }
    }
    return index_sayisi;
}

/**************************************************************************************
                   KAYIT_EKLEME_YETKISI_VAR_MI
***************************************************************************************/

int KAYIT_EKLEME_YETKISI_VAR_MI ( int kullanici_id )
{
    SQL_QUERY sql_query(DB);
    sql_query.PREPARE_SELECT("sql_kullanici_yetkileri","kayit_ekleyebilir_mi","kullanici_id = :kullanici_id");
    sql_query.SET_VALUE(":kullanici_id", kullanici_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}


/**************************************************************************************
                   KAYIT_GUNCELLEME_YETKISI_VAR_MI
***************************************************************************************/

int KAYIT_GUNCELLEME_YETKISI_VAR_MI ( int kullanici_id )
{
    SQL_QUERY sql_query(DB);
    sql_query.PREPARE_SELECT("sql_kullanici_yetkileri","kayit_guncelleyebilir_mi","kullanici_id = :kullanici_id");
    sql_query.SET_VALUE(":kullanici_id", kullanici_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   KAYIT_SILME_YETKISI_VAR_MI
***************************************************************************************/

int KAYIT_SILME_YETKISI_VAR_MI ( int kullanici_id )
{
    SQL_QUERY sql_query(DB);
    sql_query.PREPARE_SELECT("sql_kullanici_yetkileri","kayit_silebilir_mi","kullanici_id = :kullanici_id");
    sql_query.SET_VALUE(":kullanici_id", kullanici_id );
    if ( sql_query.SELECT() EQ 0 ) {
        return 0;
    }
    sql_query.NEXT();
    return sql_query.VALUE(0).toInt();
}

/**************************************************************************************
                   SQL_YETKILI_KULLANICI_FISI_VE_YETKILI_KULLANICI_ORDER_NUM_OLUSTUR
***************************************************************************************/

void SQL_YETKILI_KULLANICI_FISI_VE_YETKILI_KULLANICI_ORDER_NUM_OLUSTUR ()
{
    SQL_QUERY sql_query    ( DB );
    SQL_QUERY second_query ( DB );

    sql_query.PREPARE_SELECT("sql_kullanici_yetkileri_fisi","kullanici_yetkileri_fisi_id");

    if ( sql_query.SELECT() > 0 ) {
        return;
    }

    DB->START_TRANSACTION();

    sql_query.PREPARE_INSERT("sql_kullanici_yetkileri_fisi","kullanici_yetkileri_fisi_id", "kullanici_yetkileri_fisi_id");
    sql_query.SET_VALUE(":kullanici_yetkileri_fisi_id", 1 );
    sql_query.INSERT();

    int yetki_order_num = 0;

    sql_query.PREPARE_SELECT("sql_kullanici_yetkileri", "yetki_id");

    if ( sql_query.SELECT() EQ 0 ) {
        DB->COMMIT_TRANSACTION();
        return;
    }

    while ( sql_query.NEXT() EQ true ) {

        yetki_order_num += 1024;

        second_query.PREPARE_UPDATE("sql_kullanici_yetkileri","yetki_id","yetki_order_num", "yetki_id = :yetki_id");

        second_query.SET_VALUE(":yetki_order_num", yetki_order_num            );
        second_query.SET_VALUE(":yetki_id",        sql_query.VALUE(0).toInt() );

        second_query.UPDATE();
    }

    DB->COMMIT_TRANSACTION();
}
