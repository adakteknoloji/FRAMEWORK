#ifndef GRUP_SECIM_KERNEL_H
#define GRUP_SECIM_KERNEL_H

#include <QtGui/QDialog>
#include <tree_secim_kernel.h>
#include <QToolButton>
#include "base_grup_kernel.h"

class GRUP_SECIM_KERNEL : public TREE_SECIM_KERNEL , public BASE_GRUP_KERNEL {
    Q_OBJECT

public:
   GRUP_SECIM_KERNEL                                    ( int program_id , int module_id ,bool * ids_changed , ADAK_SQL * db = NULL , QWidget * parent = 0 , bool ana_gruplar_secilebilir_mi = false );
    ~GRUP_SECIM_KERNEL                                  ( );

    void                        SET_SECILEN_ID_PTR      (int *        secilen_id);
    //Multirow kullanilacaksa QList gonderilmeli.
    void                        SET_SECILEN_ID_PTR      (QList<int> * secilen_idler);

    void                        INIT_GRUP_SECIM_KERNEL  ( QWidget * parent,ADAK_SQL * db );

protected:
    virtual int                 SINGLE_LINE_SELECTED    (QStringList        selected_item);
    virtual int                 MULTI_LINE_SELECTED     (QList<QStringList> selected_items);
    virtual void                SELECTED_LINE           (QStringList column_datas);

private:
    ADAK_SQL *                 m_grup_secim_db;
    QToolButton *               m_grup_ekle_button;

    int                         m_id_column;
    int *                       m_secilen_id;
    QList<int> *                m_secilen_idler;
    QList<int>                  m_copy_secilen_idler;

    bool *                      m_ids_changed;

    void                        ADD_CHILD_ITEMS         ( QStringList  item);

    void                        ADD_ITEM                ( QObject * button, int item_id );
    void                        FILL_TREE_TABLE         ();

    //Eger ekrana button eklenmek istenirse tree secim kernel dan gelen fonksiyonlar kullanilabilir.
    // BUTTON_CLICKED gibi
    //void                        BUTTON_CLICKED    (QAbstractButton *button, QStringList *selected_item);

};

#endif // GRUP_SECIM_KERNEL_H
