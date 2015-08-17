/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Lukasz Dmitrowski <lukasz.dmitrowski@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

#include "supported.h"
#include "ui_supported.h"

#include "memory.h"

#include <QDebug>

Supported::Supported(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Supported)
{
        ui->setupUi(this);
        model = new QStandardItemModel(this);
        sortFilterModel= new MultiSortFilterModel(this);
        sortFilterModel->setSourceModel(model);

        QList<qint32> filter_columns;
        filter_columns.append(0);
        filter_columns.append(1);
        filter_columns.append(2);
        filter_columns.append(3);
        filter_columns.append(4);
        sortFilterModel->setFilterKeyColumns(filter_columns);
        flashchip_info = fl_supported_flash_chips();
        boards_list = fl_supported_boards();
        chipsets_list = fl_supported_chipsets();
        ui->cb_sel_hardware->addItem("Chips");
        ui->cb_sel_hardware->addItem("Boards");
        ui->cb_sel_hardware->addItem("Chipsets");
}

Supported::~Supported()
{
        fl_data_free(flashchip_info);
        fl_data_free(boards_list);
        fl_data_free(chipsets_list);
        delete ui;
}

void Supported::show_flash_chips()
{
        QList<qint16> size_list_numbers;

        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Test OK")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Broken")));
        model->setHorizontalHeaderItem(4, new QStandardItem(QString("Size [kB]")));

        for (unsigned int i = 0; flashchip_info[i].name; ++i) {
                QList<QStandardItem*> flashchip_row;
                QString tested;
                QString known_broken;

                if (flashchip_info[i].tested.probe == FL_TESTED_OK)
                        tested.append("P");
                else if (flashchip_info[i].tested.probe == FL_TESTED_NA)
                        tested.append("-");
                else
                        tested.append(" ");
                if (flashchip_info[i].tested.read == FL_TESTED_OK)
                        tested.append("R");
                else if (flashchip_info[i].tested.read == FL_TESTED_NA)
                        tested.append("-");
                else
                        tested.append(" ");
                if (flashchip_info[i].tested.erase == FL_TESTED_OK)
                        tested.append("E");
                else if (flashchip_info[i].tested.erase == FL_TESTED_NA)
                        tested.append("-");
                else
                        tested.append(" ");
                if (flashchip_info[i].tested.write == FL_TESTED_OK)
                        tested.append("W");
                else if (flashchip_info[i].tested.write == FL_TESTED_NA)
                        tested.append(" ");

                if (flashchip_info[i].tested.probe == FL_TESTED_BAD)
                        known_broken.append("P");
                else
                        known_broken.append(" ");
                if (flashchip_info[i].tested.read == FL_TESTED_BAD)
                        known_broken.append("R");
                else
                        known_broken.append(" ");
                if (flashchip_info[i].tested.erase == FL_TESTED_BAD)
                        known_broken.append("E");
                else
                        known_broken.append(" ");
                if (flashchip_info[i].tested.write == FL_TESTED_BAD)
                        known_broken.append("W");
                else
                        known_broken.append(" ");

                if (!vendor_list.contains(flashchip_info[i].vendor))
                        vendor_list.append(flashchip_info[i].vendor);

                if (custom_list.isEmpty()) {
                        if (!size_list_numbers.contains(flashchip_info[i].total_size))
                                size_list_numbers.append(flashchip_info[i].total_size);
                }

                flashchip_row.append(new QStandardItem(QString(flashchip_info[i].vendor)));
                flashchip_row.append(new QStandardItem(QString(flashchip_info[i].name)));
                flashchip_row.append(new QStandardItem(tested));
                flashchip_row.append(new QStandardItem(known_broken));
                flashchip_row.append(new QStandardItem(QString::number(flashchip_info[i].total_size)));
                model->appendRow(flashchip_row);
        }

        vendor_list.sort();
        qSort(size_list_numbers);

        foreach (qint16 num, size_list_numbers)
                custom_list.append(QString::number(num));

        ui->cb_sel_vendor->addItems(vendor_list);
        ui->cb_sel_custom->addItems(custom_list);
        ui->l_custom->setText("Size");

        ui->tableView->setModel(sortFilterModel);
        ui->tableView->setColumnWidth(0, 155);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 70);
        ui->tableView->setColumnWidth(3, 70);
        ui->tableView->setColumnWidth(4, 70);
}

