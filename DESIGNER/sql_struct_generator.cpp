#include <QDir>
#include <QFile>

#include "adak_sql.h"
#include "yonetim.h"
#include "sql_struct_generator.h"
#include "sql_designer.h"
#include "adak_utils.h"

extern ADAK_SQL * G_YONETIM_DB;
extern ADAK_SQL * DB;

QString file_content;



/**************************************************************************************
                   nl
***************************************************************************************/
// Add new line to the string
QString nl(const QString& str)
{
    return str + "\n";
}

// Indent the line
/**************************************************************************************
                   tab
***************************************************************************************/
QString tab(int indent, const QString& str)
{
    const QString indentStr = "    ";

    QString rv;
    while (indent-- > 0) {
        rv += indentStr;
    }

    rv += str;
    return rv;
}


// Convert QVariant (char, int vs) to QString
/**************************************************************************************
                   toString
***************************************************************************************/
QString toString(QVariant v)
{
    return v.toString();
}

/**************************************************************************************
                   HEADER_DOSYASI_YARAT
***************************************************************************************/

bool HEADER_DOSYASI_YARAT( QWidget * parent )
{
    SQL_QUERY yonetim_query ( G_YONETIM_DB);
    SQL_QUERY db_query      ( DB          );

    db_query.PREPARE_SELECT     ("sql_db_info","program_id");

    if ( db_query.SELECT () EQ 0 ) {
        exit(88);
    }

    db_query.NEXT();

    int program_id = db_query.VALUE(0).toInt();

    yonetim_query.PREPARE_SELECT("ynt_veritabanlari", "veritabani_ismi","veritabani_id = :veritabani_id");
    yonetim_query.SET_VALUE(":veritabani_id", VERITABANI_ID());
    if ( yonetim_query.SELECT("veritabani_ismi") EQ 0 ) {
        return false;
    }
    yonetim_query.NEXT();
    QString db_name   = yonetim_query.VALUE(0).toString();
    QString file_path = db_name.toLower() + "_db.h";


    QFile header_file ( file_path );
    header_file.open(QIODevice::WriteOnly | QIODevice::Text);

    file_content = "#ifndef " + db_name.toUpper() + "_DB_H\n"
                   "#define " + db_name.toUpper() + "_DB_H\n\n";

    file_content += nl( "ADAK_SQL_STRUCT " + db_name + "_db" +  " = {" );
    file_content += tab (1, nl(QVariant(program_id).toString() + ","));

    TABLOLARI_YAZ ( parent );

    file_content += nl("};");


    file_content +=  "#endif // " + db_name.toUpper() + "_DB_H \n";

    header_file.write ( file_content.toLatin1() );
    header_file.close();

    return true;
}

/**************************************************************************************
                   TABLOLARI_YAZ
***************************************************************************************/

bool TABLOLARI_YAZ (QWidget * parent )
{
        SQL_QUERY sql_query     (DB);
        sql_query.PREPARE_SELECT("sql_tablolar","tablo_id ,tablo_adi");
        int tablo_sayisi = sql_query.SELECT("tablo_adi");
        if ( tablo_sayisi EQ 0 ) {
            ADAK_ERROR(QObject::tr ("Table is not found."),NULL,parent);
            return false;
        }
        int virgul_sayisi = tablo_sayisi;
        file_content += tab( 1, nl ( toString(tablo_sayisi) + ","));
        file_content += tab(1, nl("{"));

        while ( sql_query.NEXT() EQ true ) {
            file_content += tab(2, nl("{"));
            file_content += tab(3, nl("\"" + sql_query.VALUE(1).toString() + "\","));
            ALANLARI_YAZ( sql_query.VALUE(0).toInt());
            file_content += tab(2, "}");
            virgul_sayisi--;
            if ( virgul_sayisi > 0 ) {
                file_content += ",";
            }
            file_content += "\n";
        }

        file_content += tab(1, nl("}"));


        return true;
}

/**************************************************************************************
                   ALANLARI_YAZ
***************************************************************************************/

