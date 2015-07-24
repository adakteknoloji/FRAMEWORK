#include <QApplication>
#include <QPixmap>
#include <QVariant>
#include <QImageReader>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QPushButton>
#include <QIcon>
#include "adak_utils.h"

/**************************************************************************************
                   ADAK_BTYE_ARRAY_2_PIXMAP
***************************************************************************************/

QPixmap ADAK_BYTE_ARRAY_2_PIXMAP (QByteArray byte_array)
{
    QPixmap pixmap;

    pixmap.loadFromData(byte_array);

    return pixmap;
}

/**************************************************************************************
                 ADAK_PIXMAP_2_BYTE_ARRAY
***************************************************************************************/

QByteArray ADAK_PIXMAP_2_BYTE_ARRAY (QString p_pixmap_path)
{
    QFile file( p_pixmap_path );
    if ( file.open(QIODevice::ReadOnly) EQ false ) {
        return NULL;
    }

    QByteArray image_byte_array = file.readAll();
    file.close();

    return image_byte_array;
}

/**************************************************************************************
                   ADAK_ADD_PIXMAP_TO_BUTTON
***************************************************************************************/

bool ADAK_ADD_PIXMAP_TO_BUTTON (QPushButton * push_button, QString * file_path, QWidget * parent)
{
    QIcon icon = push_button->icon();

    QFileDialog         file_dialog (parent);
    QStringList         file_extensions;

    file_dialog.setFileMode ( QFileDialog::AnyFile );

    //Dikkat burdaki dosya uzantilari silinmeli dosya isminden.
    file_extensions << QT_TR_NOOP("Image Files - Max. 512kb (*.png *.xpm *.jpg *.jpeg *.svg *.gif *.ico  *.tiff");

    file_dialog.setNameFilters ( file_extensions    );
    file_dialog.setViewMode    ( QFileDialog::Detail );

    if ( file_dialog.exec() ) {
        QString pixmap_path = file_dialog.selectedFiles().at(0);

        /////////// Qt Image formatini otomatik tespit etmesi icin dosya formatina bakmasi engellenir.

        QPixmap pixmap;
        QImageReader * image_reader = new QImageReader(pixmap_path);
        image_reader->setDecideFormatFromContent(true);
        if (image_reader->canRead() EQ true) {
            QImage image = image_reader->read();
            pixmap = QPixmap::fromImage(image);
        }

        ///////////////////////

        QFile file(pixmap_path);

        if ( file.size() > 512000) {
            //ADAK_WARNING (qApp->tr("Resim dosyaları 512kb büyük olamaz."),NULL, parent );
            ADAK_WARNING (QT_TR_NOOP("Image files must be smaller than 512KB."),NULL, parent );
            return false;
        }

        if (icon.pixmap(32,32).toImage() EQ QIcon(pixmap).pixmap(32,32).toImage() AND file_path->isEmpty() EQ true) {
            *file_path = "";
            return false;
        }

        *file_path = pixmap_path;

        //Button width ve height na gore setlenmesi her sette buttonun buyumesine neden olur.
        //Bu yuzden button widht ve height daha ufak bir deger girilmelidir.
        int w = push_button->width()  - 20;
        int h = push_button->height() - 20;

        push_button->setIcon     ( QIcon ( pixmap ) );
        push_button->setText     ("");

        push_button->setIconSize ( QSize ( w,h ) );
        return true;
    }

    return false;
}
