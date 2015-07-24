#include <QMessageBox>
#include <QPushButton>
#include "adak_utils.h"

/**************************************************************************************
                   ADAK_WARNING
***************************************************************************************/

void ADAK_WARNING (QString message,QObject * focus_widget,QWidget * parent)
{
    //translate QString title = QString("Uyarı");
    QString title = QString(QT_TR_NOOP("Warning"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Warning);
    message_box.setWindowTitle(title);
    message_box.setText(message);

    // translate QPushButton * ok_button = message_box.addButton(QString("Tamam"), QMessageBox::AcceptRole);
    QPushButton * ok_button = message_box.addButton(QString(QT_TR_NOOP("Okey")), QMessageBox::AcceptRole);

    //translate ok_button->setShortcut ( QKeySequence( "T" ));
    ok_button->setShortcut ( QKeySequence( QT_TR_NOOP("O") ));

    message_box.exec();

    if (focus_widget EQ NULL) {
        return;
    }

    static_cast<QWidget *>(focus_widget)->setFocus();
}

/**************************************************************************************
                   ADAK_ERROR
***************************************************************************************/

void ADAK_ERROR (QString message,QObject * focus_widget,QWidget * parent)
{
    //translate QString title = QString("Hata");
    QString title = QString(QT_TR_NOOP("Error"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText(message);

    QPushButton * ok_button = message_box.addButton(QString(QT_TR_NOOP("Okey")), QMessageBox::AcceptRole);

    ok_button->setShortcut ( QKeySequence( QT_TR_NOOP("O") ));

    message_box.exec();

    if (focus_widget EQ NULL) {
        return;
    }

    static_cast<QWidget *>(focus_widget)->setFocus();
}

/**************************************************************************************
                   ADAK_INFO
***************************************************************************************/

void ADAK_INFO (QString message,QObject * focus_widget,QWidget * parent)
{
    //translate QString title = QString("Bilgi");
    QString title = QString(QT_TR_NOOP("Information"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText(message);

    //translate QPushButton * ok_button = message_box.addButton(QString("Tamam"), QMessageBox::AcceptRole);
    QPushButton * ok_button = message_box.addButton(QString(QT_TR_NOOP("Okey")), QMessageBox::AcceptRole);

    //translate ok_button->setShortcut ( QKeySequence( "T" ));
    ok_button->setShortcut ( QKeySequence( QT_TR_NOOP("O") ));

    message_box.exec();

    if (focus_widget EQ NULL) {
        return;
    }

    static_cast<QWidget *>(focus_widget)->setFocus();
}

/**************************************************************************************
                   ADAK_YES_NO
***************************************************************************************/

ADAK_MSG_ENUM ADAK_YES_NO (QString message,QObject * focus_widget,QWidget * parent)
{
    //translate QString title = QString("Karar");
    QString title = QString(QT_TR_NOOP("Decision"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText(message);

    //translate QPushButton * yes_button = message_box.addButton(QString("Evet"), QMessageBox::YesRole);
    //translate QPushButton * no_button  = message_box.addButton(QString("Hayır"), QMessageBox::NoRole);
    QPushButton * yes_button = message_box.addButton(QString(QT_TR_NOOP("Yes")), QMessageBox::YesRole);
    QPushButton * no_button  = message_box.addButton(QString(QT_TR_NOOP("No")), QMessageBox::NoRole);

    //translate yes_button->setShortcut ( QKeySequence( "E" ));
    //translate no_button->setShortcut  ( QKeySequence( "H" ));
    yes_button->setShortcut ( QKeySequence( QT_TR_NOOP("Y") ));
    no_button->setShortcut  ( QKeySequence( QT_TR_NOOP("N") ));

    message_box.exec();

    if (focus_widget NE NULL) {
        static_cast<QWidget *>(focus_widget)->setFocus();
    }

    QAbstractButton * clicked_button = message_box.clickedButton();

    if ( clicked_button EQ (QAbstractButton*)yes_button ) {
        return ADAK_YES;
    }

    return ADAK_NO;
}

/**************************************************************************************
                   ADAK_YES_NO_CANCEL
***************************************************************************************/

ADAK_MSG_ENUM ADAK_YES_NO_CANCEL (QString message,QObject * focus_widget,QWidget * parent)
{
    //translate QString title = QString("Karar");
    QString title = QString(QT_TR_NOOP("Decision"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText(message);

    //translate QPushButton * yes_button     = message_box.addButton(QString("Evet"), QMessageBox::YesRole);
    //translate QPushButton * no_button      = message_box.addButton(QString("Hayır"), QMessageBox::NoRole);
    //translate QPushButton * cancel_button  = message_box.addButton(QString("İptal"), QMessageBox::AcceptRole);
    QPushButton * yes_button     = message_box.addButton(QString(QT_TR_NOOP("Yes")), QMessageBox::YesRole);
    QPushButton * no_button      = message_box.addButton(QString(QT_TR_NOOP("No")), QMessageBox::NoRole);
    QPushButton * cancel_button  = message_box.addButton(QString(QT_TR_NOOP("Cancel")), QMessageBox::AcceptRole);

    //translate yes_button->setShortcut     ( QKeySequence( "E" ));
    //translate no_button->setShortcut      ( QKeySequence( "H" ));
    //translate cancel_button->setShortcut  ( QKeySequence( "'" )); // TURKCE "İ" harfi
    yes_button->setShortcut     ( QKeySequence( QT_TR_NOOP("Y") ));
    no_button->setShortcut      ( QKeySequence( QT_TR_NOOP("N") ));
    cancel_button->setShortcut  ( QKeySequence( QT_TR_NOOP("C") )); // TURKCE "İ" harfi

    message_box.setEscapeButton((QAbstractButton*)cancel_button);

    message_box.exec();

    if (focus_widget NE NULL) {
        static_cast<QWidget *>(focus_widget)->setFocus();
    }

    QAbstractButton * clicked_button = message_box.clickedButton();

    if (clicked_button EQ (QAbstractButton*)yes_button) {
        return ADAK_YES;
    }
    else if (clicked_button EQ (QAbstractButton*)no_button) {
        return ADAK_NO;
    }

    return ADAK_CANCEL;
}

