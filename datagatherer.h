#ifndef DATAGATHERER_H
#define DATAGATHERER_H

#include <QString>
#include "stdio.h"

extern "C" {
#include "libflashrom.h"
}

class DataGatherer
{
public:
        DataGatherer();
        int probe_chip();
        void save_lspci_output();
        void save_edid_data();
        void save_dmidecode_output();
        void save_bios_rom_factory();
        void save_bios_rom_from_iomem();
        void extract_rom(QString bios_rom_path);

        QString get_graphic_card_model();
        QString get_display_panel_model();
        QString get_motherboard_model();

        void create_hardware_data_archive();
        void unpack_hardware_data_archive(QString filename);

private:
        fl_flashctx_t *flash_context;

};

#endif // DATAGATHERER_H
