#include <QMessageBox>
#include <QPushButton>
#include "adak_utils.h"

/**************************************************************************************
                   ADAK_WARNING
***************************************************************************************/

void ADAK_WARNING (QString message,QObject * focus_widget,QWidget * parent)
{
    QString title = QObject::tr ("Warning");

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Warning);
    message_box.setWindowTitle(title);
    message_box.setText ( message );

    QPushButton * ok_button = message_box.addButton(QObject::tr ("Okey"), QMessageBox::AcceptRole);

    ok_button->setShortcut ( QObject::tr ("O") );

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
    QString title = QString(QObject::tr ("Error"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText ( message );

    QPushButton * ok_button = message_box.addButton(QObject::tr("Okey"), QMessageBox::AcceptRole);

    ok_button->setShortcut ( QKeySequence( QObject::tr("O") ));

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
    QString title = QString(QObject::tr ("Information"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText ( message );

    QPushButton * ok_button = message_box.addButton(QString(QObject::tr ("Okey")), QMessageBox::AcceptRole);

    ok_button->setShortcut ( QKeySequence( QObject::tr ("O") ));

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
    QString title = QString(QObject::tr ("Decision"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText ( message );

    QPushButton * yes_button = message_box.addButton(QString(QObject::tr ("Yes")), QMessageBox::YesRole);
    QPushButton * no_button  = message_box.addButton(QString(QObject::tr ("No")), QMessageBox::NoRole);

    yes_button->setShortcut ( QKeySequence( QObject::tr ("Y") ));
    no_button->setShortcut  ( QKeySequence( QObject::tr ("N") ));

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
    QString title = QString(QObject::tr ("Decision"));

    QMessageBox message_box (parent);

    message_box.setIcon(QMessageBox::Information);
    message_box.setWindowTitle(title);
    message_box.setText ( message );

    QPushButton * yes_button     = message_box.addButton(QString(QObject::tr ("Yes")), QMessageBox::YesRole);
    QPushButton * no_button      = message_box.addButton(QString(QObject::tr ("No")), QMessageBox::NoRole);
    QPushButton * cancel_button  = message_box.addButton(QString(QObject::tr ("Cancel")), QMessageBox::AcceptRole);

    yes_button->setShortcut     ( QKeySequence( QObject::tr ("Y") ));
    no_button->setShortcut      ( QKeySequence( QObject::tr ("N") ));
    cancel_button->setShortcut  ( QKeySequence( QObject::tr ("C") )); // TURKCE "İ" harfi

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

