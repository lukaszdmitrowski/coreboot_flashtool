#include "datagatherer.h"
#include "choosechip.h"
#include "mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

extern "C" {
#include "libcbfstool.h"
#include "libbiosext.h"
}

DataGatherer::DataGatherer()
{
}

int DataGatherer::probe_chip()
{
        int ret_val = 1;

        if (w->is_programmer_initialized()) {
                if (!w->is_chip_found()) {
                        ret_val = fl_flash_probe(&flash_context, NULL);
                        if (ret_val == 3) {
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
                                ret_val = 0;
                        }
                } else {
                        qDebug() << "Already probed for a chip - " + w->chip_name;
                }

        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

void DataGatherer::save_lspci_output()
{
        char buffer[2048];
        FILE *pipe, *file;

        file = fopen("hardware_data/lspci_output.txt", "w");
        if (!file) {
                qDebug() << "Can't open file!";
        }
        if ((pipe = popen("lspci -nn", "r")) != NULL) {
                while (!feof(pipe)) {
                    if (fgets(buffer, 2048, pipe) != NULL)
                            fprintf(file, "%s", buffer);
                }
                pclose(pipe);
        }
        fclose(file);
        //system("lspci -nn > lspci_output.txt");
}

void DataGatherer::save_edid_data()
{
        system("cat /sys/class/drm/card0-LVDS-1/edid | edid-decode > hardware_data/edid-decode_output.txt");
}

void DataGatherer::save_dmidecode_output()
{
        system("sudo dmidecode -t 2 > hardware_data/dmidecode_output.txt");
}

void DataGatherer::save_bios_rom_factory()
{
        if (w->is_programmer_initialized()) {
                if (!w->is_chip_found()) {
                        FILE *dump_file;
                        unsigned char *buf = NULL;
                        unsigned long chip_size = fl_flash_getsize(flash_context);
                        unsigned long written_bytes = 0;

                        buf = new unsigned char[chip_size];
                        memset(buf, 0, chip_size);
                        if (buf) {
                                fl_image_read(flash_context, buf, chip_size);
                        } else {
                                qDebug() << "Out of memory!";
                        }

                        if (!(dump_file = fopen("hardware_data/factory_bios.bin", "wb"))) {
                                qDebug() << "Can't open file!";
                        } else {
                                written_bytes = fwrite(buf, 1, chip_size, dump_file);
                                fclose(dump_file);
                        }

                        if (written_bytes < chip_size) {
                            qDebug() << "File not fully written!";
                        }

                        delete buf;
                } else {
                        qDebug() << "Already probed for a chip - " + w->chip_name;
                }

        } else {
                qDebug() << "Please initialize programmer!";
        }
}

void DataGatherer::save_bios_rom_from_iomem()
{
        system("./extract_vga_bios.sh");
}

void DataGatherer::extract_rom(QString bios_rom_path)
{
        start_bios_extract(bios_rom_path.toStdString().c_str());
}

QString DataGatherer::get_graphic_card_model()
{
        QString graphic_card_model;
        QFile lspci_output_file("hardware_data/lspci_output.txt");

        if (!lspci_output_file.open(QIODevice::ReadOnly)) {
                qDebug() << "Error while loading file";
        } else {
                QTextStream in(&lspci_output_file);

                while (!in.atEnd()) {
                        QString line = in.readLine();
                        if (line.contains("VGA")) {
                                graphic_card_model = line.right(line.length() - 42);
                                qDebug() << graphic_card_model;
                        }
                }
        }

        lspci_output_file.close();
        return graphic_card_model;
}

QString DataGatherer::get_display_panel_model()
{
        QString display_panel_model;
        QFile edid_output_file("hardware_data/edid-decode_output.txt");

        if (!edid_output_file.open(QIODevice::ReadOnly)) {
                qDebug() << "Error while loading file";
        } else {
                QTextStream in(&edid_output_file);

                while (!in.atEnd()) {
                        QString line = in.readLine();
                        if (line.contains("ASCII string:")) {
                                display_panel_model = line.right(12);
                                qDebug() << display_panel_model;
                        }
                }
        }

        edid_output_file.close();
        return display_panel_model;
}

QString DataGatherer::get_motherboard_model()
{
        QString motherboard_model;
        QFile dmidecode_output_file("hardware_data/dmidecode_output.txt");

        if (!dmidecode_output_file.open(QIODevice::ReadOnly)) {
                qDebug() << "Error while loading file";
        } else {
                QTextStream in(&dmidecode_output_file);

                while (!in.atEnd()) {
                        QString line = in.readLine();
                        if (line.contains("Product Name:")) {
                                motherboard_model = line.right(line.length() - 15);
                                qDebug() << motherboard_model;
                        }
                }
        }

        dmidecode_output_file.close();
        return motherboard_model;
}



void DataGatherer::create_hardware_data_archive()
{
        system("tar -cf hardware_data.tar hardware_data");
}

void DataGatherer::unpack_hardware_data_archive(QString filename)
{
        QString untar_command = "tar -xf " + filename;
        system(untar_command.toStdString().c_str());
}


