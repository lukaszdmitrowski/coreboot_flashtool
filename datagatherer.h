#ifndef DATAGATHERER_H
#define DATAGATHERER_H

#include <QString>
#include "stdio.h"
#include "constants.h"

class DataGatherer
{
public:
        DataGatherer();

        /**
         * @brief Save output of 'lspci -nn' to hardware_data/lspci_output.txt
         *
         * @return SUCCESS - Success
         * @return ERR_CMD_LSPCI_NOT_EXEC - Command execution failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL save_lspci_output();

        /**
         * @brief Save output of 'cat /sys/class/drm/card0-LVDS-1/edid | edid-decode'
         *        command to hardware_data/edid-decode_output.txt
         *
         * @return SUCCESS - Success
         * @return ERR_CMD_EDID_NOT_EXEC - Command execution failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL save_edid_data();

        /**
         * @brief Save output of 'dmidecode -t 2' to hardware_data/dmidecode_output.txt
         *
         * @return SUCCESS - Success
         * @return ERR_CMD_DMI_NOT_EXEC - Command execution failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL save_dmidecode_output();

        /**
         * @brief Read and save factory BIOS image
         *
         * Reads factory BIOS image and saves it to specified path
         *
         * @param [in] save_path - path of saved image
         *
         * @return SUCCESS - Success
         * @return ERR_PROG_NOT_INIT - Programmer not initialized
         * @return UNKNOWN - Unknown error
         */
        RET_VAL save_bios_rom_factory(QString save_path);

        /**
         * @brief Call 'extract_vga_bios.sh' responsible for extracting VGABIOS from memory
         *
         * Calls 'extract_vga_bios.sh' script which extracts VGABIOS from running system
         * memory and saves it in hardware_data/vgabios_from_mem.bin
         *
         * @return SUCCESS - Success
         * @return ERR_CMD_VGBABIOS_NOT_EXEC - Command execution failed
         * @return ERR_CHIP_NOT_PROBED - Chip not probed (no flash context)
         * @return UNKNOWN - Unknown error
         */
        RET_VAL save_bios_rom_from_iomem();

        /**
         * @brief Extract rom components to specified path
         *
         * @param [in] bios_rom_path - path for extracted submodules
         *
         * Extracts rom coomponents to specified path with use of bios_extract tool
         *
         */
        void extract_rom(QString bios_rom_path);

        /**
         * @brief Return graphic card model basing on lspci_output.txt
         * @return Graphic card model
         */
        QString get_graphic_card_model();

        /**
         * @brief Return display panel model basing on edid-decode_output.txt
         * @return Display panel model
         */
        QString get_display_panel_model();

        /**
         * @brief Return motherboard model basing on dmidecode_output.txt
         * @return Motherboard model
         */
        QString get_motherboard_model();

        /**
         * @brief Create tar file from all files related to gathered hardware data
         *
         * Create archive of all files related to gathered hardware data with use
         * of 'tar -cf hardware_data.tar hardware_data' command
         *
         * @return SUCCESS - Tar file successfully created
         * @return ERR_CMD_TAR_PACK_NOT_EXEC - Command execution failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL create_hardware_data_archive();

        /**
         * @brief Unpack tar file from specified path
         *
         * @param [in] filename - path to tar file
         *
         * @return SUCCESS - Tar file successfully extracted
         * @return ERR_CMD_TAR_UNPACK_NOT_EXEC - Command execution failed
         * @return UNKNOWN - Unknown error
         */
        RET_VAL unpack_hardware_data_archive(QString filename);
};

#endif // DATAGATHERER_H
