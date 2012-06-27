#ifndef ADAK_EXPORTER_H
#define ADAK_EXPORTER_H

#include <QList>

#include "zip.h"
#include "unzip.h"

class QString;
class QStringList;
class QByteArray;

class ADAK_EXPORTER
{
public:
                ADAK_EXPORTER        ();
                ~ADAK_EXPORTER       ();

                void        EXPORT          (  QList<QStringList> rows , QString file_path  );

private:
    QString GET_XML_HEADER      ();
    QString GET_XML_BODY        ( QList<QStringList> rows , QString page_name = "adak_import");
    QString GET_XML_FOOTER      ();

    void    WRITE_FILE_TO_ZIP   (zipFile file,QString file_name,QByteArray data);
};

#endif // ADAK_EXPORTER_H
