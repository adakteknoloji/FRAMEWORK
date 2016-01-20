#include <QtGui>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QLocale>
#include <QCalendarWidget>
#include "adak_utils.h"
#include "QAdakDate.h"

/****************************************************************
               QAdakDate::QAdakDate
****************************************************************/

QAdakDate::QAdakDate(QWidget * parent) : QWidget(parent)
{
    line_edit    = new QLineEdit(this);
    line_edit->setObjectName("adak_date_line_edit");
    line_edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line_edit->installEventFilter(this);
    line_edit->setMaxLength(100);
    connect(line_edit,SIGNAL(editingFinished()),this,SLOT(SLOT_SET_DATE()));

    tool_button  = new QToolButton(this);
    tool_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    tool_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tool_button->setIcon(QIcon(":/kernel_icons/kernel_calendar_ikonu.png"));
    tool_button->setObjectName("adak_date_tool_button");
    tool_button->setMaximumSize(32,line_edit->height() - 9);
    tool_button->setIconSize(QSize(tool_button->width(),line_edit->height() - 9));
    tool_button->setFocusPolicy(Qt::ClickFocus);
    connect(tool_button,SIGNAL(clicked()),this,SLOT(SLOT_POPUP_BUTTON_CLICKED()));

    calendar_widget = new QCalendarWidget(this);
    calendar_widget->setFirstDayOfWeek ( Qt::Monday );
    calendar_widget->setDateRange(MALI_YIL_FIRST_DATE(), MALI_ARRAY_LAST_DATE());
    calendar_widget->setWindowFlags(Qt::Popup);
    calendar_widget->close();
    connect(calendar_widget,SIGNAL(clicked(QDate)),this,SLOT(SLOT_POPUP_DATE_SELECTED(QDate)));

    h_box_layout = new QHBoxLayout(this);
    h_box_layout->setContentsMargins(0,0,0,0);
    h_box_layout->setSpacing(0);
    h_box_layout->addWidget(line_edit);
    h_box_layout->addWidget(tool_button);

    setLayout       (h_box_layout);
    setFocusPolicy  (Qt::StrongFocus);
    setFocusProxy   (line_edit);
    setMaximumHeight(line_edit->height());
    setTabOrder     (line_edit,tool_button);

    m_mali_yil_disable  = false;
    m_date_changed      = false;
    m_current_date      = QDate::currentDate();
    m_old_date          = QDate::currentDate();

    SET_DATE_FORMAT ("dd MMMM yyyy dddd");
    SET_DATE        (QDate::currentDate());
}

/****************************************************************
               QAdakDate::~QAdakDate
****************************************************************/

QAdakDate::~QAdakDate()
{

}

/****************************************************************
               QAdakDate::SET_DATE_FORMAT
****************************************************************/

void QAdakDate::SET_DATE_FORMAT(const QString& format)
{
    m_date_format = format;
    REFRESH_DATE();
}

/****************************************************************
               QAdakDate::SET_DATE
****************************************************************/

void QAdakDate::SET_DATE(const QDate& date)
{
    m_old_date  = m_current_date;
    m_current_date  = date;

    REFRESH_DATE();
}

/****************************************************************
               QAdakDate::SET_DATE
****************************************************************/

void QAdakDate::SET_DATE(const QString& date)
{
    SET_DATE ( QDate::fromString( date, "yyyy.MM.dd" ));
}

/****************************************************************
               QAdakDate::DATE
****************************************************************/

QString QAdakDate::DATE()
{
    return m_current_date.toString("yyyy.MM.dd");
}

/****************************************************************
               QAdakDate::QDATE
****************************************************************/

QDate QAdakDate::QDATE()
{
    return m_current_date;
}

/****************************************************************
               QAdakDate::REFRESH_DATE
****************************************************************/

void QAdakDate::REFRESH_DATE()
{
    m_date_str = QDATE().toString(m_date_format);

    if ( m_date_str.isEmpty() EQ true ) {
        if ( m_old_date.isValid() EQ true ) {
            SET_DATE( m_old_date );
        }
        else {
            if (m_mali_yil_disable EQ false) {
                SET_DATE( MALI_YIL_FIRST_DATE());
            }
            else {
                SET_DATE( QDate::currentDate() );
            }
        }
        return;
    }

    if (m_date_str EQ line_edit->text()) {
        return;
    }

    if ( m_mali_yil_disable EQ true) {
        line_edit->setText( m_date_str );
    }
    else { // false ise
        if ( m_current_date >= MALI_YIL_FIRST_DATE() AND m_current_date <= MALI_ARRAY_LAST_DATE() ) {
            line_edit->setText( m_date_str );
        }
        else {
            if( m_date_changed EQ true ) {
                QMessageBox::information( this, tr( "Warning!" ), tr("Your entered date is not in the financial year. Please enter a new date! "));//Girmiş olduğunuz tarih mali yıl içinde değil, lütfen yeni bir tarih girin.!
                m_date_changed = false;
            }
            if ( m_current_date < MALI_YIL_FIRST_DATE() ) {
                line_edit->setText( MALI_YIL_FIRST_DATE().toString( m_date_format ) );
            }
            else {
                line_edit->setText( MALI_ARRAY_LAST_DATE().toString( m_date_format ) );
            }
        }
    }

    if ( signalsBlocked() EQ false ) {
        emit SIGNAL_DATE_CHANGED();
    }
}

