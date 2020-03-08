#ifndef DBHANDLER_H
#define DBHANDLER_H
#include <QSqlDatabase>

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
    WorkerName getName(const QModelIndex &index);
    bool addWorker(const WorkerName &name, const QString &phone,
                   const QDateTime &nextDate, const uint16_t &vacancy, const QString &comment);
    bool saveToHistory(const uint16_t &user, const uint16_t &step, const QString &comment);
    Vacancies getVacancies();
    WorkFlow getWorkerWorkflow(const QModelIndex &index);
    History getWorkerHistory(const QModelIndex &index);
    bool updateWorker(const uint16_t &worker, const uint16_t &step, const QString &comment);

private:

    QSqlDatabase &dataBase;
};

#endif // DBHANDLER_H
