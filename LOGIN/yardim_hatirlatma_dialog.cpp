#include "yardim_hatirlatma_dialog.h"
#include "ui_yardim_hatirlatma_dialog.h"


/*******************************************************************************
                        SHOW_YARDIM_HATIRLATMA_DIALOG
********************************************************************************/

void SHOW_YARDIM_HATIRLATMA_DIALOG()
{
    YARDIM_HATIRLATMA_DIALOG * F = new YARDIM_HATIRLATMA_DIALOG;
    F->exec();
}

/*******************************************************************************
            YARDIM_HATIRLATMA_DIALOG::YARDIM_HATIRLATMA_DIALOG
********************************************************************************/

YARDIM_HATIRLATMA_DIALOG::YARDIM_HATIRLATMA_DIALOG(QWidget *parent) : QDialog(parent), ui(new Ui::YARDIM_HATIRLATMA_DIALOG)
{
    ui->setupUi(this);

    connect( ui->push_button_baslat, SIGNAL( clicked() ),this,SLOT( close() ));
}

/*******************************************************************************
        YARDIM_HATIRLATMA_DIALOG::YARDIM_HATIRLATMA_DIALOG
********************************************************************************/

YARDIM_HATIRLATMA_DIALOG::~YARDIM_HATIRLATMA_DIALOG()
{
    delete ui;
}