/****************************************************************
               QAdakDate::SLOT_SET_DATE
****************************************************************/

void QAdakDate::SLOT_SET_DATE()
{
    m_date_changed = true;
    TRY_TO_SET_DATE("");
}

/****************************************************************
               QAdakDate::TRY_TO_SET_DATE
****************************************************************/

void QAdakDate::TRY_TO_SET_DATE(const QString &date_str)
{
    Q_UNUSED(date_str);

    int   current_year;
    if (m_mali_yil_disable EQ true) {
        current_year  = QDate::currentDate().year();
    }
    else {
        current_year  = MALI_YIL();
    }
    int   current_month = QDate::currentDate().month();
    int   current_day   = QDate::currentDate().day();

    QDate current_date  = QDate::currentDate();

    if ( line_edit->text() EQ m_date_str ) {
        return;
    }
    // tarih degismedi ise bir sey yapma
    if ( line_edit->isModified() EQ false ) {
        return;
    }

    QString user_text = line_edit->text();

    if (user_text EQ "00") {
        SET_DATE(current_date);
    }

    if ( user_text.size() >= 2 ) {
        current_day   = user_text.mid(0,2).toInt();
    }
    if ( user_text.size() >= 4 ) {
        current_month = user_text.mid(2,2).toInt();
    }
    if ( user_text.size() > 4 ) {
        current_year = user_text.mid(4,4).toInt();
    }

    current_date.setYMD(current_year,current_month,current_day);

    SET_DATE( current_date );
}

/****************************************************************
               QAdakDate::SLOT_POPUP_BUTTON_CLICKED
****************************************************************/

void QAdakDate::SLOT_POPUP_BUTTON_CLICKED()
{
    int y = QCursor::pos().y() - calendar_widget->height();

    int x = QCursor::pos().x() - calendar_widget->width();

    if ( y < 0 ) {
        y = QCursor::pos().y() + 20;
    }

    if ( x < 0 ) {
        x = QCursor::pos().x() + 20;
    }

    calendar_widget->move( x , y );

    calendar_widget->setSelectedDate (QDATE());
    calendar_widget->show();
}

/****************************************************************
               QAdakDate::SLOT_POPUP_DATE_SELECTED
****************************************************************/

void QAdakDate::SLOT_POPUP_DATE_SELECTED(const QDate& date)
{
    calendar_widget->close();

    SET_DATE( date );
}

/****************************************************************
               QAdakDate::MALI_YIL_ARRAY_INDIS
****************************************************************/

int QAdakDate::MALI_YIL_ARRAY_INDIS()
{
    if (m_current_date < MALI_YIL_FIRST_DATE()) {
        return -1; // Will crash
    }
    if (m_current_date > MALI_ARRAY_LAST_DATE()) {
        return -1; // Will crash
    }

    return MALI_YIL_FIRST_DATE().daysTo ( m_current_date ) + 1;
}

/****************************************************************
               QAdakDate::MALI_YIL_KISITI_YOK
****************************************************************/

void QAdakDate::MALI_YIL_KISITI_YOK( bool p_disable )
{
    m_mali_yil_disable = p_disable;
    if (m_mali_yil_disable EQ false) {
        calendar_widget->setDateRange(MALI_YIL_FIRST_DATE(), MALI_ARRAY_LAST_DATE());
    }
    else {
        calendar_widget->setDateRange(QDate(1,1,1), QDate(9999,12,31));
    }
}

/****************************************************************
               QAdakDate::eventFilter
****************************************************************/

bool QAdakDate::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() EQ QEvent::MouseButtonPress) {
        if ( obj EQ line_edit AND line_edit->isEnabled() EQ true) {
            if (line_edit NE NULL ) {
                line_edit->setSelection(line_edit->text().size(),-line_edit->text().size());
                return true;
            }
        }
    }

    return false;
}

