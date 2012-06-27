#ifndef STRUCT_GENERATOR_H
#define STRUCT_GENERATOR_H

bool    HEADER_DOSYASI_YARAT             ( QWidget * parent );
bool    TABLOLARI_YAZ                    ( QWidget * parent );
bool    ALANLARI_YAZ                     ( int       tablo_id );
bool    INDEXLERI_YAZ                    ( int       tablo_id );
bool    HEADER_ICIN_INDEX_YAPISI_OLUSTUR ( int       index_sayisi, int       tablo_id );

#endif // STRUCT_GENERATOR_H
