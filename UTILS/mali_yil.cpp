#include <QDate>
#include "adak_utils.h"

static QDate m_mali_yil_first_date  = QDate::fromString("01.01.1999", "ddMMyyyyy");
static QDate m_mali_yil_365_date    = QDate::fromString("01.01.1999", "ddMMyyyyy");
static QDate m_array_last_date      = QDate::fromString("01.01.1999", "ddMMyyyyy");
static int   mali_yil               = 1999;


/****************************************************************
               SET_MALI_YIL_FIRST_DATE
****************************************************************/

void SET_MALI_YIL_FIRST_DATE( QDate p_mali_yil_first_date )
{
    m_mali_yil_first_date   = p_mali_yil_first_date;

    m_mali_yil_365_date     = p_mali_yil_first_date.addYears(  1 );
    m_mali_yil_365_date     = m_mali_yil_365_date.addDays ( -1 );

    m_array_last_date       = p_mali_yil_first_date.addDays( 499 ); // 501 -2 (Acilis / Kapanis Fisleri)

    mali_yil                = p_mali_yil_first_date.year();

}

/****************************************************************
               MALI_YIL_ARRAY_INDIS
****************************************************************/

int MALI_YIL_ARRAY_INDIS (QDate p_date )
{
    if (p_date < m_mali_yil_first_date) {
        return -1; // Will crash
    }
    if (p_date > m_array_last_date) {
        return -1; // Will crash
    }

    return m_mali_yil_first_date.daysTo ( p_date ) + 1;
}

/****************************************************************
               MALI_YIL_FIRST_DATE
****************************************************************/

QDate MALI_YIL_FIRST_DATE()
{
    return m_mali_yil_first_date;
}

/****************************************************************
               MALI_YIL_365_DATE
****************************************************************/

QDate MALI_YIL_365_DATE ()
{
    return m_mali_yil_365_date;
}

/****************************************************************
               MALI_ARRAY_LAST_DATE
****************************************************************/

QDate MALI_ARRAY_LAST_DATE()
{
    return m_array_last_date;
}

/****************************************************************
               MALI_YIL
****************************************************************/

int MALI_YIL ()
{
    return mali_yil;
}

