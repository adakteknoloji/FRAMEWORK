#include "print.h"
#include "adak_defines.h"

/**************************************************************************************
                               FILL_PAPERSIZE_COMBOBOX
***************************************************************************************/

void FILL_PAPERSIZE_COMBOBOX ( QComboBox * combo_box )
{
    QStringList paper_size;
    paper_size << "A3" << "A4" << "A5" << "B3" << "B4" << "B5";

    combo_box->addItems(paper_size);
    combo_box->setCurrentIndex(1);
}

/**************************************************************************************
                                 GET_PAPERSIZE_ENUM
***************************************************************************************/

int GET_PAPERSIZE_ENUM ( QString paper_size)
{
    if ( paper_size EQ "A3" ) {
        return QPrinter::A3;
    }
    if ( paper_size EQ "A4" ) {
        return QPrinter::A4;
    }
    if ( paper_size EQ "A5" ) {
        return QPrinter::A5;
    }
    if ( paper_size EQ "B3" ) {
        return QPrinter::B3;
    }
    if ( paper_size EQ "B4" ) {
        return QPrinter::B4;
    }
    if ( paper_size EQ "B5" ) {
        return QPrinter::B5;
    }

    return -1;
}
