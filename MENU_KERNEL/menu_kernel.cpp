#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QMenuBar>
#include <QVariant>
#include <QKeyEvent>
#include "yonetim.h"
#include "adak_utils.h"
#include "menu_kernel.h"

#define BUTTONLAR_ARASI_SEPERATOR_BOSLUGU 4
#define ICONSUZ_BUTTON_YUKSEKLIGI         32

/**************************************************************************************
                   MENU_KERNEL::MENU_KERNEL
***************************************************************************************/

MENU_KERNEL::MENU_KERNEL ( QWidget * parent ) : QMainWindow ( parent )
{
    SET_BUTTON_TEXT_POS ( BUTTON_ICON_YOK );
    SET_ICON_SIZE ( 1, 1 );
    SET_THIS(this);

    HIDE_MENU_ACTIONS();
    SHOW_BUTTON_COLUMN_HEADER();

    m_main_widget = NULL;

    m_default_style_sheet = "";

    SET_ICON_SIZE ( 32 , 32 );

    installEventFilter(this);
}

/**************************************************************************************
                   MENU_KERNEL::REGISTER_MENU_WIDGET
***************************************************************************************/

void MENU_KERNEL::REGISTER_MENU_WIDGET ( QWidget * widget )
{
    m_menu_widget = widget;

    QHBoxLayout * main_layout = new QHBoxLayout;

    m_menu_widget->setLayout ( main_layout );
    m_menu_widget->setSizePolicy ( QSizePolicy::Ignored,QSizePolicy::Ignored );
}

/**************************************************************************************
                   MENU_KERNEL::START_KERNEL
***************************************************************************************/

void MENU_KERNEL::START_KERNEL()
{
    SETUP_FORM  ();
    INIT_KERNEL ();
}

/**************************************************************************************
                   MENU_KERNEL::INIT_KERNEL
***************************************************************************************/

void MENU_KERNEL::INIT_KERNEL()
{
    CREATE_MENU();
    ADD_STATUS_BAR_INFO();
}

/**************************************************************************************
                   MENU_KERNEL::SET_KERNEL_STRUCT
***************************************************************************************/

void MENU_KERNEL::SET_KERNEL_STRUCT ( MENU_STRUCT * menu_struct )
{
    m_num_of_secenek = 0;

    while ( menu_struct [ m_num_of_secenek ].enum_id NE 0 ) {
        m_num_of_secenek++;
    }
    M_KERNEL_MENU_SECENEGI = new KERNEL_MENU_SECENEGI [ m_num_of_secenek + 1 ];

    m_secenek_no = 0;

    FIND_CHILDS  ( menu_struct , 0 , 0, 0 );

    ADD_BOS_BUTTON_PROPERTY();

    m_max_item_count_on_column = 0;
    int child_count        = 0;


    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].parent_enum_id EQ 0 ) {
            child_count = 0;

        }
        else if ( M_KERNEL_MENU_SECENEGI[i].menu_column_visibility EQ MENU_SAYFADA OR M_KERNEL_MENU_SECENEGI[i].menu_column_visibility EQ MENU_HERYERDE ) {
            child_count++;
        }

        if ( child_count > m_max_item_count_on_column ) {
            m_max_item_count_on_column = child_count;
        }
    }
}

/**************************************************************************************
                   MENU_KERNEL::FIND_CHILDS
***************************************************************************************/

void MENU_KERNEL::FIND_CHILDS ( MENU_STRUCT * menu_struct , int parent_secenek_no , int parent_enum_id, int menu_level)
{
    for ( int i=0; i < m_num_of_secenek; i++ ) {
        if ( menu_struct[i].parent_enum_id NE parent_enum_id ) {
            continue;
        }
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].enum_id                    =  menu_struct[i].enum_id;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].is_visible                 =  false;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].is_enable                  =  true;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].menu_level                 =  menu_level;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].parent_enum_id             =  menu_struct[i].parent_enum_id;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].num_of_childs              =  0;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].icon_path                  =  menu_struct[i].icon_path;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].shortcut                   =  menu_struct[i].shortcut;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].text_to_display            =  menu_struct[i].text_to_display;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].menu_column_visibility     =  menu_struct[i].menu_column_visibility;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].tab_str                    =  menu_struct[i].tab_str;
        M_KERNEL_MENU_SECENEGI [ m_secenek_no ].column_stretch             =  menu_struct[i].column_stretch;
        if ( menu_struct[i].row_stretch NE 0 ) {
            M_KERNEL_MENU_SECENEGI [ m_secenek_no ].row_stretch                =  menu_struct[i].row_stretch;
        }
        else {
            M_KERNEL_MENU_SECENEGI [ m_secenek_no ].row_stretch                = 1;
        }

        m_secenek_no++;

        if ( i NE parent_secenek_no ) {
            M_KERNEL_MENU_SECENEGI [ parent_secenek_no ].num_of_childs++;
        }

        FIND_CHILDS ( menu_struct ,  m_secenek_no-1, menu_struct[i].enum_id , menu_level+1 );
    }
}

