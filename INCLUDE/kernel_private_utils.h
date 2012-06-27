#ifndef KERNEL_PRIVATE_UTILS_H
#define KERNEL_PRIVATE_UTILS_H

#include <QTableWidget>
#include <QToolButton>
#include <QLayout>
#include <QLabel>
#include <QTreeView>


////////////// SEARCH VE SECIM UTILS

QList<int>                  KERNEL_TABLE_SELECTED_ROWS              (QTableWidget * p_table_widget);
QToolButton *               KERNEL_CREATE_SEC_BUTTON                (QLayout * buttonun_eklenecegi_layout,bool is_multi_select);
void                        KERNEL_SET_TABLE_CHECK_BOX_COLUMN       (QTableWidget * p_table_widget,bool is_multi_select,int num_of_columms);
void                        KERNEL_SET_TABLE_SELECTION_MODE         (QTableWidget * p_table_widget,bool is_multi_select);
QLabel *                    KERNEL_CREATE_STATUS_LABEL              (QLayout * labelin_eklenecegi_layout);
void                        KERNEL_UPDATE_STATUS_LABEL              (QLabel * label,int bulunan_kayit_sayisi);


////////////// BUTTONS

QToolButton *               KERNEL_CREATE_DEFAULT_HELP_BUTTON       (QWidget * parent);

///////////// TREE KERNEL

void                        KERNEL_UPDATE_TREE_VIEW                 (QTreeView * tree_view);

#endif // KERNEL_PRIVATE_UTILS_H
