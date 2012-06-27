#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QMdiSubWindow>
#include <QAction>
#include <QTextDocument>
#include <QPainter>
#include "QSearchEdit.h"
#include "kernel_utils.h"
#include "form_kernel.h"
#include "adak_defines.h"

// bu enum da her bir // spacer anlamina gelir
enum {
    BUTTON_BUL            = 0,
    //1
    BUTTON_GETIR          = 2,
    //3
    BUTTON_YENILE         = 4,
    //5
    //6
    //7
    BUTTON_ILK_KAYIT      = 8,
    BUTTON_ONCEKI_KAYIT   = 9,
    //10
    //11
    BUTTON_SONRAKI_KAYIT  = 12,
    BUTTON_SON_KAYIT      = 13,
    //14
    //15
    BUTTON_YENI_KAYIT     = 16,
    //17
    BUTTON_KAYDI_SIL      = 18,
    //19
    //20
    BUTTON_YARDIM         = 21,
    //22
    BUTTON_KAYDET         = 23
};

/**************************************************************************************
                   FORM_KERNEL::FORM_KERNEL
***************************************************************************************/

FORM_KERNEL::FORM_KERNEL ( QWidget * parent ) : BASE_EVENT_KERNEL ( parent )
{
    m_kernel_button_text            = "";

    m_is_readonly_mode_on           = false;
    p_widget_changed                = false;
    close_event_started             = false;
    p_event_started                 = false;
    is_fis_kernel                   = false;
    p_table_is_filling              = false;
    m_widget_changed_active         = true;
    m_register_button_widget_called = false;
    p_current_record_id             = 0;
    p_previous_record_id            = 0;
    m_degisen_widget_counter        = 0;
    p_lock_status                   = LOCK_NOT_REQUESTED;
    SET_RECORD_STATUS(EMPTY);

    SET_RECORD_MODE(MULTI_RECORD_MODE);
    setModal(true);
}

/**************************************************************************************
                   FORM_KERNEL::~FORM_KERNEL
***************************************************************************************/

FORM_KERNEL::~FORM_KERNEL ()
{

}

/**************************************************************************************
                   FORM_KERNEL::START_FORM_KERNEL
***************************************************************************************/

void FORM_KERNEL::START_FORM_KERNEL ( QObject *parent, ADAK_SQL *database)
{
    SET_ADAK_WINDOW_DEFAULTS   ( parent     );
    SET_DB_CONN                 ( database   );
//    if (is_fis_kernel EQ true) { // FIXME
//        SET_EVENTS_AND_SIGNALS      ( parent,ADAK_FIS_KERNEL    );
//    }
//    else {
//        SET_EVENTS_AND_SIGNALS      ( parent,ADAK_FORM_KERNEL    );
//    }

    CLOSE_ALL_EVENTS    ();
    SETUP_FORM          ();
    OPEN_ALL_EVENTS     ();

    if (is_fis_kernel EQ true) { // FIXME
        SET_EVENTS_AND_SIGNALS      ( parent,ADAK_FIS_KERNEL    );
    }
    else {
        SET_EVENTS_AND_SIGNALS      ( parent,ADAK_FORM_KERNEL    );
    }

    INIT_KERNEL         ();
}

/**************************************************************************************
                   FORM_KERNEL::INIT_KERNEL
***************************************************************************************/

void FORM_KERNEL::INIT_KERNEL ()
{
    if ( p_record_mode EQ SINGLE_RECORD_MODE OR p_current_record_id > 0) {
        if ( FILL_FORM(p_current_record_id) EQ false ) {
            SET_RECORD_STATUS(EMPTY);
            UPDATE_BUTTON_STATUS();
            return;
        }
        SET_RECORD_STATUS(SAVED);
        UPDATE_BUTTON_STATUS();
    }
    else {
        SET_RECORD_STATUS(EMPTY);
        YENI_KAYIT();
    }
}

/**************************************************************************************
                   FORM_KERNEL::SET_SINGLE_RECORD_ID
***************************************************************************************/

void FORM_KERNEL::SET_SINGLE_RECORD_ID ( int single_record_id )
{
    SET_CURRENT_RECORD_ID ( single_record_id );
    SET_RECORD_MODE       ( SINGLE_RECORD_MODE );
}

/**************************************************************************************
                   FORM_KERNEL::SET_RECORD_MODE
***************************************************************************************/

void FORM_KERNEL::SET_RECORD_MODE ( RECORD_MODE record_mode )
{
    p_record_mode      = record_mode;
}

/**************************************************************************************
                   FORM_KERNEL::AUTOSTART_FORM_KERNEL
***************************************************************************************/

void FORM_KERNEL::AUTOSTART_FORM_KERNEL ( int record_id )
{
    if ( record_id <= 0 ) {
        return;
    }

    if (p_record_mode EQ SINGLE_RECORD_MODE) {
        qDebug("******UYARI: SINGLE RECORD MODE AKTIFKEN AUTOSTART KULLANILAMAZ.*******");
        return;
    }

    SET_CURRENT_RECORD_ID(record_id);
}


/**************************************************************************************
                   FORM_KERNEL::REGISTER_BUTTONS_WIDGET
***************************************************************************************/

