#ifndef TASARIM_H
#define TASARIM_H

#include <QList>

class QWidget;
class QStringList;
class TASARIM_DEGISKENI;

void OPEN_TASARIM_BELGE_BATCH     ( int enum_id, QWidget * parent );
void OPEN_TASARIM_BATCH     ( double belge_genisligi , double belge_yuksekligi , int tasarim_id, int p_printer_type, QWidget * parent );
void OPEN_TASARIM_DOTMATRIX_BATCH ( int tasarim_id, QWidget * parent );

TASARIM_DEGISKENI* OPEN_TASARIM_BELGE_DEGISKENLERI_SEC ( QList< TASARIM_DEGISKENI*> p_degisken_listesi, QWidget * parent );

#endif // TASARIM_H


