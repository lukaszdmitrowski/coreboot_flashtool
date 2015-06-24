#!/bin/bash
gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o libcbfstool.o libcbfstool.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o partitioned_file.o partitioned_file.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o cbfs-mkstage.o cbfs-mkstage.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o cbfs-mkpayload.o cbfs-mkpayload.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o cbfs-payload-linux.o cbfs-payload-linux.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o fit.o fit.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o fmap.o flashmap/fmap.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o compress.o compress.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o elfheaders.o elfheaders.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o linux_trampoline.o linux_trampoline.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o kv_pair.o flashmap/kv_pair.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o valstr.o flashmap/valstr.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o lzma.o lzma/lzma.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -Wno-sign-compare -Wno-cast-qual -c -o LzmaEnc.o lzma/C/LzmaEnc.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o LzmaDec.o lzma/C/LzmaDec.c

gcc -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Iflashmap -g3 -std=c99 -Werror -Wall -Wextra -Wcast-qual -Wmissing-prototypes -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -c -o LzFind.o lzma/C/LzFind.c


ar rcs libcbfstool.a partitioned_file.o cbfs-mkstage.o cbfs-mkpayload.o cbfs-payload-linux.o fit.o fmap.o compress.o elfheaders.o linux_trampoline.o kv_pair.o valstr.o lzma.o LzmaEnc.o LzmaDec.o LzFind.o libcbfstool.o

