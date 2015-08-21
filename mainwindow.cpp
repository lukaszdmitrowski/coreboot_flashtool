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
#include "choosechip.h"
#include "hashlibpp/hashlibpp.h"
#include "flasher.h"
#include "constants.h"
#include "preferences.h"
#include <sys/stat.h>
#include <cstdarg>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QtXml>
#include <QApplication>
#include <QTextStream>

extern "C" {
#include "libbiosext.h"
#include "libcbfstool.h"
}

#include "loggers.h"


fl_log_callback_t *my_log_callback;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        active_log_out = ui->log_auto;
        chip_name = "";
        coreboot_dir = "coreboot";
        factory_bios_dir = "hardware_data/factory_bios.bin";
        chip_found = false;
        programmer_initialized = false;
        info_dialog = new InfoDialog(this);
        model = new QStandardItemModel();

        if (!QDir("hardware_data").exists())
            QDir().mkdir("hardware_data");

        ui->log_auto->setReadOnly(true);
        ui->log_flash->setReadOnly(true);
        ui->log_rom_opt->setReadOnly(true);
        ui->log_extract->setReadOnly(true);
        ui->log_create_rom->setReadOnly(true);
        ui->tv_rom_content->setSelectionBehavior(QAbstractItemView::SelectRows);

        my_log_callback = &libflashrom_log;
        fill_cb_arch();
        fill_cb_programmers();
        fill_cb_payload();
        fl_set_log_callback(my_log_callback);

        QFile config_file("preferences.cfg");

        if (config_file.open(QIODevice::ReadOnly)) {
                QTextStream config_stream(&config_file);
                QString coreboot_path_line;
                QString configurations_path_line;

                coreboot_path_line = config_stream.readLine();
                coreboot_path_line.remove("coreboot_path: ");
                coreboot_dir = coreboot_path_line;

                configurations_path_line = config_stream.readLine();
                configurations_path_line.remove("configurations_path: ");
                configurations_path = configurations_path_line;
        } else {
                w->info_dialog->show_message(ERR_FILE_CONFIG);
        }
}

MainWindow::~MainWindow()
{
        Flasher flasher;
        flasher.shutdown_flashrom();
        delete ui;
}

void MainWindow::on_b_probe_clicked()
{
        Flasher flasher;
        RET_VAL ret = UNKNOWN;

        setWindowTitle("Please wait...");
        QApplication::processEvents();
        ret = flasher.probe_chip();
        setWindowTitle("Coreboot Flash Tool");
        info_dialog->show_message(ret);
}

