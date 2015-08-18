#ifndef CONSTANTS_H
#define CONSTANTS_H

enum RET_VAL {
        SUCCESS = 0,
        ERR_PROG_NOT_INIT,
        ERR_PROBE_FAILED,
        ERR_WRITE_FAILED,
        ERR_CHIP_NOT_PROBED,
        ERR_CHIP_PROBED,
        ERR_CHIP_NOT_READ,
        ERR_CHIP_NOT_VERIFIED,
        ERR_CANT_OPEN_FILE,
        ERR_MEM_ALLOC,
        ERR_DIR_NOT_SEL,
        ERR_BUF_SIZE_DIFF,
        ERR_CMD_LSPCI_NOT_EXEC,
        ERR_CMD_EDID_NOT_EXEC,
        ERR_CMD_DMI_NOT_EXEC,
        ERR_CMD_VGBABIOS_NOT_EXEC,
        ERR_CMD_TAR_PACK_NOT_EXEC,
        ERR_CMD_TAR_UNPACK_NOT_EXEC,
        UNKNOWN
};

#endif // CONSTANTS_H

