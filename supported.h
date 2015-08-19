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

#ifndef SUPPORTED_H
#define SUPPORTED_H

#include <QDialog>
#include <QStandardItemModel>

#include "multisortfiltermodel.h"

extern "C" {
#include "libflashrom.h"
}

namespace Ui {
class Supported;
}

class Supported : public QDialog
{
        Q_OBJECT

public:
        explicit Supported(QWidget *parent = 0);
        ~Supported();

private slots:
        void show_flash_chips();
        void show_boards();
        void show_chipsets();

        void on_cb_sel_hardware_currentIndexChanged(int index);
        void on_cb_sel_vendor_currentIndexChanged();
        void on_edit_name_textChanged(const QString &arg1);
        void on_cb_sel_custom_currentIndexChanged();

private:
        QString test_state_to_qstring(fl_test_state test_state);
        Ui::Supported *ui;
        QStandardItemModel *model;
        MultiSortFilterModel *sortFilterModel;
        fl_flashchip_info *flashchip_info;
        fl_board_info *boards_list;
        fl_chipset_info *chipsets_list;
        QStringList vendor_list;
        QStringList custom_list;
};

#endif // SUPPORTED_H
