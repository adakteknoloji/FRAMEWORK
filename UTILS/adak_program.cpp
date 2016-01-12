#include <QDate>
#include <QTime>
#include <QObject>
#include "adak_utils.h"
#include "adak_defines.h"

struct ADAK_PROGRAM_STRUCT {
    QString program_name;
    QString executable;
    QString shortname;
    QString version;
    bool    need_dongle;
};

#define ADAK_VERSION "3.1"

static struct ADAK_PROGRAM_STRUCT  ADAK_PROGRAM_LIST[NUM_OF_ADAK_PROGRAMS] =
{

    {QString::fromUtf8("Yonetim / libADAK")            , "libADAK"       , "libADAK"     , ADAK_VERSION, false  },

    {QString::fromUtf8("Adak Designer")                , "ADAK_DESIGNER" , "DESIGNER"    , ADAK_VERSION, false  },

    {QString::fromUtf8("E9 [ CRM + e-ticaret ]")       , "E9"            , "E9"          , ADAK_VERSION, false  },

    {""                                                , ""              , ""            , ""          , false  },

    {QString::fromUtf8("Ultimate Tests for Android's Functionality and Stability")      , "uTAFS"          , "uTAFS"         , ADAK_VERSION, false  },

    {""        , ""        , "" , "", false  }, // 5
    {""        , ""        , "" , "", false  }, // 6
    {""        , ""        , "" , "", false  }, // 7
    {""        , ""        , "" , "", false  }, // 9
    {""        , ""        , "" , "", false  }, // 10
    {""        , ""        , "" , "", false  }, // 11
    {""        , ""        , "" , "", false  }, // 12
    {""        , ""        , "" , "", false  }, // 13
    {""        , ""        , "" , "", false  }, // 14
    {""        , ""        , "" , "", false  }, // 15
    {""        , ""        , "" , "", false  }, // 16
    {""        , ""        , "" , "", false  }, // 17
    {""        , ""        , "" , "", false  }, // 18
    {""        , ""        , "" , "", false  }, // 19
    {""        , ""        , "" , "", false  }, // 20
    {""        , ""        , "" , "", false  }, // 21
    {""        , ""        , "" , "", false  }, // 22
    {""        , ""        , "" , "", false  }, // 23
    {""        , ""        , "" , "", false  }, // 24
    {""        , ""        , "" , "", false  }, // 25
    {""        , ""        , "" , "", false  }, // 26
    {""        , ""        , "" , "", false  }, // 27
    {""        , ""        , "" , "", false  }, // 28
    {""        , ""        , "" , "", false  }, // 29
    {""        , ""        , "" , "", false  }, // 30
    {""        , ""        , "" , "", false  }, // 31
    {""        , ""        , "" , "", false  }, // 32
    {""        , ""        , "" , "", false  }, // 33
    {""        , ""        , "" , "", false  }, // 34
    {""        , ""        , "" , "", false  }, // 35
    {""        , ""        , "" , "", false  }, // 36
    {""        , ""        , "" , "", false  }, // 37
    {""        , ""        , "" , "", false  }, // 38
    {""        , ""        , "" , "", false  }, // 39
    {""        , ""        , "" , "", false  }, // 40
    {""        , ""        , "" , "", false  }, // 41
    {""        , ""        , "" , "", false  }, // 42
    {""        , ""        , "" , "", false  }, // 43
    {""        , ""        , "" , "", false  }, // 44
    {""        , ""        , "" , "", false  }, // 45
    {""        , ""        , "" , "", false  }, // 46
    {""        , ""        , "" , "", false  }, // 47
    {""        , ""        , "" , "", false  }, // 48
    {""        , ""        , "" , "", false  }, // 49
    {""        , ""        , "" , "", false  }, // 50
    {""        , ""        , "" , "", false  }, // 51
    {""        , ""        , "" , "", false  }, // 52
    {""        , ""        , "" , "", false  }, // 53
    {""        , ""        , "" , "", false  }, // 54
    {""        , ""        , "" , "", false  }, // 55
    {""        , ""        , "" , "", false  }, // 56
    {""        , ""        , "" , "", false  }, // 57
    {""        , ""        , "" , "", false  }, // 58
    {""        , ""        , "" , "", false  }, // 59
    {""        , ""        , "" , "", false  }, // 60
    {""        , ""        , "" , "", false  }, // 61
    {""        , ""        , "" , "", false  }, // 62
    {""        , ""        , "" , "", false  }, // 63
    {""        , ""        , "" , "", false  }, // 64
    {""        , ""        , "" , "", false  }, // 65
    {""        , ""        , "" , "", false  }, // 66
    {""        , ""        , "" , "", false  }, // 67
    {""        , ""        , "" , "", false  }, // 68
    {""        , ""        , "" , "", false  }, // 69
    {""        , ""        , "" , "", false  }, // 70
    {""        , ""        , "" , "", false  }, // 71
    {""        , ""        , "" , "", false  }, // 72
    {""        , ""        , "" , "", false  }, // 73
    {""        , ""        , "" , "", false  }, // 74
    {""        , ""        , "" , "", false  }, // 75
    {""        , ""        , "" , "", false  }, // 76
    {""        , ""        , "" , "", false  }, // 77
    {""        , ""        , "" , "", false  }, // 78
    {""        , ""        , "" , "", false  }, // 79
    {""        , ""        , "" , "", false  }, // 80
    {""        , ""        , "" , "", false  }, // 81
    {""        , ""        , "" , "", false  }, // 82
    {""        , ""        , "" , "", false  }, // 83
    {""        , ""        , "" , "", false  }, // 84
    {""        , ""        , "" , "", false  }, // 85
    {""        , ""        , "" , "", false  }, // 86
    {""        , ""        , "" , "", false  }, // 87
    {""        , ""        , "" , "", false  }, // 88
    {""        , ""        , "" , "", false  }, // 89
    {""        , ""        , "" , "", false  }, // 90
    {""        , ""        , "" , "", false  }, // 91
    {""        , ""        , "" , "", false  }, // 92
    {""        , ""        , "" , "", false  }, // 93
    {""        , ""        , "" , "", false  }, // 94
    {""        , ""        , "" , "", false  }, // 95
    {""        , ""        , "" , "", false  }, // 96
    {""        , ""        , "" , "", false  }, // 97
    {""        , ""        , "" , "", false  }, // 98

