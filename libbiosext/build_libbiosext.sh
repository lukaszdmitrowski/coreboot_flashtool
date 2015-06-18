#!/bin/bash
gcc -c -g -fpack-struct -Wall -O0 -o ami.o ami.c
gcc -c -g -fpack-struct -Wall -O0 -o award.o award.c
gcc -c -g -fpack-struct -Wall -O0 -o phoenix.o phoenix.c
gcc -c -g -fpack-struct -Wall -O0 -o bios_extract.o bios_extract.c
gcc -c -g -fpack-struct -Wall -O0 -o compat.o compat.c
gcc -c -g -fpack-struct -Wall -O0 -o lh5_extract.o lh5_extract.c
ar rcs libbiosext.a ami.o award.o phoenix.o bios_extract.o compat.o lh5_extract.o