void FORM_KERNEL::REGISTER_BUTTONS_WIDGET (QWidget * kernel_buttons_widget,bool remove_button_text, bool readonly_mode )
{
    QHBoxLayout * kernel_buttons_widget_layout     = new QHBoxLayout ( kernel_buttons_widget );

    kernel_buttons_widget_layout->setSpacing(2);
    kernel_buttons_widget_layout->setMargin (2);

    kernel_buttons_widget->setObjectName    ( "kernel_widget"                               );
    kernel_buttons_widget->setLayout        ( kernel_buttons_widget_layout                  );
    kernel_buttons_widget->setSizePolicy    ( QSizePolicy::Ignored , QSizePolicy::Fixed     );

    kernel_buttons_widget->setMinimumHeight(KERNEL_BUTTON_MINIMUN_HEIGHT + 10);

    //Button textlerin boyutu
    int button_text_size = 8;
    // methodun cagrilip cagrilmadigini tutar
    m_register_button_widget_called    = true;
    m_is_readonly_mode_on               = readonly_mode;

    for ( int i = 0; i <  MULTI_RECORD_MODE_BUTTON_COUNT; i++ ) {
        switch ( i ) {
            case BUTTON_KAYDI_SIL:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Kaydı Sil"),":/kernel_icons/kernel_sil_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
                FORM_BUTTONS [ i ] ->setToolTip("Ctrl + Del/Delete");
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_KAYDI_SIL_CLICKED()));
                break;
            case BUTTON_KAYDET:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Kaydet"),":/kernel_icons/kernel_kaydet_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setEnabled(true);
                FORM_BUTTONS [ i ] ->setToolTip("Ctrl + S");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence("Ctrl+S"));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_KAYDET_CLICKED()));
                break;
            case BUTTON_YENI_KAYIT:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Yeni Kayıt"),":/kernel_icons/kernel_yeni_kayit_ac_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("F8");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_F8));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_YENI_KAYIT_CLICKED()));
                break;
            case BUTTON_YENILE  :
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Yenile"),":/kernel_icons/kernel_yenile_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("F5");
                FORM_BUTTONS [ i ] ->setEnabled(true);
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_F5));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_YENILE_CLICKED()));
                break;
            case BUTTON_GETIR:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Getir"),":/kernel_icons/kernel_getir_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("F4");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_F4));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_GETIR_CLICKED()));
                break;
            case BUTTON_BUL:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Ara / Bul"),":/kernel_icons/kernel_ara_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("F9");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_F9));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_BUL_CLICKED()));
                break;
            case BUTTON_SONRAKI_KAYIT:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Sonraki"),":/kernel_icons/kernel_sonraki_kayit_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("Page Down");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_PageDown));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_SONRAKI_KAYIT_CLICKED()));
                break;
            case BUTTON_SON_KAYIT:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Son Kayıt"),":/kernel_icons/kernel_son_kayit_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("Ctrl + Page Down");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageDown));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_SON_KAYIT_CLICKED()));
                break;
            case BUTTON_ONCEKI_KAYIT:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Önceki"),":/kernel_icons/kernel_onceki_kayit_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("Page Up");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_PageUp));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_ONCEKI_KAYIT_CLICKED()));
                break;
            case BUTTON_ILK_KAYIT:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("İlk Kayıt"),":/kernel_icons/kernel_ilk_kayit_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("Ctrl + Page Up");
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageUp));
                connect(FORM_BUTTONS [ i ], SIGNAL(clicked()),   this, SLOT(SLOT_BUTTON_ILK_KAYIT_CLICKED()));
                break;
            case BUTTON_YARDIM:
                FORM_BUTTONS [ i ] = CREATE_KERNEL_BUTTON(tr("Yardım"),":/kernel_icons/kernel_yardim_ikonu.png",kernel_buttons_widget,remove_button_text);
                FORM_BUTTONS [ i ] ->setToolTip("F1");
                FORM_BUTTONS [ i ] ->setEnabled(true);
                FORM_BUTTONS [ i ] ->setShortcut(QKeySequence(Qt::Key_F1));
                connect ( FORM_BUTTONS [ i ], SIGNAL ( clicked() ),   this, SLOT ( YARDIM_BUTTON_CLICKED()));
                break;
            default:
                FORM_BUTTONS [ i ] = NULL;
                QSpacerItem * spacer_item = new QSpacerItem ( 20,40,QSizePolicy::Expanding,QSizePolicy::Fixed);
                kernel_buttons_widget_layout->addSpacerItem ( spacer_item);
                break;
        }

        if ( FORM_BUTTONS[i] NE NULL ) {
            QFont font = FORM_BUTTONS [ i ]->font();
            if ( font.pointSize() EQ -1 ) {
                font.setPixelSize(14);
            }
            else {
                font.setPixelSize(button_text_size + 4);
            }

            FORM_BUTTONS [ i ]->setFont(font);
            FORM_BUTTONS [ i ]->installEventFilter(this);
            kernel_buttons_widget_layout->addWidget( FORM_BUTTONS [ i ] );
        }
    }

    // SETS READONLY MODE IF ON
    SET_READONLY_MODE();
}

/**************************************************************************************
                    FORM_KERNEL::CREATE_KERNEL_BUTTON
***************************************************************************************/

QPushButton * FORM_KERNEL::CREATE_KERNEL_BUTTON ( QString button_text,QString icon_path,QWidget * parent,bool remove_button_text)
{
    QPushButton * button = new QPushButton(parent);
    button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding);
    button->setFocusPolicy( Qt::ClickFocus );

    if ( remove_button_text EQ false ) {
        button -> setText(button_text);
        button -> setMinimumHeight(KERNEL_BUTTON_MINIMUN_HEIGHT);
        if ( button->maximumHeight() < KERNEL_BUTTON_MINIMUN_HEIGHT ) {
            button->setMaximumHeight(KERNEL_BUTTON_MAXIMUN_HEIGHT);
        }
    }
    else {
        button->setText("");
        button -> setMaximumSize(KERNEL_BUTTON_MAXIMUN_HEIGHT,KERNEL_BUTTON_MAXIMUN_HEIGHT);
        button -> setMinimumSize(KERNEL_BUTTON_MINIMUN_HEIGHT,KERNEL_BUTTON_MINIMUN_HEIGHT);
    }

    button -> setIcon(QIcon(icon_path));
    button -> setIconSize(QSize(24,24));

    if ( p_record_mode EQ SINGLE_RECORD_MODE )  {
        button->setEnabled ( false );
    }

    return button;
}

