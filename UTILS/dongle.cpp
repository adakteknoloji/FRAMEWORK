#include <QString>
#ifndef MSVC
    #include <unistd.h>
#endif
#include "adak_defines.h"
#include "adak_utils.h"

struct DONGLE_STRUCT {
    unsigned char a1;
    unsigned char a2;
    unsigned char a3;
    unsigned char a4;
};

static struct DONGLE_STRUCT DESIGNER_DONGLELARI[] = {
    {10,45,249,106},        // Cavit VURAL
    {10,46,53,180},         // Celal SAVUR
    {0,0,0,0}
};

/**************************************************************************************
                   PROGRAMA_UYGUN_DONGLE_VARMI
***************************************************************************************/

bool PROGRAMA_UYGUN_DONGLE_VARMI ()
{
    QString        dongle_id_str;
    QString        temp_str;

    switch (ADAK_DISPLAY_ID()) {
        default               :
        case E9_PROGRAMI      :
            break;
        case ADAK_DESIGNER       :
            break;
            /*
            dongle_id_str = GET_DONGLE_IP ();
            for (int i=0; ; i++) {
                if (DESIGNER_DONGLELARI[i].a1 EQ 0) {
                    return false;
                }
                temp_str = QString("%1.%2.%3.%4").arg(DESIGNER_DONGLELARI[i].a1)
                                                 .arg(DESIGNER_DONGLELARI[i].a2)
                                                 .arg(DESIGNER_DONGLELARI[i].a3)
                                                 .arg(DESIGNER_DONGLELARI[i].a4);
                if (temp_str EQ dongle_id_str) {
                    return true;
                }
            }
            return false;
            */
    }

    return true;
}

/**************************************************************************************
                   GET_DONGLE_IP
***************************************************************************************/

QString GET_DONGLE_IP ()
{
    static QString  str_ip_number = "";

/*
    if (str_ip_number EQ "") {
        char            buf[256];
        unsigned int    dig_ip_number;
        #ifndef ADAK_MACOSX
            if (HL_Serial (buf) EQ 0) {
                dig_ip_number = * (unsigned int *)buf;
                dig_ip_number &= 0x00FFFFFF;
                dig_ip_number |= 0x0A000000;
                str_ip_number = QString("%1.%2.%3.%4").arg(dig_ip_number >> 24)
                                                      .arg((dig_ip_number >> 16) & 0x0FF)
                                                      .arg((dig_ip_number >>  8) & 0x0FF)
                                                      .arg(dig_ip_number         & 0x0FF);
            }
        #endif
    }
*/
    return (str_ip_number);
}
