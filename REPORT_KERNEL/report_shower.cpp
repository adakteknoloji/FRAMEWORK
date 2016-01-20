#include <QApplication>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QTextDocumentFragment>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QMessageBox>
#include <QLabel>
#include <QSplashScreen>
#include "adak_sql.h"
#include "adak_utils.h"
#include "kernel_utils.h"
#include "yonetim.h"
#include "report_shower.h"
#include "email.h"

extern ADAK_SQL *      G_YONETIM_DB;

/**************************************************************************************
                   DIREKT_YAZDIR
***************************************************************************************/

void DIREKT_YAZDIR ( QString printer_name,QString printer_document )
{
     QPrinter printer ( QPrinter::HighResolution );
     printer.setPrinterName ( printer_name );

     QWebView * view = new QWebView;
     view->setHtml( printer_document );

     view->print( &printer );

     QMessageBox::warning ( qApp->activeWindow(), "Printer",  QObject::tr("File sent to printer!") );//Çıktı yazıcıya gönderildi.!
}

/**************************************************************************************
                   OPEN_REPORT_SHOWER
***************************************************************************************/

void OPEN_REPORT_SHOWER ( QString string_html, QWidget * parent, QPrinter::Orientation p_orientation , qreal left_margin,qreal top_margin,qreal bottom_margin,qreal right_margin ,
                        int     paper_type ,double user_defined_width,double user_defined_height )
{
    if  ( string_html.isEmpty() EQ true ) {
         QMessageBox::warning ( parent, parent->QObject::tr ( "Preview" ),parent->QObject::tr ( "A recording will be reported or displayed does not found!" ) );
         //Raporlanacak / gösterilecek bir kayit bulunamadı.
         return;
    }

    REPORT_SHOWER * report_shower   = new REPORT_SHOWER (string_html,paper_type,user_defined_width,user_defined_height, parent);
    report_shower->p_left_margin    = left_margin;
    report_shower->p_top_margin     = top_margin;
    report_shower->p_bottom_margin  = bottom_margin;
    report_shower->p_right_margin   = right_margin;
    report_shower->p_orientation    = p_orientation;

    report_shower->EXEC(FULL_SCREEN);
}

/**************************************************************************************
                   REPORT_SHOWER::REPORT_SHOWER
***************************************************************************************/

REPORT_SHOWER::REPORT_SHOWER ( QString printer_document,int paper_type,double user_defined_width,double user_defined_height, QWidget * parent ) : BASE_KERNEL ( parent )
{
    SET_PAGE_TITLE       (tr("OUTPUT PREVIEW "));
    SET_HELP_PAGE        ("report_shower");
    SET_SETTING_NAME     ("REPORT_SHOWER");

    QSplashScreen * splash = CREATE_SPLASH_SCREEN();
    splash->show();
    splash->showMessage ( ( QString ( tr("Report preview is preparing. Please wait...") ) ),Qt::AlignCenter,Qt::white);//Rapor Önizlemesi Hazırlanıyor.Lütfen Bekleyiniz...
    qApp->processEvents();

    QVBoxLayout *vertical_layout   = new QVBoxLayout();
    QHBoxLayout *horizontal_layout = new QHBoxLayout();
    preview_view                   = new QWebView;

    m_paper_type                   = paper_type;
    m_user_defined_width           = user_defined_width;
    m_user_defined_height          = user_defined_height;

    button_print_edit              = new QPushButton ( tr("Print") );
    button_print_edit->setIconSize(QSize(32,32));
    button_print_edit->setIcon(QIcon(":/kernel_icons/kernel_yazdir_ikonu.png"));

    button_email                   = new QPushButton ( tr("Sending E-mail") );
    button_email->setIconSize(QSize(32,32));
    button_email->setIcon(QIcon(":/yonetim_icons/yonetim_mail.png"));

    button_ihrac                   = new QPushButton ( tr(" Export") );//İhraç
    button_ihrac->setIconSize(QSize(32,32));
    button_ihrac->setIcon(QIcon(":/kernel_icons/kernel_export_ikonu.png"));

    this->printer_document = printer_document;

    setWindowIcon  ( QIcon (  "yazdir_ikonu.png"  ) );

    preview_view->setHtml ( printer_document , QUrl ( "file:///" ) );
    preview_view->show();

    vertical_layout->addWidget ( preview_view );
    vertical_layout->addLayout ( horizontal_layout );

    horizontal_layout->addSpacerItem ( new QSpacerItem ( 40,20,QSizePolicy::Expanding,QSizePolicy::Fixed ) );
    horizontal_layout->addWidget     ( button_print_edit );
    horizontal_layout->addWidget     ( button_email );
    horizontal_layout->addWidget     ( button_ihrac );

    connect ( button_print_edit , SIGNAL ( clicked() ), this, SLOT ( SLOT_PRINT_PREVIEW() ) );
    connect ( button_email      , SIGNAL ( clicked() ), this, SLOT ( SLOT_SEND_EMAIL() ) );
    connect ( button_ihrac      , SIGNAL ( clicked() ), this, SLOT ( SLOT_EXPORT_POPUP() ));

    setLayout ( vertical_layout );    

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    splash->finish ( NULL);
}

