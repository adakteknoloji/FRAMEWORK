#include <QtGui>
#include <QHBoxLayout>
#include "adak_defines.h"
#include "QSearchEdit.h"

/****************************************************************/
/*               QSearchEdit::QSearchEdit                       */
/****************************************************************/

QSearchEdit::QSearchEdit(QWidget * parent) : QWidget(parent)
{
    h_box_layout = new QHBoxLayout(this);
    h_box_layout->setContentsMargins(0,0,0,0);
    h_box_layout->setSpacing(0);

    line_edit    = new QLineEdit(this);
    line_edit->setObjectName("search_edit_line_edit");
    line_edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    tool_button  = new QToolButton(this);
    tool_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    tool_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tool_button->setMaximumSize(50,line_edit->height() - 9 );
    tool_button->setIconSize(QSize(tool_button->width(),line_edit->height() - 9 ));
    tool_button->setIcon(QIcon(":/kernel_icons/kernel_ara_searchedit_ikonu.png"));
    tool_button->setObjectName("search_edit_tool_button");


    setLayout(h_box_layout);

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(line_edit);

    h_box_layout->addWidget(line_edit);
    h_box_layout->addWidget(tool_button);

    setMaximumHeight(line_edit->height());

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    qt_key = -1;

    SET_SHORTCUT(Qt::Key_F12);

    m_last_entered_value = "";

    line_edit_changed = false;

    line_edit->installEventFilter(parent);

    connect(tool_button, SIGNAL(clicked())            ,this ,SLOT(SLOT_BUTTON_CLICKED()));
    connect(line_edit  , SIGNAL(textEdited(QString))  ,this ,SLOT(SLOT_SET_LINE_EDIT_CHANGED_FLAG()));
    connect(line_edit  , SIGNAL(textChanged(QString)) ,this ,SLOT(SLOT_SET_LINE_EDIT_CHANGED_FLAG()));
    connect(line_edit  , SIGNAL(editingFinished())    ,this ,SLOT(SLOT_LINE_EDIT_CHANGED()));

    tool_button->setFocusPolicy(Qt::ClickFocus);

    setTabOrder(line_edit,tool_button);
}

/****************************************************************/
/*               QSearchEdit::~QSearchEdit                      */
/****************************************************************/

QSearchEdit::~QSearchEdit()
{

}

/****************************************************************/
/*               QSearchEdit::SET_BUTTON_ICON                   */
/****************************************************************/

void QSearchEdit::SET_BUTTON_ICON(QString icon_path)
{
    tool_button->setIcon(QIcon(icon_path));
}

/****************************************************************/
/*               QSearchEdit::SET_SHORTCUT                      */
/****************************************************************/

void QSearchEdit::SET_SHORTCUT(int qkey)
{
    qt_key = qkey;
}

/****************************************************************/
/*               QSearchEdit::SET_BUTTON_TEXT                   */
/****************************************************************/

void QSearchEdit::SET_BUTTON_TEXT(QString text)
{
    tool_button->setText(text);
}

/****************************************************************/
/*               QSearchEdit::SET_TEXT                          */
/****************************************************************/

void QSearchEdit::SET_TEXT(QString text)
{
    line_edit->blockSignals(true);
    line_edit->setText(text);
    m_last_entered_value = text;
    line_edit->blockSignals(false);
}


/****************************************************************/
/*               QSearchEdit::SET_MAX_LENGTH                    */
/****************************************************************/

void QSearchEdit::SET_MAX_LENGTH ( int length )
{
    line_edit->setMaxLength(length);
}

/****************************************************************/
/*               QSearchEdit::CLEAR                             */
/****************************************************************/

void QSearchEdit::CLEAR()
{
    SET_TEXT("");
}

/****************************************************************/
/*               QSearchEdit::GET_TEXT                          */
/****************************************************************/

QString QSearchEdit::GET_TEXT()
{
    return line_edit->text();
}

/****************************************************************/
/*               QSearchEdit::SET_BUTTON_SIZE                   */
/****************************************************************/

void QSearchEdit::SET_BUTTON_SIZE(int height,int width)
{
    tool_button->setMaximumSize(width,height);
    tool_button->setMinimumSize(width,height);
}

/****************************************************************/
/*               QSearchEdit::SLOT_BUTTON_CLICKED               */
/****************************************************************/

void QSearchEdit::SLOT_BUTTON_CLICKED()
{
    emit SIGNAL_BUTTON_CLICKED(this,line_edit,tool_button);
}

/****************************************************************/
/*               QSearchEdit::SLOT_BUTTON_CLICKED               */
/****************************************************************/

void QSearchEdit::SLOT_LINE_EDIT_CHANGED()
{
    if (tool_button->hasFocus() EQ true) {
        return;
    }

    if(line_edit_changed EQ false) {
        return;
    }

    if (m_last_entered_value EQ line_edit->text()) {
        return;
    }

    line_edit_changed = false;
    emit SIGNAL_LINE_EDIT_CHANGED(this,line_edit,tool_button);    
}

/****************************************************************/
/*               QSearchEdit::SLOT_SET_LINE_EDIT_CHANGED_FLAG   */
/****************************************************************/

void QSearchEdit::SLOT_SET_LINE_EDIT_CHANGED_FLAG()
{
    line_edit_changed = true;
}

/****************************************************************/
/*               QSearchEdit::keyPressEvent                     */
/****************************************************************/

void QSearchEdit::keyPressEvent(QKeyEvent * key_event)
{  
//    if(key_event->key() EQ Qt::Key_Enter OR key_event->key() EQ Qt::Key_Return) {
//        emit SIGNAL_BUTTON_CLICKED(this,line_edit,tool_button);
//    }

    if(qt_key EQ -1) {
        return;
    }

    if (key_event->key() EQ qt_key) {
        blockSignals(true);
        tool_button->setFocus();
        blockSignals(false);
        SLOT_BUTTON_CLICKED();
    }
}



