#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QMdiSubWindow>
#include <QEvent>
#include <QSearchEdit.h>
#include <QCommaEdit.h>
#include <QKeyEvent>
#include <QScrollBar>
#include <QCloseEvent>
#include <QCryptographicHash>
#include <QWebEngineView>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSettings>
#include <QHeaderView>
#include <QTreeWidget>

#include "base_kernel.h"
#include "kernel_utils.h"
#include "yonetim.h"
#include "email.h"
#include "adak_utils.h"

#include "sekme_menu_kernel.h"

QStringList G_FORM_NAME;
extern ADAK_SQL * G_YONETIM_DB;

enum {
    BASE_WINDOW_INVISIBLE_DEFAULT_SIZES = 0,
    BASE_WINDOW_VISIBLE_ON_QT           = 1,
    BASE_WINDOW_VISIBLE_ON_USER         = 2
};

extern QList<QWidget *> G_SEKME_MENU_KERNEL;

/**************************************************************************************
                   BASE_KERNEL::BASE_KERNEL
***************************************************************************************/

BASE_KERNEL::BASE_KERNEL ( QWidget * parent ) : QDialog ( parent )
{
    m_kernel_button_text    = "";

    m_first_focus_widget = NULL;
    m_db_conn            = NULL;

    G_FORM_NAME << objectName ();
    SET_PAGE_TITLE ( tr("ERROR!..  <<<<<  This screen unnamed >>>>>  ERROR !..") + objectName() );//HATA! Bu ekrana isim verilmemis.
    SET_HELP_PAGE ( "" );
    SET_SETTING_NAME ( "" );
    SET_THIS(this);

    m_widget_resized.clear();

    m_close_resize_slot  = false;
    m_screen_size_enum   = NOT_FULL_SCREEN;

    m_resize_count       = BASE_WINDOW_INVISIBLE_DEFAULT_SIZES;
}

/**************************************************************************************
                   BASE_KERNEL::~BASE_KERNEL
***************************************************************************************/

BASE_KERNEL::~BASE_KERNEL()
{
    G_FORM_NAME.removeLast();
}

/**************************************************************************************
                   BASE_KERNEL::exec
***************************************************************************************/

int BASE_KERNEL::exec()
{
    return QDialog::exec();
}


/**************************************************************************************
                   BASE_KERNEL::show
***************************************************************************************/

void BASE_KERNEL::show()
{
    return QDialog::show();
}

/**************************************************************************************
                   GET_PARENT_SEKME_MENU_KERNEL
***************************************************************************************/

SEKME_MENU_KERNEL * GET_PARENT_SEKME_MENU_KERNEL (QWidget * parent)
{
    for ( ; ; ) {
        if ( parent EQ NULL ) {
            qDebug("Parent window is not found. ");
        }

        if ( G_SEKME_MENU_KERNEL.contains( parent ) EQ true ) {
            return static_cast< SEKME_MENU_KERNEL * >( G_SEKME_MENU_KERNEL.at(G_SEKME_MENU_KERNEL.indexOf( parent ) ));
        }

        parent = parent->parentWidget();
    }
}

/**************************************************************************************
                   BASE_KERNEL::SHOW_AND_EXEC_COMMON_CODE
***************************************************************************************/

bool BASE_KERNEL::SHOW_AND_EXEC_COMMON_CODE( SCREEN_SIZE_ENUM p_screen_size )
{
    setWindowTitle                  (PAGE_TITLE());

    FIND_RESIZE_WIDGETS             (this);
    UPDATE_DEFAULT_WIDGET_SIZES     ();
    RESIZE_WIDGETS                  ();
    LOAD_WINDOW_SIZES               ();

    m_screen_size_enum = p_screen_size;

    m_resize_count = BASE_WINDOW_VISIBLE_ON_QT;

    if ( parentWidget() NE NULL AND G_SEKME_MENU_KERNEL.size() NE 0 ) {
        SEKME_MENU_KERNEL * MENU_KERNEL = GET_PARENT_SEKME_MENU_KERNEL( parentWidget() );

        if ( MENU_KERNEL NE NULL ) {
            if ( parentWidget() EQ MENU_KERNEL ) {
                MENU_KERNEL->ADD_PARENT_TAB( this, m_screen_size_enum);
                // acilan son tab indisine focuslaniyor
                int index = MENU_KERNEL->GET_TAB_COUNT();
                MENU_KERNEL->SLOT_SET_CURRENT_TAB_INDEX( index );
                // first widget focuslaniyor
                FOCUS_FIRST_WIDGET();
            }
            else {
                MENU_KERNEL->ADD_CHILD_DIALOG ( this, parentWidget(), m_screen_size_enum );
            }

            return false;
        }
    }

    return true;
}

