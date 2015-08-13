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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "supported.h"
#include "about.h"
#include "addpayload.h"
#include "addcomponent.h"
#include "deletecomponents.h"
#include "datagatherer.h"
#include "progressdialog.h"
#include "choosechip.h"
#include "hashlibpp/hashlibpp.h"
#include <cstdarg>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QtXml>

extern "C" {
#include "libbiosext.h"
#include "libcbfstool.h"
}

fl_log_callback_t *my_log_callback;

int libflashrom_log(fl_log_level_t log_level, const char *format, va_list vl)
{
        QString text;

        text.vsprintf(format, vl);
        w->active_log_out->append(text);

        return 1;
}

int libbiosext_log(const char *const format, ...)
{
        int ret = 0;
        QString text;
        va_list args;

        va_start(args, format);
        text.vsprintf(format, args);
        w->active_log_out->insertPlainText(text);
        va_end(args);

        return ret;
}

int libcbfstool_log(const char *const format, ...)
{
        int ret = 0;
        QString text;
        va_list args;

        va_start(args, format);
        text.vsprintf(format, args);
        w->active_log_out->insertPlainText(text);
        va_end(args);

        return ret;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        if (!QDir("hardware_data").exists())
            QDir().mkdir("hardware_data");
        if (!QDir("bios_dump").exists())
            QDir().mkdir("bios_dump");

        ui->log_auto->setReadOnly(true);
        ui->log_flash->setReadOnly(true);
        ui->log_rom_opt->setReadOnly(true);
        ui->log_extract->setReadOnly(true);
        ui->log_create_rom->setReadOnly(true);
        active_log_out = ui->log_auto;

        flash_context = NULL;
        my_log_callback = &libflashrom_log;
        fl_init(0);
        fill_cb_arch();
        fill_cb_programmers();
        fill_cb_payload();
        fl_set_log_callback(my_log_callback);
}

MainWindow::~MainWindow()
{
        fl_flash_release(flash_context);
        fl_shutdown();
        delete ui;
}

void MainWindow::on_b_sel_payload_clicked()
{
        open_select_rom_window();
}

void MainWindow::on_b_probe_clicked()
{
        ui->log_flash->clear();
        if (fl_flash_probe(&flash_context, NULL) == 3) {
                ChooseChip choose_chip_dialog;
                const char **chip_names = NULL;
                int chip_count = 0;
                int i = 0;

                chip_names = fl_multiple_flash_probe(&chip_count);
                for (; i < chip_count; ++i) {
                        choose_chip_dialog.add_chip(chip_names[i]);
                }

                choose_chip_dialog.setModal(true);
                choose_chip_dialog.set_flash_ctx_ptr(&flash_context);
                choose_chip_dialog.exec();
                fl_data_free(chip_names);
        }
}

void MainWindow::on_b_read_clicked()
{
        ui->log_flash->clear();
        char buffer[2097152];
        if (flash_context) {
               fl_image_read(flash_context, buffer, 2097152);
        }
}

void MainWindow::on_b_verify_clicked()
{
        ui->log_flash->clear();
        char buffer[2097152];
        memset(buffer, 0, 2097152);
        if (flash_context) {
                fl_image_verify(flash_context, buffer, 2097152);
        }
}

void MainWindow::on_b_erase_clicked()
    {
        ui->log_flash->clear();
        fl_flash_erase(flash_context);
}

void MainWindow::on_b_flash_clicked()
{
    ui->log_flash->clear();
    char buffer[2097152];
    memset(buffer, 0, 2097152);
    if (flash_context) {
           fl_image_write(flash_context, buffer, 2097152);
    }
}

void MainWindow::on_b_sel_bios_rom_clicked()
{
        open_select_bios_rom_window();
}

void MainWindow::on_b_sel_bios_out_clicked()
{
        open_select_bios_out_window();
}

void MainWindow::on_b_sel_boot_block_clicked()
{
        QString bootblock_name;

        bootblock_path = QFileDialog::getOpenFileName(this, tr("Select bootblock"), ".", "All files (*.*)");
        bootblock_name = bootblock_path.section('/', -1);
        ui->b_sel_boot_block->setVisible(false);
        ui->l_bootblack_name->setText(bootblock_name);
}

void MainWindow::on_b_auto_build_clicked()
{
        DataGatherer data_gatherer;
        QFile hardware_info("hardware_info.xml");

        /* Save lspci -nn output*/
        data_gatherer.save_lspci_output();

        /* Dump VGABIOS from iomem */
        data_gatherer.save_bios_rom_from_iomem();

        /* Save EDID data */
        data_gatherer.save_edid_data();
}

