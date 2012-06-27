#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "zlib.h"

#include "adak_defines.h"

#include "adak_exporter.h"
#include <qtextcodec.h>

#define BUF_SIZE 1024

/****************************************************************************************
                                ADAK_EXPORTER::ADAK_EXPORTER
*****************************************************************************************/

ADAK_EXPORTER::ADAK_EXPORTER ()
{
}

/****************************************************************************************
                                ADAK_EXPORTER::~ADAK_EXPORTER
*****************************************************************************************/

ADAK_EXPORTER::~ADAK_EXPORTER()
{

}

/****************************************************************************************
                                ADAK_EXPORTER::GET_XML_HEADER
*****************************************************************************************/

QString ADAK_EXPORTER::GET_XML_HEADER()
{
     return QString ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<office:document-content "
    "xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" "
    "xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" "
    "xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" "
    "xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" "
    "xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" "
    "xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" "
    "xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
    "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" "
    "xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\" "
    "xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" "
    "xmlns:presentation=\"urn:oasis:names:tc:opendocument:xmlns:presentation:1.0\" "
    "xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" "
    "xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\" "
    "xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\" "
    "xmlns:math=\"http://www.w3.org/1998/Math/MathML\" "
    "xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\" "
    "xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\" "
    "xmlns:ooo=\"http://openoffice.org/2004/office\" "
    "xmlns:ooow=\"http://openoffice.org/2004/writer\" "
    "xmlns:oooc=\"http://openoffice.org/2004/calc\" "
    "xmlns:dom=\"http://www.w3.org/2001/xml-events\" "
    "xmlns:xforms=\"http://www.w3.org/2002/xforms\" "
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:rpt=\"http://openoffice.org/2005/report\" "
    "xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" "
    "xmlns:xhtml=\"http://www.w3.org/1999/xhtml\" "
    "xmlns:grddl=\"http://www.w3.org/2003/g/data-view#\" "
    "xmlns:tableooo=\"http://openoffice.org/2009/table\" "
    "xmlns:field=\"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0\" "
    "xmlns:formx=\"urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0\" "
    "xmlns:css3t=\"http://www.w3.org/TR/css3-text/\" office:version=\"1.2\" "
    "grddl:transformation=\"http://docs.oasis-open.org/office/1.2/xslt/odf2rdf.xsl\">"
    "<office:scripts/><office:font-face-decls> "
    "<style:font-face style:name=\"Liberation Sans\" svg:font-family=\"&apos;Liberation "
    "Sans&apos;\" style:font-family-generic=\"swiss\" style:font-pitch=\"variable\"/> "
    "<style:font-face style:name=\"DejaVu Sans\" svg:font-family=\"&apos;DejaVu "
    "Sans&apos;\" style:font-family-generic=\"system\" style:font-pitch=\"variable\"/> "
    "<style:font-face style:name=\"Lohit Hindi\" svg:font-family=\"&apos;Lohit "
    "Hindi&apos;\" style:font-family-generic=\"system\" style:font-pitch=\"variable\"/> "
    "</office:font-face-decls><office:automatic-styles><style:style style:name=\"co1\" "
    "style:family=\"table-column\"> "
    "<style:table-column-properties fo:break-before=\"auto\" "
    "style:column-width=\"2.267cm\"/></style:style> "
    "<style:style style:name=\"ro1\" "
    "style:family=\"table-row\"><style:table-row-properties style:row-height=\"0.452cm\" "
    "fo:break-before=\"auto\" "
    "style:use-optimal-row-height=\"true\"/></style:style><style:style style:name=\"ta1\" "
    "style:family=\"table\" style:master-page-name=\"Default\"> "
    "<style:table-properties table:display=\"true\" "
    "style:writing-mode=\"lr-tb\"/></style:style><style:style style:name=\"ta_extref\" "
    "style:family=\"table\"><style:table-properties "
    "table:display=\"false\"/></style:style></office:automatic-styles><office:body><office:spreadsheet>");


}

/****************************************************************************************
                                ADAK_EXPORTER::GET_XML_FOOTER
*****************************************************************************************/

QString ADAK_EXPORTER::GET_XML_FOOTER()
{
    return QString("</office:spreadsheet>"
                   "</office:body>"
                   "</office:document-content>");
}

/****************************************************************************************
                                ADAK_EXPORTER::GET_XML_FOOTER
*****************************************************************************************/