/**************************************************************************************
                   MENU_KERNEL::ADD_BOS_BUTTON_PROPERTY
***************************************************************************************/

void MENU_KERNEL::ADD_BOS_BUTTON_PROPERTY()
{
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].enum_id                    =  -999;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].is_visible                 =  true;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].is_enable                  =  false;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].menu_level                 =  0;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].parent_enum_id             =  0;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].num_of_childs              =  0;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].icon_path                  =  ":/kernel_icons/kernel_empty_icon.png";
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].shortcut                   =  "";
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].text_to_display            =  "";
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].menu_column_visibility     =  MENU_SAYFADA;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].tab_str                    =  "";
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].column_stretch             =  0;
    M_KERNEL_MENU_SECENEGI [ m_secenek_no ].row_stretch                =  1;
}

/**************************************************************************************
                   MENU_KERNEL::CREATE_MENU
***************************************************************************************/

void MENU_KERNEL::CREATE_MENU()
{
    QHBoxLayout * main_layout = new QHBoxLayout;

    m_main_widget = new QWidget;
    m_main_widget->setAttribute ( Qt::WA_DeleteOnClose );
    m_main_widget->setLayout ( main_layout );

    m_menu_widget->layout()->addWidget ( m_main_widget );


    QVBoxLayout * layout = NULL;
    QList<QMenu *> menu;

    QList<int> column_stretch;
    QList<int> row_stretch;

    int  num_of_child                  =  -1;
    int  column_eklenen_button_sayisi  =   0;

    //Hizalama icin button lazimmi?
    bool check_align_buttons_needed;
    bool add_new_column;

    for ( int index = 0 ; index < m_num_of_secenek ; index++ ) {
        check_align_buttons_needed = false;
        add_new_column             = false;

        if ( M_KERNEL_MENU_SECENEGI [index].parent_enum_id NE 0 ) {
            ADD_ITEMS_TO_MENU_COLUMN ( layout,&menu,&num_of_child,index );
            row_stretch << M_KERNEL_MENU_SECENEGI [ index ].row_stretch;

            column_eklenen_button_sayisi++;

            if ( num_of_child > -1 ) {
                 num_of_child--;
                if ( num_of_child EQ -1 ) {
                    menu.removeLast();
                }
            }

            if (index + 1 EQ m_num_of_secenek ) {
                check_align_buttons_needed = true;
            }
        }
        else {
            add_new_column             = true;
            check_align_buttons_needed = true;
        }

        // Check for align buttons and stretch

        if ( layout NE NULL AND check_align_buttons_needed EQ true ) {
            if ( column_eklenen_button_sayisi < m_max_item_count_on_column ) {
                for ( int i = column_eklenen_button_sayisi ; i < m_max_item_count_on_column ; i++ ) {
                    ADD_ITEMS_TO_MENU_COLUMN ( layout,&menu,&num_of_child,m_secenek_no );
                    row_stretch << M_KERNEL_MENU_SECENEGI [ index ].row_stretch;
                }
            }

            for ( int i = 0 ; i < m_max_item_count_on_column ; i++  ) {
                 layout->setStretch ( i,row_stretch.at(i) ) ;
            }
        }

        // Check for new column

        if ( add_new_column EQ true ) {
            column_eklenen_button_sayisi = 0;
            row_stretch.clear();            

            layout = CREATE_NEW_MENU_COLUMN ( layout,&menu,main_layout,index );
            column_stretch << M_KERNEL_MENU_SECENEGI [ index ].column_stretch;
        }
    }

    for ( int i = 0 ; i < column_stretch.size(); i++ ) {
        main_layout->setStretch ( i,column_stretch.at(i) );
    }
}

/**************************************************************************************
                   MENU_KERNEL::CLEAR_MENU
***************************************************************************************/

void MENU_KERNEL::CLEAR_MENU()
{
    if ( m_main_widget NE NULL ) {
        m_main_widget->close();
    }

    this->menuBar()->clear();
}

/**************************************************************************************
                   MENU_KERNEL::CREATE_NEW_MENU_COLUMN
***************************************************************************************/

