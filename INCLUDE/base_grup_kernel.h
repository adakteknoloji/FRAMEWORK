#ifndef BASE_GRUP_KERNEL_H
#define BASE_GRUP_KERNEL_H

class QStringList;
class QString;

/* Grup Kernel kullanılacak formlarda database isimleri aşagıdaki şekilde olmalıdır

    gruplar_tablo_ismi                     = "tablo ismini setlenmesi gereklidir.Ornek;SET_GRUPLAR_TABLE_NAME("tablo_ismi");

    grup_id_column_name                    = "grup_id";
    grup_parent_id_column_name             = "grup_parent_id";
    grup_seviyesi_column_name              = "grup_seviyesi";
    grup_kodu_column_name                  = "grup_kodu";
    grup_adi_column_name                   = "grup_adi";
    tam_grup_kodu_column_name              = "tam_grup_kodu";
    grup_son_seviyemi_column_name          = "grup_son_seviye_mi";
    grupta_kayitli_uye_sayisi_column_name  = "gruptaki_kayit_sayisi";
    alt_grup_sayisi_column_name            = "alt_grup_sayisi";
    program_id_column_name                 = "program_id";
    modul_id_column_name                   = "modul_id";


    --Diger Farkli column isimleri varsa da onlarda ayri olarak setlenmelidir.Mesela
      UYE_SAYISI_COLUMN yukardakinden farkli ise asagidaki gibi setlenebilir

    SET_GRUPLAR_UYE_SAYISI_COLUMN_NAME("gruptaki_kayit_sayisi");
    .
    .
    .
    --Delimeter e setleriz
    SET_DELIMETER("delimeter");

    --Son olarak da set_delimeter ve init kernel i kullanarak kerneli initleriz. Tum yapilmasi gereken budur.
      Yetersiz kaliyorsa grup kernel virtual fonksiyonlar kullanilabilir yada tree kernel miras alinarak
      bastan tasarlanabilir.
    INIT_BASE_GRUP_KERNEL( parent, DB );
*/

//adak utilse cekilecek
enum KERNEL_GRUP_MODU {
    KERNEL_ALT_GRUP_EKLE  = 0,
    KERNEL_ANA_GRUP_EKLE  = 1,
    KERNEL_GRUBU_GUNCELLE = 2
};

struct GRUPLAR_DB_INFO_STRUCT {
    QString gruplar_tablo_ismi;
    QString grup_id_column_name;
    QString grup_parent_id_column_name;
    QString grup_seviyesi_column_name;
    QString grup_son_seviyemi_column_name;
    QString grup_kodu_column_name;
    QString grup_adi_column_name;
    QString tam_grup_kodu_column_name;
    QString grupta_kayitli_uye_sayisi_column_name;
    QString alt_grup_sayisi_column_name;
    QString program_id_column_name;
    QString modul_id_column_name;
};

class BASE_GRUP_KERNEL {

public:
    BASE_GRUP_KERNEL                 ();
    ~BASE_GRUP_KERNEL                ();

    GRUPLAR_DB_INFO_STRUCT M_GRUPLAR_DB_INFO;

    void                 SET_DELIMETER                               (QString delimeter);
    void                 SET_GRUPLAR_TABLE_NAME                      (QString column_name);
    void                 SET_GRUPLAR_ID_COLUMN_NAME                  (QString column_name);
    void                 SET_GRUPLAR_PARENT_ID_COLUMN_NAME           (QString column_name);
    void                 SET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME        (QString column_name);
    void                 SET_GRUPLAR_GRUP_KODU_COLUMN_NAME           (QString column_name);
    void                 SET_GRUPLAR_GRUP_ADI_COLUMN_NAME            (QString column_name);
    void                 SET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME       (QString column_name);
    void                 SET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME       (QString column_name);
    void                 SET_GRUPLAR_UYE_SAYISI_COLUMN_NAME          (QString column_name);
    void                 SET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME     (QString column_name);
    void                 SET_GRUPLAR_PROGRAM_ID_COLUMN_NAME          (QString column_name);
    void                 SET_GRUPLAR_MODUL_ID_COLUMN_NAME            (QString column_name);

    QString              GET_DELIMETER                               ();
    QString              GET_GRUPLAR_TABLE_NAME                      ();
    QString              GET_GRUPLAR_ID_COLUMN_NAME                  ();
    QString              GET_GRUPLAR_PARENT_ID_COLUMN_NAME           ();
    QString              GET_GRUPLAR_GRUP_SEVIYESI_COLUMN_NAME       ();
    QString              GET_GRUPLAR_GRUP_KODU_COLUMN_NAME           ();
    QString              GET_GRUPLAR_GRUP_ADI_COLUMN_NAME            ();
    QString              GET_GRUPLAR_TAM_GRUP_KODU_COLUMN_NAME       ();
    QString              GET_GRUPLAR_SON_SEVIYEMI_COLUMN_NAME        ();
    QString              GET_GRUPLAR_UYE_SAYISI_COLUMN_NAME          ();
    QString              GET_GRUPLAR_ALT_GRUP_SAYISI_COLUMN_NAME     ();
    QString              GET_GRUPLAR_PROGRAM_ID_COLUMN_NAME          ();
    QString              GET_GRUPLAR_MODUL_ID_COLUMN_NAME            ();

    void                 UPDATE_GRUP_GUI_EKLENEN_KAYIT_SATIR         (QStringList column_name);
    void                 UPDATE_GRUP_GUI_GUNCELLENEN_SATIR           (QStringList column_name);
    void                 UPDATE_GRUP_GUI_GUNCELLENEN_UST_GRUP        (QStringList column_name);
    void                 RESET_GUI_UPDATE_VALUES                     ();
    void                 LOAD_DEFAULT_DB_INFO                        ();
    void                 CREATE_SELECT_QUERY                         (QString * table_name , QString * column_name , QString *  where_str , QString * order_by ,int parent_id = 0,int item_id = -1);


    int                  GRP_GET_MODUL_ID                            ();
    int                  GRP_GET_PROGRAM_ID                          ();
    void                 GRP_RESET_STATIC_VARIABLES                  ();

    bool                 m_grubu_parentine_aktar;

    bool                 m_grup_satir_eklendi_mi;
    bool                 m_grup_satir_guncellendi_mi;
    QStringList          m_grup_eklenen_kayit;
    QStringList          m_grup_guncellenen_satir;
    QStringList          m_grup_guncellenen_ust_grup;

protected:
    void                 GRP_SET_VISIBLE_PROG_MODULE                 (int program_id , int module_id);

    QList<int>           m_visible_modules;
    QString              m_delimeter;
    int                  m_program_id;

};

#endif // BASE_GRUP_KERNEL_H
