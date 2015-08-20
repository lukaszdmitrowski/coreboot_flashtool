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

#include "preferences.h"
#include "ui_preferences.h"
#include "mainwindow.h"

#include <QTextStream>
#include <QFileDialog>
#include <QFile>

Preferences::Preferences(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Preferences)
{
        ui->setupUi(this);
        ui->edit_cor_path->setText(w->coreboot_dir);
}

Preferences::~Preferences()
{
        delete ui;
}

void Preferences::on_b_sel_cor_path_clicked()
{
        QString coreboot_dir = QFileDialog::getExistingDirectory(this,
                                                                tr("Select coreboot dir"),
                                                                ".",
                                                                QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);
        coreboot_dir.append("/");
        ui->edit_cor_path->setText(coreboot_dir);
        w->coreboot_dir = coreboot_dir;
}

void Preferences::on_b_ok_clicked()
{
        QFile config_file("preferences.cfg");
        QString new_coreboot_path = "coreboot_path: " + ui->edit_cor_path->text();

        if (config_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
                QTextStream config_stream(&config_file);
                config_stream << new_coreboot_path;
        } else {
                w->info_dialog->show_message(ERR_FILE_CONFIG);
        }
        close();
}
