#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QSortFilterProxyModel>

class Filter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    Filter(QObject *parent = nullptr);
public slots:
    void filterTextChanged(const QString &mask);
private:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    using QSortFilterProxyModel::sourceModel;
    using QSortFilterProxyModel::filterRegExp;
    using QSortFilterProxyModel::setFilterRegExp;
};

#endif // FILTER_H
