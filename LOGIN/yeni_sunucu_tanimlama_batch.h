#ifndef YENI_SUNUCU_TANIMLAMA_BATCH_H
#define YENI_SUNUCU_TANIMLAMA_BATCH_H

#include "batch_kernel.h"

class PROGRAM_CORE;
struct USER_LOGIN_INFO_STRUCT;

namespace Ui {
    class YENI_SUNUCU_TANIMLAMA_BATCH;
}

class YENI_SUNUCU_TANIMLAMA_BATCH : public BATCH_KERNEL
{
    Q_OBJECT

public:
    YENI_SUNUCU_TANIMLAMA_BATCH   (bool yeni_profil , USER_LOGIN_INFO_STRUCT * user_informations,
                          bool * run_batch_clicked, QWidget * parent = 0 );
    ~YENI_SUNUCU_TANIMLAMA_BATCH  () {}

private:

    USER_LOGIN_INFO_STRUCT *m_user_info;

    bool                   *m_run_batch_click;
    bool                    m_yeni_profil;
    QString                 m_kayitli_profil_ismi;
    Ui::YENI_SUNUCU_TANIMLAMA_BATCH *m_ui;

    void  SETUP_FORM          ();
    int   CHECK_VAR           ( QObject * object );
    int   CHECK_RUN           ();
    int   CHECK_EXIT          ();
    void  RUN_BATCH           ();
    void  ORGANIZE_SQL_VALUES ();
    void  ORGANIZE_SQL_WIDGETS();
    void  KAYITLI_PROFILI_SIL (bool kaydi_sil_button_clicked = false);

    void  SET_UYARI_LABEL     ();
    void  FILL_SELECTED_SQL   (QString sql_motoru);

    void  OTOMATIK_SIFRE_KONTROLU();

    QString m_argumanli_label_txt;
    void  SQL_UYARISINI_GUNCELLE( DB_DRIVER p_sql_motoru );

};

#endif // YENI_SUNUCU_TANIMLAMA_BATCH_H