QVBoxLayout * MENU_KERNEL::CREATE_NEW_MENU_COLUMN ( QLayout * layout,QList<QMenu *> * action_menu,QLayout * parent_layout,int index)
{
    if ( layout NE NULL ) {
         action_menu->clear();
    }

    QFrame      * container          = new QFrame ( this ) ;
    QVBoxLayout * vertical_layout    = new QVBoxLayout ( container );

    container->setLayout ( vertical_layout );

    if ( M_KERNEL_MENU_SECENEGI[index].is_visible EQ true AND ( M_KERNEL_MENU_SECENEGI [ index ].menu_column_visibility EQ MENU_HERYERDE OR M_KERNEL_MENU_SECENEGI[index].menu_column_visibility EQ MENU_SAYFADA) ) {
         parent_layout->addWidget ( container );
        if (M_KERNEL_MENU_SECENEGI [ index ].num_of_childs EQ 0 )  {
            container->hide();
            QHBoxLayout * box_layout = static_cast< QHBoxLayout * > ( parent_layout ) ;
            box_layout->addSpacerItem ( new QSpacerItem ( 40,20,QSizePolicy::Expanding,QSizePolicy::Fixed ) );
        }
    }
    else {
        container->hide();
    }

    QMenu * menu = new QMenu ( M_KERNEL_MENU_SECENEGI[index].text_to_display );
    *action_menu << menu;

    if ( M_KERNEL_MENU_SECENEGI[index].is_visible EQ true AND ( M_KERNEL_MENU_SECENEGI [ index ].menu_column_visibility EQ MENU_HERYERDE OR M_KERNEL_MENU_SECENEGI[index].menu_column_visibility EQ MENU_TOPMENUDE) ) {
         this->menuBar()->addMenu ( menu );
    }

    SET_WIDGET_DEFAULTS( action_menu->last(),index );
    menu->hide();

    QVBoxLayout * menu_header_vertical_layout  = ADD_FRAME ( vertical_layout,container,MENU_HEADER_ITEM,index );
    QVBoxLayout * menu_body_vertical_layout    = ADD_FRAME ( vertical_layout,container,MENU_BODY_ITEM  ,index );

    if ( M_KERNEL_MENU_SECENEGI[index].num_of_childs NE 0 ) {
        ADD_MENU_KERNEL_LABEL ( menu_header_vertical_layout,MENU_HEADER_ITEM,M_KERNEL_MENU_SECENEGI [index].tab_str + M_KERNEL_MENU_SECENEGI[index].text_to_display,index);
    }
    else {
        ADD_MENU_KERNEL_BUTTON ( menu_header_vertical_layout,MENU_HEADER_ITEM,M_KERNEL_MENU_SECENEGI[index].tab_str + M_KERNEL_MENU_SECENEGI[index].text_to_display,index);
    }

    return menu_body_vertical_layout;
}

/**************************************************************************************
                   MENU_KERNEL::ADD_ITEMS_TO_MENU_COLUMN
***************************************************************************************/

void MENU_KERNEL::ADD_ITEMS_TO_MENU_COLUMN ( QLayout *layout, QList<QMenu *> *action_menu,int * number_of_child,int index)
{
    if ( M_KERNEL_MENU_SECENEGI [index].num_of_childs > 0 ) {
         QMenu * menu_sub_window = new QMenu ( M_KERNEL_MENU_SECENEGI [index].text_to_display );
        if ( M_KERNEL_MENU_SECENEGI[index].is_visible EQ true AND ( M_KERNEL_MENU_SECENEGI[index].menu_column_visibility EQ MENU_HERYERDE OR M_KERNEL_MENU_SECENEGI[index].menu_column_visibility EQ MENU_TOPMENUDE) ) {
             action_menu->last()->addMenu ( menu_sub_window );
        }

        *action_menu << menu_sub_window;
        
        *number_of_child = M_KERNEL_MENU_SECENEGI[index].num_of_childs;

        M_KERNEL_MENU_SECENEGI[index].is_enable = false;

        ADD_MENU_KERNEL_BUTTON ( layout,MENU_BODY_ITEM,M_KERNEL_MENU_SECENEGI[index].tab_str + M_KERNEL_MENU_SECENEGI[index].text_to_display,index);
    }
    else {        
        if (M_KERNEL_MENU_SECENEGI[index].text_to_display.isEmpty() EQ false) {
            QAction * action = new QAction ( M_KERNEL_MENU_SECENEGI[index].text_to_display,action_menu->last() );
            action->setObjectName ( QVariant ( M_KERNEL_MENU_SECENEGI[index].enum_id).toString() );
            connect ( action,SIGNAL ( triggered()),this,SLOT(SLOT_MENU_ITEM_CLICKED() ) );
            action_menu->last()->addAction ( action );
            if ( m_seperator_list.contains ( M_KERNEL_MENU_SECENEGI[index].enum_id) EQ true ) {
                action_menu->last()->addSeparator();
            }
            SET_WIDGET_DEFAULTS ( action,index );
        }
        ADD_MENU_KERNEL_BUTTON ( layout,MENU_BODY_ITEM,M_KERNEL_MENU_SECENEGI[index].tab_str + M_KERNEL_MENU_SECENEGI[index].text_to_display,index);
    }
}

/**************************************************************************************
                   MENU_KERNEL::ADD_FRAME
***************************************************************************************/

