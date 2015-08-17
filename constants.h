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
        UNKNOWN
};

#endif // CONSTANTS_H

