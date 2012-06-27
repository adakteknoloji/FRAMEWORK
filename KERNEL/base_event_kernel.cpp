#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QWebView>
#include <QLabel>
#include <QCalendarWidget>

#include "QCommaEdit.h"
#include "QSearchEdit.h"
#include "QAdakDate.h"

#include "adak_utils.h"
#include "email.h"
#include "base_event_kernel.h"
#include "yonetim.h"

/**************************************************************************************
                   BASE_EVENT_KERNEL::BASE_EVENT_KERNEL
***************************************************************************************/

BASE_EVENT_KERNEL::BASE_EVENT_KERNEL ( QWidget * parent )  : BASE_KERNEL ( parent )
{
    m_search_clicked_started    = false;
    m_widget_undo_value         = "";
    m_current_widget            = NULL;
    m_event_eklenmiyecek_widget = NULL;
    m_last_focused_widget       = NULL;
    m_previous_widget           = NULL;
    m_next_focus_widget         = NULL;
    m_check_var_is_open         = true;
    m_is_all_events_open        = true;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::BASE_EVENT_KERNEL
***************************************************************************************/

BASE_EVENT_KERNEL::~BASE_EVENT_KERNEL()
{

}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SET_EVENTS_AND_SIGNALS
***************************************************************************************/

void BASE_EVENT_KERNEL::SET_EVENTS_AND_SIGNALS ( QObject * parent, int kernel_type )
{
    QList<QObject *> parentin_child_objeleri = parent->children();

    for ( int i=0 ; i < parentin_child_objeleri.count(); i++ ) {

        bool add_widget_to_list = true;

        if ( parentin_child_objeleri.at(i) EQ m_event_eklenmiyecek_widget ) {
             continue;
        }
        else if ( parentin_child_objeleri.at(i)->objectName() EQ "kernel_widget" ) {
             continue;
        }
        else if ( m_widget_lists.contains(parentin_child_objeleri.at(i)) EQ true ) {
            if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() OR
                 parentin_child_objeleri.at(i)->metaObject()->className() EQ QDateEdit::staticMetaObject.className() OR
                 parentin_child_objeleri.at(i)->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() OR
                 parentin_child_objeleri.at(i)->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {

                continue;
            }

            QList<QObject *> childin_child_objeleri = parentin_child_objeleri.at(i)->children();

            if ( childin_child_objeleri.count() > 0 ) {
                SET_EVENTS_AND_SIGNALS ( parentin_child_objeleri.at(i),kernel_type );
            }

            continue;
        }
        else if (  parentin_child_objeleri.at(i)->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {
            QAdakDate * adak_date = static_cast<QAdakDate *>(parentin_child_objeleri.at(i));
            adak_date->installEventFilter(this);

            QLineEdit * line_edit = adak_date->findChild<QLineEdit *>("adak_date_line_edit");
            QToolButton * tool_button = adak_date->findChild<QToolButton *>("adak_date_tool_button");

            line_edit->installEventFilter(this);
            tool_button->installEventFilter(this);

            connect ( adak_date, SIGNAL ( SIGNAL_DATE_CHANGED() ), this, SLOT ( CHECK_VAR_SLOT_EVENT()));
            connect ( adak_date, SIGNAL ( destroyed ( QObject*)), this, SLOT ( WIDGET_DESTROYED(QObject *)));

            m_widget_lists << parentin_child_objeleri.at(i);
            continue;
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
            QLineEdit * line_edit = static_cast<QLineEdit *> ( parentin_child_objeleri.at(i) );

            QRegExp rx ("[^\r\n]*");
            QRegExpValidator * validator = new QRegExpValidator ( rx, this );
            line_edit->setValidator(validator);

            line_edit->installEventFilter(this);
            if (kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                if (line_edit->isReadOnly() EQ false AND line_edit->isEnabled() EQ true AND line_edit->isHidden() EQ false) {
                    if ( line_edit->maxLength() EQ 32767 ) {                        
                        ADAK_ERROR(QString("Uzunluk atanmamış : %1").arg(line_edit->objectName()),NULL,NULL);
                        line_edit->setMaxLength(0);
                    }
                }
                connect ( line_edit, SIGNAL ( textEdited ( QString  ) ), this, SLOT ( WIDGET_CHANGED() ) );
                connect ( line_edit, SIGNAL ( textChanged( QString  ) ), this, SLOT ( WIDGET_TEXT_CHANGED() ) );
                connect ( line_edit, SIGNAL ( destroyed  ( QObject* ) ), this, SLOT ( WIDGET_DESTROYED(QObject *)));
            }
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QCheckBox::staticMetaObject.className() ) {
            QCheckBox * check_box = static_cast<QCheckBox *>(parentin_child_objeleri.at(i) );
            check_box->installEventFilter(this);
            connect ( check_box, SIGNAL(stateChanged(int)), this, SLOT(CHECK_VAR_SLOT_EVENT() ) );
            if ( kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                connect ( check_box, SIGNAL ( stateChanged(int) ), this, SLOT ( WIDGET_CHANGED() ) );
                connect ( check_box, SIGNAL ( destroyed ( QObject* ) ), this, SLOT ( WIDGET_DESTROYED(QObject *)));
            }
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QRadioButton::staticMetaObject.className() ) {
            QRadioButton * radio_button = static_cast<QRadioButton *>(parentin_child_objeleri.at(i));
            radio_button->installEventFilter(this);
            connect(radio_button, SIGNAL ( toggled ( bool ) ), this, SLOT ( CHECK_VAR_SLOT_EVENT() ) );
            if (kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                connect(radio_button, SIGNAL( toggled ( bool ) ), this, SLOT ( WIDGET_CHANGED()));
                connect(radio_button, SIGNAL( destroyed ( QObject* ) ), this, SLOT ( WIDGET_DESTROYED(QObject *)));
            }
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
            QComboBox * combo_box = static_cast<QComboBox *>(parentin_child_objeleri.at(i));
            combo_box->installEventFilter(this);
            connect(combo_box, SIGNAL(currentIndexChanged(int)),      this, SLOT(CHECK_VAR_SLOT_EVENT()));
            if ( combo_box->lineEdit() NE NULL) {
                combo_box->setInsertPolicy(QComboBox::NoInsert);
                connect(combo_box->lineEdit(),SIGNAL(editingFinished()),this,SLOT(COMBO_BOX_LINE_EDIT_CHANGED()));
                connect(combo_box->lineEdit(),SIGNAL ( textEdited  ( QString ) ), this, SLOT ( WIDGET_CHANGED() ) );
                connect(combo_box, SIGNAL(destroyed(QObject*)), this, SLOT ( WIDGET_DESTROYED(QObject *)));
            }
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
            QTextEdit * text_edit = static_cast<QTextEdit *>(parentin_child_objeleri.at(i));
            text_edit->installEventFilter(this);
            if ( kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                if (text_edit->isReadOnly() EQ false AND text_edit->isEnabled() EQ true AND text_edit->isHidden() EQ false) {
                    MSG_ERROR("TextEdit kullanilmis. TextEdit yerine QLimitedTextEdit kullanilarak limitlenmesi gerekmektedir.",NULL);
                }
                connect(text_edit, SIGNAL(textChanged()), this, SLOT(WIDGET_CHANGED()));
                connect(text_edit, SIGNAL(textChanged()), this, SLOT(WIDGET_TEXT_CHANGED()));
                connect(text_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
            }
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
            QLimitedTextEdit * limited_text_edit = static_cast<QLimitedTextEdit *>(parentin_child_objeleri.at(i));
            limited_text_edit->installEventFilter(this);
            if (kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                connect(limited_text_edit, SIGNAL(textChanged()), this, SLOT(WIDGET_CHANGED()));
                connect(limited_text_edit, SIGNAL(textChanged()), this, SLOT(WIDGET_TEXT_CHANGED()));
                connect(limited_text_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
            }
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
            QPlainTextEdit * plain_text_edit = static_cast<QPlainTextEdit *>(parentin_child_objeleri.at(i));
            plain_text_edit->installEventFilter(this);
            if ( kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                if (plain_text_edit->isReadOnly() EQ false AND plain_text_edit->isEnabled() EQ true AND plain_text_edit->isHidden() EQ false) {
                    MSG_ERROR("TextEdit kullanilmis. TextEdit yerine QLimitedTextEdit kullanilarak limitlenmesi gerekmektedir.",NULL);
                }
                connect ( plain_text_edit, SIGNAL ( textChanged() ), this, SLOT ( WIDGET_CHANGED() ) );
                connect ( plain_text_edit, SIGNAL ( textChanged() ), this, SLOT ( WIDGET_TEXT_CHANGED() ) );
                connect ( plain_text_edit, SIGNAL ( destroyed(QObject*) ), this, SLOT ( WIDGET_DESTROYED ( QObject *)));
            }
        }

        else if( parentin_child_objeleri.at(i)->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
            QCommaEdit * comma_edit = static_cast<QCommaEdit *>(parentin_child_objeleri.at(i));
            comma_edit->installEventFilter(this);
            if ( kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                 connect ( comma_edit, SIGNAL(textEdited(QString)), this, SLOT(WIDGET_CHANGED()));
                 connect ( comma_edit, SIGNAL(textChanged(QString)), this, SLOT(WIDGET_TEXT_CHANGED()));
                 connect ( comma_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
            }
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() ) {
            QTimeEdit * time_edit = static_cast<QTimeEdit *>(parentin_child_objeleri.at(i));
            time_edit->installEventFilter(this);
            if ( kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                 connect(time_edit, SIGNAL(timeChanged(QTime)), this, SLOT(WIDGET_CHANGED()));
                 connect(time_edit, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
            }
            m_widget_lists << parentin_child_objeleri.at(i);
	    continue;
        }
        else if (  parentin_child_objeleri.at(i)->metaObject()->className() EQ QDateEdit::staticMetaObject.className() ) {
            QDateEdit * date_edit = static_cast<QDateEdit *>(parentin_child_objeleri.at(i));
            if ( date_edit->calendarPopup() EQ false ) {
                date_edit->setCalendarPopup(true);
            }
            date_edit->calendarWidget()->setFirstDayOfWeek ( Qt::Monday );

            date_edit->installEventFilter(this);
            connect ( date_edit, SIGNAL ( dateChanged ( QDate ) ), this, SLOT ( CHECK_VAR_SLOT_EVENT()),Qt::QueuedConnection);
            connect ( date_edit, SIGNAL ( destroyed ( QObject*)), this, SLOT ( WIDGET_DESTROYED(QObject *)));
            m_widget_lists << parentin_child_objeleri.at(i);
            continue;
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTableWidget::staticMetaObject.className() ) {
            if (kernel_type NE ADAK_FIS_KERNEL) {
                QTableWidget * table_widget = static_cast<QTableWidget *>(parentin_child_objeleri.at(i) );
                table_widget->installEventFilter(this);
                connect(table_widget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(WIDGET_ITEM_CHANGED()));
                connect(table_widget, SIGNAL(cellDoubleClicked(int,int))     , this, SLOT(WIDGET_ITEM_DOUBLE_CLICKED(int,int)));
                connect(table_widget, SIGNAL(cellClicked(int,int))           , this, SLOT(WIDGET_ITEM_CLICKED(int,int)));
                connect(table_widget, SIGNAL(destroyed(QObject*)), this, SLOT(WIDGET_DESTROYED(QObject *)));
            }
        }
        else if( parentin_child_objeleri.at(i)->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
            QSearchEdit * search_edit = static_cast<QSearchEdit *>(parentin_child_objeleri.at(i));
            QLineEdit * line_edit = search_edit->findChild<QLineEdit *>("search_edit_line_edit");
            QToolButton * tool_button = search_edit->findChild<QToolButton *>("search_edit_tool_button");
            line_edit->installEventFilter(this);
            tool_button->installEventFilter(this);

            m_widget_lists << (QObject *)tool_button;
            m_widget_lists << (QObject *)line_edit;
            m_widget_lists << parentin_child_objeleri.at(i);

            if (kernel_type EQ ADAK_FORM_KERNEL OR kernel_type EQ ADAK_FIS_KERNEL) {
                connect(line_edit,   SIGNAL ( textEdited  ( QString ) ), this, SLOT ( WIDGET_CHANGED() ) );
                connect(line_edit,   SIGNAL ( textChanged ( QString ) ), this, SLOT ( WIDGET_TEXT_CHANGED() ) );
                connect(line_edit,   SIGNAL ( destroyed   ( QObject*) ), this, SLOT ( WIDGET_DESTROYED ( QObject *)));
            }
            connect ( search_edit, SIGNAL ( SIGNAL_BUTTON_CLICKED ( QWidget*,QLineEdit*,QToolButton* ) ),    this, SLOT(SEARCH_BUTTON_CLICKED(QWidget *,QLineEdit *,QToolButton*)));
	    continue;
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QPushButton::staticMetaObject.className() ) {
            QPushButton * push_button = static_cast<QPushButton *> ( parentin_child_objeleri.at(i));
            push_button->installEventFilter ( this );
            if (push_button->objectName() NE "saver_button" AND push_button->objectName() NE "changer_button" ) {
                push_button->setObjectName("normal_button");
                connect(push_button, SIGNAL ( clicked() ),      this, SLOT ( CHECK_VAR_SLOT_EVENT() ) );
            }
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QToolButton::staticMetaObject.className() ) {
            QToolButton * tool_button = static_cast<QToolButton *> ( parentin_child_objeleri.at(i) );
            tool_button->installEventFilter(this);
            if ( tool_button->objectName() NE "saver_button" AND tool_button->objectName() NE "changer_button" ) {
                 tool_button->setObjectName ( "normal_button" );
                connect ( tool_button, SIGNAL ( clicked() ),  this, SLOT ( CHECK_VAR_SLOT_EVENT() ) );
            }
        }
        else {
            add_widget_to_list = false;
        }

        if ( add_widget_to_list EQ true ) {
            m_widget_lists << parentin_child_objeleri.at(i);
        }

        QList<QObject *> childin_child_objeleri = parentin_child_objeleri.at(i)->children();

        if ( childin_child_objeleri.count() > 0 ) {
            SET_EVENTS_AND_SIGNALS ( parentin_child_objeleri.at(i),kernel_type );
        }
    }
    return;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::CHECK_VAR_CONTROL
***************************************************************************************/

bool BASE_EVENT_KERNEL::CHECK_VAR_CONTROL ( QObject * object )
{
    if (m_check_var_is_open EQ false ) {
        return false;
    }

    if ( object EQ NULL AND m_current_widget EQ NULL ) {
         return true;
    }

    CLOSE_CHECK_VAR_CONTROL();
    bool return_value = CHECK_VAR_FOCUS_EVENT ( object );
    OPEN_CHECK_VAR_CONTROL();

    return return_value;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::CHECK_VAR_FOCUS_EVENT
***************************************************************************************/

bool BASE_EVENT_KERNEL::CHECK_VAR_FOCUS_EVENT ( QObject * object )
{
    int return_value;

    //------------------------------- Herhangi bir button a basilip sadece son obje icin check var gerekirse buraya duser.

    if ( object EQ NULL AND m_current_widget NE NULL ) {
        //Dikkat buraya sadece son objeyi kontrol icin duser bu durumda sadece object null gelir.
        if ( IS_SLOT_WIDGETS(m_current_widget) EQ true OR IS_WIDGET_CHANGED() EQ false ) {
             return true;
        }

        /////FIXME OZEL BIR DURUM LINEEDITLERDEKI SPACELERI YOKEDER  (SONDAKI)  ///////

        if ( m_current_widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
            QString value = static_cast<QLineEdit *>(m_current_widget)->text();

            value = value.trimmed();

            static_cast<QLineEdit *>(m_current_widget)->setText(value);
        }
        else if ( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
            QString value = static_cast<QSearchEdit *>(m_current_widget)->GET_TEXT();

            value = value.trimmed();

            static_cast<QSearchEdit *>(m_current_widget)->SET_TEXT(value);
        }

        /////FIXME OZEL BIR DURUM LINEEDITLERDEKI SPACELERI YOKEDER (SONDAKI) /////////////////////

        return_value = CHECK_VAR ( m_current_widget );

        if ( return_value EQ ADAK_OK ) {
            INCREASE_DEGISEN_WIDGET_COUNT ( m_current_widget );
            RESET_CHECK_VAR_VARIABLES();
            return true;
        }
        else if ( return_value EQ ADAK_FAIL_UNDO ) {
             UNDO_CHANGES();
        }
        else {
            m_next_focus_widget = NULL;
        }

        return false;
    }

    //--------------------------------- Event Filter dan cagrilirsa donus degerleri event filtera uygun olmali.

    if ( object NE NULL ) {
        if ( object->objectName() EQ "search_edit_line_edit") {
            object = object->parent();
        }
        if ( object->objectName() EQ "adak_date_line_edit" OR object->objectName() EQ "adak_date_tool_button") {
            object = object->parent();
        }
    }

    if ( object EQ m_current_widget ) {
         return false;
    }

    if ( m_current_widget NE NULL AND object NE NULL ) {
        if ( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
            QToolButton * tool_button = m_current_widget->findChild<QToolButton *>("search_edit_tool_button");
            if ( tool_button->hasFocus() EQ true ) {
                return false;
            }
        }
    }

    return_value = FOCUS_IN_EVENT ( object );

    if ( return_value EQ ADAK_OK ) {
        return false;
    }

    PROCESS_CHECK_VAR_RETURN_VALUE ( return_value );

    return true;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::IS_WIDGET_FOCUSABLE
***************************************************************************************/

bool BASE_EVENT_KERNEL::IS_WIDGET_FOCUSABLE(QWidget *focus_widget)
{
    m_next_focus_widget = focus_widget;

    return false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::CHECK_VAR_SLOT_EVENT
***************************************************************************************/

void BASE_EVENT_KERNEL::CHECK_VAR_SLOT_EVENT ( QObject * object )
{
    if ( m_check_var_is_open EQ false ) {
         return;
    }

    QObject * check_var_object = NULL;

    if ( object NE NULL) {
         check_var_object = object;
    }
    else {
        check_var_object = sender();
    }

    //Button olan objeler

    if ( check_var_object->metaObject()->className() EQ QPushButton::staticMetaObject.className() OR
         check_var_object->metaObject()->className() EQ QToolButton::staticMetaObject.className() ) {
	CLOSE_ALL_EVENTS ();
        CHECK_VAR ( check_var_object );
	OPEN_ALL_EVENTS ();
        return;
    }

    if ( check_var_object->metaObject()->className() EQ QRadioButton::staticMetaObject.className() ) {
         QRadioButton * radio_button = static_cast<QRadioButton *> ( check_var_object );
        if ( radio_button->isChecked() EQ false) {
            m_previous_widget = radio_button;
            return;
        }
    }

    //QAdakDate

    if (check_var_object->parent() NE NULL ) {
        if ( check_var_object->parent()->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {
            check_var_object = check_var_object->parent();
        }
    }

    /////////////////

    //Geri kalanlar objeler

    CLOSE_CHECK_VAR_CONTROL();

    int return_value = CHECK_VAR ( check_var_object );

    if ( return_value EQ ADAK_FAIL OR return_value EQ ADAK_FAIL_UNDO) {
        UNDO_CHANGES();
    }
    else {
        m_previous_widget = NULL;
    }

    INCREASE_DEGISEN_WIDGET_COUNT ( ( QWidget * )check_var_object);
    WIDGET_CHANGED();    

    PROCESS_CHECK_VAR_RETURN_VALUE ( return_value );

    OPEN_CHECK_VAR_CONTROL();
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::FOCUS_IN_EVENT
***************************************************************************************/

int BASE_EVENT_KERNEL::FOCUS_IN_EVENT ( QObject * current_object )
{
    int return_value = FOCUS_OUT_EVENT();

    if ( return_value NE ADAK_OK ) {
        if ( m_current_widget NE NULL ) {
            if ( m_current_widget->hasFocus() EQ false ) {
                 m_current_widget->setFocus();
            }
        }
        return return_value;
    }

    RESET_CHECK_VAR_VARIABLES();

    if (current_object NE NULL) {
        m_current_widget = static_cast<QWidget *>(current_object);
    }

    if (m_current_widget EQ NULL) {
        return ADAK_OK;
    }

    if ( m_current_widget->metaObject()->className() EQ QCheckBox::staticMetaObject.className() ) {
        if ( static_cast<QCheckBox *>(m_current_widget)->checkState() EQ Qt::Checked ) {
            m_widget_undo_value = "Checked";
        }
        else {
            m_widget_undo_value = "Unchecked";
        }
    }
    if ( m_current_widget->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
        m_widget_undo_value.setNum(static_cast<QComboBox *>(m_current_widget)->currentIndex());
    }
    if ( m_current_widget->metaObject()->className() EQ QDateEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QDateEdit *>(m_current_widget)->date().toString("yyyy.MM.dd");
    }
    if ( m_current_widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QLineEdit *>(m_current_widget)->text();
        if ( m_widget_undo_value.isEmpty() EQ false ) {
            static_cast<QLineEdit *>(m_current_widget)->setSelection(m_widget_undo_value.length(),-m_widget_undo_value.length());
        }
    }
    if ( m_current_widget->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QCommaEdit *>(m_current_widget)->GET_TEXT();
        if ( m_widget_undo_value.isEmpty() EQ false ) {
            static_cast<QCommaEdit *>(m_current_widget)->setSelection(m_widget_undo_value.length(),-m_widget_undo_value.length());
        }
    }
    if ( m_current_widget->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QTextEdit *> ( m_current_widget )->toPlainText();
        if ( m_widget_undo_value.isEmpty() EQ false ) {
            static_cast<QTextEdit *>(m_current_widget)->selectAll();
        }
    }
    if ( m_current_widget->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QLimitedTextEdit *>(m_current_widget)->toPlainText();
        if ( m_widget_undo_value.isEmpty() EQ false ) {
            static_cast<QLimitedTextEdit *>(m_current_widget)->selectAll();
        }
    }
    if ( m_current_widget->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QPlainTextEdit *> ( m_current_widget )->toPlainText();
        if ( m_widget_undo_value.isEmpty() EQ false ) {
            static_cast<QPlainTextEdit *>(m_current_widget)->selectAll();
        }
    }
    if( m_current_widget->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QTimeEdit *>(m_current_widget)->text();
    }
    if( m_current_widget->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QAdakDate *>(m_current_widget)->DATE();
    }
    if( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
        m_widget_undo_value = static_cast<QSearchEdit *> ( m_current_widget )->GET_TEXT();
        if ( m_widget_undo_value.isEmpty() EQ false ) {
            QLineEdit * line_edit = static_cast<QSearchEdit *> ( m_current_widget )->findChild<QLineEdit *>("search_edit_line_edit");
            line_edit->setSelection(m_widget_undo_value.length(),-m_widget_undo_value.length());
        }
    }

    return ADAK_OK;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::FOCUS_OUT_EVENT
***************************************************************************************/

int BASE_EVENT_KERNEL::FOCUS_OUT_EVENT ( QObject * object )
{
    Q_UNUSED ( object );

    if  ( m_current_widget EQ NULL) {
         return ADAK_OK;
    }

    if ( m_current_widget->isWidgetType() EQ false ) {
         return ADAK_OK;
    }

    if( m_current_widget->objectName() EQ "search_edit_line_edit" OR m_current_widget->objectName() EQ "search_edit_tool_button") {
        RESET_CHECK_VAR_VARIABLES();
        return ADAK_OK;
    }

    if ( IS_SLOT_WIDGETS(m_current_widget) EQ true) {
         return ADAK_OK;
    }

    if ( IS_WIDGET_CHANGED() EQ false) {
         RESET_CHECK_VAR_VARIABLES();
         return ADAK_OK;
    }

    /////FIXME OZEL BIR DURUM LINEEDITLERDEKI SPACELERI YOKEDER  (SONDAKI)  ///////

    if ( m_current_widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
        QString value = static_cast<QLineEdit *>(m_current_widget)->text();

        value = value.trimmed();

        static_cast<QLineEdit *>(m_current_widget)->setText(value);
    }
    else if ( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
        QString value = static_cast<QSearchEdit *>(m_current_widget)->GET_TEXT();

        value = value.trimmed();

        static_cast<QSearchEdit *>(m_current_widget)->SET_TEXT(value);
    }

    /////FIXME OZEL BIR DURUM LINEEDITLERDEKI SPACELERI YOKEDER (SONDAKI) /////////////////////

    int return_value = CHECK_VAR(m_current_widget);

    if ( return_value EQ  ADAK_OK ) {
        INCREASE_DEGISEN_WIDGET_COUNT(m_current_widget);
    }
    else if ( return_value EQ ADAK_FAIL_UNDO) {
         UNDO_CHANGES();
         return_value = ADAK_FAIL;
    }
    else {
        m_next_focus_widget = NULL;
    }

    return return_value;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::IS_WIDGET_CHANGED
***************************************************************************************/

bool BASE_EVENT_KERNEL::IS_WIDGET_CHANGED()
{
    if ( m_current_widget EQ NULL) {
         return false;
    }

    if ( m_current_widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
        if ( m_widget_undo_value NE static_cast<QLineEdit *>(m_current_widget)->text() ) {
            return true;
        }
    }

    else if ( m_current_widget->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
        if ( m_widget_undo_value NE static_cast<QCommaEdit *>(m_current_widget)->GET_TEXT() ) {
            return true;
        }
    }

    else if ( m_current_widget->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
        if ( m_widget_undo_value NE static_cast<QTextEdit *>(m_current_widget)->toPlainText() ) {
            return true;
        }
    }

    else if ( m_current_widget->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
        if ( m_widget_undo_value NE static_cast<QLimitedTextEdit *>(m_current_widget)->toPlainText() ) {
            return true;
        }
    }

    else if ( m_current_widget->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
        if ( m_widget_undo_value NE static_cast<QPlainTextEdit *>(m_current_widget)->toPlainText() ) {
            return true;
        }
    }

    else if ( m_current_widget->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() ) {
        if ( m_widget_undo_value NE static_cast<QTimeEdit *>(m_current_widget)->text() ) {
            return true;
        }
    }

    else if ( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
        if ( m_search_clicked_started EQ true ) {
            return false;
        }
        if ( m_widget_undo_value NE static_cast<QSearchEdit *> ( m_current_widget )->GET_TEXT() ) {
            return true;
        }
    }

    return false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::UNDO_CHANGES
***************************************************************************************/

void BASE_EVENT_KERNEL::UNDO_CHANGES()
{
    if ( m_current_widget EQ NULL) {
        return;
    }

    m_current_widget->blockSignals ( true );

    if ( m_current_widget->metaObject()->className() EQ QCheckBox::staticMetaObject.className() ) {
        if ( m_widget_undo_value.contains("Checked") ) {
            static_cast<QCheckBox *>(m_current_widget)->setCheckState(Qt::Checked);
        }
        else {
            static_cast<QCheckBox *> ( m_current_widget )->setCheckState ( Qt::Unchecked );
        }
    }

    if ( m_current_widget->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
        static_cast<QComboBox *> ( m_current_widget)->setCurrentIndex ( m_widget_undo_value.toInt() );
    }
    if ( m_current_widget->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {
        static_cast<QAdakDate *> ( m_current_widget)->SET_DATE(m_widget_undo_value);
    }
    if ( m_current_widget->metaObject()->className() EQ QDateEdit::staticMetaObject.className() ) {
        static_cast<QDateEdit *> ( m_current_widget )->setDate ( QDate::fromString ( m_widget_undo_value,"yyyy.MM.dd"));
    }
    if ( m_current_widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
        static_cast<QLineEdit *> ( m_current_widget )->setText ( m_widget_undo_value );
    }
    if ( m_current_widget->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
        static_cast<QCommaEdit *> ( m_current_widget )->UNDO_TEXT ( m_widget_undo_value );
    }
    if ( m_current_widget->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
        static_cast<QTextEdit *> ( m_current_widget )->setText ( m_widget_undo_value );
    }
    if ( m_current_widget->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
        static_cast<QLimitedTextEdit *>( m_current_widget )->setText ( m_widget_undo_value );
    }
    if ( m_current_widget->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
        static_cast<QPlainTextEdit *> ( m_current_widget)->setPlainText ( m_widget_undo_value);
    }
    if( m_current_widget->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() ) {
        static_cast<QTimeEdit *> ( m_current_widget )->setTime ( QTime::fromString ( m_widget_undo_value,"hh:mm"));
    }
    if( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
        static_cast<QSearchEdit *> ( m_current_widget )->SET_TEXT ( m_widget_undo_value );
    }
    if (  m_current_widget->metaObject()->className() EQ QRadioButton::staticMetaObject.className() ) {
        if ( m_previous_widget NE NULL ) {
             m_previous_widget->blockSignals ( true );
            static_cast<QRadioButton *> ( m_previous_widget )->setChecked ( true );
            m_previous_widget->blockSignals ( false );
            m_previous_widget = NULL;
        }

    }

    m_current_widget->blockSignals ( false );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::COMBO_BOX_LINE_EDIT_CHANGED
***************************************************************************************/

void BASE_EVENT_KERNEL::COMBO_BOX_LINE_EDIT_CHANGED()
{
    QLineEdit * line_edit = static_cast<QLineEdit *> ( sender() );
    QComboBox * combo_box = static_cast<QComboBox *> ( sender()->parent() );

    QString current_text = line_edit->text();

    if ( current_text.isEmpty() ) {
        return;
    }

    if ( combo_box->findText ( current_text ) NE -1 ) {
        return;
    }

    combo_box->addItem ( line_edit->text() );
    combo_box->setCurrentIndex ( combo_box->findText ( line_edit->text() ) );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SET_FOCUS
***************************************************************************************/

void BASE_EVENT_KERNEL::SET_FOCUS(QWidget *focus_widget)
{
    if (focus_widget EQ NULL) {
        return;
    }

    m_next_focus_widget = focus_widget;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::RESET_KERNEL_VARIABLES
***************************************************************************************/

void BASE_EVENT_KERNEL::RESET_KERNEL_VARIABLES()
{
    if ( m_current_widget NE NULL ) {
         m_current_widget->clearFocus();
         SET_LAST_FOCUSED_WIDGET ( m_current_widget );
    }

    m_current_widget            = NULL;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::RESET_CHECK_VAR_VARIABLES
***************************************************************************************/

void BASE_EVENT_KERNEL::RESET_CHECK_VAR_VARIABLES()
{
    if ( m_current_widget NE NULL) {
         m_current_widget->clearFocus();
    }
    m_current_widget = NULL;
    m_widget_undo_value.clear();
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SEARCH_EDIT_LINEEDIT_CHANGED
***************************************************************************************/

void BASE_EVENT_KERNEL::SEARCH_EDIT_LINEEDIT_CHANGED ( QWidget * widget, QLineEdit * line_edit, QToolButton * tool_button)
{
    Q_UNUSED ( tool_button );
    Q_UNUSED ( line_edit );

    if ( m_search_clicked_started EQ true ) {
        return;
    }

    /////FIXME OZEL BIR DURUM LINEEDITLERDEKI SPACELERI YOKEDER  (SONDAKI)  ///////

    if ( m_current_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
        QString value = static_cast<QSearchEdit *>(widget)->GET_TEXT();

        value = value.trimmed();

        static_cast<QSearchEdit *>(widget)->SET_TEXT(value);
    }

    /////FIXME OZEL BIR DURUM LINEEDITLERDEKI SPACELERI YOKEDER (SONDAKI) /////////////////////

    int return_value = CHECK_VAR ( widget );

    if ( return_value EQ ADAK_FAIL ) {
        QString value = line_edit->text();
        line_edit->clear();
        if (line_edit->hasFocus() EQ false) {
            line_edit->setFocus();
        }
        line_edit->setText ( value );
        m_next_focus_widget = NULL;
    }
    else if ( return_value EQ ADAK_FAIL_UNDO ) {
        UNDO_CHANGES();
    }
    else if ( return_value EQ ADAK_OK  ){
        ( ( QSearchEdit *)widget)->SET_TEXT ( line_edit->text() );
        INCREASE_DEGISEN_WIDGET_COUNT ( ( ( QSearchEdit * )widget ) );
    }
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SEARCH_BUTTON_CLICKED
***************************************************************************************/

void BASE_EVENT_KERNEL::SEARCH_BUTTON_CLICKED ( QWidget * widget,QLineEdit * line_edit,QToolButton * tool_button)
{
    if ( m_search_clicked_started EQ true ) {
        return;
    }

    m_search_clicked_started = true;

    QString widget_degeri = line_edit->text();
    if ( tool_button NE NULL) {
        SEARCH_EDIT_CLICKED ( widget,line_edit );
        if ( widget_degeri NE line_edit->text() ) {
            if ( TRY_LOCK_RECORD() EQ false ) {
                m_search_clicked_started = false;
                return;
            }
            int return_value = CHECK_VAR ( widget );

            if ( return_value EQ ADAK_OK AND line_edit->text().isEmpty() EQ false) {
                 INCREASE_DEGISEN_WIDGET_COUNT ( widget );
                 WIDGET_CHANGED();
            }
            else if ( return_value EQ ADAK_FAIL_UNDO ) {
                UNDO_CHANGES();
            }
            else {
                m_next_focus_widget = NULL;
            }
        }
    }
    widget->setFocus();
    m_search_clicked_started = false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SEARCH_EDIT_CLICKED
***************************************************************************************/

void BASE_EVENT_KERNEL::SEARCH_EDIT_CLICKED ( QWidget * widget,QLineEdit * line_edit )
{
    Q_UNUSED ( widget ) ;
    Q_UNUSED ( line_edit );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::DISABLE_EVENTS_ON_WIDGET
***************************************************************************************/

void BASE_EVENT_KERNEL::DISABLE_EVENTS_ON_WIDGET ( QWidget * widget )
{
   m_event_eklenmiyecek_widget = widget;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::WIDGET_CHANGED
***************************************************************************************/

void BASE_EVENT_KERNEL::WIDGET_CHANGED()
{
    TRY_LOCK_RECORD();
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::RESET_WIDGET_CHANGED
***************************************************************************************/

void BASE_EVENT_KERNEL::RESET_WIDGET_CHANGED()
{

}

/**************************************************************************************
                   BASE_EVENT_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE
***************************************************************************************/

void BASE_EVENT_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE  ( int return_value )
{
    Q_UNUSED ( return_value );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::IS_SLOT_WIDGETS
***************************************************************************************/

bool BASE_EVENT_KERNEL::IS_SLOT_WIDGETS ( QObject * widget )
{
    if ( widget->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
        return true;
    }
    else if ( widget->metaObject()->className() EQ QCheckBox::staticMetaObject.className() ) {
        return true;
    }
    else if ( widget->metaObject()->className() EQ QRadioButton::staticMetaObject.className() ) {
        return true;
    }
    else if ( widget->metaObject()->className() EQ QDateEdit::staticMetaObject.className() ) {
        return true;
    }
    else if ( widget->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {
        return true;
    }

    return false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::TRY_LOCK_RECORD
***************************************************************************************/

bool BASE_EVENT_KERNEL::TRY_LOCK_RECORD()
{
    return true;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT
***************************************************************************************/

void BASE_EVENT_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT ( QWidget * )
{

}

/**************************************************************************************
                   BASE_EVENT_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT
***************************************************************************************/

void BASE_EVENT_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT ( QWidget * )
{

}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SET_CURRENT_WIDGET
***************************************************************************************/

void BASE_EVENT_KERNEL::SET_CURRENT_WIDGET ( QWidget *widget )
{
    if ( m_current_widget EQ widget ) {
        return;
    }
    m_current_widget = NULL;
    FOCUS_IN_EVENT ( widget );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SET_LAST_FOCUSED_WIDGET
***************************************************************************************/

void BASE_EVENT_KERNEL::SET_LAST_FOCUSED_WIDGET ( QWidget * widget )
{
    if ( widget EQ NULL ) {
        m_last_focused_widget = NULL;
        return;
    }

    if ( widget->objectName() EQ "table_widget_child" ) {
        return;
    }
    if ( widget->parent() NE NULL ) {
        if ( widget->parent()->objectName() EQ "table_widget_child" ) {
            return;
        }
    }

    m_last_focused_widget = widget;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::FOCUS_LAST_FOCUSED_WIDGET
***************************************************************************************/

void BASE_EVENT_KERNEL::FOCUS_LAST_FOCUSED_WIDGET()
{
    if ( m_last_focused_widget EQ NULL ) {
        FOCUS_FIRST_WIDGET();
        return;
    }

    m_last_focused_widget->blockSignals(true);
    m_last_focused_widget->setFocus();
    m_last_focused_widget->blockSignals(false);

    m_current_widget = NULL;
    FOCUS_IN_EVENT ( m_last_focused_widget );

}

/**************************************************************************************
                   BASE_EVENT_KERNEL::WIDGET_DESTROYED
***************************************************************************************/

void BASE_EVENT_KERNEL::WIDGET_DESTROYED ( QObject * destroyed_widget )
{
    if ( m_last_focused_widget EQ (QWidget *)destroyed_widget ) {
         m_last_focused_widget = NULL;
         RESET_CHECK_VAR_VARIABLES();
    }
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::REGISTER_CHANGER_BUTTON
***************************************************************************************/

void BASE_EVENT_KERNEL::REGISTER_CHANGER_BUTTON ( QWidget * button_widget )
{
    QAbstractButton * button = KERNEL_FIND_BUTTON_WIDGET ( button_widget );

    if ( button EQ NULL ) {
        return;
    }
    disconnect(button,SIGNAL ( clicked() ),0,0 );
    button->setObjectName ( "changer_button" );
    connect ( button,SIGNAL ( clicked() ),this,SLOT ( SLOT_CHANGER_BUTTON_CLICKED() ) );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SLOT_CHANGER_BUTTON_CLICKED
***************************************************************************************/

void BASE_EVENT_KERNEL::SLOT_CHANGER_BUTTON_CLICKED()
{
    if ( sender() EQ NULL ) {
        return;
    }

    QAbstractButton * button = static_cast<QAbstractButton *> ( sender() );

    CLOSE_ALL_EVENTS();
    int return_value = CHANGER_BUTTON_CLICKED ( button );
    OPEN_ALL_EVENTS();

    if ( return_value EQ ADAK_RECORD_UNCHANGED ) {
        return;
    }

    INCREASE_DEGISEN_WIDGET_COUNT ( button );
    WIDGET_CHANGED();
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::CHANGER_BUTTON_CLICKED
***************************************************************************************/

int BASE_EVENT_KERNEL::CHANGER_BUTTON_CLICKED ( QAbstractButton * button )
{
    Q_UNUSED ( button );

    return ADAK_RECORD_UNCHANGED;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::KERNEL_FIND_BUTTON_WIDGET
***************************************************************************************/

QAbstractButton * BASE_EVENT_KERNEL::KERNEL_FIND_BUTTON_WIDGET ( QWidget * button_widget )
{
    QWidget * button = NULL;

    if (button_widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className()) {
        button = button_widget->findChild<QWidget *>("search_edit_tool_button");
    }
    if ( button_widget->metaObject()->className() EQ QPushButton::staticMetaObject.className()) {
         button = button_widget;
    }
    if ( button_widget->metaObject()->className() EQ QToolButton::staticMetaObject.className() ) {
         button = button_widget;
    }

    if  ( button EQ NULL ) {
          return NULL;
    }

    QAbstractButton * abstract_button = static_cast<QAbstractButton *> ( button );
    
    return abstract_button;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::OPEN_ALL_EVENTS
***************************************************************************************/

void BASE_EVENT_KERNEL::OPEN_ALL_EVENTS()
{
    QList<QDateEdit *> date_edits = findChildren<QDateEdit *>();

    foreach(QDateEdit * dates,date_edits) {
        dates->blockSignals(false);
    }

    if (m_is_all_events_open EQ true) {
        return;
    }

    OPEN_CHECK_VAR_CONTROL();
    ENABLE_CATCH_WIDGET_CHANGES();
    m_is_all_events_open = true;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::CLOSE_ALL_EVENTS
***************************************************************************************/

void BASE_EVENT_KERNEL::CLOSE_ALL_EVENTS()
{
    QList<QDateEdit *> date_edits = findChildren<QDateEdit *>();

    foreach ( QDateEdit * dates,date_edits ) {
        dates->blockSignals(true);
    }

    if (m_is_all_events_open EQ false) {
        return;
    }

    DISABLE_CATCH_WIDGET_CHANGES();
    CLOSE_CHECK_VAR_CONTROL();
    m_is_all_events_open = false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::ENABLE_CATCH_WIDGET_CHANGES
***************************************************************************************/

void BASE_EVENT_KERNEL::ENABLE_CATCH_WIDGET_CHANGES()
{
    m_widget_changed_active = true;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::DISABLE_CATCH_WIDGET_CHANGES
***************************************************************************************/

void BASE_EVENT_KERNEL::DISABLE_CATCH_WIDGET_CHANGES()
{
    m_widget_changed_active = false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::OPEN_CHECK_VAR_CONTROL
***************************************************************************************/

void BASE_EVENT_KERNEL::OPEN_CHECK_VAR_CONTROL()
{
    m_check_var_is_open = true;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::BASE_EVENT_KERNEL::CLOSE_CHECK_VAR_CONTROL
***************************************************************************************/

void BASE_EVENT_KERNEL::CLOSE_CHECK_VAR_CONTROL()
{
    m_check_var_is_open = false;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::WIDGET_ITEM_CHANGED
***************************************************************************************/

void BASE_EVENT_KERNEL::WIDGET_ITEM_CHANGED()
{
    QString temp_object_name = sender()->objectName();

    sender()->setObjectName ( "ITEM_CHANGED" );
    CHECK_VAR_SLOT_EVENT ( sender() );
    sender()->setObjectName ( temp_object_name );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::WIDGET_ITEM_DOUBLE_CLICKED
***************************************************************************************/

void BASE_EVENT_KERNEL::WIDGET_ITEM_DOUBLE_CLICKED ( int row, int column )
{
    Q_UNUSED ( row );
    Q_UNUSED ( column );

    QString temp_object_name = sender()->objectName();

    sender()->setObjectName ( "ITEM_DOUBLE_CLICKED" );
    CHECK_VAR_SLOT_EVENT ( sender() );
    sender()->setObjectName ( temp_object_name );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::WIDGET_ITEM_CLICKED
***************************************************************************************/

void BASE_EVENT_KERNEL::WIDGET_ITEM_CLICKED ( int row, int column )
{
    Q_UNUSED ( row );
    Q_UNUSED ( column );

    QString temp_object_name = sender()->objectName();

    sender()->setObjectName ( "ITEM_CLICKED" );
    CHECK_VAR_SLOT_EVENT(sender());
    sender()->setObjectName ( temp_object_name );
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::DISABLE_EVENTS_ON_WIDGET
***************************************************************************************/

void BASE_EVENT_KERNEL::DISABLE_CLEAR_ON_WIDGET( QObject * widget )
{
    m_disable_clear_widget_list << widget;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::CLEAR_ALL_WIDGETS
***************************************************************************************/

void BASE_EVENT_KERNEL::CLEAR_ALL_WIDGETS(QObject * parent)
{
    if (parent EQ NULL) {
        parent = this;
    }

    QList<QObject *> parentin_child_objeleri = parent->children();

    for ( int i=0 ; i < parentin_child_objeleri.count(); i++ ) {
        if ( m_disable_clear_widget_list.contains(parentin_child_objeleri.at(i)) EQ true ) {
             continue;
        }
        else if (  parentin_child_objeleri.at(i)->metaObject()->className() EQ QAdakDate::staticMetaObject.className() ) {
            QAdakDate * date_edit = static_cast<QAdakDate *>(parentin_child_objeleri.at(i));
            date_edit->SET_DATE(QDate::currentDate());
            continue;
        }
        if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {            
            QLineEdit * line_edit = static_cast<QLineEdit *> ( parentin_child_objeleri.at(i) );
            line_edit->clear();
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QCheckBox::staticMetaObject.className() ) {
            QCheckBox * check_box = static_cast<QCheckBox *>(parentin_child_objeleri.at(i) );
            check_box->setChecked(false);
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
            QComboBox * combo_box = static_cast<QComboBox *>(parentin_child_objeleri.at(i));
            combo_box->setCurrentIndex(-1);
        }
        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
            QTextEdit * text_edit = static_cast<QTextEdit *>(parentin_child_objeleri.at(i));
            text_edit->clear();
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
            QLimitedTextEdit * limited_text_edit = static_cast<QLimitedTextEdit *>(parentin_child_objeleri.at(i));
            limited_text_edit->clear();
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
            QPlainTextEdit * plain_text_edit = static_cast<QPlainTextEdit *>(parentin_child_objeleri.at(i));
            plain_text_edit->clear();
        }

        else if( parentin_child_objeleri.at(i)->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
            QCommaEdit * comma_edit = static_cast<QCommaEdit *>(parentin_child_objeleri.at(i));
            comma_edit->clear();
        }

        else if ( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTimeEdit::staticMetaObject.className() ) {
            QTimeEdit * time_edit = static_cast<QTimeEdit *>(parentin_child_objeleri.at(i));
            time_edit->setTime(QTime());
        }
        else if (  parentin_child_objeleri.at(i)->metaObject()->className() EQ QDateEdit::staticMetaObject.className() ) {
            QDateEdit * date_edit = static_cast<QDateEdit *>(parentin_child_objeleri.at(i));
            date_edit->setDate(QDate());
        }
        else if( parentin_child_objeleri.at(i)->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
            QSearchEdit * search_edit = static_cast<QSearchEdit *>(parentin_child_objeleri.at(i));
            search_edit->SET_TEXT("");
        }
        else if( parentin_child_objeleri.at(i)->metaObject()->className() EQ QTableWidget::staticMetaObject.className() ) {
            QTableWidget * table_widget = static_cast<QTableWidget *>(parentin_child_objeleri.at(i));
            table_widget->setRowCount(0);
        }

        QList<QObject *> childin_child_objeleri = parentin_child_objeleri.at(i)->children();

        if ( childin_child_objeleri.count() > 0 ) {
            CLEAR_ALL_WIDGETS ( parentin_child_objeleri.at(i) );
        }
    }
    return;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::WIDGET_TEXT_CHANGED
***************************************************************************************/

void BASE_EVENT_KERNEL::WIDGET_TEXT_CHANGED()
{
    QWidget * widget = static_cast<QWidget *>(sender());

    if (widget EQ NULL) {
        return;
    }

    if ( widget->hasFocus() EQ true ) {
        return;
    }

    if ( widget->metaObject()->className() EQ QLineEdit::staticMetaObject.className() ) {
        QLineEdit * line_edit = static_cast<QLineEdit *>(widget);
        line_edit->setCursorPosition(0);
    }
    else if ( widget->metaObject()->className() EQ QCommaEdit::staticMetaObject.className() ) {
        QCommaEdit * comma_edit = static_cast<QCommaEdit *>(widget);
        comma_edit->setCursorPosition(0);
    }
//    else if ( widget->metaObject()->className() EQ QTextEdit::staticMetaObject.className() ) {
//        QTextEdit * text_edit = static_cast<QTextEdit *>(widget);
//        text_edit->selectAll();
//    }
//    else if ( widget->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
//        QLimitedTextEdit * limited_text_edit = static_cast<QLimitedTextEdit *>(widget);
//        limited_text_edit->selectAll();
//    }
//    else if ( widget->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() ) {
//        QPlainTextEdit * plain_text_edit = static_cast<QPlainTextEdit *>(widget);
//        plain_text_edit->selectAll();
//    }
    else if( widget->metaObject()->className() EQ QSearchEdit::staticMetaObject.className() ) {
        QLineEdit * line_edit = static_cast<QSearchEdit *> ( widget )->findChild<QLineEdit *>("search_edit_line_edit");
        line_edit->setCursorPosition(0);
    }
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::ADD_ADAK_EVENT
***************************************************************************************/

void BASE_EVENT_KERNEL::ADD_ADAK_EVENT(int event_type)
{
    if ( m_adak_event_queue.size() > 0 ) {
        if ( m_adak_event_queue.last() EQ event_type ) {
            return;
        }
    }
    m_adak_event_queue << event_type;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::GET_ADAK_EVENT_QUEUE
***************************************************************************************/

QList<int> * BASE_EVENT_KERNEL::GET_ADAK_EVENT_QUEUE()
{
    return &m_adak_event_queue;
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::PROCESS_ADAK_EVENTS
***************************************************************************************/

void BASE_EVENT_KERNEL::PROCESS_ADAK_EVENTS()
{

}

/**************************************************************************************
                   BASE_EVENT_KERNEL::eventFilter
***************************************************************************************/

bool BASE_EVENT_KERNEL::eventFilter(QObject * event,  QEvent * obj)
{
    if (m_check_var_is_open EQ true) {
        if (m_next_focus_widget NE NULL) {
            QWidget * temp = m_next_focus_widget;
            m_next_focus_widget = NULL;
            temp->setFocus();
        }
    }

    return BASE_KERNEL::eventFilter(event,obj);
}

/**************************************************************************************
                   BASE_EVENT_KERNEL::SET_COMBOBOX_LENGTH
***************************************************************************************/

void BASE_EVENT_KERNEL::SET_COMBOBOX_LENGTH ( QComboBox * combobox , int length ) 
{
    combobox->setEditable (true);
    if ( combobox->lineEdit() NE NULL ) {
        combobox->lineEdit()->setMaxLength(length);
    }
}