void MainWindow::on_b_auto_flash_clicked()
{
        DataGatherer data_gatherer;
        ProgressDialog progress_dialog;
        unsigned int rom_size = 0;
        QDomDocument xmlBOM;
        QFile hardware_info("hardware_info.xml");

        //progress_dialog.setModal(true);
        //progress_dialog.exec();

        /* Probe for a chip */
        if (!data_gatherer.probe_chip()) {
                /* Save lspci -nn output*/
                data_gatherer.save_lspci_output();

                /* Make backup of current bios */
                data_gatherer.save_bios_rom();

                /* Extract rom components */
                data_gatherer.extract_rom("bios_dump/bios_dump.rom");

                if (!hardware_info.open(QIODevice::ReadOnly ))
                {
                    qDebug() << "Error while loading file";
                }

                xmlBOM.setContent(&hardware_info);
                hardware_info.close();

                QDomElement root = xmlBOM.documentElement();
                hashwrapper *md5_wrapper = new md5wrapper();;
                QDirIterator file_iterator("bios_dump");
                while (file_iterator.hasNext()) {
                        if (file_iterator.next().contains("oprom_")) {
                                QString vgabios_hash = QString(md5_wrapper->getHashFromFile(file_iterator.filePath().toStdString()).c_str());
                                QDomElement chipset = root.firstChild().toElement();

                                while (!chipset.isNull()) {
                                        if (chipset.tagName() == "chipset") {
                                                QDomElement chipset_child = chipset.firstChild().toElement();
                                                QDomNode vgabios_node = chipset_child.nextSibling();

                                                for (int i = 0; !vgabios_node.isNull(); ++i) {
                                                        QString md5 = vgabios_node.firstChild().toText().data();
                                                        //qDebug() << "md5: " << md5;
                                                        //qDebug() << "vgabios_hash: " << vgabios_hash;

                                                        if (md5 == vgabios_hash)
                                                                qDebug() << "VGABIOS OK";

                                                        vgabios_node = vgabios_node.nextSibling();
                                                }
                                        }
                                        chipset = chipset.nextSiblingElement();
                                }
                        }
                }
                delete md5_wrapper;

                rom_size = fl_flash_getsize(flash_context);

                /* Create rom */
                //char **cbfs_params;
                /* PROGNAME(1) + NAME(1) + COMMAND(1) + ARCH(2) + SIZE(2) */
                /*int param_count = 7;
                QString params[11];

                ui->log_create_rom->clear();
                params[0] = "flash_tool";
                params[1] = "auto_rom.rom";
                params[2] = "create";
                params[3] = "-m";
                params[4] =  "x86";
                params[5] = "-s";
                params[6] = QString(rom_size).toStdString().c_str() + 'K';*/

                /* HOW TO DETERMINE BOOTBLOCK OFFSET?
                 * params[7] = "-b ";
                 * params[8] =
                 */

                /* HOW TO DETERMINE CBFS OFFSET?
                 * params[9] = "-o ";
                 * params[10] =
                 */

                /*cbfs_params = new char*[param_count];
                for (int i = 0; i < param_count; ++i) {
                        cbfs_params[i] = new char[params[i].length()];
                        strcpy(cbfs_params[i], params[i].toStdString().c_str());
                }*/

                //start_cbfs(param_count, cbfs_params);

                /*for (int i = 0; i < param_count; ++i) {
                        delete [] cbfs_params[i];
                }
                delete [] cbfs_params;*/

                /* ADD COMPONENTS */

                /* Read rom file to array */
                /*QFile file("auto_rom.rom");
                QByteArray blob;
                //char rom_buffer[rom_size];

                if (!file.open(QIODevice::ReadOnly)) {
                        qDebug() << "Can't open file!";
                } else {
                        blob = file.readAll();
                }*/

                /* WRITE IMAGE TO CHIP */
                //fl_image_write(flash_context, blob.data(), rom_size);
        }
}

void MainWindow::on_b_extract_clicked()
{
        DataGatherer data_gatherer;
        data_gatherer.extract_rom(bios_rom_path.toStdString().c_str());
}

void MainWindow::on_b_create_rom_clicked()
{
        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) + ARCH(2) + SIZE(2) */
        int param_count = 7;
        QString params[11];

        ui->log_create_rom->clear();
        params[0] = "flash_tool";
        params[1] = ui->edit_cbfs_name->text();
        params[2] = "create";
        params[3] = "-m";
        params[4] =  ui->cb_sel_arch->currentText();
        params[5] = "-s";
        params[6] = ui->edit_size->text() + 'K';

        if (params[1].isEmpty())
                params[1] = "coreboot.rom";
        if (!ui->edit_bootblock_off->text().isEmpty()) {
                params[param_count] = "-b ";
                params[param_count + 1] = ui->edit_bootblock_off->text().isEmpty();
                param_count += 2;
        }
        if (!ui->edit_cbfs_off->text().isEmpty()) {
                params[param_count] = "-o ";
                params[param_count + 1] = ui->edit_cbfs_name->text();
                param_count += 2;
        }

        cbfs_params = new char*[param_count];
        for (int i = 0; i < param_count; ++i) {
                cbfs_params[i] = new char[params[i].length()];
                strcpy(cbfs_params[i], params[i].toStdString().c_str());
        }

        start_cbfs(param_count, cbfs_params);

        for (int i = 0; i < param_count; ++i) {
                delete [] cbfs_params[i];
        }
        delete [] cbfs_params;
}

