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

#ifndef DELETECOMPONENTS_H
#define DELETECOMPONENTS_H

#include <QDialog>

namespace Ui {
class DeleteComponents;
}

class DeleteComponents : public QDialog
{
        Q_OBJECT

public:
        explicit DeleteComponents(QWidget *parent = 0);
        ~DeleteComponents();

private slots:
        void on_b_remove_comp_clicked();

private:
        Ui::DeleteComponents *ui;
};

#endif // DELETECOMPONENTS_H
