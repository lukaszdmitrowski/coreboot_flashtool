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
#include "flashrom.h"
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

        active_log_out = ui->log_auto;
        chip_name = "";
        coreboot_dir = "coreboot/";
        chip_found = false;
        programmer_initialized = false;
        info_dialog = new InfoDialog(this);
        model = new QStandardItemModel();

        if (!QDir("hardware_data").exists())
            QDir().mkdir("hardware_data");
        if (!QDir("bios_dump").exists())
            QDir().mkdir("bios_dump");

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
}

MainWindow::~MainWindow()
{
        Flashrom flashrom;
        flashrom.shutdown_flashrom();
        delete ui;
}

void MainWindow::on_b_probe_clicked()
{
        Flashrom flashrom;
        RET_VAL ret = UNKNOWN;

        setWindowTitle("Please wait...");
        QApplication::processEvents();
        ret = flashrom.probe_chip();
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
        Flashrom flashrom;
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
                                fread(data, sizeof(unsigned char), st.st_size, verify_file);
                                ret = flashrom.verify_chip(&data, st.st_size);

                                if (ret == ERR_CHIP_NOT_PROBED)
                                        ret = flashrom.verify_chip(&data, st.st_size);

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
        Flashrom flashrom;
        RET_VAL ret = UNKNOWN;

        ui->log_flash->clear();
        setWindowTitle("Please wait...");
        QApplication::processEvents();

        ret = flashrom.erase_chip();
        if (ret == ERR_CHIP_NOT_PROBED)
                flashrom.erase_chip();

        setWindowTitle("Coreboot Flash Tool");
        info_dialog->show_message(ret);
}

void MainWindow::on_b_flash_clicked()
{
        Flashrom flashrom;
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
                                        ret = flashrom.write_chip(&data, st.st_size);
                                        if (ret == ERR_CHIP_NOT_PROBED)
                                                ret = flashrom.write_chip(&data, st.st_size);
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

void MainWindow::on_b_auto_get_bios_clicked()
{
        DataGatherer data_gatherer;
        RET_VAL ret = UNKNOWN;

        setWindowTitle("Please wait...");
        QApplication::processEvents();
        ret = data_gatherer.save_bios_rom_factory("hardware_data/factory_bios.bin");
        setWindowTitle("Coreboot Flash Tool");
        info_dialog->show_message(ret);
}

void MainWindow::on_b_auto_get_hw_data_clicked()
{
        DataGatherer data_gatherer;
        RET_VAL ret = UNKNOWN;

        /* Save lspci -nn output*/
        if ((ret = data_gatherer.save_lspci_output()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Save EDID data */
        if ((ret = data_gatherer.save_edid_data()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Save dmidecode output */
        if ((ret = data_gatherer.save_dmidecode_output()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Dump VGABIOS from iomem */
        if ((ret = data_gatherer.save_bios_rom_from_iomem()) != SUCCESS)
                info_dialog->show_message(ret);

        /* Create archive from gathered data */
        if ((ret = data_gatherer.create_hardware_data_archive()) != SUCCESS)
                info_dialog->show_message(ret);
}

void MainWindow::on_b_auto_build_img_clicked()
{
        DataGatherer data_gatherer;

        QDomDocument xmlBOM;
        QFile hardware_info("hardware_info.xml");
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

        qDebug() << "motherboard: " << motherboard;
        qDebug() << "graphic_card: " << graphic_card;
        qDebug() << "display_panel: " << display_panel;

        if (!hardware_info.open(QIODevice::ReadOnly)) {
                ret = ERR_FILE_HW_XML;
                info_dialog->show_message(ret);
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
                bool gpu_ok = (graphic_card == conf_child.firstChild().toText().data());
                conf_child = conf_child.nextSibling();
                bool panel_ok = (display_panel == conf_child.firstChild().toText().data());
                conf_child = conf_child.nextSibling();
                QString need_vgabios = conf_child.firstChild().toText().data();

                qDebug() << board_ok;
                qDebug() << gpu_ok;
                qDebug() << panel_ok;

                if (board_ok && gpu_ok && panel_ok) {
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
                                        ret = ERR_COREBOOT_NEED_FACTORY_BIOS;
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

                //system("rm coreboot/build/coreboot.rom");
                if (system(copy_config_cmd.toStdString().c_str()) != 0) {
                        ret = ERR_COREBOOT_COPY_CONFIG;
                        return;
                }

                system("make -C coreboot");

                QFile coreboot_rom_file("coreboot/build/coreboot.rom");
                if (coreboot_rom_file.exists()) {
                    if (system("cp coreboot/build/coreboot.rom ../") != 0)
                            ret = ERR_COREBOOT_COPY_ROM;
                } else {
                        ret = ERR_COREBOOT_MAKE;
                }
        } else {
                ret = ERR_CONFIG_NOT_KNOWN;
        }
        info_dialog->show_message(ret);
}

void MainWindow::on_b_auto_flash_clicked()
{
        Flashrom flashrom;
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
                        fread(data, sizeof(unsigned char), st.st_size, write_file);
                        ret = flashrom.write_chip(&data, st.st_size);
                        if (ret == ERR_CHIP_NOT_PROBED)
                                ret = flashrom.write_chip(&data, st.st_size);
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
