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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        /**
         * @brief Return path to rom loaded to cbfs_tool
         * @return path tom rom loaded to cbfs_tool
         */
        QString get_cbfs_rom_path();
        Ui::MainWindow *ui;

        /**
         * @brief Pointer to active log output
         */
        QTextEdit *active_log_out;

        /**
         * @brief Name of probed chip
         */
        QString chip_name;

        /**
         * @brief Path to coreboot directory
         */
        QString coreboot_dir;

        /**
         * @brief Path to factory BIOS file
         */
        QString factory_bios_dir;

        /**
         * @brief Indicates if chip was found by flashrom
         */
        bool chip_found;

        /**
         * @brief Indicates if programmer has been initialized by flashrom
         */
        bool programmer_initialized;

        /**
         * @brief Pointer to InfoDialog
         */
        InfoDialog *info_dialog;

private slots:
        /* Main window */
        /**
         * @brief Called when b_init_prog button is clicked
         * Initializes a programmer selected in cb_sel_progr combo box
         */
        void on_b_init_prog_clicked();

        /* Toolbar */
        /**
         * @brief Called when on_act_supported_list action is clicked
         * Opens Supported window with list of hardware supported by flashrom
         */
        void on_act_supported_list_triggered();

        /**
         * @brief Called when on_act_preferences action is clicked
         * Opens Preferences window with possible application options
         */
        void on_act_preferences_triggered();

        /**
         * @brief Called when on_act_about action is clicked
         * Opens About window with information about application and used tools
         */
        void on_act_about_triggered();

        /* Auto tab */
        /**
         * @brief Called when on_b_auto_get_bios button is clicked
         * Reads BIOS image from chip and saves it in hardware_data/factory_bios.bin
         */
        void on_b_auto_get_bios_clicked();

        /**
         * @brief Called when on_b_auto_set_bios button is clicked
         * Opens QFileDialog to set a path to factory BIOS image
         */
        void on_b_auto_set_bios_clicked();

        /**
         * @brief Called when on_b_auto_get_hw_data button is clicked
         * Opens QFileDialog to set a path to factory BIOS image
         */
        void on_b_auto_get_hw_data_clicked();

        /**
         * @brief Called when on_b_auto_build_img button is clicked
         * Builds coreboot image and copies it to application directory
         */
        void on_b_auto_build_img_clicked();

        /**
         * @brief Called when on_b_auto_flash button is clicked
         * Writes coreboot image to chip
         */
        void on_b_auto_flash_clicked();

        /* Flash tab */
        /**
         * @brief Called when on_b_read button is clicked
         * Opens QFileDialog, reads chip content and saves it to selected file
         */
        void on_b_read_clicked();

        /**
         * @brief Called when on_b_probe button is clicked
         * Probes for all known chips, if multiple chips are found it opens ChooseChip
         * dialog to select particular chip and probe for this specified chip again
         */
        void on_b_probe_clicked();

        /**
         * @brief Called when on_b_verify button is clicked
         * Opens QFileDialog, reads chip content and compares it with selected file
         */
        void on_b_verify_clicked();

        /**
         * @brief Called when on_b_erase button is clicked
         * Erases chip conetent
         */
        void on_b_erase_clicked();

        /**
         * @brief Called when on_b_flash button is clicked
         * Opens QFileDialog and writes selected file content to chip
         */
        void on_b_flash_clicked();

        /* Extract tab */
        /**
         * @brief Called when on_sel_bios_rom button is clicked
         * Opens QFileDialog to select a rom for extraction
         */
        void on_b_sel_bios_rom_clicked();

        /**
         * @brief Called when on_b_sel_bios_out button is clicked
         * Opens QFileDialog to select output directory for extraction
         */
        void on_b_sel_bios_out_clicked();

        /**
         * @brief Called when on_b_extract button is clicked
         * Extracts rom contents with use of bios_extract tool
         */
        void on_b_extract_clicked();

        /* Create rom tab */
        /**
         * @brief Called when on_b_create_rom button is clicked
         * Creates image with use of cbfs_tool
         */
        void on_b_create_rom_clicked();

        /**
         * @brief Called when on_b_sel_boot_block button is clicked
         * Opens QFileDialog to select boot block file
         */
        void on_b_sel_boot_block_clicked();

        /* Rom options tab */
        /**
         * @brief Called when on_b_ropt_select button is clicked
         * Opens QFileDialog to select rom file for cbfs_tool
         */
        void on_b_ropt_select_clicked();

        /**
         * @brief Called when on_b_add_component button is clicked
         * Opens AddComponent dialog
         */
        void on_b_add_component_clicked();

        /**
         * @brief Called when on_b_remove_comp button is clicked
         * Opens DeleteComponents dialog
         */
        void on_b_remove_comp_clicked();

        /* Other */
        /**
         * @brief Called when tabWidget tab is changed
         * Sets appropriate log output to active_log_out pointer
         */
        void on_tabWidget_currentChanged(int index);

private:
        /**
         * @brief Fill cb_programmers combo box with available programmers
         */
        void fill_cb_programmers();

        /**
         * @brief Fill cb_arch combo box with available architectures
         */
        void fill_cb_arch();

        /**
         * @brief Fill cb_payload combo box with available payloads
         */
        void fill_cb_payload();

        /**
         * @brief Path to CBFS rom image for cbfs_tool
         */
        QString cbfs_rom_path;

        /**
         * @brief Path to BIOS image for bios_extract tool
         */
        QString bios_rom_path;

        /**
         * @brief Path to boot block for bios_extract tool
         */
        QString bootblock_path;

        /**
         * @brief Pointer to model responsible for storing data of corresponding rom content in cbfs_tool
         */
        QStandardItemModel *model;
};

extern MainWindow *w;

#endif // MAINWINDOW_H
