#include <QTemporaryFile>
#include <QList>
#include <QString>
#include <QDir>
#include "adak_utils.h"


static QList<QTemporaryFile *> temporary_file;
#define MAX_TEMP_FILE 10


/**************************************************************************************
                  ADAK_CREATE_TEMP_FILE
***************************************************************************************/

QString ADAK_CREATE_TEMP_FILE (QByteArray bytearray)
{
    for ( int i = 0 ; i < temporary_file.size() ; i++ ) {
        QByteArray tmp_file = ADAK_PIXMAP_2_BYTE_ARRAY(temporary_file.at(i)->fileName());
        if (bytearray EQ tmp_file) {
            return temporary_file.at(i)->fileName();
        }
    }

    if (temporary_file.size() EQ MAX_TEMP_FILE) {
        temporary_file.pop_front();
    }

    temporary_file << new QTemporaryFile;

    temporary_file.last()->setFileTemplate(QString("%1/XXXXXX").arg(QDir::tempPath()));

    temporary_file.last()->open();

    temporary_file.last()->write(bytearray);

    temporary_file.last()->close();

    return temporary_file.last()->fileName();
}


/**************************************************************************************
                  ADAK_REMOVE_TEMP_FILES
***************************************************************************************/

void ADAK_REMOVE_TEMP_FILES ()
{
    for ( int i = 0 ; i < temporary_file.size() ; i++ ) {
        delete temporary_file.at(i);
    }
}
