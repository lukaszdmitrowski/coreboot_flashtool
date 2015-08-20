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

#ifndef FLASHER_H
#define FLASHER_H

#include "stdio.h"
#include "constants.h"

extern "C" {
#include "libflashrom.h"
}

class Flasher
{
public:
        Flasher();

        /**
         * @brief Initialize flashrom
         * @return 0 on success
         */
        int init_flashrom();

        /**
         * @brief Shut down flashrom
         * @return 0 on success
         */
        int shutdown_flashrom();

        /**
         * @brief Probe for all known chips
         *
         * Probes for all known chips and if multiple chips are found outputs
         * them in choose dialog
         *
         * @return SUCCESS - Chip successfully probed
         * @return ERR_PROG_NOT_INIT - Programmer not initialized
         * @return ERR_CHIP_PROBED - Chip has been already probed
         * @return ERR_PROBE_FAILED - Probing failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL probe_chip();

        /**
         * @brief Read chip content to array
         *
         * Reads chip content, then allocates memory and fills it with read data
         *
         * @param data_out [out] Pointer to read data
         * @param chip_size [out] Size of chip in bytes
         *
         * @return SUCCESS - Data successfully read
         * @return ERR_PROG_NOT_INIT - Programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - Chip not probed (no flash context)
         * @return ERR_MEM_ALLOC - Memory allocation failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL read_chip(unsigned char **data_out, unsigned long *const chip_size);

        /**
         * @brief Verify chip content with specified data
         *
         * @param buffer [in] pointer to input data
         * @param buffer_size [in] size of pointed data in bytes
         *
         * @return SUCCESS - Data successfully read
         * @return ERR_PROG_NOT_INIT - Programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - Chip not probed (no flash context)
         * @return ERR_CHIP_NOT_VERIFIED - Chip contents is not consistent with input data
         * @return ERR_BUF_SIZE_DIFF - Size of input data is not consistent with size of a chip
         * @return UNKNOWN - Unknown error
         */
        RET_VAL verify_chip(unsigned char **buffer, unsigned long buffer_size);

        /**
         * @brief Erase chip contents
         *
         * @return SUCCESS - Data successfully read
         * @return ERR_PROG_NOT_INIT - Programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - Chip not probed (no flash context)
         */
        RET_VAL erase_chip();

        /**
         * @brief Write input data to chip
         *
         * @param data [in] Pointer to input data
         * @param data_size [in] Size of pointed data in bytes
         *
         * @return SUCCESS - Data successfully read
         * @return ERR_PROG_NOT_INIT - Programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - Chip not probed (no flash context)
         * @return ERR_WRITE_FAILED - Write to chip failed
         * @return ERR_BUF_SIZE_DIFF - Size of input data is not consistent with size of a chip
         * @return UNKNOWN - Unknown error
         */
        RET_VAL write_chip(unsigned char **data, unsigned long data_size);
        unsigned long get_chip_size();

private:
        /**
         * @brief Flash context
         * Pointer to libflashrom flash context (struct flashctx)
         */
        static fl_flashctx_t *flash_context;
};

#endif // FLASHER_H
