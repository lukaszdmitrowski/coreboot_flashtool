#!/bin/bash
gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o libcbfstool.o libcbfstool.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o common.o common.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o xdr.o xdr.c

ar rcs libcbfstool.a xdr.o common.o libcbfstool.o

