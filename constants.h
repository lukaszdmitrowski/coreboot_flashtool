#ifndef CONSTANTS_H
#define CONSTANTS_H

enum RET_VAL {
        SUCCESS = 0,
        ERR_PROG_NOT_INIT,
        ERR_PROBE_FAILED,
        ERR_CHIP_NOT_PROBED,
        ERR_CHIP_PROBED,
        ERR_CHIP_NOT_READ,
        ERR_CANT_OPEN_FILE,
        ERR_MEM_ALLOC,
        UNKNOWN
};

#endif // CONSTANTS_H

