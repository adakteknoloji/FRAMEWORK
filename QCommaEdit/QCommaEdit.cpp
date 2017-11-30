#include <QtWidgets>
#include <QHBoxLayout>
#include "adak_utils.h"
#include "QCommaEdit.h"
#include <QMessageBox>

/*******************************************************/
/*               QCommaEdit::QCommaEdit                */
/*******************************************************/

QCommaEdit::QCommaEdit(QWidget *parent) : QLineEdit(parent)
{
    p_return_empty             = true;
    p_eksi_deger_girilsin      = false;
    p_virgulsuz_calis          = false;
    p_minus_sign               = '-';

    SET_ALIGNMENT(Qt::AlignRight);

    p_precision = 2;

    connect(this,SIGNAL(textChanged(const QString &)),this,SLOT(SAYIYA_VIRGUL_EKLE(const QString &)));
}

/*******************************************************/
/*               QCommaEdit::~QCommaEdit               */
/*******************************************************/

QCommaEdit::~QCommaEdit()
{

}

/*******************************************************/
/*               QCommaEdit::SET_DOUBLE                */
/*******************************************************/

void QCommaEdit::SET_DOUBLE (const double deger)
{
   QString str;
   str.setNum(deger, 'f', p_precision);
   str.replace(".",DECIMAL_SEPERATOR());
   SET_LINE_EDIT_TEXT (str);
}

/*******************************************************/
/*               QCommaEdit::SET_INTEGER               */
/*******************************************************/

void QCommaEdit::SET_INTEGER (const int deger)
{
   QString str;
   str.setNum(deger);
   SET_LINE_EDIT_TEXT (str);
}

/*******************************************************/
/*               QCommaEdit::SET_PRECISION             */
/*******************************************************/

void QCommaEdit::SET_PRECISION (int precision)
{
    p_precision = precision;
}

/*******************************************************/
/*               QCommaEdit::SET_RETURN_EMPTY          */
/*******************************************************/

void QCommaEdit::SET_RETURN_EMPTY( bool value )
{
    p_return_empty = value;
}

/*******************************************************/
/*            QCommaEdit::SAYIYA_VIRGUL_EKLE           */
/*******************************************************/

void QCommaEdit::SAYIYA_VIRGUL_EKLE(const QString &gelen_sayi)
{
    if (gelen_sayi.isEmpty() EQ true) {
        return;
    }

    QString default_deger = gelen_sayi;
    QChar   last_char     = default_deger[default_deger.size() - 1];

    if (last_char EQ ',' OR last_char EQ '.') {
        if (default_deger.size() EQ 1) { // Sadece virgule veya noktaya basildi.
            default_deger.prepend("0");
        }
        default_deger[default_deger.size() - 1] = DECIMAL_SEPERATOR();        
    }

    if (p_virgulsuz_calis EQ true) {
        if (p_precision EQ 0) {
            SET_LINE_EDIT_TEXT(NUMERIC_OLMAYAN_KARAKTERLERI_SIL(default_deger));
        }
        setText (COMMA_FLOAT_CEVIR(default_deger));
        return;
    }

    SET_LINE_EDIT_TEXT(VIRGUL_EKLE(COMMA_FLOAT_CEVIR(default_deger),-1,p_eksi_deger_girilsin,false));
}

/*******************************************************/
/*             QCommaEdit::COMMA_FLOAT_CEVIR           */
/*******************************************************/

QString QCommaEdit::COMMA_FLOAT_CEVIR(const QString &cevrilecek_sayi)
{
    QString     sayi_float;
    int         sayi_float_index;

    sayi_float_index = 0;

    for (int i = 0 ; i < cevrilecek_sayi.count() ; i++) {
        if (p_eksi_deger_girilsin EQ true AND sayi_float_index EQ 0) {
            if (cevrilecek_sayi[i] EQ p_minus_sign) {
              sayi_float[sayi_float_index] = p_minus_sign;
              sayi_float_index++;
            }
        }

        if (cevrilecek_sayi[i].isNumber() EQ true) {
            sayi_float[sayi_float_index] = cevrilecek_sayi[i];
            sayi_float_index++;
        }

        if (cevrilecek_sayi[i] EQ DECIMAL_SEPERATOR() AND sayi_float_index NE 0 ) {
            if (sayi_float.contains(DECIMAL_SEPERATOR()) EQ false AND p_precision NE 0) {
                sayi_float[sayi_float_index] = DECIMAL_SEPERATOR();
                sayi_float_index++;
            }
        }

    }

    sayi_float.replace(DECIMAL_SEPERATOR(), ".");

    return sayi_float;
}

/*******************************************************/
/*              QCommaEdit::REMOVE_COMMA               */
/*******************************************************/

QString QCommaEdit::REMOVE_COMMA()
{
    QString sayi_comma;

    sayi_comma = COMMA_FLOAT_CEVIR(QLineEdit::text());

    if (p_return_empty EQ true) {
        if (p_precision EQ 0) {
            if (sayi_comma.toInt() EQ 0){
                return "";
            }
        }
        else {
            if (sayi_comma.toDouble() EQ 0.00) {
                return "";
            }
        }
    }

    return sayi_comma;
}

/*******************************************************/
/*              QCommaEdit::setText                    */
/*******************************************************/

void QCommaEdit::setText(const QString &text)
{

    QString temp_text = text;
    temp_text.replace(".",DECIMAL_SEPERATOR());
    SET_LINE_EDIT_TEXT (temp_text);
}

