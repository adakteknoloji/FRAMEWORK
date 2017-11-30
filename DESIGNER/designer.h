#ifndef ADAK_DESIGNER_H
#define ADAK_DESIGNER_H

#include <QString>
#include <QDate>
#include <QWidget>

struct GLOBAL_VARS {
    QString versiyon;
    QDate   versiyon_tarihi;
};

void        OPEN_DESIGNER_ANA_MENU              ();
void        OPEN_DESIGNER_HAKKINDA              ( QWidget * parent );
void        OPEN_DESIGNER_YETKILENDIRME_FORMU   ( int p_kullanici_id, bool * yetkileri_tazele, QWidget * parent );
void        OPEN_SQL_DESIGNER                   ( QWidget * p_parent );
void        OPEN_BELGE_DESIGNER                 ( int record_id, QWidget * parent );

#endif // ADAK_DESIGNER_H
