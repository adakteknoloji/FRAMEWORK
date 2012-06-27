#include <QApplication>
#include <QCursor>
#include <QPixmap>

static void CHANGE_CURSOR_IMAGE(int type = -1);

/**************************************************************************************
                   BASE_KERNEL::MAKE_CURSOR_BUSY
***************************************************************************************/

void ADAK_CURSOR_BUSY()
{
    CHANGE_CURSOR_IMAGE();
}

/**************************************************************************************
                   BASE_KERNEL::MAKE_CURSOR_NORMAL
***************************************************************************************/

void ADAK_CURSOR_NORMAL()
{
    QApplication::restoreOverrideCursor();
}

/**************************************************************************************
                   BASE_KERNEL::CHANGE_CURSOR_IMAGE
***************************************************************************************/

static void CHANGE_CURSOR_IMAGE(int type)
{
    Q_UNUSED(type); // Su anda sadece bir tane var gerekli degil

    QCursor * cursor = new QCursor(QPixmap(":/kernel_icons/kernel_wait_icon.png"), 5 , 5);
    QApplication::setOverrideCursor(*cursor);
}