QString ADAK_EXPORTER::GET_XML_BODY(QList<QStringList> rows, QString page_name)
{
    QString xml_body = "";
    QString xml_rows = "";

    int max_column_count   = 0;
    int empty_column_count = 0;
    int empty_row_count    = 0;

    for ( int i = 0 ; i < rows.size() ; i++ ) {
        if (rows.at(i).size() EQ 0 ) {
            empty_row_count++;
            continue;
        }

        if (max_column_count < rows.at(i).size()) {
            max_column_count = rows.at(i).size();
        }

        xml_rows.append(QString("<table:table-row table:style-name=\"ro1\""));

        if (empty_row_count NE 0) {
             xml_rows.append(QString(" table:number-rows-repeated=\"%1\">").arg(empty_row_count));
             empty_row_count = 0;
        }
        else {
            xml_rows.append(QString(">"));
        }

        QStringList columns = rows.at(i);

        for ( int i = 0 ; i < columns.size(); i++ ) {
            if (columns.at(i).isEmpty() EQ true) {
                empty_column_count++;
                continue;
            }

            if (empty_column_count NE 0) {
                xml_rows.append(QString("<table:table-cell table:number-columns-repeated=\"%1\"/>").arg(empty_column_count));
                empty_column_count = 0;
            }

            QString value = columns.at(i);

            value.replace("&","&#38;");

            xml_rows.append("<table:table-cell office:value-type=\"string\">");
            xml_rows.append(QString("<text:p>%1</text:p></table:table-cell>").arg(value));
        }

        if (empty_column_count NE 0) {
            xml_rows.append(QString("<table:table-cell table:number-columns-repeated=\"%1\"/>").arg(empty_column_count));
            empty_column_count = 0;
        }

        xml_rows.append("</table:table-row>");

    }

    xml_body.append(QString("<table:table table:name=\"%1\" table:style-name=\"ta1\" table:print=\"false\">").arg(page_name));
    xml_body.append(QString("<table:table-column table:style-name=\"co1\" table:number-columns-repeated=\"%1\" "
                            "table:default-cell-style-name=\"Default\"/>").arg(max_column_count));

    xml_body.append(xml_rows);

    xml_body.append("</table:table>");

    return xml_body;
}

/****************************************************************************************
                                ADAK_EXPORTER::EXPORT
*****************************************************************************************/

void ADAK_EXPORTER::EXPORT( QList<QStringList> rows, QString file_path )
{
    zipFile ods_file;

    ods_file = zipOpen(file_path.toUtf8().data(),APPEND_STATUS_CREATE);

    if (ods_file EQ NULL) {
        return;
    }

    QStringList ods_file_struct;


    ods_file_struct<<"styles.xml";
    ods_file_struct<<"settings.xml";
    ods_file_struct<<"mimetype";
    ods_file_struct<<"meta.xml";
    ods_file_struct<<"Thumbnails->thumbnail.png";
    ods_file_struct<<"META-INF->manifest.xml";
    ods_file_struct<<"Configurations2->toolbar->";
    ods_file_struct<<"Configurations2->statusbar->";
    ods_file_struct<<"Configurations2->progressbar->";
    ods_file_struct<<"Configurations2->popupmenu->";
    ods_file_struct<<"Configurations2->menubar->";
    ods_file_struct<<"Configurations2->images->Bitmaps->";
    ods_file_struct<<"Configurations2->floater->";
    ods_file_struct<<"Configurations2->accelerator->current.xml";

    for ( int i = 0 ; i < ods_file_struct.size() ; i++ ) {
        QString file_name;
        QString qrc_file_name;

        QStringList file_info = ods_file_struct.at(i).split("->");

        for ( int j = 0 ; j < file_info.size() ; j++ ) {
            if ( file_name.isEmpty() EQ false ) {
                file_name.append("/");
            }

            file_name.append(file_info.at(j));
        }

        if ( file_info.last().isEmpty() EQ false ) {
            qrc_file_name = file_info.last();
        }

        QByteArray zip_data;

        if ( qrc_file_name.isEmpty() EQ false ) {
            QFile file(QString(":/ods_io_resources/ods_file/%1").arg(qrc_file_name));

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return;
            }

            zip_data = file.readAll();

            file.close();
        }

        WRITE_FILE_TO_ZIP(ods_file,file_name,zip_data);
    }

    QString xml_content = GET_XML_HEADER();

    xml_content.append(GET_XML_BODY(rows));

    xml_content.append(GET_XML_FOOTER());

    if ( xml_content.isEmpty() EQ false ) {
        WRITE_FILE_TO_ZIP(ods_file,"content.xml",QVariant(xml_content).toByteArray());
    }

    zipClose(ods_file,NULL);    
}

/****************************************************************************************
                                ADAK_EXPORTER::WRITE_FILE_TO_ZIP
*****************************************************************************************/

void ADAK_EXPORTER::WRITE_FILE_TO_ZIP(zipFile file, QString file_name, QByteArray data)
{
    zip_fileinfo zi;

    int opt_compress_level = Z_DEFAULT_COMPRESSION;

    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour = zi.tmz_date.tm_mday = zi.tmz_date.tm_min = zi.tmz_date.tm_year =  0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;

    int err = zipOpenNewFileInZip(file,file_name.toUtf8().data(),
            &zi,NULL,0,NULL,0,"Adak Import",(opt_compress_level != 0) ? Z_DEFLATED : 0,opt_compress_level);

    if (err != ZIP_OK) {
        return;
    }

    if ( data.isEmpty() EQ false ) {
        err = zipWriteInFileInZip (file,data.data(),data.size());
        if (err != ZIP_OK) {
            return;
        }
    }

    zipCloseFileInZip(file);
}
