#include <QEvent>
#include <QTabBar>
#include <QString>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

#include "QAdakTabWidget.h"
#include "adak_defines.h"
#include "adak_utils.h"

/******************************************************************************************
                        QAdakTabWidget::QAdakTabWidget
*******************************************************************************************/

QAdakTabWidget::QAdakTabWidget(QWidget *parent) : QTabWidget(parent)
{
    QWidget * widget                    = new QWidget;
    QHBoxLayout * h_layout              = new QHBoxLayout;

    tabBar()->setTabsClosable          (true);
    tabBar()->setSizePolicy            (QSizePolicy::Fixed,QSizePolicy::Expanding);
    connect(tabBar(),SIGNAL(currentChanged(int))   ,this,SLOT  (SLOT_TAB_BAR_CHANGED(int)));
    connect(tabBar(),SIGNAL(tabCloseRequested(int)),this,SLOT  (CLOSE_TAB(int)));

    QPushButton * main_menu             = new QPushButton("Ana Menü");
    main_menu->setIconSize              (QSize(16,16));
    main_menu->setIcon                  (QIcon(":/kernel_icons/kernel_home_ikonu.png"));
    main_menu->setSizePolicy            (QSizePolicy::Fixed,QSizePolicy::Expanding);
    connect(main_menu,SIGNAL(clicked()),this,SLOT(SLOT_OPEN_MAIN_MENU()));

    QPushButton * menu_button           = new QPushButton;
    menu_button->setIconSize            (QSize(16,16));
    menu_button->setShortcut            (QKeySequence("Alt+G"));
    menu_button->setIcon                (QIcon(":/kernel_icons/kernel_menu_ikonu.png"));
    menu_button->setSizePolicy          (QSizePolicy::Fixed,QSizePolicy::Expanding);
    connect(menu_button,SIGNAL(clicked()),this,SIGNAL(SIGNAL_MENU_BUTTON_CLICKED()));

    h_layout->setSpacing                (0);
    h_layout->setMargin                 (0);
    h_layout->addWidget                 (menu_button     );
    h_layout->addWidget                 (main_menu       );
    //h_layout->addSpacerItem             (new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed));

    widget->setLayout(h_layout);

    this->setMovable( true );

    setCornerWidget(widget,Qt::TopLeftCorner);

    cursor_pos_tab_index = 1;
    setMouseTracking(true);
    tabBar()->setMouseTracking(true);
}

/******************************************************************************************
                        QAdakTabWidget::ADD_TAB
*******************************************************************************************/

int QAdakTabWidget::ADD_TAB(QString name, QWidget *widget,int index, QString icon_path)
{
    Q_UNUSED(icon_path);

    HIDE_MAIN_MENU();

    if ( name EQ "Yeni Sekme" ) {
        m_main_menu = widget;
        index = addTab(widget,name);
    }
    else {
        index = addTab(widget,name);
    }

    setCurrentIndex       (index);

    return index;
}

/******************************************************************************************
                        QAdakTabWidget::REMOVE_TAB
*******************************************************************************************/

void QAdakTabWidget::REMOVE_TAB(int index)
{
    removeTab(index);

    if ( count() EQ 0 ) {
        SLOT_OPEN_MAIN_MENU();
    }
    // SEKME KAPATILDIKTAN SONRA BIR ONCEKI SEKMEYE ODAKLANIR.
    setCurrentIndex( count() );
    setFocus();
}

/******************************************************************************************
                        QAdakTabWidget::SET_CURRENT_INDEX
*******************************************************************************************/

void QAdakTabWidget::SET_CURRENT_INDEX(int index)
{
    setCurrentIndex(index);
}


/******************************************************************************************
                        QAdakTabWidget::SLOT_TAB_BAR_CHANGED
*******************************************************************************************/

void QAdakTabWidget::HIDE_MAIN_MENU()
{
    //FIXME sondan baslamak daha mantikli
    for ( int i = 0 ; i < count() ; i++ ) {
        if ( tabText(i) EQ "Yeni Sekme" )  {
            removeTab(i);
            break;
        }
    }
}

/******************************************************************************************
                        QAdakTabWidget::CLOSE_TAB
*******************************************************************************************/

void QAdakTabWidget::CLOSE_TAB(int index)
{
    if ( tabText(index) EQ "Yeni Sekme" )  {
        // BIRDEN FAZLA TAB ACIKSA SORMASINA GEREK YOK
        if ( count() EQ 1 ) {
            ADAK_MSG_ENUM answer = ADAK_YES_NO( "Program kapatılacaktır. Devam etmek istiyormusunuz?", NULL, this );

            if ( answer EQ ADAK_NO ) {
                return;
            }
        }

        removeTab(index);
        if ( count() EQ 0 ){
            emit SIGNAL_LAST_TAB_CLOSE( "Yeni Sekme" );
        }
    }
    else {
        emit SIGNAL_CLOSE_TAB(index);
    }
}

/******************************************************************************************
                        QAdakTabWidget::SLOT_OPEN_MAIN_MENU
*******************************************************************************************/

void QAdakTabWidget::SLOT_OPEN_MAIN_MENU()
{
    ADD_TAB("Yeni Sekme",m_main_menu,0);
}

/******************************************************************************************
                        QAdakTabWidget::SLOT_TAB_BAR_CHANGED
*******************************************************************************************/

void QAdakTabWidget::SLOT_TAB_BAR_CHANGED(int index)
{
    if ( tabText(index) EQ "Yeni Sekme" ) {
        return;
    }

    HIDE_MAIN_MENU();
}

/******************************************************************************************
                        QAdakTabWidget::mouseMoveEvent
*******************************************************************************************/

void QAdakTabWidget::mouseMoveEvent ( QMouseEvent * event )
{
    QTabBar *wid = tabBar();
    if ( wid && wid->tabAt( event->pos() ) NE -1) {
      //qDebug() << "mouse hovering in tab            :" << wid->tabAt( event->pos() );
      cursor_pos_tab_index = wid->tabAt( event->pos());
      if ( wid->tabAt( event->pos() ) EQ 0 ) {
        cursor_pos_tab_index = 1;
      }
    }
    return QTabWidget::mouseMoveEvent(event);
}

/******************************************************************************************
                        QAdakTabWidget::GET_CURSOR_POS_TAB_INDEX
*******************************************************************************************/

int QAdakTabWidget::GET_CURSOR_POS_TAB_INDEX   ()
{
    return cursor_pos_tab_index;
}