    {QString::fromUtf8 ("Test / Deneme Programı")        , "TEST"          , "TEST"     , "11.01.a", false }

};


/**************************************************************************************
                   ADAK_ISLETIM_SISTEMI
***************************************************************************************/

QString ADAK_ISLETIM_SISTEMI ()
{
#ifdef WIN32
    return "Windows Xp/Vista/7/8";
#endif
#ifdef LNX32
    return "LINUX 32Bit";
#endif
#if defined (LNX64)
    return "LINUX 64Bit";
#endif
#ifdef MACOSX
    return "MacOS X";
#endif
    return "UNKNOWN ???";
}

/**************************************************************************************
                   SET_ADAK_VERITABANI_ID
***************************************************************************************/

static int g_adak_veritabani_id = -1;
void SET_ADAK_VERITABANI_ID (int adak_program_id)
{
    g_adak_veritabani_id = adak_program_id;
}

/**************************************************************************************
                   SET_ADAK_DISPLAY_ID
***************************************************************************************/

static int g_adak_display_id = -1;
void SET_ADAK_DISPLAY_ID (int adak_display_id)
{
    g_adak_display_id = adak_display_id;
}

/**************************************************************************************
                   ADAK_VERITABANI_ID
***************************************************************************************/

int ADAK_VERITABANI_ID ()
{
    return g_adak_veritabani_id;
}

/**************************************************************************************
                   ADAK_DISPLAY_ID
***************************************************************************************/

int ADAK_DISPLAY_ID ()
{
    return g_adak_display_id;
}

/**************************************************************************************
                   ADAK_PROGRAM_SHORTNAME
***************************************************************************************/

QString ADAK_PROGRAM_SHORTNAME (int adak_program_id)
{
    return ADAK_PROGRAM_LIST[adak_program_id].shortname;
}

/**************************************************************************************
                   ADAK_PROGRAM_LONGNAME
***************************************************************************************/

QString ADAK_PROGRAM_LONGNAME (int adak_program_id)
{
    return ADAK_PROGRAM_LIST[adak_program_id].program_name;
}

/**************************************************************************************
                   ADAK_PROGRAM_EXE
***************************************************************************************/

QString ADAK_PROGRAM_EXE (int adak_program_id)
{
    return ADAK_PROGRAM_LIST[adak_program_id].executable;
}

/**************************************************************************************
                   ADAK_PROGRAM_EMAIL
***************************************************************************************/

QString ADAK_PROGRAM_EMAIL (int adak_program_id)
{
    switch (adak_program_id) {
        case E9_PROGRAMI :
            return "e9@adak.com.tr";
        default :
            return "info@adak.com.tr";
    }
}

/**************************************************************************************
                   ADAK_PROGRAM_WEB
***************************************************************************************/

QString ADAK_PROGRAM_WEB (int adak_program_id)
{
    switch (adak_program_id) {
        case E9_PROGRAMI :
            return "http://www.adak.com.tr";
        default :
            return "http://www.adak.com.tr";
    }
}

/**************************************************************************************
                   ADAK_PROGRAM_VERSION
***************************************************************************************/

QString ADAK_PROGRAM_VERSION (int adak_program_id)
{
    return ADAK_PROGRAM_LIST[adak_program_id].version;
}

/**************************************************************************************
                   ADAK_PROGRAM_NEED_DONGLE
***************************************************************************************/

bool ADAK_PROGRAM_NEED_DONGLE ()
{
    return ADAK_PROGRAM_LIST[g_adak_display_id].need_dongle;
}

/**************************************************************************************
                   GET_REPORT_TOP_RIGHT_HEADER
***************************************************************************************/

QString GET_REPORT_TOP_RIGHT_HEADER ()
{
    QString tarih               = QDate::currentDate().toString("dd/MMMM/yyyy");
    QString saat                = QTime::currentTime().toString("hh:mm");

    return tarih + " " + saat + " " + ADAK_PROGRAM_SHORTNAME(g_adak_display_id) + " " + ADAK_PROGRAM_VERSION(g_adak_display_id);
}

