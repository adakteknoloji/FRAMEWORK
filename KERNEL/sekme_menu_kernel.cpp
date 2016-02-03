#include <QApplication>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QEvent>
#include <QMdiArea>
#include <QBrush>
#include <QMdiSubWindow>
#include <QFontMetrics>
#include <QTabBar>
#include <QMenuBar>
#include <QFrame>
#include <QDialog>
#include <QScrollArea>
#include <QLineEdit>
#include <QVariant>
#include <QSettings>
#include <QToolTip>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QToolButton>
#include <algorithm>

#include "QAdakTabWidget.h"
#include "adak_defines.h"
#include "yonetim.h"
#include "kernel_utils.h"
#include "sekme_menu_kernel.h"

QList<QWidget *> G_SEKME_MENU_KERNEL;

using namespace std;

/******************************************************************************************
                        SEKME_MENU_KERNEL::SEKME_MENU_KERNEL
*******************************************************************************************/

SEKME_MENU_KERNEL::SEKME_MENU_KERNEL( int program_id , QWidget *parent ) : QMainWindow(parent) , m_program_id(program_id)
{
    CLEAR_MENU_PROPERTIES_STRUCT    (&m_menu_properties);
    //LOAD_MENU_PROPERTIES            ();

    m_close_events          = false;
    m_num_of_columns        = 0;
    m_bottom_label          = NULL;

    m_sub_window_width_inc  = 10;
    m_sub_window_height_inc = 50;

    ENABLE_SCROLLABLE_MENUS();

    //3 column lik sabit bir yapi kullanildigindan dinamik olarak atanmasi gereksiz.
    m_column_lenghts << 0 << 0 << 0;

    QHBoxLayout * layout = new QHBoxLayout;

    layout->setContentsMargins         ( 0,0,0,0                                   );
    layout->setSpacing                 ( 0                                         );
    layout->setSizeConstraint          ( QLayout::SetNoConstraint                  );

    setMinimumWidth  (780);
    setMinimumHeight (580);

    m_main_widget        = new QWidget ( this                                      );
    m_main_widget->setLayout           ( layout                                    );

    setCentralWidget                   ( m_main_widget                             );
    layout->addWidget                  ( CREATE_TAB_WIDGET()                       );

    //FIXME Tam ekran sorununu arastir
    QDesktopWidget * desktop_widget = qApp->desktop();
    resize(desktop_widget->availableGeometry().width(),desktop_widget->availableGeometry().height() - 10 );

    m_background         = new QLabel;

    installEventFilter   (this);

    G_SEKME_MENU_KERNEL  << this;

    SET_MENU_BOTTOM_TEXT (tr("This program created by <b>Adak Technology</b>, anyone can download from <b>http://www.adak.com.tr</b>.<br>  "
              "There is no restriction in the program."));
    //Bu program <b>ADAK Teknoloji</b> tarafından üretilmiştir , isteyen herkes <b>http://www.adak.com.tr</b> ' dan indirebilir.<br>
    //Programda hiç bir kısıt yoktur.</b>.
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::~SEKME_MENU_KERNEL
*******************************************************************************************/

SEKME_MENU_KERNEL::~SEKME_MENU_KERNEL()
{
    m_menu_items.clear();

    G_SEKME_MENU_KERNEL.removeOne(this);
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::CREATE_MENU
*******************************************************************************************/

void SEKME_MENU_KERNEL::START_MENU_KERNEL()
{
    FILL_MENU_STRUCT                   ();

    HIDE_ALL                           ();

    PREPARE_PROGRAM_FOR_USER           ();

    SHOW_MENU                          ();

    m_main_widget->setSizePolicy       ( QSizePolicy::Ignored,QSizePolicy::Ignored );
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SHOW_MENU
*******************************************************************************************/

void SEKME_MENU_KERNEL::SHOW_MENU()
{
    SET_MAIN_MENU_DEFAULTS             ();

    SET_WINDOW_DEFAULTS                ();

    CREATE_MENU                        ();

    ADD_MENU_BAR                       ( m_tab_widget );

    REPAINT                            ();
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_MENU_STRUCT
*******************************************************************************************/

void SEKME_MENU_KERNEL::FILL_MENU_STRUCT(int parent_index, int parent_enum_id)
{
    MENU_STRUCT * menu_struct = GET_MENU_STRUCT(parent_enum_id);

    if ( menu_struct EQ NULL ) {
        return;
    }

    for ( int i = 0 ;  ; i++ ) {
        if ( menu_struct[i].enum_id EQ MENU_END ) {
            return;
        }

        if ( menu_struct[i].enum_id EQ MENU_SEPERATOR ) {
            continue;
        }

        KERNEL_MENU_ITEM * menu_item = new KERNEL_MENU_ITEM;

        SET_STRUCT_DEFAULTS(menu_item);

        menu_item->enum_id                    = menu_struct[i].enum_id;
        menu_item->text_to_display            = menu_struct[i].text_to_display;
        menu_item->icon_path                  = menu_struct[i].icon_path;
        menu_item->shortcut                   = menu_struct[i].shortcut;
        menu_item->is_header                  = menu_struct[i].is_header;
        menu_item->menu_column                = menu_struct[i].menu_column;
        menu_item->font_size_increment        = menu_struct[i].font_size_increment;
        menu_item->parent_enum_id             = parent_enum_id;

        if ( menu_struct[i+1].enum_id EQ MENU_SEPERATOR ) {
            menu_item->add_seperator_after        = true;
        }

        m_menu_codes << menu_struct[i].menu_code;

        if ( parent_enum_id EQ MENU_ROOT ) {
            menu_item->menu_column                = 0;
            menu_item->font_dec_multiplier        = 0;
        }
        else {
            m_menu_items.at(parent_index)->num_of_childs++;
        }

        if ( m_column_lenghts.size() > menu_item->menu_column ) {
            if ( menu_item->text_to_display.size() > m_column_lenghts.at(menu_item->menu_column) ) {
                m_column_lenghts[menu_item->menu_column] = menu_item->text_to_display.size();
            }
        }
        else {
            qDebug("The maximum number of column must be 2 at tab kernel menu.Given the column values may be 1 or 2.");//Sekme menu kernel da max column 2 olmalidir.Verilen column degerleri 1 yada 2 olabilir
            abort();
        }

        if ( menu_item->menu_column + 1 > m_num_of_columns ) {
            m_num_of_columns = menu_item->menu_column + 1;
        }

        m_menu_items.push_back(menu_item);

        FILL_MENU_STRUCT( (m_menu_items.size() - 1) , menu_item->enum_id );
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::ADD_MENU_BAR
*******************************************************************************************/

void SEKME_MENU_KERNEL::ADD_MENU_BAR(QAdakTabWidget *tab_widget)
{
    m_menu             = new QMenu(tr("Menu"));

    ADD_MENU_BAR_CHILDS(m_menu,MENU_ROOT);

    m_menu->hide();

    connect ( tab_widget,SIGNAL(SIGNAL_MENU_BUTTON_CLICKED()) , this , SLOT(SLOT_MENU_CLICKED()) );
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::ADD_MENU_BAR_CHILDS
*******************************************************************************************/

void SEKME_MENU_KERNEL::ADD_MENU_BAR_CHILDS(QMenu * p_menu, int parent_id)
{
    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {
        if ( m_menu_items.at(i)->is_visible EQ false ) {
            continue;
        }

        if ( m_menu_items.at(i)->parent_enum_id NE parent_id ) {
            continue;
        }

        if ( m_menu_items.at(i)->num_of_childs NE 0 ) {
            QMenu * menu = p_menu->addMenu  (m_menu_items.at(i)->text_to_display);
            menu->setObjectName             (QVariant(i).toString());

            if ( m_menu_items.at(i)->add_seperator_after EQ true ) {
                p_menu->addSeparator();
            }

            ADD_MENU_BAR_CHILDS(menu,m_menu_items.at(i)->enum_id);
        }
        else {
            QString text =  m_menu_items.at(i)->text_to_display;

            if ( m_menu_codes.at(i).isEmpty() EQ false ) {
                text.append(QString("\t(%1)").arg(m_menu_codes.at(i)));
            }

            QAction * action = p_menu->addAction(QIcon(m_menu_items.at(i)->icon_path),text);
            action->setObjectName               (QVariant(i).toString());

            if ( m_menu_items.at(i)->add_seperator_after EQ true ) {
                p_menu->addSeparator();
            }

            connect(action,SIGNAL(triggered()),this,SLOT(SLOT_ITEM_CLICKED()));
        }
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::OPEN_EKRANI_AC_DIALOG
*******************************************************************************************/

void SEKME_MENU_KERNEL::OPEN_EKRANI_AC_DIALOG()
{
    QHBoxLayout * h_layout = new QHBoxLayout;
    QDialog * dialog       = new QDialog;

    dialog->setLayout      ( h_layout               );
    dialog->setWindowTitle ( tr("Open Screen") );

    QLabel * label         = new QLabel(tr("Menu code") );
    QLineEdit * line_edit  = new QLineEdit;
    QPushButton * button   = new QPushButton(tr("Bring"));

    QAction * action = new QAction(this);

    action->setShortcuts(QList<QKeySequence>() << QKeySequence("Enter") << QKeySequence("Return") );

    h_layout->addWidget(label);
    h_layout->addWidget(line_edit);
    h_layout->addWidget(button);

    connect(button , SIGNAL(clicked())   , action, SLOT(trigger()));
    connect(action , SIGNAL(triggered()) , dialog, SLOT(accept()));

    dialog->exec();

    if ( line_edit->text().isEmpty() EQ false ) {
        int index = m_menu_codes.indexOf(line_edit->text().toUpper());

        if ( index EQ -1 ) {
            MSG_ERROR(QString(tr("'%1' could not find a menu with a code number.")).arg(line_edit->text()),NULL);//'%1' kod numarasına sahip bir menü bulunamadı.
        }
        else {
            SLOT_ITEM_CLICKED(QVariant(index).toString());
        }
    }

    dialog->setAttribute   ( Qt::WA_DeleteOnClose   );
    dialog->close();
}


/******************************************************************************************
                        SEKME_MENU_KERNEL::CREATE_TAB_WIDGET
*******************************************************************************************/

QWidget * SEKME_MENU_KERNEL::CREATE_TAB_WIDGET()
{
    m_tab_widget      = new QAdakTabWidget (this);

    m_tab_widget->setObjectName             ("main_tab_widget");
    m_tab_widget->installEventFilter        (this);
    m_tab_widget->clearFocus                ();

    QTabBar * tab_bar = m_tab_widget->findChild<QTabBar *>();

    if ( tab_bar NE NULL ) {
        tab_bar->setStyleSheet("QTabBar::tab:selected {"
                                    "margin-left: 0px;"
                                    "margin-right: 0px;"
                                    "}"
                                    "QTabBar::tab:first {"
                                    "margin-left: 3px;"
                                    "}"
                                    "QTabBar::tab:only-one {"
                                    "margin-left: 3px;"
                                    "}"
                                    "QTabBar::tab:first:selected {"
                                    "margin-left: 3px;"
                                    "}"
                                    "QTabBar::tab:first:unselected {"
                                    "margin-left: 3px;"
                                    "}"
                                    "QTabBar::tab:unselected {"
                                    "margin-left: 0px;"
                                    "}"
                                    "QTabBar::tab:last:selected {"
                                    "margin-right: 0;"
                                    "}"
                                    "QTabWidget::pane { "
                                    "border-top: 2px solid #C2C7CB;"
                                    "}"
                                    "QTabBar::tab {"
                                    "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
                                    "border: 2px solid #C4C4C3;"
                                    "border-bottom-color: #C2C7CB; "
                                    "border-top-left-radius: 4px;"
                                    "border-top-right-radius: 4px;"
                                    "min-width: 8ex;"
                                    "padding: 2px;"
                                    "}"
                                    "QTabBar::tab:selected, QTabBar::tab:hover {"
                                    "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fafafa, stop: 0.4 #f4f4f4, stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
                                    "}"
                                    "QTabBar::tab:selected {"
                                    "border-color: #9B9B9B;"
                                    "border-bottom-color: #C2C7CB;"
                                    "}"
                                    );
    }

    m_tab_widget->setStyleSheet("QTabWidget::tab-bar { alignment: left; }");

    connect ( m_tab_widget, SIGNAL( SIGNAL_CLOSE_TAB     ( int     )), this, SLOT( SLOT_CLOSE_WIDGET   ( int     )));
    connect ( m_tab_widget, SIGNAL( SIGNAL_LAST_TAB_CLOSE( QString )), this, SLOT( SLOT_CLOSE_LAST_TAB ( QString )));

    return m_tab_widget;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::ENABLE_SCROLLABLE_MENUS
*******************************************************************************************/

void SEKME_MENU_KERNEL::ENABLE_SCROLLABLE_MENUS()
{
    m_container_type = MENU_SCHEMA_SCROLLABLE;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::DISABLE_SCROLLABLE_MENUS
*******************************************************************************************/

void SEKME_MENU_KERNEL::DISABLE_SCROLLABLE_MENUS()
{
    m_container_type = MENU_SCHEMA_NO_SCROLL;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_STRUCT_DEFAULTS
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_STRUCT_DEFAULTS(KERNEL_MENU_ITEM * menu_item)
{
    menu_item->enum_id                      = -1;
    menu_item->text_to_display              = "";
    menu_item->parent_enum_id               = -1;
    menu_item->icon_path                    = "";
    menu_item->shortcut                     = "";
    menu_item->menu_column                  = -1;
    menu_item->add_seperator_after          = false;
    menu_item->is_visible                   = false;
    menu_item->is_enable                    = true;
    menu_item->is_header                    = false;
    menu_item->num_of_childs                = 0;
    menu_item->font_size_increment          = 0;
    menu_item->font_dec_multiplier          = 1;    
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::GET_CONTAINER_WIDGET
*******************************************************************************************/

QWidget * SEKME_MENU_KERNEL::GET_CONTAINER_WIDGET(QWidget * widget)
{
    if ( m_container_type EQ MENU_SCHEMA_SCROLLABLE ) {
        QScrollArea * scroll_area = new QScrollArea;
        scroll_area->setWidget                    ( widget                                                   );        
        scroll_area->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff                                   );
        scroll_area->setFrameStyle                ( QFrame::NoFrame                                          );

        return scroll_area;
    }

    return widget;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::REFRESH_MENU_KERNEL
*******************************************************************************************/

void SEKME_MENU_KERNEL::REFRESH_MENU_KERNEL()
{
    if ( CLOSE_PROGRAM(true) EQ false ) {
        return;
    }

    hide                          ();
    HIDE_ALL                      ();

    DESTROY_ALL                   ();

    PREPARE_PROGRAM_FOR_USER      ();

    SHOW_MENU                     ();

    show                          ();
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SHOW_MENU_ITEM
*******************************************************************************************/

void SEKME_MENU_KERNEL::SHOW_MENU_ITEM(int menu_item_enum)
{
    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {
        if ( m_menu_items.at(i)->enum_id EQ menu_item_enum ) {
           m_menu_items.at(i)->is_visible = true;
            return;
        }
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::HIDE_MENU_ITEM
*******************************************************************************************/

void SEKME_MENU_KERNEL::HIDE_MENU_ITEM(int menu_item_enum)
{
    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {
        if ( m_menu_items.at(i)->enum_id EQ menu_item_enum ) {
            m_menu_items.at(i)->is_visible = false;
        }
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::HIDE_ALL
*******************************************************************************************/

void SEKME_MENU_KERNEL::HIDE_ALL()
{
    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {
        m_menu_items.at(i)->is_visible = false;
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::DESTROY_ALL
*******************************************************************************************/

void SEKME_MENU_KERNEL::DESTROY_ALL()
{
    m_tab_widget->clear();

    //Anasayfayi yok et
    m_container_widgets[0]->close();
    m_container_widgets[0] = NULL;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::MENU_INDEX_OF_WIDGET
*******************************************************************************************/

int SEKME_MENU_KERNEL::MENU_INDEX_OF_WIDGET( QWidget * widget )
{
    // BIRDEN FAZLA CIKTI TAB ACILA  BILMELI
    if ( widget->windowTitle().trimmed() NE QObject::tr("OUTPUT PREVIEW") ) {//ÇIKTI ÖNİZLEME
        for ( int i = 0 ; i < m_tab_widgets.size() ; i++ ) {
            if ( widget->windowTitle() EQ m_tab_widgets.at(i)->windowTitle() ) {
                return i;
            }
        }
    }

    return -1;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::MENU_SET_OPENED_WIDGET
*******************************************************************************************/

void SEKME_MENU_KERNEL::MENU_SET_OPENED_WIDGET(int index)
{
    m_tab_widget->SET_CURRENT_INDEX(index);
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::MENU_DELETE_WIDGET
*******************************************************************************************/

void SEKME_MENU_KERNEL::MENU_DELETE_WIDGET(QWidget *widget)
{
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->close();
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::ADD_PARENT_TAB
*******************************************************************************************/

void SEKME_MENU_KERNEL::ADD_PARENT_TAB(QWidget * widget,  SCREEN_SIZE_ENUM p_screen_size )
{
    int index = MENU_INDEX_OF_WIDGET(widget);

    if ( index >= 0 ) {
        MENU_SET_OPENED_WIDGET ( index  );
        MENU_DELETE_WIDGET     ( widget );
        return;
    }

    if ( widget->layout() NE NULL ) {
        widget->layout()->setContentsMargins( 2,2,2,1 );
        widget->layout()->setSpacing        ( 2       );
        widget->layout()->setSizeConstraint (QLayout::SetNoConstraint);
    }

    QMdiArea * mdi_area = new QMdiArea;

    mdi_area->setOption(QMdiArea::DontMaximizeSubWindowOnActivation,true);

    QBrush brush                    (QColor(0, 0, 0, 0));
    brush.setStyle                  (Qt::SolidPattern);
    mdi_area->setBackground         (brush);

    mdi_area->setAttribute          (Qt::WA_DeleteOnClose);
    mdi_area->installEventFilter    (this);

    widget->setAttribute            (Qt::WA_DeleteOnClose);

    int tab_index = ADD_TAB( mdi_area, QString("%1\t").arg( widget->windowTitle() ) );

    QApplication::restoreOverrideCursor();

    QMdiSubWindow * sub_window;
    if ( p_screen_size EQ NOT_FULL_SCREEN) {
        sub_window = mdi_area->addSubWindow(widget);

        widget->parentWidget()->resize(widget->width() + m_sub_window_width_inc , widget->height() + m_sub_window_height_inc);
        widget->parentWidget()->setMinimumSize (widget->minimumWidth() + m_sub_window_width_inc ,widget->minimumHeight() + m_sub_window_height_inc);

        //16777215 Qt icin max boyut
        if ( widget->maximumHeight() + m_sub_window_height_inc < 16777215 ) {
            widget->parentWidget()->setMaximumHeight(widget->maximumHeight() + m_sub_window_height_inc);
        }
        if ( widget->maximumWidth() + m_sub_window_height_inc < 16777215 ) {
            widget->parentWidget()->setMaximumWidth(widget->maximumWidth() + m_sub_window_width_inc);
        }
        widget->show();

        int x_pos = (m_tab_widget->width()  - 30) /2-widget->parentWidget()->width() /2;
        int y_pos = (m_tab_widget->height() - 30) /2-widget->parentWidget()->height()/2;

        widget->parentWidget()->move(x_pos,y_pos);
        sub_window->setFocus();
    }
    else {
        sub_window = mdi_area->addSubWindow(widget, Qt::Widget | Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
        sub_window->showMaximized();\
        sub_window->setFocus();
    }

    m_mdi_areas   << mdi_area;
    m_tab_widgets << widget;


    //SET CURRENT INDEX(tab_index) <-visible yapar widget i
    m_tab_widget->SET_CURRENT_INDEX( tab_index );

    connect( widget  , SIGNAL( destroyed() ), this, SLOT( SLOT_CLOSE_CHILD_WIDGET() ));
    connect( mdi_area, SIGNAL( destroyed() ), this, SLOT( SLOT_CLOSE_CHILD_WIDGET() ));
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::ADD_CHILD_DIALOG
*******************************************************************************************/
void SEKME_MENU_KERNEL::ADD_CHILD_DIALOG(QWidget * widget, QWidget * parent,  SCREEN_SIZE_ENUM p_screen_size )
{
    if ( parent NE NULL ) {
        parent->setEnabled( false );
    }

    int index = -1;

    QWidget * widget_parent = parent;

    for ( ; ; ) {
        if ( widget_parent EQ NULL ) {
            qDebug("Child's parent could not be determined.");//Child parenti tespit edilemedi!!!!
            return;
        }

        index = m_tab_widgets.indexOf( widget_parent );

        if ( index EQ -1 ) {
            if ( widget_parent->metaObject()->className() EQ QMdiSubWindow::staticMetaObject.className() ) {
                QMdiSubWindow * mdi_sub_window = static_cast<QMdiSubWindow *>(widget_parent);
                index = m_mdi_areas.indexOf( mdi_sub_window->mdiArea() );
            }
        }

        if ( index NE -1 ) {
            break;
        }

        widget_parent = widget_parent->parentWidget();
    }

    widget->setAttribute(Qt::WA_DeleteOnClose);

    QApplication::restoreOverrideCursor();

    m_mdi_areas.at(index)->addSubWindow( widget, ( windowFlags() & ~Qt::WindowMinimizeButtonHint )| Qt::WindowStaysOnTopHint );

    if ( p_screen_size EQ FULL_SCREEN ) {
        widget->setWindowState                (Qt::WindowMaximized);

        if (widget->layout() NE NULL) {
            widget->layout()->setSizeConstraint(QLayout::SetNoConstraint);
        }

        widget->parentWidget()->showMaximized();
    }
    else {
        widget->parentWidget()->resize(widget->width() + m_sub_window_width_inc , widget->height() + m_sub_window_height_inc);
        widget->parentWidget()->setMinimumSize (widget->minimumWidth() + m_sub_window_width_inc ,widget->minimumHeight() + m_sub_window_height_inc);

        //16777215 Qt icin max boyut
        if ( widget->maximumHeight() + m_sub_window_height_inc < 16777215 ) {
            widget->parentWidget()->setMaximumHeight(widget->maximumHeight() + m_sub_window_height_inc);
        }
        if ( widget->maximumWidth() + m_sub_window_height_inc < 16777215 ) {
            widget->parentWidget()->setMaximumWidth (widget->maximumWidth() + m_sub_window_width_inc);
        }
        widget->parentWidget()->show();

        int x_pos = (m_tab_widget->width()  - 30) /2-widget->parentWidget()->width() /2;
        int y_pos = (m_tab_widget->height() - 30) /2-widget->parentWidget()->height()/2;

        widget->parentWidget()->move(x_pos,y_pos);
    }

    connect( widget, SIGNAL(destroyed()),this,SLOT(SLOT_CLOSE_CHILD_WIDGET()));

    // Qt 'nin acilan child pencereye odaklanmasi gerekir
    widget->setFocus();

    static_cast<QDialog *>(widget)->exec();

    if ( parent NE NULL ) {
        parent->setEnabled( true );
        // PARENT FOCUSLAMAM GEREKLI
        static_cast<QDialog *>( parent )->setFocus();
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::ADD_TAB
*******************************************************************************************/

int SEKME_MENU_KERNEL::ADD_TAB(QWidget *tab_widget, QString name)
{
    int index = m_tab_widget->ADD_TAB( name, tab_widget );

    //FIXME bu tranlasition sorunu ama simdilik tr yok.
    //Bu yuzden daha ideal bir sekilde cozulebilir.Programi trye cevirmek gibi
    QList<QWidget *> tool_tips = m_tab_widget->findChildren<QWidget *>("");

    for ( int i = 0 ; i < tool_tips.size() ; i++ ) {
        if ( tool_tips.at(i)->toolTip().contains(tr("Close Tab"),Qt::CaseInsensitive) EQ true ) {
            tool_tips.at(i)->setToolTip("Sekmeyi Kapat");
        }
    }

    return index;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::CREATE_MENU_COLUMN
*******************************************************************************************/

QWidget * SEKME_MENU_KERNEL::CREATE_MENU_COLUMN(int column_index, KERNEL_MENU_ITEM *menu_items, bool recursive)
{
    QList<int> enums;

    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {
        enums << menu_items[i].enum_id;
    }

    return CREATE_MENU_COLUMN(column_index,&enums,recursive);
}


/******************************************************************************************
                        SEKME_MENU_KERNEL::CREATE_MENU
*******************************************************************************************/

void SEKME_MENU_KERNEL::CREATE_MENU(int parent_enum_id, bool recursive)
{
    //Columnlar icin enumlar.Su anki yapinin dinamik olmasi istenmedigi icin bole birakildi.
    //Amac dinamik olan column bazli yapiyi bozmamak.Burda commentli olan satirlar
    //dinamik yapiya ait.Su anki dinamik olmayan yapi kalici olacagi netlesene kadar silinmemeli.
    QList<int> f_enums;
    QList<int> s_enums;
    QList<int> p_enums;

    //Bu deger parent itemin columni +1 dir
    int menu_column_index = -1;

    //if (parent_enum_id EQ MENU_ROOT) {
        menu_column_index = 1;
    //}

    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {
        if ( m_menu_items.at(i)->parent_enum_id EQ parent_enum_id ) {
            if ( m_menu_items.at(i)->menu_column EQ 1 ) {
                f_enums << m_menu_items.at(i)->enum_id;
            }
            else if ( m_menu_items.at(i)->menu_column EQ 2 ) {
                s_enums << m_menu_items.at(i)->enum_id;
            }
            else if ( m_menu_items.at(i)->menu_column EQ 0 ) {
                p_enums << m_menu_items.at(i)->enum_id;
            }
        }
        //else if ( m_menu_items.at(i)->enum_id EQ parent_enum_id ) {
        //    menu_column_index = m_menu_items.at(i)->menu_column + 1;
        //}
    }

    if ( menu_column_index NE -1 ) {
        //Mevcut index baslayarak tum treeyi temizle
        for ( int i = menu_column_index ; i < m_container_widgets.size() ; i++ ) {
            if ( m_container_widgets.at(i) NE NULL ) {
                m_container_widgets.at(i)->close();
                m_container_widgets[i]   = NULL;
            }
        }
    }

    //Kullanicinin widget degisikliklerinin farkina varmasini istemiyoruz.
    //Hersey hazirlanana kadar hide la

    if ( p_enums.size() NE 0) {
        QWidget * f_widget = CREATE_MENU_COLUMN(0,&p_enums,recursive);
        f_widget->hide();
        f_widget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
        m_container_widgets[0] = f_widget;
        m_containers.at(0)->addWidget(f_widget);
        f_widget->show();
    }
    if ( f_enums.size() NE 0) {
        QWidget * f_widget = CREATE_MENU_COLUMN(1,&f_enums,recursive);
        f_widget->hide();
        f_widget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
        m_container_widgets[menu_column_index] = f_widget;
        m_containers.at(menu_column_index)->addWidget(f_widget);
        f_widget->show();
        menu_column_index++;
    }
    if ( s_enums.size() NE 0 ) {
        QWidget * s_widget = CREATE_MENU_COLUMN(2,&s_enums,recursive);
        s_widget->hide();
        s_widget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
        m_container_widgets[menu_column_index] = s_widget;
        m_containers.at(menu_column_index)->addWidget(s_widget);
        s_widget->show();
    }
}


/******************************************************************************************
                        SEKME_MENU_KERNEL::CREATE_MENU_COLUMN
*******************************************************************************************/

QWidget * SEKME_MENU_KERNEL::CREATE_MENU_COLUMN(int column_index, QList<int> *enum_ids, bool )
{
    if ( enum_ids EQ NULL ) {
        return NULL;
    }

    QWidget     * widget          = new QWidget;
    QVBoxLayout * vertical_layout = new QVBoxLayout;

    widget->setAttribute    (Qt::WA_DeleteOnClose);
    widget->setLayout       (vertical_layout);

    for ( unsigned int i = 0 ; i < m_menu_items.size() ; i++ ) {

        if ( enum_ids->contains( m_menu_items.at(i)->enum_id ) EQ false ) {
            continue;
        }

        if ( m_menu_items.at(i)->is_visible EQ false ) {
            continue;
        }

        QLabel      * label  = new QLabel("");
        QPushButton * button = new QPushButton();

        int indent = 0;

        for ( int space_index = 0 ; space_index < m_menu_items.at(i)->text_to_display.size(); space_index++ ) {
            QChar chr =  m_menu_items.at(i)->text_to_display.at(space_index).toUpper();
            if ( chr.isSpace() EQ false AND chr NE QChar('\t').toUpper() ) {
                break;
            }
            if ( chr.isSpace() EQ true ) {
                indent += 5;
            }
            else if (chr NE QChar('\t').toUpper()) {
                indent += 20;
            }
        }

        if ( m_menu_items.at(i)->is_header EQ true ) {
            label->setText( m_menu_items.at(i)->text_to_display );
        }
        else {
            button->installEventFilter(this);

            button->setText      ( m_menu_items.at(i)->text_to_display );
            // MENU ITEM ENUM ID OBJECT NAME SETLENDI GEREKTIGINDE KULLANILACAK
            button->setObjectName( QVariant( m_menu_items.at(i)->enum_id ).toString() );
        }

        QString style_sheet( "color: rgb(0, 0, 0);                  "
                             "background-color: rgb(216, 240, 255); "
                             "max-height: 22px; "
                             "text-align: left; ");

        button->setStyleSheet( style_sheet );

        label->setStyleSheet ( "max-height:13px;" );

        ADAK_SET_FONT_SIZE( label, 1.00 ); //x1

        button->setMinimumHeight(GET_LABEL_HEIGHT(column_index));
        button->setMinimumWidth (GET_LABEL_WIDTH(column_index ));
        button->setObjectName   (QVariant(i).toString());
        button->setSizePolicy   (QSizePolicy::Fixed , QSizePolicy::Fixed);

        connect( button, SIGNAL( clicked() ), this, SLOT ( SLOT_MENU_TOOL_BUTTON_CLICK()));

        if ( m_menu_items.at(i)->num_of_childs > 0 ) {
            QHBoxLayout * layout = new QHBoxLayout;

            layout->addWidget(button);

            QLabel * arrow = new QLabel("");

            arrow->setScaledContents(true);
            arrow->setPixmap(QPixmap(":/kernel_icons/kernel_alt_menu_ikonu.png"));
            //arrow->setMaximumSize(8,16);

            layout->addWidget (arrow);

            vertical_layout->addLayout(layout);
        }
        else {
            // BASLIK ISE LABEL
            if ( m_menu_items.at(i)->is_header EQ true ) {
                label->setText( label->text().replace( m_menu_properties.parent_label_color.name(), m_menu_properties.child_label_color.name()));
                vertical_layout->addWidget( label );
            }
            else {
                button->setText( button->text().replace(m_menu_properties.parent_label_color.name(), m_menu_properties.child_label_color.name() ));
                vertical_layout->addWidget( button );
            }
        }

        if ( m_menu_items.at(i)->add_seperator_after EQ true ) {
            QLabel * seperator = new QLabel("  ");
            seperator->setMaximumHeight(GET_LABEL_HEIGHT(column_index)/ 8);
            seperator->setMinimumWidth (GET_LABEL_WIDTH (column_index));
            seperator->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
            vertical_layout->addWidget(seperator);
        }
    }

    vertical_layout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Expanding));

    QWidget * container             = GET_CONTAINER_WIDGET(widget);

    //+60 menu kucultuldugunde ikonlar kaybolmasin diye

    container->setMinimumWidth      ( GET_LABEL_WIDTH(column_index) + 60 );

    return container;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_MENU_BOTTOM_TEXT
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_MENU_BOTTOM_TEXT(QString text)
{
    m_bottom_text = text;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_LABEL_DEFAULT_PROPERTIES
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_LABEL_DEFAULT_PROPERTIES( QPushButton  * button )
{
     button->styleSheet().clear();

     QString style_sheet( "color: rgb(0, 0, 0);                  "
                          "background-color: rgb(216, 240, 255); "
                          "max-height: 22px; "
                          "text-align: left; ");

     button->setStyleSheet( style_sheet );
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_LABEL_SELECTED_PROPERTIES
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_LABEL_SELECTED_PROPERTIES( QPushButton * button,  int menu_column )
{

    QString style_sheet;
    // ilk menu rengi
    if ( menu_column EQ 0 ) {
        style_sheet = "color: rgb(0, 0, 0);                 "
                      "background-color: rgb(112, 177, 255); "
                      "max-height: 22px;                    "
                      "text-align: left;                    ";
    }
    else {
        style_sheet = "color: rgb(0, 0, 0);                 "
                      "background-color: rgb(108, 199, 255); "
                      "max-height: 22px;                    "
                      "text-align: left;                    ";

    }

    button->setStyleSheet( style_sheet );

}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_LABEL_MOUSE_OVER_PROPERTIES
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_LABEL_MOUSE_OVER_PROPERTIES( QPushButton *label)
{
    Q_UNUSED ( label );
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_NEW_MENU_PROPERTIES
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_NEW_MENU_PROPERTIES(MENU_PROPERTIES menu_properties)
{
    m_menu_properties = menu_properties;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_MAIN_MENU_DEFAULTS
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_MAIN_MENU_DEFAULTS()
{
    if ( m_tab_widget->count() EQ 0 ) {
        QWidget *     main_widget           = new QWidget;

        QHBoxLayout * main_box_layout       = new QHBoxLayout;
        QHBoxLayout * backgroud_box_layout  = new QHBoxLayout;
        QVBoxLayout * backgroud_vbox_layout = new QVBoxLayout;

        main_widget->setLayout(main_box_layout);

        backgroud_box_layout->setContentsMargins( 2,2,2,2 );
        backgroud_box_layout->setSpacing        ( 2       );

        m_background->setObjectName        ("SEKME_MENU_KERNEL_BACKGROUD_WIDGET");

        m_background->setLayout            ( backgroud_vbox_layout                          );
        backgroud_vbox_layout->addLayout   ( backgroud_box_layout                           );

        m_bottom_label  = new QLabel;
        m_bottom_label->setText      (m_bottom_text);
        ADAK_SET_FONT_SIZE (m_bottom_label , 0.82);

        m_bottom_label->setAlignment (Qt::AlignCenter | Qt::AlignCenter);
        m_bottom_label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

        backgroud_vbox_layout->addWidget   (m_bottom_label);

        m_background->setSizePolicy        ( QSizePolicy::Ignored,QSizePolicy::Ignored );
        m_background->setScaledContents    ( true );

        for (int i = 0 ; i < m_num_of_columns ; i++ ) {
            QVBoxLayout * vertical_layout     = new QVBoxLayout;

            backgroud_box_layout->addLayout  ( vertical_layout );

            m_containers        << vertical_layout;
            m_container_widgets << NULL;
        }

        backgroud_box_layout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Expanding));

        main_box_layout->addWidget(m_background);

        ADD_TAB(main_widget,tr("New Tab"));

        statusBar()->addPermanentWidget(&m_sb_label_firma,1);
        statusBar()->addPermanentWidget(&m_sb_label_user,1);
        statusBar()->addPermanentWidget(&m_sb_label_pc,1);
        statusBar()->addPermanentWidget(&m_sb_label_version);

        QAction * action = new QAction(this);

        addAction              (action);
        m_tab_widget->addAction(action);

        action->setShortcuts(QList<QKeySequence>() << QKeySequence("ALT+Z") << QKeySequence("ALT+X")
                                                   << QKeySequence("ALT+C") << QKeySequence("ALT+J")
                                                   << QKeySequence("ALT+W") << QKeySequence("ALT+,")
                                                   << QKeySequence("ALT+V") );

        connect( action , SIGNAL(triggered()) , this , SLOT(OPEN_EKRANI_AC_DIALOG()));
    }

    statusBar()->setMaximumHeight(statusBar()->height() - 5);

    ADAK_SET_FONT_SIZE( statusBar(), 0.85 );

    m_sb_label_firma.setText( QString(tr("Database : <b>%1-%2</b>")).arg(VERITABANI_ISMI()).arg(VERITABANI_TANIMI()));
    m_sb_label_user.setText   ( QString(tr("User : <b>%1-%2</b>")).arg(KULLANICI_KODU()).arg(KULLANICI_ADI()));
    m_sb_label_pc.setText     ( QString(tr("Computer : <b>%1-%2</b>")).arg(BILGISAYAR_KODU()).arg(BILGISAYAR_ADI()));
    m_sb_label_version.setText( "<b>" + ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()) +"-"+ ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID())+ "</b>");


}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SET_WINDOW_DEFAULTS
*******************************************************************************************/

void SEKME_MENU_KERNEL::SET_WINDOW_DEFAULTS()
{
    setWindowTitle ( VERITABANI_ISMI() + " ( "+ ADAK_PROGRAM_LONGNAME( ADAK_DISPLAY_ID() )+" )");
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::GET_LABEL_WIDTH
*******************************************************************************************/

int SEKME_MENU_KERNEL::GET_LABEL_WIDTH ( int column_index    )
{
    if ( m_menu_properties.label_width EQ -1 ) {
        QFontMetrics font_metrics      ( m_menu_properties.font );
        //+24 ok ikon icin olan genislik
        m_menu_properties.label_width = (font_metrics.width('X') * m_column_lenghts.at(column_index)) + 24 ;
    }

    return m_menu_properties.label_width;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::GET_LABEL_HEIGHT
*******************************************************************************************/

int SEKME_MENU_KERNEL::GET_LABEL_HEIGHT( int )
{
    if ( m_menu_properties.label_heigth EQ -1 ) {
        QFontMetrics font_metrics(m_menu_properties.font);
        m_menu_properties.label_heigth = font_metrics.height();
    }

    return m_menu_properties.label_heigth;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::GET_MENU_PROPERTIES
*******************************************************************************************/

MENU_PROPERTIES SEKME_MENU_KERNEL::GET_MENU_PROPERTIES ()
{
    return m_menu_properties;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::CLEAR_SELECTED_FOCUS
*******************************************************************************************/

void SEKME_MENU_KERNEL::CLEAR_SELECTED_FOCUS (int menu_column )
{

    int colon = menu_column;
    int list  = m_selected_buttons.size();

    // ILK MENUDEN BIR TANE SECTE ISE ONCEKILERIN HEPSINI SILINIYOR
    if ( colon EQ 0 AND list NE 0 ) {

        for ( int i = menu_column ; i < m_selected_buttons.size() ; i++ ) {
            if ( m_selected_buttons.at(i) NE NULL ) {
                SET_LABEL_DEFAULT_PROPERTIES( m_selected_buttons.at(i) );
            }
        }
        m_selected_buttons.clear();
    }

    // IKINCI veya UCUNCU SEKMEDE BIR ONCEKI SILMEK ICIN
    if ( menu_column EQ 2 ) {
        menu_column = menu_column -1;
    }
    for ( int i = menu_column ; i < m_selected_buttons.size(); i++ ) {
        if ( m_selected_buttons.at(i) NE NULL ) {
            SET_LABEL_DEFAULT_PROPERTIES( m_selected_buttons.at(i) );
            m_selected_buttons.removeAt( i );
        }
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::CLOSE_PROGRAM
*******************************************************************************************/

bool SEKME_MENU_KERNEL::CLOSE_PROGRAM (bool no_message)
{

    ADAK_MSG_ENUM answer;

    // QAdakTabWidget geldigi zaman tan sayisi 0 oluyor , cunku tabwidget once Yeni sekmeyi remove edit kapatiyor.
    if ( m_tab_widget->count() EQ 0 ) {
        return true;
    }

    // YENI SEKME HARIC DIGER MENU SAYISI
    if ( m_tab_widgets.size() EQ 0 ) {
        if (no_message EQ false) {
            answer = MSG_YES_NO(tr("The program will be closed. Do you want to continue?"),NULL);//Program kapatılacaktır. Devam etmek istiyormusunuz?

            if ( answer EQ ADAK_NO ) {
                return false;
            }
        }

        return true;
    }

    // buraya gelirse mutlaka 1 den fazla tab aciktir
    if (no_message EQ false) {
        answer = MSG_YES_NO(tr("The program will be closed. There are currently open tabs. Do you want to continue?"),NULL);//Program kapatılacaktır. Açık olan Sekmeler var. Devam etmek istiyormusunuz?

        if ( answer EQ ADAK_NO ) {
            return false;
        }
    }

    for ( int i = ( m_tab_widget->count() - 1) ; i >= 0 ; i-- ) {
        if ( CLOSE_TAB(i) EQ false ) {
            return false;
        }
    }

    return true;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_MENU_CLICKED
*******************************************************************************************/

void SEKME_MENU_KERNEL::SLOT_MENU_CLICKED()
{
    if ( sender()->metaObject()->className() EQ QAdakTabWidget::staticMetaObject.className() ) {
        QWidget * widget = static_cast<QWidget *>(sender());

        int x = (-1 * widget->mapFromGlobal(QPoint(0,0)).x()) + 8;
        int y = (-1 * widget->mapFromGlobal(QPoint(0,0)).y()) + 8;

        m_menu->move(x,y);
        m_menu->show();
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_ITEM_CLICKED
*******************************************************************************************/

void SEKME_MENU_KERNEL::SLOT_ITEM_CLICKED(QString index_str , QObject * obj)
{
    int index = -1;

    if (sender() NE NULL AND obj EQ NULL) {
        obj = sender();
    }

    if ( index_str.isEmpty() EQ false ) {
        index = QVariant(index_str).toInt();
    }
    else {
        index = QVariant(obj->objectName()).toInt();
    }

    // ONCE REKLENDIRME
    if ( obj NE NULL ) {
        if (  obj->metaObject()->className() EQ QPushButton::staticMetaObject.className() ) {
            QPushButton * button = static_cast < QPushButton * >(obj);

            // sadece ilk sekme icin gecerli
            if ( m_menu_items.at(index)->menu_column EQ 0 AND m_menu_items.at(index)->num_of_childs NE 0 ) {
                if ( m_selected_buttons.contains( button ) EQ true ) {
                    return;
                }
            }

            CLEAR_SELECTED_FOCUS(m_menu_items.at(index)->menu_column);
            SET_LABEL_SELECTED_PROPERTIES( button, m_menu_items.at(index)->menu_column );
            m_selected_buttons << button;
        }
    }

    // EGER UC MENU ISE PENCERE AC
    if ( m_menu_items.at(index)->num_of_childs EQ 0 ) {
        if ( m_menu_items.at(index)->menu_column EQ 0 ) {
            CREATE_MENU             (m_menu_items.at(index)->enum_id);
        }
        MENU_ITEM_CLICKED(m_menu_items.at(index)->parent_enum_id,m_menu_items.at(index)->enum_id);
        return;
    }

    // DEGILSE ALT MENULERI AC
    CREATE_MENU(m_menu_items.at(index)->enum_id);
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_CLOSE_WIDGET
*******************************************************************************************/

void SEKME_MENU_KERNEL::SLOT_CLOSE_WIDGET(int index)
{
    if ( m_tab_widget->count() EQ 0 ) {
        close();
        return;
    }

    if ( m_close_events EQ true ) {
        return;
    }

    CLOSE_TAB(index);
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_CLOSE_LAST_TAB
*******************************************************************************************/

void SEKME_MENU_KERNEL::SLOT_CLOSE_LAST_TAB( QString tab_name )
{
    if ( tab_name EQ "Yeni Sekme" ) {
        close();
        return;
    }
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_CLOSE_CHILD_WIDGET
*******************************************************************************************/

void SEKME_MENU_KERNEL::SLOT_CLOSE_CHILD_WIDGET()
{
    int index = m_tab_widgets.indexOf(static_cast<QWidget *>(sender()));

    if ( index EQ -1 ) {
        int parent_index = m_tab_widgets.indexOf(static_cast<QWidget *>(sender())->parentWidget());

        if (parent_index NE -1 ) {
            m_tab_widgets.at(parent_index)->setFocus();
        }

        return;
    }

    CLOSE_TAB(index);
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_SET_CURRENT_TAB_INDEX
*******************************************************************************************/

void  SEKME_MENU_KERNEL::SLOT_SET_CURRENT_TAB_INDEX ( int index )
{
    if ( index >= 0) {
        m_tab_widget->setCurrentIndex( index );
    }
}


/******************************************************************************************
                        SEKME_MENU_KERNEL::GET_TAB_COUNT
*******************************************************************************************/

int  SEKME_MENU_KERNEL::GET_TAB_COUNT()
{
    return m_tab_widget->count();
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::CLOSE_TAB
*******************************************************************************************/

bool SEKME_MENU_KERNEL::CLOSE_TAB(int index)
{
    //Once objeler tmp e alinarak listten kaldirilmali.
    //Sonra close edilmeli. Tum child objeler yok edilirken parenttin
    //varligini listten kontrol edecekler.

    QMdiArea * mdi_area_buf = m_mdi_areas.at  (index);

    QList<QMdiSubWindow *> sub_window_list = mdi_area_buf->subWindowList();

    bool is_parent_widget_removed = false;

    for ( int i = 0 ; i < sub_window_list.size() ; i++ ) {
        m_tab_widget->SET_CURRENT_INDEX(index);
        if ( sub_window_list.at(i)->close() EQ false ) {
            return false;
        }

        if ( m_tab_widgets.indexOf(sub_window_list.at(i)->widget()) EQ index AND is_parent_widget_removed EQ false) {
            is_parent_widget_removed = true;
            m_tab_widgets.removeAt  (index);
        }
    }

    if ( is_parent_widget_removed EQ false ) {
        m_tab_widgets.removeAt(index);
    }
    m_tab_widget->REMOVE_TAB(index);
    m_mdi_areas  .removeAt  (index);

    mdi_area_buf->close();

    return true;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::REPAINT
*******************************************************************************************/

void SEKME_MENU_KERNEL::REPAINT()
{
    m_background->setPixmap    (QPixmap(m_menu_properties.background_img_path));
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::CLEAR_MENU_PROPERTIES_STRUCT
*******************************************************************************************/

void SEKME_MENU_KERNEL::CLEAR_MENU_PROPERTIES_STRUCT(MENU_PROPERTIES *menu_properties)
{
    menu_properties->font.setPointSize                      (11);

    menu_properties->parent_label_color.setRgb              (255,255,255      );
    menu_properties->parent_label_mouse_over_color.setRgb   (255,255,255      );
    menu_properties->child_label_color.setRgb               (232,232,21       );
    menu_properties->child_label_mouse_over_color.setRgb    (232,232,21       );
    menu_properties->header_color.setRgb                    (170,170,170      );
    menu_properties->select_label_color.setRgb              (244,159,48       );
    menu_properties->background_color.setRgba               (qRgba(0,0,0,255)   );
    menu_properties->background_img_path                    = "";

    //-1 olacaklar ihtiyac aninda dinamik olarak bir seferligine hesaplanacaklar
    menu_properties->label_heigth                           = -1;
    menu_properties->label_width                            = -1;

    menu_properties->column_font_diff                       = 1;
    menu_properties->mouse_over_font_inc                    = 0;
    menu_properties->default_font_weight                    = "font-weight:400;";
    menu_properties->mouse_over_font_weight                 = "font-weight:400;";
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::eventFilter
*******************************************************************************************/

bool SEKME_MENU_KERNEL::eventFilter(QObject * obj, QEvent * obj_event)
{
    if ( m_close_events EQ true ) {
        return false;
    }

    if ( obj->isWidgetType() EQ true ) {
        if ( m_tab_widgets.contains(static_cast<QWidget *>(obj)) EQ true ) {
            return true;
        }
    }

    if ( obj_event->type() EQ QEvent::KeyPress ) {
        QKeyEvent * key_event = static_cast<QKeyEvent *>(obj_event);
        // SADECE QADAKTABWIDGET VE SEKME MENU KERNELDAN GELENLER ICIN
        if ( obj->metaObject()->className() EQ QAdakTabWidget::staticMetaObject.className() ) {
            if ( key_event->key() EQ Qt::Key_Escape ) {
                m_tab_widget->CLOSE_TAB(m_tab_widget->currentIndex());
            }
        }
    }
    else if ( obj_event->type() EQ QEvent::Enter ) {
        if ( obj->metaObject()->className() EQ QLabel::staticMetaObject.className() ) {
            if ( m_selected_buttons.contains(static_cast< QPushButton * >(obj)) EQ false) {
                QApplication::restoreOverrideCursor();
            }
        }
    }
    else if ( obj_event->type() EQ QEvent::Leave ) {
        if ( obj->metaObject()->className() EQ QLabel::staticMetaObject.className() ) {
            if ( m_selected_buttons.contains(static_cast< QPushButton * >(obj)) EQ false) {
                QApplication::setOverrideCursor(Qt::PointingHandCursor);
            }
        }
    }
    else if ( obj_event->type() EQ QEvent::Close ) {
        if ( obj EQ this ) {
            if ( CLOSE_PROGRAM() EQ true ) {
                return false;
            }
            obj_event->ignore();
            return true;
        }
    }
    // mouse orta tusuna tiklandigi zaman kapatmak icin
    if ( obj_event->type() EQ QEvent::MouseButtonPress ) {
        QWheelEvent     * wheel_event = static_cast< QWheelEvent * >( obj_event );

        if ( obj->metaObject()->className() EQ QAdakTabWidget::staticMetaObject.className() ) {
            if ( wheel_event->buttons() EQ Qt::MidButton ) {
                if( m_tab_widget->GET_CURSOR_POS_TAB_INDEX() NE -1 ) {
                    m_tab_widget->CLOSE_TAB( m_tab_widget->GET_CURSOR_POS_TAB_INDEX() -1 );
                }
            }
        }
    }

    return false;
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SLOT_MENU_TOOL_BUTTON_CLICK
*******************************************************************************************/

void SEKME_MENU_KERNEL::SLOT_MENU_TOOL_BUTTON_CLICK()
{
    QPushButton * button = static_cast<QPushButton * >( sender() );

    SLOT_ITEM_CLICKED( button->objectName() );
}

/******************************************************************************************
                        SEKME_MENU_KERNEL::SISTEM_MENU_SHOWS
*******************************************************************************************/

void SEKME_MENU_KERNEL::SISTEM_MENU_SHOWS ( SYSTEM_MENU_ITEMS &SYSTEM_STRUCT )
{
    //SISTEM

    if ( SYSTEM_STRUCT.sis_blg_formuna_girebilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SIS_BILGISAYARLAR );
    }

    if ( SYSTEM_STRUCT.sis_kul_formuna_girebilir_mi EQ 1  ) {
        SHOW_MENU_ITEM ( MENU_SIS_KULLANICILAR );
    }

    if ( SYSTEM_STRUCT.sis_veritabani_formuna_girebilir_mi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SIS_VERITABANLARI );
    }

    if ( SYSTEM_STRUCT.sis_mail_srv_tanimlayabilirmi EQ 1  ) {
        SHOW_MENU_ITEM ( MENU_SIS_POSTA_SUNUCUSU );
    }

    if ( SYSTEM_STRUCT.sis_sms_srv_tanimlayabilirmi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SIS_SMS_SUNUCUSU );
    }

    if ( SYSTEM_STRUCT.sis_yonetim_log_inceleme_yetkisi EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SIS_YONETIM_LOG_BILGILERI );
    }

    if ( SYSTEM_STRUCT.sis_otomatik_giris_gorunsun_mu EQ 1 ) {
        SHOW_MENU_ITEM ( MENU_SIS_OTOMATIK_GIRIS_KAPAT   );
    }

    SHOW_MENU_ITEM ( MENU_SIS_HAKKIMIZDA             );
    SHOW_MENU_ITEM ( MENU_SIS_SIFRE_DEGISTIR         );
    SHOW_MENU_ITEM ( MENU_SIS_KULLANICI_DEGISTIR     );
    SHOW_MENU_ITEM ( MENU_SIS_VERITABANI_DEGISTIR    );
}

