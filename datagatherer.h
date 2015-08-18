#ifndef DATAGATHERER_H
#define DATAGATHERER_H

#include <QString>
#include "stdio.h"
#include "constants.h"

class DataGatherer
{
public:
        DataGatherer();
        RET_VAL save_lspci_output();
        RET_VAL save_edid_data();
        RET_VAL save_dmidecode_output();
        RET_VAL save_bios_rom_factory(QString save_path);
        RET_VAL save_bios_rom_from_iomem();
        void extract_rom(QString bios_rom_path);

        QString get_graphic_card_model();
        QString get_display_panel_model();
        QString get_motherboard_model();

        RET_VAL create_hardware_data_archive();
        void unpack_hardware_data_archive(QString filename);
};

#endif // DATAGATHERER_H
