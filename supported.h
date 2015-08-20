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

#ifndef SUPPORTED_H
#define SUPPORTED_H

#include <QDialog>
#include <QStandardItemModel>

#include "multisortfiltermodel.h"

extern "C" {
#include "libflashrom.h"
}

namespace Ui {
class Supported;
}

class Supported : public QDialog
{
        Q_OBJECT

public:
        explicit Supported(QWidget *parent = 0);
        ~Supported();

private slots:
        /**
         * @brief Show supported flash chips
         */
        void show_flash_chips();

        /**
         * @brief Show supported motherboards
         */
        void show_boards();

        /**
         * @brief Show supported chipsets
         */
        void show_chipsets();

        /**
         * @brief Called when cb_sel_hardware combo box index changed
         *
         * Shows supported chips, boards or chipsets according to selected index
         * in cb_sel_hardware combo box
         *
         * @param index - specifies selected value index
         */
        void on_cb_sel_hardware_currentIndexChanged(int index);

        /**
         * @brief Called when cb_sel_vendor combo box index changed
         * Applies vendor filter according to selected value in cb_sel_vendor combo box
         */
        void on_cb_sel_vendor_currentIndexChanged();

        /**
         * @brief Called when value of edit_name edit text changed
         * Applies name filter according to edit_name text
         */
        void on_edit_name_textChanged(const QString &arg1);

        /**
         * @brief Called when sel_custom combo box index changed
         * Applies custom filterr according to selected value in cb_sel_custom combo box
         */
        void on_cb_sel_custom_currentIndexChanged();

private:
        /**
         * @brief Conver fl_test_state to QString
         * @param test_state [in] - test state to be converted
         * @return QString created according to particular fl_test_state
         */
        QString test_state_to_qstring(fl_test_state test_state);
        Ui::Supported *ui;

        /**
         * @brief Pointer to model responsible for storing data of supported hardware
         */
        QStandardItemModel *model;

        /**
         * @brief Pointer to extended model responsible for applying filters
         */
        MultiSortFilterModel *sortFilterModel;

        /**
         * @brief Pointer to flashchip_info structure responsible for storing data about chip
         */
        fl_flashchip_info *flashchip_info;

        /**
         * @brief Pointer to boards_list structure responsible for storing data about board
         */
        fl_board_info *boards_list;

        /**
         * @brief Pointer to chipstes_list structure responsible for storing data about chipstes
         */
        fl_chipset_info *chipsets_list;

        /**
         * @brief List of hardware vendors
         */
        QStringList vendor_list;

        /**
         * @brief List of custom filter options
         */
        QStringList custom_list;
};

#endif // SUPPORTED_H