/**************************************************************************************
                   BASE_KERNEL::EXEC
***************************************************************************************/

int BASE_KERNEL::EXEC(SCREEN_SIZE_ENUM p_screen_size)
{

    if ( SHOW_AND_EXEC_COMMON_CODE( p_screen_size ) EQ false ) {
        return 1;
    }

    return exec();
}

/**************************************************************************************
                   BASE_KERNEL::SHOW
***************************************************************************************/

void BASE_KERNEL::SHOW(SCREEN_SIZE_ENUM p_screen_size)
{

    if ( SHOW_AND_EXEC_COMMON_CODE( p_screen_size ) EQ false ) {
        return;
    }

    show();
}

/**************************************************************************************
                   BASE_KERNEL::SET_ADAK_WINDOW_DEFAULTS
***************************************************************************************/

void BASE_KERNEL::SET_ADAK_WINDOW_DEFAULTS ( QObject * this_ptr )
{
    QDialog * dialog = static_cast<QDialog *> ( this_ptr );

    m_main_this = dialog;

    dialog->setAttribute ( Qt::WA_DeleteOnClose, true );
    dialog->installEventFilter(this);

    SET_WINDOW_FLAGS(this);
}

/**************************************************************************************
                   BASE_KERNEL::SET_FIRST_FOCUS_WIDGET
***************************************************************************************/

void BASE_KERNEL::SET_FIRST_FOCUS_WIDGET ( QWidget *focus_widget )
{
    m_first_focus_widget = focus_widget;
}

/**************************************************************************************
                   BASE_KERNEL::SET_PAGE_TITLE
***************************************************************************************/

void BASE_KERNEL::SET_PAGE_TITLE ( QString page_title )
{
    G_FORM_NAME.removeLast();
    m_page_title = page_title;
    setWindowTitle(m_page_title);
    G_FORM_NAME << page_title;
}

/**************************************************************************************
                   BASE_KERNEL::SET_HELP_PAGE
***************************************************************************************/

void BASE_KERNEL::SET_HELP_PAGE ( QString help_page )
{
    m_help_page = help_page;
}

/**************************************************************************************
                   BASE_KERNEL::SET_SETTING_NAME
***************************************************************************************/

void BASE_KERNEL::SET_SETTING_NAME ( QString setting_name )
{
    m_setting_name = setting_name;
    UPDATE_DEFAULT_WIDGET_SIZES();
}

/**************************************************************************************
                   BASE_KERNEL::PAGE_TITLE
***************************************************************************************/

QString BASE_KERNEL::PAGE_TITLE()
{
    return m_page_title;
}

/**************************************************************************************
                   BASE_KERNEL::HELP_PAGE
***************************************************************************************/

QString BASE_KERNEL::HELP_PAGE()
{
    return m_help_page;
}

/**************************************************************************************
                   BASE_KERNEL::SETTINGE_NAME
***************************************************************************************/

QString BASE_KERNEL::SETTING_NAME()
{
    return m_setting_name;
}

/**************************************************************************************
                   BASE_KERNEL::FOCUS_FIRST_WIDGET
***************************************************************************************/

void BASE_KERNEL::FOCUS_FIRST_WIDGET()
{
    if ( m_first_focus_widget EQ NULL ) {
        return;
    }
    m_first_focus_widget->clearFocus();
    m_first_focus_widget->setFocus();
}

/**************************************************************************************
                   BASE_KERNEL::SET_DB_CONN
***************************************************************************************/

