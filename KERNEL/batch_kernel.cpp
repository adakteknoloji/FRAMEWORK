#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QHBoxLayout>
#include <QSearchEdit.h>
#include "batch_kernel.h"
#include "adak_defines.h"
#include "kernel_utils.h"
#include "kernel_private_utils.h"

/**************************************************************************************
                   BATCH_KERNEL::BATCH_KERNEL
***************************************************************************************/

BATCH_KERNEL::BATCH_KERNEL ( QWidget* parent ) : BASE_EVENT_KERNEL( parent )
{
    m_kernel_button_text        = "";

    p_name_of_run_batch_button  = "Tamam";
    p_icon_file_name            = ":/kernel_icons/kernel_run_icon.png";
    close_event_started         = false;
    p_run_batch_on_enter        = false;
    m_auto_exit                 = false;
    run_batch_button            = NULL;
}

/**************************************************************************************
                   BATCH_KERNEL::~BATCH_KERNEL
***************************************************************************************/

BATCH_KERNEL::~BATCH_KERNEL ()
{

}

/**************************************************************************************
                   BATCH_KERNEL::REGISTER_SAVE_BUTTON
***************************************************************************************/

void BATCH_KERNEL::REGISTER_SAVER_BUTTON ( QAbstractButton *button )
{
    disconnect(button,SIGNAL ( clicked()),0,0 );
    button->setObjectName ( "saver_button" ) ;
    connect(button, SIGNAL( clicked()), this, SLOT ( SLOT_CHECK_BUTTON() ) );
}

/**************************************************************************************
                   BATCH_KERNEL::SLOT_CHECK_BUTTON
***************************************************************************************/

void BATCH_KERNEL::SLOT_CHECK_BUTTON()
{
    if ( CHECK_VAR_CONTROL() EQ false ) {
        return;
    }

    if ( CHECK_RUN() EQ ADAK_FAIL ) {
        return;
    }

    QAbstractButton * button = static_cast<QAbstractButton *> ( sender() );

    RUN_BATCH();

    CLOSE_ALL_EVENTS();
    SAVER_BUTTON_CLICKED ( button );
    OPEN_ALL_EVENTS();

}

/**************************************************************************************
                   BATCH_KERNEL::SAVER_BUTTON_CLICKED
***************************************************************************************/

void BATCH_KERNEL::SAVER_BUTTON_CLICKED ( QAbstractButton * button )
{
    Q_UNUSED(button);
}

/**************************************************************************************
                   BATCH_KERNEL::START_BATCH_KERNEL
***************************************************************************************/

void BATCH_KERNEL::START_BATCH_KERNEL(QObject *parent, ADAK_SQL *database)
{
    SET_ADAK_WINDOW_DEFAULTS ( parent );
    SET_DB_CONN               ( database );
    //SET_EVENTS_AND_SIGNALS    ( parent,ADAK_BATCH_KERNEL );

    CLOSE_ALL_EVENTS          ();
    SETUP_FORM                ();
    OPEN_ALL_EVENTS           ();

    SET_EVENTS_AND_SIGNALS    ( parent,ADAK_BATCH_KERNEL );

    INIT_KERNEL               ();
}

/**************************************************************************************
                   BATCH_KERNEL::INIT_KERNEL
***************************************************************************************/

