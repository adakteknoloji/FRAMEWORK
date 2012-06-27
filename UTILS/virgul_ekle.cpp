#include <QLocale>
#include "adak_utils.h"

static bool  g_locale_is_ready               = false;
static QChar g_minus_sign                    = '-';
static QChar g_universal_decimal_seperator   = '.';

static QChar g_thousand_seperator;
static QChar g_decimal_seperator ;


/**************************************************************************************
                   THOUSAND_SEPERATOR
***************************************************************************************/

QChar THOUSAND_SEPERATOR ()
{
    if (g_locale_is_ready EQ false) {
        QLocale   default_seperator;
        g_thousand_seperator     = default_seperator.groupSeparator();
        g_decimal_seperator      = default_seperator.decimalPoint();
        g_locale_is_ready      = true;
    }

    return g_thousand_seperator;
}

/**************************************************************************************
                   DECIMAL_SEPERATOR
***************************************************************************************/

QChar DECIMAL_SEPERATOR ()
{
    if (g_locale_is_ready EQ false) {
        QLocale   default_seperator;
        g_thousand_seperator     = default_seperator.groupSeparator();
        g_decimal_seperator      = default_seperator.decimalPoint();
        g_locale_is_ready      = true;
    }

    return g_decimal_seperator;
}

/**************************************************************************************
                   VIRGUL_EKLE
***************************************************************************************/

QString VIRGUL_EKLE(const QString &cevrilecek_sayi, int precision, bool eksi_deger_girilsin, bool return_empty)
{
    if (return_empty EQ true) {
        if (precision EQ 0) {
            if (cevrilecek_sayi.toInt() EQ 0){
                return "";
            }
        }
        else {
            if (cevrilecek_sayi.toDouble() EQ 0.00) {
                return "";
            }
        }
    }

    QString     cevrilen_sayi;

    if (precision EQ 0) {
        if( cevrilecek_sayi.contains(g_universal_decimal_seperator) ) {
            cevrilen_sayi = cevrilecek_sayi;
            cevrilen_sayi.remove(QChar(g_universal_decimal_seperator),Qt::CaseInsensitive);
            //return cevrilen_sayi;
        }
    }

    int         i;
    int         length;
    int         cevrilen_sayi_index;

    cevrilen_sayi_index         = 0;

    for (i = 0 ; i < cevrilecek_sayi.count() ; i++) {

        if (eksi_deger_girilsin EQ true AND cevrilen_sayi_index EQ 0) {
            if (cevrilecek_sayi[i] EQ g_minus_sign) {
                if (cevrilen_sayi.contains(g_minus_sign) EQ true) {
                    continue;
                }
                cevrilen_sayi[cevrilen_sayi_index] = g_minus_sign;
                cevrilen_sayi_index++;
            }
        }

        if (cevrilecek_sayi[i].isNumber() EQ true) {
            cevrilen_sayi[cevrilen_sayi_index] = cevrilecek_sayi[i];
            cevrilen_sayi_index++;
        }

        if (cevrilecek_sayi[i] EQ g_universal_decimal_seperator AND cevrilen_sayi_index NE 0) {
            if (cevrilen_sayi.contains(g_universal_decimal_seperator) EQ true) {
                continue;
            }
            cevrilen_sayi[cevrilen_sayi_index] = DECIMAL_SEPERATOR();
            cevrilen_sayi_index++;
        }

    }

    length = cevrilen_sayi.length();

    if (cevrilen_sayi.contains(DECIMAL_SEPERATOR()) > 0 ) {
        length = cevrilen_sayi.indexOf(DECIMAL_SEPERATOR());
    }
    else if (cevrilen_sayi.contains(' ') > 0) {
        length = cevrilen_sayi.indexOf(' ');
    }

    if (cevrilen_sayi[0] EQ g_minus_sign) {
        for ( i = length-4; i > 0; i -= 3) {
            cevrilen_sayi.insert(i+1,THOUSAND_SEPERATOR());
        }
    }
    else {
        for ( i = length-3; i > 0; i -= 3) {
            cevrilen_sayi.insert(i,THOUSAND_SEPERATOR());
        }
    }
    if (precision > 0) {
        QString float_part_of_number;

      if ( cevrilen_sayi.contains(DECIMAL_SEPERATOR()) NE true ) {
          cevrilen_sayi.append(DECIMAL_SEPERATOR());
      }
      float_part_of_number = cevrilen_sayi.mid(cevrilen_sayi.indexOf(DECIMAL_SEPERATOR()));

      for (int i=float_part_of_number.count() ; i < precision + 1 ; i++ ) {
          cevrilen_sayi.append("0");
      }
    }

    return cevrilen_sayi;
}