void BASE_KERNEL::SET_DB_CONN ( ADAK_SQL * db )
{
    if (db EQ NULL) {
        return;
    }

    m_db_conn = db;
    m_num_of_transactions = m_db_conn->m_sql_transaction_counter;
}

/**************************************************************************************
                   BASE_KERNEL::CHECK_EXIT
***************************************************************************************/

int BASE_KERNEL::CHECK_EXIT()
{
    return ADAK_OK;
}

/**************************************************************************************
                   BASE_KERNEL::ADAK_CLOSE
***************************************************************************************/

void BASE_KERNEL::ADAK_CLOSE()
{
    if (parentWidget() NE NULL) {
        if(parentWidget()->metaObject()->className() EQ QMdiSubWindow::staticMetaObject.className()) {
            parentWidget()->close();
            return;
        }
    }

    close();
}

/**************************************************************************************
                   BASE_KERNEL::START_KERNEL_TRANSACTION
***************************************************************************************/

void BASE_KERNEL::START_KERNEL_TRANSACTION()
{
    if (m_db_conn EQ NULL) {
        return;
    }
    m_db_conn->START_TRANSACTION();
}

/**************************************************************************************
                   BASE_KERNEL::COMMIT_KERNEL_TRANSACTION
***************************************************************************************/

void BASE_KERNEL::COMMIT_KERNEL_TRANSACTION()
{
    if (m_db_conn EQ NULL) {
        return;
    }
    m_db_conn->COMMIT_TRANSACTION();
}

/**************************************************************************************
                   BASE_KERENEL :: CANCEL_KERNEL_TRANSACTION
***************************************************************************************/

void BASE_KERNEL::CANCEL_KERNEL_TRANSACTION()
{
    if (m_db_conn EQ NULL) {
        return;
    }
    m_db_conn->CANCEL_TRANSACTION();
}

/**************************************************************************************
                   BASE_KERNEL :: IGNORE_CLOSE_EVENT
***************************************************************************************/

bool BASE_KERNEL::IGNORE_CLOSE_EVENT ( QEvent * event )
{
    QCloseEvent * close_event = static_cast<QCloseEvent *>(event);
    close_event->ignore();
    return true;
}

/**************************************************************************************
                   BASE_KERNEL::OPEN_HELP_PAGE
***************************************************************************************/

void BASE_KERNEL::OPEN_HELP_PAGE(QString yardim_sayfasi)
{
    YARDIM_BUTTON_CLICKED(yardim_sayfasi);
}

/**************************************************************************************
                   BASE_KERNEL::YARDIM_BUTTON_CLICKED
***************************************************************************************/

void BASE_KERNEL::YARDIM_BUTTON_CLICKED(QString yardim_sayfasi)
{
    QString url = yardim_sayfasi;
    if (url.isEmpty() EQ true) {
        url = HELP_PAGE();
    }
    if (url.isEmpty() EQ true) {
        SHOW_NO_HELP_DIALOG ();
        return;
    }

    if (url.contains("http") EQ false) {
        //url.prepend( "http://www.adak.com.tr/" );
        url.prepend( "http://utafs.com/" );
    }

    if (IS_ADAK_HELP_URL_VALID (url) EQ false) {
        return;
    }

    QDesktopServices::openUrl(url);

//    QDialog  * online_help = new QDialog(this);
//    online_help->setModal(false);
//    online_help->setMinimumWidth(500);
//    online_help->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//    QVBoxLayout * layout = new QVBoxLayout();
//    online_help->setLayout(layout);

//    QWebView * help = new QWebView();
//    help->setUrl(url);
//    layout->addWidget(help);

//    online_help->setWindowTitle ( url );
//    online_help->adjustSize();
//    online_help->show();

}

/**************************************************************************************
                  BASE_KERNEL::IS_ADAK_HELP_URL_VALID
***************************************************************************************/