/**************************************************************************************
                   FORM_KERNEL::SET_READONLY_MODE
***************************************************************************************/

void FORM_KERNEL::SET_READONLY_MODE()
{
    if ( m_is_readonly_mode_on EQ true ) {
        for ( int i = 0 ; i < MULTI_RECORD_MODE_BUTTON_COUNT ; i++ ) {
            if (    FORM_BUTTONS [i] EQ NULL                 OR
                    i                EQ BUTTON_SONRAKI_KAYIT OR
                    i                EQ BUTTON_SON_KAYIT     OR
                    i                EQ BUTTON_ONCEKI_KAYIT  OR
                    i                EQ BUTTON_ILK_KAYIT     OR
                    i                EQ BUTTON_YENILE        OR
                    i                EQ BUTTON_YARDIM        OR
                    i                EQ BUTTON_BUL
               )
            {
                continue;
            }
            FORM_BUTTONS[i]->setDisabled(true);
        }
    }
}

/**************************************************************************************
                   FORM_KERNEL::YENI_KAYIT
***************************************************************************************/

void FORM_KERNEL::YENI_KAYIT()
{
    m_degisen_widget_counter = 0;
    RESET_CHECK_VAR_VARIABLES();
    if ( p_record_status EQ SAVED OR p_record_status EQ NEED_UPDATE ) {
         TRY_UNLOCK_RECORD();
    }
    SET_CURRENT_RECORD_ID(0);
    SET_RECORD_STATUS(EMPTY);

    CLOSE_ALL_EVENTS();

    CLEAR_ALL_WIDGETS();
    CLEAR_FORM_MEMBERS();

    if ( is_fis_kernel EQ true ) {
        OPEN_ALL_EVENTS();
    }

    NEW_RECORD();

    if ( is_fis_kernel EQ false ) {
         OPEN_ALL_EVENTS();
    }

    SET_RECORD_STATUS(EMPTY);
    UPDATE_BUTTON_STATUS();
    FOCUS_FIRST_WIDGET();
}

/**************************************************************************************
                   FORM_KERNEL::FILL_FORM
***************************************************************************************/