QVBoxLayout * MENU_KERNEL::ADD_FRAME ( QLayout * layout, QWidget * parent, int frame_type,int index )
{
    QFrame      * menu_frame                  = new QFrame ( parent );
    QVBoxLayout * menu_frame_vertical_layout  = new QVBoxLayout ( menu_frame );

    menu_frame->setLayout ( menu_frame_vertical_layout );

    if ( frame_type EQ MENU_HEADER_ITEM ) {
        if ( M_KERNEL_MENU_SECENEGI[index].text_to_display.isEmpty() EQ false ) {
            menu_frame->setStyleSheet ( "QFrame { background-color: rgb(228, 228, 228 ) ;  }; " );
            menu_frame->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Fixed );
            layout->addWidget ( menu_frame );
        }
    }
    else {
        menu_frame->setFrameShape ( QFrame::Box );
        menu_frame->setFrameStyle ( QFrame::Raised );
        menu_frame->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
        menu_frame_vertical_layout->setSpacing (0);
        layout->addWidget ( menu_frame );
        SET_WIDGET_DEFAULTS ( menu_frame,index );
    }

    return menu_frame_vertical_layout;
}

/**************************************************************************************
                   MENU_KERNEL::SLOT_MENU_ITEM_CLICKED
***************************************************************************************/

void MENU_KERNEL::SLOT_MENU_ITEM_CLICKED()
{
    MENU_ITEM_CLICKED ( sender()->objectName().toInt() );
}

/**************************************************************************************
                   MENU_KERNEL::ADD_MENU_KERNEL_BUTTON
***************************************************************************************/

