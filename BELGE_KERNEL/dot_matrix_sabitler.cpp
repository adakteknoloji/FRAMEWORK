#include "dot_matrix_sabitler.h"
#include <QString>

#define     IBM_NORMAL_INS  4
#define     IBM_NLQ_INS     2
#define     IBM_NORMAL_MODE 4


static char S_IBM_NORMAL[IBM_NORMAL_INS] = { 18, 20, 27, 72 };
static char S_IBM_NLQ   [IBM_NLQ_INS]    = { 27, 71 };

static char S_IBM_RESET_MODE[IBM_NORMAL_MODE] = { 18, 20, 27, 72 };
static char S_IBM_LARGE_MODE[1]      = { 14 };
static char S_IBM_CONDENSEN_MODE[1]  = { 15 };


static char S_EPSON_NORMAL[7]        = { 18, 27, 87, 48, 27, 'x', 1 };
static char S_EPSON_NLQ[3]           = { 27, 'x', 1 };

static char S_EPSON_NORMAL_MODE[4]   = { 18, 27, 87, 48 };
static char S_EPSON_LARGE_MODE[3]    = { 27, 87, 49 };
static char S_EPSON_CONDENSEN_MODE[2]= { 27, 15 };


/**********************************************************
                    S_GET_IBM_NORMAL
***********************************************************/

QString GET_IBM_NORMAL()
{
    QString code;
    for (int i = 0; i < IBM_NORMAL_INS; ++i) {
        code.append(S_IBM_NORMAL[i]);
    }
    return code;
}

/**********************************************************
                    S_GET_IBM_NLQ
***********************************************************/

QString GET_IBM_NLQ()
{
    QString code;
    for (int i = 0; i < IBM_NLQ_INS; ++i) {
        code.append(S_IBM_NLQ[i]);
    }
    return code;
}

/**********************************************************
                    S_GET_IBM_NORMAL_MODE
***********************************************************/

QString GET_IBM_RESET_MODE()
{
    QString code;
    for (int i = 0; i < IBM_NORMAL_MODE; ++i) {
        code.append( S_IBM_RESET_MODE[i]);
    }
    return code;
}

/**********************************************************
                    S_GET_IBM_LARGE_MODE
***********************************************************/

QString GET_IBM_LARGE_MODE()
{
    QString code;
    code.append( S_IBM_LARGE_MODE[0]);
    return code;
}

/**********************************************************
                    S_GET_IBM_CONDENSEN_MODE
***********************************************************/

QString GET_IBM_CONDENSEN_MODE()
{
    QString code;
    code.append(S_IBM_CONDENSEN_MODE[0]);
    return code;
}


/**********************************************************
                    S_GET_EPSON_NORMAL
***********************************************************/

QString GET_EPSON_NORMAL()
{
   QString code;
   for (int i = 0; i < 7; ++i) {
       code.append(S_EPSON_NORMAL[i]);
   }
   return code;
}

/**********************************************************
                    S_GET_EPSON_NLQ
***********************************************************/

QString GET_EPSON_NLQ()
{
    QString code;
    for (int i = 0; i < 3; ++i) {
        code.append(S_EPSON_NLQ[i]);
    }
    return code;
}

/**********************************************************
                    S_GET_EPSON_NORMAL_MODE
***********************************************************/

QString GET_EPSON_NORMAL_MODE()
{
    QString code;
    for (int i = 0; i < 4; ++i) {
        code.append( S_EPSON_NORMAL_MODE[i]);
    }
    return code;
}

/**********************************************************
                    S_GET_EPSON_LARGE_MODE
***********************************************************/

QString GET_EPSON_LARGE_MODE()
{
    QString code;
    for (int i = 0; i < 3; ++i) {
        code.append( S_EPSON_LARGE_MODE[i]);
    }
    return code;
}

/**********************************************************
                    S_GET_EPSON_CONDENSEN_MODE
***********************************************************/

QString GET_EPSON_CONDENSEN_MODE()
{
    QString code;
    for (int i = 0; i < 2; ++i) {
        code.append( S_EPSON_CONDENSEN_MODE[i]);
    }
    return code;
}