bool FORM_KERNEL::FILL_FORM ( int record_id )
{
    CLOSE_ALL_EVENTS();

    m_degisen_widget_counter = 0;
    RESET_CHECK_VAR_VARIABLES();
    p_table_is_filling = true;
    CLEAR_ALL_WIDGETS();
    CLEAR_FORM_MEMBERS();

    if ( is_fis_kernel EQ true ) {
        OPEN_ALL_EVENTS();
    }

    if ( GET_RECORD(record_id) EQ ADAK_OK) {
        if ( p_record_status EQ SAVED OR p_record_status EQ NEED_UPDATE) {
             TRY_UNLOCK_RECORD(p_previous_record_id);
        }

        p_table_is_filling = false;
        FOCUS_LAST_FOCUSED_WIDGET();
        if ( is_fis_kernel EQ false) {
             OPEN_ALL_EVENTS();
        }
        return true;
    }
    p_table_is_filling = false;
    FOCUS_LAST_FOCUSED_WIDGET();

    if ( is_fis_kernel EQ false) {
         OPEN_ALL_EVENTS();
    }
    return false;
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_BUL_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_BUL_CLICKED ()
{
    if ( CHECK_VAR_CONTROL() EQ false ) {
        return;
    }

    int temp_record_id = SELECT_RECORD();

    if ( temp_record_id < 0 ) {
        return;
    }

    int kullanici_secimi = UNSAVED_RECORD_WARNING();
    if ( kullanici_secimi EQ KERNEL_CANCEL ) {
        return;
    }

    SET_CURRENT_RECORD_ID(temp_record_id);

    if ( FILL_FORM(p_current_record_id) EQ false ) {
        return;
    }
    p_lock_status = LOCK_NOT_REQUESTED;
    SET_RECORD_STATUS(SAVED);
    UPDATE_BUTTON_STATUS();
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_YENI_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_YENI_KAYIT_CLICKED ()
{
    if ( CHECK_VAR_CONTROL() EQ false ) {
        return;
    }

    int kullanici_secimi = UNSAVED_RECORD_WARNING();
    if ( kullanici_secimi EQ KERNEL_CANCEL ) {
        return;
    }

    YENI_KAYIT();
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_KAYDI_SIL_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_KAYDI_SIL_CLICKED ()
{
    if ( TRY_LOCK_RECORD() EQ false ) {
        return;
    }

    //if ( CHECK_VAR_CONTROL() EQ false ) {
    //    return;
    //}

    ADAK_MSG_ENUM answer = MSG_YES_NO("Kayıt Silinecektir. Devam etmek istermisiniz?" , NULL);

    if ( answer NE ADAK_YES ) {
        return;
    }


    if ( CHECK_DELETE ( p_current_record_id ) EQ ADAK_FAIL ) {
        return;
    }

    if ( is_fis_kernel EQ false) {
         ADAK_CURSOR_BUSY();
    }
    START_KERNEL_TRANSACTION();
    DELETE_RECORD ( p_current_record_id );
    if ( is_fis_kernel EQ false) {
         ADAK_CURSOR_NORMAL();
    }
    COMMIT_KERNEL_TRANSACTION();
    YENI_KAYIT();
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_KAYDET_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_KAYDET_CLICKED ()
{
    if ( p_record_status EQ RECORD_EMPTY ) {
        return;
    }

    int save_return_value = SAVE_RECORD();

    if ( save_return_value EQ RECORD_CANT_SAVED OR save_return_value EQ RECORD_EMPTY ) {
        return;
    }

    if ( is_fis_kernel EQ true ) {
        PROCESS_ADAK_EVENTS();
        return;
    }

    int return_value = LEAVE_RECORD();

    if ( return_value EQ ADAK_FAIL OR return_value EQ ADAK_CONTINUE ) {
         return;
    }

    if ( p_record_mode NE SINGLE_RECORD_MODE )  {
        YENI_KAYIT();
    }
    else if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
        ADAK_CLOSE();
    }
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_YENILE_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_YENILE_CLICKED()
{
    REFRESH_FORM_RECORD();
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_ILK_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_ILK_KAYIT_CLICKED()
{
    READONLY_BUTTONS_CLICKED ( BUTTON_ILK_KAYIT );
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_SON_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_SON_KAYIT_CLICKED()
{
    READONLY_BUTTONS_CLICKED ( BUTTON_SON_KAYIT );
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_ONCEKI_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_ONCEKI_KAYIT_CLICKED()
{
    READONLY_BUTTONS_CLICKED ( BUTTON_ONCEKI_KAYIT );
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_SONRAKI_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_SONRAKI_KAYIT_CLICKED()
{
    READONLY_BUTTONS_CLICKED ( BUTTON_SONRAKI_KAYIT );
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_GETIR_CLICKED
***************************************************************************************/

void FORM_KERNEL::BUTTON_GETIR_CLICKED()
{
    int temp_record_id = FIND_RECORD();

    if ( temp_record_id <= 0 ) {
        return;
    }

    SET_CURRENT_RECORD_ID(temp_record_id);

    if ( FILL_FORM(p_current_record_id) EQ false ) {
        return;
    }
    p_lock_status = LOCK_NOT_REQUESTED;
    SET_RECORD_STATUS(SAVED);
    UPDATE_BUTTON_STATUS();
}

/**************************************************************************************
                   FORM_KERNEL::SAVE_RECORD
***************************************************************************************/

int FORM_KERNEL::SAVE_RECORD()
{
    if ( TRY_LOCK_RECORD() EQ false ) {
         return RECORD_CANT_SAVED;
    }

    if ( CHECK_VAR_CONTROL() EQ false ) {
        return RECORD_CANT_SAVED;
    }

    if ( CHECK_EMPTY() EQ ADAK_FAIL ) {
        return RECORD_CANT_SAVED;
    }

    if ( is_fis_kernel EQ false) {
         CLOSE_ALL_EVENTS();
    }

    switch ( p_record_status ) {
        case NEED_ADD           :
            {
                if ( CHECK_ADD() EQ ADAK_FAIL) {
                    if (is_fis_kernel EQ false ) {
                        OPEN_ALL_EVENTS();
                    }
                    return RECORD_CANT_SAVED;
                }
                if ( is_fis_kernel EQ false) {
                     ADAK_CURSOR_BUSY();
                }
                START_KERNEL_TRANSACTION();
                int temp_record_id = ADD_RECORD();
                if ( is_fis_kernel EQ false) {
                    SET_RECORD_STATUS(SAVED);
                    ADAK_CURSOR_NORMAL();
                }
                SET_CURRENT_RECORD_ID ( temp_record_id );
            }
            break;
        case NEED_UPDATE        :
            if ( CHECK_UPDATE ( p_current_record_id ) EQ ADAK_FAIL ) {
                if (is_fis_kernel EQ false) {
                    OPEN_ALL_EVENTS();
                }
                return RECORD_CANT_SAVED;
            }
            if ( is_fis_kernel EQ false) {
                 ADAK_CURSOR_BUSY();
            }
            START_KERNEL_TRANSACTION();
            UPDATE_RECORD(p_current_record_id);            
            if ( is_fis_kernel EQ false) {
                SET_RECORD_STATUS(SAVED);
                ADAK_CURSOR_NORMAL();
            }
            //TRY_UNLOCK_RECORD();
            break;
        default:
            if ( is_fis_kernel EQ false) {
                OPEN_ALL_EVENTS();
            }
            if ( p_record_status EQ EMPTY) {
                return RECORD_EMPTY;
            }
            else if ( p_record_status EQ SAVED) {
                return RECORD_ALREADY_SAVED;
            }

            return RECORD_CANT_SAVED;
    }
    COMMIT_KERNEL_TRANSACTION();
    if ( is_fis_kernel EQ false ) {
        OPEN_ALL_EVENTS();
    }
    UPDATE_BUTTON_STATUS();
    m_degisen_widget_counter = 0;
    return RECORD_SAVED;
}

/**************************************************************************************
                   FORM_KERNEL::REFRESH_FORM_RECORD
***************************************************************************************/

void FORM_KERNEL::REFRESH_FORM_RECORD(int record_id)
{
    if ( record_id > 0) {
         YENI_KAYIT();
         SET_CURRENT_RECORD_ID(record_id);
    }
    else {
        if (p_record_status EQ EMPTY OR p_record_status EQ NEED_ADD OR p_record_status EQ CANT_LOCKED) {
            return;
        }
        SET_CURRENT_RECORD_ID(p_current_record_id);
    }

    if ( FILL_FORM(p_current_record_id) EQ false ) {
        return;
    }
    p_lock_status = LOCK_NOT_REQUESTED;
    SET_RECORD_STATUS(SAVED);
    UPDATE_BUTTON_STATUS();
}

/**************************************************************************************
                   FORM_KERNEL::WIDGET_CHANGED
***************************************************************************************/

void FORM_KERNEL::WIDGET_CHANGED()
{
    if ( m_widget_changed_active EQ false) {
        return;
    }

    if ( m_degisen_widget_counter EQ 0) {
        if( IS_WIDGET_CHANGED() EQ false ) {
            RESET_WIDGET_CHANGED();
            return;
        }
    }

    switch ( p_record_status ) {
        case EMPTY:
            SET_RECORD_STATUS(NEED_ADD);
            break;
        case SAVED:
            SET_RECORD_STATUS(NEED_UPDATE);
            break;
        default:
            return;
    }

    UPDATE_BUTTON_STATUS();
    BASE_EVENT_KERNEL::WIDGET_CHANGED();
}

/**************************************************************************************
                   FORM_KERNEL::SET_RECORD_STATUS
***************************************************************************************/

void FORM_KERNEL::SET_RECORD_STATUS(int record_status)
{
    if (m_is_readonly_mode_on EQ true) {
        return;
    }

    p_record_status = record_status;
}

/**************************************************************************************
                   FORM_KERNEL::RESET_WIDGET_CHANGED
***************************************************************************************/

void FORM_KERNEL::RESET_WIDGET_CHANGED()
{
    switch ( p_record_status ) {
        case NEED_ADD:
            SET_RECORD_STATUS(EMPTY);
            break;
        case NEED_UPDATE:
            SET_RECORD_STATUS(SAVED);
            break;
        default:
            return;
    }

    UPDATE_BUTTON_STATUS();
}

/**************************************************************************************
                   FORM_KERNEL::UNSAVED_RECORD_WARNING
***************************************************************************************/

int FORM_KERNEL::UNSAVED_RECORD_WARNING()
{
    if (m_is_readonly_mode_on EQ true) {
        return KERNEL_NO;
    }
    if ( p_lock_status EQ CANT_LOCKED OR p_lock_status EQ LOCK_NOT_REQUESTED ) {
        if ( p_record_status EQ SAVED OR p_record_status EQ NEED_UPDATE OR p_record_status EQ CANT_LOCKED ) {
             return KERNEL_NO;
        }
    }
    if ( p_record_status EQ SAVED OR p_record_status EQ EMPTY ) {
        int return_value = LEAVE_RECORD();
        if ( return_value EQ ADAK_FAIL OR return_value EQ ADAK_CONTINUE) {
             return KERNEL_CANCEL;
        }
        return KERNEL_NO;
    }

    ADAK_MSG_ENUM answer = MSG_YES_NO_CANCEL(tr ( "Kayıtta değişiklikler yapıldı.Kayıt etmek istiyor musunuz?"),NULL);

    switch ( answer ) {
        case ADAK_YES :
            {
                int save_return_value = SAVE_RECORD();

                if ( save_return_value EQ RECORD_CANT_SAVED) {
                    return KERNEL_CANCEL;
                }

                int return_value = LEAVE_RECORD();

                if ( return_value EQ ADAK_FAIL OR return_value EQ ADAK_CONTINUE) {
                     return KERNEL_CANCEL;
                }

                return KERNEL_YES;
            }
            break;
        case ADAK_NO:
            return KERNEL_NO;
            break;
        default:
            break;
    }

    return KERNEL_CANCEL;
}

/**************************************************************************************
                   FORM_KERNEL::READONLY_BUTTONS_CLICKED
***************************************************************************************/

void FORM_KERNEL::READONLY_BUTTONS_CLICKED ( int button_enum )
{
    if ( CHECK_VAR_CONTROL() EQ false ) {
        return;
    }

    int kullanici_secimi = UNSAVED_RECORD_WARNING();
    if ( kullanici_secimi EQ KERNEL_CANCEL ) {
        return;
    }

    int temp_record_id;

    switch ( button_enum ) {
        case BUTTON_ONCEKI_KAYIT:
            temp_record_id = FIND_PREV_RECORD();
            break;
        case BUTTON_SONRAKI_KAYIT:
            temp_record_id = FIND_NEXT_RECORD();
            break;
        case BUTTON_ILK_KAYIT:
            temp_record_id = FIND_FIRST_RECORD();
            break;
        case BUTTON_SON_KAYIT:
            temp_record_id = FIND_LAST_RECORD();
            break;
        default:
            return;
    }

    if (temp_record_id <= 0) {
        FOCUS_FIRST_WIDGET();
        return;
    }

    SET_CURRENT_RECORD_ID(temp_record_id);

    if ( FILL_FORM(p_current_record_id) EQ false ) {
        return;
    }

    p_lock_status = LOCK_NOT_REQUESTED;
    SET_RECORD_STATUS(SAVED);
    UPDATE_BUTTON_STATUS();
}

/**************************************************************************************
                   FORM_KERNEL::UPDATE_BUTTON_STATUS
***************************************************************************************/

void FORM_KERNEL::UPDATE_BUTTON_STATUS()
{
    if ( m_register_button_widget_called EQ false ) {
        MSG_WARNING( QObject::tr( "Navigator için Widget Setlenmedi" ), NULL );
        qDebug( "not call REGISTER_BUTTONS_WIDGET Method" );
        exit(0);
    }
    if ( m_is_readonly_mode_on EQ true ) {
        return;
    }

    switch (p_record_status) {
        case SAVED:
            FORM_BUTTONS [ BUTTON_KAYDET   ]        -> setEnabled( false );
            FORM_BUTTONS [ BUTTON_YENILE   ]        -> setEnabled( true );
            if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
                break;
            }
            FORM_BUTTONS [ BUTTON_YENI_KAYIT ]      -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_KAYDI_SIL ]       -> setEnabled( true );
            for (int i = 0; i < p_template_buttons.size(); i++) {
                p_template_buttons.at(i)->setEnabled(true);
            }
            break;
        case NEED_UPDATE:
            FORM_BUTTONS [ BUTTON_KAYDET   ]        -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_YENILE   ]        -> setEnabled( true );
            if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
                break;
            }
            FORM_BUTTONS [ BUTTON_YENI_KAYIT ]      -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_KAYDI_SIL ]       -> setEnabled( true );
            for (int i = 0; i < p_template_buttons.size(); i++) {
                p_template_buttons.at(i)->setEnabled(true);
            }
            break;
        case NEED_ADD:
            FORM_BUTTONS [ BUTTON_KAYDET ]          -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_YENILE   ]        -> setEnabled( false );
            if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
                break;
            }
            FORM_BUTTONS [ BUTTON_YENI_KAYIT ]      -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_KAYDI_SIL ]       -> setEnabled( false );
            for (int i = 0; i < p_template_buttons.size(); i++) {
                p_template_buttons.at(i)->setEnabled(true);
            }
            break;
        case EMPTY:
            FORM_BUTTONS [ BUTTON_YENILE   ]        -> setEnabled( false );
            FORM_BUTTONS [ BUTTON_KAYDET ]          -> setEnabled( false );
            if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
                break;
            }
            FORM_BUTTONS [ BUTTON_YENI_KAYIT ]      -> setEnabled( false );
            FORM_BUTTONS [ BUTTON_KAYDI_SIL ]       -> setEnabled( false );
            for (int i = 0; i < p_template_buttons.size(); i++) {
                p_template_buttons.at(i)->setEnabled(false);
            }
            break;
        case CANT_LOCKED:
            FORM_BUTTONS [ BUTTON_YENILE   ]        -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_KAYDET ]          -> setEnabled( false );
            if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
                break;
            }
            FORM_BUTTONS [ BUTTON_YENI_KAYIT ]      -> setEnabled( true );
            FORM_BUTTONS [ BUTTON_KAYDI_SIL ]       -> setEnabled( false );
            for (int i = 0; i < p_template_buttons.size(); i++) {
                p_template_buttons.at(i)->setEnabled(false);
            }
            break;

        default:
            break;
    }
}

