#include "flashrom.h"
#include "mainwindow.h"
#include "choosechip.h"

#include <QDebug>

Flashrom::Flashrom()
{
}

int Flashrom::probe_chip()
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (!w->chip_found) {
                        ret_val = fl_flash_probe(&flash_context, NULL);

                        if (ret_val == 0)
                                w->chip_found = true;

                        if (ret_val == 3) {
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
                        }
                } else {
                        qDebug() << "Already probed for a chip - " + w->chip_name;
                }

        } else {
                qDebug() << "Please initialize programmer!";
        }

        return ret_val;
}

int Flashrom::read_chip(unsigned char *data_out, unsigned long chip_size)
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        if (data_out) {
                                fl_image_read(flash_context, data_out, chip_size);
                        } else {
                                qDebug() << "Out of memory!";
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

int Flashrom::verify_chip(void *buffer, unsigned long chip_size)
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        if (fl_image_verify(flash_context, buffer, chip_size) == 0) {
                                ret_val = 0;
                        } else {
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

int Flashrom::write_chip(char *data, unsigned long chip_size)
{
        int ret_val = 1;

        if (w->programmer_initialized) {
                if (w->chip_found) {
                        fl_image_write(flash_context, data, chip_size);
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

