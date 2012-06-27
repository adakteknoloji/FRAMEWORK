#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <zlib.h>
#include "adak_defines.h"
#include "adak_importer.h"
#include <QByteArray>
#include <qtextcodec.h>

#define BUF_SIZE 1024


/****************************************************************************************
                                ADAK_IMPORT::ADAK_IMPORT
*****************************************************************************************/

ADAK_IMPORT::ADAK_IMPORT ()
{

}

/****************************************************************************************
                                ADAK_IMPORT::~ADAK_IMPORT
*****************************************************************************************/

ADAK_IMPORT::~ADAK_IMPORT()
{

}

/****************************************************************************************
                                ADAK_IMPORT::IMPORT
*****************************************************************************************/

QList<QStringList> ADAK_IMPORT::IMPORT( QString file_path , QString text_codec, int column_size )
{
    QString content;
    int     num_of_bytes;
    char    buffer[BUF_SIZE];
    unzFile hArchive;

    hArchive = unzOpen(file_path.toUtf8().data());

    if (hArchive != NULL) {
        if (unzLocateFile(hArchive, "content.xml", 0) == UNZ_OK) {
            if (unzOpenCurrentFile(hArchive) == UNZ_OK) {
                while (1) {
                    num_of_bytes = unzReadCurrentFile(hArchive, buffer, BUF_SIZE-1);
                    if (num_of_bytes <= 0) {
                        break;
                    }
                    buffer[num_of_bytes] = '\0';
                    content.append( buffer );
                }
                unzCloseCurrentFile(hArchive);
            }
        }
    }
    unzClose(hArchive);

    QXmlStreamReader    reader (content);
    QList<QStringList>  rows;
    QStringList         column;

    QByteArray          atArr    = "@";
    QByteArray          tmp_text_codec_name;
    tmp_text_codec_name.insert( 0 , text_codec );

    QTextCodec          *codec   = QTextCodec::codecForName( tmp_text_codec_name );
    QString             atStr    = codec->toUnicode( atArr );

    while (reader.atEnd() NE true) {
        reader.readNext();

        if (reader.isStartElement() EQ true) {
            if (reader.name() EQ "table-cell") {
                if (reader.attributes().value("table:number-columns-repeated").toString().toInt() > 0 ) {
                    int repeated_column_count = reader.attributes().value("table:number-columns-repeated").toString().toInt();

                    QByteArray tmp_byte_array ;
                    tmp_byte_array.insert( 0 , reader.readElementText(QXmlStreamReader::IncludeChildElements) );
                    for ( int i = 0 ; i < repeated_column_count ; i++ ) {
                        if ( repeated_column_count < column_size ) {
                            column <<  codec->toUnicode( tmp_byte_array ).toUtf8();
                        }
                    }
                }
                else {
                    reader.readNext();
                    if (reader.isEndElement() EQ true AND reader.name() EQ "table-cell") {
                        column << atStr ;
                    }
                }
            }

            if (reader.name() EQ "table-row") {
                if (reader.attributes().value("table:number-rows-repeated").toString().toInt() > 0 ) {
                    //Bos satirlari ekleme
                }
            }

            if (reader.name() EQ "p") {
                QByteArray tmp_byte_array ;
                tmp_byte_array.insert( 0 , reader.readElementText(QXmlStreamReader::IncludeChildElements) );
                column <<  codec->toUnicode( tmp_byte_array ).toUtf8();
            }
        }

        if (reader.isEndElement() EQ true) {
            if (reader.name() EQ "table-row") {
                if ( column.size() > 0 ) {
                    for ( int i = 0 ; i < column.size() ; i++ ) {
                        if ( column.at(i).isEmpty() EQ true OR column.at(i) EQ "@" ) {
                            continue;
                        }

                        rows << column;
                        column.clear();
                        break;
                    }
                }
            }
        }
    }

    //! setlenen kolon sayisi kadar kolon gitmeli
    QList<QStringList>  new_rows;
    if ( rows.at(0).size() < column_size ) {
        for (int i = 0; i < rows.size(); ++i) {
            QStringList row = rows.at(i);
            for (int j = 0; j < column_size - rows.at(0).size(); ++j) {
                row.append(" ");
            }
            new_rows << row;
        }
    }
    else {
        // fazla kolon varsa hepsini gonder
        return rows;
    }

    return new_rows;
}
