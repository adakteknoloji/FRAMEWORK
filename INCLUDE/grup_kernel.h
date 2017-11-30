#ifndef GRUP_KERNEL_H
#define GRUP_KERNEL_H

#include <QDialog>
#include <tree_kernel.h>
#include "base_grup_kernel.h"

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
    INIT_GRUP_KERNEL( parent, DB );
*/

namespace Ui {
    class GRUP_KERNEL;
}


class GRUP_KERNEL : public TREE_KERNEL , public BASE_GRUP_KERNEL {
    Q_OBJECT
public:
    GRUP_KERNEL                 (int program_id , int module_id , QString p_win_title, QString p_help_page, ADAK_SQL * db, QWidget * parent);
    ~GRUP_KERNEL                ();

protected:
    void                        INIT_GRUP_KERNEL                            (QWidget * parent,ADAK_SQL * db);

private:

    Ui::GRUP_KERNEL *           m_ui;

    int                         m_grp_kodu_column;
    int                         m_grp_isim_column;
    int                         m_grp_alt_grup_sayisi_column;
    int                         m_grp_kayit_sayisi_column;
    int                         m_grp_yaratildigi_modul_column;
    int                         m_grp_id_column;
    int                         m_grp_parent_id_column;
    int                         m_grp_son_seviye_mi_column;
    int                         m_grp_tam_grup_kodu_column;

    QString                     m_select_query;


    ADAK_SQL *                 m_db;

    void                        ADD_CHILD_ITEMS             ( QStringList item );
    void                        ADD_PARENT_ITEMS            ();
    void                        ADD_ITEM                    ( QObject * button, QStringList item);
    void                        UPDATE_ITEM                 ( QStringList item );
    int                         CHECK_DELETE_ITEM           ( QStringList item );
    void                        DELETE_ITEM                 ( QStringList item );
};

#endif // GRUP_KERNEL_H











