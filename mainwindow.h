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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QStandardItemModel>

#include "infodialog.h"
#include "progressdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    QTextEdit *active_log_out;
    QString get_cbfs_rom_path();
    QString chip_name;
    bool chip_found;
    bool programmer_initialized;
    InfoDialog *info_dialog;
    ProgressDialog *progress_dialog;

private slots:
    /* Main window */
    void on_b_init_prog_clicked();

    /* Toolbar */
    void on_act_supported_list_triggered();
    void on_act_about_triggered();

    /* Auto tab */
    void on_b_auto_get_hw_data_clicked();
    void on_b_auto_build_img_clicked();
    void on_b_auto_get_bios_clicked();
    void on_b_auto_flash_clicked();

    /* Flash tab */
    void on_b_read_clicked();
    void on_b_probe_clicked();
    void on_b_verify_clicked();
    void on_b_erase_clicked();
    void on_b_flash_clicked();

    /* Extract tab */
    void on_b_sel_bios_rom_clicked();
    void on_b_sel_bios_out_clicked();
    void on_b_extract_clicked();

    /* Create rom tab */
    void on_b_create_rom_clicked();
    void on_b_sel_boot_block_clicked();

    /* Rom options tab */
    void on_b_ropt_select_clicked();
    void on_b_add_component_clicked();
    void on_b_remove_comp_clicked();

    /* Other */
    void on_tabWidget_currentChanged(int index);

private:
    void fill_cb_programmers();
    void fill_cb_arch();
    void fill_cb_payload();
    void open_select_bios_rom_window();
    void open_select_bios_out_window();
    QString cbfs_rom_path;
    QString bios_rom_path;
    QString bootblock_path;
    QStandardItemModel *model;
};

extern MainWindow *w;

#endif // MAINWINDOW_H
