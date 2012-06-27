#ifndef REPORT_SHOWER_H
#define REPORT_SHOWER_H

#include <QPushButton>
#include <QWebView>
#include <QDialog>
#include <QPrinter>

#include "base_kernel.h"

enum ENUM_EXPORT_TYPES {
    ENUM_KERNEL_HTML        = 1,
    ENUM_KERNEL_OO_WRITER   = 2,
    ENUM_KERNEL_TXT         = 3
};

class REPORT_SHOWER : public BASE_KERNEL {
    Q_OBJECT

public:
    REPORT_SHOWER(QString printer_document, int paper_type,double user_defined_width,double user_define_height, QWidget * parent = 0);
    qreal p_left_margin;
    qreal p_top_margin;
    qreal p_bottom_margin;
    qreal p_right_margin;
    QPrinter::Orientation p_orientation;
    ~REPORT_SHOWER();

private:
    QPushButton *       button_print_edit;
    QPushButton *       button_email;
    QPushButton *       button_ihrac;

    QWebView *          preview_view;
    QString             printer_document;

    int                 m_paper_type;
    double              m_user_defined_width;
    double              m_user_defined_height;

private slots:
    void        SLOT_PRINT_PREVIEW();
    void        SLOT_PAINT_REQUESTED (QPrinter *);
    void        SLOT_SEND_EMAIL();
    void        SLOT_EXPORT_POPUP();
    void        SLOT_EXPORT_TO_FILE ();

};

#endif // REPORT_SHOWER_H

