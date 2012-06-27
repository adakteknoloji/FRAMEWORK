#include <QtGui>
#include <QHBoxLayout>
#include "adak_defines.h"
#include "QLimitedTextEdit.h"

/*****************************************************************************/
/*               QLimitedTextEdit::QLimitedTextEdit                          */
/*****************************************************************************/

QLimitedTextEdit::QLimitedTextEdit(QWidget *parent) : QTextEdit(parent)
{
    SET_MAX_STRING_LENGTH(1);
    SET_LINE_LIMIT(-1);

    connect(this,SIGNAL(textChanged()),this,SLOT(TEXT_EDIT_CHANGED()));
}

/*****************************************************************************/
/*               QLimitedTextEdit::~QLimitedTextEdit                         */
/*****************************************************************************/

QLimitedTextEdit::~QLimitedTextEdit()
{

}

/*****************************************************************************/
/*               QLimitedTextEdit::SET_MAX_STRING_LENGTH                     */
/*****************************************************************************/

void QLimitedTextEdit::SET_MAX_STRING_LENGTH(int max_string_length)
{
    m_max_str_length = max_string_length;
    m_remaing_length = max_string_length;
}

/*****************************************************************************/
/*               QLimitedTextEdit::SET_MAX_STRING_LENGTH                     */
/*****************************************************************************/

void QLimitedTextEdit::SET_LINE_LIMIT(int line_limit)
{
    m_line_limit = line_limit;
}

/*****************************************************************************/
/*               QLimitedTextEdit::SET_MAX_STRING_LENGTH                     */
/*****************************************************************************/

int QLimitedTextEdit::GET_REMAINING_LENGTH()
{
    return m_remaing_length;
}

/*****************************************************************************/
/*               QLimitedTextEdit::TEXT_EDIT_CHANGED                         */
/*****************************************************************************/
//Limit asildigi zaman yapilmasi gerekenleri yapar
void QLimitedTextEdit::TEXT_EDIT_CHANGED()
{
    QString text = document()->toPlainText();

    if ( text.length() > m_max_str_length ) {
        text.resize(m_max_str_length - 1);
        SET_TEXT(text);
    }

    if (m_line_limit NE -1) {
        if (text.count("\n") >= m_line_limit ) {
            QStringList lines = text.split("\n");
            text.clear();
            text.append(lines.at(0));
            for ( int i = 1 ; i < m_line_limit ; i++ ) {
                text.append("\n" + lines.at(i));
            }
            SET_TEXT(text);
        }
    }

    m_remaing_length = m_max_str_length - text.length();
}

/*****************************************************************************/
/*               QLimitedTextEdit::SET_MAX_STRING_LENGTH                     */
/*****************************************************************************/

void QLimitedTextEdit::SET_TEXT(QString str)
{
    blockSignals(true);
    setPlainText(str);
    blockSignals(false);
    moveCursor(QTextCursor::End);
}

/*****************************************************************************/
/*               QLimitedTextEdit::keyPressEvent                             */
/*****************************************************************************/
// Eventlari kontrol eder sadece
void QLimitedTextEdit::keyPressEvent(QKeyEvent * e)
{
    QString text = document()->toPlainText();

    if ( text.length() > m_max_str_length ) {
        return;
    }

    if (e->key() EQ Qt::Key_Enter OR e->key() EQ Qt::Key_Return) {
        if (m_line_limit NE -1) {
            if (text.count("\n") >= m_line_limit - 1 ) {
                return;
            }
        }
    }

    QTextEdit::keyPressEvent(e);
}