/**************************************************************************************
                   FORM_KERNEL::REGISTER_SAVER_BUTTON
***************************************************************************************/

void FORM_KERNEL::REGISTER_SAVER_BUTTON ( QAbstractButton *button )
{
    p_template_buttons << button;

    disconnect(button,SIGNAL ( clicked()),0,0 );
    button->setObjectName ( "saver_button" );
    connect(button, SIGNAL ( clicked() ), this, SLOT ( SLOT_BUTTON_KAYDET_TEMPLATE_BUTTONS()));
}

/**************************************************************************************
                   FORM_KERNEL::BUTTON_KAYDET_TEMPLATE_BUTTONS
***************************************************************************************/

void FORM_KERNEL::BUTTON_KAYDET_TEMPLATE_BUTTONS ( QObject * button_obj )
{
    QAbstractButton * button = ((QAbstractButton *)button_obj );

    if (button EQ NULL) {
        return;
    }

    if ( p_record_status EQ NEED_UPDATE OR p_record_status EQ NEED_ADD) {
        int save_return_value = SAVE_RECORD();
        if ( save_return_value EQ RECORD_CANT_SAVED OR save_return_value EQ RECORD_EMPTY) {
            return;
        }
    }

    if ( GET_RECORD_SAVE_STATUS() EQ NEED_UPDATE ) {
        return;
    }

    CLOSE_ALL_EVENTS();
    SAVER_BUTTON_CLICKED ( button, p_current_record_id);
    OPEN_ALL_EVENTS();
}

