#ifndef DATAGATHERER_H
#define DATAGATHERER_H

#include "stdio.h"

extern "C" {
#include "libflashrom.h"
}

class DataGatherer
{
public:
        DataGatherer();
        void probe_chip();
        void save_lspci_output();
        void read_bios_rom();
        void extract_rom();
        void extract_vga_bios();

private:
        fl_flashctx_t *flash_context;

};

#endif // DATAGATHERER_H
