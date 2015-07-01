#include <QDebug>

#include "multisortfiltermodel.h"


MultiSortFilterModel::MultiSortFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void MultiSortFilterModel::setFilterKeyColumns(const QList<qint32> &filterColumns)
{
        columnPatterns.clear();

        foreach (qint32 column, filterColumns)
                columnPatterns.insert(column, QString());
}

void MultiSortFilterModel::setFilter(qint32 column, const QString &pattern)
{
        if (!columnPatterns.contains(column))
                return;

m_columnPatterns[column] = pattern;
}

bool MultiSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
        if (columnPatterns.isEmpty())
                return true;

        bool ret = false;

        for (QMap<qint32, QString>::const_iterator iter = columnPatterns.constBegin();
             iter != columnPatterns.constEnd();
             ++iter)
        {
                qDebug() << "iter.value: " << iter.value();
                if (iter.value().isEmpty()) {
                        qDebug() << "empty iter.value()";
                        return true;
                }
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                //qDebug() << "index.data: " << index.data().toString();
                ret = index.data().toString().contains(iter.value());

                if(!ret)
                        return ret;
        }

        return ret;
}
