#include <QtWidgets>
#include <QKeyEvent>
#include <QLabel>
#include "adak_defines.h"
#include "tasarim_design_widget.h"
#include "adak_utils.h"
#include "tasarim_degiskeni.h"
#include "belge_kernel_defines.h"
#include "belge_kernel_utils.h"

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::TASARIM_DESIGN_WIDGET                                    
***************************************************************************************/

TASARIM_DESIGN_WIDGET::TASARIM_DESIGN_WIDGET(double oran,QWidget * parent) : QWidget(parent)
{
    setAttribute( Qt::WA_DeleteOnClose );

    m_last_selected_variable = NULL;
    m_last_move_label          = NULL;

    m_oran = oran;

    setWindowTitle(tr("Adak Document Design"));
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::INIT_WIDGET
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::INIT_WIDGET ( QList< TASARIM_DEGISKENI *> p_variable, TASARIMCI_MODU p_modu )
{
    int x = 0;
    int y = 0;

    for (int i = 0 ; i < p_variable.size() ; i++ ){

        TASARIM_DEGISKENI *degisken = p_variable.at(i);

        QString label_txt = degisken->GET_TEXT();

        if (label_txt.isEmpty() EQ false) {

            if( p_modu EQ ONIZLEME_MODU ) {
                label_txt = degisken->GET_DISPLAY_TXT();
            }

            QLabel * variable_label = CREATE_NEW_LABEL(label_txt, this);

            QFont label_font = variable_label->font();

            label_font.setFamily(degisken->GET_FONT_FAMILY());

            if ( degisken->IS_MONOSPACE_MI() EQ true ) {
                label_font.setFamily("Monospace");
                degisken->SET_FONT_FAMILY("Monospace");
            }
            label_font.setPointSize (degisken->GET_FONT_SIZE());
            label_font.setBold      (degisken->IS_BOLD());
            label_font.setUnderline (degisken->IS_UNDER_LINE());
            label_font.setItalic    (degisken->IS_ITALIC());
            label_font.setStrikeOut (degisken->IS_STRIKEOUT());

            variable_label->setFont (label_font);

            variable_label->move( x*m_oran, y*m_oran );
            variable_label->show();
            variable_label->setAttribute  ( Qt::WA_DeleteOnClose );
            variable_label->setObjectName ( QString::number( p_variable.at( i )->GET_PRIMARY_ID()) + "\t" + QString::number(  p_variable.at(i)->GET_GRUP() ));

            degisken->SET_LABEL( variable_label );

            ADD_LABEL_LIST( variable_label );

            emit VARIABLE_CREATED( variable_label );

            x += variable_label->width() + 2;
            if ( x >= ( m_width * m_oran ) ) {
                x = 0;
                y += variable_label->height() + 2;
            }

            SET_LABEL_FONTS_UNSELECTED( variable_label );
        }
    }

    setAcceptDrops( true );
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_WIDGET_SIZE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_WIDGET_SIZE ( int width, int height )
{
    m_width     = width;
    m_height    = height;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::ADD_NEW_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::ADD_NEW_VARIABLE(int x, int y, QLabel *label)
{
    label->move( x*m_oran, y*m_oran );
    label->show();
    label->setAttribute( Qt::WA_DeleteOnClose );
    ADD_LABEL_LIST( label );
    emit VARIABLE_CREATED( label );
    x += label->width() + 2;
    if ( x >= (m_width * m_oran) ) {
        x = 0;
        y += label->height() + 2;
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::CREATE_NEW_LABEL                                 
***************************************************************************************/

QLabel * TASARIM_DESIGN_WIDGET::CREATE_NEW_LABEL ( const QString &text, QWidget * parent )
{
    if ( parent EQ NULL ) {
        parent = this;
    }

    QLabel * label = new QLabel(text.toUtf8(),parent);
    label->setMargin(0);
    label->setIndent(0);
    QFont label_font = label->font();
    label_font.setFixedPitch    ( false );
    label_font.setKerning       ( false );
    label_font.setLetterSpacing ( QFont::AbsoluteSpacing, 0.01 );
    label_font.setWordSpacing   ( 0.01 );
    label->setScaledContents    ( true );
    label->setFont              ( label_font );
    label->setFocusPolicy       ( Qt::StrongFocus );

    int width = label->width();
    label->setMinimumWidth( width + 5 );

    label->installEventFilter( this );

    SET_LABEL_FONTS_UNSELECTED( label );

    return label;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SELECT_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SELECT_VARIABLE ( QLabel * label )
{
    if (label EQ NULL) {
        return;
    }
    m_last_move_label = label;
    UNSELECT_VARIABLE();
    SET_LABEL_FONTS_SELECTED( label );
    m_last_selected_variable = label;
    emit VARIABLE_SELECTED( label );
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SELECT_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SELECT_VARIABLE ( int index )
{
    if ( index EQ -1 ) {
        return;
    }

    QLabel * label = m_labels.at( index );

    SELECT_VARIABLE ( label );
    SET_LAST_SELECTED_VARIABLE( label );
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::GET_LABEL
***************************************************************************************/

QLabel * TASARIM_DESIGN_WIDGET::GET_LABEL ( int index )
{
    if ( m_labels.size() <= index ) {
        return NULL;
    }

    if ( index EQ -1 ) {
        return NULL;
    }

    QLabel * label = m_labels.at(index);

    return label;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::UNSELECT_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::UNSELECT_VARIABLE()
{
    if ( m_last_selected_variable NE NULL ) {
         SET_LABEL_FONTS_UNSELECTED( m_last_selected_variable );
         m_last_selected_variable = NULL;
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_LAST_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_LAST_SELECTED_VARIABLE(QLabel * label)
{
    m_last_selected_variable = label;
    SET_LABEL_FONTS_SELECTED(m_last_selected_variable);
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_LABEL_FONTS_UNSELECTED
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_LABEL_FONTS_UNSELECTED(QLabel * label)
{
    label->setFrameShape ( QFrame::Panel );
    label->setFrameShadow( QFrame::Raised);

    QStringList object_name_list = label->objectName().split("\t");

    if ( object_name_list.size() > 1 ) {
        int grup = object_name_list.at(1).toInt();
        if ( grup EQ USER_TEXT OR grup EQ USER_IMG ) {
            label->setStyleSheet("color:rgb(128, 128, 0); background-color:rgba(255, 255, 255,0);");
            return;
        }
    }

    label->setStyleSheet("background-color:white;");
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_LABEL_FONTS_SELECTED
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_LABEL_FONTS_SELECTED( QLabel *label )
{
    label->setStyleSheet("background-color:red;");
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::HIDE_SELECTED_VARIABLE                           
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::HIDE_SELECTED_VARIABLE()
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    m_last_selected_variable->hide();
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SHOW_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SHOW_SELECTED_VARIABLE()
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    m_last_selected_variable->show();
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_TEXT_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_TEXT_SELECTED_VARIABLE(QString label_text)
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    m_last_selected_variable->setText(label_text);    
    m_last_selected_variable->adjustSize();
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_OBJNAME_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_OBJNAME_SELECTED_VARIABLE(QString obj_name)
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    m_last_selected_variable->setObjectName(obj_name);
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::MOVE_VARIABLE
***************************************************************************************/

QLabel * TASARIM_DESIGN_WIDGET::MOVE_VARIABLE(QLabel * label,double x, double y,bool block_pos_changed_signal)
{
    if (label EQ NULL) {
        return NULL;
    }

    label->move(x,y);

    if ( block_pos_changed_signal EQ false) {
        emit VARIABLE_POSITION_CHANGED( label->objectName().split("\t").at(0).toInt(), x, y);
    }

    return label;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::MOVE_SELECTED_VARIABLE                           
***************************************************************************************/

QLabel * TASARIM_DESIGN_WIDGET::MOVE_SELECTED_VARIABLE(double x, double y,bool block_pos_changed_signal)
{
    if (m_last_selected_variable EQ NULL) {
        return NULL;
    }

    m_last_selected_variable->move(x,y);

    if (block_pos_changed_signal EQ false) {
        emit VARIABLE_POSITION_CHANGED(m_last_selected_variable->objectName().split("\t").at(0).toInt(), x, y);
    }

    return m_last_selected_variable;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_SELECTED_FONT_SIZE                           
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_SELECTED_FONT_SIZE(int size_pt)
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    SET_FONT(m_last_selected_variable,size_pt);
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_SELECTED_FONT_SIZE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_ALIGN_SELECTED_VARIABLE(int align_enum, QLabel *label)
{
    if (m_last_selected_variable EQ NULL AND label EQ NULL) {
        return;
    }

    Qt::Alignment align;

    switch (align_enum) {
        case LEFT_ALIGN:
            align = Qt::AlignLeft;
            break;
        case RIGHT_ALIGN:
            align = Qt::AlignRight;
            break;
        case CENTER_ALIGN:
        default:
            align = Qt::AlignCenter;
            break;
    }
    if (label NE NULL) {
        label->setAlignment(align);
        return;
    }

    m_last_selected_variable->setAlignment(align);
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_SELECTED_STYLE_SHEET
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_SELECTED_STYLE_SHEET(QString style_sheet)
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    m_last_selected_variable->setStyleSheet(style_sheet);
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::DELETE_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::DELETE_SELECTED_VARIABLE()
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    REMOVE_LABEL_LIST(m_last_selected_variable);
    m_last_selected_variable->close();
    m_last_selected_variable = NULL;    
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::GET_SELECTED_LABEL
***************************************************************************************/

QLabel * TASARIM_DESIGN_WIDGET::GET_SELECTED_LABEL()
{
    if ( m_last_selected_variable EQ NULL ) {
        return NULL;
    }

    return m_last_selected_variable;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_SIZE_SELECTED_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_SIZE_SELECTED_VARIABLE(int width, int height)
{
    if (m_last_selected_variable EQ NULL) {
        return;
    }

    m_last_selected_variable->setMinimumSize(width,height);
    m_last_selected_variable->setMaximumSize(width,height);
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::ADD_LABEL_LIST
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::ADD_LABEL_LIST ( QLabel * label )
{
    for (int i = 0 ; i < m_labels.size() ; i++ ) {
        if (m_labels.at(i) NE NULL) {
            continue;
        }

        m_labels[i] = label;
        return;
    }

    m_labels << label;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::REMOVE_LABEL_LIST
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::REMOVE_LABEL_LIST ( QLabel * label )
{
    for (int i = 0 ; i < m_labels.size() ; i++ ) {
        if (m_labels.at(i) NE label) {
            continue;
        }

        m_labels[i] = NULL;
        return;
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_FONT
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::SET_FONT ( QLabel * label, int size_pt,bool oranla )
{
    double oran = 1.00;

    if (oranla EQ true) {
        oran = m_oran;
    }

    QFont font = label->font();
    font.setPointSize(size_pt * oran);
    label->setFont(font);

    label->adjustSize();

    QStringList label_ozellikleri = label->objectName().split("\t");

    if ( label_ozellikleri.size() > 1 ) {
        if (label_ozellikleri.at(1).toInt() EQ USER_IMG ) {
            label->setMinimumSize(label->width()*oran, label->height()*oran);
            label->setMaximumSize(label->width()*oran, label->height()*oran);
        }
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::SET_FONT
***************************************************************************************/

bool TASARIM_DESIGN_WIDGET::eventFilter(QObject *p_obj, QEvent *p_event)
{
    if ( p_event->type() EQ QEvent::KeyPress ) {
        QKeyEvent * key_event = static_cast < QKeyEvent * >( p_event );

        if ( m_last_selected_variable NE NULL ) {
            emit SIGNAL_KEY_DIRECTION_PRESS( (Qt::Key)key_event->key() );
        }

        return true;
    }

    return QWidget::eventFilter( p_obj, p_event );
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::dragEnterEvent
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::dragMoveEvent
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::dropEvent
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {

        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPoint offset;
        dataStream >> offset;

        m_last_move_label->move(event->pos() - offset);

        SELECT_VARIABLE( m_last_move_label );

        SET_LAST_SELECTED_VARIABLE( m_last_move_label );

        emit VARIABLE_POSITION_CHANGED( m_last_move_label->objectName().split("\t").at(0).toInt(), m_last_move_label->pos().x(), m_last_move_label->pos().y() );

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    }
    else {
        event->ignore();
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::mousePressEvent
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child ) {
        // LABEL HARIC YER TIKLANDI ISE SECILI DEGISKEN SERBEST BIRAKILMALI
        RESET_VARIABLE();
        emit VARIABLE_UNSELECTED();
        return;
    }

    SELECT_VARIABLE( child );

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << QPoint(event->pos() - child->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - child->pos());

    m_last_move_label = child;

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        // move
    }
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::GET_LAST_MOVE_LABEL
***************************************************************************************/

QLabel* TASARIM_DESIGN_WIDGET::GET_LAST_MOVE_LABEL()
{
   return m_last_move_label;
}

/**************************************************************************************
                   TASARIM_DESIGN_WIDGET::RESET_VARIABLE
***************************************************************************************/

void TASARIM_DESIGN_WIDGET::RESET_VARIABLE()
{
    m_last_move_label = NULL;
    UNSELECT_VARIABLE();
}
