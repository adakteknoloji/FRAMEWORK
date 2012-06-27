#ifndef TASARIM_DESIGN_WIDGET_H
#define TASARIM_DESIGN_WIDGET_H

#include <QWidget>
#include <QStringList>
#include <QLabel>
#include "tasarim_defines.h"

class QDragEnterEvent;
class QDropEvent;
class TASARIM_DEGISKENI;

class TASARIM_DESIGN_WIDGET : public QWidget
{
    Q_OBJECT

public:
                        TASARIM_DESIGN_WIDGET                   (double oran = 1.00,QWidget * parent = 0);
    void                INIT_WIDGET                             ( QList< TASARIM_DEGISKENI *> p_variable, TASARIMCI_MODU p_modu );
    void                SET_WIDGET_SIZE                         (int width,int height);
    void                HIDE_SELECTED_VARIABLE                  ();
    void                SHOW_SELECTED_VARIABLE                  ();
    void                SET_SELECTED_FONT_SIZE                  (int size_pt);
    void                SET_SELECTED_STYLE_SHEET                (QString style_sheet);
    void                SET_TEXT_SELECTED_VARIABLE              (QString label_text);
    void                SET_SIZE_SELECTED_VARIABLE              (int width , int height );
    void                SET_OBJNAME_SELECTED_VARIABLE           (QString obj_name);
    void                SET_ALIGN_SELECTED_VARIABLE             (int align_enum,QLabel * label = NULL);
    QLabel *            MOVE_SELECTED_VARIABLE                  (double x,double y,bool block_pos_changed_signal = false);
    QLabel *            MOVE_VARIABLE                           (QLabel * label,double x,double y,bool block_pos_changed_signal = false);
    void                DELETE_SELECTED_VARIABLE                ();
    void                SELECT_VARIABLE                         (int index);
    void                SET_FONT                                (QLabel * label,int size_pt,bool oranla = true);

    void                ADD_NEW_VARIABLE                        (int x,int y,QLabel * label);
    QLabel *            CREATE_NEW_LABEL                        (const QString &text, QWidget * parent = NULL);

    QLabel *            GET_LABEL                               (int index);
    // Secili olan label i dondurur. Eger label secili degilse NULL doner.
    QLabel *            GET_SELECTED_LABEL                      ();

    bool                eventFilter                             (QObject * p_obj, QEvent * p_event );

    QLabel *            GET_LAST_MOVE_LABEL                     ();


    void                SELECT_VARIABLE                         (QLabel * label);
    void                UNSELECT_VARIABLE                       ();

    void                RESET_VARIABLE                          ();

protected:
    void                dragEnterEvent(QDragEnterEvent *event);
    void                dragMoveEvent(QDragMoveEvent *event);
    void                dropEvent(QDropEvent *event);
    void                mousePressEvent(QMouseEvent *event);

signals:
    void                VARIABLE_CREATED                        (QLabel * label);
    void                VARIABLE_POSITION_CHANGED               ( int, double x, double y);
    void                VARIABLE_SELECTED                       (QLabel * label);
    void                VARIABLE_UNSELECTED                     ();
    void                SIGNAL_KEY_DIRECTION_PRESS              (Qt::Key p_key );

public:
    void                SET_LABEL_FONTS_UNSELECTED              (QLabel * label);

private:
    QLabel              *m_last_move_label;

    double              m_oran;

    int                 m_width;
    int                 m_height;

    QList<QLabel *>     m_labels;

    QLabel *            m_last_selected_variable;

    QPoint              m_start_point;

    void                SET_LAST_SELECTED_VARIABLE              (QLabel * label);

    void                SET_LABEL_FONTS_SELECTED                (QLabel * label);

    void                ADD_LABEL_LIST                          (QLabel * label);
    void                REMOVE_LABEL_LIST                       (QLabel * label);

};

#endif
