#include "choosechip.h"
#include "ui_choosechip.h"

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
