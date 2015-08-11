#include "datagatherer.h"
#include "choosechip.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>

extern "C" {
#include "libcbfstool.h"
#include "libbiosext.h"
}

DataGatherer::DataGatherer()
{
}

void DataGatherer::probe_chip()
{
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
        }
}

void DataGatherer::save_bios_rom()
{
        QFile file("bios_dump.rom");
        QDataStream out(&file);
        int chip_size = fl_flash_getsize(flash_context);
        unsigned char *buf = NULL;
        //QByteArray qbyte_buf;

        buf = new unsigned char[chip_size];
        if (buf) {
                fl_image_read(flash_context, buf, chip_size);
        } else {
                qDebug() << "Out of memory!";
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "Can't open file!";
        } else {
                //qbyte_buf.fromRawData(reinterpret_cast<char*>(buf), chip_size);
                out << buf;
                file.close();
        }

        delete buf;
}

void DataGatherer::extract_rom(QString bios_rom_path)
{
        start_bios_extract(bios_rom_path.toStdString().c_str());
}

void DataGatherer::save_lspci_output()
{
        char buffer[2048];
        FILE *pipe, *file;

        file = fopen("./lspci_output.txt", "w");
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

void DataGatherer::extract_vga_bios()
{

}

