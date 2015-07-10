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

#include <QApplication>
#include <QTest>

#include "mainwindow.h"
#include "utests.h"

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow;
    w->show();

#if UTEST
    Test_1 test_1;
    QTest::qExec(&test_1, argc, argv);
#endif

    return a.exec();
}
