#!/bin/bash

cat /proc/iomem | grep 'Video ROM' | (read m; m=${m/ :*}; s=${m/-*}; e=${m/*-}; \
dd if=/dev/mem of=hardware_data/vgabios_from_mem.bin bs=1c skip=$[0x$s] count=$[$[0x$e]-$[0x$s]+1])
