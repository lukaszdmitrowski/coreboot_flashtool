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
        void save_bios_rom();
        void extract_rom(QString bios_rom_path);
        void extract_vga_bios();

private:
        fl_flashctx_t *flash_context;

};

#endif // DATAGATHERER_H
