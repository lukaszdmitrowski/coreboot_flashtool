#include "datagatherer.h"
#include "choosechip.h"

#include <QDebug>

extern "C" {
#include "libcbfstool.h"
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

void DataGatherer::read_bios_rom()
{
        int chip_size = fl_flash_getsize(flash_context);
        unsigned char *buf = NULL;

        buf = new unsigned char[chip_size];
        if (buf) {
                //fl_image_read(&flash_context, )
        } else
        {
                qDebug() << "Out of memory!";
        }

        delete buf;
}

void DataGatherer::extract_rom()
{

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

