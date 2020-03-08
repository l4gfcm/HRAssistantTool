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

WorkerName DBHandler::getName(const QModelIndex &index){
    return std::make_pair(index.siblingAtColumn(1).data().toString(),
                          index.siblingAtColumn(2).data().toString()
                );
}

bool DBHandler::addWorker(const WorkerName &name, const QString &phone,
                          const QDateTime &nextDate, const uint16_t &vacancyId, const QString &comment){
    QSqlQuery query;
    query.prepare("INSERT INTO workers (id_worker, fname, lname, mphone, next_date, fd_vacancy, fd_state) "
                   "VALUES (?, ?, ?, ?, ?, ?, ?)");
     //AutoIncrement
    query.bindValue(1, name.first);
    query.bindValue(2, name.second);
    query.bindValue(3, phone);
    query.bindValue(4, nextDate);
    query.bindValue(5, vacancyId);
    query.bindValue(6, 0); // New records get a start state

    return query.exec() &&
            saveToHistory(query.lastInsertId().toInt(), 0, comment);

}

bool DBHandler::saveToHistory(const uint16_t &user, const uint16_t &step, const QString &comment){
    QSqlQuery query(dataBase);
    query.prepare("INSERT INTO history (id_history, fid_worker, comment, fid_step, date) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.bindValue(1, user);
    query.bindValue(2, comment);
    query.bindValue(3, step);
    query.bindValue(4, QDateTime::currentDateTime());
    return query.exec();
}

Vacancies DBHandler::getVacancies(){
    Vacancies vacancies;
    QSqlQuery query("SELECT * FROM vacancies");
    for (uint16_t index = 0; query.next(); index++) {
        vacancies.push_back(std::make_pair(query.value(0).toUInt(), query.value(1).toString()));
    }
    return vacancies;
}
