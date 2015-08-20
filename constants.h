#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file constants.h
 * Contains constant variables
 */

/**
 * @brief Possible error codes
 */
enum RET_VAL {
        /**
         * @brief Operation executed successfully
         */
        SUCCESS = 0,

        /**
         * @brief Programmer not initialized
         */
        ERR_PROG_NOT_INIT,

        /**
         * @brief Probing for a chip failed
         */
        ERR_PROBE_FAILED,

        /**
         * @brief Writing to a chip failed
         */
        ERR_WRITE_FAILED,

        /**
         * @brief Not enough amount of bytes has been read
         */
        ERR_READ_LESS_BYTES,

        /**
         * @brief No probed chip (no flash cotext)
         */
        ERR_CHIP_NOT_PROBED,

        /**
         * @brief Chip already probed
         */
        ERR_CHIP_PROBED,

        /**
         * @brief Chip can't be read
         */
        ERR_CHIP_NOT_READ,

        /**
         * @brief Chip not verfied
         */
        ERR_CHIP_NOT_VERIFIED,

        /**
         * @brief Selected file can't be opened
         */
        ERR_FILE_SELECTED,

        /**
         * @brief hardware_data.xml file can't be read
         */
        ERR_FILE_HW_XML,

        /**
         * @brief File can't be written
         */
        ERR_FILE_WRITE,

        /**
         * @brief Config file can't be accessed
         */
        ERR_FILE_CONFIG,

        /**
         * @brief Memory allocation failed
         */
        ERR_MEM_ALLOC,

        /**
         * @brief Directory in QFileDialog not selected
         */
        ERR_DIR_NOT_SEL,

        /**
         * @brief Size of input data differ from chip size
         */
        ERR_BUF_SIZE_DIFF,

        /**
         * @brief Command 'lspci -nn' not executed
         */
        ERR_CMD_LSPCI_NOT_EXEC,

        /**
         * @brief Command 'cat /sys/class/drm/card0-LVDS-1/edid | edid-decode' not executed
         */
        ERR_CMD_EDID_NOT_EXEC,

        /**
         * @brief Command 'lspci -nn' not executed
         */
        ERR_CMD_DMI_NOT_EXEC,

        /**
         * @brief Script 'extract_vga_bios.sh' not executed
         */
        ERR_CMD_VGBABIOS_NOT_EXEC,

        /**
         * @brief Files not packed to tar file
         */
        ERR_CMD_TAR_PACK_NOT_EXEC,

        /**
         * @brief Files not extracted from tar file
         */
        ERR_CMD_TAR_UNPACK_NOT_EXEC,

        /**
         * @brief No working configuration is known
         */
        ERR_CONFIG_NOT_KNOWN,

        /**
         * @brief Coreboot rom can't be copied
         */
        ERR_COREBOOT_COPY_ROM,

        /**
         * @brief VGABIOS can't be copied to coreboot
         */
        ERR_COREBOOT_COPY_VGABIOS,

        /**
         * @brief Configuration can't be copied to coreboot
         */
        ERR_COREBOOT_COPY_CONFIG,

        /**
         * @brief Coreboot make error
         */
        ERR_COREBOOT_MAKE,

        /**
         * @brief Coreboot configuration needs factory BIOS
         */
        ERR_COREBOOT_NEED_FACTORY_BIOS,

        /**
         * @brief No coreboot rom found
         */
        ERR_COREBOOT_NOROM,

        /**
         * @brief No coreboot dir found
         */
        ERR_COREBOOT_WRONG_DIR,

        /**
         * @brief Unknown error
         */
        UNKNOWN
};

#endif // CONSTANTS_H

