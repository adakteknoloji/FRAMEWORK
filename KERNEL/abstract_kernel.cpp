#include <QWidget>

#include "abstract_kernel.h"


/**************************************************************************************
                   ABSTRACT_KERNEL::ABSTRACT_KERNEL
***************************************************************************************/

ABSTRACT_KERNEL::ABSTRACT_KERNEL()
{
    m_parent = NULL;
}

/**************************************************************************************
                   ABSTRACT_KERNEL::~ABSTRACT_KERNEL
***************************************************************************************/

ABSTRACT_KERNEL::~ABSTRACT_KERNEL()
{
    m_parent = NULL;
}

/**************************************************************************************
                   ABSTRACT_KERNEL::SET_THIS
***************************************************************************************/

void ABSTRACT_KERNEL::SET_THIS(QWidget * parent)
{
    if (parent NE NULL) {
        m_parent = parent;
    }
}

/**************************************************************************************
                   BASE_KERNEL::SET_FOCUS
***************************************************************************************/

void ABSTRACT_KERNEL::SET_FOCUS(QWidget * focus_widget)
{
    if (focus_widget EQ NULL) {
        return;
    }

    if (IS_WIDGET_FOCUSABLE(focus_widget) EQ false ) {
        return;
    }

    focus_widget->setFocus();
}

/**************************************************************************************
                   ABSTRACT_KERNEL::IS_WIDGET_FOCUSABLE
***************************************************************************************/

bool ABSTRACT_KERNEL::IS_WIDGET_FOCUSABLE(QWidget * focus_widget)
{
    Q_UNUSED(focus_widget);

    return true;
}

/**************************************************************************************
                   ABSTRACT_KERNEL::MSG_WARNING
***************************************************************************************/

void ABSTRACT_KERNEL::MSG_WARNING (QString message,QObject * focus_widget)
{
    ADAK_WARNING(message,NULL,m_parent);

    if (focus_widget NE NULL) {
        SET_FOCUS(static_cast<QWidget *>(focus_widget));
    }
}

/**************************************************************************************
                   ABSTRACT_KERNEL::MSG_ERROR
***************************************************************************************/

void ABSTRACT_KERNEL::MSG_ERROR (QString message,QObject * focus_widget)
{
    ADAK_ERROR(message,NULL,m_parent);

    if (focus_widget NE NULL) {
        SET_FOCUS(static_cast<QWidget *>(focus_widget));
    }
}

/**************************************************************************************
                   ABSTRACT_KERNEL::MSG_INFO
***************************************************************************************/

void ABSTRACT_KERNEL::MSG_INFO (QString message,QObject * focus_widget)
{
    ADAK_INFO(message,NULL,m_parent);

    if (focus_widget NE NULL) {
        SET_FOCUS(static_cast<QWidget *>(focus_widget));
    }
}

/**************************************************************************************
                   ABSTRACT_KERNEL::MSG_YES_NO
***************************************************************************************/

ADAK_MSG_ENUM ABSTRACT_KERNEL::MSG_YES_NO (QString message,QObject * focus_widget)
{
    ADAK_MSG_ENUM return_value = ADAK_YES_NO(message,NULL,m_parent);

    if (focus_widget NE NULL) {
        SET_FOCUS(static_cast<QWidget *>(focus_widget));
    }

    return return_value;
}

/**************************************************************************************
                   ABSTRACT_KERNEL::MSG_YES_NO_CANCEL
***************************************************************************************/

ADAK_MSG_ENUM ABSTRACT_KERNEL::MSG_YES_NO_CANCEL (QString message,QObject * focus_widget)
{
    ADAK_MSG_ENUM return_value = ADAK_YES_NO_CANCEL(message,NULL,m_parent);

    if (focus_widget NE NULL) {
        SET_FOCUS(static_cast<QWidget *>(focus_widget));
    }

    return return_value;
}

