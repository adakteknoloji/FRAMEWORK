#include <QObject>
#include <QVariant>
#include <QStringList>
#include "adak_utils.h"

static const QString LEVEL_NAMES    [] = { "",  QObject::tr ( "Thousand" ),
                                                QObject::tr ( "Million"  ),
                                                QObject::tr ( "Billion"  ),
                                                QObject::tr ( "Trillion" ),
                                                QObject::tr ( "Quadtrillion" ) };

static const  QString BIRLER        []  = { "", QObject::tr ( "One"   ),
                                                QObject::tr ( "Two"   ),
                                                QObject::tr ( "Three"    ),
                                                QObject::tr ( "Four"  ),
                                                QObject::tr ( "Five"   ),
                                                QObject::tr ( "Six"  ),
                                                QObject::tr ( "Seven"  ),
                                                QObject::tr ( "Eight" ),
                                                QObject::tr ( "Nine" ) };


static const QString ONLAR          []  = { "", QObject::tr ( "Ten"     ),
                                                QObject::tr ( "Twenty"  ),
                                                QObject::tr ( "Thirty"   ),
                                                QObject::tr ( "Fourty"   ),
                                                QObject::tr ( "Fifty"   ),
                                                QObject::tr ( "Sixty" ),
                                                QObject::tr ( "Seventy" ),
                                                QObject::tr ( "Eighty" ),
                                                QObject::tr ( "Ninety" ) };


static QString YUZLUK_PARCAYI_YAZDIR ( QString p_yazdirilacak_yuzluk );

QString p_lira_string  = QObject::tr("$");
QString p_kurus_string = QObject::tr("Cent");

/***************************************************************************
                            ADAK_YAZIYLA_YAZ
****************************************************************************/

QString ADAK_YAZIYLA_YAZ ( double p_yazdirilacak_sayi, QString p_para_birimi)
{
    Q_UNUSED (p_para_birimi);

    QString yazdirilacak_sayi = QVariant ( ROUND ( p_yazdirilacak_sayi ) ).toString();

    QString     tamsayi_kisim;
    QString     ondalik_kisim;
    QString     sayi_to_yazi_string;

    tamsayi_kisim = yazdirilacak_sayi;

    if ( yazdirilacak_sayi.contains( "." ) EQ true ) {

        QStringList SAYI_BILGISI = yazdirilacak_sayi.split ( ".");

        if ( SAYI_BILGISI.size() > 1 ) {
            tamsayi_kisim = SAYI_BILGISI.at(0);
            ondalik_kisim = SAYI_BILGISI.at(1);
        }
        else {
            return "";
        }
    }

    if ( tamsayi_kisim.size() > 14 ) {
        return "";
    }

    if ( ondalik_kisim.size() EQ 1 ) {
        ondalik_kisim.append ( "0" ); // 34.1 icin 34 lira 10 KURUS yazdirmak tek basamakli kurus degerleri sonuna sifir ekliyoruz
    }

    QString sayi_dilimi;
    QString son_dilim;

    int     level_sayisi            =   0;
    int     tamsayi_kisim_size      =   tamsayi_kisim.size();

    if ( tamsayi_kisim_size < 4 ) {
        son_dilim = tamsayi_kisim;
    }
    else {  // LEVEL SAYISI TESPIT EDILIYOR
        for ( int i = 0; i < tamsayi_kisim.size(); i++ ) {
            if ( tamsayi_kisim_size < 4 ) {
                break;
            }
            level_sayisi++;
            tamsayi_kisim_size -= 3;
        }
    }

    int     tamsayi_string_index    =   0;
    int     kalan_kisim_size        = tamsayi_kisim.size();

    while ( level_sayisi > 0 ) {
        int yazdirilan_parca_size  = kalan_kisim_size - ( level_sayisi * 3 );
        QString yazdirilacak_parca = tamsayi_kisim.mid  ( tamsayi_string_index, ( yazdirilan_parca_size ) );
        if ( level_sayisi EQ 1 AND QVariant ( yazdirilacak_parca ).toInt() EQ 1 ) {
            sayi_dilimi.append ( LEVEL_NAMES [ level_sayisi ] );    // Eger 1234 gibi bir deger yazdiriyorsak 'bir bin' yazdirmamak icin
        }                                                           // sadece 'bin' ifadesi yaziya eklenmeli
        else {
            sayi_dilimi.append ( YUZLUK_PARCAYI_YAZDIR      ( yazdirilacak_parca ) + LEVEL_NAMES [ level_sayisi ] );
        }
        tamsayi_string_index += yazdirilan_parca_size;
        kalan_kisim_size     -= yazdirilan_parca_size;
        level_sayisi--;
        if ( level_sayisi EQ 0 ) {
            son_dilim = tamsayi_kisim.mid ( tamsayi_string_index, kalan_kisim_size );
        }
    }

    sayi_dilimi.append ( YUZLUK_PARCAYI_YAZDIR ( son_dilim ) );

    if ( sayi_dilimi.size() > 0 ) {
        sayi_to_yazi_string.append ( sayi_dilimi + " " + p_lira_string );
    }


    QString ondalik_kisim_yazi = YUZLUK_PARCAYI_YAZDIR ( ondalik_kisim );

    if ( ondalik_kisim_yazi.size() > 0 ) {
        sayi_to_yazi_string.append ( " " + ondalik_kisim_yazi + " " + p_kurus_string );
    }

    return sayi_to_yazi_string;
}

/***************************************************************************
                            YUZLUK_PARCAYI_YAZDIR
****************************************************************************/

static QString YUZLUK_PARCAYI_YAZDIR ( QString p_yazdirilacak_yuzluk )
{
    int yuzler_basamagi  = 0;
    int onlar_basamagi   = 0;
    int birler_basamagi  = 0;

    QString yuzluk_sayi_to_yazi_string;

    switch ( p_yazdirilacak_yuzluk.size() ) {

        case 3 :
            yuzler_basamagi =  QString ( p_yazdirilacak_yuzluk.at(0) ).toInt();
            onlar_basamagi  =  QString ( p_yazdirilacak_yuzluk.at(1) ).toInt();
            birler_basamagi =  QString ( p_yazdirilacak_yuzluk.at(2) ).toInt();
            break;

        case 2 :
            onlar_basamagi  = QString ( p_yazdirilacak_yuzluk.at(0) ).toInt();
            birler_basamagi = QString ( p_yazdirilacak_yuzluk.at(1) ).toInt();
            break;

        case 1  :

            birler_basamagi = QString ( p_yazdirilacak_yuzluk.at(0) ).toInt();
            break;

        case 0  :
        default :
            return "";
    }

    if ( yuzler_basamagi > 0 ) {
        if ( yuzler_basamagi > 1 ) {
            yuzluk_sayi_to_yazi_string.append ( BIRLER [ yuzler_basamagi ] );
        }
        yuzluk_sayi_to_yazi_string.append ( QObject::tr("Hundred") );
    }

    if ( onlar_basamagi > 0 ) {
        yuzluk_sayi_to_yazi_string.append ( ONLAR [ onlar_basamagi ] );
    }

    if ( birler_basamagi > 0 ) {
        yuzluk_sayi_to_yazi_string.append ( BIRLER [ birler_basamagi ] );
    }

    return yuzluk_sayi_to_yazi_string;
}
