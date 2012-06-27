#ifndef BELGE_STRUCT_H
#define BELGE_STRUCT_H


struct DEGISKENLER {
    char * degisken_adi;
};


struct DEGISKEN_OZELLIKLERI {
    int font_size;
    int align;
};

struct BELGE_DEGISKEN {
    DEGISKENLER *           degiskenler;
    DEGISKEN_OZELLIKLERI *  degisken_ozellikleri;

};

#endif // BELGE_STRUCT_H
