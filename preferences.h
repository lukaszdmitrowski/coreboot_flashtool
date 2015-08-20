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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
        Q_OBJECT

public:
        explicit Preferences(QWidget *parent = 0);
        ~Preferences();

private slots:
        /**
         * @brief Called when on_b_sel_cor_path is clicked
         * Opens QFileDialog for selecting path to coreboot directory
         */
        void on_b_sel_cor_path_clicked();

        void on_b_sel_conf_path_clicked();

        /**
         * @brief Called when on_b_ok button is clicked
         * Saves coreboot path to preferences.cfg file and application variables
         */
        void on_b_ok_clicked();



private:
        Ui::Preferences *ui;
};

#endif // PREFERENCES_H
