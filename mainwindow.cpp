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
    ui(new Ui::MainWindow),
    programmer_initialized(false),
    chip_found(false),
    flash_context(NULL)
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

/*void MainWindow::on_b_sel_payload_clicked()
{
        open_select_rom_window();
}*/

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
        QString save_dir;
        unsigned int rom_size = 0;
        char *buffer = NULL;

        if (flash_context) {
                save_dir = QFileDialog::getExistingDirectory(this,
                                                             tr("Select output directory"),
                                                             ".",
                                                             QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);

                rom_size = fl_flash_getsize(flash_context);
                buffer = new char[rom_size];

                if (!buffer) {
                        qDebug() << "Out of memory!";
                } else {
                        ui->log_flash->clear();
                        fl_image_read(flash_context, buffer, rom_size);
                        delete [] buffer;
                }
        }
}

void MainWindow::on_b_verify_clicked()
{
        QString verify_dir;
        unsigned int rom_size = 0;

        if (flash_context) {
                verify_dir = QFileDialog::getExistingDirectory(this,
                                                             tr("Select ROM to verify"),
                                                             ".",
                                                             QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);
                ui->log_flash->clear();
                rom_size = fl_flash_getsize(flash_context);
                QFile file(verify_dir);
                QByteArray blob;

                if (!file.open(QIODevice::ReadOnly)) {
                        qDebug() << "Can't open file!";
                } else {
                        blob = file.readAll();
                        fl_image_verify(flash_context, blob.data(), rom_size);
                }
        }
}

void MainWindow::on_b_erase_clicked()
{
        ui->log_flash->clear();
        fl_flash_erase(flash_context);
}

