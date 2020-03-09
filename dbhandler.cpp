#include "dbhandler.h"

#include <utility>
#include <QModelIndex>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlRecord>


DBHandler::DBHandler(QSqlDatabase &db):
    dataBase{db}
{

}

DBHandler::~DBHandler(){

}

bool DBHandler::addWorker(const WorkerName &name, const QString &phone,
                          const QDateTime &nextDate, const uint16_t &vacancy, const QString &comment){
    QSqlQuery query;
    query.prepare("INSERT INTO workers (id_worker, fname, lname, mphone, next_date, fd_vacancy, fd_state) "
                   "VALUES (?, ?, ?, ?, ?, ?, ?)");
     //AutoIncrement
    query.bindValue(1, name.first);
    query.bindValue(2, name.second);
    query.bindValue(3, phone);
    query.bindValue(4, nextDate);
    query.bindValue(5, vacancy);
    query.bindValue(6, 0); // New records get a start step

    return query.exec() &&
            saveToHistory(query.lastInsertId().toInt(), 0, comment);

}

bool DBHandler::saveToHistory(const uint16_t &worker, const uint16_t &step, const QString &comment){
    QSqlQuery query(dataBase);
    query.prepare("INSERT INTO history (id_history, fid_worker, comment, fid_step, date) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.bindValue(1, worker);
    query.bindValue(2, comment);
    query.bindValue(3, step);
    query.bindValue(4, QDateTime::currentDateTime());
    return query.exec();
}

Vacancies DBHandler::getVacancies(){
    Vacancies vacancies;
    QSqlQuery query(dataBase);
    query.prepare("SELECT * FROM `vacancies`");
    query.exec();

    while (query.next()) {
        vacancies.push_back(std::make_pair(query.value(0).toUInt(), query.value(1).toString()));;
    }
    return vacancies;
}

WorkFlow DBHandler::getWorkerWorkflow(const uint16_t &worker){
    WorkFlow result;
    QSqlQuery query(dataBase);
    query.prepare("SELECT `id_step`, `sname` FROM `workflow` WHERE `pid_step` = "
                        "(SELECT fd_state FROM `workers` WHERE `id_worker` = ?)");
    query.bindValue(0, worker);
    query.exec();
    while (query.next()) {
        result.push_back(std::make_pair(query.record().value(0).toInt(), //step id
                                        query.record().value(1).toString())); //step name
    }

    return result;
}

History DBHandler::getWorkerHistory(const uint16_t &worker){
    QSqlQuery query(dataBase);
    query.prepare("SELECT `workflow`.`sname`, `comment`, `date` FROM `history` "
                       "INNER JOIN `workflow` ON `history`.`fid_step` = `workflow`.`id_step` WHERE fid_worker = ?");
    query.bindValue(0, worker);
    query.exec();

    History userHistory; //StepName, Comment, date
    while (query.next()) {
        userHistory.push_back(
                    std::make_tuple(
                        query.record().value(0).toString(),
                        query.record().value(1).toString(),
                        query.record().value(2).toDateTime()
                        )
                    );
    }
    return userHistory;
}

bool DBHandler::updateWorker(const uint16_t &worker, const uint16_t &step, const QString &comment){
    QSqlQuery query(dataBase);
    query.prepare("UPDATE `workers` SET `fd_state` = ? WHERE (`id_worker` = ?)");
    query.bindValue(0, step); //step id
    query.bindValue(1, worker); //id value of worker
    return query.exec() &&
            saveToHistory(worker, step, comment);

}

bool DBHandler::deleteFromHistory(const uint16_t &worker){
    QSqlQuery query(dataBase);
    query.prepare("DELETE FROM `history` WHERE (`fid_worker` = ?)");
    query.bindValue(0, worker);
    return query.exec();
}

bool DBHandler::deleteWorker(const uint16_t &worker){
    QSqlQuery query(dataBase);
    query.prepare("DELETE FROM `workers` WHERE (`id_worker` = ?)");
    query.bindValue(0, worker);
    return query.exec() &&
            deleteFromHistory(worker);

}

bool DBHandler::addVacancy(const QString &vacName){
    QSqlQuery query(dataBase);
    query.prepare("INSERT INTO `vacancies` (`id_vacancy`, `vname`) VALUES (?, ?)");
    query.bindValue(1, vacName);
    return query.exec();
}

bool DBHandler::deleteVacancies(const std::list<uint16_t> &vacList){
    QVariantList list;
    for (const auto& item : vacList) {
        list.push_back(item);
    }
    QSqlQuery query(dataBase);
    query.prepare("DELETE FROM `vacancies` WHERE (`id_vacancy` = ?)");
    query.addBindValue(list);
    return query.execBatch();
}

bool DBHandler::restartWorkflow(const uint16_t &worker, const uint16_t &vacancy, const QDateTime &nextDate){
    QSqlQuery query(dataBase);
    query.prepare("UPDATE `workers` SET `next_date` = ?, `fd_vacancy` = ?, "
                  "`fd_state` = '0' WHERE (`id_worker` = ?)");
    query.bindValue(0, nextDate);
    query.bindValue(1, vacancy);
    query.bindValue(2, worker);
    return query.exec();
}

uint16_t DBHandler::getWorkerVacancy(const uint16_t &worker){
    QSqlQuery query(dataBase);
    query.prepare("SELECT `fd_vacancy` FROM `workers` WHERE (`id_worker` = ?)");
    query.bindValue(0, worker);
    query.exec();
    query.next();
    return query.record().value(0).toUInt();
}
