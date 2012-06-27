#ifndef ADAK_IMPORT_H
#define ADAK_IMPORT_H

#include <QList>
#include <QStringList>

#include "zip.h"
#include "unzip.h"

class QString;

class ADAK_IMPORT
{

public:
      ADAK_IMPORT ();
     ~ADAK_IMPORT ();

      QList< QStringList > IMPORT ( QString file_path , QString text_codec, int column_size );

};

#endif // ADAK_IMPORT_H
