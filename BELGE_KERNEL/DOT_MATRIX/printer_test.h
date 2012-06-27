#ifndef PRINTER_TEST_H
#define PRINTER_TEST_H


#include <QWidget>
#include "dot_matrix.h"

namespace Ui {
    class PRINTER_TEST;
}

class PRINTER_TEST : public QWidget
{
    Q_OBJECT

public:
    explicit PRINTER_TEST(QWidget *parent = 0);
    ~PRINTER_TEST();

private:
    Ui::PRINTER_TEST *ui;

    DOT_MATRIX m_printer;

private slots:
    void SEND_TEXT();
    void CHANGE_QUALITY();
    void CHANGE_MODE();
};

#endif // PRINTER_TEST_H