bool BASE_KERNEL::IS_ADAK_HELP_URL_VALID ( QString url )
{
    QUrl help_url(url);
    QNetworkAccessManager * network_access_manager = new QNetworkAccessManager();
    QNetworkReply * network_reply = network_access_manager->get(QNetworkRequest(help_url));
    QEventLoop answer_loop;
    answer_loop.connect(network_reply, SIGNAL(finished()), &answer_loop, SLOT(quit()));
    QTimer bekleme_suresi;
    answer_loop.connect(&bekleme_suresi, SIGNAL(timeout()), &answer_loop, SLOT(quit()));
    bekleme_suresi.start(10000);
    answer_loop.exec();

    int http_status_code = network_reply->attribute ( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

    if (http_status_code EQ 404) {
        SHOW_NO_HELP_DIALOG ();
        return false;
    }
    return true;
}

/**************************************************************************************
                  BASE_KERNEL::SHOW_NO_HELP_DIALOG
***************************************************************************************/

void BASE_KERNEL::SHOW_NO_HELP_DIALOG ()
{
    QString prog_name = ADAK_PROGRAM_LONGNAME(ADAK_DISPLAY_ID()) + " ( " + ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID()) + "- v" + ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID()) + " )";

    QLabel * label_program_adi_txt          =    new QLabel(tr("Program Name: "));
    m_line_edit_program_adi                 =    new QLineEdit(prog_name);
    m_line_edit_program_adi->setStyleSheet("font:monospace; color:blue;");
    m_line_edit_program_adi->setReadOnly(true);

    QLabel      * label_email_adres     = new QLabel( tr("Your E-Mail Address :" ));
    m_lineEdit_email_adres  = new QLineEdit;

    QLabel * label_bulunamayan_sayfa_txt    =    new QLabel(tr("Page can be Found at: "));//Bulunamayan Sayfa

    QString bulunamayan_sayfa = objectName();

    if (HELP_PAGE().isEmpty() EQ false) {
        bulunamayan_sayfa = HELP_PAGE();
    }

    m_line_edit_bulunamayan_sayfa           =    new QLineEdit( bulunamayan_sayfa );
    m_line_edit_bulunamayan_sayfa->setStyleSheet("font:monospace; color:blue;"    );
    m_line_edit_bulunamayan_sayfa->setReadOnly(true);

    QToolButton         * send_email        = new QToolButton;
    send_email->setText(tr("Report Adak Technology."));//Adak Teknoloji ' ye Bildir.
    send_email->setIcon(QIcon(":/yonetim_icons/yonetim_mail.png"));
    send_email->setIconSize(QSize(32,32));
    send_email->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect( send_email, SIGNAL( clicked() ), this, SLOT( SLOT_SEND_ADAK_ERROR_EMAIL() ) );

    QGridLayout * screen_layout             = new QGridLayout();

    QLabel * label_header = new QLabel ( tr("ERROR - Help page not found."));
    label_header->setStyleSheet ( "font:monospace; color:red;");

    ADAK_SET_FONT_SIZE( label_header, 2.00 ); // x2

    screen_layout->addWidget(label_header, 0, 0, 2, 3, Qt::AlignCenter);

    screen_layout->addWidget( new QLabel(""), 3, 0, 1, 2 );

    screen_layout->addWidget(label_program_adi_txt, 5, 0, 1, 1 );
    screen_layout->addWidget(m_line_edit_program_adi, 5, 1, 1, 3 );

    screen_layout->addWidget(label_bulunamayan_sayfa_txt, 6, 0, 1, 1 );
    screen_layout->addWidget(m_line_edit_bulunamayan_sayfa, 6, 1, 1, 3 );

    screen_layout->addWidget( label_email_adres, 7, 0, 1, 1 );
    screen_layout->addWidget( m_lineEdit_email_adres, 7, 1, 1, 3 );


    QLabel * label_note = new QLabel ( tr("Note: If you deliver this error message to Adak technology,\n"
                                          "help pages will be prepared as soon as possible."));//yardım sayfası hazırlanacaktır.
    label_note->setStyleSheet("font:bold;");
    screen_layout->addWidget(label_note,8,0,1,3);

    screen_layout->addWidget(send_email                     ,10,2,1,1,Qt::AlignRight);
    screen_layout->setColumnStretch(1,5);

    m_error_dialog = new QDialog(this);
    m_error_dialog->setLayout(screen_layout);
    m_error_dialog->setWindowTitle(tr("ERROR"));
    m_error_dialog->setAttribute(Qt::WA_DeleteOnClose);
    m_error_dialog->show();

    send_email->setFocus();
}

