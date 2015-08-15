#ifndef DATAGATHERER_H
#define DATAGATHERER_H

#include <QString>
#include "stdio.h"

class DataGatherer
{
public:
        DataGatherer();
        void save_lspci_output();
        void save_edid_data();
        void save_dmidecode_output();
        void save_bios_rom_factory(QString save_path);
        void save_bios_rom_from_iomem();
        void extract_rom(QString bios_rom_path);

        QString get_graphic_card_model();
        QString get_display_panel_model();
        QString get_motherboard_model();

        void create_hardware_data_archive();
        void unpack_hardware_data_archive(QString filename);
};

#endif // DATAGATHERER_H
