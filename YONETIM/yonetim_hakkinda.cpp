#include "yonetim_hakkinda.h"
#include "ui_yonetim_hakkinda.h"
#include "adak_utils.h"
#include <QDateEdit>

/****************************************/
/*           OPEN_YONETIM_HAKKIMIZDA            */
/****************************************/

void OPEN_YONETIM_HAKKIMIZDA( QWidget * parent )
{
    YONETIM_HAKKIMIZDA * H = new YONETIM_HAKKIMIZDA( parent );
    H->exec();
}

/**************************************************/
/*           YONETIM_HAKKIMIZDA::YONETIM_HAKKIMIZDA         */
/**************************************************/

YONETIM_HAKKIMIZDA::YONETIM_HAKKIMIZDA( QWidget *parent) : QDialog(parent), m_ui(new Ui::YONETIM_HAKKIMIZDA)
{
    m_ui->setupUi ( this );

    QString surum = QString (tr("Version: ")).append ( ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()) );

    QLocale locale(QLocale::English);
    QString tarih    = (locale.toDate(__DATE__,"MMM  d yyyy")).toString("dd MMMM yyyy");

    if (tarih.isEmpty() EQ true) {
        //MMM__d ustekinde 2 space varken burda tek space var MMM_d
        tarih    = (locale.toDate(__DATE__,"MMM d yyyy")).toString("dd MMMM yyyy");
    }

    QString saat  = QString (" Saat: " + QString(__TIME__));
    surum += "\n - " + tarih + saat;
    ADAK_SET_FONT_SIZE( m_ui->label_surum, 1.00 );
    m_ui->label_surum->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
    m_ui->label_surum->setFrameShape(QFrame::StyledPanel);
    m_ui->label_surum->setTextFormat(Qt::RichText);
    m_ui->label_surum->setText(surum);
    m_ui->label_web_adresi->setText ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" />"
                                     "<style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style>"
                                     "</head><body style=\" font-family:'Sans Serif'; font-size:13pt; font-weight:400; font-style:normal;\">"
                                     "\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                     "<a href=\""+ ADAK_PROGRAM_WEB(ADAK_DISPLAY_ID()) + "\">"
                                     "<span style=\" text-decoration: underline; color:#0000c0;\">" + ADAK_PROGRAM_WEB(ADAK_DISPLAY_ID()) +
                                     "</span></a></p></body></html>");

    m_ui->label_posta_adresi->setText ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head>"
                                       "<body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
                                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                       "<a href=\"mailto:"+ ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()) + "\">"
                                       "<span style=\" text-decoration: underline; color:#0000c0;\">" + ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()) +
                                       "</span></a></p></body></html>");
}

/**************************************************/
/*           YONETIM_HAKKIMIZDA::~YONETIM_HAKKIMIZDA        */
/**************************************************/

YONETIM_HAKKIMIZDA::~YONETIM_HAKKIMIZDA()
{
    delete m_ui;
}