/**************************************************************************************
                   FORM_KERNEL::LEAVE_RECORD
***************************************************************************************/

int FORM_KERNEL::LEAVE_RECORD()
{
    return ADAK_OK;
}

/**************************************************************************************
                   FORM_KERNEL::SAVER_BUTTON_CLICKED
***************************************************************************************/

void FORM_KERNEL::SAVER_BUTTON_CLICKED ( QAbstractButton * button, int record_id )
{
    Q_UNUSED(button);
    Q_UNUSED(record_id);
    return;
}

/**************************************************************************************
                   FORM_KERNEL::GET_RECORD_SAVE_STATUS
***************************************************************************************/

int FORM_KERNEL::GET_RECORD_SAVE_STATUS()
{
    return p_record_status;
}

/**************************************************************************************
                   FORM_KERNEL::GET_CURRENT_RECORD_ID
***************************************************************************************/

void FORM_KERNEL::SET_CURRENT_RECORD_ID ( int record_id )
{
    p_previous_record_id = p_current_record_id;
    p_current_record_id  = record_id;
}

/**************************************************************************************
                   FORM_KERNEL::SAVE_FIS_RECORD
***************************************************************************************/

int FORM_KERNEL::SAVE_FIS_RECORD()
{
    return SAVE_RECORD();
}

/**************************************************************************************
                   FORM_KERNEL::TRY_LOCK_RECORD
***************************************************************************************/

