#include "choosechip.h"
#include "ui_choosechip.h"
#include "mainwindow.h"

extern "C" {
#include "libflashrom.h"
}

ChooseChip::ChooseChip(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ChooseChip)
{
        ui->setupUi(this);
}

ChooseChip::~ChooseChip()
{
        delete ui;
}

void ChooseChip::add_chip(QString chip_name)
{
        ui->chip_list->addItem(chip_name);
}

void ChooseChip::on_b_chip_ok_clicked()
{
    fl_flash_probe(w->get_flash_context_ptr(), ui->chip_list->currentItem()->text().toStdString().c_str());
}
