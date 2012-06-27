#include <QApplication>
#include "sms_sender_gui.h"

#include "sms.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv );

    //SEND_SMS("asdf", "asdf", "asdfasdf");
    SHOW_SMS_GUI("a","a","a",NULL);

    a.exec();

}

