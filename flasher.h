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
         * @return SUCCESS - chip successfully probed
         * @return ERR_PROG_NOT_INIT - programmer not initialized
         * @return ERR_CHIP_PROBED - chip has been already probed
         * @return ERR_PROBE_FAILED - probing failed
         * @return UNKNOWN - unknown error
         */
        RET_VAL probe_chip();

        /**
         * @brief Read chip content to array
         *
         * Reads chip content, then allocates memory and fills it with read data
         *
         * @param data_out [out] pointer to read data
         * @param chip_size [out] size of chip in bytes
         * @return SUCCESS - data successfully read
         * @return ERR_PROG_NOT_INIT - programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - chip not probed (no flash context)
         * @return ERR_MEM_ALLOC - memory allocation failed
         * @return UNKNOWN - unknown error
         */
        RET_VAL read_chip(unsigned char **data_out, unsigned long *const chip_size);

        /**
         * @brief Verify chip content with specified data
         *
         * @param buffer [in] pointer to input data
         * @param buffer_size [in] size of pointed data in bytes
         * @return SUCCESS - data successfully read
         * @return ERR_PROG_NOT_INIT - programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - chip not probed (no flash context)
         * @return ERR_CHIP_NOT_VERIFIED - chip contents is not consistent with input data
         * @return ERR_BUF_SIZE_DIFF - size of input data is not consistent with size of a chip
         * @return UNKNOWN - unknown error
         */
        RET_VAL verify_chip(unsigned char **buffer, unsigned long buffer_size);

        /**
         * @brief Erase chip contents
         *
         * @return SUCCESS - data successfully read
         * @return ERR_PROG_NOT_INIT - programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - chip not probed (no flash context)
         */
        RET_VAL erase_chip();

        /**
         * @brief Write input data to chip
         *
         * @param data [in] pointer to input data
         * @param data_size [in] size of pointed data in bytes
         * @return SUCCESS - data successfully read
         * @return ERR_PROG_NOT_INIT - programmer not initialized
         * @return ERR_CHIP_NOT_PROBED - chip not probed (no flash context)
         * @return ERR_WRITE_FAILED - write to chip failed
         * @return ERR_BUF_SIZE_DIFF - size of input data is not consistent with size of a chip
         * @return UNKNOWN - unknown error
         */
        RET_VAL write_chip(unsigned char **data, unsigned long data_size);
        unsigned long get_chip_size();

private:
        /** @brief Flash context
         *
         * Pointer to libflashrom flash context (struct flashctx)
         *
         */
        static fl_flashctx_t *flash_context;
};

#endif // FLASHER_H
