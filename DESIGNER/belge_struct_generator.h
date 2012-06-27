#ifndef BELGE_STRUCT_GENERATOR_H
#define BELGE_STRUCT_GENERATOR_H

#include <QString>

class BELGE_STRUCT_GENERATOR
{
    public:
        BELGE_STRUCT_GENERATOR();
        ~BELGE_STRUCT_GENERATOR();        
        void              CREATE_BELGELER_HEADER               ();

    private:
        int               m_belge_id;
        int               m_header_element_count;
        int               m_line_element_count;
        int               m_footer_element_count;
        QString           m_belge_adi;

        void              CREATE_HEADER_FILES                  ();

        QString           CREATE_TASARIM_HEADER                ();

        QString           CREATE_BLG_FILES                     (int belge_id);


        QString           CREATE_DEFINE                        (QString belge_adi,QString grup,int count);
        QString           CREATE_ENUM_STRING                   (QString belge_adi,QString grup,QString enum_list_str,int count);
        QString           CREATE_DEGISKEN_OZELLIKLERI_STRING   (QString grup,QString degiskenler,int count);
        QString           CREATE_INCLUDE_BEGIN_STR             (QString belge_adi);
        QString           CREATE_INCLUDE_END_STR               (QString belge_adi);

        void              INCLUDE_STR_OLUSTUR                  (QString * enum_str,QString * degiskenler_str,
                                                                QString degisken_id,QString degisken_adi,QString degisken_kodu,QString degisken_font,
                                                                QString degisken_align, QString monospace_mi, QString text_boyutu );

        //QString           CREATE_STRUCT_STRING                 (QString belge_adi,QString grup,int count);

};

#endif // BELGE_STRUCT_GENERATOR_H