/**************************************************************************************
                   REPORT_SHOWER::~REPORT_SHOWER
***************************************************************************************/

REPORT_SHOWER::~REPORT_SHOWER()
{

}

/**************************************************************************************
                   REPORT_SHOWER::SLOT_PRINT_PREVIEW
***************************************************************************************/

void REPORT_SHOWER::SLOT_PRINT_PREVIEW()
{
    QPrinter printer ( QPrinter::HighResolution );

    printer.setOrientation( p_orientation );

    if ( p_left_margin NE 0.0 OR p_top_margin NE 0.0 OR p_bottom_margin NE 0.0 OR p_right_margin NE 0.0 ) {
         printer.setPageMargins ( p_left_margin,p_top_margin, p_right_margin,p_bottom_margin,QPrinter::Millimeter);
    }
    if (m_paper_type EQ QPrinter::Custom) {
        printer.setPaperSize(QSizeF(m_user_defined_width,m_user_defined_height),QPrinter::Millimeter);
    }
    else {
        QPrinter::PaperSize paper_type = (QPrinter::PaperSize)m_paper_type;
        printer.setPaperSize(paper_type);
    }

    QPrintPreviewDialog preview ( &printer, this );

    printer.setOrientation( p_orientation );

    preview.setWindowState      ( Qt::WindowMaximized );
    preview.setWindowFlags      ( Qt::Window );
    connect(&preview, SIGNAL ( paintRequested ( QPrinter * ) ), SLOT ( SLOT_PAINT_REQUESTED ( QPrinter * ) ) );
    preview.exec();
}

/**************************************************************************************
                   REPORT_SHOWER::SLOT_PAINT_REQUESTED
***************************************************************************************/

void REPORT_SHOWER::SLOT_PAINT_REQUESTED ( QPrinter *printer )
{
    preview_view->print ( printer );
}

/**************************************************************************************
                   REPORT_SHOWER::SLOT_SEND_EMAIL
***************************************************************************************/

void REPORT_SHOWER::SLOT_SEND_EMAIL()
{
    SQL_QUERY       sql_query_yonetim (  G_YONETIM_DB );
    QString         email_address = "";

    sql_query_yonetim.PREPARE_SELECT ( "ynt_kullanicilar","email_adresi,kullanici_adi","kullanici_id = :kullanici_id" ,"");
    sql_query_yonetim.SET_VALUE      (  ":kullanici_id", KULLANICI_ID() );

    if ( sql_query_yonetim.SELECT() > 0 ) {
        sql_query_yonetim.NEXT();
        email_address = sql_query_yonetim.VALUE(0).toString();
    }

    ADAK_EMAIL_SENDER ( email_address,( sql_query_yonetim.VALUE(1).toString() ),"",tr ( "No Subject" ), printer_document );
}

