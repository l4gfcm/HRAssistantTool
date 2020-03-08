#ifndef DBHANDLER_H
#define DBHANDLER_H
#include <QSqlDatabase>

class QModelIndex;
class QDateTime;
using WorkerName = std::pair<QString, QString>;
using Vacancies = std::vector<std::pair<uint16_t, QString>>;

class DBHandler
{
public:
    explicit DBHandler(QSqlDatabase &db);

    WorkerName getName(const QModelIndex &index);
    bool addWorker(const WorkerName &name, const QString &phone,
                   const QDateTime &nextDate, const uint16_t &vacancyId, const QString &comment);
    bool saveToHistory(const uint16_t &user, const uint16_t &step, const QString &comment);
    Vacancies getVacancies();
private:

    QSqlDatabase &dataBase;
};

#endif // DBHANDLER_H