void MainWindow::on_b_flash_clicked()
{
        QString rom_dir;
        unsigned int rom_size = 0;

        if (flash_context) {
                rom_dir = QFileDialog::getExistingDirectory(this,
                                                             tr("Select ROM to flash"),
                                                             ".",
                                                             QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);
                ui->log_flash->clear();
                rom_size = fl_flash_getsize(flash_context);
                QFile file(rom_dir);
                QByteArray blob;

                if (!file.open(QIODevice::ReadOnly)) {
                        qDebug() << "Can't open file!";
                } else {
                        blob = file.readAll();
                        fl_image_write(flash_context, blob.data(), rom_size);
                }
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

void MainWindow::on_b_auto_get_hw_data_clicked()
{
        DataGatherer data_gatherer;

        /* Save lspci -nn output*/
        data_gatherer.save_lspci_output();

        /* Save EDID data */
        data_gatherer.save_edid_data();

        /* Save dmidecode output */
        data_gatherer.save_dmidecode_output();

        /* Dump VGABIOS from iomem */
        data_gatherer.save_bios_rom_from_iomem();

        /* Create archive from gathered data */
        data_gatherer.create_hardware_data_archive();
}

void MainWindow::on_b_auto_build_img_clicked()
{
        DataGatherer data_gatherer;
        ProgressDialog progress_dialog;

        QDomDocument xmlBOM;
        QFile hardware_info("hardware_info.xml");
        QString hardware_data_path;
        bool is_config_ok = false;

        //progress_dialog.setModal(true);
        //progress_dialog.exec();

        hardware_data_path = QFileDialog::getOpenFileName(this, tr("Select hardware data for target system"), ".", "All files (*.tar)");
        data_gatherer.unpack_hardware_data_archive(hardware_data_path);

        QString motherboard = data_gatherer.get_motherboard_model();
        QString display_panel = data_gatherer.get_display_panel_model();
        QString graphic_card = data_gatherer.get_graphic_card_model();
        //QString chip_name = w->chip_name;

        qDebug() << "motherboard: " << motherboard;
        //qDebug() << "chip_name: " << chip_name;
        qDebug() << "graphic_card: " << graphic_card;
        qDebug() << "display_panel: " << display_panel;

        if (!hardware_info.open(QIODevice::ReadOnly))
        {
                qDebug() << "Error while loading file: " + hardware_info.fileName();
        }

        xmlBOM.setContent(&hardware_info);
        hardware_info.close();

        QDomElement root = xmlBOM.documentElement();
        hashwrapper *sha_wrapper = new sha256wrapper();

        QDomElement config = root.firstChild().toElement();

        while ((!config.isNull()) && (!is_config_ok)) {
                QDomNode conf_child = config.firstChild();
                bool board_ok = (motherboard == conf_child.firstChild().toText().data());
                conf_child = conf_child.nextSibling();
                //bool chipset_ok = (chip_name == conf_child.firstChild().toText().data());
                //conf_child = conf_child.nextSibling();
                bool gpu_ok = (graphic_card == conf_child.firstChild().toText().data());
                conf_child = conf_child.nextSibling();
                bool panel_ok = (display_panel == conf_child.firstChild().toText().data());
                conf_child = conf_child.nextSibling();
                QString need_vgabios = conf_child.firstChild().toText().data();

                qDebug() << board_ok;
                //qDebug() << chipset_ok;
                qDebug() << gpu_ok;
                qDebug() << panel_ok;

                if (board_ok /*&& chipset_ok*/ && gpu_ok && panel_ok) {
                        if (need_vgabios == "factory") {
                                QFileInfo factory_bios("hardware_data/factory_bios.bin");
                                if (factory_bios.exists()) {
                                        /* Extract factory bios components */
                                        if (!QDir("hardware_data/factory_bios_components").exists())
                                                QDir().mkdir("hardware_data/factory_bios_components");
                                        set_output_directory("hardware_data/factory_bios_components/");
                                        data_gatherer.extract_rom("hardware_data/factory_bios.bin");
                                        conf_child = conf_child.nextSibling();
                                        QDirIterator file_iterator("hardware_data/factory_bios_components");
                                        QString vgabios_xml_hash = conf_child.firstChild().toText().data();

                                        while (file_iterator.hasNext()) {                 
                                                if (file_iterator.next().contains("oprom_")) {
                                                        QString vgabios_hash = QString(sha_wrapper->getHashFromFile(file_iterator.filePath().toStdString()).c_str());

                                                        //qDebug() << vgabios_hash;
                                                        //qDebug() << vgabios_xml_hash;

                                                        if (vgabios_hash == vgabios_xml_hash) {
                                                                //QFile vgabios_file(file_iterator.filePath());
                                                                QString rename_command = "cp " + file_iterator.filePath() +
                                                                                         " hardware_data/factory_bios_components/factory_vgabios.bin";
                                                                system(rename_command.toStdString().c_str());
                                                                is_config_ok = true;
                                                                /*if (!vgabios_file.open(QIODevice::WriteOnly )) {
                                                                        qDebug() << "Error while loading file: " + vgabios_file.fileName();
                                                                } else {
                                                                    vgabios_file.rename("factory_bios.bin");
                                                                    vgabios_file.close();
                                                                }*/
                                                        }
                                                }
                                        }
                                } else {
                                        qDebug() << "This configuration requires factory VGABIOS, please use 'Get factory BIOS' button";
                        }
                } else if (need_vgabios == "memory") {
                                QString vgabios_hash = QString(sha_wrapper->getHashFromFile("hardware_data/vgabios_from_mem.bin").c_str());
                                QString vgabios_xml_hash = conf_child.firstChild().toText().data();

                                qDebug() << vgabios_hash;
                                qDebug() << vgabios_xml_hash;

                                if (vgabios_hash == vgabios_xml_hash)
                                        is_config_ok = true;
                        }
                }

                if (!is_config_ok)
                    config = config.nextSibling().toElement();
        }
        delete sha_wrapper;

        if (is_config_ok) {
                QString working_config = config.lastChild().firstChild().toText().data() + "_"
                                + ui->cb_auto_sel_payload->currentText();
                QString copy_config_cmd = "cp coreboot_configs/" + working_config + " coreboot/.config";

                qDebug() << "copy_config_cmd: " << copy_config_cmd;

                system("rm coreboot/build/coreboot.rom");
                system(copy_config_cmd.toStdString().c_str());
                system("make -C coreboot");

                QFile coreboot_rom_file("coreboot/build/coreboot.rom");

                if (coreboot_rom_file.exists()) {
                    system("cp coreboot/build/coreboot.rom ../");
                } else {
                    qDebug() << "coreboot image compilation failed!";
                }

        } else {
                qDebug() << "No configuration for your system! Please send hardware_data.tar to"
                                    " lukasz.dmitrowski@gmail.com";
        }
}

void MainWindow::on_b_auto_get_bios_clicked()
{
        DataGatherer data_gatherer;
        data_gatherer.save_bios_rom_factory();
}

void MainWindow::on_b_auto_flash_clicked()
{
        //unsigned int rom_size = fl_flash_getsize(flash_context);

        /* Read rom file to array */
        //QFile file("automatic_build_rom.rom");
        //QByteArray blob;
        //char rom_buffer[rom_size];

        //if (!file.open(QIODevice::ReadOnly)) {
        //        qDebug() << "Can't open file!";
        //} else {
        //        blob = file.readAll();
        //}

        /* WRITE IMAGE TO CHIP */
        //fl_image_write(flash_context, blob.data(), rom_size);
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

/*void MainWindow::on_b_add_payload_clicked()
{
        AddPayload add_payload_window;
        add_payload_window.setModal(true);
        add_payload_window.exec();
}*/

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
                if (!fl_programmer_init(programmer.toStdString().c_str(), ui->edit_prog_param->text().toStdString().c_str()))
                        programmer_initialized = true;
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
}

/*void MainWindow::open_select_rom_window()
{
        QString rom_name;

        flash_rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        rom_name = flash_rom_path.section('/', -1);
        //ui->b_sel_payload->setVisible(false);
        //ui->l_payload_name->setText(rom_name);
        ui->l_opt_sel_name->setText(rom_name);
        print_rom();
}*/

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


/*void MainWindow::on_act_sel_payload_triggered()
{
        open_select_rom_window();
}*/

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
