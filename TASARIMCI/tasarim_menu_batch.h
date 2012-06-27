#ifndef TASARIM_MENU_BATCH_H
#define TASARIM_MENU_BATCH_H

#include "batch_kernel.h"
#include "belge_struct.h"

namespace Ui {
    class TASARIM_MENU_BATCH;
}

class TASARIM_MENU_BATCH : public BATCH_KERNEL
{
    Q_OBJECT


public:
              TASARIM_MENU_BATCH  ( ADAK_BELGELER_STRUCT * p_belge_struct, int p_belge_count, QWidget * parent = 0 );
              ~TASARIM_MENU_BATCH () {}
private:

    Ui::TASARIM_MENU_BATCH *  m_ui;

    int                     m_belge_count;
    ADAK_BELGELER_STRUCT * m_belge_struct;

    int                     m_last_belge_id;
    int                     m_tasarim_id;



    //! METHOD
    void      SETUP_FORM    ();
    int       CHECK_VAR     ( QObject * object );
    int       CHECK_RUN     ();
    int       CHECK_EXIT    ();
    void      RUN_BATCH     ();

    void      FILL_ALT_MENU ( int p_belge_id );

private slots:

    void      SLOT_ALT_TASARIM_DOUBLE_CLICKED( int p_row, int p_colomn );
    void      SLOT_TASARIM_CHANGE            ( QTableWidgetItem* p_current_item, QTableWidgetItem* p_previous_item );
    void      SLOT_YENI_BELGE                ( int p_row, int p_colomn  );
    void      SLOT_TASARIMI_SIL              ();

};

#endif // TASARIM_MENU_BATCH_H
