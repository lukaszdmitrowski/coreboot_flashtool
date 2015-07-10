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

#ifndef MULTISORTFILTERMODEL_H
#define MULTISORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class MultiSortFilterModel : public QSortFilterProxyModel
{
        Q_OBJECT

public:
        explicit MultiSortFilterModel(QObject *parent = 0);
        void setFilterKeyColumns(const QList<qint32> &filterColumns);
        void setFilter(qint32 column, const QString &pattern);

protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
        QMap<qint32, QString> columnPatterns;
};

#endif // MULTISORTFILTERMODEL_H