/*******************************************************/
/*              QCommaEdit::UNDO_TEXT                  */
/*******************************************************/

void QCommaEdit::UNDO_TEXT (const QString &text)
{
    setText (text);
}

/*******************************************************/
/*              QCommaEdit::text                       */
/*******************************************************/

const QString QCommaEdit::text()
{
    const QString double_string = REMOVE_COMMA();

    return double_string;
}

/*******************************************************/
/*              QCommaEdit::GET_DOUBLE                 */
/*******************************************************/

double QCommaEdit::GET_DOUBLE()
{
    if ( p_precision EQ 0 ) {
        QMessageBox::warning(this,tr("Warning!"),QString(tr("commaEdit only accepts integer value. You dont want to double value.\nWidget:%1")).arg(objectName()));
    }//Sadece integer değer kabul eden bir commaEdit ten double değer istendi.

    const QString double_string = REMOVE_COMMA();

    if (double_string.isEmpty() EQ true) {
        return 0.0;
    }

    return ROUND (double_string.toDouble(), -1 * p_precision);
}

/*******************************************************/
/*              QCommaEdit::GET_INTEGER                */
/*******************************************************/

int QCommaEdit::GET_INTEGER()
{
    if ( p_precision NE 0 ) {
        QMessageBox::warning(this,tr("Warning!"),QString(tr("commaEdit only accepts double value. You dont want to integer value.\nWidget:%1")).arg(objectName()));
    }//Sadece double değer kabul eden bir commaEdit ten integer değer istendi.

    const QString double_string = REMOVE_COMMA();

    if (double_string.isEmpty() EQ true) {
        return 0;
    }

    return QVariant(double_string).toInt();
}

/*******************************************************/
/*              QCommaEdit::GET_TEXT                   */
/*******************************************************/

QString QCommaEdit::GET_TEXT()
{
    return QLineEdit::text();
}

/*******************************************************/
/*           QCommaEdit::focusInEvent                  */
/*******************************************************/

void QCommaEdit::focusInEvent(QFocusEvent * focus_in_event)
{
    setCursorPosition(0);
    selectAll();

    QLineEdit::focusInEvent(focus_in_event);
}

/*******************************************************/
/*              QCommaEdit::focusOutEvent              */
/*******************************************************/

void QCommaEdit::focusOutEvent(QFocusEvent *focus_out_event)
{
    if ( p_precision EQ 0 ) {
        if (COMMA_FLOAT_CEVIR(QLineEdit::text()) EQ 0 ) {
            if (p_return_empty EQ true) {
                setText("");
            }
        }
        QLineEdit::focusOutEvent(focus_out_event);
        return;
    }
    else {
        if (COMMA_FLOAT_CEVIR(QLineEdit::text()).toDouble() EQ 0.00) {
            if (p_return_empty EQ true) {
                setText("");
            }
            QLineEdit::focusOutEvent(focus_out_event);
            return;
        }
    }

    if (QLineEdit::text().isEmpty() EQ true ) {
        QLineEdit::focusOutEvent(focus_out_event);
        return;
    }

    // GELEN DEGER NE OLURSAN OLSUN ROUNDLUYORUZ
    SET_DOUBLE( ROUND( GET_DOUBLE() , p_precision) );

    QLineEdit::focusOutEvent(focus_out_event);

    return;
}

/*******************************************************/
/*              QCommaEdit::EKSI_DEGER_GIRILSIN        */
/*******************************************************/

void QCommaEdit::EKSI_DEGER_GIRILSIN(bool value)
{
    p_eksi_deger_girilsin = value;
}

/*******************************************************/
/*              QCommaEdit::VIRGULSUZ_CALIS            */
/*******************************************************/

void QCommaEdit::VIRGULSUZ_CALIS(bool value)
{
    p_virgulsuz_calis = value;
}

/*******************************************************/
/*              QCommaEdit::SET_ALIGNMENT              */
/*******************************************************/

void QCommaEdit::SET_ALIGNMENT(Qt::Alignment flag)
{
    setAlignment(flag);
}

/*******************************************************/
/*           QCommaEdit::SET_LINE_EDIT_TEXT            */
/*******************************************************/

void QCommaEdit::SET_LINE_EDIT_TEXT(const QString &str)
{
    int cursor_position = cursorPosition();
    int sagdan_posiyon  = QLineEdit::text().size() - cursor_position;
    QLineEdit::setText (str);
    setCursorPosition(str.size() - sagdan_posiyon);
}

/*******************************************************/
/*         NUMERIC_OLMAYAN_KARAKTERLERI_SIL            */
/*******************************************************/

QString QCommaEdit::NUMERIC_OLMAYAN_KARAKTERLERI_SIL(const QString &cevrilecek_sayi)
{
    QString     sayi;
    int         sayi_index;

    sayi_index = 0;

    for (int i = 0 ; i < cevrilecek_sayi.count() ; i++) {
            if (cevrilecek_sayi[i].isNumber() EQ true) {
                sayi[sayi_index] = cevrilecek_sayi[i];
                sayi_index++;
            }
            if (p_eksi_deger_girilsin EQ true) {
                if (cevrilecek_sayi[0] EQ p_minus_sign AND sayi_index EQ 0) {
                    sayi[sayi_index] = cevrilecek_sayi[i];
                    sayi_index++;
                }
            }
    }

    return sayi;
}