void MENU_KERNEL::ADD_MENU_KERNEL_BUTTON ( QLayout * layout,int button_type,QString button_name,int index )
{
    QWidget * widget = new QWidget ( this );

    QString button_text_label_style_sheet = "font: 12px;";

    int button_pos = -1;

    for ( int i = 0 ; i < m_button_positions.size() ; i++ ) {
         QStringList button_info = m_button_positions.at(i).split ( "//t//" );
        if ( button_info.at(0).toInt() EQ M_KERNEL_MENU_SECENEGI[index].enum_id ) {
            button_pos = button_info.at(1).toInt();
            break;
        }
    }

    QToolButton * menu_button_item = new QToolButton ( widget );

    menu_button_item->setText ( button_name );

    if ( button_type EQ MENU_BODY_ITEM ) {
        connect ( menu_button_item,SIGNAL ( clicked() ),this,SLOT ( SLOT_MENU_ITEM_CLICKED() ) );
        menu_button_item->setSizePolicy ( QSizePolicy::Fixed,QSizePolicy::Fixed );
        menu_button_item->setObjectName ( QVariant ( M_KERNEL_MENU_SECENEGI[index].enum_id ).toString() );
        menu_button_item->setStyleSheet ( "QToolButton { text-align: left;  background-color:rgba (255, 255, 255, 0); "
                                          "font: 14px \"Monospace\";  } ;" );
    }
    else {
        menu_button_item->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Fixed );
    }

    if ( button_pos EQ -1 ) {
         button_pos = m_default_button_pos;
    }

    if ( button_pos EQ BUTTON_USTUNDE_SAG_TARAFTA ) {
        QVBoxLayout * box_layout = new QVBoxLayout;
        widget->setLayout ( box_layout );
        box_layout->addWidget ( menu_button_item );
        menu_button_item->setToolButtonStyle ( Qt::ToolButtonTextBesideIcon );
    }
    else if ( button_pos EQ BUTTON_USTUNDE_ALT_TARAFTA ) {
        QVBoxLayout * box_layout = new QVBoxLayout;
        widget->setLayout ( box_layout );
        box_layout->addWidget ( menu_button_item );
        menu_button_item->setToolButtonStyle ( Qt::ToolButtonTextUnderIcon );
    }
    else if ( button_pos EQ BUTTON_DISINDA_SAG_TARAFTA ) {
        QHBoxLayout * box_layout = new QHBoxLayout;
        widget->setLayout ( box_layout );
        box_layout->addWidget ( menu_button_item );
        menu_button_item->setToolButtonStyle ( Qt::ToolButtonIconOnly ) ;

        QLabel * label = new QLabel;
        label->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Fixed );
        label->setAlignment ( Qt::AlignCenter );
        label->setStyleSheet(button_text_label_style_sheet);
        label->setText ( button_name );
        box_layout->addWidget ( label );
        SET_WIDGET_DEFAULTS ( label,index );
    }
    else if ( button_pos EQ BUTTON_DISINDA_ALT_TARAFTA ) {
        QVBoxLayout * box_layout = new QVBoxLayout;
        box_layout->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding));
        widget->setLayout ( box_layout );

        QHBoxLayout * horizontal_box_layout = new QHBoxLayout;
        box_layout->addLayout ( horizontal_box_layout );

        horizontal_box_layout->addSpacerItem ( new QSpacerItem ( 40,20,QSizePolicy::Expanding,QSizePolicy::Fixed));
        horizontal_box_layout->addWidget ( menu_button_item );
        horizontal_box_layout->addSpacerItem ( new QSpacerItem ( 40,20,QSizePolicy::Expanding,QSizePolicy::Fixed));
        menu_button_item->setToolButtonStyle ( Qt::ToolButtonIconOnly );

        QLabel * label = new QLabel;
        label->setSizePolicy ( QSizePolicy::Preferred,QSizePolicy::Fixed );
        label->setAlignment ( Qt::AlignCenter );
        label->setStyleSheet(button_text_label_style_sheet);
        label->setText ( button_name );
        box_layout->addWidget ( label );
        box_layout->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Fixed,QSizePolicy::Expanding));
        SET_WIDGET_DEFAULTS ( label,index );
    }
    else if ( button_pos EQ BUTTON_TEXT_YOK            ) {
        QVBoxLayout * box_layout = new QVBoxLayout;
        widget->setLayout ( box_layout );
        box_layout->addWidget ( menu_button_item );
        menu_button_item->setToolButtonStyle ( Qt::ToolButtonIconOnly );
    }

    if ( button_pos EQ BUTTON_ICON_YOK            ) {
        QVBoxLayout * box_layout = new QVBoxLayout;
        widget->setLayout ( box_layout );
        layout->addWidget ( widget );
        menu_button_item->setToolButtonStyle ( Qt::ToolButtonTextBesideIcon );
        menu_button_item->setIcon ( QIcon ( ":/kernel_icons/kernel_align_icon.png" ) );
        menu_button_item->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Fixed);
        QString temp_obj_name = menu_button_item->objectName();
        menu_button_item->setObjectName ( "NO_ICON" );
        menu_button_item->setIconSize ( QSize(32,ICONSUZ_BUTTON_YUKSEKLIGI ) );
        SET_WIDGET_DEFAULTS ( menu_button_item,index );
        menu_button_item->setObjectName ( temp_obj_name );
        box_layout->addWidget(menu_button_item);
        layout->addWidget ( widget );
    }
    else if ( button_pos EQ -2 ) {
        //FIXME Qt nin 12095 nolu toolbutton stylesheet bugi duzelene kadar kalacak

        disconnect(menu_button_item);

        menu_button_item->close();

        QString button_tab = M_KERNEL_MENU_SECENEGI[index].tab_str;

        button_tab.replace("\t","      ",Qt::CaseInsensitive);

        QPushButton * push_button = new QPushButton (widget);
        push_button->setStyleSheet("text-align: left; margin:0; padding:0;");
        push_button->setText      (button_tab + menu_button_item->text());
        push_button->setObjectName(menu_button_item->objectName());

        connect ( push_button,SIGNAL ( clicked() ),this,SLOT ( SLOT_MENU_ITEM_CLICKED() ) );

        layout->setSpacing(0);
        layout->addWidget ( push_button );
        push_button->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Expanding);
        QString temp_obj_name = push_button->objectName();
        push_button->setObjectName ( "NO_ICON" );
        push_button->setIconSize ( QSize(32,ICONSUZ_BUTTON_YUKSEKLIGI ) );
        SET_WIDGET_DEFAULTS ( push_button,index );
        push_button->setObjectName ( temp_obj_name );
    }
    else {
        SET_WIDGET_DEFAULTS ( menu_button_item,index );
        layout->addWidget ( widget );
    }

    if (M_KERNEL_MENU_SECENEGI[index].enum_id EQ -999) {
        //Bos icon
        menu_button_item->setStyleSheet("background:rgba(255,255,255,0); color:rgba(255,255,255,0); border:0;");
    }

    if ( m_seperator_list.contains(M_KERNEL_MENU_SECENEGI[index].enum_id ) ) {
        QWidget * spacer = new QWidget;
        spacer->setMinimumHeight ( BUTTONLAR_ARASI_SEPERATOR_BOSLUGU );
        spacer->setMaximumHeight  ( BUTTONLAR_ARASI_SEPERATOR_BOSLUGU );
        layout->addWidget ( spacer );
    }
}

/**************************************************************************************
                   MENU_KERNEL::ADD_MENU_KERNEL_LABEL
***************************************************************************************/

void MENU_KERNEL::ADD_MENU_KERNEL_LABEL ( QLayout * layout,int label_type,QString label_text,int index )
{
    QLabel * label = new QLabel ( label_text, this );

    if ( label_type EQ MENU_HEADER_ITEM ) {
         label->setStyleSheet( "QLabel { font: 14px \"Serif\"; }; " );
        label->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Fixed );
        label->setAlignment  ( Qt::AlignLeft );
    }

    SET_WIDGET_DEFAULTS ( label,index );

    layout->addWidget ( label );
}

/**************************************************************************************
                   MENU_KERNEL::ADD_SPACER_ITEM
***************************************************************************************/

void MENU_KERNEL::ADD_SPACER_ITEM ( QLayout * layout )
{
    if ( layout EQ NULL ) {
        return;
    }
    QSpacerItem * spacer_item = new QSpacerItem ( 30,20,QSizePolicy::Fixed,QSizePolicy::Expanding );
    ( ( QVBoxLayout * )layout )->addSpacerItem ( spacer_item );
}