bool FORM_KERNEL::TRY_LOCK_RECORD()
{
    if ( p_table_is_filling EQ true ) {
        return true;
    }

    if ( p_record_status EQ NEED_UPDATE OR p_record_status EQ SAVED OR p_record_status EQ CANT_LOCKED ) {
        switch ( p_lock_status ) {
            case CANT_LOCK:
                MSG_WARNING(tr ( "Kayıt şu anda başka biri tarafından güncelleniyor."),NULL);
                return false;
                break;
            case LOCK_NOT_REQUESTED:
                {
                    if ( LOCK_RECORD ( p_current_record_id) EQ ADAK_FAIL ) {
                        MSG_WARNING(tr ( "Kayıt şu anda başka biri tarafından güncelleniyor."),NULL);
                        SET_RECORD_STATUS(CANT_LOCKED);
                        UPDATE_BUTTON_STATUS();
                        p_lock_status = CANT_LOCK;
                        return false;
                        break;
                    }
                    p_lock_status = LOCKED;
                    return true;
                }
                break;
            case LOCKED:
            default:
                return true;
                break;
        }
    }
    return true;
}

/**************************************************************************************
                   FORM_KERNEL::TRY_UNLOCK_RECORD
***************************************************************************************/

void FORM_KERNEL::TRY_UNLOCK_RECORD ( int record_id)
{
    if ( p_lock_status EQ LOCKED) {
        if ( record_id EQ -1) {
            UNLOCK_RECORD ( p_current_record_id);
        }
        else {
            UNLOCK_RECORD ( p_previous_record_id);
        }
    }
    p_lock_status = LOCK_NOT_REQUESTED;
}

/**************************************************************************************
                   FORM_KERNEL::GET_LOCK_STATUS
***************************************************************************************/

int FORM_KERNEL::GET_LOCK_STATUS()
{
    return p_lock_status;
}

/**************************************************************************************
                   FORM_KERNEL::GET_RECORD_ID
***************************************************************************************/

int FORM_KERNEL::GET_RECORD_ID ()
{
    return p_current_record_id;
}

/**************************************************************************************
                   FORM_KERNEL::eventFilter
***************************************************************************************/

