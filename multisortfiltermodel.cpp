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

        columnPatterns[column] = pattern;
}

bool MultiSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
        if (columnPatterns.isEmpty())
                return true;

        bool ret = true;

        for (QMap<qint32, QString>::const_iterator iter = columnPatterns.constBegin();
             iter != columnPatterns.constEnd();
             ++iter)
        {
                if (iter.value().isEmpty()) {
                        //Debug() << "empty iter.value()";
                        continue;
                }

                //qDebug() << "iter.value: " << iter.value();
                QRegExp re("\\d*");
                QModelIndex index = sourceModel()->index(sourceRow, iter.key(), sourceParent);
                //qDebug() << "index.data: " << index.data().toString();
                if (re.exactMatch(index.data().toString())) {
                        ret = index.data().toString() == iter.value();
                } else {
                        ret = index.data().toString().contains(iter.value());
                }

                if(!ret)
                        return ret;
        }

        return ret;
}
