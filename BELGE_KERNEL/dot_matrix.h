#ifndef DOT_MATRIX_H
#define DOT_MATRIX_H

#include <QString>
#include "../INCLUDE/belge_kernel_defines.h"

class DOT_MATRIX
{
public:
    DOT_MATRIX();

private:
    PRINTER_TYPE     m_printer_type;
    PRINTER_QUALITY  m_printer_quality;
    PRINTER_MODE     m_printer_mode;
    FILE *           m_printer_pipe;
    QString          m_printer_name;

public:
    FILE * OPEN_PRINTER_FILE();

public:

    void SET_PRINTER_NAME   (QString p_name );
    void SET_PRINTER_TYPE   ( PRINTER_TYPE p_printer_type );
    void SET_PRINTER_QUALITY( PRINTER_QUALITY p_printer_quality );
    void SET_PRINTER_MODE   ( PRINTER_MODE p_printer_mode );

    QString         GET_PREPARE_PRINTER();

    PRINTER_TYPE    GET_PRINTER_TYPE();
    PRINTER_QUALITY GET_PRINTER_QUALITY();
    PRINTER_MODE    GET_PRINTER_MODE();

    void WRITE_PRINTER(QString p_line_str);
    void FINISH_PRINTER();
};

#endif // DOT_MATRIX_H