/**************************************************************************************
                  BASE_KERNEL::SLOT_SEND_ADAK_ERROR_EMAIL
***************************************************************************************/

void BASE_KERNEL::SLOT_SEND_ADAK_ERROR_EMAIL()
{
    if ( m_lineEdit_email_adres->text().isEmpty() EQ false ) {
        SEND_ADAK_ERROR_EMAIL();
        m_error_dialog->close();
    }
    else {
        MSG_WARNING( tr( "Please write your E-mail address!" ), m_lineEdit_email_adres );//Lütfen E-Posta Adresinizi yazın.
    }
}

/**************************************************************************************
                  BASE_KERNEL::SET_CURSOR_POS_TEXT_WIDGETS
***************************************************************************************/

void BASE_KERNEL::SET_CURSOR_POS_TEXT_WIDGETS(QObject * parent)
{
    if (parent EQ NULL) {
        if (this->parent() NE NULL) {
            parent = this->parent();
        }
        else {
            parent = this;
        }
    }

    QList<QObject *> parentin_child_objeleri = parent->children();

    for ( int i=0 ; i < parentin_child_objeleri.count(); i++ ) {
        if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
            QLineEdit * line_edit = static_cast<QLineEdit *>(parentin_child_objeleri.at(i));
            line_edit->setCursorPosition(0);
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
            QCommaEdit * comma_edit = static_cast<QCommaEdit *>(parentin_child_objeleri.at(i));
            comma_edit->setCursorPosition(0);
        }
        else if( parentin_child_objeleri.at(i)->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
            QLineEdit * line_edit = static_cast<QSearchEdit *> ( parentin_child_objeleri.at(i) )->findChild<QLineEdit *>("search_edit_line_edit");
            line_edit->setCursorPosition(0);
        }

        QList<QObject *> childin_child_objeleri = parentin_child_objeleri.at(i)->children();

        if ( childin_child_objeleri.count() > 0 ) {
            SET_CURSOR_POS_TEXT_WIDGETS ( parentin_child_objeleri.at(i) );
        }
    }
    return;

}

/**************************************************************************************
                  BASE_KERNEL::SEND_ADAK_ERROR_EMAIL
***************************************************************************************/

void BASE_KERNEL::SEND_ADAK_ERROR_EMAIL()
{
    QString from;
    SQL_QUERY sql_query ( G_YONETIM_DB );
    sql_query.PREPARE_SELECT ( "ynt_kullanicilar","email_adresi","kullanici_id = :kullanici_id AND silinmis_mi = 0","");
    sql_query.SET_VALUE      ( ":kullanici_id", KULLANICI_ID() );
    if ( sql_query.SELECT() EQ 0 ) {
        from    = "No Name (Not defined yet)";
    }
    else {
        sql_query.NEXT();
        from    = sql_query.VALUE(0).toString();
    }

    QString subject       = tr ( "Help page is not found." );

    QString mesaj_icerigi = tr("Program Name      : ") + m_line_edit_program_adi      ->text()   + "\n\n" +
                            tr("Opareting System  : ") + ADAK_ISLETIM_SISTEMI()                 + "\n\n" +
                            tr("E-Mail Address   : ") + m_lineEdit_email_adres->text()           + "\n\n" +
                            tr("Page can be Found at: ") + m_line_edit_bulunamayan_sayfa->text()   + "  " + PAGE_TITLE() + "\n\n";

    EMAIL_SET_EMAIL_SERVER  ( "smtp.gmail.com", 587, STARTTLS  );
    EMAIL_SET_USER_EMAIL_INFO ( "adakerror", "qaz123XSW", "Adak Error Sender", true );
    SEND_MAIL ( "adakerror@gmail.com", from, ADAK_PROGRAM_EMAIL(ADAK_DISPLAY_ID()),  subject, mesaj_icerigi , -1 , -1 , tr("Your message has been sent.") , false);
}

/**************************************************************************************
                  BASE_KERNEL::eventFilter
***************************************************************************************/

