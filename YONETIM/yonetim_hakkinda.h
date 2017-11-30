#ifndef YONETIM_HAKKINDA
#define YONETIM_HAKKINDA

#include <QDialog>

class QString;
class QWidget;

namespace Ui {
    class YONETIM_HAKKIMIZDA;
}

class YONETIM_HAKKIMIZDA : public QDialog {
    Q_OBJECT
public:
    YONETIM_HAKKIMIZDA( QWidget *parent = 0);
    ~YONETIM_HAKKIMIZDA();

private:
    Ui::YONETIM_HAKKIMIZDA *m_ui;
};

void OPEN_YONETIM_HAKKIMIZDA ( QWidget * );

#endif // YONETIM_HAKKINDA
