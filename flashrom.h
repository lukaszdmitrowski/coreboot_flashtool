/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Lukasz Dmitrowski <lukasz.dmitrowski@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

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
        int init_flashrom();
        int shutdown_flashrom();
        int probe_chip();
        int read_chip(unsigned char *data_out, unsigned long * const chip_size);
        int verify_chip(void *buffer);
        int erase_chip();
        int write_chip(char *data);
        unsigned long get_chip_size();

private:
        static fl_flashctx_t *flash_context;
};

#endif // FLASHROM_H