bool BASE_KERNEL::eventFilter ( QObject *obj, QEvent *event )
{
    //-------------------------------------------RESIZE_EVENT

    if ( event->type() EQ QEvent::Resize ) {
        if ( this EQ obj ) {
            if ( m_resize_count EQ BASE_WINDOW_VISIBLE_ON_USER ) {
                RESIZE_WIDGETS();
                SAVE_WINDOW_SIZES();
                SET_CURSOR_POS_TEXT_WIDGETS();
            }
            else {
                m_resize_count = BASE_WINDOW_VISIBLE_ON_USER;
            }
        }

        if (obj->metaObject()->className() EQ QScrollBar::staticMetaObject.className()) {
            QScrollBar * scroll_bar = static_cast<QScrollBar *>(obj);
            if ( scroll_bar->orientation() EQ Qt::Horizontal ) {
                return false;
            }
        }

        if ( QApplication::mouseButtons() EQ Qt::LeftButton ) {
            return false;
        }

        RESIZE_WIDGETS();

        return false;
    }

    if ( event->type() EQ QEvent::Show OR event->type() EQ QEvent::Hide ) {
        RESIZE_WIDGETS();
    }

    //-------------------------------------------KEY-PRESS EVENT

    if ( event->type() EQ QEvent::KeyPress ) {
        QKeyEvent * key_event = static_cast<QKeyEvent *> ( event );

        if ( key_event->key() EQ Qt::Key_Escape ) {
            ADAK_CLOSE();
            return true;
        }

        if ( key_event->modifiers() EQ Qt::AltModifier ) {
            if ( key_event->key() EQ Qt::Key_X OR
                 key_event->key() EQ Qt::Key_Z OR
                 key_event->key() EQ Qt::Key_C OR
                 key_event->key() EQ Qt::Key_J OR
                 key_event->key() EQ Qt::Key_V OR
                 key_event->key() EQ Qt::Key_Comma OR
                 key_event->key() EQ Qt::Key_W )

                GET_PARENT_SEKME_MENU_KERNEL(parentWidget())->OPEN_EKRANI_AC_DIALOG();
                return true;
        }
    }

    //----------------------------------------------CLOSE EVENT

    if ( event->type() EQ QEvent::Close ) {
        if ( m_db_conn NE NULL ) {
            if ( m_num_of_transactions NE m_db_conn->m_sql_transaction_counter ) {
                 MSG_ERROR(tr ( "Forgot open TRANSACTION, you must end! .." ) , NULL);//Açık TRANSACTION unuttunuz , sonlandırmanız gerekmektedir!..
                 abort();
            }
        }
        int return_value = CHECK_EXIT();
        if ( return_value EQ ADAK_OK OR return_value EQ ADAK_EXIT ) {
            ADAK_CLOSE();
            return false;
        }
        return IGNORE_CLOSE_EVENT(event);
    }
    return false;
}

/**************************************************************************************
                  BASE_KERNEL::FIND_RESIZE_WIDGETS
***************************************************************************************/

void BASE_KERNEL::FIND_RESIZE_WIDGETS ( QObject * parent )
{
    QList<QObject *> parentin_child_objeleri = parent->children();

    for ( int i=0 ; i < parentin_child_objeleri.count(); i++ ) {
        //FIXME header degisikliginde SET lenebilir hale getirilmeli.
        if ( parentin_child_objeleri.at(i)->objectName() EQ "NO_EVENT_ON_WIDGET") {
            continue;
        }

        if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTableWidget::staticMetaObject.className() ) {

            QTableWidget * table_widget = static_cast<QTableWidget *> ( parentin_child_objeleri.at(i) );

            table_widget->setAlternatingRowColors(true);

            table_widget->horizontalHeader()->installEventFilter(this);
            table_widget->verticalScrollBar()->installEventFilter(this);
            table_widget->horizontalScrollBar()->installEventFilter(this);

            m_resized_table_widgets << table_widget;
            m_resized_tree_widgets  << NULL;
            m_resized_views         << static_cast<QAbstractItemView  *>(parentin_child_objeleri.at(i));
            m_resized_models        << static_cast<QTableWidget *>(parentin_child_objeleri.at(i))->model();
            m_widget_resized        << false;
        }

        if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTreeWidget::staticMetaObject.className() ) {

            QTreeWidget * tree_widget = static_cast<QTreeWidget *>(parentin_child_objeleri.at(i));

            tree_widget->setAlternatingRowColors(true);

            tree_widget->header()->installEventFilter(this);
            tree_widget->verticalScrollBar()->installEventFilter(this);
            tree_widget->horizontalScrollBar()->installEventFilter(this);

            m_resized_table_widgets << NULL;
            m_resized_tree_widgets  << tree_widget;
            m_resized_views         << static_cast<QAbstractItemView  *>(parentin_child_objeleri.at(i));
            m_resized_models        << static_cast<QTreeWidget *>(parentin_child_objeleri.at(i))->model();
            m_widget_resized        << false;
        }

        QList<QObject *> childin_child_objeleri = parentin_child_objeleri.at(i)->children();

        if ( childin_child_objeleri.count() > 0 ) {
            FIND_RESIZE_WIDGETS(parentin_child_objeleri.at(i));
        }
    }
}

