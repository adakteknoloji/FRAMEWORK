#ifndef YONETIM_007_ARAMA_H
#define YONETIM_007_ARAMA_H

#include <QtGui/QDialog>
#include "arama_kernel.h"


namespace Ui {
    class YONETIM_007_ARAMA;
}

class YONETIM_007_ARAMA : public ARAMA_KERNEL
{
    Q_OBJECT

public:
                           YONETIM_007_ARAMA                            (QWidget *parent = 0);
                           ~YONETIM_007_ARAMA                           () {}
private:
    Ui::YONETIM_007_ARAMA *   m_ui;

    void                   SETUP_FORM                                 ();
    int                    CHECK_VAR                                  (QObject * object);
    int                    CHECK_RUN                                  ();
    void                   SEARCH                                     ();
    int                    SINGLE_LINE_SELECTED                       (int current_row);
    void                   SELECTED_LINE                              (int selected_line_id);

};

#endif // YONETIM_007_ARAMA_H
