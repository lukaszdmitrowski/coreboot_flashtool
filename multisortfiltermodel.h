#ifndef MULTISORTFILTERMODEL_H
#define MULTISORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class MultiSortFilterModel : public QSortFilterProxyModel
{
        Q_OBJECT

public:
        explicit MultiSortFilterModel(QObject *parent = 0);
        void setFilterKeyColumns(const QList<qint32> &filterColumns);
        void addFilterFixedString(qint32 column, const QString &pattern);

protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
        QMap<qint32, QString> columnPatterns;
};

#endif // MULTISORTFILTERMODEL_H