void Supported::show_boards()
{
        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Board")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));

        for (unsigned int i = 0; boards_list[i].vendor; ++i) {
                QList<QStandardItem*> board_row;
                QString status;

                status = test_state_to_qstring(boards_list[i].working);
                if (!vendor_list.contains(boards_list[i].vendor))
                        vendor_list.append(boards_list[i].vendor);

                if (!custom_list.contains(status))
                                custom_list.append(status);

                board_row.append(new QStandardItem(QString(boards_list[i].vendor)));
                board_row.append(new QStandardItem(QString(boards_list[i].name)));
                board_row.append(new QStandardItem(status));
                model->appendRow(board_row);
        }

        ui->cb_sel_vendor->addItems(vendor_list);
        ui->cb_sel_custom->addItems(custom_list);
        ui->l_custom->setText("Status");

        ui->tableView->setModel(model);
        ui->tableView->setColumnWidth(0, 155);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 70);
}

void Supported::show_chipsets()
{
        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Chipset")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("PCI IDs")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Status")));

        for (unsigned int i = 0; chipsets_list[i].vendor; ++i) {
                QList<QStandardItem*> chipset_row;
                QString status;

                status = test_state_to_qstring(boards_list[i].working);
                if (!vendor_list.contains(chipsets_list[i].vendor))
                        vendor_list.append(chipsets_list[i].vendor);

                if (!custom_list.contains(status))
                                custom_list.append(status);

                chipset_row.append(new QStandardItem(QString(chipsets_list[i].vendor)));
                chipset_row.append(new QStandardItem(QString(chipsets_list[i].chipset)));
                chipset_row.append(new QStandardItem(QString("xxxx::xxxx")));
                chipset_row.append(new QStandardItem(status));
                model->appendRow(chipset_row);
        }

        ui->cb_sel_vendor->addItems(vendor_list);
        ui->cb_sel_custom->addItems(custom_list);
        ui->l_custom->setText("Status");

        ui->tableView->setModel(model);
        ui->tableView->setColumnWidth(0, 155);
        ui->tableView->setColumnWidth(1, 350);
        ui->tableView->setColumnWidth(2, 90);
        ui->tableView->setColumnWidth(3, 140);
}

QString Supported::test_state_to_qstring(fl_test_state test_state)
{
        switch (test_state) {
        case FL_TESTED_OK:
                return QString("OK");
        case FL_TESTED_BAD:
                return QString("Not working");
        case FL_TESTED_NA:
                return QString("N/A");
        case FL_TESTED_DEP:
                return QString("Config-dependent");
        case FL_TESTED_NT:
        default:
                return QString("Untested");
        }
}

void Supported::on_cb_sel_hardware_currentIndexChanged(int index)
{
        vendor_list.clear();
        custom_list.clear();
        ui->cb_sel_vendor->clear();
        ui->cb_sel_custom->clear();
        ui->cb_sel_vendor->addItem(QString());
        ui->cb_sel_custom->addItem(QString());

        switch (index) {
        case 0:
                show_flash_chips();
                break;
        case 1:
                show_boards();
                break;
        case 2:
                show_chipsets();
                break;
        }
}

void Supported::on_cb_sel_vendor_currentIndexChanged(int index)
{
        sortFilterModel->setFilter(0, ui->cb_sel_vendor->currentText());
        sortFilterModel->setSourceModel(model);
        ui->tableView->setModel(sortFilterModel);
}

void Supported::on_edit_name_textChanged(const QString &arg1)
{
        sortFilterModel->setFilter(1, arg1);
        sortFilterModel->setSourceModel(model);
        ui->tableView->setModel(sortFilterModel);
}

void Supported::on_cb_sel_custom_currentIndexChanged(int index)
{
        if (ui->cb_sel_hardware->currentText() == "Chips")
                sortFilterModel->setFilter(4, ui->cb_sel_custom->currentText());
        if (ui->cb_sel_hardware->currentText() == "Boards")
                sortFilterModel->setFilter(2, ui->cb_sel_custom->currentText());
        if (ui->cb_sel_hardware->currentText() == "Chipsets")
                sortFilterModel->setFilter(3, ui->cb_sel_custom->currentText());

        sortFilterModel->setSourceModel(model);
        ui->tableView->setModel(sortFilterModel);
}