/**************************************************************************************
                  BASE_KERNEL::RESIZE_WIDGETS
***************************************************************************************/

void BASE_KERNEL::RESIZE_WIDGETS()
{
    m_close_resize_slot = true;

    for (int i = 0 ; i < m_resized_views.size() ; i++ ) {
        QHeaderView * header_view = NULL;
        double widget_header_width   = 0.00;

        if (m_resized_table_widgets.at(i) NE NULL) {
            header_view             = m_resized_table_widgets.at(i)->horizontalHeader();
            widget_header_width     = m_resized_table_widgets.at(i)->width();

            if ( m_resized_table_widgets.at(i)->verticalHeader()->isVisible() EQ true ) {
                widget_header_width -= m_resized_table_widgets.at(i)->verticalHeader()->width();
            }

            if ( m_resized_table_widgets.at(i)->verticalScrollBar()->isVisible() EQ true ) {
                widget_header_width -= m_resized_table_widgets.at(i)->verticalScrollBar()->width();
            }

            m_resized_table_widgets.at(i)->horizontalHeader()->setMinimumSectionSize(0);
            m_resized_table_widgets.at(i)->horizontalHeader()->setStretchLastSection(true);
            m_resized_table_widgets.at(i)->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        }
        else {
            header_view             = m_resized_tree_widgets.at(i)->header();
            widget_header_width     = m_resized_tree_widgets.at(i)->width();
        }

        double fazlalik = widget_header_width * 0.000;

        int column_id    = -1;
        int column_width = 0;

        QString default_size_str = m_resized_widgets_default_size.at(i);

        QStringList columns_default_size = default_size_str.split("-");

        for ( int j = 0 ; j < columns_default_size.size() ; j++ ) {
            double new_column_width = widget_header_width * columns_default_size.at(j).toDouble();

            if ( new_column_width > column_width) {
                column_id    = j;
                column_width = new_column_width;
            }

            if ( m_resized_table_widgets.at(i) NE NULL ) {
                if ( new_column_width NE 0.00) {
                    m_resized_table_widgets.at(i)->setColumnWidth  ( j, new_column_width);
                }
            }
            else {
                if ( new_column_width NE 0.00) {
                    m_resized_tree_widgets.at(i)->setColumnWidth (  j, new_column_width );
                }
            }
        }

        if ( column_id NE -1 ) {
            if ( m_resized_table_widgets.at(i) NE NULL ) {
                m_resized_table_widgets.at(i)->setColumnWidth ( column_id, column_width - fazlalik );
            }
            else {
                m_resized_tree_widgets.at(i)->setColumnWidth (  column_id, column_width - fazlalik );
            }
        }
    }

    m_close_resize_slot = false;
}

/**************************************************************************************
                   BASE_KERNEL::UPDATE_DEFAULT_WIDGET_SIZES
***************************************************************************************/

