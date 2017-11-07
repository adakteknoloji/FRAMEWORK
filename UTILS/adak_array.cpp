#include <QStringList>
#include <QVariant>
#include <QByteArray>
#include <QDebug>
#include "adak_utils.h"

static const QString            DELIMITER  = QString('\x03');
static const QString            DUPLICATOR = QString('\x04');

static QStringList              UNPACK_ARRAY ( const QString& array_string );

/**************************************************************************************
                   IS_DATA_COMPRESSED
***************************************************************************************/

bool IS_DATA_COMPRESSED ( const QByteArray& data_array )
{
    QByteArray byte_array;

    if ( data_array.size() < 6 ) {
        return false;
    }

    byte_array.append(data_array.at(4));
    byte_array.append(data_array.at(5));

    QByteArray header_hex = byte_array.toHex();

    if ( header_hex EQ "789c") {
        return true;
    }

    return false;
}

/**************************************************************************************
                   UNPACK_INT_ARRAY
***************************************************************************************/

void UNPACK_INT_ARRAY ( const QString& array_string , int * int_array, int array_size )
{
    memset (int_array, 0, sizeof (int) * array_size);

    QByteArray compressed_data  = QByteArray::fromHex(QVariant(array_string).toByteArray());

    if ( IS_DATA_COMPRESSED(compressed_data) EQ true ) {
        QByteArray uncompressed_data = qUncompress(compressed_data);
        if ( uncompressed_data.size() NE 0 )  {
            memcpy ((char *)int_array , (char *)uncompressed_data.constData (), uncompressed_data.size());
        }
        return;
    }


    QStringList     values = UNPACK_ARRAY (array_string);

    qDebug() << "values.size() : " << values.size();

    if (values.size() > 0) { // Empty field
        for (int i=0; i < values.size(); i++) {
            int_array[i] = values[i].toInt();
        }
    }
}

/**************************************************************************************
                   UNPACK_DOUBLE_ARRAY
***************************************************************************************/

void UNPACK_DOUBLE_ARRAY ( const QString& array_string , double * double_array, int array_size )
{
    memset (double_array, 0, sizeof (double) * array_size);


    QByteArray compressed_data  = QByteArray::fromHex(QVariant(array_string).toByteArray());

    if ( IS_DATA_COMPRESSED(compressed_data) EQ true ) {
        QByteArray uncompressed_data = qUncompress(compressed_data);
        if ( uncompressed_data.size() NE 0 )  {
            memcpy ((char *)double_array , (char *)uncompressed_data.constData (), uncompressed_data.size());
        }
        return;
    }

    QStringList     values = UNPACK_ARRAY (array_string);

    if (values.size() > 0) { // Empty field
        for (int i=0; i < values.size(); i++) {
            double_array[i] = values[i].toDouble();
        }
    }
}

/**************************************************************************************
                   UNPACK_STRING_ARRAY
***************************************************************************************/

void UNPACK_STRING_ARRAY ( const QString& array_string , QString * string_array, int array_size )
{

    for ( int i = 0; i < array_size; i++ ) {
        string_array[i] = "";
    }

    QStringList     values = UNPACK_ARRAY (array_string);

    if (values.size() > 0) { // Empty field
        for (int i=0; i < values.size(); i++) {
            string_array[i] = values[i];
        }
    }
}

/**************************************************************************************
                   UNPACK_ARRAY
***************************************************************************************/

static QStringList UNPACK_ARRAY ( const QString& array_string )
{
    QStringList     values;
    QString         value="";

    bool   last_element_is_in_value = false;
    for (int i=0; i < array_string.size(); i++) {
        if (QString(array_string[i]) EQ DELIMITER) {
            values.append(value);
            last_element_is_in_value = false;
            value="";
            continue;
        }
        if (QString(array_string[i]) EQ DUPLICATOR) {
            values.append(value);
            if (i > 0) {
                if (QString(array_string[i-1]) NE DUPLICATOR) {
                    values.append(value);
                }
            }
            last_element_is_in_value = false;
            continue;
        }
        if (i > 0) {
            if (QString(array_string[i-1]) EQ DUPLICATOR) {
                value="";
            }
        }
        value += array_string[i];
        last_element_is_in_value = true;
    }
    if (last_element_is_in_value EQ true) {
        values.append(value);
    }

    return values;
}


/**************************************************************************************
                   PACK_INT_ARRAY
***************************************************************************************/

QByteArray  PACK_INT_ARRAY ( int * int_array, int array_size )
{
    return QVariant(qCompress( (uchar *)int_array, array_size * sizeof (int)).toHex()).toByteArray();
}

/**************************************************************************************
                   PACK_DOUBLE_ARRAY
***************************************************************************************/

QByteArray  PACK_DOUBLE_ARRAY ( double * double_array, int array_size )
{
    return QVariant(qCompress( (uchar *)double_array, array_size * sizeof (double)).toHex()).toByteArray();
}


// Ilk string bos ise !...
/**************************************************************************************
                   PACK_STRING_ARRAY
***************************************************************************************/

QByteArray  PACK_STRING_ARRAY ( QString * str_array, int array_size )
{
    QString      array_string;
    QString      onceki;

    array_string = str_array[0];
    onceki        = str_array[0];

    for (int i = 1; i < array_size; i++) {
        array_string += DELIMITER;
        array_string += str_array[i];
        onceki        = str_array[i];
    }
    return QVariant(array_string).toByteArray();
}

