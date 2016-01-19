#include <stdio.h>
#include "yonetim.h"

#include "adak_sql.h"
#include "belge.h"

#include "belge_struct_generator.h"

extern ADAK_SQL * DB;
extern ADAK_SQL * G_YONETIM_DB;

/**************************************************************************************
                   OPEN_HEADER_DOSYASI_OLUSTUR
***************************************************************************************/

void OPEN_HEADER_DOSYASI_OLUSTUR(int)
{
    BELGE_STRUCT_GENERATOR * belge_struct_generator = new BELGE_STRUCT_GENERATOR();
    belge_struct_generator->CREATE_BELGELER_HEADER();
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::BELGE_STRUCT_GENERATOR
***************************************************************************************/

BELGE_STRUCT_GENERATOR::BELGE_STRUCT_GENERATOR()
{

}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::~BELGE_STRUCT_GENERATOR
***************************************************************************************/

BELGE_STRUCT_GENERATOR::~BELGE_STRUCT_GENERATOR()
{

}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_BLG_FILES
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_BLG_FILES(int belge_id)
{
    m_header_element_count = 0;
    m_line_element_count   = 0;
    m_footer_element_count = 0;

    m_belge_id = belge_id;

    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("blg_belgeler","belge_adi","belge_id = :belge_id");
    sql_query.SET_VALUE(":belge_id",m_belge_id);

    if (sql_query.SELECT("belge_id") EQ 0) {
        return "";
    }

    sql_query.NEXT();

    m_belge_adi = sql_query.VALUE(0).toString();

    return CREATE_TASARIM_HEADER();
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_TASARIM_HEADER
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_TASARIM_HEADER()
{
    SQL_QUERY sql_query(DB);

    sql_query.PREPARE_SELECT("blg_belge_degiskenleri",
                             "degisken_id, grubu, adi, font_size, align, kodu, monospace_mi, text_boyutu ",
                             "belge_id = :belge_id");
    sql_query.SET_VALUE(":belge_id",m_belge_id);

    if ( sql_query.SELECT("degisken_id") EQ 0 ) {
        return "";
    }

    QString upper_belge_adi             = m_belge_adi.toUpper();

    QString header_enum_list       = "";
    QString footer_enum_list       = "";
    QString line_enum_list         = "";

    QString header_degiskenler     = "";
    QString footer_degiskenler     = "";
    QString line_degiskenler       = "";

    while(sql_query.NEXT()) {

        switch(sql_query.VALUE(1).toInt()) {
            case HEADER:
                m_header_element_count++;
                INCLUDE_STR_OLUSTUR(&header_enum_list,&header_degiskenler,sql_query.VALUE(0).toString(),sql_query.VALUE(2).toString(),
                                   sql_query.VALUE(5).toString(),sql_query.VALUE(3).toString(),sql_query.VALUE(4).toString(),
                                   sql_query.VALUE(6).toString(), sql_query.VALUE(7).toString());
                break;
            case FOOTER:
                m_footer_element_count++;
                INCLUDE_STR_OLUSTUR(&footer_enum_list,&footer_degiskenler,sql_query.VALUE(0).toString(),sql_query.VALUE(2).toString(),sql_query.VALUE(5).toString(),
                                            sql_query.VALUE(3).toString(),sql_query.VALUE(4).toString(), sql_query.VALUE(6).toString(), sql_query.VALUE(7).toString());
                break;
            case LINE:
                m_line_element_count++;
                INCLUDE_STR_OLUSTUR(&line_enum_list,&line_degiskenler,sql_query.VALUE(0).toString(),sql_query.VALUE(2).toString(),sql_query.VALUE(5).toString(),
                                            sql_query.VALUE(3).toString(),sql_query.VALUE(4).toString(), sql_query.VALUE(6).toString(), sql_query.VALUE(7).toString());
                break;
            default:
                break;
        }
    }

    QString all_include_file = "";

    all_include_file.append(QString(QObject::tr("\n\n//---------DOCUMENT OF %1 ---------//\n\n")).arg(upper_belge_adi));

    all_include_file.append(CREATE_DEFINE(upper_belge_adi,"HEADER",m_header_element_count));
    all_include_file.append(CREATE_DEFINE(upper_belge_adi,"LINE",m_line_element_count));
    all_include_file.append(CREATE_DEFINE(upper_belge_adi,"FOOTER",m_footer_element_count));

    all_include_file.append(CREATE_ENUM_STRING(upper_belge_adi,"HEADER",header_enum_list,m_header_element_count));
    all_include_file.append(CREATE_ENUM_STRING(upper_belge_adi,"LINE",line_enum_list,m_line_element_count));
    all_include_file.append(CREATE_ENUM_STRING(upper_belge_adi,"FOOTER",footer_enum_list,m_footer_element_count));

    all_include_file.append(CREATE_DEGISKEN_OZELLIKLERI_STRING("HEADER",header_degiskenler,m_header_element_count));
    all_include_file.append(CREATE_DEGISKEN_OZELLIKLERI_STRING("LINE",line_degiskenler,m_line_element_count));
    all_include_file.append(CREATE_DEGISKEN_OZELLIKLERI_STRING("FOOTER",footer_degiskenler,m_footer_element_count));

    all_include_file.append("\n\n//-------------------------------------------//\n\n");

    return all_include_file;
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_BELGELER_HEADER
***************************************************************************************/

void BELGE_STRUCT_GENERATOR::CREATE_BELGELER_HEADER()
{
    SQL_QUERY query_yonetim(G_YONETIM_DB);
    SQL_QUERY sql_query(DB);
    
    query_yonetim.PREPARE_SELECT("ynt_veritabanlari","veritabani_ismi","veritabani_id = :veritabani_id");
    query_yonetim.SET_VALUE(":veritabani_id", VERITABANI_ID());
    if ( query_yonetim.SELECT() EQ 0 ) {
        return;
    }
    query_yonetim.NEXT();

    QString header_ismi = QString("%1_belge").arg(query_yonetim.VALUE(0).toString());

    sql_query.PREPARE_SELECT("blg_belgeler","belge_id,belge_adi");

    if ( sql_query.SELECT() EQ 0 ) {
        return;
    }

    FILE * file_pointer;

    char * file_name = QString(header_ismi + ".h").toUtf8().data();

    file_pointer = fopen (file_name,"w");

    if (file_pointer EQ NULL) {
        return;
    }

    QString enums        = "";
    QString structure    = "";
    QString belgeler_str = "";

    int belge_counter = 0;

    while(sql_query.NEXT()) {

        belgeler_str.append( "\n\n" + CREATE_BLG_FILES(sql_query.VALUE(0).toInt()) + "\n\n");

        QString belge_id  = sql_query.VALUE(0).toString();
        QString belge_adi = sql_query.VALUE(1).toString();

        if (enums.isEmpty() EQ false) {
            enums.append(",\n");
        }

        if (structure.isEmpty() EQ false) {
            structure.append(",\n\t");
        }

        enums    .append(QString("\t%1\t\t = %2").arg(belge_adi.toUpper()).arg(QVariant(belge_counter).toString()));
        structure.append(QString("{ %1, \"%2\", %3_HEADER_VARIABLE_COUNT, %3_FOOTER_VARIABLE_COUNT, %3_LINE_VARIABLE_COUNT,%4_header,%4_footer,%4_line }")
                          .arg(belge_id.toUpper()).arg(belge_adi).arg(belge_adi.toUpper()).arg(belge_adi));

        belge_counter++;

    }

    QString all_include_file = "";

    all_include_file.append(CREATE_INCLUDE_BEGIN_STR(header_ismi.toUpper()));
    all_include_file.append(belgeler_str);
    all_include_file.append(QString("\n\n#define ADAK_BELGE_COUNT %1").arg(QVariant(sql_query.NUM_OF_ROWS()).toString()));
    all_include_file.append(QString("\n\n\nenum %1_ENUM {\n%2\n};").arg(header_ismi.toUpper()).arg(enums));
    all_include_file.append(QString("\n\nstatic ADAK_BELGELER_STRUCT %1_struct[ADAK_BELGE_COUNT] = {\n\t%2 \n};").arg(header_ismi).arg(structure));
    all_include_file.append(CREATE_INCLUDE_END_STR(header_ismi.toUpper()));


    fputs (all_include_file.toUtf8().data(),file_pointer);

    fclose (file_pointer);
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_ENUM_STRING
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_ENUM_STRING(QString belge_adi,QString grup,QString enum_list_str,int count)
{
    if (count EQ 0) {
        return "";
    }

    return QString("\n\nenum %1_%2_ENUM {\n"
                               "%3"
                               "\n};").arg(belge_adi).arg(grup).arg(enum_list_str);
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_DEGISKEN_OZELLIKLERI_STRING
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_DEGISKEN_OZELLIKLERI_STRING(QString grup,QString degiskenler,int count)
{
    if (count EQ 0) {
        return "";
    }

    return QString("\n\nstatic ADAK_BELGE_DEFAULTS_STRUCT %1_%2[%3] = {\n"
                   "\t %4\n"
                   "};").arg(m_belge_adi).arg(grup.toLower()).arg(count).arg(degiskenler);
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_DEFINE
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_DEFINE(QString belge_adi,QString grup,int count)
{
    if (count EQ 0) {
        return "";
    }

    return QString("\n#define %1_%2_VARIABLE_COUNT %3").arg(belge_adi).arg(grup).arg(QVariant(count).toInt());
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_INCLUDE_BEGIN_STR
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_INCLUDE_BEGIN_STR(QString belge_adi)
{
    return QString("#ifndef %1_H\n#define %1_H\n\n#include \"belge_struct.h\"\n").arg(belge_adi);
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::CREATE_INCLUDE_END_STR
***************************************************************************************/

QString BELGE_STRUCT_GENERATOR::CREATE_INCLUDE_END_STR(QString belge_adi)
{
    return QString("\n\n#endif // %1_H").arg(belge_adi);
}

/**************************************************************************************
                   BELGE_STRUCT_GENERATOR::INCLUDE_STR_OLUSTUR
***************************************************************************************/

void BELGE_STRUCT_GENERATOR::INCLUDE_STR_OLUSTUR(QString *enum_str, QString *degiskenler_str, QString degisken_id, QString degisken_adi,
                                                 QString degisken_kodu, QString degisken_font, QString degisken_align, QString monospace_mi, QString text_boyutu)
{
    if (enum_str->isEmpty() EQ false) {
        enum_str->append(",\n");
    }
    if (degiskenler_str->isEmpty() EQ false) {
        degiskenler_str->append(",");
    }
    enum_str->append(QString("\t%1\t\t = %2").arg(degisken_kodu.toUpper()).arg(degisken_id));

    degiskenler_str->append("{ ");
    degiskenler_str->append(QString("%1,\"%2\",\"%3\",").arg(QVariant(degisken_id).toString()).arg(degisken_kodu).arg(degisken_adi));
    degiskenler_str->append(QString("%1,").arg(degisken_font));
    degiskenler_str->append(QString("%1,true,0,0," ).arg(degisken_align));
    degiskenler_str->append(QString("%1," ).arg( monospace_mi ));
    degiskenler_str->append(QString("%1" ).arg( text_boyutu ));

    degiskenler_str->append(" }");
}

