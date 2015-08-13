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
        void save_bios_rom_factory();
        void save_bios_rom_from_iomem();
        void extract_rom(QString bios_rom_path);
        void create_hardware_data_archive();
        void unpack_hardware_data_archive(QString filename);

private:
        fl_flashctx_t *flash_context;

};

#endif // DATAGATHERER_H