void MainWindow::on_b_add_payload_clicked()
{
        AddPayload add_payload_window;
        add_payload_window.setModal(true);
        add_payload_window.exec();
}

void MainWindow::on_b_add_component_clicked()
{
        AddComponent add_component_window;
        add_component_window.setModal(true);
        add_component_window.exec();
}

void MainWindow::on_b_remove_comp_clicked()
{
        DeleteComponents delete_component_window;
        delete_component_window.setModal(true);
        delete_component_window.exec();
}

void MainWindow::on_b_init_prog_clicked()
{
        QString programmer = ui->cb_sel_progr->currentText();

        if (!(programmer == "select")) {
                w->active_log_out->clear();
                fl_shutdown();
                fl_programmer_init(programmer.toStdString().c_str(), ui->edit_prog_param->text().toStdString().c_str());
        }
}

void MainWindow::fill_cb_programmers()
{
        const char **programmers = 0;

        programmers = fl_supported_programmers();

        for (int i = 0; programmers[i]; ++i){
                ui->cb_sel_progr->addItem(programmers[i]);
        }

        fl_data_free(programmers);
}

void MainWindow::fill_cb_arch()
{
        ui->cb_sel_arch->addItem("x86");
        ui->cb_sel_arch->addItem("mips");
        ui->cb_sel_arch->addItem("arm");
        ui->cb_sel_arch->addItem("arm64");
}

void MainWindow::fill_cb_payload()
{
        ui->cb_auto_sel_payload->addItem("SeaBIOS");
        ui->cb_auto_sel_payload->addItem("GRUB 2");
        ui->cb_auto_sel_payload->addItem("GRUB legacy");
        ui->cb_auto_sel_payload->addItem("FILO");
        ui->cb_auto_sel_payload->addItem("Etherboot");
        ui->cb_auto_sel_payload->addItem("Open Firmware");
        ui->cb_auto_sel_payload->addItem("OpenBIOS");
        ui->cb_auto_sel_payload->addItem("Tiano Core");
}

void MainWindow::open_select_rom_window()
{
        QString rom_name;

        flash_rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        rom_name = flash_rom_path.section('/', -1);
        ui->b_sel_payload->setVisible(false);
        ui->l_payload_name->setText(rom_name);
        ui->l_opt_sel_name->setText(rom_name);
        print_rom();
}

void MainWindow::open_select_bios_rom_window()
{
        QString rom_name;

        bios_rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        rom_name = bios_rom_path.section('/', -1);
        ui->b_sel_bios_rom->setVisible(false);
        ui->l_ex_rom_name->setText(rom_name);
}

void MainWindow::open_select_bios_out_window()
{
        QString rom_dir = QFileDialog::getExistingDirectory(this,
                                                            tr("Select output dir"),
                                                            ".",
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        rom_dir.append("/");
        ui->b_sel_bios_out->setVisible(false);
        ui->l_ex_out_name->setText(rom_dir);
        set_output_directory(rom_dir.toStdString().c_str());
}


void MainWindow::on_act_sel_payload_triggered()
{
        open_select_rom_window();
}

void MainWindow::on_act_supported_list_triggered()
{
        Supported supported_window;
        supported_window.setModal(true);
        supported_window.exec();
}

void MainWindow::on_act_about_triggered()
{
        About about_window;
        about_window.setModal(true);
        about_window.exec();
}

void MainWindow::print_rom()
{
        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) */
        int param_count = 3;
        QString params[3];

        w->active_log_out->clear();
        params[0] = "./flash_tool";
        params[1] = flash_rom_path;
        params[2] = "print";

        cbfs_params = new char*[param_count];
        for (int i = 0; i < param_count; ++i) {
                cbfs_params[i] = new char[params[i].length()];
                strcpy(cbfs_params[i], params[i].toStdString().c_str());
        }

        start_cbfs(param_count, cbfs_params);

        for (int i = 0; i < param_count; ++i) {
                delete [] cbfs_params[i];
        }
        delete [] cbfs_params;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
        switch (index) {
        case 0:
                active_log_out = ui->log_auto;
                break;
        case 1:
                active_log_out = ui->log_flash;
                break;
        case 2:
                active_log_out = ui->log_rom_opt;
                break;
        case 3:
                active_log_out = ui->log_extract;
                break;
        case 4:
                active_log_out = ui->log_create_rom;
                break;
        }
}

QString MainWindow::get_flash_rom_path()
{
        return flash_rom_path;
}
