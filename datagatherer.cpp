#include "datagatherer.h"
#include "choosechip.h"
#include "mainwindow.h"
#include "flashrom.h"
#include "constants.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>

extern "C" {
#include "libcbfstool.h"
#include "libbiosext.h"
}

DataGatherer::DataGatherer()
{
}

RET_VAL DataGatherer::save_lspci_output()
{
        char buffer[2048];
        FILE *pipe, *file;
        RET_VAL ret = UNKNOWN;

        file = fopen("hardware_data/lspci_output.txt", "w");
        if (!file) {
                ret = ERR_CANT_OPEN_FILE;
                qDebug() << "Can't opeddn file!";
        } else {
                if ((pipe = popen("lspci -nn", "r")) != NULL) {
                        while (!feof(pipe)) {
                                if (fgets(buffer, 2048, pipe) != NULL)
                                        fprintf(file, "%s", buffer);
                        }
                        pclose(pipe);
                        ret = SUCCESS;
                }
                fclose(file);
        }

        return ret;
}

void DataGatherer::save_edid_data()
{
        system("cat /sys/class/drm/card0-LVDS-1/edid | edid-decode > hardware_data/edid-decode_output.txt");
}

void DataGatherer::save_dmidecode_output()
{
        system("sudo dmidecode -t 2 > hardware_data/dmidecode_output.txt");
}

RET_VAL DataGatherer::save_bios_rom_factory(QString save_path)
{
        FILE *dump_file;
        Flashrom flashrom;
        unsigned char *buf = NULL;
        unsigned long chip_size = 0;
        unsigned long written_bytes = 0;
        RET_VAL ret = UNKNOWN;

        ret = static_cast<RET_VAL>(flashrom.read_chip(&buf, &chip_size));

        if (ret != ERR_PROG_NOT_INIT) {
                if (ret == ERR_CHIP_NOT_PROBED)
                        ret = static_cast<RET_VAL>(flashrom.read_chip(&buf, &chip_size));

                if (!(dump_file = fopen(save_path.toStdString().c_str(), "wb"))) {
                        ret = ERR_CANT_OPEN_FILE;
                } else {
                        written_bytes = fwrite(buf, 1, chip_size, dump_file);
                        fclose(dump_file);

                        if (written_bytes < chip_size) {
                                ret = ERR_CHIP_NOT_READ;
                        } else {
                                ret = SUCCESS;
                        }
                        delete buf;
                }
        }

        return ret;
}

void DataGatherer::save_bios_rom_from_iomem()
{
        system("./extract_vga_bios.sh");
}

void DataGatherer::extract_rom(QString bios_rom_path)
{
        qDebug() << "extract rom";
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
                                //qDebug() << graphic_card_model;
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
                                //qDebug() << display_panel_model;
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
                                //qDebug() << motherboard_model;
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


