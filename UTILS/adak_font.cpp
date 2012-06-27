#include <QWidget>
#include <QFont>

/**************************************************************************************
                   ADAK_SET_FONT_SIZE
***************************************************************************************/

void ADAK_SET_FONT_SIZE (QWidget * widget, double oran)
{
    qreal font_size = widget->font().pointSizeF();
    font_size *= oran;
    QFont font (widget->font());

    font.setPointSizeF(font_size);
    widget->setFont (font);
}
