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

void MultiSortFilterModel::addFilterFixedString(qint32 column, const QString &pattern)
{
        if (!columnPatterns.contains(column))
                return;

        columnPatterns[column] = pattern;
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
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                ret = (index.data().toString() == iter.value());

                if(!ret)
                        return ret;
        }

        return ret;
}
