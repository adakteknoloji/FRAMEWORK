#ifndef TREE_ARAMA_KERNEL_H
#define TREE_ARAMA_KERNEL_H

#include <QTreeView>
#include <QAbstractButton>
#include <QEvent>
#include <QCloseEvent>
#include <QDialog>
#include <QStatusBar>
#include <QToolButton>
#include <QHBoxLayout>
#include <adak_sql.h>
#include "base_event_kernel.h"

class TREE_ARAMA_KERNEL : public BASE_EVENT_KERNEL
{
    Q_OBJECT

public:
    TREE_ARAMA_KERNEL(QWidget * parent = 0);
    virtual ~TREE_ARAMA_KERNEL();

protected:
    int                         ADD_NEW_ROW                      ();
    QTreeView *                 GET_TREE_VIEW                    ();
    void                        SET_WINDOW_SIZE                  (int height, int width);
    void                        INIT_KERNEL                      (QTreeWidget * tree_widget , QObject * parent,ADAK_SQL * database);

    void                        SET_RIGHT_ALIGN_COLUMNS          (QList<int> list);

    //Secim kernelinda kullanilan table ki verileri yenilemek icin kullanilir.
    void                        REFRESH_TREE_WIDGET              ();

    //Bu fonksiyonla table widgetin altina buttonlar eklenebilir.Button text gonderdigimizde fonksiyon
    //bize o button text e sahip push button in adresini dondurur.Bunu kullanarak DOUBLE_CLICKED fonksiyonunda
    //hangi buttona tiklandigini bulabiliriz.Icon ve size bilgileri istenirse gonderilebilir.Default olarak
    //Gelecek icon ekle ikonudur.Default size da layouta uygun olarak ayarlanir.
    QToolButton *               ADD_BUTTON                       (QString button_text, Qt::ToolButtonStyle style = Qt::ToolButtonTextUnderIcon,
                                                                      QString icon = ":/kernel_icons/kernel_ekle_ikonu.png",int size_height=-1,int size_width=-1);
    virtual void                BUTTON_CLICKED                   (QAbstractButton * button,QStringList selected_item);

    //Bu fonksiyonla sonuclardan secilen degerler programciya doner.Eger programci ADAK_CONTINUE dondururse
    //kernel calismaya devam eder aksi takdirde ADAK_EXIT dondururse program sonlanir.
    virtual int                 DOUBLE_CLICKED                   (QStringList selected_item) = 0;

    //virtual olan bu fonksiyon programciya secilen row un idsini gonderir.
    virtual void                SELECTED_LINE                    (QStringList selected_item);

    void                        REGISTER_TREE_WIDGET             (QTreeView * tree_view);
    void                        SET_COLUMN_HEADERS               (QStringList column_headers);
    void                        SET_ID_COLUMN_NUM                (int column);

    virtual void                ADD_CHILD_ITEMS                  (QStringList selected_item)      = 0;

    void                        ADD_PARENT_ITEM                  (QStringList column_datas,int number_of_child);
    void                        ADD_CHILD_ITEM                   (QStringList column_datas,int number_of_child);
    void                        UPDATE_SELECTED_ITEM             (QStringList column_datas);

    //Bu fonksiyonla gonderilen QStringList i kullanarak istedigimiz columndaki degerleri alabiliriz.
    //Mesela kullanici double click lediginde bir row a DOUBLE_CLICKED fonksiyonuna kernel tarafindan otomatik olarak QStringList gonderilir.
    //GET_SELECTED_VALUE ya bu item i ve o satirdaki erismek istedigimiz degeri tutan columni gonderirsek kernel bize o degeri dondurur.
    //Burda dikkat edilmesi gereken gelen deger QVariant tir bu yuzden mevcut deger e gore cast edilmelidir.
    QVariant                    GET_SELECTED_VALUE               (QStringList * row_item, int column);

    //Bu fonksiyonla istedigimiz column i hide layabiliriz.
    void                        HIDE_COLUMN                      (int column_name);


    QToolButton *               run_search_button;
    virtual bool                eventFilter                      (QObject * obj, QEvent * event);
    void                        SET_ENTER_KEY_FOR_SEARCH         (bool enable);
    void                        REGISTER_BUTTON_WIDGET           (QWidget* search_buttons_widget,bool is_yardim_button_visible = true);
    void                        SET_NAME_OF_SEARCH_BUTTON        (QString name_of_start_button);

    void                        SET_ICON_OF_SEARCH_BUTTON        (QString filename);

    void                        RESET_FOCUS                      ();

    virtual int                 CHECK_EMPTY                      ()                                 = 0;
    virtual void                SEARCH                           ()                                 = 0;
    void                        PROCESS_CHECK_VAR_RETURN_VALUE   (int return_value);

    virtual int                 SINGLE_LINE_SELECTED             ( QStringList        column_datas ) = 0;

    //Sirasi ile QList secilen rowlari verir.
    //Yani selected_datas.at(0)      -> 0. Secili olan(!!!) ilk Row bilgisi
    //Yani selected_datas.at(0).at(1)-> Secili olan(!!!) ilk rowun 1. Column daki degeri

    virtual int                 MULTI_LINE_SELECTED              ( QList<QStringList> selected_datas );

private:
    QStringList *               p_selected_item;
    int                         p_height;
    int                         p_width;
    int                         m_secilen_check_box_column;
    QHBoxLayout *               p_h_box_layout;
    QTreeWidget *               m_tree_widget;
    QStringList                 m_headers;
    QString                     p_window_title;
    QList<QTreeWidgetItem * >   m_selected_items;
    QTreeWidgetItem *           m_last_expanded_item;

    QString                     p_name_of_run_search_button;
    QString                     p_icon_file_name;
    bool                        m_is_multi_row;
    bool                        p_run_search_on_enter;
    bool                        close_event_started;

    void                        SET_TREE_WIDGET_DEFAULTS          ();
    QStringList                 GET_COLUMN_DATAS_FROM_ITEM        (QTreeWidgetItem * item);


private slots:
    void                        FIND_SELECTED_ROW                 ();
    void                        SLOT_ROW_CLICKED                  ();
    void                        SLOT_BUTTON_CLICKED               ();
    void                        ITEM_EXPANDED                     (QTreeWidgetItem * item);
    void                        ITEM_COLLAPSED                    (QTreeWidgetItem * item);
    void                        SLOT_SEARCH_BUTTON                ();
};

#endif // TREE_ARAMA_KERNEL_H
