#include <QtGui>
#include "printer_test.h"
#include "ui_printer_test.h"
#include "dot_matrix.h"

PRINTER_TEST::PRINTER_TEST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PRINTER_TEST)
{
    ui->setupUi(this);

    m_printer.SET_PRINTER_NAME   ("IBM-ProPrinterII");

    connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(SEND_TEXT()));

    connect(ui->radioButton_nlq_quality, SIGNAL(toggled(bool)), this, SLOT(CHANGE_QUALITY()));
    connect(ui->radioButton_normal_quality, SIGNAL(toggled(bool)), this, SLOT(CHANGE_QUALITY()));

    connect(ui->radioButton_compressed, SIGNAL(toggled(bool)), this, SLOT(CHANGE_MODE()));
    connect(ui->radioButton_large, SIGNAL(toggled(bool)), this, SLOT(CHANGE_MODE()));
    connect(ui->radioButton_normal, SIGNAL(toggled(bool)), this, SLOT(CHANGE_MODE()));

}

void PRINTER_TEST::CHANGE_MODE()
{
    QRadioButton *radio_button_mode = static_cast<QRadioButton*>(sender());

    if ( radio_button_mode->text() == "Large") {
        m_printer.SET_PRINTER_MODE(LARGE_MODE);
    }
    else if ( radio_button_mode->text() == "Normal"){
        m_printer.SET_PRINTER_MODE(NORMAL_MODE);
    }
    else {
        m_printer.SET_PRINTER_MODE(CONDENSED_MODE);
    }

}

void PRINTER_TEST::CHANGE_QUALITY()
{
    QRadioButton *radio_button_quality = static_cast<QRadioButton*>(sender());

    if ( radio_button_quality->text() == "Normal") {
        m_printer.SET_PRINTER_QUALITY(NORMAL_QUALITY);
    }
    else {
        m_printer.SET_PRINTER_QUALITY(NLQ_QUALITY);
    }
}

PRINTER_TEST::~PRINTER_TEST()
{
    delete ui;
}

void PRINTER_TEST::SEND_TEXT()
{
    m_printer.OPEN_PRINTER_FILE();
    m_printer.WRITE_PRINTER(m_printer.GET_PREPARE_PRINTER());
    m_printer.WRITE_PRINTER(ui->textEdit->toPlainText());
    m_printer.FINISH_PRINTER();
}