/**************************************************************************************
                   MENU_KERNEL::ADD_STATUS_BAR_INFO
***************************************************************************************/

void MENU_KERNEL::ADD_STATUS_BAR_INFO()
{
    QStatusBar * status_bar = new QStatusBar;
    status_bar->setAttribute ( Qt::WA_DeleteOnClose );

    QString values_font = "Ouhod";
    status_bar->setStyleSheet ( "QStatusBar { background-color:rgb(240,240,240); } ; " );
    status_bar->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Fixed );

    QLabel * label_kullanici_info    = CREATE_STATUS_BAR_LABEL();
    label_kullanici_info->setText ( QString ( tr("User  :  <span style=\"font-family:'%1'; \">%2 - %3</span>")).arg(values_font).arg(KULLANICI_KODU()).arg(KULLANICI_ADI()));

    QLabel * label_firma_info        = CREATE_STATUS_BAR_LABEL();
    label_firma_info->setText ( QString ( tr("Company  :  <span style=\"font-family:'%1'; \">%2 - %3</span>") ).arg(values_font).arg ( VERITABANI_ISMI() ).arg ( VERITABANI_TANIMI() ) );

    QLabel * label_bilgisayar_info   = CREATE_STATUS_BAR_LABEL();
    label_bilgisayar_info->setText ( QString ( tr("Computer  :  <span style=\"font-family:'%1'; \">%2 - %3</span>") ).arg(values_font).arg(BILGISAYAR_KODU()).arg(BILGISAYAR_ADI() ) );

    QLabel * label_program_info      = CREATE_STATUS_BAR_LABEL();
    label_program_info->setText ( QString ( "<span style=\"font-family:'%1'; \">%2 %3</span>" ).arg(values_font).arg(ADAK_PROGRAM_SHORTNAME(ADAK_DISPLAY_ID())).arg(ADAK_PROGRAM_VERSION(ADAK_DISPLAY_ID())));

    status_bar->insertPermanentWidget ( 0, label_firma_info      ,3 );
    status_bar->insertPermanentWidget ( 1, label_kullanici_info  ,3 );
    status_bar->insertPermanentWidget ( 2, label_bilgisayar_info ,3 );
    status_bar->insertPermanentWidget ( 3, label_program_info    ,0 );

    setStatusBar ( status_bar );
}

/**************************************************************************************
                   MENU_KERNEL::HIDE_MENU_ITEM
***************************************************************************************/

QLabel * MENU_KERNEL::CREATE_STATUS_BAR_LABEL()
{
    QLabel * label    = new QLabel ( this );
    label->setFont ( QFont ( "Sans Serif",9 ) );
    label->setTextFormat ( Qt::RichText );

    return label;
}

/**************************************************************************************
                   MENU_KERNEL::SHOW_MENU_ITEM
***************************************************************************************/

void MENU_KERNEL::SHOW_MENU_ITEM ( int enum_id )
{
    int menu_item_index = -1;

    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].enum_id EQ enum_id ) {
             M_KERNEL_MENU_SECENEGI[i].is_visible = true;
             M_KERNEL_MENU_SECENEGI[i].is_enable  = true;
             menu_item_index                      = i;
        }
    }

    if ( menu_item_index EQ -1 ) {
        return;
    }

    for( ; ; ) {
        if ( M_KERNEL_MENU_SECENEGI [ menu_item_index ].parent_enum_id EQ 0 ) {
            break;
        }
        for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
            if ( M_KERNEL_MENU_SECENEGI [ menu_item_index ].parent_enum_id EQ M_KERNEL_MENU_SECENEGI[i].enum_id ) {
                 M_KERNEL_MENU_SECENEGI[i].is_visible = true;
                 M_KERNEL_MENU_SECENEGI[i].is_enable  = true;
                menu_item_index                      = i;
                break;
            }
        }
    }
}

/**************************************************************************************
                   MENU_KERNEL::HIDE_MENU_ITEM
***************************************************************************************/

void MENU_KERNEL::HIDE_MENU_ITEM ( int enum_id )
{
    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].enum_id EQ enum_id ) {
             M_KERNEL_MENU_SECENEGI[i].is_visible = false;
             M_KERNEL_MENU_SECENEGI[i].is_enable  = false;
        }
    }

    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].parent_enum_id EQ enum_id ) {
             M_KERNEL_MENU_SECENEGI[i].is_visible = false;
             M_KERNEL_MENU_SECENEGI[i].is_enable  = false;
        }
    }
}

/**************************************************************************************
                   MENU_KERNEL::ENABLE_MENU_ITEM
***************************************************************************************/

void MENU_KERNEL::ENABLE_MENU_ITEM ( int enum_id )
{
    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].enum_id EQ enum_id ) {
             M_KERNEL_MENU_SECENEGI[i].is_enable = true;
        }
    }
}

