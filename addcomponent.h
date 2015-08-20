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

#ifndef ADDCOMPONENT_H
#define ADDCOMPONENT_H

#include <QDialog>

namespace Ui {
class AddComponent;
}

class AddComponent : public QDialog
{
        Q_OBJECT

public:
        explicit AddComponent(QWidget *parent = 0);
        ~AddComponent();

private slots:
        /**
         * @brief Called when b_sel_component button is clicked
         *
         * Calls QFileDialog to select a path for component
         *
         */
        void on_b_sel_component_clicked();

        /**
         * @brief Called when b_add_component button is clicked
         *
         * Adds component to CBFS image
         *
         */
        void on_b_add_component_clicked();

private:
        /**
         * @brief Fills cb_type combo box with possible component types
         */
        void fill_cb_type();
        Ui::AddComponent *ui;

        /**
         * @brief Path to component
         */
        QString component_path;
};

#endif // ADDCOMPONENT_H
