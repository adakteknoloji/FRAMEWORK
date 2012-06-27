#ifndef SQL_TABLO_ARAMA_H
#define SQL_TABLO_ARAMA_H

#include "arama_kernel.h"

namespace Ui {
    class SQL_TABLO_ARAMA;
}

class SQL_TABLO_ARAMA: public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           SQL_TABLO_ARAMA             ( QWidget * parent = 0 );
                           ~SQL_TABLO_ARAMA            () {}

    int           *        m_secilen_id;
private:
    Ui::SQL_TABLO_ARAMA *  m_ui;

    void   		   SETUP_FORM                          	      ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();

    int                    SINGLE_LINE_SELECTED                       ( int selected_row_number );

};

#endif // SQL_TABLO_ARAMA_H