/**************************************************************************************
                   MENU_KERNEL::DISABLE_MENU_ITEM
***************************************************************************************/

void MENU_KERNEL::DISABLE_MENU_ITEM ( int enum_id )
{
    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].enum_id EQ enum_id ) {
             M_KERNEL_MENU_SECENEGI[i].is_enable = false;
        }
    }

    for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
        if ( M_KERNEL_MENU_SECENEGI[i].parent_enum_id EQ enum_id ) {
             M_KERNEL_MENU_SECENEGI[i].is_enable  = false;
        }
    }
}

/**************************************************************************************
                   MENU_KERNEL::REFRESH_MENU
***************************************************************************************/

void MENU_KERNEL::REFRESH_MENU()
{
    CLEAR_MENU();
    CREATE_MENU();
    ADD_STATUS_BAR_INFO();
}

/**************************************************************************************
                   MENU_KERNEL::SET_ICON_SIZE
***************************************************************************************/

void MENU_KERNEL::SET_ICON_SIZE ( int width, int height,int column_enum_num )
{
    if ( column_enum_num NE -1 ) {
        m_icon_sizes << QVariant ( column_enum_num ).toString() + "//t//" + QVariant ( width ).toString() + "//t//" + QVariant(height).toString();
    }
    else {
        m_default_icon_size.setWidth  ( width );
        m_default_icon_size.setHeight ( height );
    }
}

/**************************************************************************************
                   MENU_KERNEL::SET_STYLE_SHEET
***************************************************************************************/

void MENU_KERNEL::SET_STYLE_SHEET ( QString style_sheet,int column_enum_num )
{
    if ( column_enum_num NE -1 ) {
         m_style_sheets << QVariant ( column_enum_num ).toString() + "//t//" + style_sheet;
    }
    else {
        m_default_style_sheet = style_sheet;
    }
}

/**************************************************************************************
                   MENU_KERNEL::SET_BUTTON_TEXT_POS
***************************************************************************************/

void MENU_KERNEL::SET_BUTTON_TEXT_POS ( int button_pos_enum, int column_enum_num )
{
    if ( column_enum_num NE -1 ) {
        QList<int> parent_ids;

        parent_ids << column_enum_num;

        for ( ; ; ) {
            if ( parent_ids.size() EQ 0 ) {
                break;
            }
            int parent_id = parent_ids.last();
            parent_ids.removeLast();

            for ( int i = 0 ; i < m_num_of_secenek ; i++ ) {
                int added_enum = -1;
                if ( M_KERNEL_MENU_SECENEGI[i].parent_enum_id EQ parent_id ) {
                    if ( parent_ids.contains(M_KERNEL_MENU_SECENEGI[i].enum_id ) EQ false ) {
                        parent_ids << M_KERNEL_MENU_SECENEGI[i].enum_id;
                    }
                    added_enum = M_KERNEL_MENU_SECENEGI[i].parent_enum_id;
                }
                else if ( M_KERNEL_MENU_SECENEGI[i].enum_id EQ parent_id ) {
                    added_enum = M_KERNEL_MENU_SECENEGI[i].enum_id;
                }
                if (added_enum NE -1) {
                    m_button_positions << QVariant(added_enum).toString() + "//t//" + QVariant(button_pos_enum).toString();
                }
            }
        }
    }
    else {
        m_default_button_pos = button_pos_enum;
    }
}

/**************************************************************************************
                   MENU_KERNEL::ADD_SEPERATOR
***************************************************************************************/

void MENU_KERNEL::ADD_SEPERATOR ( int enum_id )
{
    m_seperator_list << enum_id;
}

/**************************************************************************************
                   MENU_KERNEL::SHOW_MENU_ACTIONS
***************************************************************************************/

void MENU_KERNEL::SHOW_MENU_ACTIONS()
{
    m_menu_actions_is_visible = true;
}

/**************************************************************************************
                   MENU_KERNEL::HIDE_MENU_ACTIONS
***************************************************************************************/

void MENU_KERNEL::HIDE_MENU_ACTIONS()
{
    m_menu_actions_is_visible = false;
}

/**************************************************************************************
                   MENU_KERNEL::SHOW_BUTTON_COLUMN_HEADER
***************************************************************************************/

void MENU_KERNEL::SHOW_BUTTON_COLUMN_HEADER()
{
    m_menu_headers_is_visible = true;
}

/**************************************************************************************
                   MENU_KERNEL::HIDE_BUTTON_COLUMN_HEADER
***************************************************************************************/

void MENU_KERNEL::HIDE_BUTTON_COLUMN_HEADER()
{
    m_menu_headers_is_visible = false;
}

/**************************************************************************************
                   MENU_KERNEL::SET_WIDGET_DEFAULTS
***************************************************************************************/

