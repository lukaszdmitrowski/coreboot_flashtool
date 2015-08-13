#include "datagatherer.h"
#include "choosechip.h"

#include <QDebug>
#include <QFile>

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

        return ret_val;
}

void DataGatherer::save_bios_rom_factory()
{
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
}

void DataGatherer::save_bios_rom_from_iomem()
{
        system("cat /proc/iomem | grep \'Video ROM\' | (read m; m=${m/ :*}; s=${m/-*}; e=${m/*-}; \
                dd if=/dev/mem of=vgabios.bin bs=1c skip=$[0x$s] count=$[$[0x$e]-$[0x$s]+1]) > hardware_data/vgabios_from_mem.bin");
}

void DataGatherer::extract_rom(QString bios_rom_path)
{
        set_output_directory("hardware_data/factory_bios_components/");
        start_bios_extract(bios_rom_path.toStdString().c_str());
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
        system("cat /sys/class/drm/card0-LVDS-1/edid | edid-decode > hardware_data/edid-decode_output");
}

void DataGatherer::create_hardware_data_archive()
{
        system("tar -cf hardware_data.tar hardware_data");
}

void DataGatherer::unpack_hardware_data_archive(QString filename)
{
        QString untar_command = "tar -xf " + filename + " -C hardware_data/";
        system(untar_command.toStdString().c_str());
}


