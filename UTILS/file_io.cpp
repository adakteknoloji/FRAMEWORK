#include <QFile>
#include "adak_utils.h"

/********************************************/
/*              FILE_EXISTS                 */
/********************************************/

bool  FILE_EXISTS ( const QString& file_path )
{
    return QFile::exists( file_path );
}

/**********************************************/
/*              DELETE_FILE                   */
/**********************************************/

bool DELETE_FILE ( const QString& file_path )
{
    return QFile::remove(file_path);
}

