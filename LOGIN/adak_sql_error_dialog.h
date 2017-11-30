#ifndef ADAK_SQL_ERROR_DIALOG_H
#define ADAK_SQL_ERROR_DIALOG_H

#include <QDialog>

namespace Ui {
    class ADAK_SQL_ERROR_DIALOG;
}

class ADAK_SQL_ERROR_DIALOG : public QDialog
{
    Q_OBJECT

public:

    ADAK_SQL_ERROR_DIALOG              ( QWidget * parent = 0 );
    ~ADAK_SQL_ERROR_DIALOG             ();

    Ui::ADAK_SQL_ERROR_DIALOG * m_ui;


private slots:

    void SLOT_HATA_MAILI_AT             ();
};

#endif // ADAK_SQL_ERROR_DIALOG_H
