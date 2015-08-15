#ifndef FLASHROM_H
#define FLASHROM_H

#include "stdio.h"

extern "C" {
#include "libflashrom.h"
}

class Flashrom
{
public:
        Flashrom();
        int probe_chip();
        int read_chip(unsigned char *data_out, unsigned long chip_size);
        int verify_chip(void *buffer, unsigned long chip_size);
        int erase_chip();
        int write_chip(char *data, unsigned long chip_size);
        unsigned long get_chip_size();

private:
        fl_flashctx_t *flash_context;
};

#endif // FLASHROM_H
