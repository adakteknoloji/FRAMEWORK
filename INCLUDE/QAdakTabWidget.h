#ifndef QADAKTABWIDGET_H
#define QADAKTABWIDGET_H

#include <QTabWidget>

class QTabBar;
class QTabWidget;
class QString;

class QAdakTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QAdakTabWidget(QWidget * parent);


    int              ADD_TAB                    (QString name , QWidget * widget,int index = -1,QString icon_path = "");
    void             REMOVE_TAB                 (int index);

    void             SET_CURRENT_INDEX          (int index);

    int              GET_CURSOR_POS_TAB_INDEX   ();

    void             mouseMoveEvent             ( QMouseEvent * event );

public slots:
    void             CLOSE_TAB                  (int);

signals:
    void             SIGNAL_CLOSE_TAB           (int);
    void             SIGNAL_MENU_BUTTON_CLICKED ();
    void             SIGNAL_LAST_TAB_CLOSE      ( QString tab_name );

private:
    QWidget *        m_main_menu;
    int              cursor_pos_tab_index; // cursor hangi tab uzerinde

    void             HIDE_MAIN_MENU             ();

private slots:    
    void             SLOT_OPEN_MAIN_MENU        ();
    void             SLOT_TAB_BAR_CHANGED       (int);

};

#endif // QADAKTABWIDGET_H
