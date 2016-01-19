#include <stdio.h>
#include <QString>
#include <iostream>
#include <stdio.h>
#include "dot_matrix.h"
#include "dot_matrix_sabitler.h"

using namespace std;

/**************************************************************************************
                            DOT_MATRIX::DOT_MATRIX
***************************************************************************************/

DOT_MATRIX::DOT_MATRIX()
{
    m_printer_name = "none";

    SET_PRINTER_TYPE   (IBM);
    SET_PRINTER_QUALITY(NORMAL_QUALITY);
    SET_PRINTER_MODE   (NORMAL_MODE);

}

/**************************************************************************************
                            DOT_MATRIX::OPEN_PRINTER_FILE
***************************************************************************************/

FILE * DOT_MATRIX::OPEN_PRINTER_FILE()
{
    #ifdef MINGW
        m_printer_pipe = popen  (m_printer_name.toStdString().c_str(), "w+");
    #elif MSVC
        m_printer_pipe = _popen (m_printer_name.toStdString().c_str(), "w+");
    #else
        m_printer_pipe = popen( QString("lp -d "+ m_printer_name +" -o raw").toStdString().c_str(), "w");
    #endif

    if (!m_printer_pipe) {
        cout << QObject::tr("printer not found").toASCII() << endl;
    }
    return m_printer_pipe;
}

/**************************************************************************************
                            DOT_MATRIX::SET_PRINTER_TYPR
***************************************************************************************/

void DOT_MATRIX::WRITE_PRINTER(QString p_line_str)
{
    //! HER TEXT ICIN BIR PIPE ACILIR VE TEXT YAZDIRILIR
    //! PIPE KAPATILMAYINCAYA KADAR YAZICIYA GONDERMEZ

    if ( m_printer_name == "none"){
        cerr << QObject::tr("Printer name is not set.").toASCII() << endl;
        return;
    }

    fprintf ( m_printer_pipe, "%s", p_line_str.toStdString().c_str());
}


/**************************************************************************************
                            DOT_MATRIX::FINISH_PRINTER
***************************************************************************************/

void DOT_MATRIX::FINISH_PRINTER()
{
    #ifdef MSVC
        if (_pclose (m_printer_pipe) != 0) {
              fprintf (stderr, QObject::tr("Printer connection has been lost.\n").toASCII());
        }
    #else
        if (pclose (m_printer_pipe) != 0) {

              fprintf (stderr, QObject::tr("Printer connection has been lost.\n").toASCII());
        }
    #endif
}

/**************************************************************************************
                            DOT_MATRIX::GET_PRE_TXT
***************************************************************************************/

QString DOT_MATRIX::GET_PREPARE_PRINTER()
{
    QString pre_txt;
    if ( m_printer_type == IBM ) {

        pre_txt.append(GET_IBM_RESET_MODE());

        if ( m_printer_quality == NORMAL_QUALITY ) {
           pre_txt.append( GET_IBM_NORMAL() );
        }
        else { // NLQ_QUALITY
           pre_txt.append( GET_IBM_NLQ());
        }

        if ( m_printer_mode == CONDENSED_MODE ) {
            pre_txt.append(GET_IBM_CONDENSEN_MODE());
        }
        else if( m_printer_mode == LARGE_MODE) { // LARGE_MODE
            pre_txt.append(GET_IBM_LARGE_MODE());
        }
    }
    else { // EPSON

        if ( m_printer_quality == NORMAL_QUALITY ) {
           pre_txt.append( GET_EPSON_NORMAL() );
        }
        else { // NLQ_QUALITY
           pre_txt.append( GET_EPSON_NLQ());
        }

        if ( m_printer_mode == CONDENSED_MODE ) {
            pre_txt.append(GET_EPSON_NORMAL_MODE());
            pre_txt.append(GET_EPSON_CONDENSEN_MODE());
        }
        else if ( m_printer_mode == NORMAL_MODE ) {
           pre_txt.append(GET_EPSON_NORMAL_MODE());
        }
        else { // LARGE_MODE
            pre_txt.append(GET_EPSON_NORMAL_MODE());
            pre_txt.append(GET_EPSON_LARGE_MODE());
        }

    }

    return pre_txt;
}

/**************************************************************************************
                            DOT_MATRIX::SET_PRINTER_NAME
***************************************************************************************/

void DOT_MATRIX::SET_PRINTER_NAME(QString p_name)
{
    m_printer_name = p_name;
}

/**************************************************************************************
                            DOT_MATRIX::SET_PRINTER_TYPR
***************************************************************************************/

void DOT_MATRIX::SET_PRINTER_TYPE( PRINTER_TYPE p_printer_type )
{
    m_printer_type = p_printer_type;
}

/**************************************************************************************
                            DOT_MATRIX::SET_PRINTER_QUALITY
***************************************************************************************/

void DOT_MATRIX::SET_PRINTER_QUALITY( PRINTER_QUALITY p_printer_quality )
{
    m_printer_quality = p_printer_quality;
}

/**************************************************************************************
                            DOT_MATRIX::SET_PRINYER_MODE
***************************************************************************************/

void DOT_MATRIX::SET_PRINTER_MODE( PRINTER_MODE p_printer_mode )
{
    m_printer_mode = p_printer_mode;
}

/**************************************************************************************
                            DOT_MATRIX::GET_PRINTER_TYPE
***************************************************************************************/

PRINTER_TYPE DOT_MATRIX::GET_PRINTER_TYPE()
{
    return m_printer_type;
}

/**************************************************************************************
                            DOT_MATRIX::GET_PRINTER_QUALITY
***************************************************************************************/

PRINTER_QUALITY DOT_MATRIX::GET_PRINTER_QUALITY()
{
    return m_printer_quality;
}

/**************************************************************************************
                            DOT_MATRIX::GET_PRINTER_MODE
***************************************************************************************/

PRINTER_MODE DOT_MATRIX::GET_PRINTER_MODE()
{
    return m_printer_mode;
}
