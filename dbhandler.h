#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QSqlDatabase>
#include <list>

class QModelIndex;
class QDateTime;
using WorkerName = std::pair<QString, QString>;
using Vacancies = std::vector<std::pair<uint16_t, QString>>;
using WorkFlow = std::vector<std::pair<uint16_t, QString>>;
using History = std::vector<std::tuple<QString, QString, QDateTime>>;

class DBHandler
{
public:
    explicit DBHandler(QSqlDatabase &db);
    virtual ~DBHandler();
    bool addWorker(const WorkerName &name, const QString &phone,
                   const QDateTime &nextDate, const uint16_t &vacancy, const QString &comment);
    Vacancies getVacancies();
    WorkFlow getWorkerWorkflow(const uint16_t &worker);
    History getWorkerHistory(const uint16_t &worker);
    bool updateWorker(const uint16_t &worker, const uint16_t &step, const QString &comment);
    bool deleteWorker(const uint16_t &worker);
    bool addVacancy(const QString &vacName);
    bool deleteVacancies(const std::list<uint16_t> &vacList);
private:
    bool saveToHistory(const uint16_t &worker, const uint16_t &step, const QString &comment);
    bool deleteFromHistory(const uint16_t &worker);
    QSqlDatabase &dataBase;
};

#endif // DBHANDLER_H
