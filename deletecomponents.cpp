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

#include "deletecomponents.h"
#include "ui_deletecomponents.h"
#include "mainwindow.h"

extern "C" {
#include "libcbfstool.h"
}

DeleteComponents::DeleteComponents(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DeleteComponents)
{
        ui->setupUi(this);
}

DeleteComponents::~DeleteComponents()
{
        delete ui;
}

void DeleteComponents::on_b_remove_comp_clicked()
{
        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) */
        int param_count = 3;

        QString params[7];
        params[0] = "flash_tool";
        params[1] = w->get_cbfs_rom_path();
        params[2] = "remove";

        if (!ui->edit_name->text().isEmpty()) {
                params[param_count] = "-n";
                params[param_count + 1] = ui->edit_name->text();
                param_count += 2;
        }

        cbfs_params = new char*[param_count];
        for (int i = 0; i < param_count; ++i) {
                cbfs_params[i] = new char[params[i].length() + 1];
                strcpy(cbfs_params[i], params[i].toStdString().c_str());
        }

        start_cbfs(param_count, cbfs_params);

        for (int i = 0; i < param_count; ++i) {
                delete [] cbfs_params[i];
        }
        delete [] cbfs_params;
        close();
}
