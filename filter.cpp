#include "filter.h"

Filter::Filter(QObject *parent)
    :   QSortFilterProxyModel(parent)
{

}

bool Filter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex fname = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex lname = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex phone = sourceModel()->index(sourceRow, 3, sourceParent);
    QModelIndex vacancy = sourceModel()->index(sourceRow, 5, sourceParent);
    QModelIndex state = sourceModel()->index(sourceRow, 6, sourceParent);

    return (
               sourceModel()->data(fname).toString().contains(filterRegExp())
            || sourceModel()->data(lname).toString().contains(filterRegExp())
            || sourceModel()->data(phone).toString().contains(filterRegExp())
            || sourceModel()->data(vacancy).toString().contains(filterRegExp())
            || sourceModel()->data(state).toString().contains(filterRegExp())
            );
}

void Filter::filterTextChanged(const QString &mask){

    setFilterRegExp(QRegExp(mask, Qt::CaseInsensitive, QRegExp::RegExp));
}