/**************************************************************************************
                   REPORT_SHOWER::SLOT_EXPORT_POPUP
***************************************************************************************/

void REPORT_SHOWER::SLOT_EXPORT_POPUP ()
{
    QStringList         button_names;
    QList<QIcon>        button_icons;
    QList<int>          button_enums;

    button_names << tr("OpenOffice Writer");
    button_icons << QIcon(":/kernel_icons/kernel_oo_writer_ikonu.png");
    button_enums << ENUM_KERNEL_OO_WRITER;
    button_names << tr("Text File");
    button_icons << QIcon(":/kernel_icons/kernel_text_ikonu.png");
    button_enums << ENUM_KERNEL_TXT;
    button_names << "Html";
    button_icons << QIcon(":/kernel_icons/kernel_html_ikonu.png");
    button_enums << ENUM_KERNEL_HTML;


    QWidget * widget         = new QWidget(this,Qt::Popup);

    QVBoxLayout * box_layout = new QVBoxLayout;
    widget->setLayout(box_layout);

    QLabel * label = new QLabel (tr("Export"),widget);
    label->setAlignment ( Qt::AlignCenter );
    label->setStyleSheet("color:blue; font-weight : bold; font-size:14;");
    box_layout->addWidget( label );

    for ( int i= 0; i < button_names.size() ; i++ ) {
        QPushButton * push_button = new QPushButton ( button_names.at(i) , widget );
        push_button->setIconSize(QSize(32,32));
        push_button->setIcon(button_icons.at(i));
        push_button->setObjectName(QVariant(button_enums.at(i)).toString());
        push_button->setStyleSheet("text-align:left;");
        connect(push_button,SIGNAL(clicked()),this,SLOT(SLOT_EXPORT_TO_FILE()));
        box_layout->addWidget(push_button);
    }

    widget->show();

    int x = QCursor::pos().x();
    int y = QCursor::pos().y();

    x    -= ( widget->width() - 10 );
    y    -= ( widget->height() - 10 );


    widget->move( x , y );
}


/**************************************************************************************
                   REPORT_SHOWER::SLOT_EXPORT_TO_FILE
***************************************************************************************/

void REPORT_SHOWER::SLOT_EXPORT_TO_FILE()
{
    QString save_type;
    QString file_format;
    QString file_dialog_filter;
    QString file_dialog_header;

    switch ( sender()->objectName().toInt() ) {
        case ENUM_KERNEL_OO_WRITER:
            save_type           = "ODF";
            file_format         = ".odf";
            file_dialog_filter  = tr("Open Document Format (*.odf)");
            file_dialog_header  = tr("Open Document Format - Export");
        break;
        case ENUM_KERNEL_HTML:
            save_type           = "HTML";
            file_format         = ".html";
            file_dialog_filter  = tr("Html File (*.html)");
            file_dialog_header  = tr("HTML - Export");
        break;
        case ENUM_KERNEL_TXT:
        default:
            save_type           = "plaintext";
            file_format         = ".txt";
            file_dialog_filter  = tr("Text File (*.txt)");
            file_dialog_header  = tr("Text File - Export");
        break;
    }

    QTextDocument html_document;
    html_document.setHtml(printer_document);
    QString file_name = QFileDialog::getSaveFileName(this,file_dialog_header,"",file_dialog_filter);

    if (file_name.isEmpty() EQ true) {
        return;
    }

    if ( file_name.contains(file_format) EQ false) {
        file_name.append(file_format);
    }

    QTextDocumentWriter writer;
    writer.setFileName(file_name);
    writer.setFormat(QByteArray(save_type.toUtf8().data()));
    writer.write(&html_document);
}
