#include "choosechip.h"
#include "ui_choosechip.h"
#include "mainwindow.h"

#include <QDebug>

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

void ChooseChip::set_flash_ctx_ptr(fl_flashctx_t **ctx_ptr)
{
        flash_context_ptr = ctx_ptr;
}

void ChooseChip::on_b_chip_ok_clicked()
{
        QString chosen_chip = ui->chip_list->currentItem()->text();
        w->chip_name = chosen_chip;
        if (fl_flash_probe(flash_context_ptr, chosen_chip.toStdString().c_str()) == 0) {
                qDebug() << "Chip found!";
        } else {
                qDebug() << "Probing failed!";
        }
        close();
}