bool ALANLARI_YAZ ( int tablo_id )
{
    SQL_QUERY sql_query (DB);
    sql_query.PREPARE_SELECT("sql_alanlar","veritabanindaki_adi, alan_tipi, uzunluk, dizi_mi, dizi_boyutu, auto_increment_mi, "
                             "not_null_mi","tablo_id = :tablo_id");
    sql_query.SET_VALUE(":tablo_id", tablo_id );
    int alan_sayisi = sql_query.SELECT("alan_order_num");
    if ( alan_sayisi EQ 0 ) {
        return false;
    }
    file_content += tab ( 3, nl(toString(alan_sayisi) + ","));
    file_content += tab ( 3, nl("{"));
    
    int virgul_sayisi = alan_sayisi;

    while ( sql_query.NEXT() EQ true ) {
        file_content += tab(4,"{");
        file_content += "\"" + sql_query.VALUE(0).toString() + "\", ";
        file_content += GET_ADAK_SQL_ENUM_STRING(GET_ADAK_SQL_ENUM(sql_query.VALUE(1).toString())) + ", ";
        file_content += sql_query.VALUE(2).toString() + ", ";
        if (sql_query.VALUE(3).toInt() EQ 0 ) {
            file_content += QString("false") + ", ";
        }
        else {
            file_content += QString("true") + ", ";
        }
        file_content += sql_query.VALUE(4).toString() + ", ";
        if (sql_query.VALUE(5).toInt() EQ 0 ) {
            file_content += QString("false") + ", ";
        }
        else {
            file_content += QString("true") + ", ";
        }
        if (sql_query.VALUE(6).toInt() EQ 0 ) {
            file_content += QString("false");
        }
        else {
            file_content += QString("true");
        }
        file_content += " }";
        virgul_sayisi --;
        if ( virgul_sayisi > 0 ) {
            file_content += ",";
        }
        file_content += "\n";

    }
    file_content += tab(3, nl("},"));

    INDEXLERI_YAZ( tablo_id );

    return true;
}

/**************************************************************************************
                   INDEXLERI_YAZ
***************************************************************************************/

bool INDEXLERI_YAZ ( int tablo_id )
{
    int index_sayisi = INDEX_SAYISINI_BUL(tablo_id);
    file_content += tab (3, nl( QVariant(index_sayisi).toString() + "," ));
    file_content += tab (3, nl ("{"));
    HEADER_ICIN_INDEX_YAPISI_OLUSTUR( index_sayisi, tablo_id );
    file_content += tab (3, nl ("}"));
    return true;
}

/**************************************************************************************
                   HEADER_ICIN_INDEX_YAPISI_OLUSTUR
***************************************************************************************/

bool HEADER_ICIN_INDEX_YAPISI_OLUSTUR (int index_sayisi, int tablo_id )
{
    int INDEXLER_ARRAY[INDEXLER_ARRAY_SIZE] = {0};

    SQL_QUERY query (DB);

    query.PREPARE_SELECT("sql_tablolar","indexler_array","tablo_id=:tablo_id");
    query.SET_VALUE     (":tablo_id",tablo_id);

    if ( query.SELECT() EQ 0 ) {
        return false;
    }

    query.NEXT();

    UNPACK_INT_ARRAY(query.VALUE(0).toString(),INDEXLER_ARRAY,INDEXLER_ARRAY_SIZE);

    for (int i = 0; i < INDEXLER_ARRAY_SIZE; i += 4 ) {
        if ( INDEXLER_ARRAY[i] EQ 0 ) {
            continue;
        }
        if ( i EQ 0 ) {
            QString alan_adi = GET_ALAN_ADI( tablo_id, INDEXLER_ARRAY[i]);
            file_content += tab (4, "{ ");
            file_content += QString("ADAK_SQL_PRIMARY , 1, {") + "\"" + alan_adi  + "\"" + "}";
            file_content += " }";
            index_sayisi--;
            if (index_sayisi > 0 ) {
                file_content += ",";
            }
            file_content += "\n";
        }
        else {
            int indexteki_kolon_sayisi = 0;
            for ( int j = i; j < i + 4; j++ ) {
                if ( INDEXLER_ARRAY[j] NE 0 ) {
                    indexteki_kolon_sayisi++;
                }
            }
            file_content += tab (4, "{ ");
            file_content += "ADAK_SQL_INDEX , " + QVariant(indexteki_kolon_sayisi).toString() + ", { ";
            for (int j = 0; j < indexteki_kolon_sayisi; j++ ) {
                QString alan_adi = GET_ALAN_ADI( tablo_id, INDEXLER_ARRAY[i+j]);
                file_content += "\"" + alan_adi  + "\"" ;
                if (j NE indexteki_kolon_sayisi -1 ) {
                    file_content += ",";
                }

            }
            file_content += " } }";
            index_sayisi--;
            if (index_sayisi > 0 ) {
                file_content += ",";
            }
            file_content += "\n";
        }

    }
    return true;
}

