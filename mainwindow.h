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

extern "C" {
#include "libflashrom.h"
}

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
    QTextEdit *cbfs_log_out;
    QString get_flash_rom_path();

private slots:
    void on_b_sel_payload_clicked();
    void on_b_read_clicked();
    void on_act_sel_payload_triggered();
    void on_act_supported_list_triggered();
    void on_act_about_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_cb_sel_progr_activated(const QString &programmer);

    void on_b_sel_bios_rom_clicked();
    void on_b_sel_bios_out_clicked();
    void on_b_extract_clicked();
    void on_b_create_rom_clicked();
    void on_b_sel_boot_block_clicked();
    void on_b_add_payload_clicked();
    void on_b_add_component_clicked();
    void on_b_remove_comp_clicked();
    void on_b_probe_clicked();

private:
    void fill_cb_programmers();
    void fill_cb_arch();
    void open_select_rom_window();
    void open_select_bios_rom_window();
    void open_select_bios_out_window();
    int extract_bios();
    void print_rom();
    QString flash_rom_path;
    QString bios_rom_path;
    QString bootblock_path;
    fl_flashctx_t *flash_context;
};

extern MainWindow *w;

#endif // MAINWINDOW_H