void BASE_KERNEL::UPDATE_DEFAULT_WIDGET_SIZES()
{
    if ( m_resized_views.size() EQ 0 ) {
        return;
    }

    for ( int i = 0 ; i < m_resized_views.size() ; i++ ) {
        double sum_of_columns = 0.00;

        if ( m_resized_table_widgets.at(i) NE NULL ) {
            if ( m_resized_table_widgets.at(i)->objectName() EQ "FIS_KERNEL_TABLE_WIDGET" ) {
                QStringList button_column = m_resized_table_widgets.at(i)->accessibleName().split("\t");
                m_resized_table_widgets.at(i)->setColumnWidth(button_column.at(0).toInt() , 30);
                m_resized_table_widgets.at(i)->setColumnWidth(button_column.at(1).toInt() , 30);
            }
        }

        for ( int j = 0 ; j < m_resized_models.at(i)->columnCount() ; j++ ) {
            if ( m_resized_table_widgets.at(i) NE NULL ) {
                sum_of_columns += m_resized_table_widgets.at(i)->columnWidth(j);
            }
            else {
                sum_of_columns += m_resized_tree_widgets.at(i)->columnWidth(j);
            }
        }

        QString default_size_str = "";

        for ( int j = 0 ; j < m_resized_models.at(i)->columnCount() ; j++ ) {
            if ( default_size_str.isEmpty() EQ false ) {
                 default_size_str.append("-");
            }

            if ( m_resized_table_widgets.at(i) NE NULL) {
                double column_width = QVariant(m_resized_table_widgets.at(i)->columnWidth(j)).toDouble();

                if ( column_width NE 0.00 ) {
                    column_width = column_width / sum_of_columns;
                }

                default_size_str.append ( QVariant(column_width ).toString() );
            }
            else {
                double column_width = QVariant(m_resized_tree_widgets.at(i)->columnWidth(j)).toDouble();

                if ( column_width NE 0.00 ) {
                    column_width = column_width / sum_of_columns;
                }

                default_size_str.append ( QVariant(ROUND(column_width) ).toString() );
            }
        }

        if ( i < m_resized_widgets_default_size.size() ) {
            m_resized_widgets_default_size[i] = default_size_str;
        }
        else {
            m_resized_widgets_default_size <<   default_size_str;
        }
    }
}

/**************************************************************************************
                   BASE_KERNEL::SAVE_WINDOW_SIZES
***************************************************************************************/

void BASE_KERNEL::SAVE_WINDOW_SIZES()
{
    QSettings settings ( "ADAK_SETTINGS",ADAK_PROGRAM_SHORTNAME ( ADAK_DISPLAY_ID() ) );

    settings.setValue ( QString ( SETTING_NAME() + "." + objectName() + "." + "IS_WINDOW_MAXIMIZED"  ), QVariant ( isMaximized() ) );
    settings.setValue ( QString ( SETTING_NAME() + "." + objectName() + "." + "WINDOW_WIDTH"  )       , QVariant ( width() ) );
    settings.setValue ( QString ( SETTING_NAME() + "." + objectName() + "." + "WINDOW_HEIGHT" )       , QVariant ( height() ) );
}

/**************************************************************************************
                   BASE_KERNEL::LOAD_WINDOW_SIZES
***************************************************************************************/

void BASE_KERNEL::LOAD_WINDOW_SIZES()
{
    QSettings settings ( "ADAK_SETTINGS",ADAK_PROGRAM_SHORTNAME ( ADAK_DISPLAY_ID() ) );

    bool is_maximized = settings.value ( QString ( SETTING_NAME() + "." + objectName() + "." + "IS_WINDOW_MAXIMIZED" ),QVariant(false)).toBool();

    if  ( is_maximized EQ true ) {
        setWindowState ( Qt::WindowMaximized );
        return;
    }

    int  m_default_width    = width();
    int  m_default_height   = height();

    int  width        = settings.value ( QString ( SETTING_NAME() + "." + objectName() + "." + "WINDOW_WIDTH"  )       ,QVariant ( m_default_width ) ).toInt();
    int  height       = settings.value ( QString ( SETTING_NAME() + "." + objectName() + "." + "WINDOW_HEIGHT" )       ,QVariant ( m_default_height ) ).toInt();

    if ( width EQ m_default_width AND height EQ m_default_height ) {
        if ( isMaximized() EQ true ) {
            return;
        }
    }

    if ( width < minimumWidth() OR height < minimumHeight() ) {
        return;
    }

    resize(width,height);
}