bool FORM_KERNEL::eventFilter ( QObject *obj, QEvent *event )
{
    //FIXME BATCH DEDE VAR AYNISI
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

    if ( event->type() EQ QEvent::User OR event->type() EQ QEvent::Resize) {
        return BASE_KERNEL::eventFilter(obj,event);
    }

    //--------------------- KEY PRESS EVENTS

    if ( event->type() EQ QEvent::KeyPress) {
        QKeyEvent * key_event = static_cast<QKeyEvent *>(event);

        if ( obj->metaObject()->className() EQ QTextEdit::staticMetaObject.className() OR
             obj->metaObject()->className() EQ QPlainTextEdit::staticMetaObject.className() OR
             obj->metaObject()->className() EQ QLimitedTextEdit::staticMetaObject.className() ) {
            if ( key_event->key() EQ Qt::Key_Tab ) {
                focusNextChild();
                return true;
            }
            return BASE_EVENT_KERNEL::eventFilter(obj,event);
        }

        if( key_event->key() EQ Qt::Key_Enter OR key_event->key() EQ Qt::Key_Return OR key_event->key() EQ Qt::Key_Tab) {
                focusNextChild();
                return true;
        }
        else if (key_event->key() EQ Qt::Key_Down) {
            if (obj->metaObject()->className() EQ QComboBox::staticMetaObject.className()) {
                static_cast<QComboBox *>(obj)->showPopup();
                return true;
            }
        }
        else if (key_event->key() EQ Qt::Key_Up) {
            if (obj->metaObject()->className() EQ QComboBox::staticMetaObject.className()) {
                static_cast<QComboBox *>(obj)->showPopup();
                return true;
            }
        }

        else if ( key_event->key() EQ Qt::Key_Escape ) {
            ADAK_CLOSE();
            return true;
        }

        else if (key_event->key() EQ Qt::Key_F2) {
            BUTTON_KAYDET_CLICKED();
            return true;
        }

        else if (key_event->key() EQ Qt::Key_F12) {
            return false;
        }

        if ( p_record_mode EQ SINGLE_RECORD_MODE ) {
            return BASE_EVENT_KERNEL::eventFilter(obj,event);
        }

        if ( key_event->key() EQ Qt::Key_PageUp ) {
            BUTTON_SONRAKI_KAYIT_CLICKED();
            return true;
        }
        else if ( key_event->key() EQ Qt::Key_PageDown ) {
            BUTTON_ONCEKI_KAYIT_CLICKED();
            return true;
        }
        return BASE_EVENT_KERNEL::eventFilter(obj,event);
    }

    //-------------------------------- MOUSE WHEEL EVENT

    if ( obj->metaObject()->className() EQ QComboBox::staticMetaObject.className() ) {
        if ( event->type() EQ QEvent::Wheel ) {
            return true;
        }
    }

    //------------------------------- CLOSE EVENT


    if ( event->type() EQ QEvent::Close ) {

        close_event_started = true;

        switch( p_record_status ) {
            case NEED_UPDATE:
            case NEED_ADD:
                {
                    int kullanici_secimi = UNSAVED_RECORD_WARNING();
                    if ( kullanici_secimi EQ KERNEL_CANCEL ) {
                         close_event_started = false;
                        return IGNORE_CLOSE_EVENT(event);
                    }
                    TRY_UNLOCK_RECORD();
                }
                break;
            case SAVED:
                TRY_UNLOCK_RECORD();
                break;
            default:
                break;
        }

        int return_value = LEAVE_RECORD();

        if ( return_value NE ADAK_OK AND return_value NE ADAK_EXIT ) {
            close_event_started = false;
            return IGNORE_CLOSE_EVENT ( event );
        }

        return_value = BASE_KERNEL::eventFilter ( obj,event );
        close_event_started = false;
        return return_value;
    }

    if (close_event_started EQ true) {
        return false;
    }

    //------------------- FOCUS IN EVENT

    if ( event->type() EQ QEvent::FocusIn ) {
        QFocusEvent * focus_event = static_cast<QFocusEvent *>(event);

        bool return_value = CHECK_VAR_CONTROL ( obj );

        //FIXME BATCH DEDE VAR AYNISI
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
                   FORM_KERNEL::UNBLOCK_BUTTON_EVENTS
***************************************************************************************/

bool FORM_KERNEL::BLOCK_BUTTON_EVENTS ( QObject * button )
{
    if ( button EQ NULL) {
        return false;
    }

    ( ( QWidget * )button )->setDisabled ( true );
    if ( p_event_started EQ true ) {
        return false;
    }

    p_event_started = true;

    return true;
}

/**************************************************************************************
                   FORM_KERNEL::UNBLOCK_BUTTON_EVENTS
***************************************************************************************/

void FORM_KERNEL::UNBLOCK_BUTTON_EVENTS ( QObject * button )
{
    if ( button EQ NULL ) {
        return;
    }

    ( ( QWidget * )button )->setDisabled ( false );
    p_event_started = false;
    UPDATE_BUTTON_STATUS();
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_BUL_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_BUL_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false) {
        return;
    }

    BUTTON_BUL_CLICKED();
    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_YENI_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_YENI_KAYIT_CLICKED()
{
    if  ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false ) {
          return;
    }

    BUTTON_YENI_KAYIT_CLICKED();
    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_KAYDI_SIL_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_KAYDI_SIL_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false ) {
        return;
    }

    BUTTON_KAYDI_SIL_CLICKED();
    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_KAYDET_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_KAYDET_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false ) {
         return;
    }

    BUTTON_KAYDET_CLICKED();
    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_SON_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_SON_KAYIT_CLICKED()
{
    if (BLOCK_BUTTON_EVENTS ( sender() ) EQ false ) {
        return;
    }

    BUTTON_SON_KAYIT_CLICKED();
    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_ILK_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_ILK_KAYIT_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false) {
        return;
    }

    BUTTON_ILK_KAYIT_CLICKED();
    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_ONCEKI_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_ONCEKI_KAYIT_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false) {
        return;
    }

    BUTTON_ONCEKI_KAYIT_CLICKED();
    UNBLOCK_BUTTON_EVENTS(sender());
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_SONRAKI_KAYIT_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_SONRAKI_KAYIT_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false ) {
        return;
    }

    BUTTON_SONRAKI_KAYIT_CLICKED();
    UNBLOCK_BUTTON_EVENTS(sender());
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_YENILE_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_YENILE_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS ( sender() ) EQ false ) {
        return;
    }

    BUTTON_YENILE_CLICKED();
    UNBLOCK_BUTTON_EVENTS(sender());
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_GETIR_CLICKED
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_GETIR_CLICKED()
{
    if ( BLOCK_BUTTON_EVENTS(sender()) EQ false ) {
        return;
    }

    BUTTON_GETIR_CLICKED();
    UNBLOCK_BUTTON_EVENTS(sender());
}

/**************************************************************************************
                   FORM_KERNEL::SLOT_BUTTON_KAYDET_TEMPLATE_BUTTONS
***************************************************************************************/

void FORM_KERNEL::SLOT_BUTTON_KAYDET_TEMPLATE_BUTTONS()
{
    if (p_event_started EQ true) {
        return;
    }

    p_event_started = true;

    BUTTON_KAYDET_TEMPLATE_BUTTONS ( sender() );

    UNBLOCK_BUTTON_EVENTS ( sender() );
}

/**************************************************************************************
                    FORM_KERNEL::FORM_KERNEL_CHECK_VAR_RETURN_VALUE
***************************************************************************************/

void FORM_KERNEL::PROCESS_CHECK_VAR_RETURN_VALUE ( int return_value )
{
    switch ( return_value ) {
        case ADAK_FAIL:
            return;
            break;
        case ADAK_NEW_RECORD:
            {
                YENI_KAYIT();
            }
            return;
            break;
        case ADAK_OK:
        default:
            break;
    }
    return;
}

/**************************************************************************************
                    FORM_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT
***************************************************************************************/

void FORM_KERNEL::INCREASE_DEGISEN_WIDGET_COUNT ( QWidget * )
{
    m_degisen_widget_counter++;
}

/**************************************************************************************
                    FORM_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT
***************************************************************************************/

void FORM_KERNEL::DECREASE_DEGISEN_WIDGET_COUNT ( QWidget * )
{
    m_degisen_widget_counter--;
    if (is_fis_kernel EQ false) {
        return;
    }
    m_degisen_widget_counter--;
}

/**************************************************************************************
                   FORM_KERNEL::PROCESS_ADAK_EVENTS
***************************************************************************************/

void FORM_KERNEL::PROCESS_ADAK_EVENTS()
{
    QList<int> * event_list = GET_ADAK_EVENT_QUEUE();

    for ( int i = 0 ; i < event_list->size() ; i++ ) {
        switch ( event_list->at(i) ) {
            default:
            case ADAK_REFRESH:
                REFRESH_FORM_RECORD();
                break;
        }
    }

    event_list->clear();
}
