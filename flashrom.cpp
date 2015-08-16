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

#include "flashrom.h"
#include "mainwindow.h"
#include "choosechip.h"

#include <QDebug>

fl_flashctx_t *Flashrom::flash_context = NULL;

Flashrom::Flashrom()
{
}

int Flashrom::init_flashrom()
{

}

int Flashrom::shutdown_flashrom()
{
        fl_flash_release(flash_context);
        fl_shutdown();
}

int Flashrom::probe_chip()
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (!w->chip_found) {
                        ret_val = fl_flash_probe(&flash_context, NULL);

                        if (ret_val == 0) {
                                w->chip_found = true;
                                qDebug() << "Chip found!";
                        } else if (ret_val == 3) {
                                ChooseChip choose_chip_dialog;
                                const char **chip_names = NULL;
                                int chip_count = 0;
                                int i = 0;

                                chip_names = fl_multiple_flash_probe(&chip_count);
                                for (; i < chip_count; ++i) {
                                        choose_chip_dialog.add_chip(chip_names[i]);
                                }

                                choose_chip_dialog.setModal(true);
                                choose_chip_dialog.set_flash_ctx_ptr(&flash_context);
                                choose_chip_dialog.exec();
                                fl_data_free(chip_names);
                                w->chip_found = true;
                                ret_val = 0;
                        } else {
                                qDebug() << "Probing failed!";
                        }
                } else {
                        qDebug() << "Already probed for a chip - " + w->chip_name;
                }

        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

int Flashrom::read_chip(unsigned char **data_out, unsigned long *const chip_size)
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        *chip_size = get_chip_size();
                        *data_out = new unsigned char[*chip_size];
                        memset(*data_out, 0, *chip_size);

                        if (data_out) {
                                fl_image_read(flash_context, *data_out, *chip_size);
                        } else {
                                qDebug() << "Out of memory!";
                                ret_val = 3;
                        }
                } else {
                        probe_chip();
                        ret_val = 2;
                }
        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

int Flashrom::verify_chip(unsigned char **buffer, unsigned long buffer_size)
{
        int ret_val = 1;
        unsigned long chip_size = 0;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        chip_size = get_chip_size();

                        if (buffer_size != chip_size) {
                                qDebug() << "Buffer size different than chip size!";
                                ret_val = 4;
                        } else {
                                if (fl_image_verify(flash_context, *buffer, chip_size) == 0) {
                                        ret_val = 0;
                                } else {
                                        ret_val = 3;
                                }
                        }
                } else {
                        probe_chip();
                        ret_val = 2;
                }
        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

int Flashrom::erase_chip()
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        fl_flash_erase(flash_context);
                } else {
                        probe_chip();
                        ret_val = 2;
                }
        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

int Flashrom::write_chip(unsigned char **data, unsigned long data_size)
{
        int ret_val = 1;
        unsigned long chip_size = 0;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        chip_size = get_chip_size();

                        if (data_size != chip_size) {
                                qDebug() << "Data size different than chip size!";
                                ret_val = 4;
                        } else {
                                if (fl_image_write(flash_context, *data, data_size) == 0) {
                                        ret_val = 0;
                                } else {
                                        ret_val = 3;
                                }
                        }
                } else {
                        probe_chip();
                        ret_val = 2;
                }
        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

unsigned long Flashrom::get_chip_size()
{
        return fl_flash_getsize(flash_context);
}