void BATCH_KERNEL::INIT_KERNEL  ()
{
    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   BATCH_KERNEL::SET_NAME_OF_RUN_BATCH_BUTTON
***************************************************************************************/

void BATCH_KERNEL::SET_NAME_OF_RUN_BATCH_BUTTON ( QString name_of_run_batch_button )
{
    p_name_of_run_batch_button = name_of_run_batch_button;
}

/**************************************************************************************
                   BATCH_KERNEL::SET_ENTER_KEY_FOR_RUN_BATCH
***************************************************************************************/

void BATCH_KERNEL::SET_ENTER_KEY_FOR_RUN_BATCH ( bool enable )
{
    p_run_batch_on_enter = enable;
}

/**************************************************************************************
                   BATCH_KERNEL::SET_ICON_OF_BATCH_BUTTON
***************************************************************************************/

void BATCH_KERNEL::SET_ICON_OF_BATCH_BUTTON ( QString filename )
{
    p_icon_file_name = filename;
}

/**************************************************************************************
                   BATCH_KERNEL::REGISTER_BUTTON_WIDGET
***************************************************************************************/

void BATCH_KERNEL::REGISTER_BUTTON_WIDGET (QWidget *batch_buttons_widget, bool is_yardim_button_visible, Qt::ToolButtonStyle style)
{
    QHBoxLayout * batch_buttons_widget_layout        = new QHBoxLayout ( batch_buttons_widget);

    batch_buttons_widget_layout->setSpacing(2);
    batch_buttons_widget_layout->setMargin (2);

    batch_buttons_widget->setSizePolicy    (QSizePolicy::Fixed,batch_buttons_widget->sizePolicy().verticalPolicy());

    batch_buttons_widget ->setObjectName   ( "kernel_widget");
    batch_buttons_widget ->setLayout       ( batch_buttons_widget_layout );

    batch_buttons_widget->setMinimumHeight (50);

    if ( is_yardim_button_visible EQ true ) {
        QToolButton * yardim_button = KERNEL_CREATE_DEFAULT_HELP_BUTTON(batch_buttons_widget);
        yardim_button -> setToolButtonStyle(style);
        yardim_button -> installEventFilter(this );

        batch_buttons_widget_layout -> addWidget( yardim_button );
        connect ( yardim_button   , SIGNAL(clicked()), this, SLOT ( YARDIM_BUTTON_CLICKED() ) );
    }

    run_batch_button = new QToolButton     ( batch_buttons_widget );

    run_batch_button -> setToolButtonStyle ( style                      );
    run_batch_button -> setText            ( p_name_of_run_batch_button );
    run_batch_button -> setIcon            ( QIcon ( p_icon_file_name ) );
    run_batch_button -> setIconSize        ( QSize(24,24) );

    run_batch_button -> installEventFilter ( this );

    batch_buttons_widget_layout -> addWidget( run_batch_button );

    connect(run_batch_button, SIGNAL(clicked()), this, SLOT(SLOT_RUN_BATCH_BUTTON()));
}

/**************************************************************************************
                   BATCH_KERNEL::SLOT_RUN_BATCH_BUTTON
***************************************************************************************/

void BATCH_KERNEL::SLOT_RUN_BATCH_BUTTON()
{
    run_batch_button->setDisabled(true);

    if ( CHECK_VAR_CONTROL() EQ false ) {
        run_batch_button->setDisabled(false);
        return;
    }

    if ( CHECK_RUN() EQ ADAK_FAIL ) {
        run_batch_button->setDisabled ( false );
        return;
    }
    RUN_BATCH();
    run_batch_button->setDisabled ( false );

    if ( m_auto_exit EQ true ) {
        accept();
    }
}

/**************************************************************************************
                   BATCH_KERNEL::eventFilter
***************************************************************************************/

bool BATCH_KERNEL::eventFilter ( QObject *obj, QEvent *event )
{
    //FIXME FORM DADA VAR AYNISI
    if ( event->type() EQ QEvent::MouseButtonPress ) {
        QMouseEvent * mouse_event = static_cast <QMouseEvent *>(event);

        if ( obj->metaObject()->className() EQ QLineEdit::staticMetaObject.className() AND mouse_event->button() EQ Qt::LeftButton ) {
            QLineEdit * line_edit = static_cast<QLineEdit *> (obj);
            if ( line_edit->hasFocus() EQ true AND line_edit->selectedText() NE line_edit->text() ) {
                if (line_edit->cursorPosition() EQ 0 ) {
                    line_edit->selectAll();
                    return true;
                }
            }
        }
    }

    //--------------------- USER VE RESIZE EVENTS

    if  ( event->type() EQ QEvent::User OR event->type() EQ QEvent::Resize ) {
         return BASE_KERNEL::eventFilter ( obj,event );
    }

    // ---------------------------- KEY PRESS

    if ( event->type() EQ QEvent::KeyPress ) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

         if ( obj->metaObject()->className() EQ QTextEdit::staticMetaObject.className() OR
              obj->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() OR
              obj->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
             if ( keyEvent->key() EQ Qt::Key_Tab ) {
                 focusNextChild();
                 return true;
             }
             return false;
         }

        if ( keyEvent->key() EQ Qt::Key_Escape ) {
            ADAK_CLOSE();
            return true;
        }

        if ( obj->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
            if ( event->type() EQ QEvent::Wheel ) {
                 return true;
            }
        }
        // run_bacth_button focus eklendi cunku run_batch cagrilmadan once focuslanmasi gerekiyor
        // boylece adakdate yapilan degisikligi daha once yapmis oluyor..
        if (  ( keyEvent->key() EQ Qt::Key_Enter OR keyEvent->key() EQ Qt::Key_Return) AND p_run_batch_on_enter ) {
            run_batch_button->setFocus();
            SLOT_RUN_BATCH_BUTTON();
            return true;
        }
    }

    // ---------------------------- CLOSE EVENT

    if ( event->type() EQ QEvent::Close ) {
        return BASE_KERNEL::eventFilter(obj,event);
    }

    if ( close_event_started EQ true) {
         return false;
    }

    // ---------------------------- FOCUS IN EVENT

    if ( event->type() EQ QEvent::FocusIn ) {
        QFocusEvent * focus_event = static_cast<QFocusEvent *>(event);

        bool return_value = CHECK_VAR_CONTROL ( obj );

        //FIXME FORM DADA VAR AYNISI
        if (focus_event->reason() EQ Qt::MouseFocusReason ) {
            if ( obj->metaObject()->className() EQ QLineEdit::staticMetaObject.className()) {
                QLineEdit * line_edit = static_cast<QLineEdit *> (obj);
                line_edit->setCursorPosition(0);
            }
        }

        return return_value;
    }

    return BASE_EVENT_KERNEL::eventFilter(obj,event);
}

/**************************************************************************************
                   BATCH_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE
***************************************************************************************/

void BATCH_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE ( int return_value )
{
    switch( return_value ) {
        case ADAK_FAIL:
            return;
            break;
        case ADAK_EXIT:
            abort();
        case ADAK_OK:
        default:
            break;
    }
    return;
}

/**************************************************************************************
                 BATCH_KERNEL::SET_AUTO_EXIT_BATCH
***************************************************************************************/

void BATCH_KERNEL::SET_AUTO_EXIT_BATCH( bool p_auto_exit )
{
    m_auto_exit = p_auto_exit;
}