void MainWindow::on_b_read_clicked()
{
        QString save_dir;
        DataGatherer data_gatherer;
        RET_VAL ret = UNKNOWN;

        save_dir = QFileDialog::getExistingDirectory(this,
                                                     tr("Select output directory"),
                                                     ".",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        if (!save_dir.isEmpty()) {
                setWindowTitle("Please wait...");
                QApplication::processEvents();
                ret = data_gatherer.save_bios_rom_factory(save_dir + "/factory_bios.bin");
                setWindowTitle("Coreboot Flash Tool");
                info_dialog->show_message(ret);
        }
}

void MainWindow::on_b_verify_clicked()
{
        Flasher flasher;
        QString verify_dir;
        FILE *verify_file;
        unsigned char *data = NULL;
        RET_VAL ret = UNKNOWN;

        ui->log_flash->clear();
        verify_dir = QFileDialog::getOpenFileName(this, tr("Select ROM to verify"), ".", "All files (*.*)");

        if (!verify_dir.isEmpty()) {
                if (!(verify_file = fopen(verify_dir.toStdString().c_str(), "rb"))) {
                        ret = ERR_FILE_SELECTED;
                } else {
                        setWindowTitle("Please wait...");
                        QApplication::processEvents();

                        struct stat st;
                        stat(verify_dir.toStdString().c_str(), &st);

                        data = new unsigned char[st.st_size];

                        if (data) {
                                if (fread(data, sizeof(unsigned char), st.st_size, verify_file) < (size_t)st.st_size) {
                                        ret = ERR_READ_LESS_BYTES;
                                } else {
                                        ret = flasher.verify_chip(&data, st.st_size);
                                }

                                if (ret == ERR_CHIP_NOT_PROBED)
                                        ret = flasher.verify_chip(&data, st.st_size);

                                delete [] data;
                        } else {
                                ret = ERR_MEM_ALLOC;
                        }

                        fclose(verify_file);
                        setWindowTitle("Coreboot Flash Tool");
                }
                info_dialog->show_message(ret);
        }
}

void MainWindow::on_b_erase_clicked()
{
        Flasher flasher;
        RET_VAL ret = UNKNOWN;

        ui->log_flash->clear();
        setWindowTitle("Please wait...");
        QApplication::processEvents();

        ret = flasher.erase_chip();
        if (ret == ERR_CHIP_NOT_PROBED)
                flasher.erase_chip();

        setWindowTitle("Coreboot Flash Tool");
        info_dialog->show_message(ret);
}

void MainWindow::on_b_flash_clicked()
{
        Flasher flasher;
        QString write_dir;
        FILE *write_file;
        unsigned char *data = NULL;
        RET_VAL ret = UNKNOWN;

        ui->log_flash->clear();
        write_dir = QFileDialog::getOpenFileName(this, tr("Select ROM to flash"), ".", "All files (*.*)");

        if (!write_dir.isEmpty()) {
                if (!(write_file = fopen(write_dir.toStdString().c_str(), "rb"))) {
                        ret = ERR_FILE_SELECTED;
                } else {
                        setWindowTitle("Please wait...");
                        QApplication::processEvents();

                        struct stat st;
                        stat(write_dir.toStdString().c_str(), &st);

                        data = new unsigned char[st.st_size];

                        if (data) {
                                if (fread(data, sizeof(unsigned char), st.st_size, write_file) == 0) {
                                        ret = ERR_FILE_WRITE;
                                } else {
                                        ret = flasher.write_chip(&data, st.st_size);
                                        if (ret == ERR_CHIP_NOT_PROBED)
                                                ret = flasher.write_chip(&data, st.st_size);
                                        delete [] data;
                                }
                        } else {
                                ret = ERR_MEM_ALLOC;
                        }
                        fclose(write_file);
                        setWindowTitle("Coreboot Flash Tool");
                }
                info_dialog->show_message(ret);
        }
}

void MainWindow::on_b_sel_bios_rom_clicked()
{
        QString rom_name;

        bios_rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        rom_name = bios_rom_path.section('/', -1);
        ui->b_sel_bios_rom->setVisible(false);
        ui->l_ex_rom_name->setText(rom_name);
}

void MainWindow::on_b_sel_bios_out_clicked()
{
        QString rom_dir = QFileDialog::getExistingDirectory(this,
                                                            tr("Select output dir"),
                                                            ".",
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        rom_dir.append("/");
        ui->b_sel_bios_out->setVisible(false);
        ui->l_ex_out_name->setText(rom_dir);
        libbiosext_set_out_dir(rom_dir.toStdString().c_str());
}

void MainWindow::on_b_sel_boot_block_clicked()
{
        QString bootblock_name;

        bootblock_path = QFileDialog::getOpenFileName(this, tr("Select bootblock"), ".", "All files (*.*)");
        bootblock_name = bootblock_path.section('/', -1);
        ui->b_sel_boot_block->setVisible(false);
        ui->l_bootblack_name->setText(bootblock_name);
}

void MainWindow::on_b_auto_get_bios_clicked()
{
        DataGatherer data_gatherer;
        RET_VAL ret = UNKNOWN;

        setWindowTitle("Please wait...");
        QApplication::processEvents();
        ret = data_gatherer.save_bios_rom_factory("hardware_data/factory_bios.bin");
        setWindowTitle("Coreboot Flash Tool");
        if (ret == SUCCESS) {
                info_dialog->show_message("BIOS image saved to hardware_data/factory_bios.bin");
        } else {
                info_dialog->show_message(ret);
        }
}

void MainWindow::on_b_auto_set_bios_clicked()
{
        factory_bios_dir = QFileDialog::getOpenFileName(this,
                                                        tr("Select factory BIOS file"),
                                                        ".",
                                                        "All files (*.*)");

}

void MainWindow::on_b_auto_get_hw_data_clicked()
{
        DataGatherer data_gatherer;
        RET_VAL ret = UNKNOWN;

        /* Save lspci -nn output*/
        if ((ret = data_gatherer.save_lspci_output()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Save EDID data */
        QFile lvds_path("/sys/class/drm/card0-LVDS-1/edid");

        if (lvds_path.exists()) {
                if ((ret = data_gatherer.save_edid_data()) != SUCCESS)
                        info_dialog->show_message(ret);
        }

        /* Save dmidecode output */
        if ((ret = data_gatherer.save_dmidecode_output()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Dump VGABIOS from iomem */
        if ((ret = data_gatherer.save_bios_rom_from_iomem()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Create archive from gathered data */
        if ((ret = data_gatherer.create_hardware_data_archive()) != SUCCESS)
                info_dialog->show_message(ret);

        info_dialog->show_message("Finished gathering data");
}

void MainWindow::on_b_auto_build_img_clicked()
{
        QDir coreboot_directory(coreboot_dir);
        if (!coreboot_directory.exists()) {
                info_dialog->show_message(ERR_COREBOOT_WRONG_DIR);
                return;
        }
        if (!(coreboot_directory.dirName() == "coreboot")) {
                info_dialog->show_message(ERR_COREBOOT_WRONG_DIR);
                return;
        }

        DataGatherer data_gatherer;
        QDomDocument xmlBOM;
        QFile hardware_info(w->configurations_path);
        QString hardware_data_path;
        bool is_config_ok = false;
        RET_VAL ret = UNKNOWN;

        hardware_data_path = QFileDialog::getOpenFileName(this, tr("Select hardware data for target system"), ".", "All files (*.tar)");

        ret = data_gatherer.unpack_hardware_data_archive(hardware_data_path);
        if (ret != SUCCESS) {
                info_dialog->show_message(ret);
                return;
        }

        QString motherboard = data_gatherer.get_motherboard_model();
        QString display_panel = data_gatherer.get_display_panel_model();
        QString graphic_card = data_gatherer.get_graphic_card_model();

        if (!hardware_info.open(QIODevice::ReadOnly)) {
                ret = ERR_FILE_HW_XML;
                info_dialog->show_message(ret);
                return;
        }

        xmlBOM.setContent(&hardware_info);
        hardware_info.close();

        QDomElement root = xmlBOM.documentElement();
        hashwrapper *sha_wrapper = new sha256wrapper();

        QDomElement config = root.firstChild().toElement();

        while ((!config.isNull()) && (!is_config_ok)) {
                QDomNode conf_child = config.firstChild();
                bool is_board_ok = false;
                bool is_gpu_ok = false;
                bool is_panel_ok = false;

                QString board_config = conf_child.firstChild().toText().data();
                conf_child = conf_child.nextSibling();
                QString gpu_config = conf_child.firstChild().toText().data();
                conf_child = conf_child.nextSibling();
                QString panel_config = conf_child.firstChild().toText().data();
                conf_child = conf_child.nextSibling();
                QString need_vgabios = conf_child.firstChild().toText().data();

                is_board_ok = (motherboard == board_config);
                if (gpu_config == "-") {
                        is_gpu_ok = true;
                } else {
                        is_gpu_ok = (graphic_card == gpu_config);
                }

                if (panel_config == "-") {
                        is_panel_ok = true;
                } else {
                        is_panel_ok = (display_panel == panel_config);
                }

                conf_child = conf_child.nextSibling();
                if (is_board_ok && is_gpu_ok && is_panel_ok) {
                        if (need_vgabios == "factory") {
                                QFileInfo factory_bios(factory_bios_dir);

                                if (factory_bios.exists()) {

                                        /* Create components dir or remove previously extracted components */
                                        if (!QDir("factory_bios_components").exists()) {
                                                QDir().mkdir("factory_bios_components");
                                        } else {
                                                QDirIterator remove_iterator("factory_bios_components");
                                                while (remove_iterator.hasNext()) {
                                                        remove_iterator.next();
                                                        QFile file(remove_iterator.filePath());
                                                        file.remove();
                                                }
                                        }

                                        /* Extract factory bios components */
                                        libbiosext_set_out_dir("factory_bios_components/");
                                        data_gatherer.extract_rom(factory_bios_dir);

                                        /* Check if extracted VGABIOS is known to work */
                                        QString vgabios_xml_hash = conf_child.firstChild().toText().data();
                                        QDirIterator file_iterator("factory_bios_components");

                                        while (file_iterator.hasNext()) {                 
                                                if (file_iterator.next().contains("oprom_")) {
                                                        QString vgabios_hash = QString(sha_wrapper->getHashFromFile(file_iterator.filePath().toStdString()).c_str());

                                                        if (vgabios_hash == vgabios_xml_hash) {
                                                                QString cp_to_coreboot_cmd = "cp " + file_iterator.filePath() + " "
                                                                                             + coreboot_dir + "vgabios.bin";
                                                                if (system(cp_to_coreboot_cmd.toStdString().c_str()) != 0) {
                                                                        info_dialog->show_message(ERR_COREBOOT_COPY_VGABIOS);
                                                                        return;
                                                                }

                                                                is_config_ok = true;
                                                        }
                                                }
                                        }
                                } else {
                                        info_dialog->show_message(ERR_COREBOOT_NEED_FACTORY_BIOS);
                                        return;
                                }
                        } else if (need_vgabios == "memory") {
                                /* Check if memory-extracted VGABIOS is known to work */
                                QString vgabios_hash = QString(sha_wrapper->getHashFromFile("hardware_data/vgabios_from_mem.bin").c_str());
                                QString vgabios_xml_hash = conf_child.firstChild().toText().data();

                                if (vgabios_hash == vgabios_xml_hash) {
                                        QString cp_to_coreboot_cmd = "cp hardware_data/vgabios_from_mem.bin "
                                                                     + coreboot_dir + "vgabios.bin";
                                        if (system(cp_to_coreboot_cmd.toStdString().c_str()) != 0) {
                                                info_dialog->show_message(ERR_COREBOOT_COPY_VGABIOS);
                                                return;
                                        }
                                        is_config_ok = true;
                                }

                        } else {
                                /* Proceed if configuration doesn't require factory VGABIOS */
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
                QString copy_config_cmd = "cp coreboot_configs/" + working_config + " " +
                                          coreboot_dir + "/.config";
                QString make_coreboot_cmd = "make -C " + coreboot_dir;
                QString cp_coreboot_cmd = "cp " + coreboot_dir + "/build/coreboot.rom coreboot.rom";

                /* Copy appropriate .config file to coreboot directory */
                if (system(copy_config_cmd.toStdString().c_str()) != 0) {
                        ret = ERR_COREBOOT_COPY_CONFIG;
                        return;
                }

                /* Call make in coreboot */
                if (system(make_coreboot_cmd.toStdString().c_str()) != 0)
                        ret = ERR_COREBOOT_MAKE;

                /* Copy coreboot rom from coreboot directory */
                if (system(cp_coreboot_cmd.toStdString().c_str()) != 0)
                            ret = ERR_COREBOOT_COPY_ROM;
        } else {
                ret = ERR_CONFIG_NOT_KNOWN;
        }
        info_dialog->show_message(ret);
}

void MainWindow::on_b_auto_flash_clicked()
{
        Flasher flasher;
        FILE *write_file;
        unsigned char *data = NULL;
        RET_VAL ret = UNKNOWN;

        ui->log_flash->clear();
        if (!(write_file = fopen("coreboot.rom", "rb"))) {
                ret = ERR_COREBOOT_NOROM;
        } else {
                struct stat st;
                stat("coreboot.rom", &st);

                data = new unsigned char[st.st_size];

                if (data) {
                        setWindowTitle("Please wait...");
                        QApplication::processEvents();                  
                        if (fread(data, sizeof(unsigned char), st.st_size, write_file) < (size_t)st.st_size) {
                                ret = ERR_READ_LESS_BYTES;
                        } else {
                                ret = flasher.write_chip(&data, st.st_size);
                        }
                        if (ret == ERR_CHIP_NOT_PROBED)
                                ret = flasher.write_chip(&data, st.st_size);
                        delete [] data;
                } else {
                        ret = ERR_MEM_ALLOC;
                }
                fclose(write_file);
                setWindowTitle("Coreboot Flash Tool");
        }
        info_dialog->show_message(ret);
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

void MainWindow::on_act_supported_list_triggered()
{
        Supported supported_window(this);
        supported_window.setModal(true);
        supported_window.exec();
}

void MainWindow::on_act_preferences_triggered()
{
        Preferences preferences_window(this);
        preferences_window.setModal(true);
        preferences_window.exec();
}

void MainWindow::on_act_about_triggered()
{
        About about_window;
        about_window.setModal(true);
        about_window.exec();
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

QString MainWindow::get_cbfs_rom_path()
{
        return cbfs_rom_path;
}

void MainWindow::on_b_ropt_select_clicked()
{
        cbfs_rom_path = QFileDialog::getOpenFileName(this, tr("Select bootblock"), ".", "All files (*.*)");

        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) */
        int param_count = 3;
        QString params[3];

        active_log_out->clear();
        params[0] = "./flash_tool";
        params[1] = cbfs_rom_path;
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

        QString rom_content_text = active_log_out->toPlainText();
        QStringList rom_info_list = rom_content_text.split(QRegExp("\\s+"));

        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Offset")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Type")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Size")));

        for (int i = 18; i < rom_info_list.size() - 1; i+=4) {
                QList<QStandardItem*> row;
                row.append(new QStandardItem(rom_info_list[i]));
                row.append(new QStandardItem(rom_info_list[i+1]));
                row.append(new QStandardItem(rom_info_list[i+2]));
                row.append(new QStandardItem(rom_info_list[i+3]));
                model->appendRow(row);
        }

        ui->tv_rom_content->setModel(model);
        ui->tv_rom_content->setColumnWidth(0, 165);
        ui->tv_rom_content->setColumnWidth(1, 90);
        ui->tv_rom_content->setColumnWidth(2, 200);
        ui->tv_rom_content->setColumnWidth(3, 100);
}
