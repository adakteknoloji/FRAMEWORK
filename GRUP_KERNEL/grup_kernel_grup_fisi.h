#ifndef GRUP_KERNEL_GRUP_FISI_H
#define GRUP_KERNEL_GRUP_FISI_H

#include <QDialog>
#include <fis_kernel.h>
#include "base_grup_kernel.h"
#include "ui_grup_kernel_grup_fisi.h"
#include "adak_sql.h"

enum ISLEM_TURU {
    ENUM_ANA_GRUP = 0,
    ENUM_ALT_GRUP = 1
};

class GRUP_KERNEL_GRUP_FISI : public FIS_KERNEL , public Ui::GRUP_KERNEL_GRUP_FISI
{
    Q_OBJECT;

public:

     GRUP_KERNEL_GRUP_FISI    ( int parent_id, ISLEM_TURU p_islem_turu, BASE_GRUP_KERNEL * grup_base_grup,QWidget * parent = 0,ADAK_SQL * db = NULL);
    ~GRUP_KERNEL_GRUP_FISI    (){}

private:

    ADAK_SQL *             grup_ekleme_db;

    BASE_GRUP_KERNEL *      m_base_grup_kernel_ptr;

    int                     m_grup_id;

    ISLEM_TURU              m_islem_turu;

    QString                 p_delimeter;
    int                     p_num_of_columns;
    QString                 p_ust_tam_grup_kodu;
    int                     p_ust_grup_seviyesi;
    bool                    p_grubun_parentini_aktar;

    int                     m_modul_id;
    int                     m_program_id;
    int                     m_grup_parent_id;
    int                     m_grup_seviyesi;

    void                    SETUP_FORM                               ();
    void                    NEW_FIS_RECORD                           ();
    void                    CLEAR_FORM_MEMBERS                       ();
    int                     GET_FIS_RECORD                           (int fis_id);

    int                     CHECK_FIS_FORM_VAR                       (QObject * object);
    int                     CHECK_FIS_FORM_EMPTY                     ();

    int                     CHECK_FIS_RECORD_ADD                     ();
    int                     ADD_FIS_RECORD                           ();
    int                     CHECK_FIS_RECORD_UPDATE                  (int fis_id);
    void                    UPDATE_FIS_RECORD                        (int fis_id);
    int                     CHECK_FIS_RECORD_DELETE                  (int fis_id);
    void                    DELETE_FIS_RECORD                        (int fis_id);

    int                     SELECT_FIS_RECORD                        ();
    int                     FIND_FIS_RECORD                          ();
    int                     FIND_NEXT_FIS_RECORD                     ();
    int                     FIND_PREV_FIS_RECORD                     ();
    int                     FIND_FIRST_FIS_RECORD                    ();
    int                     FIND_LAST_FIS_RECORD                     ();

    int                     LOCK_FIS_RECORD                          (int fis_id);
    void                    UNLOCK_FIS_RECORD                        (int fis_id);

    void                    SET_LINE_DEFAULTS                        ( int row_number );
    int                     CHECK_LINE_VAR                           ( int row_number, QObject * object );
    int                     CHECK_LINE_EMPTY                         ( int row_number );
    int                     CHECK_ADD_LINE                           ( int fis_id,    int row_number );
    void                    ADD_LINE                                 ( int fis_id,    int row_number );
    int                     CHECK_UPDATE_LINE                        ( int fis_id,    int row_number );
    void                    UPDATE_LINE                              ( int fis_id,    int row_number );
    int                     CHECK_DELETE_LINE                        ( int fis_id,    int row_number );
    void                    DELETE_LINE                              ( int fis_id,    int row_number );
    void                    REORGANIZE_ORDER_NUMBER                  ( int row_number,int new_order_number);

protected:
    void                    LINE_SAVER_BUTTON_CLICKED                (QAbstractButton *button, int row_id, int record_id, int row_number);

};

bool F_GRUP_KERNEL_GRUP_FISI( int parent_id, ISLEM_TURU p_islem_turu,  BASE_GRUP_KERNEL * base_grup,QWidget * parent ,ADAK_SQL * db);

#endif // GRUP_KERNEL_GRUP_FISI_H