void MENU_KERNEL::SET_WIDGET_DEFAULTS ( QObject * obj, int index )
{
    if ( QAction::staticMetaObject.className() EQ obj->metaObject()->className() ) {
         QAction * action = ( QAction * ) obj;

        if ( M_KERNEL_MENU_SECENEGI [index].icon_path.isEmpty() EQ false ) {
             action->setIcon ( QIcon(M_KERNEL_MENU_SECENEGI[index].icon_path ) );
        }

        action->setEnabled ( M_KERNEL_MENU_SECENEGI[index].is_enable );
        action->setVisible ( M_KERNEL_MENU_SECENEGI[index].is_visible );

        return;
    }

    QWidget * widget = ( QWidget * ) obj;

    widget->setEnabled ( M_KERNEL_MENU_SECENEGI[index].is_enable  );
    widget->setVisible ( M_KERNEL_MENU_SECENEGI[index].is_visible );

    if  ( m_default_style_sheet.isEmpty() EQ false ) {
         widget->setStyleSheet ( m_default_style_sheet + "margin:0; padding:0;" );
    }

    if ( QToolButton::staticMetaObject.className() EQ widget->metaObject()->className() ) {
         QToolButton * tool_button = ( QToolButton * ) widget;

        if ( tool_button->objectName() NE "NO_ICON" ) {
             tool_button->setIconSize ( m_default_icon_size );
            for ( int i = 0 ; i < m_icon_sizes.size() ; i++  ) {
                 QStringList icon_info = m_icon_sizes.at(i).split ( "//t//" );
                if ( icon_info.at(0).toInt() EQ M_KERNEL_MENU_SECENEGI[index].parent_enum_id OR icon_info.at(0).toInt() EQ M_KERNEL_MENU_SECENEGI[index].enum_id) {
                     tool_button->setIconSize ( QSize ( icon_info.at(1).toInt(),icon_info.at(2).toInt() ) );
                }
            }
        }

        tool_button->setShortcut ( QKeySequence ( M_KERNEL_MENU_SECENEGI[index].shortcut ) );
        tool_button->installEventFilter(this);
        tool_button->setToolTip  ( M_KERNEL_MENU_SECENEGI[index].shortcut                  );

        if ( M_KERNEL_MENU_SECENEGI[index].icon_path.isEmpty() EQ false ) {
             tool_button->setIcon ( QIcon ( M_KERNEL_MENU_SECENEGI[index].icon_path ) );
        }
    }
    else if ( QPushButton::staticMetaObject.className() EQ widget->metaObject()->className() ) {
       QPushButton * push_button = ( QPushButton * ) widget;

       push_button->setShortcut ( QKeySequence ( M_KERNEL_MENU_SECENEGI[index].shortcut ) );
       push_button->installEventFilter(this);
       push_button->setToolTip  ( M_KERNEL_MENU_SECENEGI[index].shortcut                  );

       if ( M_KERNEL_MENU_SECENEGI[index].icon_path.isEmpty() EQ false ) {
            push_button->setIcon ( QIcon ( M_KERNEL_MENU_SECENEGI[index].icon_path ) );
       }
   }

    for  ( int i = 0 ; i < m_style_sheets.size() ; i++  ) {
         QStringList style_sheet_info = m_style_sheets.at(i).split ( "//t//" );
        if ( style_sheet_info.at(0).toInt() EQ M_KERNEL_MENU_SECENEGI[index].parent_enum_id OR style_sheet_info.at(0).toInt() EQ M_KERNEL_MENU_SECENEGI[index].enum_id) {
            widget->setStyleSheet ( style_sheet_info.at(1) + "margin:0; padding:0;" );
        }
    }

    QFont font =  widget->font();
    if ( font.pointSize() EQ - 1 ) {
        font.setPixelSize(15);
    }
    else {
        font.setPixelSize(font.pointSize() + 5);
    }
}

/**************************************************************************************
                   MENU_KERNEL::HIDE_ALL
***************************************************************************************/

void MENU_KERNEL::HIDE_ALL()
{
    for (int i = 0; i < m_num_of_secenek ; i++) {
        M_KERNEL_MENU_SECENEGI[i].is_visible  = false;
    }
}

/**************************************************************************************
                   MENU_KERNEL::keyPressEvent
***************************************************************************************/

bool MENU_KERNEL::eventFilter ( QObject *obj, QEvent * event )
{
    if (event->type() EQ QEvent::KeyPress) {
        QKeyEvent * key_press_event = static_cast < QKeyEvent * > (event);

        if( key_press_event->key() EQ Qt::Key_Escape ) {
            close();
        }

        if (obj->metaObject()->className() EQ QToolButton::staticMetaObject.className()) {
            if ( key_press_event->key() EQ Qt::Key_Return OR key_press_event->key() EQ Qt::Key_Enter) {
                QToolButton * tool_button = static_cast < QToolButton * > (obj);
                MENU_ITEM_CLICKED ( tool_button->objectName().toInt() );
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(obj,event);
}
